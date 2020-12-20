#include <iostream>
#include <vector>
#include <string>
#include <ctime> //timer
#include <algorithm> //min max

#include "io.hpp"
#include "func.hpp"
#include "cte.hpp"
#include "py_wrapper.hpp"


int main ( int argc, char** argv ) {
    clock_t start=clock();

// init class
    func<> _func; // assume double long
    io<> _io;
    py_wrapper _wrapper;

// parse cmd line and get params
    _io.parse_cmd ( argc, argv );

// init arrays
    std::vector<std::vector<double long> > data;

    std::vector<double long> Z_I, Z_II, Z_III;    
    std::vector<double long> res_I, res_II, res_III; // mid computations

    std::vector<double long> ratio_I_I_II_III; // ne(FeI) / ( ne(FeI) + ne(FeII) + ne(Fe III) )
    std::vector<double long> ratio_II_I_II_III; 
    std::vector<double long> ratio_III_I_II_III;

    std::vector<double long> opt_depth;


// on lit les données du modele fort.8
    _io.read_atmos ( data );

// on calcul les polynomes des fonctions de partition avec T
    Z_I=_func.Z_func ( data[1], Z_number ); 
    Z_II=_func.Z_func ( data[1], Z_number+0.01 );
    Z_III=_func.Z_func ( data[1], Z_number+0.02 );
   
// Compute pop ratios
    res_II=_func.saha_Ne ( Z_II, Z_I, data[3], data[1], Z_number+0.01 );
    res_III=_func.saha_Ne ( Z_III, Z_I, data[3], data[1], Z_number+0.02 );
    
    ratio_II_I_II_III=_func.ratio_3elem ( res_II, res_III );
    ratio_III_I_II_III=_func.ratio_3elem ( res_III, res_II );

    ratio_I_I_II_III=_func.add_vector(
                        _func.lin_transform_vector( ratio_II_I_II_III , -1.0, 1.0),
                        _func.lin_transform_vector( ratio_III_I_II_III, -1.0, 0.0) );

    opt_depth=_func.log10_vec ( data[0] );

// write data
    io<> _io_1, _io_2, _io_3;

    _io_1.output=_io.output;
    _io_2.output=_io.output;
    _io_3.output=_io.output;

    _io_1.write_csv ( opt_depth, ratio_I_I_II_III );
    _io_2.write_csv ( opt_depth, ratio_II_I_II_III );
    _io_3.write_csv ( opt_depth, ratio_III_I_II_III );


// plot with py.matplotlib wrapper

    _wrapper.plot ( _io_1.output_stamp, "Population ratio as function of Optical depth", "$log(\\\\tau_{5000}$)","$\\\\frac{n_{"+Z_name+"I}}{n_{"+Z_name+"I}+n_{"+Z_name+"II}+n_{"+Z_name+"III}}$" );
    _wrapper.plot ( _io_2.output_stamp, "Population ratio as function of Optical depth", "$log(\\\\tau_{5000}$)","$\\\\frac{n_{"+Z_name+"I}}{n_{"+Z_name+"I}+n_{"+Z_name+"II}+n_{"+Z_name+"III}}$" );
    _wrapper.plot ( _io_3.output_stamp, "Population ratio as function of Optical depth", "$log(\\\\tau_{5000}$)","$\\\\frac{n_{"+Z_name+"I}}{n_{"+Z_name+"I}+n_{"+Z_name+"II}+n_{"+Z_name+"III}}$" );
    
    std::cout << "elapsed time: " << ( clock()-start ) / ( ( float ) CLOCKS_PER_SEC ) << " s\n";
    return 0;
}
