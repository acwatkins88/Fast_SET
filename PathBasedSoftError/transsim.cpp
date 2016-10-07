#include "transsim.h"

using namespace std;

gen_sim::gen_sim(int t)
{
    sim_type = t;
    this->event_n = 0;
    this->id_n = 0; 
}

/*
 * Generate Transient Pulse
 * type - Rising Or Falling
 * n_num - node to inject
 * Simple injection of 5 units
 */
transient gen_sim::gen_pulse(int type, int n_num, int delay)
{
    transient temp_pul;
    //vector<double> test_result;
    double st_time, end_time, width;
    
    graph[n_num].pulse = true;
    
    switch(graph[n_num].type)
    {
        case AND:
            // Generate Pulse For AND Gate
            temp_pul = inj_AND(n_num, CHARGE, type, delay);
            break;
        case NAND:
            // Generate Pulse for NAND Gate
            temp_pul = inj_NAND(n_num, CHARGE, type, delay);
            break;
        case OR:
            // Generate Pulse for OR Gate
            temp_pul = inj_OR(n_num, CHARGE, type, delay);
            break;
        case NOR:
            // Generate Pulse for NOR Gate
            temp_pul = inj_NOR(n_num, CHARGE, type, delay);
            break;
        case BUF:
            // Generate Pulse for Buffer
            temp_pul = inj_BUF(n_num, CHARGE, type, delay);
            break;
        case NOT:
            // Generate Pulse for Inverter
            temp_pul = inj_NOT(n_num, CHARGE, type, delay);
            break;
        default:
            cout<<"Error Determining Gate: "<<n_num<<" of Type: "<<graph[n_num].type<<endl;
    }
    
    if(temp_pul.width <= W_MIN)
        return temp_pul;
    
    temp_pul.e_num = this->event_n;
    this->event_n++;
    
    temp_pul.id  = this->id_n;
    this->id_n++;
    
    temp_pul.s_node = n_num;
    temp_pul.id = this->id_n;
    this->id_n++;
    
    return temp_pul;
}

/*
 * Attempt at Generalizing the enhanced injection model
 */
transient gen_sim::inj_NAND(int n_num, double charge, int type, int delay)
{
    int i;
    int inj_t = 0;
    int itr_num = graph[n_num].fanin_num;
    
    double time, temp_vg, temp_vd;
    double vd_init, vg_init;
    double n_cur, p_cur;
    double inj_cur, cur_out;
    double res, tau;
    double st_ratio, adj_step;
    double w_val1 = 0;
    double w_val2 = 0;
    double eff_load;
    
    vector<double> ip;
    vector<double> in;
    vector<double> cmp;
    vector<double> cmn;
    
    vector<vector<double> > n_volt;
    vector<double> inj_cur_arr;
    vector<double> temp_out;
    
    transient temp_pul;
    
    //inj_cur[0] = 0;
    inj_cur_arr.push_back(0);
    
    if(type == RISING)
    {
        vg_init = VDD;
        vd_init = VDD;
        temp_out.push_back(0);
    }
    else 
    {
        vg_init = 0;
        vd_init = 0;
        temp_out.push_back(VDD);
    }
    
    vector<double> temp_vec;
    // Initialize n_volt
    for(i = 0; i < itr_num-1; i++)
    {
        temp_vec.push_back(0);
        n_volt.push_back(temp_vec);
    }
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;
    
    for(int t = 1; t <= NUM_STEPS; t++)
    {
        //cout<<"Iteration: "<<t<<endl;
        for(i = 0; i < itr_num; i++)
        {
            res = ind_current(PMOS, temp_out[t-1], vg_init);
            ip.push_back(res);
            res = ind_miller(PMOS, temp_out[t-1], vg_init);
            cmp.push_back(res);
            
            if(i == 0)
                temp_vd = temp_out[t-1] - n_volt[0][t-1];
            else if(i != (itr_num-1))
                temp_vd = n_volt[i-1][t-1] - n_volt[i][t-1];
            else
                temp_vd = n_volt[i-1][t-1];
            
            if(i == (itr_num-1))
                temp_vg = vg_init;
            else
                temp_vg = vg_init - n_volt[i][t-1];
            
            res = ind_current(NMOS, temp_vd, temp_vg);
            in.push_back(res);
            
            if(i == 0)
                cmn.push_back(ind_miller(NMOS, temp_out[t-1], vg_init));
            
        }

        adj_step = STEP_GRAN*st_ratio;
        
        p_cur = sum_vector(ip);
        n_cur = avg_vector(in);

        for(i = 0; i < itr_num-1; i++)
            n_volt[i].push_back((((-in[i] + in[i+1])*STEP_GRAN)/ST_NODE_CAP) + n_volt[i][t-1]);
        
        if(type == RISING)
            tau = 175e-12;
        else
            tau = 32e-12;
        
        if(t > delay)
        {
            time = STEP_GRAN*(st_ratio)*(inj_t);
            inj_cur = ((2*charge)/(tau*sqrt(PI)))*(sqrt(time/tau))*(exp(-time/tau));
            inj_t++;
        }
        else
        {
            inj_cur = 0;
        }
        //inj_cur = 0;
        inj_cur_arr.push_back(inj_cur);
        
        // Export resulting pulse to output file
        //export_vec(inj_cur_arr, "CurOut");
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur + inj_cur)*STEP_GRAN)/(eff_load + sum_vector(cmp) + cmn[0])) + temp_out[t-1];
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur - inj_cur)*STEP_GRAN)/(eff_load + sum_vector(cmp) + cmn[0])) + temp_out[t-1];
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
        }
      
        temp_pul.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio); 
        temp_out.push_back(cur_out);
        
        //cout<<"nvolt 1: "<<n_volt[0][t]<<" nvolt 2: "<<n_volt[1][t]<<" output: "<<cur_out<<endl;
        
        in.clear();
        ip.clear();
        cmp.clear();
        cmn.clear();
    }
    
    temp_pul.volt_pulse = temp_out;
    return temp_pul;  
}

transient gen_sim::inj_AND(int n_num, double charge, int type, int delay)
{
    transient temp;
    list<transient> inp;
    
    temp = inj_NAND(n_num, charge, type, delay);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

/*
 * Routine to inject a radiation pulse into a NOR gate
 */
transient gen_sim::inj_NOR(int n_num, double charge, int type, int delay)
{
    int i;
    int inj_t = 0;
    int itr_num = graph[n_num].fanin_num;
    
    double time, temp_vg, temp_vd;
    double vd_init, vg_init;
    double n_cur, p_cur;
    double inj_cur, cur_out;
    double res, tau;
    double st_ratio, adj_step;
    double w_val1 = 0;
    double w_val2 = 0;
    double eff_load;
    
    vector<double> ip;
    vector<double> in;
    vector<double> cmp;
    vector<double> cmn;
    
    vector<vector<double> > p_volt;
    vector<double> inj_cur_arr;
    vector<double> temp_out;
    
    transient temp_pul;
    
    //inj_cur[0] = 0;
    inj_cur_arr.push_back(0);
    
    if(type == RISING)
    {
        vg_init = VDD;
        vd_init = VDD;
        temp_out.push_back(0);
    }
    else 
    {
        vg_init = 0;
        vd_init = 0;
        temp_out.push_back(VDD);
    }
    
    vector<double> temp_vec;
    // Initialize p_volt
    for(i = 0; i < itr_num-1; i++)
    {
        temp_vec.push_back(0.90);
        p_volt.push_back(temp_vec);
    }
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;
    
    for(int t = 1; t <= NUM_STEPS; t++)
    {
        //cout<<"Iteration: "<<t<<endl;
        for(i = 0; i < itr_num; i++)
        {
            res = ind_current(NMOS, temp_out[t-1], vg_init);
            in.push_back(res);
            res = ind_miller(NMOS, temp_out[t-1], vg_init);
            cmn.push_back(res);
            
            if(i == 0)
                temp_vd = temp_out[t-1] - (p_volt[0][t-1] - VDD);
            else if(i != (itr_num-1))
                temp_vd = p_volt[i-1][t-1] - (p_volt[i][t-1] - VDD);
            else
                temp_vd = p_volt[i-1][t-1];
            
            if(i == (itr_num-1))
                temp_vg = vg_init;
            else
                temp_vg = vg_init - (p_volt[i][t-1] - VDD);
            
            //cout<<"Temp vd: "<<temp_vd<<" Temp vg: "<<temp_vg<<endl;
            res = ind_current(PMOS, temp_vd, temp_vg);
            ip.push_back(res);
            
            if(i == 0)
                cmp.push_back(ind_miller(PMOS, temp_out[t-1], vg_init));
            
        }

        adj_step = STEP_GRAN*st_ratio;
        
        p_cur = avg_vector(ip);
        n_cur = sum_vector(in);

        for(i = 0; i < itr_num-1; i++)
        {
            p_volt[i].push_back((((-ip[i] + ip[i+1])*STEP_GRAN)/ST_NODE_CAP) + p_volt[i][t-1]);
            //cout<<"Cur 1: "<<in[i]<<" Cur 2: "<<in[i+1]<<endl;
        }
        
        if(type == RISING)
            tau = 32e-12;
        else
            tau = 90e-12;
        
        if(t > delay)
        {
            time = adj_step*(inj_t);
            inj_cur = ((2*charge)/(tau*sqrt(PI)))*(sqrt(time/tau))*(exp(-time/tau));
            inj_t++;
        }
        else
        {
            inj_cur = 0;
        }
        //inj_cur = 0;
        inj_cur_arr.push_back(inj_cur);
        
        // Export resulting pulse to output file
        //export_vec(inj_cur_arr, "CurOut");
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur + inj_cur)*STEP_GRAN)/(eff_load + sum_vector(cmn) + cmp[0])) + temp_out[t-1];
            //cout<<"Voltage: "<<cur_out<<endl;
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur - inj_cur)*STEP_GRAN)/(eff_load + sum_vector(cmn) + cmp[0])) + temp_out[t-1];
            //cout<<"Voltage: "<<cur_out<<endl;
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
        }
      
        temp_pul.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio); 
        temp_out.push_back(cur_out);
        
        //cout<<"nvolt 1: "<<n_volt[0][t]<<" nvolt 2: "<<n_volt[1][t]<<" output: "<<cur_out<<endl;
        //cout<<"nvolt 1: "<<p_volt[0][t]<<" output: "<<cur_out<<endl;
        
        in.clear();
        ip.clear();
        cmp.clear();
        cmn.clear();
    }
    
    temp_pul.volt_pulse = temp_out;

    return temp_pul;   
}

