#ifndef OUTPUT_CONTROL_UPDATE_H
#define OUTPUT_CONTROL_UPDATE_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "tinyXML/tinyxml.h"

#include "../controller/abstractcontroller.h"

/**
 * @brief The output_control_update class offers functions to write the update
 * of the control in a file that is readable by the solver for forward and backward
 * equation
 */
class OutputControlUpdate : public AbstractController
{
public:
    OutputControlUpdate();
    OutputControlUpdate(const char *filename);


    /**
     * @brief WritecontrolXml takes a control and writes a corresponding XML file
     * @param control (arma::mat)
     * @return 0 if processed successfully
     */
    int WritecontrolXml(arma::mat control);


    static int InterpolateControl(DataProvider provider);
};

#endif // OUTPUT_CONTROL_UPDATE_H
