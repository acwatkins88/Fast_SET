#include "node.h"

transient::transient()
{
    this->type = 0;
    this->r_slope = 0;
    this->f_slope = 0;
    this->width = 0;
    this->peak = 0;
    this->r_time = 0;
    this->f_time = 0;
    this->end_time = 0;
    this->delay = 0;
}

transient::~transient()
{
    //this->p_func.~bdd();
}

/*
 * Evaluate pulse voltage at a given time
 * time - the desired time to be solved
 * rf - flag to determe if the transistion is rising or falling
 * sttime - the start time of the transient
 */
double transient::eval_volt(double time, int rf, double sttime)
{

    double rel_time = time - sttime;
    double volt;

    /*
     * Evaluation of LHL transient pulses
     */
    if (this->type == LHL)
    {
        if (rel_time < this->r_time)
        {
            volt = this->r_slope*rel_time;
            return volt;
        }
        else if ((rel_time > this->r_time) && (rel_time < this->f_time))
        {
            volt = this->peak;
            return volt;
        }
        else if ((rel_time > this->f_time) && (rel_time <= this->end_time))
        {
            volt = this->f_slope*rel_time;
            return volt;
        }
    }

    return 0;
}

/*
 * Evaluate time point at a given voltage
 */
double transient::eval_time(double volt, int rf, double sttime)
{
    return 0;
}

node::node()
{
    this->type = NA;
    this->fanout_num = 0;
    this->fanin_num = 0;
    this->val = 5;
    this->pulse = false;
    this->proc_flag = 0;
    this->del_flag = 0;
    this->prob = 0;

    //this->r_width = rand() % MAX_WIDTH + 1;
    //this->f_width = rand() % MAX_WIDTH + 1;
    // Temp Delay Parameter
    //this->delay = 4;
    //this->out_cap = 0;
    //this->inter_cap = 0;
}

node::~node()
{

}