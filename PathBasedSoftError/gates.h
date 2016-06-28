#ifndef GATES_H
#define GATES_H

/*//////////////////////////////////////////////////
 * Normal Logic Tables
 * 0 - 0
 * 1 - 1
 //////////////////////////////////////////////////*/
static const int AND_VAL_S[2][2] = {
    {0, 0},
    {0, 1}
};

static const int OR_VAL_S[2][2] = {
    {0, 1},
    {1, 1}
};


/*/////////////////////////////////////////////////

 Defines all tables logic simulation
 0 - 0
 1 - 1
 2 - low-high-low pulse - Type 1 Pulse
 3 - high-low-high pulse - Type 2 Pulse
 4 - convergence
 5 - unknown

 0 1 rpulse fpulse rising falling unknown
 //////////////////////////////////////////////////*/


static const int AND_VAL[6][6] = {
    { 0, 0, 0, 0, 0, 5},
    { 0, 1, 2, 3, 4, 5},
    { 0, 2, 4, 4, 4, 5},
    { 0, 3, 4, 4, 4, 5},
    { 0, 4, 4, 4, 4, 5},
    { 5, 5, 5, 5, 5, 5}
};

static const int OR_VAL[6][6] = {
    { 0, 1, 2, 3, 4, 5},
    { 1, 1, 1, 1, 1, 5},
    { 2, 1, 4, 4, 4, 5},
    { 3, 1, 4, 4, 4, 5},
    { 4, 1, 4, 4, 4, 5},
    { 5, 5, 5, 5, 5, 5}
};
#endif
