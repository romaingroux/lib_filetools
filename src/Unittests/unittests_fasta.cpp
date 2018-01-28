#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "FASTAFile/FASTA_element.hpp"
#include "FASTAFile/FASTAFileReader.hpp"


// FASTA_element unittests
SUITE(FASTA_element_testsuit)
{
    TEST(message)
    {   std::cout << "Starting FASTA_element tests..." << std::endl ; }

    // tests the constructor and ==, != and = operators
    TEST(FASTA_element_new)
    {   std::string header = "header" ;
        std::string sequence  = "ATCCTCA" ;  size_t len = sequence.size() ;
        std::string sequence0 = "ATCCTCA" ;
        std::string sequence1 = "@ATCCTCA" ;

        // check constructor
        // 0-based
        FASTA_element fasta_0based(header, sequence, false) ;
        CHECK_EQUAL(header ,fasta_0based.header) ;
        CHECK_EQUAL(sequence0 ,fasta_0based.sequence) ;
        CHECK_EQUAL(len, fasta_0based.sequence_length) ;
        CHECK_EQUAL(false, fasta_0based.sequence_one_based) ;
        // 1-based
        FASTA_element fasta_1based(header, sequence, true) ;
        CHECK_EQUAL(header ,fasta_1based.header) ;
        CHECK_EQUAL(sequence1 ,fasta_1based.sequence) ;
        CHECK_EQUAL(len, fasta_1based.sequence_length) ;
        CHECK_EQUAL(true, fasta_1based.sequence_one_based) ;

        // check equality operator
        FASTA_element fasta_0based_cp ;
        fasta_0based_cp.sequence = sequence0 ;
        fasta_0based_cp.header   = header ;
        fasta_0based_cp.sequence_one_based = false ;
        fasta_0based_cp.sequence_length = len ;
        CHECK_EQUAL(true,  fasta_0based == fasta_0based) ;
        CHECK_EQUAL(true,  fasta_0based == fasta_0based_cp) ;
        CHECK_EQUAL(false, fasta_0based == fasta_1based) ;

        FASTA_element fasta_1based_cp ;
        fasta_1based_cp.sequence = sequence1 ;
        fasta_1based_cp.header   = header ;
        fasta_1based_cp.sequence_one_based = true ;
        fasta_1based_cp.sequence_length = len ;
        CHECK_EQUAL(true,  fasta_1based == fasta_1based_cp) ;
        CHECK_EQUAL(false, fasta_0based == fasta_1based_cp) ;

        // check inequality operator
        CHECK_EQUAL(false, fasta_1based != fasta_1based) ;
        CHECK_EQUAL(false, fasta_1based != fasta_1based_cp) ;
        CHECK_EQUAL(true,  fasta_0based != fasta_1based_cp) ;

        // check assignment operator
        FASTA_element fasta_new = fasta_0based ;
        CHECK_EQUAL(true, fasta_new == fasta_0based) ;
        CHECK_EQUAL(true, fasta_new == fasta_0based_cp) ;
        CHECK_EQUAL(false, fasta_new == fasta_1based) ;
    }
}

