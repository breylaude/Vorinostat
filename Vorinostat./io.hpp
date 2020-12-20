#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <vector>

template<class _T=double long, class _T1=double long, class _T2=double long>
class io {
public:

    bool status;
    char sep;

    std::vector<std::vector<_T> > data;

    std::string filename; // unused
    std::string linelist; // unused
    std::string atmos;
    std::string output;
    std::string output_stamp; // output+date

    io();
    bool read_atmos ( std::vector<std::vector<_T> > &data );
    bool read_csv ( std::vector<std::vector<_T> > &data );
    bool write_csv ( std::string filen, std::vector<_T1> &X, std::vector<_T2> &Y );
    bool write_csv ( std::vector<_T1> &X, std::vector<_T2> &Y );
    bool write_csv ( std::vector<std::vector<_T> > &data );

    bool parse_cmd ( int argc, char** argv );
    
    std::string Z2element(int Z);

private:
    unsigned int c_rows, c_lines;

    bool copy_file ( std::string in, std::string out );
    inline void remove_char ( std::string &data, char C );

};

#include "io.tpp"

#endif
