#include "parser.h"

using namespace std;

/*
 * Parse benchmark file - Capacitor file only implemented for NAND gates
 * Moved to constructor
 * Capacitor Values are not characterized for more than 2 input gates
 */
void parser::build_graph(char* filename)
{
    int maxnodeid = 0;
    int pos, nodepos;
    int inpnum = 0;
    int outnum = 0;

    string buff, tempstring, nodestring;

    ifstream graphfile;
    graphfile.open(filename, ios::in);
    
    // Find Maximum node id
    maxnodeid = 0;
    if (graphfile.is_open())
    {
        while (graphfile.good())
        {
            getline(graphfile, buff);
            pos = buff.find('#');
            if (pos == -1)
            {
                if (!buff.empty())
                {
                    // Initialize each node
                    // Node is an Input
                    if ((int) buff.find("INPUT") != -1)
                    {
                        nodepos = find_nodepos(buff);
                        graph[nodepos].type = INPUT;
                        circuit.inputs.push_back(nodepos);
                        inpnum++;
                        circuit.inpnum = inpnum;
                    }// Node is an Output
                    else if ((int) buff.find("OUTPUT") != -1)
                    {
                        nodepos = find_nodepos(buff);
                        if (nodepos > maxnodeid)
                        {
                            maxnodeid = nodepos;
                            circuit.maxnodeid = maxnodeid;
                        }
                        circuit.outputs.push_back(nodepos);
                        outnum++;
                        circuit.outnum = outnum;
                    }// Node is AND Gate
                    else if ((int) buff.find("AND") != -1 && (int) buff.find("NAND") == -1)
                    {
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = AND;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    }// Node is NAND Gate
                    else if ((int) buff.find("NAND") != -1)
                    {
                        nodestring = "NAND";
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = NAND;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    }// Node is OR Gate
                    else if ((int) buff.find("OR") != -1 && (int) buff.find("NOR") == -1)
                    {
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = OR;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    }// Node is NOR Gate
                    else if ((int) buff.find("NOR") != -1)
                    {
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = NOR;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    }// Node is BUFF
                    else if ((int) buff.find("BUFF") != -1)
                    {
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = BUF;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    }// Node is NOT
                    else if ((int) buff.find("NOT") != -1)
                    {
                        nodepos = gate_nodeid(buff);
                        graph[nodepos].type = NOT;
                        tempstring = extract_string(buff);
                        build_io(tempstring, nodepos);
                        tempstring.clear();
                    } else
                    {
                        cout << "Node type could not be found\n";
                    }
                }
            }
        }
    } else
        cout << "Benchmark file could not be opened." << endl;
    graphfile.close();
}

/*
 * Parsing Routine for inputs 
 */
int parser::find_nodepos(string buff)
{
    int nodepos;
    string nodeval;

    nodeval = extract_string(buff);
    nodepos = conval<int, string>(nodeval);
    return nodepos;
}

/*
 * Extract normal gate node id
 */
int parser::gate_nodeid(string buff)
{
    int nodepos;
    int pos;
    string nodeval;

    pos = buff.find("=");
    nodeval = buff.substr(0, pos);
    nodepos = conval<int, string>(nodeval);
    return nodepos;
}

/*
 * Extract string from parenthesis for inputs
 */
string parser::extract_string(string inpstr)
{
    int start, end, length;
    string nodeval;

    start = inpstr.find('(');
    end = inpstr.find(')');
    length = end - start;
    nodeval = inpstr.substr(start + 1, length - 1);
    return nodeval;
}

/*
 * Load I/O into graph data structure
 */
void parser::build_io(string fanin, int nodeid)
{
    int commapos = 0;
    int faninnode;
    string tempstring;

    while (commapos != -1)
    {
        commapos = fanin.find(',');
        tempstring = fanin.substr(0, commapos);
        faninnode = conval<int, string>(tempstring);
        graph[nodeid].fanin.push_back(faninnode);
        graph[nodeid].fanin_num++;
        graph[faninnode].fanout.push_back(nodeid);
        graph[faninnode].fanout_num++;
        fanin = fanin.erase(0, commapos + 1);
        tempstring.clear();
    }
}

/*
 * Return the number of partitions
 */

/*int parser::parse_hgraph(char* filename)
{
    ifstream hgfile;
    string buff;
    gmap::iterator g_it;
    int partcount = 0;
    int tempval;
    list<int> p_list;
    
        hgfile.open(filename, ios::in);
    
    g_it = graph.begin();
    
    if (hgfile.is_open())
    {
        while (hgfile.good())
        {
            getline(hgfile, buff);
            tempval = conval<int, string>(buff);
            graph[g_it->first].part = tempval;
            
            if(checklist(p_list, tempval) == false)
            {
                p_list.push_back(tempval);
                partcount++;
            }
        }
    }
    else
    {
        cout<<"Unable to Open Partition File"<<endl;
    }
    return partcount;
}*/

bool parser::checklist(list<int> inplist, int val)
{
    list<int>::iterator l_it;

    for (l_it = inplist.begin(); l_it != inplist.end(); l_it++)
    {
        if (*l_it == val)
            return true;
    }
    return false;
}

/*
 * Code to Import Current and Value Tables
 */
void parse_table(ifstream &inp_file, vector<vector<double> > &out_table)
{
    string buff;
    string num_buff;
    stringstream t_ss;
    vector<double> temp_vec;

    while (getline(inp_file, buff))
    {
        t_ss << buff;
        while (t_ss >> num_buff)
        {
            temp_vec.push_back(conval<double, string>(num_buff));
        }
        out_table.push_back(temp_vec);
        temp_vec.clear();
        t_ss.clear();
    }
}