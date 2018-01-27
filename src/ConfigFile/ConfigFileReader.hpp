#ifndef CONFIGFILEREADER_H
#define CONFIGFILEREADER_H

#include "Interfaces/FileReader.hpp"

#include <string>
#include <unordered_map>
#include <stdexcept>      // std::invalid_argument

using option_map  = std::unordered_map<std::string, std::string> ; // option -> value
using section_map = std::unordered_map<std::string, option_map> ;  // section -> option map

/*!
 * \brief Overloads the operator << for option_map objects.
 * \param o an output stream to send the unordered map to.
 * \param map the unordered map of interest.
 * \return a reference to the stream.
 */
std::ostream& operator << (std::ostream& o, const section_map& map) ;


/*!
 * \brief Converts a string to a boolean. Accepted values are
 * "0"/"1", "False"/"True", "false"/"true", "Off"/"On" and
 * "off"/"on".
 *  Any other value will raise an error.
 * \param s the string to convert.
 * \throw std::runtime_error in case <s> is not recognized.
 * \return a bool.
 */
bool stobool(const std::string& s) throw (std::invalid_argument) ;



class ConfigFileReader : public FileReader
{
    public:
        ConfigFileReader() ;
        ConfigFileReader(const std::string& file_adresse) ;

        virtual ~ConfigFileReader() override ;

        bool has_section(const std::string& section) ;
        bool has_option(const std::string& section, const std::string& option) ;

        section_map getOptions() const ;
        std::string getString(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        double      getDouble(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        int         getInt(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        bool        getBool(const std::string& section, const std::string& options) throw(std::runtime_error, std::invalid_argument) ;

    private:
        // *** methods ***
        void read_file() throw (std::runtime_error) ;
              section_map::iterator find_section(const std::string& section) ;
        const section_map::iterator find_section(const std::string& section) const ; // todo
              section_map::iterator find_option(const std::string& section, const std::string& option) ;
        const section_map::iterator find_option(const std::string& section, const std::string& option) const ; // todo
        void add_new_section(const std::string& section) ;
        void add_new_option(const std::string& section, const std::string& option, const std::string& value) ;
        // *** fields ***
        section_map _map ; // all sections are contained within one map with all their options
} ;

#endif // CONFIGFILEREADER_H
