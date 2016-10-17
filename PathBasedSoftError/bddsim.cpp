#include "transsim.h"

void bdd_sim::pnode_sim()
{
    bdd_prob s_prob;
    gmap::iterator git;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        cout<<"Starting: "<<git->first<<endl;
        
        if (graph[git->first].type == INPUT)
        {
            graph[git->first].g_func = new bdd();
            *graph[git->first].g_func = new_var();
            
            graph[git->first].prob = 0.5;
            
            s_prob.inp_map[bdd_var(*graph[git->first].g_func)] = graph[git->first].prob;          
        }
        else
        {   
            // Generate Function for the Gate
            gen_sensf(git->first);
            
            s_prob.solve_prob(*graph[git->first].g_func);
            graph[git->first].prob = s_prob.true_prob;
        }
        cout<<"Node: "<<git->first<<" Prob: "<<graph[git->first].prob<<endl;
    }
}

void bdd_sim::sim()
{
    alloc_count = 0;
    del_count = 0;
    bool is_overflowed;
    int max_partn;
    gmap::iterator git;
    double val;

    this->sim_type = BDD_SIM;
    
    // Set initial probabilities
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        //cout<<"Inj Node: "<<git->first<<" Type: "<<graph[git->first].type<<endl;
        if(graph[git->first].type == INPUT)
            graph[git->first].prob = T_PROB;
        else
            bdd_genp(git->first);
        
        //cout<<"Pulse Num: "<<graph[git->first].p_list.size()<<endl;
    }

    if (CONE_SIM == 1)
    {
        max_partn = label_cone();
        graph_m = graph;
        
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
        
        for(git = graph.begin(); git != graph.end(); ++git)
            graph[git->first].static_part = 0;
  
        max_partn = 0;
        graph_m = graph;
    
        for(int i = 0; i <= max_partn; ++i)
        {
            graph = extract_tcir(i);
            
            /*for(git = graph_m.begin(); git != graph_m.end(); ++git)
                cout<<"Node: "<<git->first<<" Part: "<<graph_m[git->first].static_part<<endl;
            */
            
            is_overflowed = sim_graph(graph);
            cout<<"Overflowed: "<<is_overflowed<<endl;
            
            if(is_overflowed)
            { 
                part_circuit(graph);
                
                conv_tpart(graph_m, max_partn, i);
                
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
            //graph = graph_m;
            bdd_gbc();
            
            /*cout<<"Testing Pulse Numbers\n";
            for(git = graph_m.begin(); git != graph_m.end(); ++git)
                cout<<"Node: "<<git->first<<" Pulses: "<<graph_m[git->first].p_list.size()<<endl;*/
        }
    }
}

/*
 * Modify bdd_genfunc and proc_pulse such that the largest bdd is found in the routine
 * Change proc_pulse so that if the bdd exceed the specified size, the change will occur instantly
 */

