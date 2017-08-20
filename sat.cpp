#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>

using namespace std;

enum Cat
{
    satisfied,
    unsatisfied,
    normal,
    completed
};

class Formula
{
    public:
        vector<int> literals;
        vector<int> literal_frequency;
        vector< vector<int> > clauses;
        int last_free_choice;
        Formula()
        {
            last_free_choice = -1;
        }
        Formula(const Formula &f)
        {
            literals = f.literals;
            clauses = f.clauses;
            literal_frequency = f.literal_frequency;
            last_free_choice = f.last_free_choice;
        }
};

class SATSolverDPLL
{
    public:
        Formula formula;
        int literal_count;
        int clause_count;
        SATSolverDPLL() {}
        void initialize();
        int unit_propagate(Formula &);
        void solve();
        int DPLL(Formula);
        int apply_transform(Formula &, int);
        void show_result(Formula &, int);
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
    
    formula.literals.clear();
    formula.literals.resize(literal_count,-1);
    formula.clauses.clear();
    formula.clauses.resize(clause_count);
    formula.literal_frequency.clear();
    formula.literal_frequency.resize(literal_count,0);
    int literal;
    int count = 0;
    for(int i = 0; i < clause_count; i++)
    {
        
        while(true)
        {
            cin>>literal;     
            if(literal > 0)
            {
                formula.clauses[i].push_back(2*(literal-1));
                formula.literal_frequency[literal-1]++;
            }
            else if(literal < 0)
            {
                formula.clauses[i].push_back(2*((-1)*literal-1)+1);
                formula.literal_frequency[-1-literal]++;
            }
            else
            {
     //           count = formula.clauses[i].size();
     //           formula.clauses[i].push_back(0); // 1 - true, 2 - false
     //           formula.clauses[i].push_back(0);
     //           formula.clauses[i].push_back(count);
     //           sort(formula.clauses[i].begin(),formula.clauses[i].end()); ////
                break;
            }    
        }        
    }
 /*   cout<<"Clauses : "<<endl;
    for(int i = 0; i < formula.clauses.size(); i++)
    {
        cout<<"i: "<<i<<endl;
        for(int j = 0; j < formula.clauses[i].size(); j++)
        {
            cout<<"j: "<<j<<" ";
            cout<<formula.clauses[i][j]<<" ";
        }
        cout<<endl;
    }*/
}

// do we change count to 0 if clauses becomes true?

int SATSolverDPLL::unit_propagate(Formula &f)
{
    bool unit_clause_found = false;
    if(f.clauses.size() == 0)
    {
        return satisfied;
    }
    do
    {
        unit_clause_found = false;
        for(int i = 0; i < f.clauses.size(); i++) // any clause that exists is neither true nor false
        { 
            if(f.clauses[i].size() == 1)
            {
                unit_clause_found = true;
                f.literals[f.clauses[i][0]/2] = f.clauses[i][0]%2; // 0 - if true, 1 - if false
                f.literal_frequency[f.clauses[i][0]/2] = -1;
                int result = apply_transform(f, f.clauses[i][0]/2);
                if(result == satisfied || result == unsatisfied)
                {
                    return result;
                }
                break;
            }
            else if(f.clauses[i].size() == 0)
            {
            //    cout<<"Empty clause found in UP"<<endl;
                return unsatisfied;
            }
        }
    }while(unit_clause_found);
//    cout<<"UP returning normally"<<endl;
    return normal;
}

// sort the clauses?
// empty clause?

