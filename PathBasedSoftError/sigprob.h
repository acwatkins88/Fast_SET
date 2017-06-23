/* 
 * File:   sigprob.h
 * Author: adam
 *
 * Created on May 19, 2014, 4:20 PM
 */

/*
 * Code to calculate line signal probability
 */

#ifndef SIGPROB_H
#define	SIGPROB_H

#include "globals.h"
//#include "gates.h"

//extern gmap graph;
//extern circuitstruct circuit;

// Calculate Signal Probabilities
void calc_sigprob();
void gen_pat(int* );
void gen_itrpat(int* , int);
void prob_sim();

// General Circuit Simulation Functions
void load_inputs(int* );
int eval_NOT(int );
int eval_AND(gmap::iterator );
int eval_OR(gmap::iterator );

#endif	/* SIGPROB_H */

