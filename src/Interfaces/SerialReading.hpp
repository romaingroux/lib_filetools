#ifndef SERIALREADING_H
#define SERIALREADING_H

#include "Interfaces/Data_element.hpp"

class SerialReading
{   public:
        virtual Data_element* get_next() throw (std::runtime_error, std::invalid_argument) = 0 ;
} ;



#endif // SERIALREADING_H
