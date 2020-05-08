#ifndef DATA_PROVIDER_H
#define DATA_PROVIDER_H

#include <map>
#include <vector>
#include <string>
#include <exception>

#include <iostream>

#include "../io/tinyXML/tinyxml.h"


class data_provider
{
public:
    data_provider();
    data_provider(const char *filename);
    ~data_provider() = default;

    std::map<std::string, std::string> read_paths(const char *filename);
    std::map<std::string, double> read_optimization_parameters(const char* filename);
    std::map<std::string,std::string> read_subroutines(const char* filename);
    std::map<int,std::vector<double>> read_mesh_barycenters(const char* filename);
    /*
     * Getters and Setters
     */

    std::map<std::string, std::string> getPaths() const;
    void setPaths(const std::map<std::string, std::string> &value);

    std::map<std::string, double> getOptimizationParameters() const;
    void setOptimizationParameters(const std::map<std::string, double> &value);

    std::map<std::string, std::string> getSubroutines() const;
    void setSubroutines(const std::map<std::string, std::string> &value);

    std::map<int, std::vector<double> > getMesh_barycenters() const;
    void setMesh_barycenters(const std::map<int, std::vector<double> > &value);

private:
    std::map<std::string,std::string> paths;
    std::map<std::string,double> optimizationParameters;
    std::map<std::string,std::string> subroutines;
    std::map<int,std::vector<double>> mesh_barycenters;
};

#endif // DATA_PROVIDER_H

