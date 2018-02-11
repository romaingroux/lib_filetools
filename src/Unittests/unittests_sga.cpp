#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "include/SGA_element.hpp"
#include "include/SGAFileReader.hpp"


// SGA_element unittests
SUITE(SGA_element_testsuit)
{
    std::string sgaline1("chr2\tMYC_P\t1000\t+\t1") ;
    std::string sgaline2("chr1\tCTCF_P\t20000\t-\t1") ;
    std::string sgaline3("chr1\tCTCF_P\t25000\t-\t1") ;

    std::string chr ("chr1") ;
    std::string feature("CTCF_P") ;
    std::string strand("-") ;
    int pos   = 20000 ;
    int count = 1 ;

    SGA_element sga0 ;
    SGA_element sga1 (sgaline1) ;
    SGA_element sga2 (sgaline2) ;
    SGA_element sga3 (sgaline3) ;
    SGA_element sga4 (chr, feature, pos, strand, count) ;

    TEST(message)
    {   std::cout << "Starting SGA_element tests..." << std::endl ; }

    // tests empty constructor
    TEST(TestSGA_element_empty_constructor)
    {   // get field values
        std::string value_chr      = sga0.chromosome ;
        std::string value_feature  = sga0.feature ;
        std::string value_strand   = sga0.strand ;
        int          value_pos     = sga0.position ;
        int          value_count   = sga0.count ;
        // check equality
        CHECK_EQUAL(value_chr.compare(std::string()),     0) ;
        CHECK_EQUAL(value_feature.compare(std::string()), 0) ;
        CHECK_EQUAL(value_strand.compare(std::string()),  0) ;
        CHECK_EQUAL(value_pos,                            0) ;
        CHECK_EQUAL(value_count,                          0) ;
    }

    // test string constructor
    TEST(TestSGA_element_string_constructor_1)
    {   // get field values
        std::string value_chr     = sga1.chromosome ;
        std::string value_feature = sga1.feature ;
        std::string value_strand  = sga1.strand ;
        int          value_pos    = sga1.position ;
        int          value_count  = sga1.count ;
        // check equality
        CHECK_EQUAL(value_chr.compare(std::string("chr2")),      0) ;
        CHECK_EQUAL(value_feature.compare(std::string("MYC_P")), 0) ;
        CHECK_EQUAL(value_strand.compare(std::string("+")),      0) ;
        CHECK_EQUAL(value_pos,                                1000) ;
        CHECK_EQUAL(value_count,                                 1) ;
    }

    // test string constructor
    TEST(TestSGA_element_string_constructor_2)
    {   // get field values
        std::string value_chr     = sga2.chromosome ;
        std::string value_feature = sga2.feature ;
        std::string value_strand  = sga2.strand ;
        int          value_pos     = sga2.position ;
        int          value_count   = sga2.count ;
        // check equality
        CHECK_EQUAL(value_chr.compare(std::string("chr1")),       0) ;
        CHECK_EQUAL(value_feature.compare(std::string("CTCF_P")), 0) ;
        CHECK_EQUAL(value_strand.compare(std::string("-")),       0) ;
        CHECK_EQUAL(value_pos,                                20000) ;
        CHECK_EQUAL(value_count,                                  1) ;
    }

    // test value constructor
    TEST(TestSGA_element_value_constructor)
    {   // get field values
        std::string value_chr     = sga4.chromosome ;
        std::string value_feature = sga4.feature ;
        std::string value_strand  = sga4.strand ;
        int          value_pos     = sga4.position ;
        int          value_count   = sga4.count ;
        // check equality
        CHECK_EQUAL(value_chr.compare(chr),         0) ;
        CHECK_EQUAL(value_feature.compare(feature), 0) ;
        CHECK_EQUAL(value_strand.compare(strand),   0) ;
        CHECK_EQUAL(value_pos,                    pos) ;
        CHECK_EQUAL(value_count,                count) ;
    }

    // test == operator
    TEST(TestSGA_element_equality_operator)
    {   CHECK_EQUAL(sga1 == sga2, false) ;
        CHECK_EQUAL(sga2 == sga3, false) ;
        CHECK_EQUAL(sga2 == sga4, true) ;
        CHECK_EQUAL(sga4 == sga4, true) ;
    }

    // test != operator
    TEST(TestSGA_element_inequality_operator)
    {   CHECK_EQUAL(sga1 != sga2, true) ;
        CHECK_EQUAL(sga2 != sga3, true) ;
        CHECK_EQUAL(sga2 != sga4, false) ;
        CHECK_EQUAL(sga4 != sga4, false) ;
    }

    // test = operator
    TEST(TestSGA_element_assign_operator)
    {   SGA_element sga_cp = sga1 ;
        CHECK_EQUAL(true, sga_cp == sga1) ;
        sga_cp = sga2 ;
        CHECK_EQUAL(true, sga_cp == sga2) ;
    }

    // test get_distance
    TEST(TestSGA_element_get_distance_1)
    {   int error_byte = 0 ;
        int dist       = 0 ;

        // this distance makes no sense (because sga1 and sga2 on diff chromosomes) and error byte should be 1
        dist = sga1.get_distance(sga2, error_byte) ;
        CHECK_EQUAL(dist,           0) ;
        CHECK_EQUAL(error_byte,     1) ;

        // this distance makes sense
        dist = sga2.get_distance(sga3, error_byte) ;
        CHECK_EQUAL(dist,       5000) ;
        CHECK_EQUAL(error_byte,    0) ;
        dist = sga3.get_distance(sga2, error_byte) ;
        CHECK_EQUAL(dist,      -5000) ;
        CHECK_EQUAL(error_byte,    0) ;
    }
}

