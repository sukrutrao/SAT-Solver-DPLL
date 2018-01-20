/*
 * Program to implement a SAT solver using the DPLL algorithm with unit propagation
 * Sukrut Rao
 * CS15BTECH11036
 */

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>

using namespace std;

/*
 * enum for different types of return flags defined
 */
enum Cat
{
    satisfied, // when a satisfying assignment has been found
    unsatisfied, // when no satisfying assignment has been found after exhaustively searching
    normal, // when no satisfying assignment has been found till now, and DPLL() has exited normally
    completed // when the DPLL algorithm has completed execution
};

/*
 * class to represent a boolean formula
 */
class Formula
{
    public:
        // a vector that stores the value assigned to each variable, where
        // -1 - unassigned
        // 0 - true
        // 1 - false
        vector<int> literals;
        vector<int> literal_frequency; // vector to store the number of occurrences of each literal
        
        // vector to store the difference in number of occurrences with
        // positive and negative polarity of each literal
        vector<int> literal_polarity;
        
        // vector to store the clauses
        // for each clauses, if the variable n is of positive polarity, then 2n is stored
        // if the variable n is of negative polarity, then 2n+1 is stored
        // here, n is assumed to be zero indexed
        vector< vector<int> > clauses;
        Formula() {}
        
        // copy constructor for copying a formula - each member is copied over
        Formula(const Formula &f)
        {
            literals = f.literals;
            clauses = f.clauses;
            literal_frequency = f.literal_frequency;
            literal_polarity = f.literal_polarity;
        }
};

/*
 * class to represent the structure and functions of the SAT Solver
 */
class SATSolverDPLL
{
    private:
        Formula formula; // the initial formula given as input
        int literal_count; // the number of variables in the formula
        int clause_count; // the number of clauses in the formula        
        int unit_propagate(Formula &); // performs unit propagation        
        int DPLL(Formula); // performs DPLL recursively
        int apply_transform(Formula &, int); // applies the value of the literal in every clause
        void show_result(Formula &, int); // displays the result
    public:        
        SATSolverDPLL() {}
        void initialize(); // intiializes the values
        void solve(); // calls the solver
};

/*
 * function that accepts the inputs from the user and initializes the attributes in the solver
 */
void SATSolverDPLL::initialize()
{
    char c; // store first character
    string s; // dummy string

    while(true)
    {
        cin>>c;
        if(c=='c') // if comment
        {
            getline(cin,s); // ignore
        }
        else // else, if would be a p
        {
            cin>>s; // this would be cnf
            break;
        }
    }
    cin>>literal_count;
    cin>>clause_count;
    
    // set the vectors to their appropriate sizes and initial values
    formula.literals.clear();
    formula.literals.resize(literal_count,-1);
    formula.clauses.clear();
    formula.clauses.resize(clause_count);
    formula.literal_frequency.clear();
    formula.literal_frequency.resize(literal_count,0);
    formula.literal_polarity.clear();
    formula.literal_polarity.resize(literal_count,0);
    
    int literal; // store the incoming literal value
    // iterate over the clauses
    for(int i = 0; i < clause_count; i++)
    {
        while(true) // while the ith clause gets more literals
        {
            cin>>literal;     
            if(literal > 0) // if the variable has positive polarity
            {
                formula.clauses[i].push_back(2*(literal-1)); // store it in the form 2n
                // increment frequency and polarity of the literal
                formula.literal_frequency[literal-1]++; 
                formula.literal_polarity[literal-1]++;
            }
            else if(literal < 0) // if the variable has negative polarity
            {
                formula.clauses[i].push_back(2*((-1)*literal-1)+1); // store it in the form 2n+1
                // increment frequency and decrement polarity of the literal
                formula.literal_frequency[-1-literal]++;
                formula.literal_polarity[-1-literal]--;
            }
            else
            {
                break; // read 0, so move to next clause
            }    
        }       
    }
}

/*
 * function to perform unit resolution in a given formula
 * arguments: f - the formula to perform unit resolution on
 * return value: int - the status of the solver after unit resolution, a member of the Cat enum
 *               Cat::satisfied - the formula has been satisfied
 *               Cat::unsatisfied - the formula can no longer be satisfied
 *               Cat::normal - normal exit
 */
int SATSolverDPLL::unit_propagate(Formula &f)
{
    bool unit_clause_found = false; // stores whether the current iteration found a unit clause
    if(f.clauses.size() == 0) // if the formula contains no clauses
    {
        return Cat::satisfied; // it is vacuously satisfied
    }
    do
    {
        unit_clause_found = false;
        // iterate over the clauses in f
        for(int i = 0; i < f.clauses.size(); i++)
        { 
            if(f.clauses[i].size() == 1) // if the size of a clause is 1, it is a unit clause
            {
                unit_clause_found = true;
                f.literals[f.clauses[i][0]/2] = f.clauses[i][0]%2; // 0 - if true, 1 - if false, set the literal
                f.literal_frequency[f.clauses[i][0]/2] = -1; // once assigned, reset the frequency to mark it closed
                int result = apply_transform(f, f.clauses[i][0]/2); // apply this change through f
                // if this caused the formula to be either satisfied or unsatisfied, return the result flag
                if(result == Cat::satisfied || result == Cat::unsatisfied)
                {
                    return result;
                }
                break; // exit the loop to check for another unit clause from the start
            }
            else if(f.clauses[i].size() == 0) // if a given clause is empty
            {
                return Cat::unsatisfied; // the formula is unsatisfiable in this branch
            }
        }
    }while(unit_clause_found);

    return Cat::normal; // if reached here, the unit resolution ended normally
}

