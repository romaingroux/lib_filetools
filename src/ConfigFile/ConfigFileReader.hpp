#ifndef CONFIGFILEREADER_H
#define CONFIGFILEREADER_H

#include "FileReader/FileReader.hpp"

#include <string>
#include <unordered_map>

using option_map  = std::unordered_map<std::string, std::string> ; // option -> value
using section_map = std::unordered_map<std::string, option_map> ;  // section -> option map

class ConfigFileReader : public FileReader
{
    public:
        ConfigFileReader() ;
        ConfigFileReader(const std::string& file_adresse) ;

        virtual ~ConfigFileReader() override ;

        bool has_section(const std::string& section) ;
        bool has_option(const std::string& section, const std::string& option) ;

        std::string getString(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        double      getDouble(const std::string& section, const std::string& options) const throw(std::runtime_error) ;
        int         getInt(const std::string& section, const std::string& options) const throw(std::runtime_error) ;

    private:
        // *** methods ***
        void read_file() throw (std::runtime_error) ;
        section_map::iterator find_section(const std::string& section) ;
        section_map::iterator find_option(const std::string& section, const std::string& option) ;
        void add_new_section(const std::string& section) ;
        void add_new_option(const std::string& section, const std::string& option, const std::string& value) ;
        // *** fields ***
        section_map _map ; // all sections are contained within one map with all their options
} ;

#endif // CONFIGFILEREADER_H
