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
//    cl_no_assignment, // literal not present, so assignment not meaningful
//    cl_unassigned, // literal in clause not assigned
//    cl_true, // literal assigned true in clause
//    cl_false // literal assigned false in clause
} 

// No need to store assignments in clauses
// Need to check the order in which literals were forced - how about keeping a vector
// that tracks the literal numbers in their forced order, and a counter that keeps the value of
// the last free choice
// Assigned status should be associated with literals, not literals within clauses

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
  //      vector<int> assignments;
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
        vector<int> choice_order;
        int clause_count;
        int true_clause_count;
        int literal_count;
        int last_free_choice;
        SATSolverDPLL();
        void initialize();
        void solve();
        void unit_propagate();
};

void SATSolverDPLL::initialize()
{
    char c;
    string s;
    last_free_choice = -1;
    choice_order.empty();
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

int SATSolverDPLL::unit_propagate()
{
    vector<int>::iterator it;
    int position;
    for(int i = 0; i < clause_count; i++)
    {
        if(clauses[i].literals_left == 1)
        {
            for(int j = 0; j < clauses[i].literals.size(); j++)
            {
                if(clauses[i].literals[j] == Cat::cl_present && literals[j] == Cat::unassigned)
                {
                    if(clauses[i].polarity[j] == Cat::cl_positive)
                    {
                        literals[j] = Cat::forced_true;
                    }
                    else
                    {
                        literals[j] = Cat::forced_false;
                    }
                    choice_order.push_back(j);
                }
                else if(clauses[i].literals[j] == Cat::cl_present )
            }  
        }
    }
}

void SATSolverDPLL::solve()
{
    
}
