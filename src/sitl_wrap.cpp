/*
* This is an open source non-commercial project. Dear PVS-Studio, please check it.
* PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
*/

#include "sitl.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wattributes"
#pragma GCC diagnostic ignored "-Wctor-dtor-privacy"
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wfloat-conversion"
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#include <boost/python.hpp>
#pragma GCC diagnostic pop

std::string Connection_str(const sitl::Connection&/*connection*/)
{
    return "Экземпляр класса соединения с утройство по протоколу SITL";
}


BOOST_PYTHON_MODULE(pysitl)
{
    using namespace boost::python;

    Py_Initialize();

    // Определяем формат docstring в функции help() в питоне
    docstring_options local_docstring_options(true, false, false);

    scope().attr("__author__")      = object( "Ivan Kalinin");
    scope().attr("__copyright__")   = object( "RC Module" );
    scope().attr("__email__")       = object( "i.kalinin@module.ru");

    {
        scope sitl_scope = class_<sitl::Connection, boost::noncopyable>("Connection", no_init)
            .def(init<const std::string&, unsigned int>(args("port", "baud_rate")))
            .def("write_memory", &sitl::Connection::writeMemory, args("address", "data"))
            .def("read_memory", &sitl::Connection::writeMemory, args("address"))
            .def("__str__", Connection_str)
            .def("__repr__", Connection_str)
            ;
    }
}
