#include "transsim.h"

void bdd_sim::sim()
{
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
    }

    if (CONE_SIM == 1)
    {
        max_partn = label_cone();
        graph_m = graph;

        for (int i = 0; i <= max_partn; i++)
        {
            graph = extract_circuit(i);
            
            /*cout<<endl<<"Master Graph\n";
            cout<<"Current Partnum: "<<i<<endl;
            for(git = graph_m.begin(); git != graph_m.end(); ++git)
            {
                cout<<"Cell: "<<git->first<<endl;
                cout<<"Partitions: ";
                for(lit = graph_m[git->first].part.begin(); lit != graph_m[git->first].part.end(); ++lit)
                    cout<<*lit<<" ";
                cout<<endl;
                cout<<"Fanouts: ";
                for(lit = graph_m[git->first].fanout.begin(); lit != graph_m[git->first].fanout.end(); ++lit)
                    cout<<*lit<<" ";
                cout<<endl<<"End"<<endl;
            }
            cout<<endl;*/
            is_overflowed = sim_graph(graph);
            cout<<"Overflowed: "<<is_overflowed<<endl;
            
            if(is_overflowed)
            {
                part_circuit(graph);

                /*for(git = inp_g.begin(); git != inp_g.end(); ++git)
                {
                    cout<<"Node: "<<git->first<<endl;
                    cout<<"Partition: "<<inp_g[git->first].b_part_num<<endl;
                }*/
                
                conv_partition(graph_m, max_partn, i);
                //cout<<"Circuit Partitioned: "<<i<<endl;
                //cout<<"Max Partition: "<<max_partn<<endl;
                
                i--;
            }
            graph.clear();
            graph = graph_m;
            //event_n = 0;
            //id_n = 0;
        }
        /*gmap::iterator git;
        list<int>::iterator lit;
        for(git = graph_m.begin(); git != graph_m.end(); ++git)
        {
            cout<<"Cell: "<<git->first<<endl;
            cout<<"Partition: ";
            for(lit = graph_m[git->first].part.begin(); lit != graph_m[git->first].part.end(); ++lit)
            {
                cout<<*lit<<" ";
            }
            cout<<endl;
        }*/
            
    } 
    else
    {
        sim_graph(graph);
        
        /*part_circuit(graph);
        
        gmap::iterator inpit;
        for(inpit = inp_g.begin(); inpit != inp_g.end(); ++inpit)
        {
            cout<<"Node: "<<inpit->first<<endl;
            cout<<"Gain: "<<inp_g[inpit->first].gain<<endl;
            cout<<"Size: "<<inp_g[inpit->first].c_size<<endl;
            cout<<"Probability: "<<inp_g[inpit->first].prob<<endl;
            cout<<"Partition: "<<inp_g[inpit->first].b_part_num<<endl;
            cout<<endl;
        }
        cout<<"Final Size Part 1: "<<size_part1<<endl;
        cout<<"Final Size Part 2: "<<size_part2<<endl;
        cout<<"Total Size: "<<total_size<<endl;
        double ratio_v = double(size_part1)/double(total_size);
        cout<<"Final Ratio: "<<ratio_v<<endl;*/
            
    }
}

