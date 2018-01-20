#ifndef FASTAFILEREADER_H
#define FASTAFILEREADER_H

// for file reading
#define BUFFER_SIZE 4096

#include <string>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include "FASTAFile/FASTA_element.hpp"

class FASTAFileReader
{   /*  A class to handle connection and data fetching from FASTA files. This class works with the class FASTA_element. */

    public:
        // *** methods ****
        FASTAFileReader() ;
        FASTAFileReader(std::string fasta_file_address, bool one_based_seq, size_t sequence_alloc_size);
        ~FASTAFileReader();

        void           set_file(std::string fasta_file_address, bool one_based_seq, size_t sequence_alloc_size) ;
        FASTA_element* get_specific_entry(std::string header_match, bool exact_match) throw (std::runtime_error) ;
        FASTA_element* get_next() ;
        void           seek(size_t pos, std::ios_base::seekdir way) ;
        void           close() ;

    private:
        // *** methods ****
        // handle stream opening
        void open() throw (std::runtime_error) ;
        // method to retrieve the sequence after a header has been read
        bool get_sequence(FASTA_element* fasta_element) ;

        //*** fields ****
        // size of the memory allocation (std::string.reserve) for FASTA_element.sequence
        size_t _seq_allocation_size ;
        // the address of the fasta file to read
        std::string _f_seq_address ;
        // a flag indicating whether the file is open or not
        bool _f_seq_open ;
        // a stream to the fasta file
        std::ifstream _f_seq ;
        // a buffer to read from the file
        char _buffer[BUFFER_SIZE] ;
        // whether the sequences should be treated as 1-based
        bool _one_based_seq ;

};

#endif // FASTAFILEREADER_H