transient gen_sim::inj_OR(int n_num, double charge, int type, int delay)
{
    transient temp;
    list<transient> inp;
    
    temp = inj_NOR(n_num, charge, type, delay);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

/*
 * Routine to inject a radiation pulse into an inverter
 */
transient gen_sim::inj_NOT(int n_num, double charge, int type, int delay)
{
    int inj_t = 0;
    
    double time;
    double vd_init, vg_init;
    double inj_cur, cur_out;
    double tau;
    double st_ratio, adj_step;
    double w_val1 = 0;
    double w_val2 = 0;
    double eff_load;
    
    double ip;
    double in;
    double cmp;
    double cmn;
   
    vector<double> inj_cur_arr;
    vector<double> temp_out;
    
    transient temp_pul;
    
    //inj_cur[0] = 0;
    inj_cur_arr.push_back(0);
    
    if(type == RISING)
    {
        vg_init = VDD;
        vd_init = VDD;
        temp_out.push_back(0);
    }
    else 
    {
        vg_init = 0;
        vd_init = 0;
        temp_out.push_back(VDD);
    }
    
    /*vector<double> temp_vec;
    // Initialize p_volt
    for(i = 0; i < itr_num-1; i++)
    {
        temp_vec.push_back(0.90);
        p_volt.push_back(temp_vec);
    }*/
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;
    
    for(int t = 1; t <= NUM_STEPS; t++)
    {
        in = ind_current(NMOS, temp_out[t-1], vg_init);
        cmn = ind_miller(NMOS, temp_out[t-1], vg_init);

        ip = ind_current(PMOS, temp_out[t-1], vg_init);
        cmp = ind_miller(PMOS, temp_out[t-1], vg_init);   

        adj_step = STEP_GRAN*st_ratio;

        if(type == RISING)
            tau = 75e-12;
        else
            tau = 45e-12;
        
        if(t > delay)
        {
            time = adj_step*(inj_t);
            inj_cur = ((2*charge)/(tau*sqrt(PI)))*(sqrt(time/tau))*(exp(-time/tau));
            inj_t++;
        }
        else
        {
            inj_cur = 0;
        }
        inj_cur_arr.push_back(inj_cur);
        
        // Export resulting pulse to output file
        //export_vec(inj_cur_arr, "CurOut");
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((ip + in + inj_cur)*STEP_GRAN)/(eff_load + cmn + cmp)) + temp_out[t-1];
            //cout<<"Voltage: "<<cur_out<<endl;
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((ip + in - inj_cur)*STEP_GRAN)/(eff_load + cmn + cmp)) + temp_out[t-1];
            //cout<<"Voltage: "<<cur_out<<endl;
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                temp_pul.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                temp_pul.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
        }
      
        temp_pul.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio); 
        temp_out.push_back(cur_out);
        
        //cout<<"Output: "<<cur_out<<endl;
    }

    temp_pul.volt_pulse = temp_out;
    
    return temp_pul;   
}

/*
 * Function to inject a pulse into a buffer
 */
transient gen_sim::inj_BUF(int n_num, double charge, int type, int delay)
{
    transient temp;
    list<transient> inp;
    
    temp = inj_NOT(n_num, charge, type, delay);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

/*
 * Function to propagate enhanced pulse
 */
void gen_sim::prop_enhpulse(int n_num)
{
    transient out_pulse;
    
    list<int>::iterator lit;
    list<transient>::iterator eit;
    list<transient> inputs;
    
    for(lit = graph[n_num].fanin.begin(); lit != graph[n_num].fanin.end(); ++lit)
    {
        for(eit = graph[*lit].p_list.begin(); eit != graph[*lit].p_list.end(); ++eit)
        {
            //cout<<"Beginning\n";
            inputs.push_back(*eit);

            out_pulse = det_pulse(inputs, n_num);
            
            //cout<<"Node: "<<n_num<<" Width: "<<out_pulse.width<<endl;
            if(out_pulse.width > W_MIN)
            {   
                out_pulse.e_num = eit->e_num;
                out_pulse.id = this->id_n;
                out_pulse.s_node = n_num;
                this->id_n++;
                
                if(this->sim_type == BDD_SIM)
                    set_propfunc(n_num, *lit, *eit, out_pulse);
                
                graph[n_num].p_list.push_back(out_pulse); 
            }
            inputs.clear();
        }
    }
}

/*
 * Routine to choose the pulse propagation algorithm based on the gate type
 */
transient gen_sim::det_pulse(list<transient> inputs, int n_num)
{
    list<transient> temp;
    
    switch(graph[n_num].type)
    {
        case NAND:
            return calc_NAND(inputs, n_num);
        case AND:
            return calc_AND(inputs, n_num);
        case NOR:
            return calc_NOR(inputs, n_num);
        case OR:
            return calc_OR(inputs, n_num);
        case NOT:
            return calc_NOT(inputs, n_num);
        case BUF:
            return calc_BUF(inputs, n_num);
        default:
            cout<<"Gate Type "<<graph[n_num].type<<" Not Implemented\n";
    }
}

/*
 * Determine the output given the inputs as a vector
 */
transient gen_sim::calc_NAND(list<transient> inputs, int n_num)
{
    //cout<<"Beginning NAND\n";
    int i, type;
    int itr_num = graph[n_num].fanin_num;
    
    double temp_vg, temp_vd;
    double vd_init, vg_cur;
    double n_cur, p_cur;
    double cur_out;
    double res;
    double eff_load;
    double st_ratio;
    double miller_factor = 0;
    double w_val1 = 0;
    double w_val2 = 0;
    
    bool flag;
    transient pulse;
    prev_p p_inf;
    
    list<transient>::iterator lit;
    list<int>::iterator fit;
    
    vector< vector<double> > ord_list;
    vector<double> temp_v;
    
    vector<double> ip;
    vector<double> in;
    vector<double> cmp;
    vector<double> cmn;
    vector<double> del_inp;
    
    vector<vector<double> > n_volt;
    vector<double> temp_out;
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;
    
    vd_init = 0;
    // Order inputs according to list
    for(fit = graph[n_num].fanin.begin(); fit != graph[n_num].fanin.end(); ++fit)
    {
        flag = false;
        for(lit = inputs.begin(); lit != inputs.end(); ++lit)
        {
            //cout<<"Cur s_node: "<<lit->s_node<<" Ideal s_node: "<<*fit<<endl;
            if(lit->s_node == *fit)
            {
                //cout<<"Source Node: "<<lit->s_node<<endl;
                ord_list.push_back(lit->volt_pulse);
                p_inf.p_gate = *fit;
                p_inf.p_pulse = lit->id;
                pulse.prev_pulse.push_back(p_inf);
                flag = true;
                if(lit->volt_pulse[0] < 0.95)
                    vd_init =  VDD;
            }
        }
        if(!flag)
        {
            temp_v.push_back(VDD);
            ord_list.push_back(temp_v);
        }  
    }
    
    if(vd_init != VDD)
        type = FALLING;
    else
        type = RISING;
    
    temp_out.push_back(vd_init);
    
    vector<double> temp_vec;
    // Initialize n_volt
    for(i = 0; i < itr_num-1; i++)
    {
        temp_vec.push_back(0);
        n_volt.push_back(temp_vec);
    }
    
    for(int t = 1; t <= NUM_STEPS; t++)
    {
        //cout<<"Node: "<<n_num<<" Iteration: "<<t<<endl;
        for(i = 0; i < itr_num; i++)
        {
            if(ord_list[i].size() != 1)
            {
                vg_cur = ord_list[i][t];
                del_inp.push_back(ord_list[i][t] - ord_list[i][t-1]);
            }
            else
            {
                vg_cur = VDD;
                del_inp.push_back(0);
            }
            //cout<<"Input: "<<i<<" Vg: "<<vg_cur<<endl;
            
            res = ind_current(PMOS, temp_out[t-1], vg_cur);
            ip.push_back(res);
            res = ind_miller(PMOS, temp_out[t-1], vg_cur);
            cmp.push_back(res);
            
            if(i == 0)
                temp_vd = temp_out[t-1] - n_volt[0][t-1];
            else if(i != (itr_num-1))
                temp_vd = n_volt[i-1][t-1] - n_volt[i][t-1];
            else
                temp_vd = n_volt[i-1][t-1];
            
            if(i == (itr_num-1))
                temp_vg = vg_cur;
            else
                temp_vg = vg_cur - n_volt[i][t-1];
            
            res = ind_current(NMOS, temp_vd, temp_vg);
            in.push_back(res);
            
            if(i == 0)
                cmn.push_back(ind_miller(NMOS, temp_out[t-1], vg_cur)); 
            
            miller_factor = miller_factor + cmp[i]*del_inp[i];
             
            if(i == 0)
                miller_factor = miller_factor + cmn[i]*del_inp[i];
        }
        
        p_cur = sum_vector(ip);
        n_cur = avg_vector(in);

        for(i = 0; i < itr_num-1; i++)
            n_volt[i].push_back((((-in[i] + in[i+1])*STEP_GRAN)/ST_NODE_CAP) + n_volt[i][t-1]);
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur)*STEP_GRAN + miller_factor)/(eff_load + sum_vector(cmp) + cmn[0])) + temp_out[t-1];
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur)*STEP_GRAN + miller_factor)/(eff_load + sum_vector(cmp) + cmn[0])) + temp_out[t-1];
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        pulse.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio);
        temp_out.push_back(cur_out);  
               
        //cout<<"nvolt 1: "<<n_volt[0][t]<<" nvolt 2: "<<n_volt[1][t]<<" output: "<<cur_out<<endl;
        //cout<<"nvolt 1: "<<n_volt[0][t]<<" output: "<<cur_out<<endl;
        
        in.clear();
        ip.clear();
        cmp.clear();
        cmn.clear();
    }
    
    pulse.volt_pulse = temp_out;
    
    return pulse;
}

