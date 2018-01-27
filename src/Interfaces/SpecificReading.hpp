#ifndef SPECIFICREADER_H
#define SPECIFICREADER_H

#include <string>
#include <stdexcept>
#include <unordered_map>
#include "Interfaces/Data_element.hpp"


class SpecificReading
{
    public:
        virtual ~SpecificReading() = 0 ;

         /*!
         * \brief  This method search in the file an entry having a header corresponding to one
         * given as argument and return a pointer to an object containing the entry. The returned
         * element should be deleted to avoid memory leaks.
         * \param pattern a pattern to search in the file entry headers.
         * \throw std::runtime_error if the stream to the file is not opened.
         * \return a pointer to an object containing the entry data, a nullptr if no entry
         * with a corresponding header was found.
         */
        virtual Data_element* get(const std::string& pattern) throw (std::runtime_error) = 0 ;

    protected:
        /*!
         * \brief This method should implement a test to check whether a string matches
         * a given pattern.
         * \param candidate the entry header to check for the pattern.
         * \param pattern a pattern to search in the file entry headers.
         * \return whether the candidate matches the pattern.
         */
        virtual bool matchesPattern(const std::string& candidate, const std::string& pattern) const = 0 ;

        /*!
         * \brief This method should implement the way the map storing the pairs of header/pointer
         * is filled.
         */
        virtual void fillMap() = 0 ;

        /*!
         * \brief Gets the position of the entry having the given header in the file.
         * \throw std::runtime_error if no entry with this header could be found.
         * \return the position in the file of the entry starting with this header.
         */
        long long get_entry_position(const std::string& header) const throw (std::runtime_error) ;

        /*!
         * \brief A map to store pairs of entry header and pointer position for the beginning of
         *  the entry with this header in the file. Pointers are offsets from the start of the
         * file.
         */
        std::unordered_map<std::string, long long> _entry_map ;

} ;

#endif // SPECIFICREADER_H
