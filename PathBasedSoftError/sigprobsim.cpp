#include "transsim.h"

using namespace std;

void sigprob_sim::sim()
{
    gmap::iterator git;
    //list<int>::iterator o_it;

    this->sim_type = SIGPROB_SIM;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(graph[git->first].type == INPUT)
        {
            graph[git->first].prob = T_PROB;

        }
        else{
            
            // Determine signal probabilities
            calc_sigprob();
            
            // Generate New Pulses
            gen_probp(git->first);
            
            // Propagate Existing Pulses
            proc_pulse(git->first);

            // Check for Convergence 
            if(git->first == 8)
            {

            conv_check(git->first);

            }
        }
    }
}

void sigprob_sim::gen_probp(int n_num)
{
    transient temp_r;
    transient temp_f;
    
    temp_r = gen_pulse(RISING, n_num);
    temp_f = gen_pulse(FALLING, n_num);
    
    temp_r.t_prob = calc_genprob(n_num, RISING);
    temp_f.t_prob = calc_genprob(n_num, FALLING);
    
    if(temp_r.t_prob != 0)
        graph[n_num].p_list.push_back(temp_r);
    if(temp_f.t_prob != 0)
        graph[n_num].p_list.push_back(temp_f);
}

double sigprob_sim::calc_genprob(int n_num, int type)
{
    double prob;
    list<int>::iterator i_it;
    
    if(type == FALLING)
        return graph[n_num].prob;
    else
        return (1-graph[n_num].prob);
    
    return prob;
}

bool sigprob_sim::check_gate(int n_num, int type)
{
    
    if(type == RISING)
    {
        if((graph[n_num].type == AND)||(graph[n_num].type == NOR)||(graph[n_num].type == BUF))
            return true;
    }
    else if(type == FALLING)
    {
        if((graph[n_num].type == NAND)||(graph[n_num].type == OR)||(graph[n_num].type == NOT))
            return true;
    }
    
    return false;
}

void sigprob_sim::set_propfunc(int n_num, int inp_node, transient p, transient temp)
{
    
    list<int>::iterator f_it;
    temp.t_prob = p.t_prob;
    
    for(f_it = graph[n_num].fanin.begin(); f_it != graph[n_num].fanin.end(); ++f_it)
    {
        if((graph[n_num].type == AND)||(graph[n_num].type == NAND))
        {
            if(*f_it != inp_node)
            {
                temp.t_prob = temp.t_prob*graph[*f_it].prob;
            }
        }
        else if((graph[n_num].type == OR)||(graph[n_num].type == NOR))
        {
            if(*f_it != inp_node)
            {
                temp.t_prob = temp.t_prob*(1-graph[*f_it].prob);
            }
        }
    }
    
    if(temp.t_prob != 0)
        graph[n_num].p_list.push_back(temp);
}

void sigprob_sim::set_convprop(int n_num, transient &temp, transient p1, transient p2)
{
    list<int>::iterator f_it;
    
    temp.t_prob = 1;
    
    for(f_it = graph[n_num].fanin.begin(); f_it != graph[n_num].fanin.end(); ++f_it)
    {
        if(is_pulse(p1.prev_pulse, *f_it) == true)
        {
            temp.t_prob = temp.t_prob*get_prob(graph[*f_it].p_list, p1.prev_pulse);
        }
        else if(is_pulse(p2.prev_pulse, *f_it) == true)
        {
            temp.t_prob = temp.t_prob*get_prob(graph[*f_it].p_list, p2.prev_pulse);
        }
        else
        {
            if((graph[n_num].type == AND)||(graph[n_num].type == NAND))
            {
                temp.t_prob = temp.t_prob*(graph[*f_it].prob);
            }
            else if((graph[n_num].type == OR)||(graph[n_num].type == NOR))
            {
                temp.t_prob = temp.t_prob*(1-graph[*f_it].prob);
            }
        }
    }
}

double sigprob_sim::get_prob(list<transient> p_list, list<prev_p> id)
{
    list<transient>::iterator p_it;
    list<prev_p>::iterator i_it;
    
    for(p_it = p_list.begin(); p_it != p_list.end(); ++p_it)
    {
        for(i_it = id.begin(); i_it != id.end(); ++i_it)
        {
            if(i_it->p_pulse == p_it->id)
            {
                return p_it->t_prob;
            }
        }
    }
    return 1;
}

void sigprob_sim::end_sim()
{
    gmap::iterator g_it;
    
    for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
    {
        graph[g_it->first].p_list.clear();
        list<transient>() = graph[g_it->first].p_list;
    }
}