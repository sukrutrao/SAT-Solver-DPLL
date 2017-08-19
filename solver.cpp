#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum Cat
{
    // literals
    unassigned, // literal not assigned
    free_true, // literal freely assigned true
    free_false, // literal freely assigned false
    forced_true, // literal forcibly assigned true
    forced_false, // literal forcibly assigned false
    // Clause::literals
    cl_not_present, // literal not present in clause
    cl_present // literal present in clause
    // polarity
    cl_no_polarity // literal not present, so polarity not meaningful
    cl_positive, // literal with positive polarity
    cl_negative, // literal with negative polarity
    // assignments
    cl_no_assignment, // literal not present, so assignment not meaningful
    cl_unassigned, // literal in clause not assigned
    cl_true, // literal assigned true in clause
    cl_false // literal assigned false in clause
} 

// No need to store assignments in clauses
// Need to check the order in which literals were forced - how about keeping a vector
// that tracks the literal numbers in their forced order, and a counter that keeps the value of
// the last free choice

enum Result
{
    success,
    failure
}

using namespace std;

class Clause  
{
    public:
        vector<int> literals;
        vector<int> assignments;
        vector<int> polarity;
        int is_true;
        int is_true_because;
        int literals_left;   
        Clause()
        {
            literals.empty();
            assignments.empty();
            polarity.empty();
            is_true = 0;
            is_true_because = -1;
            literals_left = 0;
        } 
};

class SATSolverDPLL 
{
    public:
        vector<int> literals;
        vector<Clause> clauses;
        int clause_count;
        int true_clause_count;
        int literal_count;
        SATSolverDPLL() {}
        void initialize();
        void solve();
        void unit_propagate();
};

void SATSolverDPLL::initialize()
{
    char c;
    string s;
    while(true)
    {
        cin>>c;
        if(c=='c')
        {
            getline(cin,s);
        }
        else
        {
            cin>>s;
            break;
        }
    }
    cin>>literal_count;
    cin>>clause_count;
    
    literals.empty();
    literals.resize(literal_count,Cat::unassigned);
    clauses.empty();
    clauses.resize(clause_count);
    
    int literal;
    for(int i = 0; i < clause_count; i++)
    {
        clauses[i].literals.resize(literal_count,Cat::cl_not_present);
        clauses[i].assignments.resize(literal_count,Cat::cl_no_assignment);
        clauses[i].polarity.resize(literal_count,Cat::cl_no_polarity);
        while(true)
        {
            cin>>literal;     
            if(literal > 0)
            {
                clauses[i].literals[literal-1] = Cat::cl_present;
                clauses[i].assignments[literal-1] = Cat::cl_unassigned;
                clauses[i].polarity[literal-1] = Cat::cl_positive;
                clauses[i].literals_left++;
            }
            else if(literal < 0)
            {
                clauses[i].literals[literal-1] = Cat::cl_present;
                clauses[i].assignments[literal-1] = Cat::cl_unassigned;
                clauses[i].polarity[literal-1] = Cat::cl_negative;
                clauses[i].literals_left++;
            }
            else
            {
                break;
            }    
        }        
    }
}

int SATSolverDPLL::unit_propagate()
{
    vector<int>::iterator it;
    int position;
    for(int i = 0; i < clause_count; i++)
    {
        if(clauses[i].literals_left == 1)
        {
            position = distance(clauses[i].assignments.begin(), find(clauses[i].assignments.begin(),clauses[i].assignments.end(),Cat::cl_unassigned));
            if(clauses[i].polarity[position] = Cat::cl_positive)
            {
                if(literals[position] != Cat::forced_false)
                {
                    literals[position] = Cat::forced_true;
                }
            }
        }
    }
}

void SATSolverDPLL::solve()
{
    
}
