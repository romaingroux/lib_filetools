#include "FASTAFile/FASTA_element.hpp"

FASTA_element::FASTA_element()
{   /*  Construct a FASTA_element with empty header and empty sequence.
 *
 *      Return
 *          Nothing
 *
 *      Dynamic allocation
 *          Nothing
 *  */
    sequence_length = 0 ;
    sequence_one_based = false ;
}

FASTA_element::~FASTA_element()
{}

// **** functions ****
size_t hash(const std::string& sequence)
{   /*  A function allowing to compute a hash from a DNA sequence composed of A,C,G,N,T. Other
 *      characters can easily be allowed by rewritting the function.
 *
 *      sequence    a reference to a string containing the sequence.
 *
 *      Return
 *          the hash of the function
 *
 *      Dynamic allocation$
 *          None
 *  */

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
{   /*  A function allowing to compute a hash from a DNA sequence composed of A,C,G,N,T. Other
 *      characters can easily be allowed by rewritting the function.
 *
 *      iter    a constant iterator on a string containing the sequence. The iterator should point
 *              to the first postion of interest of the sequence.
 *      length  the lenght of the sequence. The sequence is delimited in the string using <iter>
 *              and <length>.
 *
 *      Return
 *          the hash of the function
 *
 *      Dynamic allocation$
 *          None
 *  */
    size_t h = 0 ;
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
bool FASTA_element::operator == (const FASTA_element& other)
{   /*  Overloading of the '==' operator to compare two instances.
 *
 *      Return  a boolean, true if both instances are the same, false otherwise.
 *
 *      Dynamic allocation
 *              None
 */
    if((this->header).compare(other.header) == 0 &&
       (this->sequence).compare(other.sequence) == 0 &&
       (this->sequence_one_based) == other.sequence_one_based &&
       (this->sequence_length)    == other.sequence_length)
    {   return true ; }
    else
    {   return  false ; }
}


std::ostream& operator << (std::ostream& stream, const FASTA_element& fasta_element)
{   /*  A friendly function which overload the << operator for displaying FASTA_element.
 *
 *      stream          the stream to send the FASTA_element to
 *      fasta_element   the FASTA_element of interest
 *
 *      Return
 *          The reference of the stream used
 *
 *      Dynamic allocation
 *          Nothing
 *  */

    stream << fasta_element.header << std::endl ;
    stream << fasta_element.sequence ;
    return stream ;
}
