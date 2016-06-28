#include "sigprob.h"

using namespace std;

void calc_sigprob()
{
    int* pattern;
    int i, j;
    int inp_val;
    
    pattern = new int[circuit.inpnum+1];
    
    gmap::iterator it;
    
    if(circuit.inpnum < 16)
    {
        for (i = 0; i <= ITR_STEP; i++) 
        {
            gen_pat(pattern);
            load_inputs(pattern);
            prob_sim();
        }
    }
    else
    {
        inp_val = pow(2, MAX_INPT);
        for(i = 0; i < MAX_INPT; i++)
        {
            gen_itrpat(pattern, i);
            cout<<"Pattern: ";
            for(j = circuit.inpnum; j >= 0; j--)
                cout<<j;
            cout<<endl;
            load_inputs(pattern);
            prob_sim();
        }
    }
    
    for(it = graph.begin(); it != graph.end(); it++)
    {
        graph[it->first].prob /= ITR_STEP;
    }
    
    delete[] pattern;
}

void gen_pat(int* pattern)
{
    int i;

    for (i = 0; i != circuit.inpnum; i++)
    {
        pattern[i] = rand() % 2;
    }
}

void gen_itrpat(int* pattern, int val)
{
    int i;
    double exp;
    
    for(i = circuit.inpnum; i >= 0; i--)
    {
        exp = pow(2, i);
        if(exp <= val)
        {
            pattern[i] = 1;
            val -= exp;
        }
        else
            pattern[i] = 0;
    }
}

void load_inputs(int* pattern)
{
    int count = 0;
    list<int>::iterator lit;
   
    for(lit = circuit.inputs.begin(); lit != circuit.inputs.end(); lit++)
    {
        graph[*lit].val = pattern[count];
        count++;
    }
}

void prob_sim()
{
    int tempval, tempnode;
    gmap::iterator cit;
    
    for(cit = graph.begin(); cit != graph.end(); cit++)
    {
        switch(graph[cit->first].type)
        {
            case INPUT:
                break;
            case AND:
            {
                graph[cit->first].val = eval_AND(cit);
                break;
            }
            case NAND:
            {
                tempval = eval_AND(cit);
                graph[cit->first].val = eval_NOT(tempval);
                break;
            }
            case OR:
            {
                graph[cit->first].val = eval_OR(cit);
                break;
            }
            case NOR:
            {
                tempval = eval_OR(cit);
                graph[cit->first].val = eval_NOT(tempval);               
                break;
            } 
            case BUF:
            {
                tempnode = graph[cit->first].fanin.front();
                graph[cit->first].val = graph[tempnode].val;
                break;
            }
            case NOT:
            {
                tempnode = graph[cit->first].fanin.front();
                graph[cit->first].val = eval_NOT(graph[tempnode].val);
                break;
            }
        }
        if(graph[cit->first].val == 1)
            graph[cit->first].prob++;
    }
}

int eval_NOT(int tempval)
{
    switch(tempval)
    {
        case 0:
            return 1;
            break;
        case 1:
            return 0;
            break;
        case 2:
            return 3;
            break;
        case 3:
            return 2;
            break;
        default:
            break;
    }
}

int eval_AND(gmap::iterator cit) 
{
    int tval1, tval2;
    int tnode1, tnode2;

    list<int>::iterator lit;

    lit = graph[cit->first].fanin.begin();
    tnode1 = *lit;
    tval1 = graph[tnode1].val;

    while (lit != graph[cit->first].fanin.end()) 
    {
        tnode2 = *lit;
        tval2 = graph[tnode2].val;
        tval1 = AND_VAL[tval1][tval2];
        lit++;
    }
    return tval1;
}

int eval_OR(gmap::iterator cit)
{
    int tval1, tval2;
    int tnode1, tnode2;

    list<int>::iterator lit;

    lit = graph[cit->first].fanin.begin();
    tnode1 = *lit;
    tval1 = graph[tnode1].val;
    while (lit != graph[cit->first].fanin.end()) 
    {
        tnode2 = *lit;
        tval2 = graph[tnode2].val;
        tval1 = OR_VAL[tval1][tval2];
        lit++;
    }
    return tval1;
}
