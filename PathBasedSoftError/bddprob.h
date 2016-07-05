/* 
 * File:   bdd.h
 * Author: adam
 *
 * Created on March 19, 2015, 3:30 PM
 */

#ifndef BDDPROB_H
#define	BDDPROB_H

#include "globals.h"

extern gmap graph;
extern circuitstruct circuit;
extern int var_count;

/*
 * Function for Probabiltiy calculation on bdd
 * 1 - true branch
 * 0 - false branch
 */

class bdd_prob
{
public:
    double true_prob;
    double false_prob;
    prmap prob_map;
    smap sum_map;
    n_count inp_num;
    n_count num_vis;
    smap inp_map;    // map to store initial input probabilities
    
    void set_inpprob();
    void solve_prob(bdd );
    
    void del_probmap();
    
    bdd_prob();
    ~bdd_prob();
    
private:
    void init_sum(bdd );
    void init_prob(bdd );
    void eval_bddprob(bdd ); 
    bool is_inprobmap(int );
};

inline bdd new_var(){bdd x; bdd_setvarnum(var_count+1); x = bdd_ithvar(var_count); var_count++; return x;}

#endif	/* BDDPROB_H */

