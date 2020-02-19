#include "output_control_update.h"

output_control_update::output_control_update(const char *filename) {
    this->setData_provider_optim(data_provider(filename));
}

int output_control_update::writeControl_XML(arma::mat control)
{

    int number_of_nodes = static_cast<int>(control.n_rows);
    std::cout << "Number of nodes: " << number_of_nodes << std::endl;
    std::string number_of_nodes_string = std::to_string(number_of_nodes);

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    //<field name="oscillator_electric_field" type="electric_field" dimension="3" number_of_nodes="1331">

    TiXmlElement* headelement = new TiXmlElement( "field" );
    headelement->SetAttribute("name","test_random_control_field");
    headelement->SetAttribute("type","control_field");
    headelement->SetAttribute("dimensions","3");
    headelement->SetAttribute("number_of_elements",number_of_nodes_string.data());
    doc.LinkEndChild( headelement );

    for(int node_number = 1; node_number <= number_of_nodes; node_number++ ) {

        std::string fieldVector = "";

        fieldVector.append(std::to_string(control(node_number-1,0)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(node_number-1,1)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(node_number-1,2)));

        TiXmlText* text = new TiXmlText( fieldVector.data() );


        TiXmlElement* node = new TiXmlElement( "value" );
        node->SetAttribute("cell_id",std::to_string(node_number).data());
        node->LinkEndChild( text );

        headelement->LinkEndChild( node );
    }

    std::string PATH_TO_SHARED_FILES = this->getData_provider_optim().getPaths().find("PATH_TO_SHARED_FILES")->second;
    std::string saveFile = PATH_TO_SHARED_FILES + "Control_new_TEST42.xml";

    doc.SaveFile(&saveFile[0]);

    return 0;
}
