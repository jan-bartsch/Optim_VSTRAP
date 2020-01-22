#include "test_io.h"

test_io::test_io()
{

}

int test_io::test_inputController()
{

    input inputController = input();

    std::vector<particle> particlesInput = inputController.readParticleVector("/afs/ifm/home/bartsch/SPARC/Optim_VSTRAP/data/csv_dummy_particles.csv",",");

    std::vector<particle> desiredParticles;

    particle p1 = particle(-0.5,-0.5,-0.5,-1.0,-2.0,-3.0,0.624150913*pow(10,19));
    particle p2 = particle(-0.4,-0.4,-0.4,40.0,50.0,30.0,0.624150913*pow(10,19));
    particle p3 = particle(-0.3,-0.3,-0.3,1.0*pow(10,11),3.0*pow(10,12),4.0*pow(10,14),0.624150913*pow(10,19));
    particle p4 = particle(-0.4,-0.4,-0.4,0,0,0,0.624150913*pow(10,19));
    particle p5 = particle(-0.1,-0.1,-0.1,0,0,0,0.624150913*pow(10,19));

    return 0;
}

int test_io::test_output_ControlUpdate()
{
    output_control_update outputControlUpdate = output_control_update();

    //std::vector<double> test{0.5,1.5,2.5};

    std::map<int,std::vector<double>> testControl = {
        {1,{-1.0,-1-0,-1.0}},
        {2,{-2.0,2.0,-2.0}},
        {3,{-3.0,-2.0,-3.0}}
    };

    outputControlUpdate.writeControl_XML(testControl);

    return 0;
}

int test_io::test_readInParameters()
{
    optim_controller optimController = optim_controller();

    optimController.read_parameters();
}


