#include "MOTIONS.h"


int MOTIONS::Load_MOTIONS(DataProvider &data_provider_opt) {

    LoadParameters(data_provider_opt);
    LoadPaths(data_provider_opt);
    LoadSubroutines(data_provider_opt);

    return 0;
}

int MOTIONS::LoadParameters(DataProvider &data_provider_opt) {

    std::map<std::string, std::string> optimizationParameters = data_provider_opt.getOptimizationParameters();


    MOTIONS::params::simulating_plasma = std::stoi(optimizationParameters.at("simulating_plasma"));
    MOTIONS::params::fmm = std::stoi(optimizationParameters.at("fmm"));
    MOTIONS::params::magnetic_force = std::stoi(optimizationParameters.at("magnetic_force"));
    MOTIONS::params::electric_force = std::stoi(optimizationParameters.at("electric_force"));

    MOTIONS::params::armijo_descent_fraction = std::stod(optimizationParameters.at("armijo_descent_fraction"));
    MOTIONS::params::calculation_functional= std::stoi(optimizationParameters.at("calculation_functional"));
    MOTIONS::params::C_theta= std::stod(optimizationParameters.at("C_theta"));
    MOTIONS::params::C_phi= std::stod(optimizationParameters.at("C_phi"));
    MOTIONS::params::sigma_x= std::stod(optimizationParameters.at("sigma_x"));
    MOTIONS::params::sigma_v= std::stod(optimizationParameters.at("sigma_v"));


    //create adjoint particles and trajectory
    MOTIONS::params::adjoint_mu_x = std::stod(optimizationParameters.at("adjoint_mu_x"));
    MOTIONS::params::desired_position_x = std::stod(optimizationParameters.at("desired_position_x"));
    MOTIONS::params::adjoint_mu_y = std::stod(optimizationParameters.at("adjoint_mu_y"));
    MOTIONS::params::desired_position_y = std::stod(optimizationParameters.at("desired_position_y"));
    MOTIONS::params::adjoint_mu_z = std::stod(optimizationParameters.at("adjoint_mu_z"));
    MOTIONS::params::desired_position_z = std::stod(optimizationParameters.at("desired_position_z"));
    MOTIONS::params::adjoint_s_x  = std::stod(optimizationParameters.at("adjoint_s_x"));
    MOTIONS::params::adjoint_s_y  = std::stod(optimizationParameters.at("adjoint_s_y"));
    MOTIONS::params::adjoint_s_z  = std::stod(optimizationParameters.at("adjoint_s_z"));
    MOTIONS::params::adjoint_vx  = std::stod(optimizationParameters.at("adjoint_vx"));
    MOTIONS::params::adjoint_vy  = std::stod(optimizationParameters.at("adjoint_vy"));
    MOTIONS::params::adjoint_vz  = std::stod(optimizationParameters.at("adjoint_vz"));
    MOTIONS::params::adjoint_number_density  = std::stod(optimizationParameters.at("adjoint_number_density"));
    MOTIONS::params::adjoint_weight  = std::stod(optimizationParameters.at("adjoint_weight"));
    MOTIONS::params::adjoint_charge_number  = std::stod(optimizationParameters.at("adjoint_charge_number"));
    MOTIONS::params::adjoint_mass = std::stod(optimizationParameters.at("adjoint_mass"));
    MOTIONS::params::adjoint_species = std::stod(optimizationParameters.at("adjoint_species"));
    MOTIONS::params::expected_speed = std::stod(optimizationParameters.at("expected_speed"));
    MOTIONS::params::most_probable_speed = std::stod(optimizationParameters.at("most_probable_speed"));

    //MCC parameters
    MOTIONS::params::MCC_temperature = std::stod(optimizationParameters.at("MCC_temperature"));
    MOTIONS::params::MCC_species =std::stod(optimizationParameters.at("MCC_species"));
    MOTIONS::params::MCC_mass =std::stod(optimizationParameters.at("MCC_mass"));
    MOTIONS::params::MCC_fixed_number_density =std::stod(optimizationParameters.at("MCC_fixed_number_density"));

    //forward input
    MOTIONS::params::creation_forward_particles_method = optimizationParameters.at("most_probable_speed");
    MOTIONS::params::initial_condition_file = optimizationParameters.at("most_probable_speed");
    MOTIONS::params::number_density_forward =std::stod(optimizationParameters.at("number_density_forward"));
    MOTIONS::params::weight_forward =std::stod(optimizationParameters.at("weight_forward"));
    MOTIONS::params::charge_number_forward =std::stod(optimizationParameters.at("charge_number_forward"));
    MOTIONS::params::mass_forward =std::stod(optimizationParameters.at("mass_forward"));
    MOTIONS::params::species_forward =std::stod(optimizationParameters.at("species_forward"));
    MOTIONS::params::temperature_x_val =std::stod(optimizationParameters.at("temperature_x_val"));
    MOTIONS::params::temperature_y_val =std::stod(optimizationParameters.at("temperature_y_val"));
    MOTIONS::params::temperature_z_val =std::stod(optimizationParameters.at("temperature_z_val"));
    MOTIONS::params::v_drift_x_val =std::stod(optimizationParameters.at("v_drift_x_val"));
    MOTIONS::params::v_drift_y_val =std::stod(optimizationParameters.at("v_drift_y_val"));
    MOTIONS::params::v_drift_z_val =std::stod(optimizationParameters.at("v_drift_z_val"));

    MOTIONS::params::mesh_2d_writer_included = std::stoi(optimizationParameters.at("mesh_2d_writer_included"));
    MOTIONS::params::mesh_2d_name = optimizationParameters.at("mesh_2d_name");
    MOTIONS::params::mesh_2d_outputinterval = std::stoi(optimizationParameters.at("mesh_2d_outputinterval"));

    MOTIONS::params::mesh_3d_writer_included = std::stoi(optimizationParameters.at("mesh_3d_writer_included"));
    MOTIONS::params::mesh_3d_name = optimizationParameters.at("mesh_3d_name");
    MOTIONS::params::mesh_3d_outputinterval = std::stoi(optimizationParameters.at("mesh_3d_outputinterval"));

    MOTIONS::params::inflow_included = std::stoi(optimizationParameters.at("inflow_included"));
    MOTIONS::params::number_density_forward_inflow = std::stod(optimizationParameters.at("number_density_forward_inflow"));
    MOTIONS::params::weight_forward_inflow = std::stod(optimizationParameters.at("weight_forward_inflow"));
    MOTIONS::params::charge_number_forward_inflow = std::stoi(optimizationParameters.at("charge_number_forward_inflow"));
    MOTIONS::params::mass_forward_inflow = std::stod(optimizationParameters.at("mass_forward_inflow"));
    MOTIONS::params::species_forward_inflow = optimizationParameters.at("species_forward_inflow");
    MOTIONS::params::temperature_x_val_inflow = std::stod(optimizationParameters.at("temperature_x_val_inflow"));
    MOTIONS::params::temperature_y_val_inflow = std::stod(optimizationParameters.at("temperature_y_val_inflow"));
    MOTIONS::params::temperature_z_val_inflow = std::stod(optimizationParameters.at("temperature_z_val_inflow"));
    MOTIONS::params::v_drift_x_val_inflow = std::stod(optimizationParameters.at("v_drift_x_val_inflow"));
    MOTIONS::params::v_drift_y_val_inflow = std::stod(optimizationParameters.at("v_drift_y_val_inflow"));
    MOTIONS::params::v_drift_z_val_inflow = std::stod(optimizationParameters.at("v_drift_z_val_inflow"));

    //optimization techniques
    MOTIONS::params::fixed_gradient_descent_stepsize = std::stod(optimizationParameters.at("fixed_gradient_descent_stepsize"));
    MOTIONS::params::fraction_of_optimal_control = std::stod(optimizationParameters.at("fraction_of_optimal_control"));
    static unsigned int optimization_iteration_max;
    MOTIONS::params::linesearchIteration_max_gp = std::stoi(optimizationParameters.at("linesearchIteration_max_gp"));
    MOTIONS::params::numberParticles_gp = std::stoi(optimizationParameters.at("numberParticles_gp"));
    MOTIONS::params::start_zero_control = std::stoi(optimizationParameters.at("start_zero_control"));
    MOTIONS::params::tolerance_gp = std::stod(optimizationParameters.at("tolerance_gp"));
    MOTIONS::params::tolerance_relaxation_gp = std::stod(optimizationParameters.at("tolerance_relaxation_gp"));
    MOTIONS::params::armijio_base_exp = std::stod(optimizationParameters.at("armijio_base_exp"));
    MOTIONS::params::armijo_iterative_exp = std::stod(optimizationParameters.at("armijo_iterative_exp"));

    // DOMAIN PARAMETERS
    MOTIONS::params::position_max_gp = std::stod(optimizationParameters.at("position_max_gp"));

    static unsigned int number_cells_position;
    MOTIONS::params::small_discr_volume = std::stod(optimizationParameters.at("small_discr_volume"));
    MOTIONS::params::small_discr_sidelength = std::stod(optimizationParameters.at("small_discr_sidelength"));
    static unsigned int dimension_control;
    static unsigned int end_control_gp;
    static unsigned int start_control_gp;

    MOTIONS::params::dt_gp = std::stod(optimizationParameters.at("dt_gp"));
    MOTIONS::params::dt_VSTRAP = std::stod(optimizationParameters.at("dt_VSTRAP"));
    static unsigned int ntimesteps_gp;
    MOTIONS::params::plasma_state_output_interval = std::stoi(optimizationParameters.at("plasma_state_output_interval"));
    static unsigned int ntimesteps_gp_VSTRAP; // ntimesteps_gp*plasma_state_output_interval -->

    MOTIONS::params::dv_gp = std::stod(optimizationParameters.at("dv_gp"));
    static unsigned int vcell_gp;
    MOTIONS::params::vmax_gp = std::stod(optimizationParameters.at("vmax_gp"));

    MOTIONS::params::fraction_fast_particles_gp = std::stod(optimizationParameters.at("fraction_fast_particles_gp"));

    //CONTROL
    MOTIONS::params::local_control_x_min_gp = std::stod(optimizationParameters.at("local_control_x_min_gp"));
    MOTIONS::params::local_control_x_max_gp = std::stod(optimizationParameters.at("local_control_x_max_gp"));
    MOTIONS::params::weight_control_gp = std::stod(optimizationParameters.at("weight_control_gp"));
    MOTIONS::params::velocity_part_objective = std::stod(optimizationParameters.at("velocity_part_objective"));
    MOTIONS::params::shifting_objective = std::stod(optimizationParameters.at("shifting_objective"));
    MOTIONS::params::visalization_scaling = std::stod(optimizationParameters.at("visalization_scaling"));
    MOTIONS::params::fabs_tol_gp = std::stod(optimizationParameters.at("fabs_tol_gp"));

    return 0;
}

