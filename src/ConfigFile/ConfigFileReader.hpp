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
{   // static member
    public:
        /*!
         * \brief a special value attributed to options for which not value
         * could be found within the file.
         */
        static std::string novalue ;

    public:
        ConfigFileReader() ;

        /*!
         * \brief Constructs a ConfigFileReader object from the given file. At return
         * time, the file has been entirely read and its content stored within the
         * object. Additionally, the stream to the file is closed.
         * \param file_adresse
         * \throw std::runtime_error if a same section is found more than once or
         * if a given option is found more than once in the same section.
         */
        ConfigFileReader(const std::string& file_adresse) throw (std::runtime_error) ;

        virtual ~ConfigFileReader() override ;

        /*!
         * \brief Assignment operator.
         * \param other an other instance to get the values from.
         * \throw std::runtime_error if a same section is found more than once or
         * if a given option is found more than once in the same section.
         * \return a reference to the current instance.
         */
        ConfigFileReader& operator = (const ConfigFileReader& other) throw (std::runtime_error) ;

        /*!
         * \brief Sets to which file the object should be streamed to.
         * At returned time, the file has been read, the map reset and filled
         * with new values and the connection to the stream is closed.
         * \param file_address file_address the file address.
         * \throw std::runtime_error if a same section is found more than once or
         * if a given option is found more than once in the same section.
         */
        void set_file(const std::string& file_address) throw (std::runtime_error) ;

        /*!
         * \brief Checks whether a given section was found in the file.
         * \param section a section name.
         * \return whether the section was found.
         */
        bool has_section(const std::string& section) const ;

        /*!
         * \brief Checks whether a given option within a given section was found
         * in the file.
         * \param section a section name.
         * \param options an option name.
         * \return whether the option within the given section was found.
         */
        bool has_option(const std::string& section, const std::string& option) const ;

        section_map get_options() const ;

        std::string getString(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        double      getDouble(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        int         getInt(const std::string& section, const std::string& options) throw(std::runtime_error) ;
        bool        getBool(const std::string& section, const std::string& options) throw(std::runtime_error, std::invalid_argument) ;

        /* makes no sense to keep them, stream is always read at once at construct time and
         * closed straight after */
        bool good() const = delete ;
        bool bad() const = delete ;
        bool eof() const = delete ;
        bool fail() const = delete ;

    private:
        // *** methods ***
        void read_file() throw (std::runtime_error) ;
        void reset_map() ;
        section_map::iterator find_section(const std::string& section) ;
        section_map::const_iterator find_section(const std::string& section) const ; // todo
        section_map::iterator find_option(const std::string& section, const std::string& option) ;
        section_map::const_iterator find_option(const std::string& section, const std::string& option) const ; // todo
        void add_new_section(const std::string& section) ;
        void add_new_option(const std::string& section, const std::string& option, const std::string& value) ;
        // *** fields ***
        section_map _map ; // all sections are contained within one map with all their options
} ;

#endif // CONFIGFILEREADER_H
