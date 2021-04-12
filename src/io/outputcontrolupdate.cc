#include "outputcontrolupdate.h"

OutputControlUpdate::OutputControlUpdate(std::shared_ptr<MOTIONS::InputData> &input_data) {
    this->setInput_data(input_data);
}

int OutputControlUpdate::WritecontrolXml(arma::mat control) {
    int number_of_nodes = static_cast<int>(control.n_rows);
    std::cout << "Number of nodes: " << number_of_nodes << std::endl;
    std::string number_of_nodes_string = std::to_string(number_of_nodes);

    TiXmlDocument doc;
    TiXmlDeclaration *decl = new TiXmlDeclaration("1.0", "utf-8", "");
    doc.LinkEndChild(decl);

    //<field name="oscillator_electric_field" type="electric_field" dimension="3"
    //number_of_nodes="1331">

    TiXmlElement *headelement = new TiXmlElement("field");
    headelement->SetAttribute("name", "control_found_by_optimizer");
    headelement->SetAttribute("type", "control_field");
    headelement->SetAttribute("dimensions", "3");
    headelement->SetAttribute("number_of_elements",
                              number_of_nodes_string.data());
    doc.LinkEndChild(headelement);

    for (int cell_id = 1; cell_id <= number_of_nodes; cell_id++) {

        std::string fieldVector = "";

        fieldVector.append(std::to_string(control(cell_id - 1, 0)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(cell_id - 1, 1)));
        fieldVector.append(",");
        fieldVector.append(std::to_string(control(cell_id - 1, 2)));

        TiXmlText *text = new TiXmlText(fieldVector.data());

        TiXmlElement *node = new TiXmlElement("value");
        node->SetAttribute("cell_id", std::to_string(cell_id).data());
        node->LinkEndChild(text);

        headelement->LinkEndChild(node);
    }

    std::string path_to_shared_files = input_data_->path_to_shared_files;
    std::string control_field_cells_namme =
            input_data_->control_field_cells_name;

    std::string saveFile = path_to_shared_files + control_field_cells_namme;

    doc.SaveFile(&saveFile[0]);

    return 0;
}

int OutputControlUpdate::InterpolateControl(std::shared_ptr<MOTIONS::InputData> &input_data) {
    std::string path_to_shared_files = input_data->path_to_shared_files;
    std::string directory_toolset = input_data->directory_toolset;
    std::string domain_mesh = input_data->domain_mesh;

    int magnetic_force = input_data->magnetic_force;
    int electric_force = input_data->electric_force;

    std::string control_type = "";

    if (magnetic_force == 1 && electric_force == 0) {
        control_type = "magnetic_field";
    } else if (magnetic_force == 0 && electric_force == 1) {
        control_type = "electric_field";
    } else {
        std::cerr << "Force field not valid specified" << std::endl;
        throw std::runtime_error("Force field not valid specified");
    }

    std::string bgf_control = input_data->bgf_control;
    std::string control_field_cells_name =
            input_data->control_field_cells_name;
    std::string interpolating_control_python =
            "python3 " + directory_toolset + "GenerateControlField.py" + " " +
            path_to_shared_files + domain_mesh + " " + path_to_shared_files +
            control_field_cells_name + " " + path_to_shared_files + bgf_control +
            " " + control_type;

    int interpolating_flag = system(&interpolating_control_python[0]);
    if (interpolating_flag == 512) {
        throw std::runtime_error("File not found in control interpolating");
    } else if (interpolating_flag == 256) {
        throw std::runtime_error("Interpolating returned error value");
    }

    return interpolating_flag;
}
