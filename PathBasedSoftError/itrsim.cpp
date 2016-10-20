#include "transsim.h"
//#include "gates.h"

using namespace std;

void itr_sim::sim()
{
    int i, max_itr, flag;
    int* pattern = new int[circuit.inpnum];
     
    gmap::iterator git;
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
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(graph[git->first].type != INPUT)
            gen_p(git->first);
    }

    graph_m = graph;

    for (git = graph.begin(); git != graph.end(); ++git)
    {
        if (graph[git->first].type != INPUT)
        {
            graph[git->first].p_list.clear();
        }
    }
    
    // Begin Simulation
    for(i = 0; i != max_itr; i++)
    {
        cout<<"Iteration: "<<i<<endl;
        
        this->event_n = 0;
        this->id_n = 0;
        
        if(flag == 0)
        {
            gen_itrpat(pattern, i);
        }
        else if(flag == 1)
        {
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
    
        for(git = graph.begin(); git != graph.end(); ++git)
        {
            if(graph[git->first].type != INPUT)
            {
                eval_gval(git->first);
                app_pulse(git->first);

                prop_enhpulse(git->first);
                
                // Process Convergence Cases
                if ((graph[git->first].type != NOT) && (graph[git->first].type != BUF))
                {
                    if(!graph[git->first].p_list.empty())
                    {
                        enhconv_check(git->first);
                    }
                }   
                
                // Store the result in r_map
                if(out_find(git->first) == true)
                {
                    add_result(git->first);
                }
            }
            graph[git->first].prob += graph[git->first].val;
        }
        
        for(git = graph.begin(); git != graph.end(); ++git)
        {
            if(graph[git->first].type != INPUT)
            {
                graph[git->first].p_list.clear();
            }
        }
    }
        
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        graph[git->first].prob = graph[git->first].prob / max_itr;
    }
    for(l_it = circuit.outputs.begin(); l_it != circuit.outputs.end(); ++l_it)
    {
        for(m_it = graph[*l_it].r_map.begin(); m_it != graph[*l_it].r_map.end(); ++m_it)
            graph[*l_it].r_map[m_it->first] = graph[*l_it].r_map[m_it->first] / (max_itr);
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

void itr_sim::app_pulse(int n_num)
{   
    list<transient>::iterator pit;
    
    if(graph[n_num].val == 0)
    {
        for(pit = graph_m[n_num].p_list.begin(); pit != graph_m[n_num].p_list.end(); ++pit)
            if(pit->type == RISING)
                graph[n_num].p_list.push_back(*pit);
    }
    else if(graph[n_num].val == 1)
    {   
        for(pit = graph_m[n_num].p_list.begin(); pit != graph_m[n_num].p_list.end(); ++pit)
            if(pit->type == FALLING)
                graph[n_num].p_list.push_back(*pit);
    }
}

/*
 * Need to be updated for new pulse generation algorithm
 */

/*void itr_sim::gen_p(int n_num)
{   
    if(graph[n_num].val == 0)
    {
        graph[n_num].p_list.push_back(gen_pulse(RISING, n_num, INJ_DELAY));
        
        // Increase event number since a falling pulse is not generated
        this->event_n++;
    }
    else if(graph[n_num].val == 1)
    {   
        graph[n_num].p_list.push_back(gen_pulse(FALLING, n_num, INJ_DELAY));
        
        // Increase event number since a rising pulse is not generated
        this->event_n++;
    }
}*/

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
    
    double latch_prob;
    for(p_it = graph[n_num].p_list.begin(); p_it != graph[n_num].p_list.end(); ++p_it)
    {
        latch_prob = (p_it->width - (SETUP_T + HOLD_T))/CLK;
        if(m_find(graph[n_num].r_map, p_it->e_num) == true)
        {
            graph[n_num].r_map[p_it->e_num] = graph[n_num].r_map[p_it->e_num] + latch_prob;
            //graph[n_num].r_map[p_it->e_num]++;
        }
        else
        {
            graph[n_num].r_map[p_it->e_num] = latch_prob;
            //graph[n_num].r_map[p_it->e_num] = 1;
        }
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
