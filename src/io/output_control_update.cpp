#include "output_control_update.h"

output_control_update::output_control_update() { }

int output_control_update::writeControl_XML(std::map<int, std::vector<double>> control)
{

    int number_of_nodes = static_cast<int>(control.size());
    std::string number_of_nodes_string = std::to_string(number_of_nodes);

    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration( "1.0", "utf-8", "" );
    doc.LinkEndChild( decl );

    //<field name="oscillator_electric_field" type="electric_field" dimension="3" number_of_nodes="1331">

    TiXmlElement* headelement = new TiXmlElement( "field" );
    headelement->SetAttribute("name","control_field");
    headelement->SetAttribute("type","control_field");
    headelement->SetAttribute("dimensions","3");
    headelement->SetAttribute("number_of_nodes",number_of_nodes_string.data());
    doc.LinkEndChild( headelement );

    for(int node_number =1; node_number <= number_of_nodes; node_number++ ) {

        std::string fieldVector = "";

        fieldVector.append(std::to_string(control.at(node_number)[0]).data());
        fieldVector.append(",");
        fieldVector.append(std::to_string(control.at(node_number)[1]).data());
        fieldVector.append(",");
        fieldVector.append(std::to_string(control.at(node_number)[2]).data());

        TiXmlText* text = new TiXmlText( fieldVector.data() );


        TiXmlElement* node = new TiXmlElement( "value" );
        node->SetAttribute("node_number",std::to_string(node_number).data());
        node->LinkEndChild( text );

        headelement->LinkEndChild( node );
    }



    doc.SaveFile( "Control_field.xml" );

    return 0;
}
