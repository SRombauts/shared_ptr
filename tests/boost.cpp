/**
 * @file  boost.hpp
 * @brief Boost Test the subset implementation of the C++11 std::shared_ptr or boost::shared_ptr.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#define BOOST_TEST_MODULE shared_ptr
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE (test_case)
{
   BOOST_CHECK(true);
}
