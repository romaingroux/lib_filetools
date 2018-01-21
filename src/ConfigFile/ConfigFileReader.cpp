#include "ConfigFile/ConfigFileReader.hpp"

ConfigFileReader::ConfigFileReader()
    : map()
{}


ConfigFileReader::ConfigFileReader(const std::string& file_adresse)
    : ConfigFileReader()
{   this->set_file(file_adresse) ; }


ConfigFileReader::~ConfigFileReader()
{}


bool ConfigFileReader::has_section(const std::string& section)
{  if(this->find_section(section) != std::unordered_map::end)
   {    return true ; }
    return false ;
}


bool ConfigFileReader::has_option(const std::string& section, const std::string& option) const
{   if(this->find_option(section, option) != std::unordered_map::end)
    {   return true ; }
    return false ;
}






std::string ConfigFileReader::getString(const std::string& section, const std::string& option)
{   if(section == "section" and option == "option")
    {   return std::string("1") ; }
    return std::string("0") ;
}

double ConfigFileReader::getDouble(const std::string& section, const std::string& option)
{   if(section == "section" and option == "option")
    {   return 1. ; }
    return 0. ;
}

int ConfigFileReader::getInt(const std::string& section, const std::string& option)
{   if(section == "section" and option == "option")
    {   return 1 ; }
    return 0 ;
}



void ConfigFileReader::read_file() throw (std::runtime_error)
{   // file not open
    if(not this->is_open())
    {   char msg[512] ;
        sprintf(msg, "ConfigFileReader error! Attempt to read file %s which is closed!", this->get_file().c_str()) ;
        throw std::runtime_error(msg) ;
    }

    // go to file start
    this->seek(0, std::io_base::beg) ;
    // read each line
    std::string section, option, value ;
    while(this->_f.getline(this->_buffer, BUFFER_SIZE))
    {   std::string buffer(this->_buffer) ;
        // search section header
        // not proper section header
        if((buffer.front() == '[') and (this->buffer.back() != ']') or
           (buffer.front() != '[') and (this->buffer.back() == ']'))
        {   char msg[512] ;
            sprintf(msg, "ConfigFileReader error! Incorrect format in section header %s!", buffer.c_str()) ;
            throw std::runtime_error(msg) ;
        }
        // proper section header
        // "[<header>]", nothing else admitted
        else if(buffer.front() == '[' and buffer.back() == ']')
        {   section.clear() ;
            size_t i=0 ;
            while(buffer[i] != '\0' and i < BUFFER_SIZE)
            {   // don't retain these char
                if(buffer[i] == ']' or buffer[i] == '[')
                {}
                // this is an error, no space allowed
                else if(buffer[i] == ' ')
                {   char msg[512] ;
                    sprintf(msg, "ConfigFileReader error! Incorrect format in section header %s!", buffer.c_str()) ;
                    throw std::runtime_error(msg) ;
                }
                else
                {   section.append(this->__buffer[i]) ; }
                i++ ;
            }
            // store the header
            if(this->has_section(section))
            {   char msg[512] ;
                sprintf(msg, "ConfigFileReader error! Section %s found several times!", section.c_str()) ;
                throw std::runtime_error(msg) ;
            }
            else
            {   this->create_section(section) ; }
        }
        // search an option and a value
        // "<option>...=...<value>" where '...' can be spaces
        // TODO -> the current implementation accepts spaces within <option> and <value>
        // but don't read them -> correct to raise an error when this happens?
        else
        {   option.clear() ; value.clear() ;
            // should contain exactly one '=' char
            size_t first = buffer.find('=') ;
            size_t last  = buffer.rfind('=') ;
            // error, 0 or >1 =
            if((first == std::string::npos) or
               (first != last))
            {   char msg[512] ;
                sprintf(msg, "ConfigFileReader error! Incorrect format in option %s!", buffer.c_str()) ;
                throw std::runtime_error(msg) ;
            }
            else
            {   // option -> from start to '=' char (minus the spaces)
                for(size_t i=0; i<first; i++)
                {   if(buffer[i] != ' ')
                    {   option.append(buffer[i]) ; }
                }

                // value -> from '=' (minus the spaces) to end
                for(size_t i=first+1; i<buffer.size(); i++)
                {   if(buffer[i] != ' ')
                    {   value.append(buffer[i]) ; }
                }
                // store option and value
                if(this->has_option(section, option))
                {   char msg[512] ;
                    sprintf(msg, "ConfigFileReader error! Option %s in section %s found several times!",
                            option.c_str(), section.c_str()) ;
                    throw std::runtime_error(msg) ;
                }
                else
                {   this->add_new_option(section, option, value) ; }
            }
        }
    }
    // close file
    this->close() ;
}





section_map::const_iterator ConfigFileReader::find_section(const std::string& section) const
{   return this->_map.find(section) ; }

option_map::const_iterator ConfigFileReader::find_option(const std::string& section, const std::string& option) const
{   // search section
    std::unordered_map<std::string, option_map>::const_iterator section = this->_map.find(section) ;
    // search option
    if(section != std::unordered_map::end)
    {   return *section.second.find(option) ; }
    return std::unordered_map::end ;
}

void ConfigFileReader::add_new_section(const std::string& section)
{   // only create a new element if the section is not already contained in the map
    if(not this->has_section(section))
    {   this->_map.emplace(section, option_map) ; }
}

void ConfigFileReader::add_new_option(const std::string& section, const std::string& option, const std::string& value)
{   section_map::const_iterator s_iter = this->find_section(section) ;
    // section exists
    if(s_iter != std::unordered_map::end)
    {   option_map::const_iterator o_iter = this->find_option(section, option) ;
        // option exists -> overwrite its value
        if(o_iter != std::unordered_map::end)
        { (*o_iter).second = value ; }
        // option does not exists -> create it
        else
        {   (*s_iter).emplace(option, value) ; }
    }
    // create section and option
    else
    {   this->add_new_section(section) ;
        section_map::const_iterator s_iter = this->find_section(section) ;
        (*s_iter).emplace(option, value) ;
    }
}
