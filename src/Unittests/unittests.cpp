#include <UnitTest++/UnitTest++.h>
#include <iostream>
#include <string>
#include <stdexcept>

#include "FASTAFile/FASTA_element.hpp"
#include "FASTAFile/FASTAFileReader.hpp"

#include "SGAFile/SGA_element.hpp"
#include "SGAFile/SGAFileReader.hpp"



// FASTA_element unittests
SUITE(FASTA_element_testsuit)
{
    // this test tests the values of the fields at instanciation and after setting them
    TEST(FASTA_element)
    {   FASTA_element fasta ;
        std::string header = "header" ;
        std::string sequence = "ATCCTCA" ;
        size_t sequence_len = sequence.length() ;

        // empty instance
        CHECK_EQUAL(true, fasta.header   == "") ;
        CHECK_EQUAL(true, fasta.sequence == "") ;
        CHECK_EQUAL(true, fasta.sequence_length == 0) ;
        CHECK_EQUAL(true, fasta.sequence_one_based == false) ;

        // change fields and check their values
        fasta.header   = header ;
        fasta.sequence = sequence ;
        CHECK_EQUAL(true, fasta.header   == header) ;
        CHECK_EQUAL(true, fasta.sequence == sequence) ;
        CHECK_EQUAL(true, fasta.sequence_length == 0) ;
        CHECK_EQUAL(true, fasta.sequence_one_based == false) ;

        // change fields and check values
        fasta.sequence_length = fasta.sequence.length() ;
        fasta.sequence_one_based = true ;
        CHECK_EQUAL(true, fasta.sequence_length == sequence_len) ;
        CHECK_EQUAL(true, fasta.sequence_one_based == true) ;

        FASTA_element fasta2 ;
        fasta2.header = header ;
        fasta2.sequence = sequence ;
        fasta2.sequence_length = sequence_len ;
        fasta2.sequence_one_based = true ;
        CHECK_EQUAL(true, fasta == fasta2) ;
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

        // read the whole file
        for(size_t i=0; i<4; i++)
        {   fasta = fasta_reader.get_next() ;
            switch(i)
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }
        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta4) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta3) ;
        delete fasta ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta2) ;
        delete fasta ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001") ;
        CHECK_EQUAL(true, fasta==nullptr) ;
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, *fasta == fasta1) ;
        delete fasta ;

        // check whether interspersed called to get() and get_next() alters file traversal
        // start at fikle beginning
        fasta_reader.seekg(0, std::ios::beg) ;
        // get_next() -> 1st entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta1) ; delete fasta ;
        // get specifically last entry
        fasta = fasta_reader.get(">chr|NC_000004|NC_000004.1 Chromosome 4; [Homo sapiens] bla_bla_bla") ; 
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta4) ; delete fasta ;
        
        // get_next() -> 2nd entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta2); delete fasta ;
        // get specifically 3rd entry
        fasta = fasta_reader.get(">chr|NC_000003|NC_000003.1 Chromosome 3; [Homo sapiens] bla_bla_bla") ; 
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta3) ; delete fasta ;

        // get_next() -> 3rd entry
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta3) ; delete fasta ;
        // get specifically 2nd entry
        fasta = fasta_reader.get(">chr|NC_000002|NC_000002.1 Chromosome 2; [Homo sapiens] bla_bla_bla") ; 
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta2) ; delete fasta ;

				// get_next() -> 4th entry (and reaches eof)
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta4) ; delete fasta ;
        // get specifically 1st entry (reset the stream state but should put back file pointer to end of file, eofbit is not set back)
        fasta = fasta_reader.get(">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla") ;
        CHECK_EQUAL(true, fasta!=nullptr) ; CHECK_EQUAL(true, *fasta == fasta1) ; delete fasta ;
        // now file has been read to the end (but is not eof, a next call to get_next will return nothing but failbit will be set (try reading passed EOF)
        // but not eofbit (EOF will not be seen, we are already after))
        // the following call to get_next() should return nullptr because the end of the file has been reached and set stream state to eof.
        fasta = fasta_reader.get_next() ; CHECK_EQUAL(true, fasta==nullptr) ;
    }

    // this test reads an empty file, nothing should be read
    TEST(FASTA_empty)
    {   /*
        FASTAFileReader fasta_reader(f_empty, false, 500) ;
        FASTA_element* fasta ;

        size_t i=0 ;
        while((fasta = fasta_reader.get_next()))
        {   delete fasta ;
            i++ ;
        }
        CHECK_EQUAL(0, i) ;
        */
    }


    // this test reads files containing an empty entry (i.g. a header but no sequence after)
    TEST(FASTA_empty_entry)
    {   /*
        FASTA_element* fasta ;
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
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta4.sequence) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta3.sequence) ;
        delete fasta ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta2.sequence) ;
        delete fasta ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;


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
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta4.sequence) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta3.sequence) ;
        delete fasta ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta2.sequence) ;
        delete fasta ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;

        //------------------------------------------------------------------------
        // file 3
        // file 3 contains a last entry which has no sequence
        fasta_reader.set_file(f_empty_entry3, false, 500) ;

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
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta4.sequence) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta3.sequence) ;
        delete fasta ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta2.sequence) ;
        delete fasta ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;
        */
    }

    // this test reads a file which 2nd entry contains an empty line within the sequence
    TEST(FASTA_entry_with_empty_line)
    {   /*
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
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta4.sequence) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta3.sequence) ;
        delete fasta ;
        // find 2nd entry
        fasta = fasta_reader.get("NC_000002.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta2.sequence) ;
        delete fasta ;
        // find 1st entry
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;
        */
    }

    // this test reads a file which contain 2 identical header. For sequential reading, this
    // is not supposed to be a problem but for searching entries using the header it will
    // always return the first match
    TEST(FASTA_two_identical_header)
    {   /*
        FASTA_element* fasta ;
        FASTAFileReader fasta_reader ;
        FASTA_element fasta1 ;
        FASTA_element fasta2 ;
        FASTA_element fasta3 ;
        FASTA_element fasta4 ;

        fasta_reader.set_file(f_same_header, false, 500) ;

        fasta1.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
        fasta1.sequence = "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA"
                          "CCTTCACTCTCCCATGTACTTCCTGCTAGCCAACCTCTCACTCATTGATCTGTCTCTGTCTTCAGTCACA" ;
        fasta1.sequence_length = fasta1.sequence.length() ;
        fasta1.sequence_one_based = false ;

        fasta2.header   = ">chr|NC_000001|NC_000001.1 Chromosome 1; [Homo sapiens] bla_bla_bla" ;
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
            {   case 0 : CHECK_EQUAL(true, *fasta == fasta1) ; break ;
                case 1 : CHECK_EQUAL(true, *fasta == fasta2) ; break ;
                case 2 : CHECK_EQUAL(true, *fasta == fasta3) ; break ;
                case 3 : CHECK_EQUAL(true, *fasta == fasta4) ; break ;
            }
            delete fasta ;
        }

        // check whether the sequences can be retrieved specifically
        // find 4th entry
        fasta = fasta_reader.get("NC_000004.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta4.sequence) ;
        delete fasta ;
        // find 3rd entry
        fasta = fasta_reader.get("NC_000003.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta3.sequence) ;
        delete fasta ;
        // search entries with the same header, should return the 1st one
        // (fasta1) and never the 2nd one (fasta2)
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;
        // redo the search to be sure it returns again the 1st entry (fasta1)
        fasta = fasta_reader.get("NC_000001.1") ;
        CHECK_EQUAL(true, fasta->sequence == fasta1.sequence) ;
        delete fasta ;
        */
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

