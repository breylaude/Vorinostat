#include "io.hpp"
#include "cte.hpp"

#include <iostream>
#include <algorithm> //remove
#include <fstream>
#include <vector>
#include <cmath>
#include <string>
#include <cstdio>
#include <iomanip> // setprecision
#include <regex>
#include <ctime>
#include <unistd.h>

template<class _T, class _T1, class _T2>
io<_T,_T1,_T2>::io() {	
    sep=SEPARATOR; 
    Z_name=Z2element(Z_number);
}


template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::read_atmos(std::vector<std::vector<_T> > &data) {
	status=false;

	std::string atmosh="."+atmos; // tmp file
	std::string pattern_1="READ";
	std::string pattern_2="PRADK";

	std::fstream flux;
	std::fstream flux_trim;

	remove(atmosh.c_str());

	flux.open(atmos,std::ifstream::in);
	flux_trim.open(atmosh,std::ifstream::out|std::ifstream::app);

	if (flux.is_open() && flux_trim.is_open()) {
		unsigned int L1=0, L2=0;
		unsigned int l=0;
		while(flux.good()) {
			l++;
			std::string buf;
			std::getline(flux,buf);

			if (buf.find(pattern_1)!=std::string::npos) L1=l-1;
			if (l>L1 && buf.find(pattern_2)!=std::string::npos) L2=l-1;

		}
		if (verbose) std::cout << "read_atmos(): trimming: (" << L1 << ", " << L2 << ").\n";
		flux.clear();
		flux.seekg(0,std::ios::beg);

		for(unsigned int i=0;i<L2;i++) {
			std::string buf0;
			std::getline(flux,buf0);

			if (i>L1 && i<L2) {
				flux_trim << buf0 << "\n"; // copy
			}
		}
		flux.close();
		flux_trim.close();

		filename=atmosh;

		if (read_csv(data)) {
			if (verbose) std::cout << "read_atmos(): atmospheric data stored successfully.\n";
		}
		else {
			std::cerr << "read_atmos() -> read_csv(): error !\n";
			status=false;
		}
	}
	else {
		std::cerr << "read_atmos(): cannot open files !\n";
		status=false;
	}
	remove(atmosh.c_str());

	return status;
}


template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::read_csv(std::vector<std::vector<_T> > &data) {

	status=false;

	std::ifstream flux;
	flux.open(filename.c_str(), std::ifstream::in);

	if (flux.is_open()) {
		if (verbose) std::cout << "file " << atmos << " opened.\n";

		status=true;
		c_lines=0;
		std::string buf0;

		/****** check file ******/

		if (verbose) std::cout << "read_csv(): check errors in file: ";

		std::regex regex_err_1(".*(\\d-\\d).*"); // e.g. "12-34"
		std::regex regex_err_2(".*(\\d+)\\.(\\d+)\\.(\\d+).*"); // e.g. "12.34.56"
		int err_1=0, err_2=0;
		while(flux.good()) {
			std::getline(flux,buf0);

			if (std::regex_match(buf0,regex_err_1)) {
				std::cout << "-> " << buf0 << " <-" << "\n";
				err_1++;
			}

			if (std::regex_match(buf0,regex_err_2)) {
				std::cout << "-> " << buf0 << " <-" << "\n";
				err_2++;
			}
		}
		if (err_1!=0) std::cout << err_1 << " errors digit-digit .\n";
		if (err_2!=0) std::cout << err_2 << " errors digit.digit.digit .\n";

		if (err_1==0 && err_2==0) {
			if (verbose) std::cout << "seems OK.\n";

			flux.clear();
			flux.seekg(0,std::ios::beg);

			/******* row *******/

			std::getline(flux,buf0);
            if (verbose) std::cout << "read_csv(): sep: ->" << sep << "<-\n";
			c_rows=0;
			for(unsigned int i=0;i<buf0.length()-1;i++) {
				if (buf0[i]==sep && buf0[i+1]!=sep) c_rows++;

			}

			if (verbose) std::cout << "read_csv(): file contains " << c_rows << " rows, based on the first line.\n";

			/****** line ******/
			c_lines=1;
			while(std::getline(flux,buf0)) c_lines++;

			flux.clear();
			flux.seekg(0,std::ios::beg);
			buf0.clear();

			/******* empty & fill vectors *******/

			for(unsigned int i=0;i<data.size();i++) {
				data[i].clear();
			}

			data.clear();
		        data.resize(c_rows);

			unsigned int line=0;
			while (flux.good()) {

				std::string buf;
				std::getline(flux,buf,sep);

				std::replace(buf.begin(),buf.end(),'\n',' ');
				remove_char(buf,sep);

				if (!buf.empty() && buf.c_str()!=std::to_string('\n')) {
					line++;
					data[(line-1)%c_rows].push_back(static_cast<_T> (std::stof(buf)));
				}
			}

			if (verbose) std::cout << "read_csv(): " << (c_lines) << " lines extracted.\n";
		}
		else { // else what, what else ?
			std::cerr << "\n";
			status=false;
		}
		flux.close();
	}
	else {
		std::cerr << "read_csv(): enable to open " << atmos << "!\n";
	}
	return status;
}


