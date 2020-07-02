./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A401.xml
echo "Copying results A401"
rm -r results_A401 && mkdir results_A401
cp -r ./results/ ./results_A401
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A401
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A401
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A401
echo "Finished A401"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A402.xml
echo "Copying results A402"
rm -r results_A402 && mkdir results_A402
cp -r ./results/ ./results_A402
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A402
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A402
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A402
echo "Finished A402"

./Optim_VSTRAP_CMAKE ../../Optim_VSTRAP/data/box_center_CSSC/Optim_input_A403.xml
echo "Copying results A403"
rm -r results_A403 && mkdir results_A403
cp -r ./results/ ./results_A403
cp ../../Optim_VSTRAP/data/box_center_CSSC/interpolated_control_field.xml ./results_A403
cp ../../Optim_VSTRAP/data/box_center_CSSC/control_field_cells.xml ./results_A403
cp ../../Optim_VSTRAP/data/box_center_CSSC/creation_adjoint_particles.xml ./results_A403
echo "Finished A403"