/*
 * applies a value of a literal to all clauses in a given formula
 * arguments: f - the formula to apply on
 *            literal_to_apply - the literal which has just been set
 * return value: int - the return status flag, a member of the Cat enum
 *               Cat::satisfied - the formula has been satisfied
 *               Cat::unsatisfied - the formula can no longer be satisfied
 *               Cat::normal - normal exit
 */
int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply)
{
    int value_to_apply = f.literals[literal_to_apply]; // the value to apply, 0 - if true, 1 - if false
    // iterate over the clauses in f
    for(int i = 0; i < f.clauses.size(); i++)
    {
        // iterate over the variables in the clause
        for(int j = 0; j < f.clauses[i].size(); j++)
        {
            // if this is true, then the literal appears with the same polarity as it is being applied
            // that is, if assigned true, it appears positive
            // if assigned false, it appears negative, in this clause
            // hence, the clause has now become true
            if((2*literal_to_apply+value_to_apply) == f.clauses[i][j])
            {
                f.clauses.erase(f.clauses.begin()+i); // remove the clause from the list
                i--; // reset iterator
                if(f.clauses.size() == 0) // if all clauses have been removed, the formula is satisfied
                {
                    return Cat::satisfied;
                }
                break; // move to the next clause
            }
            else if(f.clauses[i][j]/2 == literal_to_apply) // the literal appears with opposite polarity
            {
                f.clauses[i].erase(f.clauses[i].begin()+j); // remove the literal from the clause, as it is false in it
                j--; // reset the iterator
                if(f.clauses[i].size() == 0) // if the clause is empty, the formula is unsatisfiable currently
                {
                    return Cat::unsatisfied;
                }
                break; // move to the next clause
            }
        }
    }
    // if reached here, the function is exiting normally
    return Cat::normal;
}

/*
 * function to perform the recursive DPLL on a given formula
 * argument: f - the formula to perform DPLL on
 * return value: int - the return status flag, a member of the Cat enum
 *               Cat::normal - exited normally
 *               Cat::completed - result has been found, exit recursion all the way
 */
int SATSolverDPLL::DPLL(Formula f)
{
    int result = unit_propagate(f); // perform unit propagation on the formula
    if(result == Cat::satisfied)  // if formula satisfied, show result and return
    {
        show_result(f,result);
        return Cat::completed;
    }
    else if(result == Cat::unsatisfied)  // if formula not satisfied in this branch, return normally
    {
        return Cat::normal;
    }
    // find the variable with maximum frequency in f, which will be the next to be assigned a value 
    // already assigned variables have this field reset to -1 in order to ignore them
    int i = distance(f.literal_frequency.begin(),max_element(f.literal_frequency.begin(),f.literal_frequency.end()));
    // need to apply twice, once true, the other false
    for(int j = 0; j < 2; j++)
    {
        Formula new_f = f; // copy the formula before recursing
        if(new_f.literal_polarity[i] > 0) // if the number of literals with positive polarity are greater
        {
            new_f.literals[i] = j; // assign positive first
        }            
        else // if not
        {
            new_f.literals[i] = (j+1)%2; // assign negative first
        }            
        new_f.literal_frequency[i] = -1; // reset the frequency to -1 to ignore in the future
        int transform_result = apply_transform(new_f,i); // apply the change to all the clauses
        if(transform_result == Cat::satisfied) // if formula satisfied, show result and return
        { 
            show_result(new_f, transform_result);
            return Cat::completed;
        }
        else if(transform_result == Cat::unsatisfied) // if formula not satisfied in this branch, return normally
        {
            continue;
        }
        int dpll_result = DPLL(new_f); // recursively call DPLL on the new formula
        if(dpll_result == Cat::completed) // propagate the result, if completed
        {
            return dpll_result;
        }
    }
    // if the control reaches here, the function has returned normally
    return Cat::normal;
}

/*
 * function to display the result of the solver
 * arguments: f - the formula when it was satisfied or shown to be unsatisfiable
 *            result - the result flag, a member of the Cat enum
 */
void SATSolverDPLL::show_result(Formula &f, int result)
{
    if(result == Cat::satisfied) // if the formula is satisfiable
    {
        cout<<"SAT"<<endl;
        for(int i = 0; i < f.literals.size(); i++)
        {
            if(i != 0)
            {
                cout<<" ";
            }
            if(f.literals[i] != -1)
            {
                cout<<pow(-1,f.literals[i])*(i+1);
            }
            else // for literals which can take either value, arbitrarily assign them to be true
            {
                cout<<(i+1);
            }
        }
        cout<<" 0";
    }
    else // if the formula is unsatisfiable
    {
        cout<<"UNSAT";
    }
}

/*
 * function to call the solver
 */
void SATSolverDPLL::solve()
{
    int result = DPLL(formula); // final result of DPLL on the original formula
    // if normal return till the end, then the formula could not be satisfied in
    // any branch, so it is unsatisfiable
    if(result == Cat::normal)
    {
        show_result(formula,Cat::unsatisfied); // the argument formula is a dummy here, the result is UNSAT
    }
}

int main()
{
    SATSolverDPLL solver; // create the solver
    solver.initialize(); // initialize
    solver.solve(); // solve
    return 0;
}
