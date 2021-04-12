#include "adjoint_particle_creator.h"

namespace vstrap {

AdjointParticleCreator::AdjointParticleCreator()
    : Solver("adjoint_particle_creator") {}

void AdjointParticleCreator::Clear() {}

void AdjointParticleCreator::ExecuteCPU(const std::string &) {
  for (uint i = 0; i < this->particle_groups_.size(); i++) {
    if ((this->abort_criterium_->GetCurrentItteration() - 1) ==
        this->particle_groups_.at(i).iteration) {
      const uint number_particles(
          this->particle_groups_.at(i).number_density *
          this->particle_groups_.at(i).domain.GetVolume() /
          this->particle_groups_.at(i).weight);

      const std::vector<Vector3d> positons = GetRandomNumbers(
          number_particles, this->particle_groups_.at(i).mu_position,
          this->particle_groups_.at(i).sigma_position, 0);
      const std::vector<Vector3d> velocities = GetRandomNumbers(
          number_particles, this->particle_groups_.at(i).mu_velocity,
          this->particle_groups_.at(i).sigma_velocity, 1);

      auto group = this->data_provider_->GetParticleGroup(
          this->particle_groups_.at(i).name);

      for (uint k = 0; k < number_particles; k++) {
        group->AddElement();
        group->SetLastStrData(Data::SPECIES,
                              this->particle_groups_.at(i).species);
        group->SetLastDoubleData(Data::WEIGHT,
                                 this->particle_groups_.at(i).weight);
        group->SetLastDoubleData(Data::MASS, this->particle_groups_.at(i).mass);
        group->SetLastDoubleData(
            Data::CHARGE, constants::kElementalCharge *
                              this->particle_groups_.at(i).charge_number);
        group->SetLastVector3dData(Data::VELOCITY, velocities.at(k));
        group->SetLastVector3dData(Data::POSITION, positons.at(k));
      }
    }
  }
}

void AdjointParticleCreator::ExecuteGPU(const std::string &) {}

void AdjointParticleCreator::InitializeCPU(
    const std::shared_ptr<DataNode> &data) {
  const DataNode node(
      data->GetNode("executables").GetNode("executable", "name", GetName()));
  const std::string group_name(
      node.GetNode("particle_group").GetAttribute("name"));
  const Cuboid domain(SetUpSimulationDomain(node));
  const std::vector<DataNode> nodes(
      node.GetNode("parameraters").GetNodes("set"));

  this->particle_groups_.resize(nodes.size());

  for (uint i = 0; i < nodes.size(); i++) {
    this->particle_groups_.at(i) =
        GetParticleGroup(group_name, domain, nodes.at(i));
  }
}

void AdjointParticleCreator::InitializeGPU(const std::shared_ptr<DataNode> &) {}

AdjointParticleCreator::Group AdjointParticleCreator::GetParticleGroup(
    const std::string &name, const Cuboid &domain, const DataNode &node) const {
  Group group;

  group.name = name;
  group.domain = domain;

  group.number_density =
      std::stod(node.GetNode("particle_values").GetAttribute("number_density"));
  group.iteration = std::stoi(node.GetAttribute("iteration"));
  group.weight =
      std::stod(node.GetNode("particle_values").GetAttribute("weight"));
  group.charge_number =
      std::stod(node.GetNode("particle_values").GetAttribute("charge_number"));
  group.mass = std::stod(node.GetNode("particle_values").GetAttribute("mass"));
  group.species = node.GetNode("particle_values").GetAttribute("species");

  group.mu_position = GetVector3dValue(node.GetNode("position"), "mu");
  group.sigma_position = GetVector3dValue(node.GetNode("position"), "sigma");
  group.mu_velocity = GetVector3dValue(node.GetNode("velocity"), "mu");
  group.sigma_velocity = GetVector3dValue(node.GetNode("velocity"), "sigma");

  return group;
}

Cuboid
AdjointParticleCreator::SetUpSimulationDomain(const DataNode &node) const {
  Cuboid domain;
  DataNode domain_node = node.GetNode("geometry");

  domain.min[0] = std::stod(domain_node.GetAttribute("x_min"));
  domain.max[0] = std::stod(domain_node.GetAttribute("x_max"));
  domain.min[1] = std::stod(domain_node.GetAttribute("y_min"));
  domain.max[1] = std::stod(domain_node.GetAttribute("y_max"));
  domain.min[2] = std::stod(domain_node.GetAttribute("z_min"));
  domain.max[2] = std::stod(domain_node.GetAttribute("z_max"));

  if (domain.min[0] > domain.max[0] || domain.min[1] > domain.max[1] ||
      domain.min[2] > domain.max[2]) {
    throw Exception("ParticleInitializer::" + std::string(__FUNCTION__),
                    "Min and max coordinates inverted for one of the "
                    "domain's components");
  }

  return domain;
}

Vector3d
AdjointParticleCreator::GetVector3dValue(const DataNode &node,
                                         const std::string &name) const {
  Vector3d value;
  DataNode value_node = node.GetNode(name);

  value[0] = std::stod(value_node.GetAttribute("x_val"));
  value[1] = std::stod(value_node.GetAttribute("y_val"));
  value[2] = std::stod(value_node.GetAttribute("z_val"));

  return value;
}

std::vector<Vector3d> AdjointParticleCreator::GetRandomNumbers(
    const uint &number_particles, const Vector3d &mu, const Vector3d &sigma,
    int equationType) const {
  std::vector<Vector3d> random_numbers(number_particles);
  std::vector<double> generated_random_numbers;

  Cuboid domain = this->particle_groups_.at(0).domain;

  this->random_number_provider_->AllocateElements(number_particles);

  for (uint i = 0; i < 3; i++) {
    this->random_number_provider_->GenerateNormalRandomNumbers(mu[i], sigma[i]);
    generated_random_numbers =
        this->random_number_provider_->GetRandomNumbers();

    for (uint k = 0; k < number_particles; k++) {
      random_numbers.at(k)[i] = generated_random_numbers.at(k);
      //            while ((domain.max[i]<random_numbers.at(k)[i] ||
      //            domain.min[i]>random_numbers.at(k)[i])
      //                   && equationType == 0) {
      //                this->random_number_provider_->GenerateNormalRandomNumbers(mu[i],
      //                sigma[i]); generated_random_numbers =
      //                this->random_number_provider_->GetRandomNumbers();
      //                random_numbers.at(k)[i] =
      //                generated_random_numbers.at(k);
      //            }
    }
  }

  return random_numbers;
}

} // namespace vstrap
