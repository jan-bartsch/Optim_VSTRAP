echo -e "\e[32mStarting B2xx\e[39m"
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_temp/Optim_input_B3001.xml
echo -e "\e[32mCopying results\e[39m"
rm -r results_B3001 
mkdir results_B3001
cp -r ./results/ ./results_B3001
cp ../../Optim_VSTRAP/data/box_shifting_temp/interpolated_control_field.xml ./results_B3001
cp ../../Optim_VSTRAP/data/box_shifting_temp/control_field_cells.xml ./results_B3001
cp ../../Optim_VSTRAP/data/box_shifting_temp/creation_adjoint_particles.xml ./results_B3001
echo -e "\e[32mFinishedB1001\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_temp/Optim_input_B3002.xml
echo -e "\e[32mCopying results \e[39m"
rm -r results_B3002 
mkdir results_B3002
cp -r ./results/ ./results_B3002
cp ../../Optim_VSTRAP/data/box_shifting_temp/interpolated_control_field.xml ./results_B3002
cp ../../Optim_VSTRAP/data/box_shifting_temp/control_field_cells.xml ./results_B3002
cp ../../Optim_VSTRAP/data/box_shifting_temp/creation_adjoint_particles.xml ./results_B3002
echo -e "\e[32mFinished A1002\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_temp/Optim_input_B3003.xml
echo -e "\e[32mCopying results \e[39m"
rm -r results_B3003 
mkdir results_B3003
cp -r ./results/ ./results_B3003
cp ../../Optim_VSTRAP/data/box_shifting_temp/interpolated_control_field.xml ./results_B3003
cp ../../Optim_VSTRAP/data/box_shifting_temp/control_field_cells.xml ./results_B3003
cp ../../Optim_VSTRAP/data/box_shifting_temp/creation_adjoint_particles.xml ./results_B3003
echo -e "\e[32mFinished \e[39m"
