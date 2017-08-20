#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

enum Cat
{
    // literals 
    unassigned
}

class Literal
{
    public:
        int assigned_value;
        bool polarity;
        Literal()
        {
            assigned_value = Cat::unassigned;
            polarity = 
        }
};

class Clause
{
    public:
        vector<Literal> literal_list;
        bool is_true;
        int is_true_because;
        int literals_left;
};

class Formula
{
    public:
        vector<Clause> clauses;
        vector<Literal> literals;
        int literal_count;
        int clause_count;
    
};

class SATSolverDPLL
{
    public:
        Formula formula;
        SATSolverDPLL() {}
        void initialize();
        void solve(Formula);
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
    cin>>formula.literal_count;
    cin>>formula.clause_count;
    
    
    formula.literals.empty();
    formula.literals.resize(literal_count);
    formula.clauses.empty();
    formula.clauses.resize(clause_count);
    
    int literal;
    for(int i = 0; i < clause_count; i++)
    {
        clauses[i].literals.resize(literal_count,Cat::cl_not_present);
        clauses[i].polarity.resize(literal_count,Cat::cl_no_polarity);
        while(true)
        {
            cin>>literal;     
            if(literal > 0)
            {
                clauses[i].literals[literal-1] = Cat::cl_present;
                clauses[i].polarity[literal-1] = Cat::cl_positive;
                clauses[i].literals_left++;
            }
            else if(literal < 0)
            {
                clauses[i].literals[literal-1] = Cat::cl_present;
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
}
