#include "outputcontrolupdate.h"

OutputControlUpdate::OutputControlUpdate() { }

OutputControlUpdate::OutputControlUpdate(const char *filename) {
    this->set_DataProviderOptim(DataProvider(filename));
}

int OutputControlUpdate::WritecontrolXml(arma::mat control)
{
    int number_of_nodes = static_cast<int>(control.n_rows);
    std::cout << "Number of nodes: " << number_of_nodes << std::endl;
    std::string number_of_nodes_string = std::to_string(number_of_nodes);

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    //<field name="oscillator_electric_field" type="electric_field" dimension="3" number_of_nodes="1331">

    TiXmlElement* headelement = new TiXmlElement( "field" );
    headelement->SetAttribute("name","control_found_by_optimizer");
    headelement->SetAttribute("type","control_field");
    headelement->SetAttribute("dimensions","3");
    headelement->SetAttribute("number_of_elements",number_of_nodes_string.data());
    doc.LinkEndChild( headelement );

    for(int cell_id = 1; cell_id <= number_of_nodes; cell_id++ ) {

        std::string fieldVector = "";

        fieldVector.append(std::to_string(control(cell_id-1,0)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(cell_id-1,1)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(cell_id-1,2)));

        TiXmlText* text = new TiXmlText( fieldVector.data() );


        TiXmlElement* node = new TiXmlElement( "value" );
        node->SetAttribute("cell_id",std::to_string(cell_id).data());
        node->LinkEndChild( text );

        headelement->LinkEndChild( node );
    }

    std::string path_to_shared_files = this->get_DataProviderOptim().getPaths().find("PATH_TO_SHARED_FILES")->second;
    std::string CONTROL_FIELD_CELLS_NAME = this->get_DataProviderOptim().getPaths().find("CONTROL_FIELD_CELLS_NAME")->second;

    std::string saveFile = path_to_shared_files + CONTROL_FIELD_CELLS_NAME;

    doc.SaveFile(&saveFile[0]);

    return 0;
}

int OutputControlUpdate::InterpolateControl()
{
    std::map<std::string, std::string> paths = provider.getPaths();
    std::map<std::string, double> parameters = provider.getOptimizationParameters();
    std::string PATH_TO_SHARED_FILES = paths.find("PATH_TO_SHARED_FILES")->second;
    std::string DIRECTORY_TOOLSET = paths.find("DIRECTORY_TOOLSET")->second;
    std::string DOMAIN_MESH = paths.find("DOMAIN_MESH")->second;
    std::string CHECK_Input_PYHTON = "python3 " + DIRECTORY_TOOLSET + "check_Input.py " + PATH_TO_SHARED_FILES;

    int magnetic_force = static_cast<int>(parameters.find("magnetic_force")->second);
    int electric_force = static_cast<int>(parameters.find("electric_force")->second);

    std::string CONTROL_TYPE = "";

    if (magnetic_force == 1 && electric_force == 0) {
        CONTROL_TYPE = "magnetic_field";
    } else if (magnetic_force == 0 && electric_force == 1) {
        CONTROL_TYPE = "electric_field";
    } else {
        std::cerr << "Force field not valid specified" << std::endl;
        throw std::runtime_error("Force field not valid specified");
    }


    std::string BGF_CONTROL = paths.find("BGF_CONTROL")->second;
    std::string CONTROL_FIELD_CELLS_NAME = paths.find("CONTROL_FIELD_CELLS_NAME")->second;
    std::string interpolating_control_python = "python3 " + DIRECTORY_TOOLSET + "GenerateControlField.py" + " " +  PATH_TO_SHARED_FILES + DOMAIN_MESH +
            " " + PATH_TO_SHARED_FILES + CONTROL_FIELD_CELLS_NAME + " " + PATH_TO_SHARED_FILES + BGF_CONTROL + " " + CONTROL_TYPE;

    int interpolating_flag = system(&interpolating_control_python[0]);
    if(interpolating_flag == 512) {
        throw std::runtime_error("File not found in control interpolating");
    } else if (interpolating_flag == 256) {
        throw std::runtime_error("Interpolating returned error value");
    }

    return interpolating_flag;

}
