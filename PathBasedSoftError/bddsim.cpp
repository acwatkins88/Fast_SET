#include "transsim.h"

void bdd_sim::sim()
{
    int max_partn;
    bdd_prob f_prob;

    this->sim_type = BDD_SIM;

    if (CONE_SIM == 1)
    {
        max_partn = label_cone();
        graph_m = graph;

        for (int i = 0; i <= max_partn; i++)
        {
            graph = extract_circuit(i);

            sim_graph(graph);

            graph = graph_m;
            event_n = 0;
            id_n = 0;
        }
    } 
    else
    {
        sim_graph(graph);
        part_circuit(graph);
        
        gmap::iterator inpit;
        for(inpit = inp_g.begin(); inpit != inp_g.end(); ++inpit)
        {
            cout<<"Node: "<<inpit->first<<endl;
            cout<<"Gain: "<<inp_g[inpit->first].gain<<endl;
            cout<<"Partition: "<<inp_g[inpit->first].b_part_num<<endl;
            cout<<endl;
        }
            
    }
}

void bdd_sim::sim_graph(gmap &graph)
{
    gmap::iterator git;
    list<transient>::iterator p_it;
    
    for (git = graph.begin(); git != graph.end(); ++git)
    {
        if (graph[git->first].type == INPUT)
        {
            graph[git->first].g_func = new bdd();
            *graph[git->first].g_func = new_var();
        } 
        else
        {
            graph[git->first].proc_flag = 1;

            // Generate Function for the Gate
            gen_sensf(git->first);

            if (bdd_nodecount(*graph[git->first].g_func) > 1000)
                bdd_reorder(BDD_REORDER_WIN2ITE);

            // Generate New Pulses
            bdd_genp(git->first);

            // Propagate Existing Pulses
            proc_pulse(git->first);

            // Check for Convergence 
            if ((graph[git->first].type != NOT) || (graph[git->first].type != BUF))
            {
                conv_check(git->first);
            }

            bdd_optimize();

            // Load into Final Result Structure
            /*if(out_find(git->first))
            {
                for(p_it = graph[git->first].p_list.begin(); p_it != graph[git->first].p_list.end(); ++p_it)
                {
                    f_prob.solve_prob(p_it->p_func);

                    if(m_find(graph[git->first].r_map, p_it->e_num))
                    {
                        graph[git->first].r_map[p_it->e_num] = graph[git->first].r_map[p_it->e_num] + f_prob.true_prob;
                    }
                    else
                    {
                        graph[git->first].r_map[p_it->e_num] = f_prob.true_prob;
                    }
                }
                graph_m[git->first].r_map = graph[git->first].r_map;
            }*/

            // Load pulses into Result Structure
            /*if(out_find(git->first) == true)
            {
                for(p_it = graph[git->first].p_list.begin(); p_it != graph[git->first].p_list.end(); ++p_it)
                {
                    f_prob.solve_prob(p_it->p_func);

                    if(m_find(graph[git->first].r_map, p_it->e_num) == true)
                    {
                        graph[git->first].r_map[p_it->e_num] = graph[git->first].r_map[p_it->e_num] + f_prob.true_prob;
                    }
                    else
                    {
                        graph[git->first].r_map[p_it->e_num] = f_prob.true_prob;
                    }
                }
                //graph[git->first].p_list.clear();
                //graph[git->first].del_flag = 1;
                //delete graph[git->first].g_func;
            }*/
        }
        cout << "Gate Processed: " << git->first << endl;
    }
}

/*
 * Generate Sensitization Function
 */
void bdd_sim::gen_sensf(int n_num)
{
    int count(0);
    list<int>::iterator inp;
    
    graph[n_num].g_func = new bdd();
    
    for(inp = graph[n_num].fanin.begin(); inp != graph[n_num].fanin.end(); ++inp)
    {
        if(count == 0)
        {
            switch(graph[n_num].type)
            {
                case AND:
                case NAND:
                case NOR:
                case OR:
                case BUF:
                    *graph[n_num].g_func = *graph[*inp].g_func;
                    count++;
                    break;
                case NOT:
                    *graph[n_num].g_func = !(*graph[*inp].g_func);
                    break;        
            }
        }
        else
        {
            switch (graph[n_num].type)
            {
                case AND:
                case NAND:
                    *graph[n_num].g_func = (*graph[n_num].g_func & *graph[*inp].g_func);
                    break;
                case OR:
                case NOR:
                    *graph[n_num].g_func = (*graph[n_num].g_func | *graph[*inp].g_func);
                    break;
            }
        }
    }
    if(graph[n_num].type == NAND || graph[n_num].type == NOR)
        *graph[n_num].g_func = !(*graph[n_num].g_func);
}

/*
 * Pulse Generation Algorithm Need Updated
 */

