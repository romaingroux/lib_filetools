#include "FASTAFile/FASTA_element.hpp"

FASTA_element::FASTA_element()
{   sequence_length = 0 ;
    sequence_one_based = false ;
}

FASTA_element::~FASTA_element()
{}

// **** functions ****
size_t hash(const std::string& sequence)
{
    size_t h = 0 ;
    size_t i = 0 ;
    size_t n = 5 ; // the number of letters of the alphabet
    size_t length = sequence.length() ;
    std::string::const_iterator iter = sequence.begin() ;

    for( ; iter!=sequence.end(); iter++)
    {   switch(*iter)
        {   case 'A' : h += ((size_t)0)*pow(n, (length-i-1)) ; break ;
            case 'C' : h += ((size_t)1)*pow(n, (length-i-1)) ; break ;
            case 'G' : h += ((size_t)2)*pow(n, (length-i-1)) ; break ;
            case 'N' : h += ((size_t)3)*pow(n, (length-i-1)) ; break ;
            case 'T' : h += ((size_t)4)*pow(n, (length-i-1)) ; break ;
        }
        i++ ;
    }
    return h ;
}

size_t hash(std::string::const_iterator iter, size_t length)
{   size_t h = 0 ;
    size_t n = 5 ; // the number of letters of the alphabet

    for(size_t i=0 ; i<length; i++, iter++)
    {   switch(*iter)
        {   case 'A' : h += ((size_t)0)*pow(n, (length-i-1)) ; break ;
            case 'C' : h += ((size_t)1)*pow(n, (length-i-1)) ; break ;
            case 'G' : h += ((size_t)2)*pow(n, (length-i-1)) ; break ;
            case 'N' : h += ((size_t)3)*pow(n, (length-i-1)) ; break ;
            case 'T' : h += ((size_t)4)*pow(n, (length-i-1)) ; break ;
        }
    }
    return h ;
}


// operator overloading
bool FASTA_element::operator == (const FASTA_element& other) const
{   if((this->header).compare(other.header) == 0 &&
       (this->sequence).compare(other.sequence) == 0 &&
       (this->sequence_one_based) == other.sequence_one_based &&
       (this->sequence_length)    == other.sequence_length)
    {   return true ; }
    else
    {   return  false ; }
}


std::ostream& operator << (std::ostream& stream, const FASTA_element& fasta_element)
{   stream << fasta_element.header << std::endl ;
    stream << fasta_element.sequence ;
    return stream ;
}
