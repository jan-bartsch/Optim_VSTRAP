echo -e "\e Starting testcases"
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_temp_weight/Optim_input_512.xml
echo -e "\e[32mCopying results\e[39m"
rm -r results_B4001 
mkdir results_B4001
cp -r ./results/ ./results_B4001
cp ../../Optim_VSTRAP/data/box_shifting_temp_weight/interpolated_control_field.xml ./results_B4001
cp ../../Optim_VSTRAP/data/box_shifting_temp_weight/control_field_cells.xml ./results_B4001
cp ../../Optim_VSTRAP/data/box_shifting_temp_weight/creation_adjoint_particles.xml ./results_B4001
echo -e "\e[32mFinished e[39m"