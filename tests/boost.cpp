/**
 * @file  boost.hpp
 * @brief Complete Boost Unit Test of this shared_ptr minimal implementation.
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#define BOOST_TEST_MODULE shared_ptr
#include <boost/test/included/unit_test.hpp>

#include "../shared_ptr.hpp"


struct Struct
{
    Struct(int aVal) :
        mVal(aVal)
    {
        ++_mNbInstances;
    }
    ~Struct(void)
    {
        --_mNbInstances;
    }

    int         mVal;
    static int _mNbInstances;
};

int Struct::_mNbInstances = 0;


BOOST_AUTO_TEST_CASE (empty_ptr)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Struct> xPtr;

    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());

    if (xPtr)
    {
        BOOST_ERROR("bool cast operator error");
    }

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());

    // sub-scope
    {
        // Copy construct the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> yPtr(xPtr);

        BOOST_CHECK(false   == xPtr.unique());
        BOOST_CHECK(0       == xPtr.use_count());
        BOOST_CHECK(NULL    == xPtr.get());
        BOOST_CHECK(false   == yPtr.unique());
        BOOST_CHECK(0       == yPtr.use_count());
        BOOST_CHECK(NULL    == yPtr.get());

        // Assign the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> zPtr;
        zPtr = xPtr;

        BOOST_CHECK(false   == xPtr.unique());
        BOOST_CHECK(0       == xPtr.use_count());
        BOOST_CHECK(NULL    == xPtr.get());
        BOOST_CHECK(false   == zPtr.unique());
        BOOST_CHECK(0       == zPtr.use_count());
        BOOST_CHECK(NULL    == zPtr.get());
    }
    // end of scope

    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());
}

BOOST_AUTO_TEST_CASE (basic_ptr)
{
    {
        // Create a shared_ptr
        shared_ptr<Struct> xPtr(new Struct(123));

        BOOST_CHECK(true    == xPtr.unique());
        BOOST_CHECK(1       == xPtr.use_count());
        BOOST_CHECK(NULL    != xPtr.get());

        if (xPtr)
        {
            BOOST_CHECK(123     == xPtr->mVal);
            BOOST_CHECK(1       == xPtr->_mNbInstances);
            BOOST_CHECK(1       == Struct::_mNbInstances);

            // Copy construct the shared_ptr
            shared_ptr<Struct> yPtr(xPtr);

            BOOST_CHECK(xPtr == yPtr);
            BOOST_CHECK(false   == xPtr.unique());
            BOOST_CHECK(2       == xPtr.use_count());
            BOOST_CHECK(NULL    != xPtr.get());
            BOOST_CHECK(123     == xPtr->mVal);
            BOOST_CHECK(1       == Struct::_mNbInstances);
            BOOST_CHECK(false   == yPtr.unique());
            BOOST_CHECK(2       == yPtr.use_count());
            BOOST_CHECK(NULL    != yPtr.get());
            BOOST_CHECK(123     == yPtr->mVal);
            BOOST_CHECK(1       == Struct::_mNbInstances);

            {
                // Assign the shared_ptr
                shared_ptr<Struct> zPtr;
                zPtr = xPtr;

                BOOST_CHECK(xPtr == zPtr);
                BOOST_CHECK(false   == xPtr.unique());
                BOOST_CHECK(3       == xPtr.use_count());
                BOOST_CHECK(NULL    != xPtr.get());
                BOOST_CHECK(123     == xPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
                BOOST_CHECK(false   == yPtr.unique());
                BOOST_CHECK(3       == yPtr.use_count());
                BOOST_CHECK(NULL    != yPtr.get());
                BOOST_CHECK(123     == yPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
                BOOST_CHECK(false   == zPtr.unique());
                BOOST_CHECK(3       == zPtr.use_count());
                BOOST_CHECK(NULL    != zPtr.get());
                BOOST_CHECK(123     == zPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
            }

            BOOST_CHECK(false   == xPtr.unique());
            BOOST_CHECK(2       == xPtr.use_count());
            BOOST_CHECK(NULL    != xPtr.get());
            BOOST_CHECK(123     == xPtr->mVal);
            BOOST_CHECK(1       == Struct::_mNbInstances);
        }
        else
        {
            BOOST_ERROR("bool cast operator error");
        }

        BOOST_CHECK(true    == xPtr.unique());
        BOOST_CHECK(1       == xPtr.use_count());
        BOOST_CHECK(NULL    != xPtr.get());
        BOOST_CHECK(123     == xPtr->mVal);
        BOOST_CHECK(1       == Struct::_mNbInstances);
    }

    BOOST_CHECK(0       == Struct::_mNbInstances);
}

BOOST_AUTO_TEST_CASE (reset_ptr)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Struct> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Struct(123));

    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(123     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);

    // Reset it with another new pointer
    xPtr.reset(new Struct(234));

    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);
}