// FASTAFileReader unittests
SUITE(FASTAFileReader_testsuit)
{
    std::string f_ok              = "src/Unittests/data/fasta/file_ok.fasta" ;
    std::string f_empty           = "src/Unittests/data/fasta/file_empty.fasta" ;
    std::string f_empty_entry1    = "src/Unittests/data/fasta/file_empty_entry1.fasta" ;
    std::string f_empty_entry2    = "src/Unittests/data/fasta/file_empty_entry2.fasta" ;
    std::string f_empty_entry3    = "src/Unittests/data/fasta/file_empty_entry3.fasta" ;
    std::string f_entry_emptyline = "src/Unittests/data/fasta/file_entry_with_empty_line.fasta" ;
    std::string f_same_header     = "src/Unittests/data/fasta/file_two_same_header.fasta" ;
    std::string f_wrong_address   = "src/Unittests/data/fasta/no_such_file.fasta" ;

    TEST(message)
    {   std::cout << "Starting FASTAFileReader tests..." << std::endl ; }

    // this test reads a FASTA file which is ok
    TEST(FASTA_ok)
    {   FASTAFileReader fasta_reader(f_ok, false, 500) ;
        FASTA_element* fasta ;

        FASTA_element fasta1 ;
        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_one_based = false ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        FASTA_element fasta2 ;
        fasta2.header   = ">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla" ;
        fasta2.sequence = "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA"
                          "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA" ;
        fasta2.sequence_one_based = false ;
        fasta2.sequence_length = fasta2.sequence.length() ;
        FASTA_element fasta3 ;
        fasta3.header   = ">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla" ;
        fasta3.sequence = "TATTTCTCCTTCACTTCTTTGGTGGGAGTGAGATGGTGATCCTCATAGCCATGGGCTTTGACAGATATAT" ;
        fasta3.sequence_one_based = false ;
        fasta3.sequence_length = fasta3.sequence.length() ;
        FASTA_element fasta4 ;
        fasta4.header   = ">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla" ;
        fasta4.sequence = "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC"
                          "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC" ;
        fasta4.sequence_one_based = false ;
        fasta4.sequence_length = fasta4.sequence.length() ;


        // 1) read the whole file sequentially
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof()) ; break ;
            }
            delete fasta ; fasta = nullptr ;
        }


        // 2) check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ; fasta = nullptr ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ; fasta = nullptr ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ; fasta = nullptr ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ; fasta = nullptr ;


        // 3) check whether interspersed called to get() and get_next() alters file traversal
        // start at file beginning
        fasta_reader.seekg(0, std::ios::beg) ;
        // get_next() -> 1st entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; delete fasta ; fasta = nullptr ;
        // get specifically last entry
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ; CHECK_EQUAL(true, fasta_reader.good()) ;
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta4) ; delete fasta ; fasta = nullptr ;

        // get_next() -> 2nd entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta2); CHECK_EQUAL(true, fasta_reader.good()) ; delete fasta ;
        // get specifically 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta3) ; delete fasta ; fasta = nullptr ;

        // get_next() -> 3rd entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; delete fasta ; fasta = nullptr ;
        // get specifically 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta2) ; delete fasta ; fasta = nullptr ;

        // get_next() -> 4th entry (and reaches eof)
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof()) ; delete fasta ; fasta = nullptr ;
        // get specifically 1st entry (reset the stream state, put back file pointer to end of file, eofbit is not set back)
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.fail()) ; delete fasta ; fasta = nullptr ;

        // extra call to get_next after file end
        // now file has been read to the end (but is not eof, a next call to get_next will return nothing
        // the following call to get_next() should return nullptr because the end of the file has been reached
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta==nullptr) ; CHECK_EQUAL(true, fasta_reader.fail()) ;
    }

    // this test reads an empty file, nothing should be read
    TEST(FASTA_empty)
    {
        FASTAFileReader fasta_reader(f_empty, false, 500) ;
        FASTA_element* fasta ;

        size_t i=0 ;
        while((fasta = fasta_reader.get_next()))
        {   delete fasta ; fasta = nullptr ;
            i++ ;
        }
        CHECK_EQUAL(0, i) ;
    }


    // this test reads files containing an empty entry (i.g. a header but no sequence after)
    TEST(FASTA_empty_entry)
    {   FASTA_element* fasta ;
        FASTAFileReader fasta_reader ;
        FASTA_element fasta1 ;
        FASTA_element fasta2 ;
        FASTA_element fasta3 ;
        FASTA_element fasta4 ;

        // file 1, contains a 2nd entry which has no sequence
        fasta_reader.set_file(f_empty_entry1, false, 500) ;
        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        fasta1.sequence_one_based = false ;

        fasta2.header   = ">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla" ;
        fasta2.sequence = "" ;
        fasta2.sequence_length = fasta2.sequence.length() ;
        fasta2.sequence_one_based = false ;

        fasta3.header   = ">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla" ;
        fasta3.sequence = "TATTTCTCCTTCACTTCTTTGGTGGGAGTGAGATGGTGATCCTCATAGCCATGGGCTTTGACAGATATAT" ;
        fasta3.sequence_length = fasta3.sequence.length() ;
        fasta3.sequence_one_based = false ;

        fasta4.header   = ">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla" ;
        fasta4.sequence = "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC"
                          "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC" ;
        fasta4.sequence_length = fasta4.sequence.length() ;
        fasta4.sequence_one_based = false ;

        // read the whole file
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof()) ; break ;
            }
            delete fasta ; fasta = nullptr ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ; fasta = nullptr ;
        // find 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ; fasta = nullptr ;
        // find 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ; fasta = nullptr ;
        // find 1st entry
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ; fasta = nullptr ;


        //------------------------------------------------------------------------
        // file 2
        // file 2, contains a 2nd entry has no sequence but a '\n'
        fasta_reader.set_file(f_empty_entry2, false, 500) ;
        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        fasta1.sequence_one_based = false ;

        fasta2.header   = ">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla" ;
        fasta2.sequence = "" ;
        fasta2.sequence_length = fasta2.sequence.length() ;
        fasta2.sequence_one_based = false ;

        fasta3.header   = ">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla" ;
        fasta3.sequence = "TATTTCTCCTTCACTTCTTTGGTGGGAGTGAGATGGTGATCCTCATAGCCATGGGCTTTGACAGATATAT" ;
        fasta3.sequence_length = fasta3.sequence.length() ;
        fasta3.sequence_one_based = false ;

        fasta4.header   = ">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla" ;
        fasta4.sequence = "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC"
                          "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC" ;
        fasta4.sequence_length = fasta4.sequence.length() ;
        fasta4.sequence_one_based = false ;

        // read the whole file
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof()) ; break ;
            }
            delete fasta ; fasta = nullptr ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ; fasta = nullptr ;
        // find 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ; fasta = nullptr ;
        // find 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ; fasta = nullptr ;
        // find 1st entry
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ; fasta = nullptr ;

        //------------------------------------------------------------------------
        // file 3
        // file 3 contains a last entry which has no sequence
        fasta_reader.set_file(f_empty_entry3, false, 500) ;
                // FASTAFileReader fasta_reader2(f_empty_entry3, false, 500) ;
        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        fasta1.sequence_one_based = false ;

        fasta2.header   = ">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla" ;
        fasta2.sequence = "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA"
                          "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA" ;
        fasta2.sequence_length = fasta2.sequence.length() ;
        fasta2.sequence_one_based = false ;

        fasta3.header   = ">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla" ;
        fasta3.sequence = "TATTTCTCCTTCACTTCTTTGGTGGGAGTGAGATGGTGATCCTCATAGCCATGGGCTTTGACAGATATAT" ;
        fasta3.sequence_length = fasta3.sequence.length() ;
        fasta3.sequence_one_based = false ;

        fasta4.header   = ">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla" ;
        fasta4.sequence = "" ;
        fasta4.sequence_length = fasta4.sequence.length() ;
        fasta4.sequence_one_based = false ;

        // read the whole file
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof())  ; break ;
            }
            delete fasta ; fasta = nullptr ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ; fasta = nullptr ;

        // find 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ; fasta = nullptr ;
        // find 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ; fasta = nullptr ;
        // find 1st entry
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ; fasta = nullptr ;
    }

    // this test reads a file which 2nd entry contains an empty line within the sequence
    TEST(FASTA_entry_with_empty_line)
    {
        FASTA_element* fasta ;
        FASTAFileReader fasta_reader ;
        FASTA_element fasta1 ;
        FASTA_element fasta2 ;
        FASTA_element fasta3 ;
        FASTA_element fasta4 ;

        fasta_reader.set_file(f_entry_emptyline, false, 500) ;

        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        fasta1.sequence_one_based = false ;

        fasta2.header   = ">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla" ;
        fasta2.sequence = "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA"
                          "GCCCCCAAGATGATTACTGACTTTTTCAGCCAGCGCAAAGTCATCTCTTTCAAGGGCTGCCTTGTTCAGA" ;
        fasta2.sequence_length = fasta2.sequence.length() ;
        fasta2.sequence_one_based = false ;

        fasta3.header   = ">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla" ;
        fasta3.sequence = "TATTTCTCCTTCACTTCTTTGGTGGGAGTGAGATGGTGATCCTCATAGCCATGGGCTTTGACAGATATAT" ;
        fasta3.sequence_length = fasta3.sequence.length() ;
        fasta3.sequence_one_based = false ;

        fasta4.header   = ">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla" ;
        fasta4.sequence = "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC"
                          "AGCAATATGCAAGCCCCTACACTACACTACAATTATGTGTGGCAACGCATGTGTCGGCATTATGGCTGTC" ;
        fasta4.sequence_length = fasta4.sequence.length() ;
        fasta4.sequence_one_based = false ;

        // read the whole file
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; CHECK_EQUAL(true, fasta_reader.good()) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; CHECK_EQUAL(true, fasta_reader.eof()) ; break ;
            }
            delete fasta ; fasta = nullptr ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry

        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ; fasta = nullptr ;
        // find 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ; fasta = nullptr ;
        // find 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ; fasta = nullptr ;
        // find 1st entry
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ; fasta = nullptr ;
    }

    // this test reads a file which contain 2 identical header. For sequential reading, this
    // is not supposed to be a problem but for searching entries using the header it will
    // always return the first match
    TEST(FASTA_two_identical_header)
    {   // a header is present twice in the file, should be detected and an error throw
        FASTAFileReader fasta_reader ;
        CHECK_THROW(fasta_reader.set_file(f_same_header, false, 500), std::runtime_error) ;
    }

    // this test attempt to read a file which does not exist which should raise a runtime_error exception
    TEST(FASTA_wrong_address)
    {   FASTAFileReader fasta_reader ;
        // check the set_file method
        CHECK_THROW(fasta_reader.set_file(f_wrong_address, true, 500), std::runtime_error) ;
        // check constructor
        CHECK_THROW(FASTAFileReader fasta_reader2(f_wrong_address, true, 500), std::runtime_error) ;
    }
}
