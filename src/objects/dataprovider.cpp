#include "dataprovider.h"

DataProvider::DataProvider() { }

DataProvider::DataProvider(const char *filename)
{
    this->setPaths(this->ReadPaths(filename));
    this->setOptimizationParameters(this->ReadOptimizationParameters(filename));
    this->setSubroutines(this->ReadSubroutines(filename));
    try {
        std::string barycenter_mesh_path = this->getPaths().find("BARYCENTERS_MESH")->second;
        this->setMesh_barycenters(this->ReadMeshBarycenters(&barycenter_mesh_path[0]));
    } catch (std::exception e) {
        std::cerr << "No Barycenters found!" << std::endl;
    }

}

std::map<std::string, std::string> DataProvider::ReadOptimizationParameters(const char* filename)
{
    std::map<std::string, std::string> globalParameters;

    TiXmlDocument InputFile(filename);
    if (!InputFile.LoadFile()) {
        std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = InputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_Input
    TiXmlElement *parameterRoot = rootElement->FirstChildElement("globalParameters"); //globalParameters
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *parameter = parameterRoot->FirstChildElement("parameter");
    int i = 0; // for sorting the entries
    while(parameter)
    {
        TiXmlAttribute *name = parameter->FirstAttribute();
        std::string name_string(name->Value());

        TiXmlAttribute *value = name->Next();
        std::string value_string(value->Value());

        globalParameters.insert(std::pair<std::string,std::string>(name_string,value_string));
        parameter = parameter->NextSiblingElement();
        i++;
    }
    return globalParameters;
}

std::map<std::string, std::string> DataProvider::ReadSubroutines(const char *filename)
{
    std::map<std::string,std::string> subroutines;

    TiXmlDocument InputFile(filename);
    if (!InputFile.LoadFile()) {
         std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = InputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_Input
    TiXmlElement *parameterRoot = rootElement->FirstChildElement("subroutines"); //globalParameters
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *parameter = parameterRoot->FirstChildElement("subroutine");
    int i = 0; // for sorting the entries
    while(parameter)
    {
        TiXmlAttribute *name = parameter->FirstAttribute();
        std::string name_string(name->Value());

        TiXmlAttribute *value = name->Next();
        std::string value_string(value->Value());

        subroutines.insert(std::pair<std::string,std::string>(name_string,value_string));
        parameter = parameter->NextSiblingElement();
        i++;
    }

    return subroutines;
}

std::map<int, std::vector<double> > DataProvider::ReadMeshBarycenters(const char *filename)
{
    std::map<int, std::vector<double>> mesh_barycenters;

    TiXmlDocument InputFile(filename);
    if (!InputFile.LoadFile()) {
         std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = InputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_Input
    TiXmlElement *barycenterRoot = rootElement->FirstChildElement("mesh_barycenters"); //globalParameters
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *barycenter = barycenterRoot->FirstChildElement("barycenter");
    int i = 0; // for sorting the entries

    size_t pos = 0;
    std::string token;
    std::vector<std::string> vec;
    std::string delimiter = ",";
    while(barycenter)
    {
        TiXmlAttribute *cell_id = barycenter->FirstAttribute();
        std::string name_string(cell_id->Value());
        int cell_id_int = std::stoi(name_string);

        std::string coordinate_string = barycenter->GetText();
        std::vector<std::string> vec;
        std::vector<double> barycenter_coordinates(3);

        while ((pos = coordinate_string.find(delimiter)) != std::string::npos) {
            token = coordinate_string.substr(0, pos);
            vec.push_back(token);
            coordinate_string.erase(0, pos + delimiter.length());
        }
        vec.push_back(coordinate_string); //push back last element

        barycenter_coordinates[0] = std::stod(vec[0]);
        barycenter_coordinates[1] = std::stod(vec[1]);
        barycenter_coordinates[2] = std::stod(vec[2]);


        mesh_barycenters.insert(std::pair<int, std::vector<double>>(cell_id_int,barycenter_coordinates));
        barycenter = barycenter->NextSiblingElement();
        i++;
    }

    return mesh_barycenters;
}

arma::mat DataProvider::ConvertBarycentersToArmaMat(std::map<int, std::vector<double> > barycenters)
{
    arma::mat bary(barycenters.size(),3);
    std::vector<double> current_barycenter;

    for(unsigned int cell_id = 0; cell_id < barycenters.size(); cell_id++) {
        current_barycenter = barycenters.at(static_cast<int>(cell_id+1));
        for(unsigned int j = 0; j<3; j++) {
            bary(cell_id,j) = current_barycenter[j];
        }
    }

    return bary;
}


std::map<std::string, std::string> DataProvider::ReadPaths(const char *filename)
{
    std::map<std::string,std::string> InputParameters;

    TiXmlDocument InputFile(filename);
    if (!InputFile.LoadFile()) {
         std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory "
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = InputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_Input
    TiXmlElement *parameterRoot = rootElement->FirstChildElement("paths"); //globalParameters
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *parameter = parameterRoot->FirstChildElement("path");
    int i = 0; // for sorting the entries
    while(parameter)
    {
        TiXmlAttribute *name = parameter->FirstAttribute();
        std::string name_string(name->Value());

        TiXmlAttribute *value = name->Next();
        std::string value_string(value->Value());

        InputParameters.insert(std::pair<std::string,std::string>(name_string,value_string));
        parameter = parameter->NextSiblingElement();
        i++;
    }
    return InputParameters;
}

std::map<std::string, std::string> DataProvider::getPaths() const { return paths_; }

void DataProvider::setPaths(const std::map<std::string, std::string> &value) { paths_ = value; }

std::map<std::string, std::string> DataProvider::getOptimizationParameters() const { return optimizationParameters; }

void DataProvider::setOptimizationParameters(const std::map<std::string, std::string> &value) { optimizationParameters = value; }

std::map<std::string, std::string> DataProvider::getSubroutines() const
{
    return subroutines;
}

void DataProvider::setSubroutines(const std::map<std::string, std::string> &value)
{
    subroutines = value;
}

std::map<int, std::vector<double> > DataProvider::getMeshBarycenters() const
{
    return mesh_barycenters;
}

void DataProvider::setMesh_barycenters(const std::map<int, std::vector<double> > &value)
{
    mesh_barycenters = value;
}