transient gen_sim::calc_AND(list<transient> inputs, int n_num)
{
    transient temp;
    list<transient> inp; 
    
    temp = calc_NAND(inputs, n_num);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

/*
 * Determine the output given the inputs as a vector
 */
transient gen_sim::calc_NOR(list<transient> inputs, int n_num)
{
    int i, type;
    int itr_num = graph[n_num].fanin_num;
    
    double temp_vg, temp_vd;
    double vd_init, vg_cur;
    double n_cur, p_cur;
    double cur_out;
    double res;
    double eff_load;
    double st_ratio;
    double miller_factor = 0;
    double w_val1 = 0;
    double w_val2 = 0;
    
    bool flag;
    transient pulse;
    prev_p p_inf;
    
    list<transient>::iterator lit;
    list<int>::iterator fit;
    
    vector< vector<double> > ord_list;
    vector<double> temp_v;
    
    vector<double> ip;
    vector<double> in;
    vector<double> cmp;
    vector<double> cmn;
    vector<double> del_inp;
    
    vector<vector<double> > p_volt;
    vector<double> temp_out;
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;
    
    vd_init = VDD;
    // Order inputs according to list
    for(fit = graph[n_num].fanin.begin(); fit != graph[n_num].fanin.end(); ++fit)
    {
        flag = false;
        for(lit = inputs.begin(); lit != inputs.end(); ++lit)
        {
            if(lit->s_node == *fit)
            {
                ord_list.push_back(lit->volt_pulse);
                p_inf.p_gate = *fit;
                p_inf.p_pulse = lit->id;
                pulse.prev_pulse.push_back(p_inf);
                flag = true;
                if(lit->volt_pulse[0] > 0.25)
                    vd_init =  0;
            }
        }
        if(!flag)
        {
            temp_v.push_back(0);
            ord_list.push_back(temp_v);
        }  
    }
    
    if(vd_init != VDD)
        type = FALLING;
    else
        type = RISING;
    
    temp_out.push_back(vd_init);
    
    vector<double> temp_vec;
    // Initialize p_volt
    for(i = 0; i < itr_num-1; i++)
    {
        temp_vec.push_back(0.90);
        p_volt.push_back(temp_vec);
    }
    
    for(int t = 1; t <= NUM_STEPS; t++)
    {
        for(i = 0; i < itr_num; i++)
        {
            if(ord_list[i].size() != 1)
            {
                vg_cur = ord_list[i][t];
                del_inp.push_back(ord_list[i][t] - ord_list[i][t-1]);
            }
            else
            {
                vg_cur = 0;
                del_inp.push_back(0);
            }
            
            res = ind_current(NMOS, temp_out[t-1], vg_cur);
            in.push_back(res);
            res = ind_miller(NMOS, temp_out[t-1], vg_cur);
            cmn.push_back(res);
            
            if(i == 0)
                temp_vd = temp_out[t-1] - (p_volt[0][t-1] - VDD);
            else if(i != (itr_num-1))
                temp_vd = p_volt[i-1][t-1] - (p_volt[i][t-1] - VDD);
            else
                temp_vd = p_volt[i-1][t-1];
            
            if(i == (itr_num-1))
                temp_vg = vg_cur;
            else
                temp_vg = vg_cur - (p_volt[i][t-1] - VDD);
            
            res = ind_current(PMOS, temp_vd, temp_vg);
            ip.push_back(res);
            
            if(i == 0)
                cmp.push_back(ind_miller(PMOS, temp_out[t-1], vg_cur));      
            //cout<<"Input: "<<i<<" Vg: "<<temp_vg<<endl;
            
            miller_factor = miller_factor + cmn[i]*del_inp[i];
             
            if(i == 0)
                miller_factor = miller_factor + cmp[i]*del_inp[i];
             
            //cout<<"Miller Factor: "<<miller_factor<<" Cap: "<<cmn[i]<<" Delta; "<<del_inp[i]<<endl;
        }
        
        p_cur = avg_vector(ip);
        n_cur = sum_vector(in);
        
        //cout<<"Ip0: "<<ip[0]<<" Ip1: "<<ip[1]<<" In0: "<<in[0]<<" In1: "<<in[1]<<endl;
        
        for(i = 0; i < itr_num-1; i++)
        {
            p_volt[i].push_back((((-ip[i] + ip[i+1])*STEP_GRAN)/ST_NODE_CAP) + p_volt[i][t-1]);
            //cout<<"Cur 1: "<<in[i]<<" Cur 2: "<<in[i+1]<<endl;
        }
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur)*STEP_GRAN + miller_factor)/(eff_load + sum_vector(cmn) + cmp[0])) + temp_out[t-1];
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((p_cur + n_cur)*STEP_GRAN + miller_factor)/(eff_load + sum_vector(cmn) + cmp[0])) + temp_out[t-1];
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        pulse.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio);
        temp_out.push_back(cur_out);  
        
        //cout<<"pvolt 1: "<<p_volt[0][t]<<" pvolt 2: "<<p_volt[1][t]<<" output: "<<cur_out<<endl;
        //cout<<"pvolt 1: "<<p_volt[0][t]<<" output: "<<cur_out<<endl;
        
        in.clear();
        ip.clear();
        cmp.clear();
        cmn.clear();
    }
    
    pulse.volt_pulse = temp_out;
    
    return pulse;
}

transient gen_sim::calc_OR(list<transient> inputs, int n_num)
{
    transient temp;
    list<transient> inp; 
    
    temp = calc_NOR(inputs, n_num);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

transient gen_sim::calc_NOT(list<transient> inputs, int n_num)
{
    int type;
    
    double vd_init, vg_cur;
    double cur_out;
    double eff_load;
    double st_ratio;
    double miller_factor = 0;
    double w_val1 = 0;
    double w_val2 = 0;
    double ip, in, cmp, cmn, del_inp;
    
    transient pulse;
    transient temp_p;
    
    vector<double> temp_out;
    
    if(vd_init != VDD)
        type = FALLING;
    else
        type = RISING;
     
    temp_p = inputs.front();
    
    if(temp_p.volt_pulse[0] > 0.25)
        vd_init = 0;
    else
        vd_init = VDD;
    
    temp_out.push_back(vd_init);
    
    st_ratio = SIM_TIME/STEP_GRAN;
    st_ratio = st_ratio/NUM_STEPS;

    for(int t = 1; t <= NUM_STEPS; t++)
    {
        //if(n_num == 6)
            //cout<<"Iteration: "<<t<<endl;
        vg_cur = temp_p.volt_pulse[t];
        del_inp = temp_p.volt_pulse[t] - temp_p.volt_pulse[t-1];
  
        in = ind_current(NMOS, temp_out[t-1], vg_cur);
        cmn = ind_miller(NMOS, temp_out[t-1], vg_cur);
            
        ip = ind_current(PMOS, temp_out[t-1], vg_cur);
        cmp = ind_miller(PMOS, temp_out[t-1], vg_cur);      
            
        miller_factor = miller_factor + cmn*del_inp + cmp*del_inp;
             
        //cout<<"Miller Factor: "<<miller_factor<<" Cap: "<<cmn[i]<<" Delta; "<<del_inp[i]<<endl;
        
        //if(n_num == 6)
            //cout<<"Ip: "<<ip<<" In: "<<in<<endl;
        
        if(type == RISING)
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((ip + in)*STEP_GRAN + miller_factor)/(eff_load + cmn + cmp)) + temp_out[t-1];
            //f(n_num == 6)
                //cout<<"Out: "<<cur_out<<" Vg: "<<vg_cur<<" Ip: "<<ip<<" In: "<<in<<endl;
            if((temp_out[t-1] < ST_THRESH) && (cur_out > ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] > END_THRESH) && (cur_out < END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] < W_THRESH1) && (cur_out > W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] > W_THRESH2) && (cur_out < W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        else
        {
            eff_load = graph[n_num].fanout_num*ST_NODE_CAP;
            cur_out = (((ip + in)*STEP_GRAN + miller_factor)/(eff_load + cmn + cmp)) + temp_out[t-1];
            //if(n_num == 6)
                //cout<<"Out: "<<cur_out<<" Vg: "<<vg_cur<<" Ip: "<<ip<<" In: "<<in<<endl;
            if((temp_out[t-1] > ST_THRESH) && (cur_out < ST_THRESH))
                pulse.st_time = t;
            else if((temp_out[t-1] < END_THRESH) && (cur_out > END_THRESH))
                pulse.end_time = t;
            if((temp_out[t-1] > W_THRESH1) && (cur_out < W_THRESH1))
                w_val1 = t;
            else if((temp_out[t-1] < W_THRESH2) && (cur_out > W_THRESH2))
                w_val2 = t;
            
            miller_factor = 0;
        }
        pulse.width = abs(((w_val1 - w_val2)*STEP_GRAN)/st_ratio);
        temp_out.push_back(cur_out);  
    }
    
    pulse.volt_pulse = temp_out;
    
    return pulse; 
}

