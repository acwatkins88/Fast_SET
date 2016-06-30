/* 
 * File:   transsim.h
 * Author: adam
 *
 * Created on November 3, 2014, 1:34 PM
 */

#ifndef TRANSSIM_H
#define	TRANSSIM_H

#include "globals.h"
#include "bddprob.h"
#include "sigprob.h"
#include "parser.h"

extern gmap graph;
extern gmap graph_m;
extern circuitstruct circuit;

extern vector<vector<double> > nmos_cur;
extern vector<vector<double> > pmos_cur;

extern vector<vector<double> > nmos_miller;
extern vector<vector<double> > pmos_miller;


void part_sim(gmap , circuitstruct );
bool out_find(int );

class gen_sim
{
public:
    gen_sim(int);
    virtual ~gen_sim(){};
    
    //gmap graph;
    //circuitstruct circuit;
    
    virtual void sim() = 0;
    virtual void end_sim() = 0;
//private:
protected:
    // Maximum pin size
    int max_pins;
    int total_size;
    int max_size;
    int size_part1;
    int size_part2;
    
    // Data structures for partitioning
    gmap inp_g;
    // cells connected to a net
    map<int, list<int> > net_l;
    
    // nets connected to a cell
    map<int, list<int> > cell_l;
    
    map<int, list<int> > bucket;
    list<int> locked_cells;
//protected:
    int sim_type;           // Type of Simulation
    int event_n;            // Overall Event Number
    int id_n;               // Pulse ID Number
    
    // General Simulation Functions
    //enh_trans gen_pulse(int, double);
    
    transient gen_pulse(int, int );
    
    // Inject Pulse
    vector<double> inj_NAND(int, double);
    
    // Gate injection functions
    double ind_current(int , double , double );
    double ind_miller(int , double , double );
    
    void proc_pulse(int);
    double calc_delay();
    
    // Functions to Determine Convergence
    void conv_check(int); 
    int det_category(transient, transient);
    void eval_cat0(map<int, list<int> >, list<transient>, int, int, transient, transient);
    void eval_cat1to6(map<int, list<int> >, list<transient>, int, int, transient, transient);
    void eval_cat78(map<int, list<int> > , list<transient> , int , int , transient , transient );
    transient calc_convtrans14(int, int, transient, transient);
    transient calc_convtrans56(int, int, transient, transient);
    
    pair<transient, transient> cat7case23(int , transient , transient );
    transient cat7case4(int , transient , transient );
    pair<transient, transient> cat8case14(int , transient , transient );
    transient cat8case2(int , transient , transient );
    
    // Supporting Routines for Convergence Calculation
    void add_toconvmap(map<int, list<int> >, list<transient>, transient, int, int, int);
    bool is_pulse(list<prev_p> , int );
    bool map_check(map<int, list<int> > , int , int );
    bool list_check(list<int> , int );
    bool find_element(map<int, list<int> > , int );
    
    // Inline Supporting Functions
    inline bool is_ovr(double pst1, double pet1, double val) 
    { if ((val <= pet1) && (val >= pst1)){return true;} else{return false;}}

    inline int is_inv(int type)
    { if (type == NAND || type == NOR || type == NOT){return 1;} else{return 0;}}
    
    // Check if element is in map
    bool m_find(map<int, double> , int );
    
    // Check if an element is in list
    bool l_find(list<int> , int );
    
    // Determine if a Node is an Output
    //bool out_find(int );
    
    int label_cone();
    gmap extract_circuit(int );
    
    // Virtual Functions for prob assignment
    virtual void set_propfunc(int , int , transient , transient );
    virtual void set_convprop(int , transient& , transient , transient );
    
    // Fuctions for partitioning a circuit
    void part_circuit(gmap );
    void load_pstruct();
    void calc_csize();
    void init_part();
    void init_gain();
    void move_cells();
    
    // Supporting Functions for partitioning
    bool check_balance(int );
    int calc_maxgain();
    bool is_cell_free(int );
    bool is_bucket_empty();
    
    
    // Test Functions
    void print_pstruct();
};

/*
 * Class for the BDD Based Simulation
 */
class bdd_sim : public gen_sim
{
public:
    void sim();
    void end_sim();
    
    // Local graph - represented as a partition
    //gmap graphn;
    
    bdd_sim(int t):gen_sim(t){}; 
    ~bdd_sim(){};
    
private:
    void gen_sensf(int);
    void bdd_genp(int n_num);
    bdd gen_bdd(int, transient);
    bdd prop_bdd(int, int , transient);
    void set_propfunc(int , int , transient , transient );
    bdd get_bdd(list<transient>, list<prev_p>);

    // Functions for Convergence Calculation
    void set_convprop(int , transient& , transient , transient );
    bdd get_convbdd(int, transient, transient);
    
    void sim_graph(gmap& );
    
    void bdd_optimize();
};

/*
 * Class for Signal Probability Based Simulation
 */
class sigprob_sim : public gen_sim
{
public:
    sigprob_sim(int t):gen_sim(t){};
    void sim();
    void end_sim();
    
private:
    void gen_probp(int );
    double calc_genprob(int , int );
    bool check_gate(int , int );
    void set_propfunc(int , int , transient , transient );
    
    // Functions for Convergence Calculation
    void set_convprop(int , transient& , transient , transient );
    double get_prob(list<transient> , list<prev_p> );
};

/*
 * Class for Iterative Monte Carlo Simulation
 */
class itr_sim : public gen_sim
{
public:
    itr_sim(int t):gen_sim(t){};
    ~itr_sim(){};
    void sim();
    void end_sim();
private:
    void apply_inpt(int* );
    void eval_gval(int );
    void gen_p(int );
    void add_result(int );
    void gen_itrpat(int* , int);
    void gen_pat(int* );
    
    // Declarations of Virtual Functions
    void set_propfunc(int, int, transient, transient );
    void set_convprop(int , transient& , transient , transient );
    
    inline int inv_val(int val)
    {if(val == 1){return 0;} else{return 1;}}
    
};

#endif	/* TRANSSIM_H */

