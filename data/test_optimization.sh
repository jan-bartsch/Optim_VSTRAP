./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A201.xml
echo "Copying results A201"
rm -r results_A201 && mkdir results_A201
cp -r ./results/ ./results_A201
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A201
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A201
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A201
echo "Finished A201"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A202.xml
echo "Copying results A202"
rm -r results_A202 && mkdir results_A202
cp -r ./results/ ./results_A202
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A202
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A202
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A202
echo "Finished A202"

#screen ./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A3.xml
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A203.xml
echo "Copying results A203"
rm -r results_A203 && mkdir results_A203
cp -r ./results/ ./results_A203
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A203
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A203
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A203
echo "Finished A203"
