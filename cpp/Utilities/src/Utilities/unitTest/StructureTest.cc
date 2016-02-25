/*******************************************************************************
 * testsuite/StructureTest.cc
 *
 * STX B+ Tree Test Suite v0.9
 * Copyright (C) 2008-2013 Timo Bingmann
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program. If not, see <http://www.gnu.org/licenses/>.
 ******************************************************************************/

// #pragma warning( disable : 4996 )

// SYSTEM INCLUDES
#include <cstdlib>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Utilities/btree_multiset.h"

namespace stx
{
namespace Tests
{

    struct testdata
    {
        unsigned int a, b;

        // required by the btree
        testdata()
            : a(0), b(0)
        { }

        // also used as implicit conversion constructor
        explicit testdata(unsigned int _a)
            : a(_a), b(0)
        { }
    };

    struct testcomp
    {
        unsigned int somevalue;

        explicit testcomp(unsigned int sv)
            : somevalue(sv)
        { }

        bool operator () (const struct testdata& a, const struct testdata& b) const
        {
            return a.a > b.a;
        }
    };

    template <typename KeyType>
    struct traits_nodebug : stx::btree_default_set_traits<KeyType>
    {
        static const bool selfverify = true;
        static const bool debug = false;

        static const int  leafslots = 8;
        static const int  innerslots = 8;
    };

    TEST_CASE("Testing inserting and erasing from btree_multiset", "[StructureTest]")
    {
        std::cout << "Running test in [StructureTest]" << std::endl;
        typedef stx::btree_multiset<struct testdata, struct testcomp,
                                    struct traits_nodebug<struct testdata> > btree_type;

        btree_type bt(testcomp(42));

        srand(34234235);
        for (unsigned int i = 0; i < 320; i++)
        {
            REQUIRE(bt.size() == i);
            bt.insert(testdata(rand() % 100));
            REQUIRE(bt.size() == i + 1);
        }

        srand(34234235);
        for (unsigned int i = 0; i < 320; i++)
        {
            REQUIRE(bt.size() == 320 - i);
            REQUIRE(bt.erase_one(testdata(rand() % 100)));
            REQUIRE(bt.size() == 320 - i - 1);
        }
    }

} // end of namespace Tests
} // end of namespace stx
