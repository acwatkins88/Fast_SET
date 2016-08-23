#ifndef NODE_H
#define NODE_H

#include "config.h"
#include <cmath>
#include <vector>
#include <list>
#include <iostream>
#include <map>
#include <cstdlib>
#include <bdd.h>

using namespace std;

struct prev_p
{
    int p_gate;
    int p_pulse;
};

/*
 * Class to store transient pulses
 * peaktime - absolute peak time
 * peaktime1 - peaktime of first transition - used for transient convergence
 * peaktime2 - peaktime of second transition - used for transient convergence
 * peak - set to VDD in code, not the real peak
 * realpeak - Calculated peak
 */
class transient 
{
public:
    transient();
    ~transient();
    
    int type;
    int id;
    int s_node;
    double width;                 
    bdd p_func; 
    double st_time;
    double end_time;
    int e_num;                 // Event Number
    double delay_r;
    double delay_f;
    double delay;              // Temporary Delay
    list<prev_p> prev_pulse;
    
    double t_prob;             // Variable to Store Probability 
    
    /*
     * Not Used Functions
     */	
    double r_slope;
    double f_slope;
    double peak;
    double r_time;
    double f_time;
    double eval_volt(double time, int rf, double sttime);
    double eval_time(double volt, int rf, double sttime);
     
};

/*
 * Class for enhanced transient pulse model
 */
class enh_trans
{
public:
    int type;
    int e_num;
    int id;
    
    double t_prob;
    
    vector<int> volt_pulse;    
};

/*
 * Class to store node information
 */
class node 
{
public:
    // General Node Elements
    int type;                       // Gate type
    int fanout_num;                 // Number of fanouts
    int fanin_num;                  // Number of fanins
    int val;                        // gate output value
    double prob;                    // gate signal probability
    bool gen_flag;                  // Flag denoting that pulses were generated
    
    int proc_flag;                  // denotes when node is processed
    int del_flag;                   // denotes when node transients are deleted
    bool func_del;
    
    int static_part;                // Partition Number for for topological simulation
    
    // Partitioning
    std::list<int> part;            // partition number
    int c_size;                     // size of cell for partitioning
    int pins;                       // number of pins in the cell
    int gain;                       // gain of cell for partitioning
    int b_part_num;                 // partition number of Fiduccia method
    bool free;                      // Flag to determine if cell is free
    
    // Temporary Width
    int r_width;                    // Rising Width
    int f_width;                    // Falling Width
    
    bdd* g_func;                    // Gate Sensitization Function
    std::list<transient> p_list;    // Contains all pulses at a gate
    std::list<enh_trans> eh_plist;  // Pulse list for enhanced pulse model
    bool pulse;                     // temp flag for a pulse
    //double delay;                 // Gate delay
    
    node();                         // constructor
    ~node();                        // deconstructor
    std::list<int> fanin;           // fanin list
    std::list<int> fanout;          // fanout list
   
    // Capacitances of the node
    double inpt_cap;
    double inter_cap;
    double load_cap;
    
    // Data structure for result calculations (only for circuit outputs)
    map<int, double> r_map;
    map<int, bdd> bdd_map;
    map<int, double> p_prob_map;
    
    // CCM Variables
    int level;
    map<int, double> cor_coef; 
};

/*
 * Class to store full circuit information
 */
struct circuitstruct 
{
    int inpnum;
    int outnum;
    int maxnodeid;
    std::list<int> inputs;
    std::list<int> outputs;
};

#endif
