#include "bddprob.h"

bdd_prob::bdd_prob()
{
    
}

bdd_prob::~bdd_prob()
{
    
}

void bdd_prob::solve_prob(bdd x)
{
    true_prob = 0;
    false_prob = 0;
    prob_map.clear();
    sum_map.clear();
    inp_num.clear();
    num_vis.clear();
    init_map.clear();
    v_nodes.clear();
    
    count1 = 0;
    count2 = 0;
    count3 = 0;
    
    //cout<<"Begin\n";
    //init_sum(x); 
    //cout<<"Initialized\n";
    init_prob(x); 
    //cout<<"Probmap Created\n";
    eval_bddprob(x);
    //cout<<"Evalulation Complete\n";
    del_probmap();
    //cout<<"Probmap Deleted\n";
    
    //cout<<"Ideal Node Number: "<<bdd_nodecount(x)<<" New Count: "<<count3<<" Init Counted: "<<count1<<" Eval Counted: "<<count2<<endl;
}

void bdd_prob::init_sum(bdd x)
{
    int var;
    
    bdd const_t = bdd_true();
    bdd const_f = bdd_false();
    bdd t_b;
    bdd f_b;
    
    var = x.id();
    
    if(x != const_t && x != const_f)
    {
        count3++;
        //cout<<"Number: "<<count3<<endl;
        
        inp_num[var] = 0;
        
        init_map[var] = false;
        v_nodes.push_back(var);
        
        t_b = bdd_high(x);
        f_b = bdd_low(x);
        
        //if((t_b != const_t)&&(t_b != const_f)&&(!check_list(v_nodes, t_b.id())))
        if((t_b != const_t)&&(t_b != const_f))
        {
            //cout<<"True ID: "<<t_b.id()<<endl;
            init_sum(t_b);
        }
        
        //if((f_b != const_t)&&(f_b != const_f)&&(!check_list(v_nodes, f_b.id())))
        if((f_b != const_t)&&(f_b != const_f))
        {
            //cout<<"False ID: "<<f_b.id()<<endl;
            init_sum(f_b);
        }
    }
}

bool bdd_prob::is_inprobmap(int var)
{
    prmap::iterator p_it;
    
    for(p_it = prob_map.begin(); p_it != prob_map.end(); ++p_it)
    {
        if(p_it->first == var)
            return true;
    }
    return false;
}

/*
 * Initialize the Probability array by assigning initial probabilities
 * Initialize the sum array to zero
 * Modify so Initial Probabilities are user defined
 */
void bdd_prob::init_prob(bdd x)
{
    int var;
    int inp_var;
    
    bdd const_t = bdd_true();
    bdd const_f = bdd_false();
    bdd t_b;
    bdd f_b;
    
    true_prob = 0;
    false_prob = 0;  
    
    if(x != const_t && x != const_f)
    {
        count1++;
        var = x.id();
        inp_var = bdd_var(x);
        
        //cout<<"Initialize: "<<var<<endl;

        if(init_map[var] == false)
        {
            prob_map[var] = new double[2];
            init_map[var] = true;
        }

        prob_map[var][0] = (1 - inp_map[inp_var]);  
        prob_map[var][1] = inp_map[inp_var];
        
        //cout<<"Var Num: "<<inp_var<<" Node Prob: "<<inp_map[inp_var]<<endl;

        sum_map[var] = 0;
        
        inp_num[var]++;
        num_vis[var] = 0;
        
        t_b = bdd_high(x);
        f_b = bdd_low(x);
        
        if((inp_num[var] == 1))
        {
            init_prob(t_b);
            init_prob(f_b);
        }
    }
}

/*
 * Recursively traverse the bdd structure and assign probabilities
 * prob_map stores the probability of 0 or 1
 * sum_map stores the sum of probabilities on incoming edges
 */

