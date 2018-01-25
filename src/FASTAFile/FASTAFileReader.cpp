#include "FASTAFile/FASTAFileReader.hpp"

FASTAFileReader::FASTAFileReader()
{   this->_f_open = false ; }


FASTAFileReader::FASTAFileReader(const std::string& fasta_file_address, bool one_based_seq, size_t sequence_alloc_size)
{   this->_f_open = false ;
    this->_f_address = fasta_file_address ;
    this->_seq_allocation_size = sequence_alloc_size ;
    this->_one_based_seq = one_based_seq ;
    this->open() ;
}


FASTAFileReader::~FASTAFileReader()
{   this->close() ; }


bool FASTAFileReader::is_1based() const
{   return this->_one_based_seq ; }


void FASTAFileReader::set_0based()
{   this->_one_based_seq = false ; }


void FASTAFileReader::set_1based()
{   this->_one_based_seq = true ; }


size_t FASTAFileReader::get_alloc_size() const
{   return this->_seq_allocation_size ; }


void FASTAFileReader::set_alloc_size(size_t size)
{   this->_seq_allocation_size = size ; }


void FASTAFileReader::set_file(const std::string& fasta_file_address, bool one_based_seq, size_t sequence_alloc_size)
{   if(this->is_open())
    {   this->close() ; }
    if(one_based_seq)
    {   this->set_1based() ; }
    else
    {   this->set_0based() ; }
    this->set_alloc_size(sequence_alloc_size) ;
    // modify _f_address and opens it
    FileReader::set_file(fasta_file_address) ;
    // fills the map
    this->fillMap() ;
}


FASTA_element* FASTAFileReader::get(const std::string& pattern, bool exact_match)
throw (std::runtime_error)
{
    // if not open, raise an error
    if(not this->is_open())
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Attempt to read file %s which is closed!", this->get_file().c_str()) ;
        throw std::runtime_error(msg) ;
    }

    // get back to file beginning
    this->_f.clear() ;
    this->_f.seekg(0, std::ios::beg) ;

    // value be returned
    FASTA_element* fasta_element = NULL ;


    // read file until finding the good chrosome header
    bool found_seq = false ;
    while(this->_f.getline(this->_buffer, BUFFER_SIZE))
    {   // this line is a header line
        if('>' == this->_buffer[0])
        {   std::string header_line(this->_buffer) ;
            if(exact_match)
            {   // in the header, searches for an exact match of pattern. The header is expected to be formatted as
                // ">[...]pattern [...]" Thus search for pattern and a space (' '). The delimited substring
                // should exactly match pattern to be considered as a match.
                // example of an exact match
                // pattern     : "NC_000001.10"
                // header_line :  ">chr|NC_000001|NC_000001.10 Chromosome 1; [Homo sapiens] ..."
                std::string header_line(this->_buffer) ;
                // the header conforms to the searched format
                if(this->isPattern(header_line, pattern))
                {   size_t start = header_line.find(pattern) ;
                    size_t end   = header_line.find(" ") ;
                    std::string header_chrom = header_line.substr(start, end-start) ;
                    if(header_chrom == pattern)
                    {   // the header is the header of interest,
                        // the next read character will be the 1st char of the sequence :-)
                        // allocate memory for a FASTA element to be returned
                        fasta_element = new FASTA_element() ;
                        // reserve memory for the sequence
                        fasta_element->sequence.reserve(this->get_alloc_size()) ;
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
                if(this->hasPattern(header_line, pattern))
                {   // the header is the header of interest,
                    // the next read character will be the 1st char of the sequence :-)
                    // allocate memory for a FASTA element to be returned
                    fasta_element = new FASTA_element() ;
                    // reserve memory for the sequence
                    fasta_element->sequence.reserve(this->get_alloc_size()) ;
                    fasta_element->header = pattern ; // stores the complete header
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
    if(found_seq and this->is_1based())
    {   fasta_element->sequence += "@" ; }

    // read sequence and stores it if the good header has been found
    found_seq = this->get_sequence(*fasta_element) ;

    // store sequence length
    if(this->is_1based() and found_seq)
    {   // remove 1 to account for the first character added.
        fasta_element->sequence_length = fasta_element->sequence.length() - 1 ;
    }
    else if(found_seq)
    {   fasta_element->sequence_length = fasta_element->sequence.length() ; }

    return fasta_element ;
}


FASTA_element* FASTAFileReader::get_next() throw (std::runtime_error, std::invalid_argument)
{   // if _f_seq is not open, raise an error
    if(not this->is_open())
    {   char msg[512] ;
        sprintf(msg, "FASTAFileReader error! Attempt to read file %s which is closed!", this->get_file().c_str()) ;
        throw std::runtime_error(msg) ;
    }
    // value to be returned
    FASTA_element* fasta_element = NULL ;
    bool header_found = false ;
    bool seq_found = false ;
    // get the header
    while(this->_f.getline(this->_buffer, BUFFER_SIZE))
    {   // looking for a header
        if(not header_found)
        {   // header found, this is an entry to be returned
            if(this->_buffer[0] == '>')
            {   // allocate memory for the FASTA_element to be returned
                fasta_element = new FASTA_element() ;
                // reserve memory for the sequence
                fasta_element->sequence.reserve(this->get_alloc_size()) ;
                if(this->is_1based())
                {   fasta_element->sequence += "@" ;
                    fasta_element->sequence_one_based = true ;
                }
                // store header
                fasta_element->header = this->_buffer ;
                header_found = true ;
                // leave the file pointer here,
                break ;
            }
        }
    }
    // get the sequence
    if(header_found)
    {   seq_found = this->get_sequence(*fasta_element) ; }

    // store sequence length
    if(this->is_1based() and seq_found)
    {   // remove 1 to account for the first character added.
        fasta_element->sequence_length = fasta_element->sequence.length() - 1 ;
    }
    else if(seq_found)
    {   fasta_element->sequence_length = fasta_element->sequence.length() ; }
    return fasta_element ;
}


bool FASTAFileReader::hasPattern(const std::string& candidate, const std::string& pattern) const
{   // check presence of subtring
    if(candidate.find(pattern) != std::string::npos)
    {   return true ; }
    else
    {   return false ; }
}


bool FASTAFileReader::isPattern(const std::string& candidate, const std::string& pattern) const
{   bool start = this->hasPattern(candidate, pattern) ;
    bool end   = this->hasPattern(candidate, " ") ;
    if(start and end)
    {   return true ; }
    else
    {   return false ; }
}


bool FASTAFileReader::get_sequence(FASTA_element& fasta_element)
{
    // before reading, check that the next character to be read from the file is not a header,
    // if not then the next line is a sequence
    bool seq_found = false ;
    while(this->_f and (this->_f.peek() != '>'))
    {   seq_found = true ;
        this->_f.getline(this->_buffer, BUFFER_SIZE) ;
        fasta_element.sequence += this->_buffer ;
    }
    return seq_found ;
}

#include <iomanip>

void FASTAFileReader::fillMap()
{   this->seek(0, std::ios::beg) ;
    char c ;

    while(this->_f.get(c))
    {   // header start
        if(c == '>')
        {   // get back 1 byte (right before header) and read whole header
            this->seek(-1, std::ios::cur) ;
            long long pos = this->_f.tellg() ;
            this->_f.getline(this->_buffer, BUFFER_SIZE) ;
            std::string header(this->_buffer) ;
            this->_entry_map.emplace(header, pos) ;
            std::cerr << std::setw(4) << pos << "   " << header << std::endl ;
        }
    }
    this->seek(0, std::ios::beg) ;
}
