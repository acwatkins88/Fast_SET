#include "transsim.h"

void bdd_sim::sim()
{
    alloc_count = 0;
    del_count = 0;
    bool is_overflowed;
    int max_partn;
    gmap::iterator git;
    list<int>::iterator lit;

    this->sim_type = BDD_SIM;
    
    // Set initial probabilities
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(graph[git->first].type == INPUT)
            graph[git->first].prob = T_PROB;
        else
        {
            bdd_genp(git->first);
        }
    }

    if (CONE_SIM == 1)
    {
        max_partn = label_cone();
        graph_m = graph;
        
        list<transient>::iterator pit;
        
        for (int i = 0; i <= max_partn; i++)
        {
            graph = extract_circuit(i);
            
            is_overflowed = sim_graph(graph);
            cout<<"Overflowed: "<<is_overflowed<<endl;
            
            if(is_overflowed)
            { 
                part_circuit(graph);
                
                conv_partition(graph_m, max_partn, i);
                
                i--;
                
                gmap::iterator dit;
                over_it++;
                for(dit = graph.begin(); dit != over_it; ++dit)
                {
                    if(graph[dit->first].func_del == false)
                    {
                        delete graph[dit->first].g_func;
                        del_count++;
                        //cout<<"Delete 0: "<<dit->first<<endl;
                    }
                }
            }
            
            graph.clear();
            graph = graph_m;
            bdd_gbc();
        }           
    } 
    else
    {
        // Partition Circuit into two parts
        
        gmap::iterator git;
        list<int>::iterator lit;
        
        max_partn = 0;
        
        for(git = graph.begin(); git != graph.end(); ++git)
        {
            graph[git->first].part.push_back(0);
        }
        
        graph_m = graph;
        part_circuit(graph_m);
        conv_partition(graph_m, max_partn, 0);
        
        graph = graph_m;
        
        for(int i = 0; i <= max_partn; i++)
        {
            //cout<<"Here: "<<i<<" Max Part: "<<max_partn<<endl;
            graph = extract_circuit(i);
            sim_graph(graph);
            
            graph.clear();
            graph = graph_m;
        }
    }
}