transient gen_sim::calc_BUF(list<transient> inputs, int n_num)
{
    transient temp;
    list<transient> inp; 
    
    temp = calc_NOT(inputs, n_num);
    
    if(temp.width > W_MIN)
    {
        inp.push_back(temp);
        return calc_NOT(inp, n_num);
    }
    else
        return temp;
}

/*
 * Subroutine to check if two pulses overlap
 */
bool gen_sim::is_overlap(transient in1, transient in2)
{
    if((in1.st_time < in2.st_time)&&(in1.end_time < in2.end_time))
        return true;
    else if((in1.st_time > in2.st_time)&&(in1.end_time > in2.end_time))
        return true;
    else if((in1.st_time < in2.st_time)&&(in1.end_time > in2.end_time))
        return true;
    else if((in1.st_time > in2.st_time)&&(in1.end_time < in2.end_time))
        return true;
    else
        return false;
}

/*
 * Print result to external file with .out at the extension
 */
void gen_sim::export_vec(vector<double> inp_vec, string file_name)
{
    ofstream o_file;
    vector<double>::iterator vit;
    
    file_name.append(".out");
    
    o_file.open(file_name.c_str(), ios::trunc);
    
    for(vit = inp_vec.begin(); vit != inp_vec.end(); ++vit)
        o_file<<*vit<<" ";
    
    o_file.close();
}

/*
 * Sum the elements of a vector
 */
double gen_sim::sum_vector(vector<double> inp)
{
    double sum = 0;
    vector<double>::iterator vit;
    
    for(vit = inp.begin(); vit != inp.end(); ++vit)
        sum = sum + *vit;
    
    return sum;
}

/*
 * Average the elements in a vector
 */
double gen_sim::avg_vector(vector<double> inp)
{
    double sum; 
    
    sum = sum_vector(inp);
        
    return sum/inp.size();
}

/*
 * Index transistor current
 */
double gen_sim::ind_current(int type, double d_volt, double g_volt)
{
    int d_size;
    int d_index;
    int g_size;
    int g_index;
    
    if(type == PMOS)
    {
        d_size = pmos_cur[0].size();
        g_size = pmos_cur.size();
        
        d_index = floor((d_volt + MIN_VAL)/(VD_GRAN));
        g_index = floor((g_volt + MIN_VAL)/(VG_GRAN));
        
        return pmos_cur[g_index][d_index];
    }
    else if(type == NMOS)
    {
        d_size = nmos_cur[0].size();
        g_size = nmos_cur.size();
        
        d_index = floor((d_volt + MIN_VAL)/(VD_GRAN));
        g_index = floor((g_volt + MIN_VAL)/(VG_GRAN));
        
        return nmos_cur[g_index][d_index];
    }
    else
    {
        cout<<"Invalid Transistor Type - Current Index\n";
        return 0;
    }
}

/*
 * Index Miller capacitance
 */
double gen_sim::ind_miller(int type, double d_volt, double g_volt)
{
    int d_size;
    int d_index;
    int g_size;
    int g_index;
    double div_d, div_g;
    
    if(type == PMOS)
    {
        d_size = pmos_miller[0].size();
        g_size = pmos_miller.size();
        
        div_d = MAX_VD + MIN_VAL;
        div_g = MAX_VG + MIN_VAL;
        
        d_index = floor((d_volt + MIN_VAL)*(d_size/div_d));
        g_index = floor((g_volt + MIN_VAL)*(g_size/div_g));
        
        return pmos_miller[g_index][d_index];
    }
    else if(type == NMOS)
    {
        d_size = nmos_miller[0].size();
        g_size = nmos_miller.size();
        
        div_d = MAX_VD + MIN_VAL;
        div_g = MAX_VG + MIN_VAL;
        
        d_index = floor((d_volt + MIN_VAL)*(d_size/div_d));
        g_index = floor((g_volt + MIN_VAL)*(g_size/div_g));
        
        return nmos_miller[g_index][d_index];
    }
    else
    {
        cout<<"Invalid Transistor Type - Current Index\n";
        return 0;
    }
}

/*
 * Process all pulses at the gate i_num
 */
void gen_sim::proc_pulse(int n_num)
{
    prev_p p_temp;
    transient temp;
    list<transient>::iterator t_it;
    list<int>::iterator i_it;
    
    for(i_it = graph[n_num].fanin.begin(); i_it != graph[n_num].fanin.end(); ++i_it)
    {
        for (t_it = graph[*i_it].p_list.begin(); t_it != graph[*i_it].p_list.end(); ++t_it)
        {
            temp.delay = calc_delay();
            
            // Attenuate glitch if width is smaller than 2*gate_delay (temporary)
            if ((t_it->width) > (2 * temp.delay))
            {
                if (is_inv(graph[*i_it].type) == 1)
                {
                    if (t_it->type == RISING)
                        temp.type = FALLING;
                    else
                        temp.type = RISING;
                }
                else
                {
                    if (t_it->type == RISING)
                        temp.type = RISING;
                    else
                        temp.type = FALLING;
                }
                temp.width = t_it->width;
                temp.st_time = t_it->st_time + temp.delay;
                temp.end_time = t_it->end_time + temp.delay;
                temp.e_num = t_it->e_num;
                temp.id = this->id_n;
                this->id_n++;
                p_temp.p_gate = *i_it;
                p_temp.p_pulse = t_it->id;
                temp.prev_pulse.push_back(p_temp);
                temp.t_prob = t_it->t_prob;
                
                set_propfunc(n_num, *i_it, *t_it, temp);
                
                temp.prev_pulse.clear();
            }
        }
    }
}

void gen_sim::set_propfunc(int n_num, int inp_node, transient p, transient temp)
{
    cout<<"Invalid Prop Function\n";
}

/*
 * Conversion check using the enhanced model
 */
void gen_sim::enhconv_check(int n_num)
{
    transient cur_trans;
    transient n_pul;
    list<int>::iterator fit;
    list<transient> temp_l;
    list<transient> inputs;
    map<int,  list<transient> > h_table;
    list<transient>::iterator pit;
    map<int, list<transient> >::iterator hit;
    
    for(fit = graph[n_num].fanin.begin(); fit != graph[n_num].fanin.end(); ++fit)
    {
        for(pit = graph[*fit].p_list.begin(); pit != graph[*fit].p_list.end(); ++pit)
        {
            h_table[pit->e_num].push_back(*pit);
        }
    }
    
    for(hit = h_table.begin(); hit != h_table.end(); ++hit)
    {
        temp_l = h_table[hit->first];
        cur_trans = temp_l.front();
        inputs.push_back(cur_trans);
        temp_l.pop_front();
        
        for(pit = temp_l.begin(); pit != temp_l.end(); ++pit)
        {
            if(is_overlap(cur_trans, *pit))
            {
                inputs.push_back(*pit);
                n_pul = det_pulse(inputs, n_num);
                
                if(n_pul.volt_pulse.size() > 1)
                {
                    eval_convfunc(n_num, cur_trans, *pit, n_pul);

                    n_pul.e_num = hit->first;
                    n_pul.id = this->id_n;
                    n_pul.s_node = n_num;
                    this->id_n++;
                    graph[n_num].p_list.push_back(n_pul);
                }
            }
        }
    }
}

void gen_sim::eval_convfunc(int n_num, transient t1, transient t2, transient& t3)
{
    cout<<"Error in Determining Conv Function\n";
}


/*
 * Process Reconvergencies
 * F_INP - Pulse 1
 * H_IT - Pulse 2
 */
