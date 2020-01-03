#ifndef OUTPUT_CONTROL_UPDATE_H
#define OUTPUT_CONTROL_UPDATE_H

#include <string>
#include <map>
#include <vector>

#include "tinyXML/tinyxml.h"


/**
 * @brief The output_control_update class offers functions to write the update
 * of the control in a file that is readable by the solver for forward and backward
 * equation
 */

class output_control_update
{
public:
    output_control_update();

    /**
     * @brief writeControl_XML
     * @param control
     * @return
     */
    int writeControl_XML(std::map<int,std::vector<double>> control);
};

#endif // OUTPUT_CONTROL_UPDATE_H
