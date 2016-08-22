/* 
 * File:   globals.h
 * Author: adam
 *
 * Created on May 20, 2014, 12:26 PM
 */

#ifndef GLOBALS_H
#define	GLOBALS_H

/*
 * Library Definitions
 */
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>
#include <iomanip>
#include <vector>
#include <cmath>
#include <vector>
#include <list>
#include <time.h>
#include <map>
#include <bdd.h>
#include <ctime>
#include <utility>
#include <algorithm>

/*
 * User File Definitions
 */
#include "node.h"
#include "gates.h"

typedef map <int, node> gmap;
typedef map <int, double*> prmap;
typedef map <int, double> smap;
typedef map <int, int> n_count;

/*
 * Monte Carlo Values
 */
#define ITR_STEP 1000000
#define MAX_INPT 20             //Max Iterative Inputs


/*
 * Enhanced Pulse Model - Number of Points
 */
#define PT_NUM 1500

/*
 * Enhanced Pulse Model Parameters
 */
// Granularity of Vd
#define VD_GRAN 0.0125

// Granularity of Vg
#define VG_GRAN 0.0125

// Absolute Value of Minimum Value For Characterization
#define MIN_VAL 1.5

// Maximum value of Vd
#define MAX_VAL 1.5

// Maximum value of Vg
#define MAX_VG 1.5

/*
 * Additional Circuit Parameters
 */

// Logic Values
#define TRUE 1
#define FALSE 0

// PMOS/NMOS Definitions
#define PMOS 0
#define NMOS 1

// Simulation Constants
#define MAXBUF 256

// Gate Representations
#define INPUT 0
#define AND 1
#define NAND 2
#define OR 3
#define NOR 4
#define BUF 5
#define NOT 6 
#define NA 7

// Pulse Types
#define LHL 1
#define HLH 2

// Flag for Peak or Arrival Determination
#define ARR 0
#define PEAK 1

// Process Parameters
#define P_THRESH .6
#define N_THRESH .68

// Max Delay LUT Time
#define M_RTIME 400e-12

// Rising or Falling
#define RISING 0
#define FALLING 1

// Delay Measure Point
#define DELAYPT .5

// Vdd Value
#define VDD 1

// Pin Capacitance
#define PINCAP .32e-15

// Temporary Gate Delay
#define DELAY 1
#define MAX_WIDTH 4

// Simulation Types
#define BDD_SIM 0 
#define SIGPROB_SIM 1
#define ITR_SIM 2
#define CCM_SIM 3

/*
 * Temp Test Values
 */
#define T_PROB .5
#define F_PROB .5

#define REORDER_TH 1000

// Set to 1 to simulate output cones
#define CONE_SIM 0

// Set initial partition ratio
#define PART_RATIO 0.5

// Maximum BDD node count
#define MAX_BDD_NODES 5000000

// Set a Minimum Number of Gates
#define MIN_GATE_NUM 10

#define NUM_INJ_PULSES 10

/*
 * 0 - No pulse
 * 1 - Rising pulse
 * 2 - Falling pulse
 */
#define pulse_type HLH

/*
 * 0 - BDD_SIM
 * 1 - Not Used
 * 2 - ITR_SIM
 */
#define CUR_SIM 0

#endif	/* GLOBALS_H */

