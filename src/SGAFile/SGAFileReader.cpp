#include "SGAFile/SGAFileReader.hpp"

SGAFileReader::SGAFileReader()
{ this->_f_open = false ; }

SGAFileReader::SGAFileReader(const std::string& sga_file_address)
{   this->set_file(sga_file_address) ;
    // this->_f_address = sga_file_address ;
    // this->open() ;
}

SGAFileReader::~SGAFileReader()
{   this->close() ; }


SGA_element* SGAFileReader::get_next() throw (std::runtime_error)
{   // file is not open
    if(not this->is_open())
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Attempt to read file %s which is closed!", _f_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    SGA_element* sga_element = nullptr ;

    this->_f.getline(this->_buffer, BUFFER_SIZE) ;
    if(this->_f)
    {   sga_element = new SGA_element(this->_buffer) ; }

    return sga_element ;
}
