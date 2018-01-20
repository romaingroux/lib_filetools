#include "FASTAFile/FASTAFileReader.hpp"

FASTAFileReader::FASTAFileReader()
{   /*  Construct a non-connected instance. Connection can be established later using this->set_file()
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    _f_seq_open = false ;
}

FASTAFileReader::FASTAFileReader(std::string fasta_file_address, bool one_based_seq, size_t sequence_alloc_size)
{   /*  Construct an instance connected to a FASTA file and set _f_seq, _f_seq_address, _f_seq_open and _one_based_seq
 *      accordingly.
 *
 *      fasta_file_address      the address of the FASTA file to which the instance should be connected-
 *      one_based_seq           whether the sequences of this file should be considered as 1-based or not.
 *                              If true, an meaningless '@' char will be insterted at index 0 of every sequence
 *                              reported from the file to make the 1st char of the sequence be at index 1.
 *      sequence_alloc_size     a memory space (in number of char) to reserve when searching in the file, to store the
 *                              found sequence. This can avoid recurrent reallocation of the space and shorten the running
 *                              time. As a rule of thumb, use the average length of the sequence stored in _f_seq. For
 *                              instance, if the sequences are 2Mb long and a space for only 2000 char is reserved, then
 *                              the program will have to reallocate memory at least 10 times. Using a value higher than
 *                              2'000'000 will result in no reallocation.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */

    _f_seq_open = false ;
    _f_seq_address = fasta_file_address ;
    _seq_allocation_size = sequence_alloc_size ;
    _one_based_seq = one_based_seq ;
    this->open() ;
}


FASTAFileReader::~FASTAFileReader()
{   this->close() ; }


void FASTAFileReader::set_file(std::string fasta_file_address, bool one_based_seq, size_t sequence_alloc_size)
{   /*  This method allows to change the file to which the instance is connected. If the a stream to the previous
 *      file was open, it is closed and a new stream to the new file is open. _f_seq, _f_seq_address, _f_seq_open
 *      and _one_based_seq are reset accordingly.
 *
 *      fasta_file_address      the address of the new FASTA file to which the instance should be connected.
 *      one_based_seq           whether the sequences of this file should be considered as 1-based or not.
 *                              If true, an meaningless '@' char will be insterted at index 0 of every sequence
 *                              reported from the file to make the 1st char of the sequence be at index 1.
 *      sequence_alloc_size     a memory space (in number of char) to reserve when searching in the file, to store the
 *                              found sequence. This can avoid recurrent reallocation of the space and shorten the running
 *                              time. As a rule of thumb, use the average length of the sequence stored in _f_seq. For
 *                              instance, if the sequences are 2Mb long and a space for only 2000 char is reserved, then
 *                              the program will have to reallocate memory at least 10 times. Using a value higher than
 *                              2'000'000 will result in no reallocation.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    if(_f_seq_open)
    {   this->close() ; }
    _one_based_seq = one_based_seq ;
    _seq_allocation_size = sequence_alloc_size ;
    _f_seq_address = fasta_file_address ;
    this->open() ;
}


FASTA_element* FASTAFileReader::get_specific_entry(std::string header_match, bool exact_match)
throw (std::runtime_error)
{   /*  This method search in _f_seq an entry having a header corresponding to to one given as argument and
 *      return the entry. The pointer in _f_seq is reset to the beginning of the file at the start of this
 *      method and left where it is at return.
 *
 *      header_match        a pattern to search in the file headers.
 *      exact_match         whether the match between the header and the given pattern should be exact (true)
 *                          or only partial (false).
 *                          A partial match is defined as a pattern being a subsequence of the file header,
 *                          i.g. file header "chromosome_101" and pattern "chromosome_10". In this case, the
 *                          first entry corresponding to a partial match is returned.
 *                          An exact match is defined as the pattern matching exactly a specific part of the
 *                          file header. For this the header is expected to be formatted as follows :
 *                          [...]<pattern> [...] where [...] can contain anything *BUT* <pattern> nor a space
 *                          char (' ') and <pattern> is the pattern to search. The method will look for
 *                          an occurence of pattern ended by a space char.
 *                          i.g. "chromosome_10" and pattern "10" -> no match because no space after pattern
 *                          i.g. "chromosome_10 human" and pattern "10" -> match!
 *
 *      Return
 *          A pointer to a FASTA_element if an entry has been found, a NULL pointer otherwise.
 *
 *      Dynamic allocation
 *          The FASTA_element returned if an entry has been found
 *
 *  */
    // if _f_seq is not open, raise an error
    if(not _f_seq_open)
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Attempt to read file %s which is closed!", _f_seq_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }

    // get back to file beginning
    _f_seq.clear() ;
    _f_seq.seekg(0, std::ios::beg) ;

    // value be returned
    FASTA_element* fasta_element = NULL ;


    // read file until finding the good chrosome header
    bool found_seq = false ;
    while(_f_seq.getline(_buffer, BUFFER_SIZE))
    {   // this line is a header line
        if('>' == _buffer[0])
        {   std::string header_line(_buffer) ;
            if(exact_match)
            {   // in the header, searches for an exact match of header_match. The header is expected to be formatted as
                // ">[...]header_match [...]" Thus search for header_match and a space (' '). The delimited substring
                // should exactly match header_match to be considered as a match.
                // example of an exact match
                // header_match : "NC_000001.10"
                // header_line :  ">chr|NC_000001|NC_000001.10 Chromosome 1; [Homo sapiens] ..."
                std::string header_line(_buffer) ;
                size_t start = header_line.find(header_match) ;
                size_t end   = header_line.find(" ") ;
                // the header conforms to the searched format
                if((start != std::string::npos) and (end != std::string::npos))
                {   std::string header_chrom = header_line.substr(start, end-start) ;
                    if(header_chrom == header_match)
                    {   // the header is the header of interest,
                        // the next read character will be the 1st char of the sequence :-)
                        // allocate memory for a FASTA element to be returned
                        fasta_element = new FASTA_element() ;
                        // reserve memory for the sequence
                        fasta_element->sequence.reserve(_seq_allocation_size) ;
                        fasta_element->header = header_chrom ; // stores the exact match searched for as header
                        found_seq = true ;

                        break ;
                    }
                }
            }
            else
            {   // in this case, only search whether header_match appears inside the header.
                // This may be dangerous because if the header_match is only a substring of the header currently
                // being tested and not *EXACTLY* the one wanted, this header will be seen as ok and the sequence
                // returned.
                // example of a match:
                // header_match : "NC_000001.1"
                // header_line :  ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] ..."
                // header_line :  ">chr|NC_000001|NC_000001.10 Chromosome 1; [Homo sapiens] ..."
                // these two example of header may be match. The returned one will be the first encountered.
                if(header_line.find(header_match) != std::string::npos)
                {   // the header is the header of interest,
                    // the next read character will be the 1st char of the sequence :-)
                    // allocate memory for a FASTA element to be returned
                    fasta_element = new FASTA_element() ;
                    // reserve memory for the sequence
                    fasta_element->sequence.reserve(_seq_allocation_size) ;
                    fasta_element->header = header_match ; // stores the complete header
                    found_seq = true ;                     // data in fasta_element are meaningfull
                    break ;
                }
            }
        }
        // this is not a header line
        else
        {   continue ; }
    }

    // if one_based is on, insert an meaning-less character at fasta_element.sequence beginning to make the
    // 1st character of the sequence be at index 1
    if(found_seq and _one_based_seq)
    {   fasta_element->sequence += "@" ; }

    // read sequence and stores it if the good header has been found
    found_seq = this->get_sequence(fasta_element) ;

    // store sequence length
    if(_one_based_seq and found_seq)
    {   // remove 1 to account for the first character added.
        fasta_element->sequence_length = fasta_element->sequence.length() - 1 ;
    }
    else if(found_seq)
    {   fasta_element->sequence_length = fasta_element->sequence.length() ; }

    return fasta_element ;
}


