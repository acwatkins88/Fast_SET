#ifndef PARSER_H
#define PARSER_H

#include "globals.h"

using namespace std;

/*
 * Variables
 */
extern gmap graph;
extern circuitstruct circuit;

/*
 * Class to convert from conv to inp
 */
template<class inp, class conv>
inp conval(conv out) {
    stringstream convert;
    inp result;

    convert << out;
    convert >> result;
    return result;
}

/*
 * Additional Parsing Functions
 */
void parse_table(char* filename, vector<vector<double> > &out_table);

/*
 * Class to parse input files
 */
class parser 
{
private:
    int find_nodepos(string buff);
    int gate_nodeid(string buff);
    string extract_string(string inpstr);
    void build_io(string fanin, int nodeid);
    bool checklist(list<int> , int );
public:
    void build_graph(char* filename);
    //int parse_hgraph(char* filename);
};
#endif
