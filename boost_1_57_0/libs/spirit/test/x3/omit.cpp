/*=============================================================================
    Copyright (c) 2001-2013 Joel de Guzman

    Distributed under the Boost Software License, Version 1.0. (See accompanying
    file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
=============================================================================*/
#include <boost/detail/lightweight_test.hpp>
#include <boost/spirit/home/x3.hpp>
#include <boost/fusion/include/vector.hpp>
#include <boost/fusion/include/at.hpp>

#include <string>
#include <iostream>
#include "test.hpp"

int
main()
{
    using namespace boost::spirit::x3::ascii;
    using boost::spirit::x3::omit;
    using boost::spirit::x3::unused_type;
    using boost::spirit::x3::unused;
    using boost::spirit::x3::int_;

    using boost::fusion::vector;
    using boost::fusion::at_c;

    using spirit_test::test;
    using spirit_test::test_attr;

    {
        BOOST_TEST(test("a", omit['a']));
    }

    {
        // omit[] means we don't receive the attribute
        char attr;
        BOOST_TEST((test_attr("abc", omit[char_] >> omit['b'] >> char_, attr)));
        BOOST_TEST((attr == 'c'));
    }

    {
        // If all elements except 1 is omitted, the attribute is
        // a single-element sequence. For this case alone, we allow
        // naked attributes (unwrapped in a fusion sequence).
        char attr;
        BOOST_TEST((test_attr("abc", omit[char_] >> 'b' >> char_, attr)));
        BOOST_TEST((attr == 'c'));
    }

    {
        // omit[] means we don't receive the attribute
        vector<> attr;
        BOOST_TEST((test_attr("abc", omit[char_] >> omit['b'] >> omit[char_], attr)));
    }

    {
        // omit[] means we don't receive the attribute
        // this test is merely a compile test, because using a unused as the
        // explicit attribute doesn't make any sense
        unused_type attr;
        BOOST_TEST((test_attr("abc", omit[char_ >> 'b' >> char_], attr)));
    }

    {
        // omit[] means we don't receive the attribute, if all elements of a
        // sequence have unused attributes, the whole sequence has an unused
        // attribute as well
        vector<char, char> attr;
        BOOST_TEST((test_attr("abcde",
            char_ >> (omit[char_] >> omit['c'] >> omit[char_]) >> char_, attr)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'e'));
    }

    {
        // "hello" has an unused_type. unused attrubutes are not part of the sequence
        vector<char, char> attr;
        BOOST_TEST((test_attr("a hello c", char_ >> "hello" >> char_, attr, space)));
        BOOST_TEST((at_c<0>(attr) == 'a'));
        BOOST_TEST((at_c<1>(attr) == 'c'));
    }

    {
        // if only one node in a sequence is left (all the others are omitted),
        // then we need "naked" attributes (not wraped in a tuple)
        int attr;
        BOOST_TEST((test_attr("a 123 c", omit['a'] >> int_ >> omit['c'], attr, space)));
        BOOST_TEST((attr == 123));
    }

    {
        // unused means we don't care about the attribute
        BOOST_TEST((test_attr("abc", char_ >> 'b' >> char_, unused)));
    }

    // $$$ Not yet implemented $$$
    //~ {   // test action with omitted attribute
        //~ char c = 0;

        //~ using boost::phoenix::ref;

        //~ BOOST_TEST(test("x123\"a string\"", (char_ >> omit[int_] >> "\"a string\"")
            //~ [ref(c) = _1]));
        //~ BOOST_TEST(c == 'x');
    //~ }

    // $$$ Not yet implemented $$$
    //~ {   // test action with omitted attribute
        //~ int n = 0;

        //~ using boost::phoenix::ref;

        //~ BOOST_TEST(test("x 123 \"a string\"",
            //~ (omit[char_] >> int_ >> "\"a string\"")[ref(n) = _1], space));
        //~ BOOST_TEST(n == 123);
    //~ }

    return boost::report_errors();
}

