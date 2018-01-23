#ifndef FASTA_ELEMENT_H
#define FASTA_ELEMENT_H

#include "Interfaces/Data_element.hpp"
#include <iostream>
#include <string>
#include <cmath>

class FASTA_element : public Data_element
{   /*  A class to store a FASTA entry. This class can store both the sequence and the header as well as a flag
 *      to indicate whether the sequence should be considered as 1-based or not.
 *  */
    public:
        FASTA_element() ;
        ~FASTA_element() ;

        // **** functions ****
        // operator overloading
        bool operator == (const FASTA_element& other) ;
        friend std::ostream& operator << (std::ostream& stream, const FASTA_element& fasta_element) ;


        // **** fields ****
        // entry header or part of the header
        std::string header ;
        // entry sequence
        std::string sequence ;
        // whether the sequence is one based. If true, the first character is meaningless
        // and only added to make the 1st character of the sequence at index 1
        bool   sequence_one_based ;
        // the sequence length, if 1 based it is the real string length - 1
        size_t sequence_length ;
} ;

// produces a hash out of the sequence
size_t hash(const std::string& sequence) ;
size_t hash(std::string::const_iterator iter, size_t length) ;

#endif // FASTA_ELEMENT_H
