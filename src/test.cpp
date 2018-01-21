#include "ConfigFile/ConfigFileReader.hpp"
#include <string>
#include <iostream>


int main()
{	
	ConfigFileReader cfr ;
	std::string section("sectiaon") ; std::string opt("optioan") ;
	std::string s = cfr.getString(section, opt) ;
	int         n = cfr.getInt(section, opt) ;
	double      x = cfr.getDouble(section, opt) ;
	std::cout << "string " << s << std::endl ;
	std::cout << "int    " << n << std::endl ;
	std::cout << "double " << x << std::endl ;
	
	return EXIT_SUCCESS ;
}
