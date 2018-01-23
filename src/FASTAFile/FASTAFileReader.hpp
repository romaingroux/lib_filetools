#ifndef FASTAFILEREADER_H
#define FASTAFILEREADER_H

#include <string>
#include <iostream>
#include <fstream>

#include "FileReader/FileReader.hpp"
#include "FASTAFile/FASTA_element.hpp"


// for file reading
#define BUFFER_SIZE 4096

/*!
 * \brief The FASTAFileReader class handles connection and data fetching from
 * FASTA files. This class uses the class FASTA_element.
 */
class FASTAFileReader : public FileReader
{
    public:
        // *** methods ****
        /*!
         * \brief Construct a non-connected instance. Connection can
         * be established later using set_file()
         */
        FASTAFileReader() ;
        /*!
         * \brief Construct an instance connected to a FASTA file.
         * If <one_based_seq> is set to true, the sequences are
         * sequences are treated as 1-based, 0-based otherwise.
         * \param fasta_file_address the file address.
         * \param one_based_seq whether to consider the sequences
         * as 1-based. If true, an meaningless '@' char will be
         * insterted at index 0 of every sequence reported from the
         * file to make the 1st char of the sequence be at index 1.
         * \param sequence_alloc_size a memory space (in number of char)
         * to reserve when searching in the file, to store the found
         * sequence. This can avoid recurrent reallocation of the space
         * and shorten the running time. As a rule of thumb, use the
         * average length of the sequence stored in the file. For instance,
         * if the sequences are 2Mb long and a space for only 2000 char is
         * reserved, then the program will have to reallocate memory at least
         * 10 times. Using a value higher than 2'000'000 will result in no
         * reallocation.
         */
        FASTAFileReader(const std::string& fasta_file_address, bool one_based_seq=false, size_t sequence_alloc_size=1000);

        virtual ~FASTAFileReader() override ;
        /*!
         * \brief Checks whether sequences are currently considered as 1-based.
         * \return Whether sequences are considered as 1-based (and thus whether
         * an initial '@' should be added). Sequences are considered 0-based
         * otherwise.
         */
        bool is_1based() const ;
        /*!
         * \brief Sets sequences as being considered as 1-based (and thus an
         * initial '@' will be added). This will affect the next call of
         * get_specific_entry() or get_next().
         */
        void set_1based() ;
        /*!
         * \brief Sets sequences as being considered as 0-based This will
         * affect the next call of get_specific_entry() and get_next().
         */
        void  set_0based() ;
        /*!
         * \brief Gets the size of the amount of memory allocated for
         * sequence reading (in number of characters).
         */
        size_t get_alloc_size() const ;
        /*!
         * \brief Sets the size of the amount of memory allocated for
         * sequence reading (in number of characters). This impacts the
         * run time of the next call of get_specific_entry() and get_next().
         */
        void set_alloc_size(size_t size) ;

        /*!
         * \brief This method allows to change the file to which the instance is connected. If
         * the a stream to the previous file was open, it is closed and a new stream to the new
         * file is open. _f_seq, _f_seq_address, _f_seq_open and _one_based_seq are reset
         * accordingly.
         * \param fasta_file_address the address of the new FASTA file to which the instance
         * should be connected to.
         * \param one_based_seq whether the sequences of this file should be considered as
         * 1-based or not. If true, an meaningless '@' char will be insterted at index 0 of
         * every sequence reported from the file to make the 1st char of the sequence be at
         * index 1.
         * \param sequence_alloc_size a memory space (in number of char) to reserve when
         * searching in the file, to store the found sequence. This can avoid recurrent
         * reallocation of the space and shorten the running time. As a rule of thumb,
         * use the average length of the sequence stored in _f_seq. For instance, if the
         * sequences are 2Mb long and a space for only 2000 char is reserved, then the program
         * will have to reallocate memory at least 10 times. Using a value higher than 2'000'000
         * will result in no reallocation.
         */
        void set_file(const std::string& fasta_file_address, bool one_based_seq=false, size_t sequence_alloc_size=1000) ;

        /*!
         * \brief  This method search in _f_seq an entry having a header corresponding to one
         *  given as argument and return the entry. The pointer in _f_seq is reset to the beginning
         * of the file at the start of this method and left where it is at return. The returned
         * FASTA_element should be deleted to avoid memory leaks.
         * \param header_match a pattern to search in the file headers.
         * \param exact_match whether the match between the header and the given pattern should
         * be exact (true) or only partial (false). A partial match is defined as a pattern being
         * a subsequence of the file header, i.g. file header "chromosome_101" and pattern "chromosome_10".
         * In this case, the first entry corresponding to a partial match is returned. An exact
         * match is defined as the pattern matching exactly a specific part of the file header.
         * For this the header is expected to be formatted as follows : [...]<pattern> [...] where
         * [...] can contain anything *BUT* <pattern> nor a space char (' ') and <pattern> is the
         * pattern to search. The method will look for an occurence of pattern ended by a space char.
         * i.g. "chromosome_10" and pattern "10" -> no match because no space after pattern
         * i.g. "chromosome_10 human" and pattern "10" -> match!
         * \throw runtime_error upon an attempt to read from a closed file.
         * \return A pointer to a dynamically FASTA_element if an entry has been found, a NULL
         * pointer otherwise. This FASTA_element should be deleted to avoid memory leaks.
         */
        FASTA_element* get_specific_entry(const std::string& header_match, bool exact_match) throw (std::runtime_error) ;

        /*!
         * \brief This method returns the next entry in _f_seq compared to the current file pointer
         * position. The next entry is defined as the next header followed by a sequence. The
         * returned FASTA_element will need to be deleted to avoid memory leaks.
         * \throw runtime_error upon an attempt to read from a closed file.
         * \return A pointer to a FASTA_element if an entry has been found, a NULL pointer
         * otherwise.
         */
        FASTA_element* get_next() throw (std::runtime_error) ;

    private:
        // *** methods ****
        // handle stream opening
        // method to retrieve the sequence after a header has been read
        /*!
         * \brief Reads the file from the current position to get a sequence.
         * This should be called only after a header has been read, never
         * else where.
         * \param fasta_element a pointer to a FASTA_element where the
         * sequence should be stored.
         * \return a boolean, whether some sequence could be read, which
         * means that initially the file state was correct (not eof) and
         * that the current line to read was not a header (starting with a
         * '>' character).
         */
        bool get_sequence(FASTA_element& fasta_element) ;

        //*** fields ****        
        /*!
         * \brief The size of the memory allocation (std::string.reserve) for
         * FASTA_element.sequence
         */
        size_t _seq_allocation_size ;

        /*!
         * \brief whether the sequences should be treated as 1-based in which
         * case all the sequences read will be added an extra '@' character
         * at position 0 to make the sequence start at position 1.
         */
        bool _one_based_seq ;

};

#endif // FASTAFILEREADER_H
