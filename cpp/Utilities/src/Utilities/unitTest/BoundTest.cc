/*******************************************************************************
 * testsuite/BoundTest.cc
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
#include <set>
#include <iostream>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Utilities/btree_multimap.h"


namespace stx
{
namespace Tests
{

    template<typename KeyType>
    struct traits_nodebug : stx::btree_default_set_traits<KeyType>
    {
        static const bool selfverify = true;
        static const bool debug = false;

        static const int leafslots = 8;
        static const int innerslots = 8;
    };

    void test_multi(const unsigned int insnum, const int modulo)
    {
        typedef stx::btree_multimap<unsigned int, unsigned int,
                                    std::less<unsigned int>, traits_nodebug<unsigned int> > btree_type;
        btree_type bt;

        typedef std::multiset<unsigned int> multiset_type;
        multiset_type set;

        // *** insert
        srand(34234235);
        for (unsigned int i = 0; i < insnum; i++)
        {
            unsigned int k = rand() % modulo;
            unsigned int v = 234;

            REQUIRE(bt.size() == set.size());
            bt.insert2(k, v);
            set.insert(k);
            REQUIRE(bt.count(k) == set.count(k));

            REQUIRE(bt.size() == set.size());
        }

        REQUIRE(bt.size() == insnum);

        // *** iterate
        {
            btree_type::iterator bi = bt.begin();
            multiset_type::const_iterator si = set.begin();
            for ( ; bi != bt.end() && si != set.end(); ++bi, ++si)
            {
                REQUIRE(*si == bi.key());
            }
            REQUIRE(bi == bt.end());
            REQUIRE(si == set.end());
        }

        // *** existance
        srand(34234235);
        for (unsigned int i = 0; i < insnum; i++)
        {
            unsigned int k = rand() % modulo;

            REQUIRE(bt.exists(k));
        }

        // *** counting
        srand(34234235);
        for (unsigned int i = 0; i < insnum; i++)
        {
            unsigned int k = rand() % modulo;

            REQUIRE(bt.count(k) == set.count(k));
        }

        // *** lower_bound
        for (int k = 0; k < modulo + 100; k++)
        {
            multiset_type::const_iterator si = set.lower_bound(k);
            btree_type::const_iterator bi = bt.lower_bound(k);

            if (bi == bt.end())
                REQUIRE(si == set.end());
            else if (si == set.end())
                REQUIRE(bi == bt.end());
            else
                REQUIRE(*si == bi.key());
        }

        // *** upper_bound
        for (int k = 0; k < modulo + 100; k++)
        {
            multiset_type::const_iterator si = set.upper_bound(k);
            btree_type::const_iterator bi = bt.upper_bound(k);

            if (bi == bt.end())
                REQUIRE(si == set.end());
            else if (si == set.end())
                REQUIRE(bi == bt.end());
            else
                REQUIRE(*si == bi.key());
        }

        // *** equal_range
        for (int k = 0; k < modulo + 100; k++)
        {
            std::pair<multiset_type::const_iterator, multiset_type::const_iterator> si = set.equal_range(k);
            std::pair<btree_type::const_iterator, btree_type::const_iterator> bi = bt.equal_range(k);

            if (bi.first == bt.end())
                REQUIRE(si.first == set.end());
            else if (si.first == set.end())
                REQUIRE(bi.first == bt.end());
            else
                REQUIRE(*si.first == bi.first.key());

            if (bi.second == bt.end())
                REQUIRE(si.second == set.end());
            else if (si.second == set.end())
                REQUIRE(bi.second == bt.end());
            else
                REQUIRE(*si.second == bi.second.key());
        }

        // *** deletion
        srand(34234235);
        for (unsigned int i = 0; i < insnum; i++)
        {
            unsigned int k = rand() % modulo;

            if (set.find(k) != set.end())
            {
                REQUIRE(bt.size() == set.size());

                REQUIRE(bt.exists(k));
                REQUIRE(bt.erase_one(k));
                set.erase(set.find(k));

                REQUIRE(bt.size() == set.size());
            }
        }

        REQUIRE(bt.empty());
        REQUIRE(set.empty());
    }

    TEST_CASE("Testing test_multi(3200, 10)", "[BoundTest]")
    {
        std::cout << "Running test in [BoundTest]" << std::endl;
        test_multi(3200, 10);
    }

    TEST_CASE("Testing test_multi(320, 1000)", "[BoundTest]")
    {
        std::cout << "Running test in [BoundTest]" << std::endl;
        test_multi(320, 1000);
    }

} // end of namespace Tests
} // end of namespace stx
