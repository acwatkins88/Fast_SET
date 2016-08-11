#include "transsim.h"

using namespace std;

void ccm_sim :: sim()
{
    level_circuit();
}

/*
 * Assign Levels to the Circuit
 */
void ccm_sim::level_circuit()
{
    int max_level;
    list<int>::iterator lit;
    gmap::iterator git;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(graph[git->first].type == INPUT)
        {
            graph[git->first].level = 0;
        }
        else if(graph[git->first].type == BUF || graph[git->first].type == NOT)
        {
            for(lit = graph[git->first].fanin.begin(); lit != graph[git->first].fanin.end(); ++lit)
            {
                graph[git->first].level = graph[*lit].level;
            }
        }
        else if(graph[git->first].type != NA)
        {
            max_level = 0;
            
            for(lit = graph[git->first].fanin.begin(); lit != graph[git->first].fanin.end(); ++lit)
            {
                if(graph[*lit].level > max_level)
                    max_level = graph[*lit].level;
            }
            max_level++;
            graph[git->first].level = max_level;
        }
        else
            cout<<"Invalid Node Type\n";
    }
}

void ccm_sim::end_sim()
{
    
}