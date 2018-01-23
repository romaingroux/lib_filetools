#ifndef SGAFILEREADER_H
#define SGAFILEREADER_H

#include <string>
#include <iostream>
#include <fstream>
#include "SGA_element.hpp"
#include "Interfaces/FileReader.hpp"


/*!
 * \brief The SGAFIleReader class handles connection and data fetching
 * from SGA files. This class uses the class SGA_element.
 */
class SGAFileReader : public FileReader
{
    public:

        // **** methods ****
        /*!
         * \brief Construct a non-connected instance.
         * Connection can be established later using set_file()
         */
        SGAFileReader() ;
        /*!
         * \brief Construct an instance connected to a SGA file.
         * \param sga_file_address the SGA file address.
         */
        SGAFileReader(const std::string& sga_file_address) ;

        virtual ~SGAFileReader() override ;

        /*!
         * \brief This method returns the next SGA line
         * in _f_seq compared to the current file pointer
         * position. The returned SGA_element will need to be
         * deleted to avoid memory leaks.
         * \throw runtime_error upon an attempt to read from a closed file.
         * \throw invalid_argument when a SGA_element cannot be build with
         * what is read from the file.
         * \return a pointer to a dynamically allocated
         * SGA_element. If no line could be read from the
         * file, nullptr is returned.
         */
        SGA_element* get_next() throw (std::runtime_error, std::invalid_argument);

    private:
        // *** methods ****

        // **** fields ****
} ;

#endif // SGAFILEREADER_H
