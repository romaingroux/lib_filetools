#include "SGAFile/SGA_element.hpp"

SGA_element::SGA_element()
{   this->chromosome = std::string() ;
    this->feature    = std::string() ;
    this->position   = 0;
    this->strand     = std::string() ;
    this->count      = 0 ;
}

SGA_element::SGA_element(std::string chromosome, std::string feature, size_t position, std::string strand, size_t count)
throw (std::invalid_argument)
{   /*  Constructs a SGA_element.
 *      chromosome      the chromsome on which the element is.
 *      feature         a description of what the element is (i.e. "CTCF_Peak")
 *      position        the position on the chromsome.
 *      strand          a string indicating the strand on which the element is located. If it sits on both strands,
 *                      use "0".
 *      count           the number of this element located at the corresponding genomic coordinates.
 *
 *      Dynamic allocation
 *              None
    */

    // SGA is one based, check position is positive
    if(position < 1)
    { throw std::invalid_argument("SGA_element error! position should be > 0!") ; }
    // check strand is +,-,0
    if(strand.compare("+") != 0 && strand.compare("-") != 0 && strand.compare("0") != 0)
    { throw std::invalid_argument("SGA_element error! strand should be +, -, 0!") ; }
    // check count is positive
    if(count < 1)
    { throw std::invalid_argument("SGA_element error! count should be > 0!") ; }

    this->chromosome = chromosome ;
    this->feature = feature ;
    this->position = position ;
    this->strand = strand ;
    this->count = count ;
}

SGA_element::SGA_element(const std::string& str)
throw(std::invalid_argument)
{   /*  Construct a SGA element from a string. Typically, this method should be used when reading a file.
 *      str     a string containing at least the 5 mendatory SGA fields. Any other field will no be taken into account.
 *
 *      Dynamic allocation
 *              None
 */
    // checks that the string is not empty
    if(str.compare("") == 0)
    {   throw std::invalid_argument("SGA_element error! cannot build an instance from empty string") ; }

    std::istringstream input(str) ;
    std::string buffer ;
    size_t field_n = 0 ;
    for(size_t i=0; i<5; i++)
    {   input >> buffer ;
        switch(i)
        {   // chromosome
            case 0 : {  this->chromosome = buffer ;
                        field_n++ ; break ;
                     }
            // feature
            case 1 : {  this->feature = buffer ;
                        field_n++ ; break ;
                     }
            // position
            case 2 : {  int buffer2 = stoi(buffer) ;
                        if(buffer2 <= 0)
                        {   throw std::invalid_argument("SGA_element error! position should be > 0!") ; }
                        this->position = (size_t) buffer2 ;
                        field_n++ ; break ;
                     }
            // strand
            case 3 : {  if(buffer.compare("+") != 0 && buffer.compare("-") != 0 && buffer.compare("0") != 0)
                        { throw std::invalid_argument("SGA_element error! strand should be +, -, 0!") ; }
                        this->strand = buffer ;
                        field_n++ ; break ;
                     }
            // count
            case 4 : {  int buffer2 = stoi(buffer) ;
                        if(buffer2 <= 0)
                        { throw std::invalid_argument("SGA_element error! count should be > 0!") ; }
                        this->count = (size_t) buffer2 ;
                        field_n++ ; break ;
                     }
            // if more fields
            default : field_n++ ; break ;
        }
    }
    // less than 5 fields where contained in the line and it did not trigger any exception so far
    if(field_n < 4)
    {   throw std::invalid_argument("SGA_element error! the line contained less than 5 fields!") ; }
}

SGA_element::~SGA_element()
{}


// operator overloading
bool SGA_element::operator == (const SGA_element& other)
{   /*  Overloading of the '==' operator to compare two instances.
 *
 *      Return  a boolean, true if both instances are the same, false otherwise.
 *
 *      Dynamic allocation
 *              None
 */
    if((this->chromosome).compare(other.chromosome) == 0 &&
       (this->feature).compare(other.feature) == 0 &&
       (this->position) == other.position &&
       (this->strand).compare(other.strand) == 0 &&
       (this->count) == other.count)
    {   return true ; }
    else
    {   return  false ; }
}


std::ostream& operator << (std::ostream& stream, const SGA_element& sga)
{   /*  Friend function overloading the << operator for displaying SGA element.
 *
 *      Return  a pointer to the stream
 *
 *      Dynamic allocation
 *              None
 */
    stream << sga.chromosome << SGA_DELIMITER ;
    stream << sga.feature    << SGA_DELIMITER ;
    stream << sga.position   << SGA_DELIMITER ;
    stream << sga.strand     << SGA_DELIMITER ;
    stream << sga.count ;
    return stream ;
}


int SGA_element::get_distance(const SGA_element& other, int& error_byte)
{   /*  Return the distance in bp between 2 SGA_element instances. If these instances
 *      are not on the same chrosomomes, no distance can be computed and error_byte is
 *      set to 1 and 0 is returned. If both instances are on the same chromosome, error_byte
 *      is set to 0 and the distance (other - this) is returned.
 *      other       another SGA_element to compute the distance from
        error_byte  a reference to a int, if the distance can be computed, the value pointed will
 *                  be set to 0, otherwise to 1.
 *
 *      Return      the distance (other - this), 0 if no distance could be computed. Be careful,
 *                  0 may be a correct results.
 *
 *      Dynamic allocation
 *              None
*/
    // both aren't on the same chromosome, set error_byte to 1 and return 0
    if(this->chromosome.compare(other.chromosome) != 0)
    {   error_byte = 1 ;
        return 0 ;
    }
    // both are on the same chromosome, set error_byte to 0 and return the distance (may be 0)
    else
    {   error_byte = 0 ;
        return ((int)other.position - (int)this->position) ;
    }
}