void bdd_prob::eval_bddprob(bdd x)
{
    int var;
    int t_flag = 0;
    int f_flag = 0;
    int var_t, var_f;
    
    bdd const_t = bdd_true();
    bdd const_f = bdd_false();
    
    if(x != const_t && x != const_f)
    {
        count2++;
        
        var = x.id();
        
        bdd t = bdd_high(x);
        bdd f = bdd_low(x);
        
        if(t == const_t)
        {
            if(sum_map[var] == 0)
                sum_map[var] = 1;
            prob_map[var][1] = sum_map[var]*prob_map[var][1];
            true_prob = true_prob + prob_map[var][1];
            t_flag = 1;
        }
        if(t == const_f)
        {
            if(sum_map[var] == 0)
                sum_map[var] = 1;
            prob_map[var][1] = sum_map[var]*prob_map[var][1];
            false_prob = false_prob + prob_map[var][1];
            t_flag = 1;
        }
        if(f == const_t)
        {
            if(sum_map[var] == 0)
                sum_map[var] = 1;
            prob_map[var][0] = sum_map[var]*prob_map[var][0];
            true_prob = true_prob + prob_map[var][0];
            f_flag = 1;
        }
        if(f == const_f)
        {
            if(sum_map[var] == 0)
                sum_map[var] = 1;
            prob_map[var][0] = sum_map[var]*prob_map[var][0];
            false_prob = false_prob + prob_map[var][0];
            f_flag = 1;
        }
        if(t_flag == 0 && f_flag == 1)
        {
            var_t = t.id();
            
            if(sum_map[var] == 0)
            {
                sum_map[var_t] = prob_map[var][1];
                
                num_vis[var_t]++;
                if(num_vis[var_t] == inp_num[var_t])
                    eval_bddprob(t);
            }
            else
            {
                prob_map[var][1] = sum_map[var]*prob_map[var][1];
                sum_map[var_t] = sum_map[var_t] + prob_map[var][1];
                
                num_vis[var_t]++;
                if(num_vis[var_t] == inp_num[var_t])
                    eval_bddprob(t);
                    
            }
        }
        if(t_flag == 1 && f_flag == 0)
        {
            var_f = f.id();
            
            if(sum_map[var] == 0)
            {
                sum_map[var_f] = prob_map[var][0];
                
                num_vis[var_f]++;
                if(num_vis[var_f] == inp_num[var_f])
                    eval_bddprob(f);
            }
            else
            {
                prob_map[var][0] = sum_map[var]*prob_map[var][0];
                sum_map[var_f] = sum_map[var_f] + prob_map[var][0];
                
                num_vis[var_f]++;
                if(num_vis[var_f] == inp_num[var_f])
                    eval_bddprob(f);
            }
        }
        if(t_flag == 0 && f_flag == 0)
        {
            var_t = t.id();
            var_f = f.id();

            if(sum_map[var] == 0)
            {
                sum_map[var_t] = prob_map[var][1];
                sum_map[var_f] = prob_map[var][0];
                
                num_vis[var_t]++;
                if(num_vis[var_t] == inp_num[var_t])
                    eval_bddprob(t);
                
                num_vis[var_f]++;
                if(num_vis[var_f] == inp_num[var_f])
                    eval_bddprob(f);
            }
            else
           {
                prob_map[var][1] = sum_map[var]*prob_map[var][1];
                prob_map[var][0] = sum_map[var]*prob_map[var][0];

                sum_map[var_t] = sum_map[var_t] + prob_map[var][1];
                sum_map[var_f] = sum_map[var_f] + prob_map[var][0];

                num_vis[var_t]++;
                if(num_vis[var_t] == inp_num[var_t])
                    eval_bddprob(t);
                
                num_vis[var_f]++;
                if(num_vis[var_f] == inp_num[var_f])
                    eval_bddprob(f);
            }
        }
    }
}

/*
 * Find an element in a list
 */
bool bdd_prob::check_list(list<int> inp_l, int key)
{
    list<int>::iterator lit;
    
    for(lit = inp_l.begin(); lit != inp_l.end(); ++lit)
        if(*lit == key)
            return true;
    
    return false;
}

/*
 * Deallocate prob_map memory
 */
void bdd_prob::del_probmap()
{
    prmap::iterator pit;
    
    for(pit = prob_map.begin(); pit != prob_map.end(); ++pit)
    {
        delete[] pit->second;
        
    }
}