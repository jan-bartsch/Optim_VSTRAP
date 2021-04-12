#pragma once

#include <string>
#include <iostream>

#include "../objects/dataprovider.h"

namespace MOTIONS {


struct InputData {
    int simulating_plasma;
    int fmm;
    int magnetic_force;
    int electric_force;

    double armijo_descent_fraction;
    int calculation_functional;
    int calculation_wasserstein;
    double C_theta;
    double C_phi;
    double sigma_x;
    double sigma_v;

    // create adjoint particles and trajectory
    double adjoint_mu_x;
    double desired_position_x;
    double adjoint_mu_y;
    double desired_position_y;
    double adjoint_mu_z;
    double desired_position_z;
    double adjoint_s_x;
    double adjoint_s_y;
    double adjoint_s_z;
    double adjoint_vx;
    double adjoint_vy;
    double adjoint_vz;
    double adjoint_number_density;
    double adjoint_weight;
    double adjoint_charge_number;
    double adjoint_mass;
    double adjoint_species;
    double expected_speed;
    double most_probable_speed;

    // MCC parameters
    double MCC_temperature;
    double MCC_species;
    double MCC_mass;
    double MCC_fixed_number_density;

    // forward input
    std::string
    creation_forward_particles_method; // create_existing create_new
    // only_inflow
    std::string
    initial_condition_file; // value="plasma_state_batch_1_forward_particles_CPU_1.csv"/>
    // <!-- only in effect if create_existing
    double number_density_forward;
    double weight_forward;
    double charge_number_forward;
    double mass_forward;
    double species_forward;
    double temperature_x_val;
    double temperature_y_val;
    double temperature_z_val;
    double v_drift_x_val;
    double v_drift_y_val;
    double v_drift_z_val;

    int mesh_2d_writer_included; //  1=true, 0=false
    std::string mesh_2d_name;
    int mesh_2d_outputinterval;

    int mesh_3d_writer_included; // value="0"  1=true, 0=false
    std::string mesh_3d_name;
    int mesh_3d_outputinterval;

    bool inflow_included;
    double number_density_forward_inflow;
    double weight_forward_inflow;
    int charge_number_forward_inflow;
    double mass_forward_inflow;
    std::string species_forward_inflow;
    double temperature_x_val_inflow;
    double temperature_y_val_inflow;
    double temperature_z_val_inflow;
    double v_drift_x_val_inflow;
    double v_drift_y_val_inflow;
    double v_drift_z_val_inflow;

    // optimization techniques
    double fixed_gradient_descent_stepsize;
    double fraction_of_optimal_control;
    unsigned int optimization_iteration_max;
    int linesearchIteration_max_gp;
    int numberParticles_gp;
    int start_zero_control; // 1=true, 0=false (start with given control) ,
    // 2: start with given files  -->
    double tolerance_gp;
    double tolerance_relaxation_gp;
    double
    armijio_base_exp; //  <!--  1.0: there is no exponential diminishing of
    //  fraction of new stepsize -->
    double armijo_iterative_exp;

    // DOMAIN PARAMETERS
    double position_max_gp;

    unsigned int number_cells_position;
    double small_discr_volume;
    double small_discr_sidelength;
    unsigned int dimension_control;
    unsigned int end_control_gp;
    unsigned int start_control_gp;

    double dt_gp;
    double dt_VSTRAP;
    unsigned int ntimesteps_gp;
    int plasma_state_output_interval;
    unsigned int
    ntimesteps_gp_VSTRAP; // ntimesteps_gp*plasma_state_output_interval -->

    double dv_gp;
    unsigned int vcell_gp;
    double vmax_gp;

    double fraction_fast_particles_gp;

    // CONTROL
    double local_control_x_min_gp;
    double local_control_x_max_gp;
    double weight_control_gp;
    double velocity_part_objective;
    double shifting_objective;
    double visalization_scaling;
    double fabs_tol_gp;

    std::string build_directory_vstrap;
    std::string build_directory_optim;
    std::string path_to_shared_files_absolute;

    std::string pvpython_absolute_dir;

    // meshes
    std::string barycenters_mesh;
    std::string domain_mesh;
    std::string domain_mesh_file;
    std::string surface_mesh;

    // shared
    std::string input_forward;
    std::string input_backward;
    std::string results_directory;
    std::string path_to_shared_files;
    std::string directory_toolset;

    std::string creation_adjoint_partcles;
    std::string bgf_control;
    std::string control_field_cells_name;

    std::string start_with_existing_control;

    std::string DSMC_interaction;
    std::string MCC_interaction;

    std::string mesh_3d_path;
    std::string mesh_2d_path;

    std::string control_update;
    std::string direction_update;
    std::string desired_trajectory;
    std::string objective_calculation;
};

class InitializeMotions {

public:
    static InputData Load_MOTIONS(DataProvider &data_provider_opt);
    static const char* GenerateInputPath(int argc, const char **argv);

private:
    static  MOTIONS::InputData LoadInputData(DataProvider &data_provider_opt);
};


};

// namespace MOTIONS
