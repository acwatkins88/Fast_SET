/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   constants.h
 * Author: acwatkins
 *
 * Created on August 29, 2016, 11:55 AM
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/*
 * Monte Carlo Values
 */
#define ITR_STEP 1000000
#define MAX_INPT 20             //Max Iterative Inputs

/*
 * Enhanced Pulse Model Parameters
 */
// Granularity of Vd
#define VD_GRAN 0.0125

// Granularity of Vg
#define VG_GRAN 0.0125

// Absolute Value of Minimum Value For Characterization
#define MIN_VAL 2

// Maximum value of Vd
#define MAX_VD 2.5

// Maximum value of Vg
#define MAX_VG 2.5

// Define the Number of Steps
#define NUM_STEPS 900

// Initial Internal Node Voltage
#define INT_NODE_VOLT 0

// Constant Tau Value for Injection
#define TAU 90e-12

#define STEP_GRAN 0.5e-12

#define C_LOAD 0.25e-15
#define ST_NODE_CAP 0.25e-15

#define PI 3.14159265359

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
#define VDD 1.05

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
#define INJ_SIM 4

/*
 * Test Values for Charge Injection
 */
#define S_NODE 4
#define CHARGE 30e-15

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
#define CUR_SIM 4

#endif /* CONSTANTS_H */

