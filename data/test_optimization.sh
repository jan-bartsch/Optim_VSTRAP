#screen ./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A1.xml
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A1.xml
echo "Copying results A1"
rm -r results_A1 && mkdir results_A1
cp -r ./results/ ./results_A1
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A1
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A1
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A1
echo "Finished A1"

#screen ./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A2.xml
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A2.xml
echo "Copying results A2"
rm -r results_A2 && mkdir results_A2
cp -r ./results/ ./results_A2
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A2
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A2
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A2
echo "Finished A2"

#screen ./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A3.xml
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A3.xml
echo "Copying results A3"
rm -r results_A3 && mkdir results_A3
cp -r ./results/ ./results_A3
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A3
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A3
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A3
echo "Finished A3"
