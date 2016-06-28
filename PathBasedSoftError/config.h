#ifndef CONFIG_H
#define CONFIG_H

// Logic Values
#define TRUE 1
#define FALSE 0

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
#define PART_SIM 3

/*
 * Temp Test Values
 */
#define T_PROB .5
#define F_PROB .5

#define REORDER_TH 1000

#endif
