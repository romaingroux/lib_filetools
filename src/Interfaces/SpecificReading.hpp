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
         * \param exact_match whether the match between the header and the given pattern should
         * be exact (true) or only partial (false). These cases should be implemented in
         * isPattern() and hasPattern() methods.
         * \return a pointer to a object containing the data read.
         */
        virtual Data_element* get(const std::string& pattern, bool exact_match) throw (std::runtime_error) = 0 ;

    protected:
        /*!
         * \brief This method shouldimplement the test to check whether a match is
         * partial.
         * \param candidate the entry header to check for the pattern.
         * \param pattern a pattern to search in the file entry headers.
         * \return whether the candidate contains the pattern (which does not mean an
         * exact match).
         */
        virtual bool hasPattern(const std::string& candidate, const std::string& pattern) const = 0;

        /*!
         * \brief This method should implement the test to check whether a match is
         * exact.
         * \param candidate the entry header to check for the pattern.
         * \param pattern a pattern to search in the file entry headers.
         * \return whether the candidate contains the pattern and can be
         * considered as an exact match.
         */
        virtual bool isPattern(const std::string& candidate, const std::string& pattern) const = 0 ;

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
