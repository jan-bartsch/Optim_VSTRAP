#include "optim_controller.h"

optim_controller::optim_controller()
{

}

int optim_controller::read_parameters()
{

    const char* filename = "/afs/ifm/home/bartsch/SPARC/Optim_VSTRAP/data/Optim_input.xml";

    std::map<std::string,double> globalParameters;

    TiXmlDocument inputFile(filename);
    if (!inputFile.LoadFile()) {
        throw std::runtime_error("File not found");
    }

    TiXmlHandle hDoc(&inputFile);
    TiXmlElement *parameter, *pRoot;

    try{


        pRoot = inputFile.FirstChildElement();
        if(pRoot)
        {
            parameter = pRoot->FirstChildElement();
            int i = 0; // for sorting the entries
            while(parameter)
            {
                double pValue = std::atof(parameter->Value());
                std::string pText(parameter->GetText());
                globalParameters.insert(std::pair<std::string,double>(pText,pValue));
                parameter = parameter->NextSiblingElement();
                i++;
            }
        }
    //for( TiXmlElement* parameter; parameter1; parameter=parameter1->NextSiblingElement())
    //{

    //}

    } catch(std::exception e) {
        return 1;
    }


    return 0;
}