void gen_sim::conv_check(int n_num)
{
    bool t_1, t_2;
    int cat;
    map <int, list<int> > conv_map;

    list<transient> h_table;
    list<transient> temp_pl = graph[n_num].p_list;
    
    list<transient>::iterator f_inp;
    list<transient>::iterator h_it;
    
    h_table.push_back(temp_pl.front());
    temp_pl.pop_front();

    for(f_inp = temp_pl.begin(); f_inp != temp_pl.end(); ++f_inp)
    {  
        for(h_it = h_table.begin(); h_it != h_table.end(); ++h_it)
        {
            if(h_it->e_num == f_inp->e_num)
            {
                t_1 = is_ovr(f_inp->st_time, f_inp->end_time, h_it->st_time);
                t_2 = is_ovr(f_inp->st_time, f_inp->end_time, h_it->end_time);
                if((t_1 == true)||(t_2 == true))
                {
                    if(map_check(conv_map, f_inp->id, h_it->id) == false)
                    {
                        //Process the Pulse Overlap
                        cat = det_category(*f_inp, *h_it);
                        
                        switch(cat)
                        {
                            case 0:
                                break;
                            case 1:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 2:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 3:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 4:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 5:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 6:
                                eval_cat1to6(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 7:
                                eval_cat78(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                            case 8:
                                eval_cat78(conv_map, temp_pl, n_num, cat, *f_inp, *h_it);
                                break;
                        }
                    }
                }
            }
        }
        // Pulse is compared to each element in hash table
        if(find_element(conv_map, f_inp->id) == false)
            h_table.push_back(*f_inp);
    } 
    //}
}

void gen_sim::set_convprop(int n_num, transient &temp, transient p1, transient p2)
{
    cout<<"Wrong Conv Prop function\n";
}

void gen_sim::eval_cat0(map<int, list<int> > conv_map, list<transient> temp_pl, int n_num, int cat, transient f_inp, transient h_it)
{
    
}

void gen_sim::eval_cat1to6(map<int, list<int> > conv_map, list<transient> temp_pl, int n_num, int cat, transient f_inp, transient h_it)
{
    transient temp;
    
    temp.id = this->id_n;
    this->id_n++;
    temp.e_num = f_inp.e_num;
     
    switch(cat)
    {
        case 1:
            temp = calc_convtrans14(1, n_num, f_inp, h_it);
            break;
        case 2:
            temp = calc_convtrans14(2, n_num, f_inp, h_it);
            break;
        case 3:
            temp = calc_convtrans14(3, n_num, f_inp, h_it);
            break;
        case 4:
            temp = calc_convtrans14(4, n_num, f_inp, h_it);
            break;
        case 5:
            temp = calc_convtrans56(5, n_num, f_inp, h_it);
            break;
        case 6:
            temp = calc_convtrans56(6, n_num, f_inp, h_it);
            break;
    }
    
    set_convprop(n_num, temp, f_inp, h_it);
    
    add_toconvmap(conv_map, temp_pl, temp, n_num, f_inp.id, h_it.id);
   
}

void gen_sim::add_toconvmap(map<int, list<int> > conv_map, list<transient> temp_pl, transient inp_t, int n_num, int p1_id, int p2_id)
{
    bool flag = false;
    list<int>::iterator l_it;
    
    if(sim_type == BDD_SIM)
    {
        if((inp_t.p_func != bdd_false()))
            flag = true;
    }
    else
    {
        flag = true;
    }
    
    if((inp_t.width > 0)&&(flag == true))
    {
        graph[n_num].p_list.push_back(inp_t);

        // Mark the generated pulse
        if(find_element(conv_map, p1_id) == true)
        {
            for(l_it = conv_map[p1_id].begin(); l_it != conv_map[p1_id].end(); ++l_it)
                if(list_check(conv_map[p1_id], *l_it) == true)
                    conv_map[inp_t.id].push_back(*l_it);
        }
        conv_map[inp_t.id].push_back(p1_id);
                                    
        if(find_element(conv_map, p2_id) == true)
        {
            for(l_it = conv_map[p2_id].begin(); l_it != conv_map[p1_id].end(); ++l_it)
                if(list_check(conv_map[p2_id], *l_it) == true)
                    conv_map[inp_t.id].push_back(*l_it);
        }
        conv_map[inp_t.id].push_back(p2_id);
        temp_pl.push_back(inp_t);
    }
}


transient gen_sim::calc_convtrans14(int cat, int n_num, transient p1, transient p2)
{
    transient temp;
    
    if ((p1.type == RISING)&&(p2.type == RISING)) 
    {
        if (graph[n_num].type == AND || graph[n_num].type == NAND)
        {
            if(graph[n_num].type == AND)
                temp.type = RISING;
            if(graph[n_num].type == NAND)
                temp.type = FALLING;
            
            switch(cat)
            {
                case 1:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 2:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 3:
                    temp.st_time = p2.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 4:
                    temp.st_time = p1.st_time;
                    temp.end_time = p1.end_time;
                    break;
            }
        }
        if(graph[n_num].type == OR || graph[n_num].type == NOR)
        {
            if(graph[n_num].type == OR)
                temp.type = RISING;
            if(graph[n_num].type == NOR)
                temp.type = FALLING;
            
            switch(cat)
            {
                case 1:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 2:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 3:
                    temp.st_time = p1.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 4:
                    temp.st_time = p2.st_time;
                    temp.end_time = p2.end_time;
                    break;
            }
        }
    }
    else if((p1.type == FALLING)&&(p2.type == FALLING))
    {
        if (graph[n_num].type == AND || graph[n_num].type == NAND) 
        {
            if(graph[n_num].type == AND)
                temp.type = FALLING;
            if(graph[n_num].type == NAND)
                temp.type = RISING;
            
            switch(cat)
            {
                case 1:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 2:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 3:
                    temp.st_time = p1.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 4:
                    temp.st_time = p2.st_time;
                    temp.end_time = p2.end_time;
                    break;
            }
        }
        else if(graph[n_num].type == OR || graph[n_num].type == NOR)
        {
            if(graph[n_num].type == OR)
                temp.type = RISING;
            if(graph[n_num].type == NOR)
                temp.type = FALLING;
            
            switch(cat)
            {
                case 1:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.end_time;
                    break;
                case 2:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 3:
                    temp.st_time = p2.st_time;
                    temp.end_time = p2.end_time;
                    break;
                case 4:
                    temp.st_time = p1.st_time;
                    temp.end_time = p1.end_time;
                    break;
            }
        }
    }
    temp.width = temp.st_time - temp.end_time;
    return temp;
}

transient gen_sim::calc_convtrans56(int cat, int n_num, transient p1, transient p2)
{
    transient temp;
    
    if ((p1.type == FALLING)&&(p2.type == RISING)) 
    {
        if (graph[n_num].type == AND || graph[n_num].type == NAND) 
        {
            if(graph[n_num].type == AND)
                temp.type = RISING;
            if(graph[n_num].type == NAND)
                temp.type = FALLING;
            switch(cat)
            {
                case 5:
                    temp.st_time = p1.end_time;
                    temp.end_time = p2.end_time;
                    break;
                case 6:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.st_time;
                    break;
            }
        }
        else if (graph[n_num].type == OR || graph[n_num].type == NOR) 
        {
            if(graph[n_num].type == OR)
                temp.type = FALLING;
            if(graph[n_num].type == NOR)
                temp.type = RISING;
            switch(cat)
            {
                case 5:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.st_time;
                    break;
                case 6:
                    temp.st_time = p2.end_time;
                    temp.end_time = p1.end_time;
                    break;
            }
        }
    }
    else if((p1.type == RISING)&&(p2.type == FALLING))
    {
        if(graph[n_num].type == AND || graph[n_num].type == NAND) 
        {
            if(graph[n_num].type == AND)
                temp.type = RISING;
            if(graph[n_num].type == NAND)
                temp.type = FALLING;
            switch(cat)
            {
                case 5:
                    temp.st_time = p1.st_time;
                    temp.end_time = p2.st_time;
                    break;
                case 6:
                    temp.st_time = p2.st_time;
                    temp.end_time = p1.st_time;
                    break;
            }
        }
        else if (graph[n_num].type == OR || graph[n_num].type == NOR) 
        {
            if(graph[n_num].type == OR)
                temp.type = RISING;
            if(graph[n_num].type == NOR)
                temp.type = FALLING;
            switch(cat)
            {
                case 5:
                    temp.st_time = p1.end_time;
                    temp.end_time = p2.end_time;
                    break;
                case 6:
                    temp.st_time = p2.end_time;
                    temp.end_time = p1.end_time;
                    break;
            }
        }
    }
    temp.width = temp.st_time - temp.end_time;
    return temp;
}

void gen_sim::eval_cat78(map<int, list<int> > conv_map, list<transient> temp_pl, int n_num, int cat, transient f_inp, transient h_it)
{
    pair<transient, transient> temp_p;
    transient temp;
    
    if ((f_inp.type == FALLING)&&(h_it.type == RISING)) 
    {
        if(graph[n_num].type == AND || graph[n_num].type == NAND)
        {
            if(cat == 8)
            {
                temp_p = cat8case14(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp_p.first, n_num, f_inp.id, f_inp.id);
                add_toconvmap(conv_map, temp_pl, temp_p.second, n_num, f_inp.id, f_inp.id);
            }
        }
        if(graph[n_num].type == OR || graph[n_num].type == NOR)
        {
            if(cat == 7)
            {
                temp_p = cat7case23(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp_p.first, n_num, f_inp.id, f_inp.id);
                add_toconvmap(conv_map, temp_pl, temp_p.second, n_num, f_inp.id, f_inp.id);
            }
            else 
            {
                temp = cat8case2(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp, n_num, f_inp.id, f_inp.id);
            }
        }
    }
    if ((f_inp.type == RISING)&&(h_it.type == FALLING)) 
    {
        if (graph[n_num].type == AND || graph[n_num].type == NAND) 
        {
            if(cat == 7)
            {
                temp_p = cat7case23(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp_p.first, n_num, f_inp.id, f_inp.id);
                add_toconvmap(conv_map, temp_pl, temp_p.second, n_num, f_inp.id, f_inp.id);
            }
        }
        else if(graph[n_num].type == OR || graph[n_num].type == NOR)
        {
            if(cat == 7)
            {
                temp = cat7case4(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp, n_num, f_inp.id, f_inp.id);
            }
            else
            {
                temp_p = cat8case14(n_num, f_inp, h_it);
                add_toconvmap(conv_map, temp_pl, temp_p.first, n_num, f_inp.id, f_inp.id);
                add_toconvmap(conv_map, temp_pl, temp_p.second, n_num, f_inp.id, f_inp.id);
            }
        }
    }
}

pair<transient, transient> gen_sim::cat7case23(int n_num, transient p1, transient p2)
{
    bdd bdd_f;
    transient temp1;
    transient temp2;
    pair<transient, transient> ret_t;

    if (graph[n_num].type == OR)
    {
        temp1.type = FALLING;
        temp2.type = FALLING;
    }
    if (graph[n_num].type == NOR)
    {
        temp1.type = RISING;
        temp2.type = RISING;
    }
    if (graph[n_num].type == AND)
    {
        temp1.type = RISING;
        temp2.type = RISING;
    }
    if (graph[n_num].type == NAND)
    {
        temp1.type = FALLING;
        temp2.type = FALLING;
    }
    
    temp1.id = id_n;
    this->id_n++;
    temp1.e_num = p1.e_num;
    temp1.st_time = p1.st_time;
    temp1.end_time = p2.st_time;
    temp1.width = temp1.st_time - temp1.end_time;

    temp2.id = id_n;
    this->id_n++;
    temp2.e_num = p1.e_num;
    temp2.st_time = p2.end_time;
    temp2.end_time = p1.end_time;
    temp2.width = temp2.st_time - temp2.end_time;
    
    set_convprop(n_num, temp1, p1, p2);
    
    ret_t = make_pair(temp1, temp2);
    return ret_t;
}

transient gen_sim::cat7case4(int n_num, transient p1, transient p2)
{
    transient temp;
    
    if (graph[n_num].type == OR)
        temp.type = RISING;
    if (graph[n_num].type == NOR)
        temp.type = FALLING;

    temp.id = id_n;
    this->id_n++;
    temp.e_num = p1.e_num;
    temp.st_time = p2.st_time;
    temp.end_time = p2.end_time;
    temp.width = temp.st_time - temp.end_time;
    
    set_convprop(n_num, temp, p1, p2);

    return temp;
}

pair<transient, transient> gen_sim::cat8case14(int n_num, transient p1, transient p2)
{
    bdd bdd_f;
    transient temp1;
    transient temp2;
    pair<transient, transient> ret_t;
    
    
    if (graph[n_num].type == AND)
    {
        temp1.type = RISING;
        temp2.type = RISING;
    }
    if (graph[n_num].type == NAND)
    {
        temp1.type = FALLING;
        temp2.type = FALLING;
    }
    if (graph[n_num].type == OR)
    {
        temp1.type = FALLING;
        temp2.type = FALLING;
    }
    if (graph[n_num].type == NOR)
    {
        temp1.type = RISING;
        temp2.type = RISING;
    }
    
    temp1.id = id_n;
    this->id_n++;
    temp1.e_num = p1.e_num;
    temp1.st_time = p2.st_time;
    temp1.end_time = p1.st_time;
    temp1.width = temp1.st_time - temp1.end_time;

    temp2.id = id_n;
    this->id_n++;
    temp2.e_num = p1.e_num;
    temp2.st_time = p1.end_time;
    temp2.end_time = p2.end_time;
    temp2.width = temp2.st_time - temp2.end_time;

    set_convprop(n_num, temp1, p1, p2);
    
    ret_t = make_pair(temp1, temp2);
    return ret_t;
}

transient gen_sim::cat8case2(int n_num, transient p1, transient p2)
{
    transient temp;
    
    if (graph[n_num].type == OR)
        temp.type = RISING;
    if (graph[n_num].type == NOR)
        temp.type = FALLING;
    temp.id = id_n;
    this->id_n++;
    temp.e_num = p1.e_num;
    temp.st_time = p2.st_time;
    temp.end_time = p2.end_time;
    temp.width = temp.st_time - temp.end_time;
    
    set_convprop(n_num, temp, p1, p2);
    return temp;
}

/*
 * Determine Convergence Category
 * 0 - pulses are of the same width and arrive at the same time
 * 1 - p1 arrives before p2
 * 2 - p1 arrives after p2
 * 3 - p1 encapsulates p2
 * 4 - p2 encapsulates p1
 * 5 - p1 arrives before p2 -> opposing types
 * 6 - p2 arrives before p1 -> opposing types
 * 7 - p1 encapsulates p2 -> opposing types 
 * 8 - p2 encapsulates p1 -> opposing types
 */
int gen_sim::det_category(transient f_inp, transient h_it)
{
    if(f_inp.type == h_it.type)
    {
        if((f_inp.st_time == h_it.st_time)&&(f_inp.end_time == h_it.end_time))
            return 0;
        if((f_inp.st_time <= h_it.st_time)&&(f_inp.end_time <= h_it.end_time))
            return 1;
        else if((f_inp.st_time >= h_it.st_time)&&(f_inp.end_time >= h_it.end_time))
            return 2;
        else if((f_inp.st_time <= h_it.st_time)&&(f_inp.end_time >= h_it.end_time))
            return 3;
        else if((f_inp.st_time >= h_it.st_time)&&(f_inp.end_time <= h_it.end_time))
            return 4;
    }
    else
    {
        if((f_inp.st_time == h_it.st_time)&&(f_inp.end_time == h_it.end_time))
            return 0;
        if((f_inp.st_time <= h_it.st_time)&&(f_inp.end_time <= h_it.end_time))
            return 5;
        if((f_inp.st_time >= h_it.st_time)&&(f_inp.end_time >= h_it.end_time))
            return 6;
        if((f_inp.st_time <= h_it.st_time)&&(f_inp.end_time >= h_it.end_time))
            return 7;
        if((f_inp.st_time >= h_it.st_time)&&(f_inp.end_time <= h_it.end_time))
            return 8;
    }
}

/*
 * Returns true if an id matches a key inpmap and the element's list also contains hash_id
 * This purpose is to check if the pulse conv_id was created by the pulse hash_id
 */
bool gen_sim::map_check(map<int, list<int> > inpmap, int conv_id, int hash_id)
{
    list<int>::iterator c_it;
    if(find_element(inpmap, conv_id) == true)
    {
        for(c_it = inpmap[conv_id].begin(); c_it != inpmap[conv_id].end(); ++c_it)
        {
            if(*c_it == hash_id)
                return true;
        }
    }
    return false; 
}

// Determines if a pulse is in the previous pulse list
bool gen_sim::is_pulse(list<prev_p> p_pulse, int n_num)
{
    for(list<prev_p>::iterator p_it = p_pulse.begin(); p_it != p_pulse.end(); ++p_it)
        if(p_it->p_gate == n_num)
            return true;
        else
            return false;
}

bool gen_sim::list_check(list<int> inp_l, int val)
{
    for (list<int>::iterator l_it = inp_l.begin(); l_it != inp_l.end(); ++l_it)
    {
        if (*l_it == val)
        {
            return true;
        }
        else
        {
            return false;
        }
    }
}

bool gen_sim::find_element(map<int, list<int> > in_map, int id)
{
    for (map<int, list<int> >::iterator m_it = in_map.begin(); m_it != in_map.end(); ++m_it)
    {
        if (m_it->first == id)
            return true;
        else
            return false;
    }
}

/*
 * Placeholder Function for Delay Calculation
 */
double gen_sim::calc_delay()
{
    return DELAY;
}

/*
 * Check if the given key is in the input map
 */
bool gen_sim::m_find(map<int, double> inpmap, int key)
{
    map<int, double>::iterator m_it;
    
    for(m_it = inpmap.begin(); m_it != inpmap.end(); ++m_it)
    {
        if(m_it->first == key)
            return true;
    }
    return false;
}

/*
 * Find element in list - return true if found
 */
bool gen_sim::l_find(list<int> inplist, int key)
{
    list<int>::iterator lit;
    
    for(lit = inplist.begin(); lit != inplist.end(); ++lit)
    {
        if(*lit == key)
            return true;
    }
    return false;
}

/*
 * Check if a pulse with the same event number exists
 */
bool gen_sim::i_find(list<transient> plist, int key)
{
    list<transient>::iterator lit;
    
    for(lit = plist.begin(); lit != plist.end(); ++lit)
        if(lit->id == key)
            return true;
    
    return false;
}

bool gen_sim::t_find(map<int, list<transient> > inp_list, int key)
{
    map<int, list<transient> >::iterator pit;
    
    for(pit = inp_list.begin(); pit != inp_list.end(); ++pit)
        if(pit->first == key)
            return true;
    
    return false;
}

/*
 * Search a list of all outputs to determine if the node given by "nodeid" is an output
 */
bool out_find(int nodeid)
{
    list<int>::iterator lit;
    
    for(lit = circuit.outputs.begin(); lit != circuit.outputs.end(); lit++)
    {
        if(*lit == nodeid)
            return true;
    }
    return false;
}

/*
 * Label graph partitions
 */
int gen_sim::label_cone()
{
    int part_l(0);
    gmap::reverse_iterator git;
    list<int>::iterator fin;
    list<int>::iterator pit;
    
    for(git = graph.rbegin(); git != graph.rend(); ++git)
    {
        graph[git->first].part.push_back(0);
        /*if(out_find(git->first))
        {
            git->second.part.push_back(part_l);
            part_l++;
        }
        else
        {
            for(fin = graph[git->first].fanout.begin(); fin != graph[git->first].fanout.end(); ++fin)
            {
                for(pit = graph[*fin].part.begin(); pit != graph[*fin].part.end(); ++pit)
                {
                    if(!l_find(graph[git->first].part, *pit))
                        graph[git->first].part.push_back(*pit);
                }
            }
        }*/
    }
    //return part_l-1;
    return 0;
}

/*
 * Create subcircuits based on partition number and set inputs
 */
gmap gen_sim::extract_circuit(int part_num)
{
    int f_count = 0;
    gmap::iterator git;
    gmap temp_graph;
    
    list<int>::iterator lit;
    
    // Create sub-graph
    for(git = graph.begin(); git != graph.end(); ++git)
    {
        if(l_find(graph[git->first].part, part_num))
        {
            temp_graph[git->first] = graph[git->first];
        }
    }
    
    // Update fanin/fanout lists
    int n_count = -1;
    
    for(git = temp_graph.begin(); git != temp_graph.end(); ++git)
    {
        temp_graph[git->first].fanin.clear();
        temp_graph[git->first].fanout.clear();
        for(lit = graph[git->first].fanin.begin(); lit != graph[git->first].fanin.end(); ++lit)
        {
            if(l_find(graph[*lit].part, part_num))
            {
                temp_graph[git->first].fanin.push_back(*lit);
                f_count++;
            }
        }
        temp_graph[git->first].fanin_num = f_count;
        f_count = 0;
        
        for(lit = graph[git->first].fanout.begin(); lit != graph[git->first].fanout.end(); ++lit)
        {
            if(l_find(graph[*lit].part, part_num))
            {
                temp_graph[git->first].fanout.push_back(*lit);
                f_count++;
            }
        }
        temp_graph[git->first].fanout_num = f_count;
        f_count = 0;
        
        /*
        // If cell is missing an input, create a new input in its place
        */
        list<int>::iterator lit;
        
        if((temp_graph[git->first].fanin_num != (graph_m[git->first].fanin_num))&&(temp_graph[git->first].type != INPUT))
        {   
            //int diff = graph_m[git->first].fanin_num - temp_graph[git->first].fanin_num;
       
            for(lit = graph_m[git->first].fanin.begin(); lit != graph_m[git->first].fanin.end(); ++ lit)
            {
                if((graph_m[*lit].type != INPUT)&&(!l_find(temp_graph[git->first].fanin, *lit)))
                {
                    temp_graph[n_count].type = INPUT;
                    temp_graph[n_count].fanout.push_back(git->first);
                    temp_graph[n_count].fanout_num = 1;
                    temp_graph[n_count].prob = graph_m[*lit].prob;
                    //cout<<"Cur Node: "<<git->first<<" Prev: "<<*lit<<" Node: "<<n_count<<" Prob Cir: "<<graph_m[*lit].prob<<endl;
                    //temp_graph[n_count].p_list = graph_m[*lit].p_list;
                    
                    /*cout<<"List of Pulses: "<<*lit<<endl;
                    list<transient>::iterator mit;
                    for(mit = graph_m[*lit].p_list.begin(); mit != graph_m[*lit].p_list.end(); ++mit)
                        cout<<"ID: "<<mit->id<<" Event: "<<mit->e_num<<endl;*/
                    
                    temp_graph[git->first].fanin.push_back(n_count);
                    temp_graph[git->first].fanin_num++;
                    n_count--;
                }
            }
        }
    }
    return temp_graph;
}

/*
 * Circuit Extraction for non-output cone simulation
 */
gmap gen_sim::extract_tcir(int part_num)
{
    int f_count = 0;
    gmap temp_graph;
    gmap::iterator git;
    list<int>::iterator lit;
    
    for(git = graph_m.begin(); git != graph_m.end(); ++git)
    {
        if(graph_m[git->first].static_part == part_num)
        {
            temp_graph[git->first] = graph_m[git->first];
        }
    }
    
    for(git = temp_graph.begin(); git != temp_graph.end(); ++git)
    {
        temp_graph[git->first].fanin.clear();
        temp_graph[git->first].fanout.clear();
        for(lit = graph_m[git->first].fanin.begin(); lit != graph_m[git->first].fanin.end(); ++lit)
        {
            if(graph_m[*lit].static_part == part_num)
            {
                temp_graph[git->first].fanin.push_back(*lit);
                f_count++;
            }
        }
        temp_graph[git->first].fanin_num = f_count;
        f_count = 0;
        
        for(lit = graph_m[git->first].fanout.begin(); lit != graph_m[git->first].fanout.end(); ++lit)
        {
            if(graph_m[*lit].static_part == part_num)
            {
                temp_graph[git->first].fanout.push_back(*lit);
                f_count++;
            }
        }
        temp_graph[git->first].fanout_num = f_count;
        f_count = 0;
    }
    
    int n_count  = -1;
    
    for(git = temp_graph.begin(); git != temp_graph.end(); ++git)
    {
        if((temp_graph[git->first].fanin_num != (graph_m[git->first].fanin_num))&&(temp_graph[git->first].type != INPUT))
        {   
            for(lit = graph_m[git->first].fanin.begin(); lit != graph_m[git->first].fanin.end(); ++ lit)
            {
                if((graph_m[*lit].type != INPUT)&&(!l_find(temp_graph[git->first].fanin, *lit)))
                {
                    temp_graph[n_count].type = INPUT;
                    temp_graph[n_count].fanout.push_back(git->first);
                    temp_graph[n_count].fanout_num = 1;
                    temp_graph[n_count].prob = graph_m[*lit].prob;
                    temp_graph[n_count].static_part = graph_m[*lit].static_part;
                    temp_graph[git->first].fanin.push_back(n_count);
                    temp_graph[git->first].fanin_num++;
                    n_count--;
                }
            }
        }
    }
    
    return temp_graph;
}

/*
 * Conversion of partition to from part_circuit to graph
 */
void gen_sim::conv_tpart(gmap& g_main, int& max_part, int cur_partnum)
{
    gmap::iterator git;  
    max_part++;
    
    for(git = g_main.begin(); git != g_main.end(); ++git)
    {
        if((g_main[git->first].static_part > cur_partnum))
            g_main[git->first].static_part++;
    }
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if((inp_g[git->first].b_part_num == 1) && (git->first >= 0))
        {
            g_main[git->first].static_part++;
        }
    }
}

/*
 * Main Routine to partition a graph
 */
void gen_sim::part_circuit(gmap graphi)
{
    inp_g.clear();
    net_l.clear();
    cell_l.clear();
    bucket.clear();
    locked_cells.clear();
    inp_g = graphi;
    
    load_pstruct();
    calc_csize();
    init_part();
    init_gain();
    move_cells();
    update_inputs();
}

/*
 * Routine to convert circuit to net and cell structures
 */
void gen_sim::load_pstruct()
{
    gmap::iterator git;
    list<int>::iterator fo_it;
    
    int net_index;
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if(inp_g[git->first].type == INPUT)
        {
            if(inp_g[git->first].fanout_num > 1)
            {
                for(fo_it = inp_g[git->first].fanout.begin(); fo_it != inp_g[git->first].fanout.end(); ++fo_it)
                {
                    net_l[git->first].push_back(*fo_it);
                    cell_l[*fo_it].push_back(git->first);
                }
            }
            net_index = git->first + 1;
        }
        else
        {
            if(inp_g[git->first].fanout_num > 0)
            {
                net_l[net_index].push_back(git->first);
                cell_l[git->first].push_back(net_index);
                
                for(fo_it = inp_g[git->first].fanout.begin(); fo_it != inp_g[git->first].fanout.end(); ++fo_it)
                {
                    net_l[net_index].push_back(*fo_it);
                    cell_l[*fo_it].push_back(net_index);
                }
                net_index++;
            }
        }
    }
}

