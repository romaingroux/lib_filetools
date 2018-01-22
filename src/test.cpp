#include "ConfigFile/ConfigFileReader.hpp"
#include <string>
#include <iostream>


int main()
{	
	ConfigFileReader cfr("/home/romaingroux/dev/lib_FileTools/src/paths.ini") ;
	section_map options = cfr.getOptions() ;
	std::cout << options << std::endl ;
	std::cout << "section2 : opt3  :  " << cfr.getString("section2", "opt3") << std::endl ;
	std::cout << "section2 : opt2  :  " << cfr.getString("section2", "opt2") << std::endl ;
	std::cout << "section2 : opt1  :  " << cfr.getString("section2", "opt1") << std::endl ;
	std::cout << "section1 : int1  :  " << cfr.getInt("section1", "int1") << std::endl ;
	std::cout << "section1 : int1  :  " << cfr.getDouble("section1", "int1") << std::endl ;
	std::cout << "section1 : bool1 :  " << cfr.getBool("section1", "bool1") << std::endl ;
	return EXIT_SUCCESS ;
}
