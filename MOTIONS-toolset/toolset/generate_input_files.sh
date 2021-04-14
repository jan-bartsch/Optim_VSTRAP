echo "Argument for generating input files:"
echo $1

python3 generate_forward_input.py $1
python3 generate_backward_input.py $1
python3 generate_adjoint_particle_creation.py $1 $2

