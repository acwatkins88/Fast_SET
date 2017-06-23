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
#include <cstdlib>

/*
 * User File Definitions
 */
#include "node.h"
#include "gates.h"

typedef map <int, node> gmap;
typedef map <int, double*> prmap;
typedef map <int, double> smap;
typedef map <int, int> n_count;

#endif	/* GLOBALS_H */