/*
 * Determine the number of pins (size), total size and the max size
 */
void gen_sim::calc_csize()
{
    int tmp_pins;
    total_size = 0;
    max_pins = 0;
    max_size = 0;
    gmap::iterator git;
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if(inp_g[git->first].type != INPUT)
        {
            // Size equals number of pins
            inp_g[git->first].pins = inp_g[git->first].fanin_num + 1;
            if(inp_g[git->first].pins > max_pins)
                max_pins = inp_g[git->first].pins;
            
            tmp_pins = inp_g[git->first].pins;
            bucket[tmp_pins].push_back(git->first);
            
            // Determine the size of the cell
            inp_g[git->first].c_size = inp_g[git->first].fanin_num + inp_g[git->first].fanout_num;
            total_size = total_size + inp_g[git->first].c_size;
            if(inp_g[git->first].c_size > max_size)
                max_size = inp_g[git->first].c_size;
        }
        else
        {
            inp_g[git->first].c_size = 0;
            inp_g[git->first].gain = 0;
        }
    }
}

/*
 * Routine to intialize the partition based on a ratio
 */
void gen_sim::init_part()
{
    size_part1 = 0;
    int cur_size = 0;
    gmap::iterator git;
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if(cur_size < (total_size*PART_RATIO))
        {
            inp_g[git->first].b_part_num = 1;
            cur_size = cur_size + inp_g[git->first].c_size;
            size_part1 = size_part1+inp_g[git->first].c_size;
        }
        else
            inp_g[git->first].b_part_num = 2;
    }
    size_part2 = total_size - size_part1;
}

