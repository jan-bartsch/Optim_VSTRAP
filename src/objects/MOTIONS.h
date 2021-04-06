#pragma once

#include <string>

#include "../objects/dataprovider.h"

namespace MOTIONS{

struct params {
    static int simulating_plasma;
    static int fmm;
    static int magnetic_force;
    static int electric_force;

    static double armijo_descent_fraction;
    static int calculation_functional;
    static int calculation_wasserstein;
    static double C_theta;
    static double C_phi;
    static double sigma_x;
    static double sigma_v;

    //create adjoint particles and trajectory
    static double adjoint_mu_x;
    static double desired_position_x;
    static double adjoint_mu_y;
    static double desired_position_y;
    static double adjoint_mu_z;
    static double desired_position_z;
    static double adjoint_s_x;
    static double adjoint_s_y;
    static double adjoint_s_z;
    static double adjoint_vx;
    static double adjoint_vy;
    static double adjoint_vz;
    static double adjoint_number_density;
    static double adjoint_weight;
    static double adjoint_charge_number;
    static double adjoint_mass;
    static double adjoint_species;
    static double expected_speed;
    static double most_probable_speed;

    //MCC parameters
    static double MCC_temperature;
    static double MCC_species;
    static double MCC_mass;
    static double MCC_fixed_number_density;

    //forward input
    static std::string creation_forward_particles_method; // create_existing create_new only_inflow
    static std::string initial_condition_file; //value="plasma_state_batch_1_forward_particles_CPU_1.csv"/> <!-- only in effect if create_existing
    static double number_density_forward;
    static double weight_forward;
    static double charge_number_forward;
    static double mass_forward;
    static double species_forward;
    static double temperature_x_val;
    static double temperature_y_val;
    static double temperature_z_val;
    static double v_drift_x_val;
    static double v_drift_y_val;
    static double v_drift_z_val;

    static int mesh_2d_writer_included; //  1=true, 0=false
    static std::string mesh_2d_name;
    static int mesh_2d_outputinterval;

    static int mesh_3d_writer_included; // value="0"  1=true, 0=false
    static std::string mesh_3d_name;
    static int mesh_3d_outputinterval;

    static bool inflow_included;
    static double number_density_forward_inflow;
    static double weight_forward_inflow;
    static int charge_number_forward_inflow;
    static double mass_forward_inflow;
    static std::string species_forward_inflow;
    static double temperature_x_val_inflow;
    static double temperature_y_val_inflow;
    static double temperature_z_val_inflow;
    static double v_drift_x_val_inflow;
    static double v_drift_y_val_inflow;
    static double v_drift_z_val_inflow;

    //optimization techniques
    static double fixed_gradient_descent_stepsize;
    static double fraction_of_optimal_control;
    static unsigned int optimization_iteration_max;
    static int linesearchIteration_max_gp;
    static int numberParticles_gp;
    static int start_zero_control; // 1=true, 0=false (start with given control) , 2: start with given files  -->
    static double tolerance_gp;
    static double tolerance_relaxation_gp;
    static double armijio_base_exp; //  <!--  1.0: there is no exponential diminishing of fraction of new stepsize -->
    static double armijo_iterative_exp;

    // DOMAIN PARAMETERS
    static double position_max_gp;

    static unsigned int number_cells_position;
    static double small_discr_volume;
    static double small_discr_sidelength;
    static unsigned int dimension_control;
    static unsigned int end_control_gp;
    static unsigned int start_control_gp;

    static double dt_gp;
    static double dt_VSTRAP;
    static unsigned int ntimesteps_gp;
    static int plasma_state_output_interval;
    static unsigned int ntimesteps_gp_VSTRAP; // ntimesteps_gp*plasma_state_output_interval -->

    static double dv_gp;
    static unsigned int vcell_gp;
    static double vmax_gp;

    static double fraction_fast_particles_gp;

    //CONTROL
    static double local_control_x_min_gp;
    static double local_control_x_max_gp;
    static double weight_control_gp;
    static double velocity_part_objective;
    static double shifting_objective;
    static double visalization_scaling;
    static double fabs_tol_gp;
};

struct paths {
    static std::string build_directory_vstrap;
    static std::string build_directory_optim;
    static std::string path_to_shared_files_absolute;

    static std::string pvpython_absolute_dir;

    //meshes
    static std::string barycenters_mesh;
    static std::string domain_mesh;
    static std::string domain_mesh_file;
    static std::string surface_mesh;

    // shared
    static std::string input_forward;
    static std::string input_backward;
    static std::string results_directory;
    static std::string path_to_shared_files;
    static std::string directory_toolset;

    static std::string creation_adjoint_partcles;
    static std::string bgf_control;
    static std::string control_field_cells_name;

    static std::string start_with_existing_control;

    static std::string DSMC_interaction;
    static std::string MCC_interaction;

    static std::string mesh_3d_path;
    static std::string mesh_2d_path;
};

struct subroutines {
    static std::string control_update;
    static std::string direction_update;
    static std::string desired_trajectory;
    static std::string objective_calculation;
};

int Load_MOTIONS(DataProvider &data_provider_opt);

int LoadParameters(DataProvider &data_provider_opt);
int LoadPaths(DataProvider &data_provider_opt);
int LoadSubroutines(DataProvider &data_provider_opt);

}
