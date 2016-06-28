#include "transsim.h"
//#include "gates.h"

using namespace std;

void itr_sim::sim()
{
    int i, max_itr, flag;
    int* pattern = new int[circuit.inpnum+1];
    
    gmap::iterator g_it;
    list<int>::iterator l_it;
    map<int, double>::iterator m_it;
    
    // Determine Number of Patterns to Apply
    if(circuit.inpnum < MAX_INPT)
    {
        max_itr = pow(2, circuit.inpnum);
        flag = 0;
    }
    else
    {
        max_itr = ITR_STEP;
        flag = 1;
    }
    
    // Begin Simulation
    for(i = 0; i != max_itr; i++)
    {
        this->event_n = 0;
        this->id_n = 0;
        
        if(flag == 0)
        {
            // Function definition located in sigprob.h
            gen_itrpat(pattern, i);
        }
        else if(flag == 1)
        {
            // Function definition located in sigprob.h
            gen_pat(pattern);
        }
        
        /*cout<<"Pattern: ";
        for(int g = circuit.inpnum; g >= 0; g--)
        {
            cout<<pattern[g];
        }
        cout<<endl;*/
        
        // Apply Input Pattern
        apply_inpt(pattern);
    
        for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
        {
            if(graph[g_it->first].type != INPUT)
            {
                eval_gval(g_it->first);
                gen_p(g_it->first);
                proc_pulse(g_it->first);
                
                // Process Convergence Cases
                //conv_check(g_it->first);
                
                // Store the result in r_map
                if(out_find(g_it->first) == true)
                {
                    add_result(g_it->first);
                }
            }
            graph[g_it->first].prob += graph[g_it->first].val;
        }
        
        for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
        {
            if(graph[g_it->first].type != INPUT)
            {
                graph[g_it->first].p_list.clear();
            }
        }
    }
        
    for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
    {
        graph[g_it->first].prob = graph[g_it->first].prob / max_itr;
    }
    for(l_it = circuit.outputs.begin(); l_it != circuit.outputs.end(); ++l_it)
    {
        for(m_it = graph[*l_it].r_map.begin(); m_it != graph[*l_it].r_map.end(); ++m_it)
            graph[*l_it].r_map[m_it->first] = graph[*l_it].r_map[m_it->first] / max_itr;
    }
    
    delete[] pattern;
}

void itr_sim::apply_inpt(int* pattern)
{
    int pat_it = 0;
    gmap::iterator g_it;
    
    for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
    {
        if(graph[g_it->first].type == INPUT)
        {
            graph[g_it->first].val = pattern[pat_it];
            pat_it++;
        }
    }
}

void itr_sim::eval_gval(int n_num)
{
    list<int> templist = graph[n_num].fanin;
    int tempval = graph[templist.front()].val;
    templist.pop_front();
    list<int>::iterator f_it;
    
    switch(graph[n_num].type)
    {
        case AND:
        case NAND:
            for(f_it = templist.begin(); f_it != templist.end(); ++f_it)
            {
                tempval = AND_VAL[tempval][graph[*f_it].val];
            }
            if(graph[n_num].type == AND)
                graph[n_num].val = tempval;
            else
                graph[n_num].val = inv_val(tempval);
            break;
        case OR:
        case NOR:
            for(f_it = templist.begin(); f_it != templist.end(); ++f_it)
            {
                tempval = OR_VAL[tempval][graph[*f_it].val];
            }
            if(graph[n_num].type == OR)
                graph[n_num].val = tempval;
            else
                graph[n_num].val = inv_val(tempval);
            break;
        case BUF:
            graph[n_num].val = tempval;
            break;
        case NOT:
            graph[n_num].val = inv_val(tempval);
            break;
    }
}

/*
 * Need to be updated for new pulse generation algorithm
 */

void itr_sim::gen_p(int n_num)
{   
    /*if(graph[n_num].val == 0)
    {
        graph[n_num].p_list.push_back(gen_pulse(RISING, n_num));
        
        // Increase event number since a falling pulse is not generated
        this->event_n++;
    }
    else if(graph[n_num].val == 1)
    {
        // Increase event number since a rising pulse is not generated
        this->event_n++;
        
        graph[n_num].p_list.push_back(gen_pulse(FALLING, n_num));
    }*/
}

void itr_sim::set_propfunc(int n_num, int inp_node, transient p, transient temp)
{
    list<int>::iterator l_it;
    bool flag = true;
    
    switch(graph[n_num].type)
    {
        case NOT:
        case BUF:
            graph[n_num].p_list.push_back(temp);
            break;
        case AND:
        case NAND:
            for(l_it = graph[n_num].fanin.begin(); l_it != graph[n_num].fanin.end(); ++l_it)
            {
               if(*l_it != inp_node)
               {
                   if(graph[*l_it].val == 0)
                       flag = false;
               }     
            }
            if(flag == true)
            {
                graph[n_num].p_list.push_back(temp);
            }
        case OR:
        case NOR:
            for(l_it = graph[n_num].fanin.begin(); l_it != graph[n_num].fanin.end(); ++l_it)
            {
               if(*l_it != inp_node)
               {
                   if(graph[*l_it].val == 1)
                       flag = false;
               }     
            }
            if(flag == true)
            {
                graph[n_num].p_list.push_back(temp);
            }
            break;
    }
}

void itr_sim::set_convprop(int n_num, transient &temp, transient p1, transient p2)
{

}

void itr_sim::add_result(int n_num)
{
    list<transient>::iterator p_it;
    
    for(p_it = graph[n_num].p_list.begin(); p_it != graph[n_num].p_list.end(); ++p_it)
    {
        if(m_find(graph[n_num].r_map, p_it->e_num) == true)
            graph[n_num].r_map[p_it->e_num]++;
        else
            graph[n_num].r_map[p_it->e_num] = 1;
    }
}

void itr_sim::gen_pat(int* pattern)
{
    int i;

    for (i = 0; i != circuit.inpnum; i++)
    {
        pattern[i] = rand() % 2;
    }
}

void itr_sim::gen_itrpat(int* pattern, int val)
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

void itr_sim::end_sim()
{
    
}