/*
 * Routine to initialize the gain
 */
void gen_sim::init_gain()
{
    int from, to;
    int f_num = 0;
    int t_num = 0;
    list<int>::iterator nit;
    list<int>::iterator nit2;
    map<int, list<int> >::iterator cit;
    
    //print_pstruct();
    
    for(cit = cell_l.begin(); cit != cell_l.end(); ++cit)
    {
        inp_g[cit->first].gain = 0;
        from = inp_g[cit->first].b_part_num;
        if(from == 1)
            to = 2;
        else
            to = 1;
        
        for(nit = cell_l[cit->first].begin(); nit != cell_l[cit->first].end(); ++nit)
        {
            // Count the number of cells in each partition connected to the net
            for(nit2 = net_l[*nit].begin(); nit2 != net_l[*nit].end(); ++nit2)
            {
                if(inp_g[*nit2].b_part_num == from)
                    f_num++;
                else
                    t_num++;
            }
            
            if(f_num == 1)
                inp_g[cit->first].gain++;
            if(t_num == 0)
                inp_g[cit->first].gain--;
            
            f_num = 0;
            t_num = 0;
        }
    }
}

/*
 * Determine if move will upset balance
 */
bool gen_sim::check_balance(int cell_in)
{
    int part = inp_g[cell_in].b_part_num;
    int top_range = int(PART_RATIO*total_size) + max_size;
    int bottom_range = int(PART_RATIO*total_size) - max_size;
    
    if((part == 1)&&((inp_g[cell_in].c_size + size_part2) <= top_range)&&((size_part1 - inp_g[cell_in].c_size) >= bottom_range))
        return true;
    else if ((part == 2)&&((inp_g[cell_in].c_size + size_part1) <= top_range)&&((size_part2 - inp_g[cell_in].c_size) >= bottom_range))
        return true;
    else
        return false;
}

