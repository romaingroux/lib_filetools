#ifndef SGAFILEREADER_H
#define SGAFILEREADER_H

// for file reading
#define BUFFER_SIZE 4096

#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
#include "SGAFile/SGA_element.hpp"

class SGAFileReader
{   /*  A class to handle connection and data fetching from SGA files. This class works with the class SGA_element. */

    public:

        // **** methods ****
        SGAFileReader() ;
        SGAFileReader(std::string sga_file_address) ;
        ~SGAFileReader();

        void         set_file(std::string sga_file_address) ;

        SGA_element* get_next() ;
        void         close() ;

    private:
        // *** methods ****
        // handle stream opening
        void open() throw (std::runtime_error) ;

        // **** fields ****
        // the SGA file address
        std::string _f_sga_address;
        // a stream to the SGA file
        std::ifstream _f_sga ;
        // whether the connection is open
        bool _f_sga_open ;
        // a buffer to read from the file
        char _buffer[BUFFER_SIZE] ;
} ;

#endif // SGAFILEREADER_H
