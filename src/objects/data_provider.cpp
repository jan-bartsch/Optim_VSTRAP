#include "data_provider.h"

data_provider::data_provider() { }

data_provider::data_provider(const char *filename)
{
   this->setPaths(this->read_paths(filename));
   this->setOptimizationParameters(this->read_optimization_parameters(filename));
}

std::map<std::string, double> data_provider::read_optimization_parameters(const char* filename)
{
    std::map<std::string,double> globalParameters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File not found");
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


std::map<std::string, std::string> data_provider::read_paths(const char *filename)
{
    std::map<std::string,std::string> inputParameters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File not found");
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
