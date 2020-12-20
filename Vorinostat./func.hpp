#ifndef FUNC_HPP
#define FUNC_HPP

#include <vector>

#include "io.hpp"

template<class _T=double long, class _T1=double long, class _T2=double long>
class func: public io<_T,_T1,_T2> {
public:

// Electronic Pressure
    std::vector<_T> e_pressure ( std::vector<_T> &N_e, std::vector<_T> &T );

// Saha equation
// Fill vector with -1 if div by 0
// Z_nb_e is ion with Kurucz conv.
    std::vector<_T2> saha_Ne (
        std::vector<_T1> &Z1,
        std::vector<_T1> &Z0,
        std::vector<_T2> &Ne,
        std::vector<_T2> &T,
        _T Z_nb_e );

    std::vector<_T2> saha_Pe (
        std::vector<_T1> &Z1,
        std::vector<_T1> &Z0,
        std::vector<_T2> &Pe,
        std::vector<_T2> &T,
        _T Z_nb_e );

// Polynomial partition function
// order is define by cmd line or by default to 8
// Z_nb_e is ion with Kurucz conv.
    std::vector<_T> Z_func ( std::vector<_T> &T, _T1 Z_nb_e );

// n_j / sum(n_i)
    std::vector<_T> ratio_2elem ( std::vector<_T> &pop_ratio );
    std::vector<_T> ratio_3elem ( std::vector<_T> &pop_ratio1, std::vector<_T> &pop_ratio2 );
    std::vector<_T> ratio_4elem ( std::vector<_T> &pop_ratio1, std::vector<_T> &pop_ratio2, std::vector<_T> &pop_ratio3 );

// log10
    std::vector<_T> log10_vec ( const std::vector<_T> &array );

// pow10
    std::vector<_T> pow10_vec( const std::vector<_T> &array );

// Add two vectors - bad behavior when overloading operator
    std::vector<_T> add_vector ( const std::vector<_T> &X, const std::vector<_T> &Y);

// a*X+b
    std::vector<_T> lin_transform_vector ( const std::vector<_T> &X, _T1 a, _T2 b);

    // Inv vector
    std::vector<_T> inv_vector ( const std::vector<_T> &X);
};


#include "func.tpp"

#endif
