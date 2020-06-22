#include "data_provider.h"

data_provider::data_provider() { }

data_provider::data_provider(const char *filename)
{
    this->setPaths(this->read_paths(filename));
    this->setOptimizationParameters(this->read_optimization_parameters(filename));
    this->setSubroutines(this->read_subroutines(filename));
    std::string barycenter_mesh_path = this->getPaths().find("BARYCENTERS_MESH")->second;
    this->setMesh_barycenters(this->read_mesh_barycenters(&barycenter_mesh_path[0]));
}

std::map<std::string, double> data_provider::read_optimization_parameters(const char* filename)
{
    std::map<std::string,double> globalParameters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        std::cout << filename << std::endl;
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = inputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_input
    TiXmlElement *parameterRoot = rootElement->FirstChildElement("globalParameters"); //globalParameters
    //std::cout << parameterRoot->Value() << std::endl;
    TiXmlElement *parameter = parameterRoot->FirstChildElement("parameter");
    int i = 0; // for sorting the entries
    while(parameter)
    {
        TiXmlAttribute *name = parameter->FirstAttribute();
        std::string name_string(name->Value());

        TiXmlAttribute *value = name->Next();
        double value_double = std::atof(value->Value());

        globalParameters.insert(std::pair<std::string,double>(name_string,value_double));
        parameter = parameter->NextSiblingElement();
        i++;
    }
    return globalParameters;
}

std::map<std::string, std::string> data_provider::read_subroutines(const char *filename)
{
    std::map<std::string,std::string> subroutines;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = inputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_input
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

std::map<int, std::vector<double> > data_provider::read_mesh_barycenters(const char *filename)
{
    std::map<int, std::vector<double>> mesh_barycenters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = inputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_input
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


std::map<std::string, std::string> data_provider::read_paths(const char *filename)
{
    std::map<std::string,std::string> inputParameters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File could not be opened. Check if directory"
                                 "and syntax are correct!");
    }

    TiXmlElement *rootElement = inputFile.RootElement();
    //std::cout << rootElement->Value() << std::endl; //optimizer_input
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

        inputParameters.insert(std::pair<std::string,std::string>(name_string,value_string));
        parameter = parameter->NextSiblingElement();
        i++;
    }
    return inputParameters;
}

std::map<std::string, std::string> data_provider::getPaths() const { return paths; }

void data_provider::setPaths(const std::map<std::string, std::string> &value) { paths = value; }

std::map<std::string, double> data_provider::getOptimizationParameters() const { return optimizationParameters; }

void data_provider::setOptimizationParameters(const std::map<std::string, double> &value) { optimizationParameters = value; }

std::map<std::string, std::string> data_provider::getSubroutines() const
{
    return subroutines;
}

void data_provider::setSubroutines(const std::map<std::string, std::string> &value)
{
    subroutines = value;
}

std::map<int, std::vector<double> > data_provider::getMesh_barycenters() const
{
    return mesh_barycenters;
}

void data_provider::setMesh_barycenters(const std::map<int, std::vector<double> > &value)
{
    mesh_barycenters = value;
}
