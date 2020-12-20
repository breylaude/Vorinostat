#ifndef CTE_HPP
#define CTE_HPP

#include <string>

/*** Constantes ***/

extern const double long hbar;
extern const double long k_B;
extern const double long m_e;
extern const double long v_light;
extern const double long Ry;
extern const double long PI;


/*** E(X) ***/
// Fe
extern const double long E_Fe_FeI;
extern const double long E_FeI_FeII;
extern const double long E_FeII_FeIII;

//Pb
extern const double long E_Pb_PbI;
extern const double long E_PbI_PbII;
extern const double long E_PbII_PbIII;

// Bi
extern const double long E_Bi_BiI;
extern const double long E_BiI_BiII;
extern const double long E_BiII_BiIII;


/*** Z(X) Fitted ***/
// Fe
extern const double long Z_o5_FeI[];
extern const double long Z_o5_FeII[];
extern const double long Z_o5_FeIII[];

extern const double long Z_o8_FeI[];
extern const double long Z_o8_FeII[];
extern const double long Z_o8_FeIII[];

// Pb
extern const double long Z_o5_PbI[];
extern const double long Z_o5_PbII[];
extern const double long Z_o5_PbIII[];

extern const double long Z_o8_PbI[];
extern const double long Z_o8_PbII[];
extern const double long Z_o8_PbIII[];

// Bi
extern const double long Z_o5_BiI[];
extern const double long Z_o5_BiII[];
extern const double long Z_o5_BiIII[];

extern const double long Z_o8_BiI[];
extern const double long Z_o8_BiII[];
extern const double long Z_o8_BiIII[];



/*** g(X) ***/
// Fe
extern const double long g_Fe;
extern const double long g_FeI;
extern const double long g_FeII;
extern const double long g_FeIII;

extern int Z_number;
extern std::string Z_name;
extern bool verbose;
extern const char SEPARATOR;
extern const std::string extname;
extern unsigned int fit_order;

#endif
