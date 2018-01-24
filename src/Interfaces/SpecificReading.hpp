#ifndef SPECIFICREADER_H
#define SPECIFICREADER_H

#include <string>
#include <stdexcept>
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
} ;

#endif // SPECIFICREADER_H
