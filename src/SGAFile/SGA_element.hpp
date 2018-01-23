#ifndef SGA_ELEMENT_H
#define SGA_ELEMENT_H

#define SGA_DELIMITER '\t'

#include "Interfaces/Data_element.hpp"
#include <stdlib.h>
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>

class SGA_element : public Data_element
{   /*  A class to store information in a SGA file line. This class does only account for the first 5 fields. */

    public:
        SGA_element() ;
        SGA_element(std::string chromosome, std::string feature, size_t position, std::string strand, size_t count) throw (std::invalid_argument) ;
        SGA_element(const std::string& str) throw (std::invalid_argument) ;
        ~SGA_element() ;

        // the chromosome field
        std::string chromosome ;
        // the feature field
        std::string feature ;
        // the position field
        size_t         position ;
        // the strand field
        std::string strand ;
        // the count field
        size_t         count ;

        // operator overloading
        bool         operator == (const SGA_element& other) ;
        // others
        int          get_distance(const SGA_element& other, int& error_byte) ;
        // friendly functions
        friend std::ostream& operator << (std::ostream& stream, const SGA_element& sga) ;
} ;

#endif // SGA_ELEMENT_H
