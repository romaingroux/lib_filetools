#include "FileReader.hpp"

FileReader::~FileReader()
{}

void FileReader::set_file(const std::string& file_address)
{
    if(this->is_open())
    {   this->close() ; }
    this->_f_address = file_address ;
    this->open() ;
}

std::string FileReader::get_file() const
{   return this->_f_address ; }


bool FileReader::is_open() const
{   return this->_f_open ; }


void FileReader::close()
{   if(this->is_open())
    {   this->_f.close() ;
        this->_f_open = false ;
    }
}

void FileReader::open()
throw (std::runtime_error)
{   this->_f.open(_f_address, std::ios::in) ;
    if(this->_f.fail())
    {   char msg[512] ;
        sprintf(msg, "FileReader error! Could not open file %s\n", _f_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }
    this->_f_open = true ;
}


