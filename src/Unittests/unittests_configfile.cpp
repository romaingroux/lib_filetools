#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "ConfigFile/ConfigFileReader.hpp"



// ConfigFileReader test suite
SUITE(ConfigFileReader_testsuit)
{   std::string f_null           = "src/Unittests/data/config/config_null.ini" ;
    std::string f_init           = "src/Unittests/data/config/config_init.ini" ;
    std::string f_ok             = "src/Unittests/data/config/config_ok.ini" ;
    std::string f_ok2            = "src/Unittests/data/config/config_ok2.ini" ;
    std::string f_same_sections  = "src/Unittests/data/config/config_same_sections.ini" ;
    std::string f_same_options   = "src/Unittests/data/config/config_same_options.ini" ;
    std::string f_empty          = "src/Unittests/data/config/config_empty.ini" ;
    std::string f_option_out     = "src/Unittests/data/config/config_option_out.ini" ;
    std::string f_wrong_address  = "src/Unittests/data/config/does_not_exist.ini" ;

    TEST(message)
    {   std::cout << "Starting ConfigFileReader tests..." << std::endl ; }

    // tests consrtruction, has_option() set_file()
    TEST(Config_OK)
    {
        // all values inside this file are "thisisinit"
        ConfigFileReader reader(f_init) ;
        CHECK_EQUAL(false, reader.has_section("section_imaginary")) ;
        CHECK_EQUAL(true, reader.has_section("section_str")) ;
        CHECK_EQUAL(true, reader.has_option("section_str",    "opt_str1")) ; CHECK_EQUAL("thisisinit", reader.getString("section_str", "opt_str1")) ;
        CHECK_EQUAL(true, reader.has_section("section_bool")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool",   "bool1")) ;    CHECK_EQUAL("thisisinit", reader.getString("section_bool", "bool1")) ;
        CHECK_EQUAL(true, reader.has_section("section_int")) ;
        CHECK_EQUAL(true, reader.has_option("section_int",    "int1")) ;     CHECK_EQUAL("thisisinit", reader.getString("section_int", "int1")) ;
        CHECK_EQUAL(true, reader.has_section("section_double")) ;
        CHECK_EQUAL(true, reader.has_option("section_double", "dbl1")) ;     CHECK_EQUAL("thisisinit", reader.getString("section_double", "dbl1")) ;
        CHECK_EQUAL(true, reader.has_section("section_thisisinit")) ;
        CHECK_EQUAL(true, reader.has_option("section_thisisinit", "val")) ;  CHECK_EQUAL("thisisinit", reader.getString("section_thisisinit", "val")) ;


        // the following test the set_file(), all sections and some options from the previous file to this one are the same but the value changes
        // check if they indeed change
        reader.set_file(f_ok) ;

        // sections which should not exist
        CHECK_EQUAL(false, reader.has_section("section_imaginary")) ;
        CHECK_EQUAL(false, reader.has_section("section_thisisinit")) ;

        // get all string values as strings
        CHECK_EQUAL(true, reader.has_section("section_str")) ;
        CHECK_EQUAL(true, reader.has_option("section_str", "opt_str1")) ;  CHECK_EQUAL("/etc/fstab", reader.getString("section_str", "opt_str1")) ;
        CHECK_EQUAL(true, reader.has_option("section_str", "opt_str2")) ;  CHECK_EQUAL("/etc/mtab", reader.getString("section_str", "opt_str2")) ;
        CHECK_EQUAL(true, reader.has_option("section_str", "opt_str3")) ;  CHECK_EQUAL("~/../data/", reader.getString("section_str", "opt_str3")) ;
        CHECK_EQUAL(false, reader.has_option("section_str", "opt_str4")) ; CHECK_THROW(reader.getString("section_str", "opt_str4"), std::runtime_error) ;

        // get all bool values as bool
        CHECK_EQUAL(true, reader.has_section("section_bool")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool1"))  ; CHECK_EQUAL(true,  reader.getBool("section_bool", "bool1")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool2"))  ; CHECK_EQUAL(false, reader.getBool("section_bool", "bool2")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool3"))  ; CHECK_EQUAL(true,  reader.getBool("section_bool", "bool3")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool4"))  ; CHECK_EQUAL(false, reader.getBool("section_bool", "bool4")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool5"))  ; CHECK_EQUAL(true,  reader.getBool("section_bool", "bool5")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool6"))  ; CHECK_EQUAL(false, reader.getBool("section_bool", "bool6")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool7"))  ; CHECK_EQUAL(true,  reader.getBool("section_bool", "bool7")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool8"))  ; CHECK_EQUAL(false, reader.getBool("section_bool", "bool8")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool9"))  ; CHECK_EQUAL(true,  reader.getBool("section_bool", "bool9")) ;
        CHECK_EQUAL(true, reader.has_option("section_bool", "bool10")) ; CHECK_EQUAL(false, reader.getBool("section_bool", "bool10")) ;
        CHECK_EQUAL(false, reader.has_option("section_bool", "bool11")) ; CHECK_THROW(reader.getString("section_bool", "bool11"), std::runtime_error) ;
        // get all int values as int
        CHECK_EQUAL(true, reader.has_section("section_int")) ;
        CHECK_EQUAL(true, reader.has_option("section_int", "int1")) ; CHECK_EQUAL(-99999, reader.getInt("section_int", "int1")) ;
        CHECK_EQUAL(true, reader.has_option("section_int", "int2")) ; CHECK_EQUAL(     0, reader.getInt("section_int", "int2")) ;
        CHECK_EQUAL(true, reader.has_option("section_int", "int3")) ; CHECK_EQUAL( 99999, reader.getInt("section_int", "int3")) ;
        CHECK_EQUAL(false, reader.has_option("section_int", "int4")) ; CHECK_THROW(reader.getInt("section_int", "int4"), std::runtime_error) ;
        // get all double values as doubles
        CHECK_EQUAL(true, reader.has_section("section_double")) ;
        CHECK_EQUAL(true, reader.has_section("section_double")) ;
        CHECK_EQUAL(true, reader.has_option("section_double", "dbl1")) ; CHECK_EQUAL(-12345.123567, reader.getDouble("section_double", "dbl1")) ;
        CHECK_EQUAL(true, reader.has_option("section_double", "dbl2")) ; CHECK_EQUAL(      .123567, reader.getDouble("section_double", "dbl2")) ;
        CHECK_EQUAL(true, reader.has_option("section_double", "dbl3")) ; CHECK_EQUAL( 12345.123567, reader.getDouble("section_double", "dbl3")) ;
        CHECK_EQUAL(false, reader.has_option("section_double", "dbl4")) ; CHECK_THROW(reader.getInt("section_double", "dbl4"), std::runtime_error) ;

        // get bool as strings
        CHECK_EQUAL("On",    reader.getString("section_bool", "bool1")) ;
        CHECK_EQUAL("Off",   reader.getString("section_bool", "bool2")) ;
        CHECK_EQUAL("on",    reader.getString("section_bool", "bool3")) ;
        CHECK_EQUAL("off",   reader.getString("section_bool", "bool4")) ;
        CHECK_EQUAL("1",     reader.getString("section_bool", "bool5")) ;
        CHECK_EQUAL("0",     reader.getString("section_bool", "bool6")) ;
        CHECK_EQUAL("True",  reader.getString("section_bool", "bool7")) ;
        CHECK_EQUAL("False", reader.getString("section_bool", "bool8")) ;
        CHECK_EQUAL("true",  reader.getString("section_bool", "bool9")) ;
        CHECK_EQUAL("false", reader.getString("section_bool", "bool10")) ;
        // get ints as double
        CHECK_EQUAL(-99999.0, reader.getDouble("section_int", "int1")) ;
        CHECK_EQUAL(      .0, reader.getDouble("section_int", "int2")) ;
        CHECK_EQUAL( 99999.0, reader.getDouble("section_int", "int3")) ;
        // get ints as strings
        CHECK_EQUAL("-99999", reader.getString("section_int", "int1")) ;
        CHECK_EQUAL(     "0", reader.getString("section_int", "int2")) ;
        CHECK_EQUAL( "99999", reader.getString("section_int", "int3")) ;
        // get doubles as ints
        CHECK_EQUAL(-12345, reader.getInt("section_double", "dbl1")) ;
        CHECK_EQUAL(     0, reader.getInt("section_double", "dbl2")) ;
        CHECK_EQUAL( 12345, reader.getInt("section_double", "dbl3")) ;
        // get doubles as strings
        CHECK_EQUAL("-12345.123567", reader.getString("section_double", "dbl1")) ;
        CHECK_EQUAL(     "0.123567", reader.getString("section_double", "dbl2")) ;
        CHECK_EQUAL( "12345.123567", reader.getString("section_double", "dbl3")) ;

        // don't test get anything as bool, will always be true except if a value is null (0, empty string)
    }

    // this file contains repeated options but in different sections (allowed)
    TEST(Config_OK2)
    {   ConfigFileReader reader(f_ok2) ;
        CHECK_EQUAL(true, reader.has_section("section1")) ;
        CHECK_EQUAL(true, reader.has_option("section1", "opt1")) ;  CHECK_EQUAL("1", reader.getString("section1", "opt1")) ;
        CHECK_EQUAL(true, reader.has_option("section1", "opt2")) ;  CHECK_EQUAL("1", reader.getString("section1", "opt2")) ;
        CHECK_EQUAL(true, reader.has_section("section2")) ;
        CHECK_EQUAL(true, reader.has_option("section2", "opt1")) ;  CHECK_EQUAL("2", reader.getString("section2", "opt1")) ;
        CHECK_EQUAL(true, reader.has_option("section2", "opt2")) ;  CHECK_EQUAL("2", reader.getString("section2", "opt2")) ;
        CHECK_EQUAL(false, reader.has_section("section3")) ;
    }

    // this file contains two sections with a same name (not allowed)
    TEST(Config_same_sections)
    {   CHECK_THROW(ConfigFileReader reader(f_same_sections), std::runtime_error) ; }

    // this file contains a sections with a twice the same option (not allowed)
    TEST(Config_same_options)
    {   CHECK_THROW(ConfigFileReader reader(f_same_options), std::runtime_error) ; }

    // this file is empty
    TEST(Config_empty)
    {   ConfigFileReader reader(f_empty) ;
        auto opts = reader.get_options() ;
        CHECK_EQUAL(0, opts.size()) ;
    }

    // this file contains sections and options but not values
    TEST(Config_null_values)
    {   ConfigFileReader reader(f_null) ;
        std::string no_value = ConfigFileReader::novalue ;
        CHECK_EQUAL(true, reader.has_section("section1")) ;
        CHECK_EQUAL(true, reader.has_option("section1", "opt1")) ;  CHECK_EQUAL(no_value, reader.getString("section1", "opt1")) ;
        CHECK_EQUAL(true, reader.has_option("section1", "opt2")) ;  CHECK_EQUAL(no_value, reader.getString("section1", "opt2")) ;
        CHECK_EQUAL(true, reader.has_section("section2")) ;
        CHECK_EQUAL(true, reader.has_option("section2", "opt1")) ;  CHECK_EQUAL(no_value, reader.getString("section2", "opt1")) ;
        CHECK_EQUAL(true, reader.has_option("section2", "opt2")) ;  CHECK_EQUAL(no_value, reader.getString("section2", "opt2")) ;
        CHECK_EQUAL(false, reader.has_section("section3")) ;
    }

    // this file contains one option before the 1st section
    TEST(Config_option_out)
    {   CHECK_THROW(ConfigFileReader reader(f_option_out), std::runtime_error) ; }

    // this file does not exists
    TEST(Config_wrong_address)
    {   CHECK_THROW(ConfigFileReader reader(f_wrong_address), std::runtime_error) ; }
}
