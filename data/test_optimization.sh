#echo -e "\e[32mStarting A10xx\e[39m"
#./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_CSSC/Optim_input_B1001.xml
#echo -e "\e[32mCopying results B1001\e[39m"
#rm -r results_B1001 
#mkdir results_B1001
#cp -r ./results/ ./results_B1001
#cp ../../Optim_VSTRAP/data/box_shifting_CSSC/interpolated_control_field.xml ./results_B1001
#cp ../../Optim_VSTRAP/data/box_shifting_CSSC/control_field_cells.xml ./results_B1001
#cp ../../Optim_VSTRAP/data/box_shifting_CSSC/creation_adjoint_particles.xml ./results_B1001
#echo -e "\e[32mFinishedB1001\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_CSSC/Optim_input_B1002.xml
echo -e "\e[32mCopying results B1002\e[39m"
rm -r results_B1002 
mkdir results_B1002
cp -r ./results/ ./results_B1002
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/interpolated_control_field.xml ./results_B1002
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/control_field_cells.xml ./results_B1002
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/creation_adjoint_particles.xml ./results_B1002
echo -e "\e[32mFinished A1002\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_CSSC/Optim_input_B1003.xml
echo -e "\e[32mCopying results B1003\e[39m"
rm -r results_B1003 
mkdir results_B1003
cp -r ./results/ ./results_B1003
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/interpolated_control_field.xml ./results_B1003
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/control_field_cells.xml ./results_B1003
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/creation_adjoint_particles.xml ./results_B1003
echo -e "\e[32mFinished B1003\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_shifting_CSSC/Optim_input_B1004.xml
echo -e "\e[32mCopying results B1004\e[39m"
rm -r results_B1004 
mkdir results_B1004
cp -r ./results/ ./results_B1004
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/interpolated_control_field.xml ./results_B1004
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/control_field_cells.xml ./results_B1004
cp ../../Optim_VSTRAP/data/box_shifting_CSSC/creation_adjoint_particles.xml ./results_B1004
echo -e "\e[32mFinished B1004\e[39m"
