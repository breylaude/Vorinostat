#ifndef PY_WRAPPER_HPP
#define PY_WRAPPER_HPP

#include <string>

#include "io.hpp"


class py_wrapper: public io<> { //assume double long
public:

    bool plot ( std::string filename,
                std::string title,
                std::string xlabel,
                std::string ylabel );

    bool exec ( std::string code );
};

#include "py_wrapper.tpp"

#endif
