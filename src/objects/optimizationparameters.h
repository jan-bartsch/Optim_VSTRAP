#pragma once

#include <string>

namespace optimizationParameters{

struct globalparameters {
    bool simulating_plasma;
    bool fmm;
    bool magnetic_force;
    bool electric_force;

    double armijo_descent_fraction;
    bool calculation_functional;
    double C_theta;
    double C_phi;
    double sigma_x;
    double sigma_v;

    //create adjoint particles and trajectory
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

    //MCC parameters
    double MCC_temperature;
    double MCC_species;
    double MCC_mass;
    double MCC_fixed_number_density;

    //forward input
    std::string creation_forward_particles_method; // create_existing create_new only_inflow
    std::string initial_condition_file; //value="plasma_state_batch_1_forward_particles_CPU_1.csv"/> <!-- only in effect if create_existing
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

    bool mesh_2d_writer_included; //  1=true, 0=false
    std::string mesh_2d_name;
    int mesh_2d_outputinterval;

    bool mesh_3d_writer_included; // value="0"  1=true, 0=false
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

    //optimization techniques
    double fixed_gradient_descent_stepsize;
    double fraction_of_optimal_control;
    int optimizationIteration_max_gp;
    int linesearchIteration_max_gp;
    int numberParticles_gp;
    int start_zero_control; // 1=true, 0=false (start with given control) , 2: start with given files  -->
    double tolerance_gp;
    double tolerance_relaxation_gp;
    double armijio_base_exp; //  <!--  1.0: there is no exponential diminishing of fraction of new stepsize -->
    double armijo_iterative_exp;

    // DOMAIN PARAMETERS
    double position_max_gp;

    unsigned int number_cells_position;
    double small_discr_volume;
    double small_discr_sidelength;
    unsigned int dimensionOfControl_gp;
    unsigned int end_control_gp;
    unsigned int start_control_gp;



    double dt_gp;
    double dt_VSTRAP;
    unsigned int ntimesteps_gp;
    int plasma_state_output_interval;
    unsigned int ntimesteps_gp_VSTRAP; // ntimesteps_gp*plasma_state_output_interval -->


    double dv_gp;
    unsigned int vcell_gp;
    double vmax_gp;

    double fraction_fast_particles_gp;

    //CONTROL
    double local_control_x_min_gp;
    double local_control_x_max_gp;
    double weight_control_gp;
    double velocity_part_objective;
    double shifting_objective;
    double visalization_scaling;
    double fabs_tol_gp;
};

struct optimizationPaths {
    std::string BUILD_DIRECTORY_VSTRAP;
    std::string BUILD_DIRECTORY_OPTIM;
   std::string PATH_TO_SHARED_FILES_ABSOLUTE;

      std::string PVPYTHON_ABSOLUTE_DIR;


      std::string BARYCENTERS_MESH;
      std::string DOMAIN_MESH;
      std::string DOMAIN_MESH_FILE;

     std::string SURFACE_MESH;

    // shared
      std::string INPUT_FORWARD;
      std::string INPUT_BACKWARD;
      std::string RESULTS_DIRECTORY;
      std::string PATH_TO_SHARED_FILES;
      std::string DIRECTORY_TOOLSET;

      std::string CREATION_ADJOINT_PARTCLES;
      std::string BGF_CONTROL;
     std::string CONTROL_FIELD_CELLS_NAME;

     std::string START_WITH_EXISTING_CONTROL;

    std::string DSMC_interaction;
     std::string MCC_interaction;


      std::string mesh_3d_path;
     std::string mesh_2d_path;
};

int LoadParameters();

}
