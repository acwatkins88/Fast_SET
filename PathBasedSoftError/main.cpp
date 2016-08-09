/* 
 * File:   main.cpp
 * Author: adam
 *
 * Created on January 15, 2014, 3:53 PM
 */

#include "parser.h"
//#include "sigprob.h"
#include "transsim.h"
#include "bddprob.h"

using namespace std;

/*
 * Partitioning Completed 
 * Tasks -
 * Implement adaptive partitioning
 * Ensure that the partitioning algorithm can take multiple passes
 * Try to implement the XOR function
 */

/*
 * graph stores the current circuit
 * circuit stores information on the inputs and outputs
 */
gmap graph;

/*
 * Master graph container - hack for bdd cone simulation
 */
gmap graph_m;
circuitstruct circuit;

/*
 * Tables for Transistor Current Values
 */
vector<vector<double> > nmos_cur;
vector<vector<double> > pmos_cur;

/*
 * Tables for Transistor Miller Capacitances
 */
vector<vector<double> > nmos_miller;
vector<vector<double> > pmos_miller;

int var_count(0);

// argv[1] - simulation file name

int main(int argc, char** argv) 
{
    //int max_partn;
    //gmap graphn;
    bdd_init(10000000, 50000);
    
    ifstream file_h;
    parser p_file;
    bdd_sim b_sim(BDD_SIM);
    itr_sim i_sim(ITR_SIM);
    bdd_prob e_bdd;
    
    p_file.build_graph(argv[1]);
    
    graph_m = graph;
    
    //max_partn = b_sim.label_cone();
    //graphn = b_sim.extract_circuit(0);
    unsigned t0=clock();
    if(CUR_SIM == BDD_SIM)
        b_sim.sim();
    else if(CUR_SIM == ITR_SIM)
        i_sim.sim();
    unsigned t1=clock()-t0;
    
    gmap::iterator g_it;
    list<transient>::iterator t_it;
    list<int>::iterator p_it;
    map<int, double>::iterator r_it;
    
    if(CUR_SIM == BDD_SIM)
    {
        double avg = 0;
        int num_events = 0;
        for(g_it = graph_m.begin(); g_it != graph_m.end(); ++g_it)
        {
            //if(out_find(g_it->first)&&(g_it->first == 880))
            if(out_find(g_it->first))
            {
                cout<<"Node: "<<g_it->first<<endl;
                cout<<"Type: "<<graph_m[g_it->first].type<<endl;
                cout<<"Pulses: "<<endl;;
                for(r_it = graph_m[g_it->first].r_map.begin(); r_it != graph_m[g_it->first].r_map.end(); ++r_it)
                {
                    cout<<"Event: "<<r_it->first<<endl;
                    cout<<"Probability: "<<graph_m[g_it->first].r_map[r_it->first]<<endl;
                    if(graph_m[g_it->first].r_map[r_it->first] > 1)
                    {
                        avg = avg + 1;
                        num_events++;
                    }
                    else
                    {
                        avg = avg + graph_m[g_it->first].r_map[r_it->first];
                        num_events++;
                    }
                }
                cout<<"Average: "<<avg/(double) num_events<<endl;
                avg = 0;
                num_events = 0;
            }
        }
        cout<<"Allocs: "<<b_sim.alloc_count<<endl;
        cout<<"Deletes: "<<b_sim.del_count<<endl;
    }
    else if(CUR_SIM == ITR_SIM)
    {
        for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
        {
            if(out_find(g_it->first))
            {
                cout<<"Node: "<<g_it->first<<endl;
                cout<<"Type: "<<graph[g_it->first].type<<endl;
                cout<<"Pulses: "<<endl;;
                for(r_it = graph[g_it->first].r_map.begin(); r_it != graph[g_it->first].r_map.end(); ++r_it)
                {
                    cout<<"Event: "<<r_it->first<<endl;
                    cout<<"Probability: "<<graph[g_it->first].r_map[r_it->first]<<endl;
                }
            }
        }
    }
    
    /*for(g_it = graph.begin(); g_it != graph.end(); ++g_it)
    {
        cout<<"Node: "<<g_it->first<<endl;
        cout<<"Type: "<<graph[g_it->first].type<<endl;
        cout<<"Val: "<<graph[g_it->first].val<<endl;
        cout<<"Partition: ";
        for(p_it = g_it->second.part.begin(); p_it != g_it->second.part.end(); ++p_it)
            cout<<*p_it<<" ";
        cout<<endl<<endl;;
        /*for (t_it = graph[g_it->first].p_list.begin(); t_it != graph[g_it->first].p_list.end(); ++t_it)
        {
            cout<<endl;
            cout << "ID: " << t_it->id<<endl;
            cout << "Event: " << t_it->e_num << endl;
            cout << "Start Time: " << t_it->st_time << endl;
            cout << "End Time: " << t_it->end_time << endl;
            cout << "Width: " << t_it->width << endl;
            e_bdd.solve_prob(t_it->p_func);
            cout<<"Probability: "<<e_bdd.true_prob<<endl;
            cout<<endl;
        }*/
    //}
    
    cout<<"Simulation Time: "<<t1/double(CLOCKS_PER_SEC)<<" Seconds"<<endl;
    
    /*
     * Parse Tables
     */
    /*file_h.open("Tables/nmoscur.out", ios::in);
    parse_table(file_h, nmos_cur);
    file_h.close();
    
    file_h.open("Tables/pmoscur.out", ios::in);
    parse_table(file_h, pmos_cur);
    file_h.close();
    */
    /*
     * Parse Tables
     */
    /*file_h.open("Tables/nmosmiller.out", ios::in);
    parse_table(file_h, nmos_miller);
    file_h.close();
    
    file_h.open("Tables/pmosmiller.out", ios::in);
    parse_table(file_h, pmos_miller);
    file_h.close();
    
    cout<<"Table Information: "<<endl;
    cout<<pmos_miller[40][50]<<endl;
    */
    /*
     * Begin Simulation Routines
     */
    
    bdd_done();
    
    return 0;
}