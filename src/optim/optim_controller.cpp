#include "optim_controller.h"

optim_controller::optim_controller()
{

}

std::map<std::string,double> optim_controller::read_parameters(const char* filename)
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