FASTA_element* FASTAFileReader::get_next()
{   /*  This method returns the next entry in _f_seq compared to the current file pointer position. The next
 *      entry is defined as the next header followed by a sequence.
 *
 *      Return
 *          A pointer to a FASTA_element if an entry has been found, a NULL pointer otherwise.
 *
 *      Dynamic allocation
 *          The FASTA_element returned if an entry has been found.
 *  */
    // if _f_seq is not open, raise an error
    if(not _f_seq_open)
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Attempt to read file %s which is closed!", _f_seq_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }
    // value to be returned
    FASTA_element* fasta_element = NULL ;
    bool header_found = false ;
    bool seq_found = false ;
    // get the header
    while(_f_seq.getline(_buffer, BUFFER_SIZE))
    {   // looking for a header
        if(not header_found)
        {   // header found, this is an entry to be returned
            if(_buffer[0] == '>')
            {   // allocate memory for the FASTA_element to be returned
                fasta_element = new FASTA_element() ;
                // reserve memory for the sequence
                fasta_element->sequence.reserve(_seq_allocation_size) ;
                if(_one_based_seq)
                {   fasta_element->sequence += "@" ;
                    fasta_element->sequence_one_based = true ;
                }
                // store header
                fasta_element->header = _buffer ;
                header_found = true ;
                // leave the file pointer here,
                break ;
            }
        }
    }
    // get the sequence
    if(header_found)
    {   seq_found = this->get_sequence(fasta_element) ; }

    // store sequence length
    if(_one_based_seq and seq_found)
    {   // remove 1 to account for the first character added.
        fasta_element->sequence_length = fasta_element->sequence.length() - 1 ;
    }
    else if(seq_found)
    {   fasta_element->sequence_length = fasta_element->sequence.length() ; }
    return fasta_element ;
}


bool FASTAFileReader::get_sequence(FASTA_element* fasta_element)
{
    // before reading, check that the next character to be read from the file is not a header,
    // if not then the next line is a sequence
    bool seq_found = false ;
    while(_f_seq and (_f_seq.peek() != '>'))
    {   seq_found = true ;
        _f_seq.getline(_buffer, BUFFER_SIZE) ;
        fasta_element->sequence += _buffer ;
    }
    return seq_found ;
}


void FASTAFileReader::seek(size_t pos, std::ios_base::seekdir way)
{   /*  Method do modifiy the position of the pointer within _f_seq.
 *
 *      pos     the new wanted position of the pointer in _f_seq
 *      way     the reference to set pos from, i.e. ios_base::beg, ios_base::cur, ios_base::end
 *
 *      Return
 *          Nothing
 *      Dynamic allocation
 *          Nothing
 *  */
    _f_seq.clear() ;
    _f_seq.seekg(pos, way) ;
}


void FASTAFileReader::close()
{   /*  This method closes _f_seq stream and sets _f_seq_open to false.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    if(_f_seq_open)
    {   _f_seq.close() ;
        _f_seq_open = false ;
    }
}


void FASTAFileReader::open()
throw (std::runtime_error)
{   /*  This method handles to stream connection opening to the _f_seq_address and turns _f_seq_open
 *      accordingly.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *
 *  */
    _f_seq.open(_f_seq_address, std::ios::in) ;
    if(_f_seq.fail())
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Could not open file %s\n", _f_seq_address.c_str()) ;
        throw std::runtime_error(msg) ;
    }
    _f_seq_open = true ;
}
