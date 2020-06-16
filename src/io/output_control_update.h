#ifndef OUTPUT_CONTROL_UPDATE_H
#define OUTPUT_CONTROL_UPDATE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "tinyXML/tinyxml.h"

#include "../controller/abstract_controller.h"

/**
 * @brief The output_control_update class offers functions to write the update
 * of the control in a file that is readable by the solver for forward and backward
 * equation
 */
class output_control_update : public abstract_controller
{
public:
    output_control_update();
    output_control_update(const char *filename);

    /**
     * @brief writeControl_XML
     * @param control
     * @return
     */
    int writeControl_XML(arma::mat control);
};

#endif // OUTPUT_CONTROL_UPDATE_H
