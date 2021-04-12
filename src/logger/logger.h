#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/utility/setup/file.hpp>

namespace logger {

void InitLog();

/** \brief Base function for logging of exceptions defined in the standard
 * library */
void STLException(std::exception &e);

void Trace(const std::string &text);
void Debug(const std::string &text);
void Warning(const std::string &text);
void Info(const std::string &text);

}; // namespace logger

#endif