int SATSolverDPLL::apply_transform(Formula &f, int literal_to_apply)
{
    int value_to_apply = f.literals[literal_to_apply];
//    cout<<"VTA : "<<value_to_apply;
//    cout<<"LTA : "<<literal_to_apply;
 /*   if(f.clauses.size() == 0)
    {
        return satisfied;
    }*/
    for(int i = 0; i < f.clauses.size(); i++)
    {
     /*   if(f.clauses[i].size() == 0)
        {
            return unsatisfied;
        }*/
        for(int j = 0; j < f.clauses[i].size(); j++)
        {
            if((2*literal_to_apply+value_to_apply) == f.clauses[i][j])
            {
        //        cout<<"Yes, i : "<<i<<" j : "<<j<<endl;
                f.clauses.erase(f.clauses.begin()+i);
                i--;
                if(f.clauses.size() == 0)
                {
                    return satisfied;
                }
                break;
            }
            else if(f.clauses[i][j]/2 == literal_to_apply)
            {
                f.clauses[i].erase(f.clauses[i].begin()+j);
                j--;
                if(f.clauses[i].size() == 0)
                {
        //            cout<<"Empty clause created in AT"<<endl;   
        //            cout<<"i: "<<i<<" j: "<<j+1<<endl;
                    return unsatisfied;
                }
                break; ////
            }
        }
    }
 /*   cout<<"At end of transform"<<endl;
    cout<<"Clauses : "<<endl;
    for(int i = 0; i < f.clauses.size(); i++)
    {
        for(int j = 0; j < f.clauses[i].size(); j++)
        {
            cout<<f.clauses[i][j]<<" ";
        }
        cout<<endl;
    }*/
    return normal;
}

int SATSolverDPLL::DPLL(Formula f)
{
/*    cout<<"In DPLL"<<endl;
    cout<<"Clauses : "<<endl;
    for(int i = 0; i < f.clauses.size(); i++)
    {
        cout<<"i: "<<i<<endl;
        for(int j = 0; j < f.clauses[i].size(); j++)
        {
            cout<<"j: "<<j<<" ";
            cout<<f.clauses[i][j]<<" ";
        }
        cout<<endl;
    }*/
    int result = unit_propagate(f);
    if(result == satisfied)
    {
        show_result(f,result);
        return Cat::completed;
    }
    else if(result == unsatisfied)
    {
        return Cat::normal;
    }
 //   for(int i = f.last_free_choice + 1; i < f.literals.size(); i++) ////
 //   {
     //   if(f.literals[i] == -1)
    //    { 
            int i = distance(f.literal_frequency.begin(),max_element(f.literal_frequency.begin(),f.literal_frequency.end()));
            int max_val = *max_element(f.literal_frequency.begin(),f.literal_frequency.end())   ;
      /*      cout<<"I: "<<i<<" max : "<<max_val<<endl;
            for(int l = 0; l < f.literal_frequency.size(); l++)
            {
                cout<<f.literal_frequency[l]<<" ";
            }
            cout<<endl;*/
            f.last_free_choice = i;
            for(int j = 0; j < 2; j++)
            {
                Formula new_f = f;
                new_f.literals[i] = j;
                new_f.literal_frequency[i] = -1;
                int transform_result = apply_transform(new_f,i);
                if(transform_result == satisfied)
                { 
                    show_result(new_f, transform_result);
                    return Cat::completed;
                }
                else if(transform_result == unsatisfied)
                {
                    return Cat::normal;
                }
                int dpll_result = DPLL(new_f);
                if(dpll_result == Cat::completed)
                {
                    return dpll_result;
                }
            }
    //        break;           
 //       }
 //   }
    return Cat::normal;
}

void SATSolverDPLL::show_result(Formula &f, int result)
{
    if(result == satisfied)
    {
        cout<<"SAT"<<endl;
        for(int i = 0; i < f.literals.size(); i++)
        {
            if(f.literals[i] != -1)
            {
                cout<<pow(-1,f.literals[i])*(i+1)<<" ";
            }
            else
            {
                cout<<(i+1)<<" ";
            }
        }
        cout<<endl;
    }
    else
    {
        cout<<"UNSAT"<<endl;
    }
}

void SATSolverDPLL::solve()
{
    int result = DPLL(formula);
    if(result == Cat::normal)
    {
        show_result(formula,Cat::unsatisfied);
    }
}

int main()
{
    SATSolverDPLL solver;
    solver.initialize();
    solver.solve();
    return 0;
}

// literal frequency instead of lexicographic
// count polarity of each literal
