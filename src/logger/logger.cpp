#include "logger.h"

void logger::InitLog() {
    boost::log::register_simple_formatter_factory<boost::log::trivial::severity_level, char>("Severity");
    boost::log::add_common_attributes();

    boost::log::add_console_log(
                std::cout, boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
                boost::log::keywords::filter = (boost::log::trivial::severity >= boost::log::trivial::debug)
            );

    boost::log::add_file_log(boost::log::keywords::file_name = "%Y-%m-%d_%H:%M:%S.log",
                             boost::log::keywords::format = "[%TimeStamp%] [%Severity%] %Message%",
                             boost::log::keywords::auto_flush = true);
}

void logger::STLException(std::exception& e) { BOOST_LOG_TRIVIAL(fatal) << e.what() << std::endl; }

void logger::Trace(const std::string& text) { BOOST_LOG_TRIVIAL(trace) << text; }

void logger::Debug(const std::string& text) { BOOST_LOG_TRIVIAL(debug) << text; }

void logger::Warning(const std::string& text) { BOOST_LOG_TRIVIAL(warning) << text; }

void logger::Info(const std::string& text) { BOOST_LOG_TRIVIAL(info) << text; }