bool bdd_sim::sim_graph(gmap &graph)
{
    int total_count = 0;
    bdd_prob s_prob;
    gmap::iterator git;
    list<transient>::iterator p_it;
    int t_id;

    for (git = graph.begin(); git != graph.end(); ++git)
    {
        if (graph[git->first].type == INPUT)
        {
            graph[git->first].g_func = new bdd();
            *graph[git->first].g_func = new_var();
            
            //cout<<"Input: "<<git->first<<" Var Number: "<<bdd_var(*graph[git->first].g_func)<<endl;
            //cout<<"Probabiltiy: "<<graph[git->first].prob<<endl;
            
            /*list<transient>::iterator tit;
            cout<<"Pulse List: ";
            for(tit = graph[git->first].p_list.begin(); tit != graph[git->first].p_list.end(); ++tit)
                cout<<tit->id<<" ";
            cout<<endl;*/
            s_prob.inp_map[bdd_var(*graph[git->first].g_func)] = graph[git->first].prob;
        }
        else
        {

            graph[git->first].proc_flag = 1;

            // Generate Function for the Gate
            gen_sensf(git->first);
            
            if (bdd_nodecount(*graph[git->first].g_func) > 1000)
                bdd_reorder(BDD_REORDER_WIN2ITE);
            
            // Generate New Pulses
            if(graph_m[git->first].gen_flag == false)
            {
                bdd_genp(git->first);
                graph_m[git->first].gen_flag = true;
            }

            // Propagate Existing Pulses
            proc_pulse(git->first);

            // Check for Convergence 
            if ((graph[git->first].type != NOT) || (graph[git->first].type != BUF))
            {
                if(!graph[git->first].p_list.empty())
                {
                    conv_check(git->first);
                }
            }
            
            total_count = total_count + count_nodes(graph, git->first);
            cout<<"Total: "<<total_count<<endl<<endl;

            gmap::iterator mit;
            if (total_count > MAX_BDD_NODES)
            {
                for(mit = graph.begin(); mit != graph.end(); ++mit)
                {
                    graph_m[mit->first].p_list.clear();
                    graph_m[mit->first].gen_flag = false;
                }
                return true;
            }
            
            // Load into Final Result Structure 
            // Rewrite so that the probability from a partition output is propagated
            if(out_find(git->first))
            {
                /*map<int, bdd>::iterator bit;
                for(p_it = graph_m[git->first].p_list.begin(); p_it != graph_m[git->first].p_list.end(); ++p_it)
                {
                    if(m_find(graph[git->first].p_prob_map, p_it->e_num))
                        graph[git->first].p_prob_map[p_it->e_num] = graph[git->first].p_prob_map[p_it->e_num]*p_it->t_prob;
                    else
                        graph[git->first].p_prob_map[p_it->e_num] = p_it->t_prob;
                }
                for(p_it = graph[git->first].p_list.begin(); p_it != graph[git->first].p_list.end(); ++p_it)
                {
                    if(m_findbdd(graph[git->first].bdd_map, p_it->e_num))
                        graph[git->first].bdd_map[p_it->e_num] = graph[git->first].bdd_map[p_it->e_num] | p_it->p_func;
                    else
                        graph[git->first].bdd_map[p_it->e_num] = p_it->p_func;
                }
                
                for(bit = graph[git->first].bdd_map.begin(); bit != graph[git->first].bdd_map.end(); ++bit)
                {
                    s_prob.solve_prob(graph[git->first].bdd_map[bit->first]);
                    cout<<"Event: "<<bit->first<<" Prob Map: "<<graph[git->first].p_prob_map[bit->first]<<endl;
                    graph[git->first].r_map[bit->first] = s_prob.true_prob * graph[git->first].p_prob_map[bit->first];
                }
                graph_m[git->first].r_map = graph[git->first].r_map;
                graph[git->first].bdd_map.clear();
                graph[git->first].p_prob_map.clear();*/
                for(p_it = graph[git->first].p_list.begin(); p_it != graph[git->first].p_list.end(); ++p_it)
                {
                    s_prob.solve_prob(p_it->p_func);
                    graph_m[git->first].r_map[p_it->id] = s_prob.true_prob;
                }
            }
            else
            {
                // Change so that the bdd is only solved if the graph is overflowed
                list<transient>::iterator p_it;

                s_prob.solve_prob(*graph[git->first].g_func);
                graph[git->first].prob = s_prob.true_prob;
                graph_m[git->first].prob = graph[git->first].prob;
                
                graph_m[git->first].p_list = graph[git->first].p_list;

                for (p_it = graph_m[git->first].p_list.begin(); p_it != graph_m[git->first].p_list.end(); ++p_it)
                {
                    s_prob.solve_prob(p_it->p_func);
                    p_it->t_prob = s_prob.true_prob;
                    p_it->p_func = bdd_true();
                }
            }
        }
        cout << "Gate Processed: " << git->first << endl;
        
        bdd_optimize();
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
    
    temp_r.p_func = gen_bdd(n_num, temp_r);
    temp_f.p_func = gen_bdd(n_num, temp_f);
    
    if(temp_r.p_func != const_f)
       graph[n_num].p_list.push_back(temp_r);
    
    if(temp_f.p_func != const_f)
        graph[n_num].p_list.push_back(temp_f);
     
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