void bdd_sim::bdd_genp(int n_num)
{
    
    bdd const_f = bdd_false();
    transient temp_r;
    //transient temp_f;
    
    temp_r = gen_pulse(RISING, n_num);
    //temp_f = gen_pulse(FALLING, n_num);
    
    temp_r.p_func = gen_bdd(n_num, temp_r);
    //temp_f.p_func = gen_bdd(n_num, temp_f);
    
    if(temp_r.p_func != const_f)
       graph[n_num].p_list.push_back(temp_r);
    
    /*if(temp_f.p_func != const_f)
        graph[n_num].p_list.push_back(temp_f);
     */
}

bdd bdd_sim::gen_bdd(int n_num, transient inp)
{
    if(inp.type == RISING)
        return bdd_not(*graph[n_num].g_func);
    else
        return *graph[n_num].g_func;
}

void bdd_sim::set_propfunc(int n_num, int inp_node, transient p, transient temp)
{
    bdd const_f = bdd_false();
    
    temp.p_func = prop_bdd(n_num, inp_node, p);

    if (temp.p_func != const_f)
        graph[n_num].p_list.push_back(temp);
}

/*
 * Propagate BDD function to node n_num
 * p_node - ID of node where the pulse originates
 */
bdd bdd_sim::prop_bdd(int n_num, int p_node, transient in_t)
{
    bdd p_func;
    list<int>::iterator inp;
    
    if(graph[n_num].type ==  NOT || graph[n_num].type == BUF)
    {
        p_func = in_t.p_func;
    }
    else if(graph[n_num].type ==  AND || graph[n_num].type == NAND)
    {
        p_func = in_t.p_func;
        
        for(inp = graph[n_num].fanin.begin(); inp != graph[n_num].fanin.end(); ++inp)
        {
            if(*inp != p_node)
            {
                p_func = p_func & *graph[*inp].g_func;
            }
        }
    }
    else if(graph[n_num].type == OR || graph[n_num].type == NOR)
    {
        p_func = in_t.p_func;
        
        for(inp = graph[n_num].fanin.begin(); inp != graph[n_num].fanin.end(); ++inp)
        {
            if(*inp != p_node)
            {
                p_func = p_func & bdd_not(*graph[*inp].g_func);
            }
        }
    }
    return p_func;
}

void bdd_sim::set_convprop(int n_num, transient &temp, transient p1, transient p2)
{

    temp.p_func = get_convbdd(n_num, p1, p2);
}

bdd bdd_sim::get_bdd(list<transient> p_list, list<prev_p> id)
{
    bdd const_t = bdd_true();
    list<transient>::iterator p_it;
    list<prev_p>::iterator i_it;
    
    for(p_it = p_list.begin(); p_it != p_list.end(); ++p_it)
    {
        for(i_it = id.begin(); i_it != id.end(); ++i_it)
        {
            if(i_it->p_pulse == p_it->id)
            {
                return p_it->p_func;
            }
        }
    }
    return const_t;
}

bdd bdd_sim::get_convbdd(int n_num, transient p1, transient p2) 
{
    bdd temp;
    bdd const_t = bdd_true();
    list<int>::iterator f_it;
    
    temp = const_t;
    for (f_it = graph[n_num].fanin.begin(); f_it != graph[n_num].fanin.end(); ++f_it) 
    {
        if (is_pulse(p1.prev_pulse, *f_it) == true) 
        {
            temp = temp & get_bdd(graph[*f_it].p_list, p1.prev_pulse);
        } 
        else if (is_pulse(p2.prev_pulse, *f_it) == true) 
        {
            temp = temp & get_bdd(graph[*f_it].p_list, p2.prev_pulse);
        } 
        else
        {
            if((graph[n_num].type == AND)||(graph[n_num].type == NAND))
                temp= temp & *graph[*f_it].g_func;
            else if((graph[n_num].type == OR)||(graph[n_num].type == NOR))
                temp= temp & !*graph[*f_it].g_func;
        }
            
    }
    return temp;
}

void bdd_sim::bdd_optimize()
{
    int flag;
    
    gmap::iterator git;
    list<int>::iterator fo_it;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if((graph[git->first].type != INPUT) && (out_find(git->first) != true))
        {
            if((graph[git->first].proc_flag == 1) && (graph[git->first].del_flag == 0))
            {
                flag = 0;
                for(fo_it = graph[git->first].fanout.begin(); fo_it != graph[git->first].fanout.end(); ++fo_it)
                {
                    if(graph[*fo_it].proc_flag == 0)
                    {
                        flag = 1;
                    }
                }
                if(flag == 0)
                {
                    graph[git->first].p_list.clear();
                    list<transient>() = graph[git->first].p_list;
                    graph[git->first].del_flag = 1;
                    *graph[git->first].g_func = bdd_true();
                    delete graph[git->first].g_func;
                } 
            }
        }
    }
}

void bdd_sim::end_sim()
{
    gmap::iterator git;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        graph[git->first].r_map.clear();
                
        if(graph[git->first].del_flag == 0)
        {
            graph[git->first].p_list.clear();
            list<transient>() = graph[git->first].p_list;
            
            //cout<<"Gate: "<<git->first<<endl;
            delete graph[git->first].g_func;
        }
    }
}