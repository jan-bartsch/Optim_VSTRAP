./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A301.xml
echo "Copying results A301"
rm -r results_A301 && mkdir results_A301
cp -r ./results/ ./results_A301
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A301
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A301
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A301
echo "Finished A301"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A302.xml
echo "Copying results A302"
rm -r results_A302 && mkdir results_A302
cp -r ./results/ ./results_A302
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A302
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A302
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A302
echo "Finished A302"

#screen ./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A3.xml
./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A303.xml
echo "Copying results A303"
rm -r results_A303 && mkdir results_A303
cp -r ./results/ ./results_A303
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A303
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A303
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A303
echo "Finished A303"
