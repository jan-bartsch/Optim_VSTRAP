#include "initializer.h"

initializer::initializer()
{

}

std::map<std::string, double> initializer::read_optimization_parameters(const char* filename)
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

std::map<std::string, std::string> initializer::read_input_parameters(const char *filename)
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