/*
 * Determine the maximum gain in the bucket
 */
int gen_sim::calc_maxgain()
{
    int p_val = -max_pins;
    bool val;
    
    map<int, list<int> >::iterator bit;
    list<int>::iterator lit;
    
    for(bit = bucket.begin(); bit != bucket.end(); ++bit)
    {
        for(lit = bucket[bit->first].begin(); lit != bucket[bit->first].end(); ++lit)
        {
            if(check_balance(*lit))
            {
                if((bit->first > p_val)&&(!bucket[bit->first].empty()))
                    p_val = bit->first;
            }
        }
    }
    return p_val;
}

/*
 * Check if the cell is free
 */
bool gen_sim::is_cell_free(int cell_n)
{
    list<int>::iterator cit;
    
    for(cit = locked_cells.begin(); cit != locked_cells.end(); ++cit)
    {
        if(cell_n == *cit)
            return false;
    }
    return true;
    
}

/*
 * Check if the bucket is empty
 */
bool gen_sim::is_bucket_empty()
{
    map<int, list<int> >::iterator bit;
    
    for(bit = bucket.begin(); bit != bucket.end(); ++bit)
    {
        if(!bucket[bit->first].empty())
            return false;
    }
    return true;
}

/*
 * Routine to move cells for partitioning
 */
void gen_sim::move_cells()
{
    int mgain_val = 0;
    int cur_cell;
    int from , to;
    int fnum = 0;
    int tnum = 0;
    
    list<int>::iterator cit;
    list<int>::iterator nit;
    list<int>::iterator bit;
    
    mgain_val = calc_maxgain();

    while((mgain_val >= 0) && !is_bucket_empty())
    {
        for(bit = bucket[mgain_val].begin(); bit != bucket[mgain_val].end(); ++bit)
        {
            if(check_balance(*bit))
            {
                cur_cell = *bit;
                bucket[mgain_val].erase(bit);
                break;
            }
        }
        
        from = inp_g[cur_cell].b_part_num;
        if(from == 1)
            to = 2;
        else
            to = 1;

        locked_cells.push_back(cur_cell);

        inp_g[cur_cell].b_part_num = to;
        
        if(to == 1)
        {
            size_part1 = size_part1 + inp_g[cur_cell].c_size;
            size_part2 = size_part2 - inp_g[cur_cell].c_size;
        }
        else
        {
            size_part2 = size_part2 + inp_g[cur_cell].c_size;
            size_part1 = size_part1 - inp_g[cur_cell].c_size;
        }       
        
        for(cit = cell_l[cur_cell].begin(); cit != cell_l[cur_cell].end(); ++cit)
        {
            for (nit = net_l[*cit].begin(); nit != net_l[*cit].end(); ++nit)
            {
                if (inp_g[*nit].b_part_num == from)
                    fnum++;
                else
                    tnum++;
            }
            
            if(tnum == 0)
            {
                for(nit = net_l[*cit].begin(); nit != net_l[*cit].end(); ++nit)
                {
                    if(is_cell_free(*nit))
                         inp_g[*nit].gain++;   
                }
            }
            else if(tnum == 1)
            {
                for(nit = net_l[*cit].begin(); nit != net_l[*cit].end(); ++nit)
                {
                    if((inp_g[*nit].b_part_num == to) && (is_cell_free(*nit)))
                        inp_g[*nit].gain--;
                }
            }
            
            fnum--;
            tnum++;
            
            if(fnum == 0)
            {
                for(nit = net_l[*cit].begin(); nit != net_l[*cit].end(); ++nit)
                {
                    if(is_cell_free(*nit))
                         inp_g[*nit].gain--;   
                }
            }
            else if(fnum == 1)
            {
                for(nit = net_l[*cit].begin(); nit != net_l[*cit].end(); ++nit)
                {
                    if(is_cell_free(*nit))
                         inp_g[*nit].gain++;   
                }
            }
        }
        fnum = 0;
        tnum = 0;
        mgain_val = calc_maxgain();
    } 
}

void gen_sim::update_inputs()
{
    int part_num;
    int part_count = 0;
    double fo_limit;
    gmap::iterator git;
    list<int>::iterator lit;
    
    for(git = inp_g.begin(); git != inp_g.end(); ++git)
    {
        if(inp_g[git->first].type == INPUT)
        {
            //cout<<"Node: "<<git->first<<" Part: "<<inp_g[git->first].b_part_num<<endl;
            part_num = inp_g[git->first].b_part_num;
            
            for(lit = inp_g[git->first].fanout.begin(); lit != inp_g[git->first].fanout.end(); ++lit)
            {
                if(part_num != inp_g[*lit].b_part_num)
                {
                    part_count++;
                }
            }
            fo_limit = 0.5*inp_g[git->first].fanout_num;
            
            if(part_count > fo_limit)
            {
                if(inp_g[git->first].b_part_num == 1)
                    inp_g[git->first].b_part_num =2;
                else
                    inp_g[git->first].b_part_num = 1;
            }
            part_count = 0;
        }
    }
}

void gen_sim::print_pstruct()
{
    map<int, list<int> >::iterator n_it;
    map<int, list<int> >::iterator c_it;
    
    list<int>::iterator nl_it;
    list<int>::iterator cl_it;
    
    
    cout<<"Net List: "<<endl;
    for(n_it = net_l.begin(); n_it != net_l.end(); ++n_it)
    {
        cout<<n_it->first<<": ";
        for(nl_it = net_l[n_it->first].begin(); nl_it != net_l[n_it->first].end(); ++nl_it)
        {
            cout<<*nl_it<<" ";
        }
        cout<<endl;
    }
    
    cout<<"Cell List: "<<endl;
    for(c_it = cell_l.begin(); c_it != cell_l.end(); ++c_it)
    {
        cout<<c_it->first<<": ";
        for(cl_it = cell_l[c_it->first].begin(); cl_it != cell_l[c_it->first].end(); ++cl_it)
        {
            cout<<*cl_it<<" ";
        }
        cout<<endl;
    }
}

/*
 * Assign Levels to the Circuit
 */
void gen_sim::level_circuit(gmap& inp_graph)
{
    int max_level;
    list<int>::iterator lit;
    gmap::iterator git;
    
    for(git = inp_graph.begin(); git != inp_graph.end(); ++git)
    {
        if(inp_graph[git->first].type == INPUT)
        {
            inp_graph[git->first].level = 0;
            //level_map[0].push_back(git->first);
        }
        else if(inp_graph[git->first].type == BUF || inp_graph[git->first].type == NOT)
        {
            for(lit = inp_graph[git->first].fanin.begin(); lit != inp_graph[git->first].fanin.end(); ++lit)
            {
                inp_graph[git->first].level = inp_graph[*lit].level;
                //level_map[inp_graph[*lit].level].push_back(git->first);
            }
        }
        else if(inp_graph[git->first].type != NA)
        {
            max_level = 0;
            
            for(lit = inp_graph[git->first].fanin.begin(); lit != inp_graph[git->first].fanin.end(); ++lit)
            {
                if(inp_graph[*lit].level > max_level)
                    max_level = inp_graph[*lit].level;
            }
            max_level++;
            inp_graph[git->first].level = max_level;
            //level_map[max_level].push_back(git->first);
        }
        else
            cout<<"Invalid Node Type\n";
    }
}