int MOTIONS::LoadPaths(DataProvider &data_provider_opt)
{
    std::map<std::string, std::string> paths = data_provider_opt.getPaths();

    MOTIONS::paths::build_directory_vstrap = paths.at("build_directory_vstrap");
    MOTIONS::paths::build_directory_optim = paths.at("build_directory_optim");
    MOTIONS::paths::path_to_shared_files_absolute = paths.at("path_to_shared_files_absolute");

    MOTIONS::paths::pvpython_absolute_dir = paths.at("pvpython_absolute_dir");

    //meshes
    MOTIONS::paths::barycenters_mesh = paths.at("barycenters_mesh");
    MOTIONS::paths::domain_mesh = paths.at("domain_mesh");
    MOTIONS::paths::domain_mesh_file = paths.at("domain_mesh_file");
    MOTIONS::paths::surface_mesh = paths.at("surface_mesh");

    // shared
    MOTIONS::paths::input_forward = paths.at("input_forward");
    MOTIONS::paths::input_backward = paths.at("input_backward");
    MOTIONS::paths::results_directory = paths.at("results_directory");
    MOTIONS::paths::path_to_shared_files = paths.at("path_to_shared_files");
    MOTIONS::paths::directory_toolset = paths.at("directory_toolset");

    MOTIONS::paths::creation_adjoint_partcles = paths.at("creation_adjoint_partcles");
    MOTIONS::paths::bgf_control = paths.at("bgf_control");
    MOTIONS::paths::control_field_cells_name = paths.at("control_field_cells_name");

    MOTIONS::paths::start_with_existing_control = paths.at("start_with_existing_control");

    MOTIONS::paths::DSMC_interaction = paths.at("DSMC_interaction");
    MOTIONS::paths::MCC_interaction = paths.at("MCC_interaction");

    MOTIONS::paths::mesh_3d_path = paths.at("mesh_3d_path");
    MOTIONS::paths::mesh_2d_path = paths.at("mesh_2d_path");

    return 0;
}

int MOTIONS::LoadSubroutines(DataProvider &data_provider_opt)
{
    std::map<std::string, std::string> subroutines = data_provider_opt.getSubroutines();

    MOTIONS::subroutines::control_update = subroutines.at("control_update");
    MOTIONS::subroutines::direction_update = subroutines.at("direction_update");
    MOTIONS::subroutines::desired_trajectory = subroutines.at("desired_trajectory");
    MOTIONS::subroutines::objective_calculation = subroutines.at("objective_calculation");

    return 0;
}