bool bdd_sim::sim_graph(gmap &graph)
{
    int total_count = 0;
    int num_removed = 0;
    bdd_prob s_prob;
    gmap::iterator git;
    list<transient>::iterator p_it;

    for (git = graph.begin(); git != graph.end(); ++git)
    {
        if (graph[git->first].type == INPUT)
        {
            graph[git->first].g_func = new bdd();
            *graph[git->first].g_func = new_var();
            alloc_count++;
            //cout<<"Adding 1: "<<git->first<<endl;
            
            //if(git->first < 0)
            s_prob.inp_map[bdd_var(*graph[git->first].g_func)] = graph[git->first].prob;     
                
            //cout<<"Total: "<<total_count<<endl;
        }
        else
        { 
            graph[git->first].proc_flag = 1;
            
            // Generate Function for the Gate
            gen_sensf(git->first);
            
            if (bdd_nodecount(*graph[git->first].g_func) > 1000)
                bdd_reorder(BDD_REORDER_WIN2ITE);
            
            list<transient>::iterator tpit;
            if(t_find(tp_map, git->first))
            {
                for(tpit = tp_map[git->first].begin(); tpit != tp_map[git->first].end(); ++tpit)
                {
                    graph[git->first].p_list.push_back(*tpit);
                }
            }
            
            bdd_genfunc(git->first, graph[git->first].p_list);
            
            // Propagate Existing Pulses
            proc_pulse(git->first);
            
            // Check for Convergence 
            /*if ((graph[git->first].type != NOT) || (graph[git->first].type != BUF))
            {
                if(!graph[git->first].p_list.empty())
                {
                    conv_check(git->first);
                }
            }*/
            
            //total_count = total_count + count_nodes(graph, git->first) - num_removed;
            total_count = bdd_getnodenum();
            num_removed = 0;
            //cout<<"Total: "<<total_count<<" List Num: "<<graph[git->first].p_list.size()<<endl;
            
            if ((total_count > MAX_BDD_NODES)&&(graph[git->first].fanout_num != 0)&&(CONE_SIM == 1))
            {   
                over_it = git;
                return true;
            }
            
            list<transient>::iterator pit;
            
            /*cout<<"Node: "<<git->first<<endl;
            for (pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
                cout<<"ID: "<<pit->id<<" Event: "<<pit->e_num<<endl;*/
            
            // Load into Final Result Structure 
            // Rewrite so that the probability from a partition output is propagated
            if((graph[git->first].fanout_num != graph_m[git->first].fanout_num)||(graph[git->first].fanout_num == 0))
            {
                s_prob.solve_prob(*graph[git->first].g_func);
                graph[git->first].prob = s_prob.true_prob;
                graph_m[git->first].prob = graph[git->first].prob;
                if(graph[git->first].fanout_num == 0)
                {
                    delete graph[git->first].g_func;
                    graph[git->first].func_del = true;
                    del_count++;
                    //cout<<"Delete 1: "<<git->first<<endl;
                }
            }
        }
        cout << "Gate Processed: " << git->first << endl;
        
        bdd_optimize();
    }
    
    list<transient>::iterator pit;
    list<int>::iterator fit;
    transient temp;
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if((graph[git->first].type == INPUT))
        {
            delete graph[git->first].g_func;
            graph[git->first].func_del = true;
            del_count++;
            //cout<<"Delete 2: "<<git->first<<endl;
        }
        
        if (out_find(git->first))
        {
            // Load data by id number
            for (pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
            {
                if (m_find(graph_m[git->first].r_map, pit->e_num))
                {
                    //cout<<"ID: "<<pit->id<<" Event: "<<pit->e_num<<" Number of BDD Nodes: "<<bdd_nodecount(pit->p_func)<<endl;
                    s_prob.solve_prob(pit->p_func);
                    /*if(git->first == 880)
                        cout<<"ID: "<<pit->id<<" Event: "<<pit->e_num<<" Prob: "<<s_prob.true_prob<<" True: "<<pit->t_prob<<endl;*/
                    graph_m[git->first].r_map[pit->e_num] = graph_m[git->first].r_map[pit->e_num]+(s_prob.true_prob * pit->t_prob);
                    //graph_m[git->first].r_map[pit->e_num] = graph_m[git->first].r_map[pit->e_num]+(s_prob.true_prob);
                }
                else
                {
                    //cout<<"ID: "<<pit->id<<" Event: "<<pit->e_num<<" Number of BDD Nodes: "<<bdd_nodecount(pit->p_func)<<endl;
                    s_prob.solve_prob(pit->p_func);
                    /*if(git->first == 880)
                        cout<<"ID: "<<pit->id<<" Event: "<<pit->e_num<<" Prob: "<<s_prob.true_prob<<" True: "<<pit->t_prob<<endl;*/
                    graph_m[git->first].r_map[pit->e_num] = (s_prob.true_prob * pit->t_prob);
                    //graph_m[git->first].r_map[pit->e_num] = (s_prob.true_prob);
                }
            }
            tp_map.clear();
        }
        else if ((graph[git->first].type != INPUT)&&((graph[git->first].fanout_num != graph_m[git->first].fanout_num) || (graph[git->first].fanout_num == 0)))
        {
            if(CONE_SIM == 1)
            {
                for (pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
                {
                    s_prob.solve_prob(pit->p_func);
                    pit->t_prob = s_prob.true_prob;
                    temp = *pit;
                    temp.p_func = bdd_true();
                    for (fit = graph_m[git->first].fanout.begin(); fit != graph_m[git->first].fanout.end(); ++fit)
                        tp_map[*fit].push_back(temp);
                }
            }
        }
    }
    
    return false;
}

/*
 * Generate Sensitization Function
 */
void bdd_sim::gen_sensf(int n_num)
{
    int count(0);
    list<int>::iterator inp;
    
    graph[n_num].g_func = new bdd();
    alloc_count++;
    //cout<<"Adding 2: "<<n_num<<endl;
    
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
    transient temp_f;
    
    temp_r = gen_pulse(RISING, n_num);
    temp_f = gen_pulse(FALLING, n_num);
    
    graph[n_num].p_list.push_back(temp_r);
    graph[n_num].p_list.push_back(temp_f);
    
    /*temp_r.p_func = gen_bdd(n_num, temp_r);
    temp_f.p_func = gen_bdd(n_num, temp_f);
    
    if(temp_r.p_func != const_f)
       graph[n_num].p_list.push_back(temp_r);
    
    if(temp_f.p_func != const_f)
        graph[n_num].p_list.push_back(temp_f);*/
     
}

/*
 * Function to generate bdd functions for new pulses
 */
void bdd_sim::bdd_genfunc(int n_num, list<transient>& inp_list)
{
    bdd const_f = bdd_false();
    list<transient>::iterator lit;
    
    for(lit = inp_list.begin(); lit != inp_list.end(); ++lit)
    {
        //if(lit->s_node == n_num)
        //{
            lit->p_func = gen_bdd(n_num, *lit);

            if(lit->p_func == const_f)
            {
                lit = inp_list.erase(lit);
            }
        //}
    }
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

bool bdd_sim::m_findbdd(map<int, bdd> bdd_map, int e_num)
{
    map<int, bdd>::iterator bit;
    
    for(bit = bdd_map.begin(); bit != bdd_map.end(); ++bit)
        if(bit->first == e_num)
            return true;
    
    return false;
}

/*
 * Routine to count the number of bdd nodes in a cell
 * Use for adaptive partitioning
 */
int bdd_sim::count_nodes(gmap g_input, int node)
{
    int count = 0;
    list<transient>::iterator lit;

    count = count + bdd_nodecount(*g_input[node].g_func);
    for (lit = g_input[node].p_list.begin(); lit != g_input[node].p_list.end(); ++lit)
    {
        count = count + bdd_nodecount(lit->p_func);
    }
    
    return count;
}

/*
 * Routine to convert the partition on the subgraph inp_g to the main graph g_main
 */
void bdd_sim::conv_partition(gmap &g_main, int &max_part, int cur_partnum)
{
    gmap::iterator git;
    list<int>::iterator lit;
    list<int> tmp_list;
    
    max_part++;
    
    for(git = g_main.begin(); git != g_main.end(); ++git)
    {
        if(git->first >= 0)
        {
           for(lit = g_main[git->first].part.begin(); lit != g_main[git->first].part.end(); ++lit)
            {
                if(*lit > cur_partnum)
                    tmp_list.push_back(*lit+1);
                else
                    tmp_list.push_back(*lit);
            }
           g_main[git->first].part.clear();
           g_main[git->first].part = tmp_list;
           tmp_list.clear();
        }
    }
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if((inp_g[git->first].b_part_num == 1) && (git->first >= 0))
        {
            g_main[git->first].part.push_back(cur_partnum+1);
            g_main[git->first].part.remove(cur_partnum);
        }
    }
}

void bdd_sim::bdd_optimize()
{
    int flag;
    
    gmap::iterator git;
    list<int>::iterator fo_it;
    list<transient>::iterator lit;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        //if((graph[git->first].type != INPUT)&&(out_find(git->first) != true)&&(graph[git->first].fanout_num != 0)&&(graph[git->first].fanout_num == graph_m[git->first].fanout_num))
        //if((out_find(git->first) != true)&&(graph[git->first].fanout_num != 0)&&(graph[git->first].fanout_num == graph_m[git->first].fanout_num))
        if((out_find(git->first) != true)&&(graph[git->first].fanout_num != 0))
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
                    graph[git->first].func_del = true;
                    del_count++;
                    //cout<<"Delete 3: "<<git->first<<endl;
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
            graph[git->first].func_del = true;
            del_count++;
        }
    }
}