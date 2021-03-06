#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <armadillo>
#include <exception>
#include <map>
#include <string>
#include <vector>

#include <iostream>

#include "../io/tinyXML/tinyxml.h"

/**
 * @brief The data_provider class provides the data given in the
 * Input file for the optimizer
 */
class DataProvider {
public:
  DataProvider();
  DataProvider(const char *filename);
  ~DataProvider() = default;

  std::map<std::string, std::string> ReadPaths(const char *filename);
  std::map<std::string, std::string>
  ReadOptimizationParameters(const char *filename);
  std::map<std::string, std::string> ReadSubroutines(const char *filename);
  std::map<int, std::vector<double>> ReadMeshBarycenters(const char *filename);

  std::map<std::string, std::string> getPaths() const;
  void setPaths(const std::map<std::string, std::string> &value);

  std::map<std::string, std::string> getOptimizationParameters() const;
  void
  setOptimizationParameters(const std::map<std::string, std::string> &value);

  std::map<std::string, std::string> getSubroutines() const;
  void setSubroutines(const std::map<std::string, std::string> &value);

  std::map<int, std::vector<double>> getMeshBarycenters() const;
  void setMesh_barycenters(const std::map<int, std::vector<double>> &value);

  static arma::mat
  ConvertBarycentersToArmaMat(std::map<int, std::vector<double>> barycenters);

private:
  std::map<std::string, std::string> paths_;
  std::map<std::string, std::string> optimizationParameters_;
  std::map<std::string, std::string> subroutines_;
  std::map<int, std::vector<double>> mesh_barycenters_;
};

#endif // DATA_PROVIDER_H
