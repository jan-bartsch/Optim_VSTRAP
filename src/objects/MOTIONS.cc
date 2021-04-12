#include "MOTIONS.h"

namespace MOTIONS {



MOTIONS::InputData InitializeMotions::Load_MOTIONS(DataProvider &data_provider_opt) {

    MOTIONS::InputData input;
    try {
        input = LoadInputData(data_provider_opt);
    } catch (std::exception e) {
        std::cout << e.what() << std::endl;
    }

    return input;
}

const char* InitializeMotions::GenerateInputPath(int argc, const char **argv)
{
    std::string current_directory(get_current_dir_name());
    std::string Input_directory;
    std::string pathToOptimInput;
    const char *Input_xml_path;

    switch (argc) {
    case 1:
      Input_directory = current_directory + "/Optim_Input.xml";
      Input_xml_path = Input_directory.c_str();
      break;
    case 2:
      Input_xml_path = argv[1];
      break;
    default:
      throw std::runtime_error("To much input parameters to start MOTIONS");
    }
    return Input_xml_path;
}

InputData InitializeMotions::LoadInputData(DataProvider &data_provider_opt) {

    std::map<std::string, std::string> optimizationParameters =
            data_provider_opt.getOptimizationParameters();


    std::map<std::string, std::string> paths = data_provider_opt.getPaths();

    std::map<std::string, std::string> subroutines =
            data_provider_opt.getSubroutines();

    MOTIONS::InputData input_data;


    input_data.simulating_plasma = std::stoi(optimizationParameters.at("simulating_plasma"));


    input_data.fmm = std::stoi(optimizationParameters.at("fmm"));
    input_data.magnetic_force =
            std::stoi(optimizationParameters.at("magnetic_force"));
    input_data.electric_force =
            std::stoi(optimizationParameters.at("electric_force"));

    input_data.armijo_descent_fraction =
            std::stod(optimizationParameters.at("armijo_descent_fraction"));
    input_data.calculation_functional =
            std::stoi(optimizationParameters.at("calculation_functional"));
    input_data.C_theta = std::stod(optimizationParameters.at("C_theta"));
    input_data.C_phi = std::stod(optimizationParameters.at("C_phi"));
    input_data.sigma_x = std::stod(optimizationParameters.at("sigma_x"));
    input_data.sigma_v = std::stod(optimizationParameters.at("sigma_v"));

    // create adjoint particles and trajectory
    input_data.adjoint_mu_x =
            std::stod(optimizationParameters.at("adjoint_mu_x"));
    input_data.desired_position_x =
            std::stod(optimizationParameters.at("desired_position_x"));
    input_data.adjoint_mu_y =
            std::stod(optimizationParameters.at("adjoint_mu_y"));
    input_data.desired_position_y =
            std::stod(optimizationParameters.at("desired_position_y"));
    input_data.adjoint_mu_z =
            std::stod(optimizationParameters.at("adjoint_mu_z"));
    input_data.desired_position_z =
            std::stod(optimizationParameters.at("desired_position_z"));
    input_data.adjoint_s_x =
            std::stod(optimizationParameters.at("adjoint_s_x"));
    input_data.adjoint_s_y =
            std::stod(optimizationParameters.at("adjoint_s_y"));
    input_data.adjoint_s_z =
            std::stod(optimizationParameters.at("adjoint_s_z"));
    input_data.adjoint_vx =
            std::stod(optimizationParameters.at("adjoint_vx"));
    input_data.adjoint_vy =
            std::stod(optimizationParameters.at("adjoint_vy"));
    input_data.adjoint_vz =
            std::stod(optimizationParameters.at("adjoint_vz"));
    input_data.adjoint_number_density =
            std::stod(optimizationParameters.at("adjoint_number_density"));
    input_data.adjoint_weight =
            std::stod(optimizationParameters.at("adjoint_weight"));
    input_data.adjoint_charge_number =
            std::stod(optimizationParameters.at("adjoint_charge_number"));
    input_data.adjoint_mass =
            std::stod(optimizationParameters.at("adjoint_mass"));
    input_data.adjoint_species =
            std::stod(optimizationParameters.at("adjoint_species"));
    input_data.expected_speed =
            std::stod(optimizationParameters.at("expected_speed"));
    input_data.most_probable_speed =
            std::stod(optimizationParameters.at("most_probable_speed"));

    // MCC parameters
    input_data.MCC_temperature =
            std::stod(optimizationParameters.at("MCC_temperature"));
    input_data.MCC_species =
            std::stod(optimizationParameters.at("MCC_species"));
    input_data.MCC_mass = std::stod(optimizationParameters.at("MCC_mass"));
    input_data.MCC_fixed_number_density =
            std::stod(optimizationParameters.at("MCC_fixed_number_density"));

    // forward input
    input_data.creation_forward_particles_method =
            optimizationParameters.at("most_probable_speed");
    input_data.initial_condition_file =
            optimizationParameters.at("most_probable_speed");
    input_data.number_density_forward =
            std::stod(optimizationParameters.at("number_density_forward"));
    input_data.weight_forward =
            std::stod(optimizationParameters.at("weight_forward"));
    input_data.charge_number_forward =
            std::stod(optimizationParameters.at("charge_number_forward"));
    input_data.mass_forward =
            std::stod(optimizationParameters.at("mass_forward"));
    input_data.species_forward =
            std::stod(optimizationParameters.at("species_forward"));
    input_data.temperature_x_val =
            std::stod(optimizationParameters.at("temperature_x_val"));
    input_data.temperature_y_val =
            std::stod(optimizationParameters.at("temperature_y_val"));
    input_data.temperature_z_val =
            std::stod(optimizationParameters.at("temperature_z_val"));
    input_data.v_drift_x_val =
            std::stod(optimizationParameters.at("v_drift_x_val"));
    input_data.v_drift_y_val =
            std::stod(optimizationParameters.at("v_drift_y_val"));
    input_data.v_drift_z_val =
            std::stod(optimizationParameters.at("v_drift_z_val"));

    input_data.mesh_2d_writer_included =
            std::stoi(optimizationParameters.at("mesh_2d_writer_included"));
    input_data.mesh_2d_name = optimizationParameters.at("mesh_2d_name");
    input_data.mesh_2d_outputinterval =
            std::stoi(optimizationParameters.at("mesh_2d_outputinterval"));

    input_data.mesh_3d_writer_included =
            std::stoi(optimizationParameters.at("mesh_3d_writer_included"));
    input_data.mesh_3d_name = optimizationParameters.at("mesh_3d_name");
    input_data.mesh_3d_outputinterval =
            std::stoi(optimizationParameters.at("mesh_3d_outputinterval"));

    input_data.inflow_included =
            std::stoi(optimizationParameters.at("inflow_included"));
    input_data.number_density_forward_inflow =
            std::stod(optimizationParameters.at("number_density_forward_inflow"));
    input_data.weight_forward_inflow =
            std::stod(optimizationParameters.at("weight_forward_inflow"));
    input_data.charge_number_forward_inflow =
            std::stoi(optimizationParameters.at("charge_number_forward_inflow"));
    input_data.mass_forward_inflow =
            std::stod(optimizationParameters.at("mass_forward_inflow"));
    input_data.species_forward_inflow =
            optimizationParameters.at("species_forward_inflow");
    input_data.temperature_x_val_inflow =
            std::stod(optimizationParameters.at("temperature_x_val_inflow"));
    input_data.temperature_y_val_inflow =
            std::stod(optimizationParameters.at("temperature_y_val_inflow"));
    input_data.temperature_z_val_inflow =
            std::stod(optimizationParameters.at("temperature_z_val_inflow"));
    input_data.v_drift_x_val_inflow =
            std::stod(optimizationParameters.at("v_drift_x_val_inflow"));
    input_data.v_drift_y_val_inflow =
            std::stod(optimizationParameters.at("v_drift_y_val_inflow"));
    input_data.v_drift_z_val_inflow =
            std::stod(optimizationParameters.at("v_drift_z_val_inflow"));

    // optimization techniques
    input_data.fixed_gradient_descent_stepsize =
            std::stod(optimizationParameters.at("fixed_gradient_descent_stepsize"));
    input_data.fraction_of_optimal_control =
            std::stod(optimizationParameters.at("fraction_of_optimal_control"));
    static unsigned int optimization_iteration_max;
    input_data.linesearchIteration_max_gp =
            std::stoi(optimizationParameters.at("linesearchIteration_max_gp"));
    input_data.numberParticles_gp =
            std::stoi(optimizationParameters.at("numberParticles_gp"));
    input_data.start_zero_control =
            std::stoi(optimizationParameters.at("start_zero_control"));
    input_data.tolerance_gp =
            std::stod(optimizationParameters.at("tolerance_gp"));
    input_data.tolerance_relaxation_gp =
            std::stod(optimizationParameters.at("tolerance_relaxation_gp"));
    input_data.armijio_base_exp =
            std::stod(optimizationParameters.at("armijio_base_exp"));
    input_data.armijo_iterative_exp =
            std::stod(optimizationParameters.at("armijo_iterative_exp"));

    // DOMAIN PARAMETERS
    input_data.position_max_gp =
            std::stod(optimizationParameters.at("position_max_gp"));

    static unsigned int number_cells_position;
    input_data.small_discr_volume =
            std::stod(optimizationParameters.at("small_discr_volume"));
    input_data.small_discr_sidelength =
            std::stod(optimizationParameters.at("small_discr_sidelength"));
    static unsigned int dimension_control;
    static unsigned int end_control_gp;
    static unsigned int start_control_gp;

    input_data.dt_gp = std::stod(optimizationParameters.at("dt_gp"));
    input_data.dt_VSTRAP =
            std::stod(optimizationParameters.at("dt_VSTRAP"));
    static unsigned int ntimesteps_gp;
    input_data.plasma_state_output_interval =
            std::stoi(optimizationParameters.at("plasma_state_output_interval"));
    static unsigned int
            ntimesteps_gp_VSTRAP; // ntimesteps_gp*plasma_state_output_interval -->

    input_data.dv_gp = std::stod(optimizationParameters.at("dv_gp"));
    static unsigned int vcell_gp;
    input_data.vmax_gp = std::stod(optimizationParameters.at("vmax_gp"));

    input_data.fraction_fast_particles_gp =
            std::stod(optimizationParameters.at("fraction_fast_particles_gp"));

    // CONTROL
    input_data.local_control_x_min_gp =
            std::stod(optimizationParameters.at("local_control_x_min_gp"));
    input_data.local_control_x_max_gp =
            std::stod(optimizationParameters.at("local_control_x_max_gp"));
    input_data.weight_control_gp =
            std::stod(optimizationParameters.at("weight_control_gp"));
    input_data.velocity_part_objective =
            std::stod(optimizationParameters.at("velocity_part_objective"));
    input_data.shifting_objective =
            std::stod(optimizationParameters.at("shifting_objective"));
    input_data.visalization_scaling =
            std::stod(optimizationParameters.at("visalization_scaling"));
    input_data.fabs_tol_gp =
            std::stod(optimizationParameters.at("fabs_tol_gp"));




    input_data.build_directory_vstrap = paths.at("build_directory_vstrap");
    input_data.build_directory_optim = paths.at("build_directory_optim");
    input_data.path_to_shared_files_absolute =
            paths.at("path_to_shared_files_absolute");

    input_data.pvpython_absolute_dir = paths.at("pvpython_absolute_dir");

    // meshes
    input_data.barycenters_mesh = paths.at("barycenters_mesh");
    input_data.domain_mesh = paths.at("domain_mesh");
    input_data.domain_mesh_file = paths.at("domain_mesh_file");
    input_data.surface_mesh = paths.at("surface_mesh");

    // shared
    input_data.input_forward = paths.at("input_forward");
    input_data.input_backward = paths.at("input_backward");
    input_data.results_directory = paths.at("results_directory");
    input_data.path_to_shared_files = paths.at("path_to_shared_files");
    input_data.directory_toolset = paths.at("directory_toolset");

    input_data.creation_adjoint_partcles =
            paths.at("creation_adjoint_partcles");
    input_data.bgf_control = paths.at("bgf_control");
    input_data.control_field_cells_name =
            paths.at("control_field_cells_name");

    input_data.start_with_existing_control =
            paths.at("start_with_existing_control");

    input_data.DSMC_interaction = paths.at("DSMC_interaction");
    input_data.MCC_interaction = paths.at("MCC_interaction");

    input_data.mesh_3d_path = paths.at("mesh_3d_path");
    input_data.mesh_2d_path = paths.at("mesh_2d_path");

    input_data.control_update = subroutines.at("control_update");
    input_data.direction_update = subroutines.at("direction_update");
    input_data.desired_trajectory =
            subroutines.at("desired_trajectory");
    input_data.objective_calculation =
            subroutines.at("objective_calculation");

    return input_data;
}

}