// SGAFileReader unittets
SUITE(SGAFileReader_testsuit)
{
    std::string f_ok             = "src/Unittests/data/sga/file_ok.sga" ;
    std::string f_empty          = "src/Unittests/data/sga/file_empty.sga" ;
    std::string f_empty_line     = "src/Unittests/data/sga/file_empty_line.sga" ;
    std::string f_position_bad1  = "src/Unittests/data/sga/file_position_bad1.sga" ;
    std::string f_position_bad2  = "src/Unittests/data/sga/file_position_bad2.sga" ;
    std::string f_count_bad1     = "src/Unittests/data/sga/file_count_bad1.sga" ;
    std::string f_count_bad2     = "src/Unittests/data/sga/file_count_bad2.sga" ;
    std::string f_strand_bad     = "src/Unittests/data/sga/file_strand_bad.sga" ;
    std::string f_missing_field1 = "src/Unittests/data/sga/file_missing_field1.sga" ;
    std::string f_missing_field2 = "src/Unittests/data/sga/file_missing_field2.sga" ;
    std::string f_missing_field3 = "src/Unittests/data/sga/file_missing_field3.sga" ;
    std::string f_missing_field4 = "src/Unittests/data/sga/file_missing_field4.sga" ;
    std::string f_missing_field5 = "src/Unittests/data/sga/file_missing_field5.sga" ;
    std::string f_wrong_address  = "src/Unittests/data/sga/no_such_file.sga" ;

    SGAFileReader sga_reader ;

    TEST(message)
    {   std::cout << "Starting SGAFileReader tests..." << std::endl ; }

    // this test read a fine SGA file
    TEST(SGA_OK)
    {   sga_reader.set_file(f_ok) ;
        SGA_element sga1("chr1", "Test", 1, "+", 1) ;
        SGA_element sga2("chr1", "Test", 2, "-", 2) ;
        SGA_element sga3("chr1", "Test", 3, "0", 1) ;

        SGA_element* sga = nullptr ;
        for(size_t i=0; i<3; i++)
        {   sga = sga_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, sga1 == *sga) ; break ;
                case 1 : CHECK_EQUAL(true, sga2 == *sga) ; break ;
                case 2 : CHECK_EQUAL(true, sga3 == *sga) ; break ;
            }
            delete sga ;
        }
    }

    // this test reads an empty SGA file
    TEST(SGA_empty)
    {   sga_reader.set_file(f_empty) ;
        SGA_element* sga = nullptr ;

        // an null ptr is supposed to be returned immediatley as the file is empty
        size_t i = 0 ;
        while((sga = sga_reader.get_next()))
        {   i++ ; }

        CHECK_EQUAL(0,    i) ;
        CHECK_EQUAL(true, sga==nullptr) ;
    }

    // this test reads a file containing an 2nd empty line, this should raise a invalid_argument exception
    TEST(SGA_empty_line)
    {   SGA_element* sga = nullptr ;
        sga_reader.set_file(f_empty_line) ;

        // 1st line is OK
        SGA_element sga1("chr1", "Test", 1, "+", 1) ;
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        // it seems the invalide_argument exception is thrown but not caught here...
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
    }

    // this tets reads files containing a position field <=0 which should raise a invalid_argument exception
    TEST(SGA_position_bad)
    {   // 1st file contains a postion equal to 0
        SGA_element* sga = nullptr ;
        sga_reader.set_file(f_position_bad1) ;
        // 1st line is OK
        SGA_element sga1("chr1", "Test", 1, "+", 1) ;
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;


        //---------------------------------------------------------------

        // 2nd file contains a postion equal to -2
        sga = nullptr ;
        sga_reader.set_file(f_position_bad2) ;
        // 1st line is OK
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
    }

    // this test reads files containing a count field <=0 which should raise a invalid_argument exception
    TEST(SGA_count_bad)
    {   // 1st file contains a postion equal to 0
        SGA_element* sga = nullptr ;
        sga_reader.set_file(f_count_bad1) ;
        // 1st line is OK
        SGA_element sga1("chr1", "Test", 1, "+", 1) ;
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;


        //---------------------------------------------------------------

        // 2nd file contains a postion equal to -1
        sga = nullptr ;
        sga_reader.set_file(f_count_bad2) ;
        // 1st line is OK
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
    }

    // this test reads a file containing a strand field with a value  different than -/+/0 which should raise a invalid_argument exception
    TEST(SGA_strand_bad)
    {   SGA_element* sga = nullptr ;
        sga_reader.set_file(f_count_bad1) ;
        // 1st line is OK
        SGA_element sga1("chr1", "Test", 1, "+", 1) ;
        sga = sga_reader.get_next() ;
        CHECK_EQUAL(true, sga1 == *sga) ;
        delete sga ;
        // 2nd line reading should trigger an exception
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
    }

    // this test reads 5 files containing each a line which misses a field
    TEST(SGA_missing_field)
    {   SGA_element* sga = nullptr ;

        // 1st file, 1st line misses the chromosome field
        sga_reader.set_file(f_missing_field1) ;
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
        if(sga) { delete sga ; }
        //-----------------------------------------------------------------
        // 2nd file, 1st line misses the feature field
        sga_reader.set_file(f_missing_field2) ;
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
        if(sga) { delete sga ; }
        //-----------------------------------------------------------------

        // 3rd file, 1st line misses the position field
        sga_reader.set_file(f_missing_field3) ;
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
        if(sga) { delete sga ; }
        //-----------------------------------------------------------------

        // 4th file, the 1st line misses the strand field
        sga_reader.set_file(f_missing_field4) ;
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
        if(sga) { delete sga ; }
        //-----------------------------------------------------------------

        // 5th file, the 1st line misses the count field
        sga_reader.set_file(f_missing_field5) ;
        CHECK_THROW(sga = sga_reader.get_next(), std::invalid_argument) ;
        if(sga) { delete sga ; }
    }

    // this test attempt to read a file which does not exist which should raise a runtime_error exception
    TEST(SGA_wrong_address)
    {   // check the set_file method
        CHECK_THROW(sga_reader.set_file(f_wrong_address), std::runtime_error) ;
        // check constructor
        CHECK_THROW(SGAFileReader sga_reader2(f_wrong_address), std::runtime_error) ;
    }
}

