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

        bool has_section(const std::string& section) const ;
        bool has_option(const std::string& section, const std::string& option) const ;

        std::string getString(const std::string& section, const std::string& options) const ;
        double      getDouble(const std::string& section, const std::string& options) const ;
        int         getInt(const std::string& section, const std::string& options) const ;

    private:
        // *** methods ***
        void read_file() std::runtime_error ;
        section_map::const_iterator find_section(const std::string& section) const ;
        option_map::const_iterator find_option(const std::string& section, const std::string& option) const ;
        void add_new_section(const std::string& section) ;
        void add_new_option(const std::string& section, const std::string& option) ;
        // *** fields ***
        section_map _map ; // all sections are contained within one map with all their options
} ;

#endif // CONFIGFILEREADER_H
