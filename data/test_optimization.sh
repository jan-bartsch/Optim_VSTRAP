#echo -e "\e[32mStarting A10xx\e[39m"
#./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A1001.xml
#echo -e "\e[32mCopying results A1001\e[39m"
#rm -r results_A1001 && mkdir results_A1001
#cp -r ./results/ ./results_A1001
#cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A1001
#cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_AA1001
#cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A1001
#echo -e "\e[32mFinishedA1001\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A1002.xml
echo -e "\e[32mCopying results A1002\e[39m"
rm -r results_A1002 && mkdir results_A1002
cp -r ./results/ ./results_A1002
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A1002
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A1002
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A1002
echo -e "\e[32mFinished A1002\e[39m"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A1003.xml
echo -e "\e[32mCopying results A1003\e[39m"
rm -r results_A1003 && mkdir results_A1003
cp -r ./results/ ./results_A1003
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A1003
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A1003
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A1003
echo -e "\e[32mFinished A1003\e[39m"