template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::write_csv(std::string filen, std::vector<_T1> &X, std::vector<_T2> &Y) {
	status=true;

	/****** get local date and time as string ******/
	time_t raw_t;
	struct tm *timeinfo;
	time(&raw_t);
	timeinfo=localtime(&raw_t);
	std::string hour=std::to_string(timeinfo->tm_hour);
	std::string minutes=std::to_string(timeinfo->tm_min);
	std::string sec=std::to_string(timeinfo->tm_sec);
	std::string mon=std::to_string(timeinfo->tm_mon+1);
	std::string day=std::to_string(timeinfo->tm_mday);
	filen+="_"+mon+day+"_"+hour+minutes+sec;

	/****** test if the file exists ******/
	int stamp_2=1;
	std::string filen_2=filen;
	std::ifstream f_exist(filen_2+"."+extname);
	while( f_exist.good() ) {
		stamp_2++;
		filen_2=filen+std::to_string(stamp_2);
		f_exist.close();
		f_exist.open(filen_2+"."+extname);
	}
	f_exist.close();

	filen=filen_2+"."+extname;
    output_stamp=filen;
	std::cout << "write_csv(): csv data destination: " << filen << " .\n";

	/***** write *****/
	std::fstream flux;

	remove(filen.c_str());

	flux.open(filen,std::fstream::out | std::fstream::app);
	if (flux.is_open()) {
		if (verbose) std::cout << filen << " opened. Writing... ";
		unsigned int dim=X.size();
		if (dim==Y.size()) {
			unsigned int i;
			for(i=0;i<dim;i++) 	flux << X[i] << sep << Y[i] << "\n";
			if (verbose) std::cout << i << " lines done.\n";
			if (verbose) std::cout << "file size: " << std::setprecision(2)  << (flux.tellg()/1024.0) << "K.\n";
		}
		else {
			std::cerr << "\nwrite_csv(): vector sizes do not match! " << X.size() << " - " << Y.size() << "\n";
			status=false;
		}
	flux.close();
	}
	else {
		std::cerr << "write_csv(): enable to open: " << filen << "!\n";
		status=false;
	}
	return status;
}


template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::write_csv(std::vector<_T1> &X, std::vector<_T2> &Y) {
	return write_csv(output,X,Y);
}


template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::write_csv(std::vector<std::vector<_T> > &data) {

	status=true;

	/****** get local date and time as string ******/
	time_t raw_t;
	struct tm *timeinfo;
	time(&raw_t);
	timeinfo=localtime(&raw_t);
	std::string hour=std::to_string(timeinfo->tm_hour);
	std::string minutes=std::to_string(timeinfo->tm_min);
	std::string sec=std::to_string(timeinfo->tm_sec);
	std::string mon=std::to_string(timeinfo->tm_mon+1);
	std::string day=std::to_string(timeinfo->tm_mday);
	output+="_"+mon+day+"_"+hour+minutes+sec;

	/****** test if the file exists ******/
	int stamp_2=1;
	std::string output_2=output;
	std::ifstream f_exist(output_2+"."+extname);
	while( f_exist.good() ) {
		stamp_2++;
		output_2=output+std::to_string(stamp_2);
		f_exist.close();
		f_exist.open(output_2+"."+extname);
	}
	f_exist.close();

	output=output_2+"."+extname;
	std::cout << "write_csv(): csv data destination: " << output << " .\n";

	/***** check consistancy of vector *****/
	bool err=false;
	for(unsigned int k=0;k<data.size()-1;k++)
		if (data[k].size()!=data[k+1].size()) err=true;

	if (!err) {

		/***** write *****/
		std::fstream flux;
		remove(output.c_str());

		flux.open(output,std::fstream::out | std::fstream::app);

		if (flux.is_open()) {
			if (verbose) std::cout << output << " opened. Writing... ";
			unsigned int i;
			for(i=0;i<data.size();i++) {
				for(unsigned int j=0;j<data[0].size();j++)
					flux << data[j][i] << " ";
				flux << "\n";
			}

			if (verbose) std::cout << i << " lines done.\n";
			if (verbose) std::cout << "file size: " << std::setprecision(2)  << (flux.tellg()/1024.0) << "K.\n";

			flux.close();
		}
		else {
			std::cerr << "\nwrite_csv(): enable to open: " << output << "!\n";
			status=false;
		}
	}
	else {
		std::cerr << "write_csv(): vector sizes do not match!\n";
		status=false;
	}
	return status;
}

