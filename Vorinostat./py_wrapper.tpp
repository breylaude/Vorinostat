#include "py_wrapper.hpp"
#include "cte.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <unistd.h>



bool py_wrapper::plot( std::string filename,
                       std::string title,
                       std::string xlabel,
                       std::string ylabel ) {

    std::stringstream tmp0;
    std::string tmp1;
    tmp0 << sep;
    tmp0 >> tmp1;

	std::string code;

	code= "#!/usr/bin/env python \n\n";

	code+= "import math \n";
	code+= "import numpy as np \n";
	code+= "import matplotlib \n";
	code+= "import matplotlib.pyplot as plt\n";
	code+= "import csv \n\n";

    code+= "matplotlib.rc('axes', labelsize=15)  \n\n";

	code+= "X=[] \n";
	code+= "Y=[] \n\n";

	code+= "with open('" + filename + "','r') as csvfile: \n";
    code+= "   plots = csv.reader(csvfile, delimiter=' ') \n"; // ? delimiter

    code+= "   for row in plots: \n";
    code+= "       X.append(float(row[0])) \n";
    code+= "       Y.append(float(row[1])) \n\n";

    code+= "plt.plot(X, Y, linewidth=4) \n";
    code+= "plt.title('" + title + "') \n";
    code+= "plt.xlabel('" + xlabel + "') \n";
    code+= "plt.ylabel('" + ylabel + "') \n\n";

    code+= "plt.grid() \n\n";
    code+="plt.tight_layout()\n\n";

    code+="plt.savefig('"+filename+".pdf')\n\n";
    //code+="plt.show()\n\n";

	return exec(code);
}


bool py_wrapper::exec(std::string code) {
	status=true;

	std::string py_file="exec.py";
	std::string py_exec="python "+py_file;

	remove(py_file.c_str());

	std::fstream flux(py_file, std::ifstream::out | std::ifstream::app);

	if (flux.is_open()) {

        if(verbose) std::cout << "exec(): " << py_file << " opened.\n";

		flux << "import sys\n"+code+"\nsys.exit(1)"; // is this able to detect errors ?

		flux.close();

		if (!system(py_exec.c_str())) {
			std::cerr << "exec(): error while exec py file!\n";
			status=false;
		}
		else {
            if (verbose) std::cout << "exec(): python script executed.\n";
        }
	}
	else {
		std::cerr << "exec(): cannot open " << py_file << "!\n";
		status=false;
	}

	remove(py_file.c_str());

	return status;
}
