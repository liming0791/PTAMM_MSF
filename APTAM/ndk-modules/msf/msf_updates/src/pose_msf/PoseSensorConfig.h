#ifndef POSESENSORCONFIG
#define POSESENSORCONFIG


// TODO::Now the config is fixed, but it should be 
// read from file
class PoseSensorConfig 
{
    public:

    bool core_init_filter;
    bool core_set_height;
    double core_height;

    double pose_initial_scale;
    bool pose_fixed_scale;
    bool pose_fixed_p_ic;
    bool pose_fixed_q_ic;
    bool pose_fixed_p_wv;
    bool pose_fixed_q_wv;

    double pose_noise_scale;

    double pose_noise_p_wv;
    double pose_noise_q_wv;
    double pose_noise_p_ic;
    double pose_noise_q_ic;
    double pose_noise_meas_p;
    double pose_noise_meas_q;
    double pose_delay;

    PoseSensorConfig(): 
            core_init_filter(false),
            core_set_height(false),
            core_height(1),

            pose_initial_scale(1),
            pose_fixed_scale(false),
            pose_fixed_p_ic(true),
            pose_fixed_q_ic(true),
            pose_fixed_p_wv(true),
            pose_fixed_q_wv(true),

            pose_noise_scale(0.003),

            pose_noise_p_wv(0),
            pose_noise_q_wv(0),
            pose_noise_p_ic(0),
            pose_noise_q_ic(0),
            pose_noise_meas_p(0.003),
            pose_noise_meas_q(0.003),
            pose_delay(0.02){}
};

#endif