template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::parse_cmd(int argc, char** argv) {
	status=false;

	if (argc>1) {
		std::string help;
        std::string p_name(argv[0]);
		help="Usage: "+p_name+" [OPTIONS]\n\n \
OPTIONS:\n \
-h              show this help\n \
-f              polynomial fit order of the partition functions: 5 or 8\n \
-l              linelist file, e.g. fort.19 (not implemented)\n \
-m filename     atmosphere model, e.g. fort.8 (required)\n \
-o filename     output file name (required)\n \
-s \"char\"       separator charactere\n \
-Z number       atomic number of the element\n \
-v              be verbose\n";

		int opt;
		while((opt=getopt(argc, argv, "f:hl:m:o:s:Z:v"))!=EOF) { // file, help, lines, model, outpout
			switch(opt) {
                case 'f':
					std::cout << "fit order: " << std::stoi(optarg) << " .\n";
					fit_order=std::stoi(optarg);
                    break;
				case 'l':
					std::cout << "linelist file: " << optarg << " .\n";
					linelist=optarg;
					break;
				case 'm':
					std::cout << "atmosphere model file: "  << optarg << " .\n";
					atmos=optarg;
					break;
				case 'o':
					std::cout << "output file: " << optarg << " .\n";
					output=optarg;
					break;
				case 's':
					std::cout << "separator char: ->" << optarg << "<-\n";
					sep=optarg[0];
					break;
				case 'h':
					std::cout << help;
					exit(EXIT_SUCCESS);
					break;
                case 'Z':
                    std::cout << "atomic number: " << optarg << "\n";
                    Z_number=std::stoi(optarg);
                    break;
				case 'v':
					verbose=true;
					break;
				case '?':
					std::cout << help;
					exit(EXIT_SUCCESS);
                	                break;
				default:
					std::cout << "error in args! Try -h.\n";
					exit(EXIT_FAILURE);
			}
        }
        if (output.empty()) {
            std::cerr << "output have to be given with -o !\n";
            exit(EXIT_FAILURE);
        }
        if (fit_order!=5 && fit_order!=8) {
            std::cout << "fit order set to " << 8 << ".\n";
            fit_order=8;
        }
        if (Z_number<1 || Z_number>118) {
            std::cerr << "invalid atomic number !\n";
            exit(EXIT_FAILURE);
        }
	}
	else {
		std::cout << "not enough args! Try -h\n";
		exit(EXIT_FAILURE);
	}
	return status;
}

template<class _T, class _T1, class _T2>
bool io<_T,_T1,_T2>::copy_file(std::string in, std::string out) {
	if (verbose) std::cout << "copy_file(): copying " << in << " to " << out << "...";

	std::fstream flux_in, flux_out;

	flux_in.open(in,std::ifstream::in);
	flux_out.open(out,std::ifstream::out|std::ifstream::app);

	if (flux_in.is_open() && flux_out.is_open()) {
		while(flux_in.good()) {
			std::string buf;
			std::getline(flux_in,buf);
			flux_out << buf << "\n";
		}
		status=true;
		flux_out.close();
		flux_in.close();
		if (verbose) std::cout << " done.\n";
	}
	else {
		std::cout << "copy_file(): error while trying to open files!\n";
		status=false;
	}
	return status;
}

template<class _T, class _T1, class _T2>
inline void io<_T,_T1,_T2>::remove_char(std::string &data, char C) {
	data.erase(std::remove(data.begin(),data.end(),C),data.end());
}

template<class _T, class _T1, class _T2>
std::string io<_T,_T1,_T2>::Z2element(int Z) {
    std::string name;
    switch (Z) {
        case 1:
            name="H";
            break;
        case 2:
            name="He";
            break;
        case 26:
            name="Fe";
            break;
        case 82:
            name="Pb";
            break;
        case 83:
            name="Bi";
            break;
        default:
            name="no_one";
            break;       
    }
    
    return name;
}
