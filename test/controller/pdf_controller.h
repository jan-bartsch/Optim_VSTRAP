#include <gtest/gtest.h>

#include "../../src/controller/pdf_controller.h"
#include "../../src/io/input.h"
#include "../../src/logger/logger.h"

TEST(pdf,assembling) {
    std::string input_directory = "./data/Optim_input_gTest.xml";
    const char *  input_xml_path = input_directory.c_str();
    data_provider data_provider_opt = data_provider(input_xml_path);
    std::map<std::string, double> optimizationParameters = data_provider_opt.getOptimizationParameters();

    bool all_checked(true);


    pdf_controller pdf_control = pdf_controller();
    pdf_control.setData_provider_optim(data_provider_opt);
    input input_control = input();
    input_control.setData_provider_optim(data_provider_opt);

    unsigned int ntimesteps_gp = static_cast<unsigned int>(optimizationParameters.find("ntimesteps_gp")->second);

    std::vector<std::vector<particle>> forwardParticles(ntimesteps_gp);
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_1;
    std::vector<std::unordered_map<coordinate_phase_space_time,double>> forwardPDF_2;

    std::vector<std::unordered_map<coordinate_phase_space_time,double>> pdf_time(ntimesteps_gp);
    int assembling_flag;

#pragma omp parallel for
    for(unsigned int o = 1; o<=ntimesteps_gp; o++) {
        forwardParticles[o-1] = input::readParticleVector("./data/vstrap_test_output/plasma_state_batch_1_forward_particles_CPU_"+std::to_string(o)+".csv",",");
    }

    assembling_flag = pdf_control.assemblingMultiDim_parallel(forwardParticles,0,pdf_time);
    forwardPDF_1 = pdf_time;
    assembling_flag = pdf_control.assemblingMultiDim(forwardParticles,0,pdf_time);
    forwardPDF_2 = pdf_time;

    for (unsigned int i = 0; i<forwardPDF_1.size(); i++) {
        if (forwardPDF_1[i] != forwardPDF_2[i]) {
            all_checked = false;
        }
    }

    ASSERT_TRUE(all_checked);
}
