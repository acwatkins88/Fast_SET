#include "transsim.h"

using namespace std;

void ccm_sim :: sim()
{
    int max_level;
    double temp_prob;
    list<int>::iterator lit;
    list<int>::iterator t_lit;
    list<int>::iterator in_it;
    list<int>::iterator cin1_it;
    list<int>::iterator cin2_it;
    
    map<int, double>::iterator cit;
    
    list<int> temp_l;
    
    max_level = level_circuit();
    
    /*for(mit = level_map.begin(); mit != level_map.end(); ++mit)
    {
        cout<<"Level: "<<mit->first<<endl;
        cout<<"Gates: ";
        for(lit = level_map[mit->first].begin(); lit != level_map[mit->first].end(); ++lit)
            cout<<*lit<<" ";
        cout<<endl;
    }*/
    
    for(int i = 0; i <= max_level; i++)
    {
        temp_l  = level_map[i];
        for(lit = level_map[i].begin(); lit != level_map[i].end(); ++lit)
        {
            if(graph[*lit].type == INPUT)
            {
                graph[*lit].prob = 0.5;
                
                if(graph[*lit].fanout_num > 1)
                    graph[*lit].cor_coef[*lit] = 1/graph[*lit].prob;
                
                temp_l.pop_front();
                for(t_lit = temp_l.begin(); t_lit != temp_l.end(); ++t_lit)
                {
                    graph[*lit].cor_coef[*t_lit] = 1;
                }
            }
            if(graph[*lit].type == NAND)
            {
                temp_prob = 1;
                for(in_it = graph[*lit].fanin.begin(); in_it != graph[*lit].fanin.end(); ++in_it)
                {
                    temp_prob = temp_prob*graph[*lit].prob;
                }
                
                for(t_lit = temp_l.begin(); t_lit != temp_l.end(); ++t_lit)
                {
                    for(cin1_it = graph[*lit].fanin.begin(); cin1_it != graph[*lit].fanin.end(); ++cin1_it)
                    {
                        for(cin2_it = graph[*t_lit].fanin.begin(); cin2_it != graph[*t_lit].fanin.end(); ++cin2_it)
                        {
                            
                        }
                    }
                }
                graph[*lit].prob = 1 - temp_prob;
            }
        }
    }
}

/*
 * Assign Levels to the Circuit
 */
int ccm_sim::level_circuit()
{
    int max_level;
    list<int>::iterator lit;
    gmap::iterator git;
    
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(graph[git->first].type == INPUT)
        {
            graph[git->first].level = 0;
            level_map[0].push_back(git->first);
        }
        else if(graph[git->first].type == BUF || graph[git->first].type == NOT)
        {
            for(lit = graph[git->first].fanin.begin(); lit != graph[git->first].fanin.end(); ++lit)
            {
                graph[git->first].level = graph[*lit].level;
                level_map[graph[*lit].level].push_back(git->first);
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
            level_map[max_level].push_back(git->first);
        }
        else
            cout<<"Invalid Node Type\n";
    }
    return max_level;
}

void ccm_sim::end_sim()
{
    
}