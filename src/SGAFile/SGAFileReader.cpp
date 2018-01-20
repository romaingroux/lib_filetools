#include "SGAFile/SGAFileReader.hpp"

SGAFileReader::SGAFileReader()
{   /*  Construct a non-connected instance. Connection can be established later using this->set_file()
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    _f_sga_open = false ;
}

SGAFileReader::SGAFileReader(std::string sga_file_address)
{   /*  Construct an instance connected to a SGA file and set _f_sga, _f_sga_address, _f_sga_open accordingly.
 *
 *      sga_file_address      the address of the SGA file to which the instance should be connected.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */

    _f_sga_address = sga_file_address ;
    this->open() ;
}

SGAFileReader::~SGAFileReader()
{   this->close() ; }


void SGAFileReader::set_file(std::string sga_file_address)
{   /*  This method allows to change the file to which the instance is connected. If the a stream to the previous
 *      file was open, it is closed and a new stream to the new file is open. _f_sga, _f_sga_address and _f_sga_open
 *      are reset accordingly.
 *
 *      sga_file_address        the address of the new SGA file to which the instance should be connected.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */

    if(_f_sga_open)
    {   this->close() ; }
    _f_sga_address = sga_file_address ;
    this->open() ;
}


SGA_element* SGAFileReader::get_next()
{   /*  This method returns the next SGA line in _f_seq compared to the current file pointer position.
 *
 *      Return
 *          A pointer to a SGA_element if a line has been found, a nullptr pointer otherwise.
 *
 *      Dynamic allocation
 *          The SGA_element returned if an line has been found.
 *  */
    SGA_element* sga_element = nullptr ;

    _f_sga.getline(_buffer, BUFFER_SIZE) ;
    if(_f_sga)
    {   sga_element = new SGA_element(_buffer) ; }

    return sga_element ;
}


void SGAFileReader::close()
{   /*  This method closes _f_sga stream and sets _f_sga_open to false.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    if(_f_sga_open)
    {   _f_sga.close() ;
        _f_sga_open = false ;
    }
}

void SGAFileReader::open()
throw (std::runtime_error)
{   /*  This method handles to stream connection opening to the _f_sga_address and turns _f_sga_open
 *      accordingly.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *
 *  */
    _f_sga.open(_f_sga_address, std::ios::in) ;
    if(_f_sga.fail())
    {   char msg[512] ;
        sprintf(msg, "SGAFileReader error! Could not open file %s\n", _f_sga_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }
    _f_sga_open = true ;

}