bool bdd_sim::sim_graph(gmap &graph)
{
    int bdd_nodenum;
    int largest_bdd = 0;
    int total_count = 0;
    int num_removed = 0;
    stringstream s;
    int f_count = 0;
    bdd_prob s_prob;
    gmap::iterator git;
    list<transient>::iterator nit;
    map<int, list<transient> > tp_temp;
    map<int, list<transient> >::iterator tmit;

    for (git = graph.begin(); git != graph.end(); ++git)
    {
        //cout<<"Starting: "<<git->first<<endl;
        
        if (graph[git->first].type == INPUT)
        {
            graph[git->first].g_func = new bdd();
            *graph[git->first].g_func = new_var();
            alloc_count++;
            //cout<<"Adding 1: "<<git->first<<endl;
            
            //if(git->first < 0)
            s_prob.inp_map[bdd_var(*graph[git->first].g_func)] = graph[git->first].prob;     
        }
        else
        { 
            graph[git->first].proc_flag = 1;
            
            // Generate Function for the Gate
            gen_sensf(git->first);
            
            if (bdd_nodecount(*graph[git->first].g_func) > 1000)
                bdd_reorder(BDD_REORDER_WIN2ITE);            
                 
            /*if(t_find(tp_map, git->first))
            {
                list<transient>::iterator tpit;
                for(tpit = tp_map[git->first].begin(); tpit != tp_map[git->first].end(); ++tpit)
                {
                    cout<<"Add Node: "<<git->first<<" Id: "<<tpit->id<<" Event: "<<tpit->e_num<<endl;
                    graph[git->first].p_list.push_back(*tpit);
                }
                tp_map.erase(git->first);
            }*/
       
            bdd_genfunc(git->first, graph[git->first].p_list);
  
            // Propagate Existing Pulses
            //proc_pulse(git->first);
            prop_enhpulse(git->first);
            
            // Check for Convergence 
            if ((graph[git->first].type != NOT) || (graph[git->first].type != BUF))
            {
                if(!graph[git->first].p_list.empty())
                {
                    enhconv_check(git->first);
                    //conv_check(git->first);
                }
            }
                        
            //for(p_it = graph[git->first].p_list.begin(); p_it != graph[git->first].p_list.end(); ++p_it)
                //cout<<"Node: "<<git->first<<" Event: "<<p_it->e_num<<" ID: "<<p_it->id<<" s_node: "<<p_it->s_node<<endl;
            
            //total_count = total_count + count_nodes(graph, git->first) - num_removed;
            //total_count = bdd_getnodenum();
            
            /*list<transient>::iterator pit;
            int max_bdd_size = 0;
            for(pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
            {
                bdd_nodenum = bdd_nodecount(pit->p_func);
                if(max_bdd_size < bdd_nodenum)
                    max_bdd_size = bdd_nodenum;
            }
        
            if(max_bdd_size > largest_bdd)
                largest_bdd = max_bdd_size;
            
            cout<<"Largest BDD: "<<largest_bdd<<" Number of Pulses: "<<graph[git->first].p_list.size()<<endl;
                
            num_removed = 0;
            //cout<<"Total: "<<total_count<<" List Num: "<<graph[git->first].p_list.size()<<endl;
            
            //if ((total_count > MAX_BDD_NODES)&&(graph[git->first].fanout_num != 0)&&(CONE_SIM == 1))
            if ((largest_bdd > MAX_BDD_NODES)&&(graph[git->first].fanout_num != 0))
            {   
                over_it = git;
                return true;
            }*/
            
            list<transient>::iterator pit;
            bdd_nodenum = 0;
            for(pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
            {
                bdd_nodenum = bdd_nodenum + bdd_nodecount(pit->p_func);
            }
            cout<<"Node: "<<git->first<<" Total Size: "<<bdd_nodenum<<" Number of Pulses: "<<graph[git->first].p_list.size()<<endl;
            
            if(bdd_nodenum > MAX_BDD_NODES)
            {
                over_it = git;
                return true;
            }
            
            /*list<transient>::iterator pit;
            
            cout<<"Node: "<<git->first<<endl;
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
    double latch_prob;
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
                s_prob.solve_prob(pit->p_func);
                latch_prob = (pit->width - (SETUP_T + HOLD_T))/CLK;
                if(latch_prob < 0)
                    latch_prob = 0;
                
                if (m_find(graph_m[git->first].r_map, pit->e_num))
                {
                    graph_m[git->first].r_map[pit->e_num] = graph_m[git->first].r_map[pit->e_num]+(s_prob.true_prob * pit->t_prob * latch_prob);
                }
                else
                {
                    graph_m[git->first].r_map[pit->e_num] = (s_prob.true_prob * pit->t_prob * latch_prob);
                }
                if(PRINT_OUT == 1)
                {
                    s <<"OutputRes"<<git->first<<"_"<<pit->e_num<<"_"<<pit->id;
                    export_vec(pit->volt_pulse, s.str());
                    f_count++;
                    s.str(string());
                }
            }
            //tp_map.clear();
        }
        else if ((graph[git->first].type != INPUT)&&((graph[git->first].fanout_num != graph_m[git->first].fanout_num) || (graph[git->first].fanout_num == 0)))
        {
            for (pit = graph[git->first].p_list.begin(); pit != graph[git->first].p_list.end(); ++pit)
            {
                s_prob.solve_prob(pit->p_func);
                pit->t_prob = s_prob.true_prob;
                temp = *pit;
                temp.p_func = bdd_true();
                tp_temp[git->first].push_back(temp);
               
                /*for (fit = graph_m[git->first].fanout.begin(); fit != graph_m[git->first].fanout.end(); ++fit)
                {
                    tp_temp[*fit].push_back(temp);
                    //cout<<"Node: "<<git->first<<" Pulse ID: "<<temp.id<<" Pulse Event: "<<temp.e_num<<endl;
                    //tp_map[*fit].push_back(temp);
                }*/
            }
            graph[git->first].p_list.clear();
            list<transient>() = graph[git->first].p_list;
        }

        if(git->first == this->max_node)
        {
            for(tmit = tp_temp.begin(); tmit != tp_temp.end(); ++tmit)
            {
                for(nit = tp_temp[tmit->first].begin(); nit != tp_temp[tmit->first].end(); ++nit)
                {
                    tp_map[tmit->first].push_back(*nit);
                    //cout<<"Node: "<<tmit->first<<" Loaded ID: "<<nit->id<<" Loaded Event: "<<nit->e_num<<endl;
                }
            }
            tp_temp.clear();
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
    bool r_flag = false;
    bool f_flag = false;
    
    double val;
    transient temp_r;
    transient temp_f;
    transient temp_i;
    gmap::iterator git;
    
    temp_r = gen_pulse(RISING, n_num, INJ_DELAY);
    temp_f = gen_pulse(FALLING, n_num, INJ_DELAY);
    
    if((temp_r.width > W_MIN))
        graph[n_num].p_list.push_back(temp_r);
    else
        r_flag == true;
    
    if((temp_f.width > W_MIN))
        graph[n_num].p_list.push_back(temp_f);
    else
        f_flag = true;
    
    //srand(10);
    val = rand() % 10 + 1;
    if((val >= floor(10*INJ_RATIO))&&(MULT_TRANS == 1))
    {
        //cout<<"MET Node: "<<n_num<<" Random Val: "<<val<<endl;
        git = graph.find(n_num);
        git++;
        temp_i = gen_pulse(RISING, git->first, INJ_DELAY);

        if(graph[git->first].type != INPUT)
        {
            if ((temp_i.width > W_MIN)&&(r_flag == false))
            {
                //cout<<"Injected Rising: "<<git->first<<endl;
                temp_i.e_num = temp_r.e_num;
                graph[git->first].p_list.push_back(temp_i);
            }

            temp_i = gen_pulse(FALLING, git->first, INJ_DELAY);
            //cout<<"Width: "<<temp_i.width<<" F Flag: "<<f_flag<<endl;

            if ((temp_i.width > W_MIN)&&(f_flag == false))
            {
                //cout<<"Injected Falling: "<<git->first<<endl;
                temp_i.e_num = temp_f.e_num;
                graph[git->first].p_list.push_back(temp_i);
            }
        }
    }    
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
        lit->p_func = gen_bdd(n_num, *lit);
            
        if(lit->p_func == const_f)
        {
            lit = inp_list.erase(lit);
        }
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
    
    //cout<<"Cur Node: "<<n_num<<" Input: "<<inp_node<<endl;
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

void bdd_sim::eval_convfunc(int n_num, transient t1, transient t2, transient& cur_p)
{
    bdd func = bdd_true();
    list<int>::iterator fit;
    
    for(fit = graph[n_num].fanin.begin(); fit != graph[n_num].fanin.end(); ++fit)
    {
        if(*fit == t1.s_node)
        {
            func = func & t1.p_func;
            cur_p.t_prob = cur_p.t_prob * t1.t_prob;
        }
        else if(*fit == t2.s_node)
        {
            func = func & t2.p_func;
            cur_p.t_prob = cur_p.t_prob * t2.t_prob;
        }
        else
        {
            if((graph[n_num].type == AND) || (graph[n_num].type == NAND))
                func = func & *graph[*fit].g_func;
            else
                func = func & bdd_not(*graph[*fit].g_func);
        }
    }
    cur_p.p_func = func;
}

void bdd_sim::bdd_optimize()
{
    int flag;
    
    gmap::iterator git;
    list<int>::iterator fo_it;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
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
                    //graph[git->first].p_list.clear();
                    //list<transient>() = graph[git->first].p_list;
                    graph[git->first].del_flag = 1;
                    *graph[git->first].g_func = bdd_true();
                    delete graph[git->first].g_func;
                    graph[git->first].func_del = true;
                    del_count++;
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