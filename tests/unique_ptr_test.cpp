/**
 * @file  unique_ptr_test.hpp
 * @brief Complete Unit Test of this unique_ptr minimal implementation using Google Test library.
 *
 * Copyright (c) 2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "unique_ptr.hpp"

#include <vector>

#include <gtest/gtest.h>

struct Struct2
{
    Struct2(int aVal) :
        mVal(aVal)
    {
        ++_mNbInstances;
    }
    ~Struct2(void)
    {
        --_mNbInstances;
    }
    void incr(void)
    {
        ++mVal;
    }
    void decr(void)
    {
        --mVal;
    }

    int         mVal;
    static int _mNbInstances;
};

int Struct2::_mNbInstances = 0;



TEST(unique_ptr, empty_ptr)
{
    // Create an empty (ie. NULL) unique_ptr
    unique_ptr<Struct2> xPtr;

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ((void*)NULL,  xPtr.get());

    if (xPtr)
    {
        GTEST_FATAL_FAILURE_("bool cast operator error");
    }

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ((void*)NULL,  xPtr.get());

    // sub-scope
    {
        // Copy construct the empty (ie. NULL) unique_ptr
        unique_ptr<Struct2> yPtr(xPtr);

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ((void*)NULL,  xPtr.get());
        EXPECT_EQ((void*)NULL,  yPtr.get());

        // Assign the empty (ie. NULL) unique_ptr
        unique_ptr<Struct2> zPtr;
        zPtr = xPtr;

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ((void*)NULL,  xPtr.get());
        EXPECT_EQ((void*)NULL,  zPtr.get());
    }
    // end of scope

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ((void*)NULL,  xPtr.get());
}

TEST(unique_ptr, basic_ptr)
{
    {
        // Create a unique_ptr
        unique_ptr<Struct2> xPtr(new Struct2(123));

        EXPECT_EQ(true, xPtr);
        EXPECT_NE((void*)NULL, xPtr.get());

        if (xPtr)
        {
            EXPECT_EQ(123,  xPtr->mVal);
            EXPECT_EQ(1,    xPtr->_mNbInstances);
            EXPECT_EQ(1,    Struct2::_mNbInstances);

            // call a function
            xPtr->incr();
            EXPECT_EQ(124,  xPtr->mVal);
            (*xPtr).incr();
            EXPECT_EQ(125,  (*xPtr).mVal);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the unique_ptr, transfering ownership
            unique_ptr<Struct2> yPtr(xPtr);

            EXPECT_NE(xPtr,  yPtr);
            EXPECT_EQ(false, xPtr);
            EXPECT_EQ((void*)NULL,  xPtr.get());
            EXPECT_EQ(true,  yPtr);
            EXPECT_NE((void*)NULL,  yPtr.get());
            EXPECT_EQ(123,   yPtr->mVal);
            EXPECT_EQ(1,     Struct2::_mNbInstances);

            if (yPtr)
            {
                // Assign the unique_ptr, transfering ownership
                unique_ptr<Struct2> zPtr;
                zPtr = yPtr;

                EXPECT_NE(yPtr,  zPtr);
                EXPECT_EQ(false, yPtr);
                EXPECT_EQ((void*)NULL, yPtr.get());
                EXPECT_EQ(true,  zPtr);
                EXPECT_NE((void*)NULL,  zPtr.get());
                EXPECT_EQ(123,   zPtr->mVal);
                EXPECT_EQ(1,     Struct2::_mNbInstances);
            }

            EXPECT_EQ(false, xPtr);
            EXPECT_EQ((void*)NULL, xPtr.get());
            EXPECT_EQ(false, yPtr);
            EXPECT_EQ((void*)NULL, yPtr.get());
            EXPECT_EQ(0, Struct2::_mNbInstances);
        }
        else
        {
            GTEST_FATAL_FAILURE_("bool cast operator error");
        }

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ((void*)NULL, xPtr.get());
        EXPECT_EQ(0, Struct2::_mNbInstances);
    }

    EXPECT_EQ(0,     Struct2::_mNbInstances);
}

TEST(unique_ptr, reset_ptr)
{
    // Create an empty (ie. NULL) unique_ptr
    unique_ptr<Struct2> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Struct2(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,  xPtr->mVal);
    EXPECT_EQ(1,    Struct2::_mNbInstances);
    Struct2* pX  = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Struct2(234));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(234,  xPtr->mVal);
    EXPECT_EQ(1,    Struct2::_mNbInstances);
    EXPECT_NE(pX,   xPtr.get());

    // Copy-construct a new unique_ptr to the same object
    unique_ptr<Struct2> yPtr(xPtr);

    EXPECT_NE(xPtr,  yPtr);
    EXPECT_EQ(false,  xPtr);
    EXPECT_EQ((void*)NULL,  xPtr.get());
    EXPECT_EQ(true,  yPtr);
    EXPECT_NE((void*)NULL,  yPtr.get());
    EXPECT_EQ(234,   yPtr->mVal);
    EXPECT_EQ(1,     Struct2::_mNbInstances);

    // Reset to NULL
    yPtr.reset();

    EXPECT_EQ((void*)NULL,  yPtr.get());
    EXPECT_EQ(false, xPtr);
    EXPECT_EQ((void*)NULL,  xPtr.get());
    EXPECT_EQ(0,     Struct2::_mNbInstances);
}

TEST(unique_ptr, compare_ptr)
{
    // Create a unique_ptr
    unique_ptr<Struct2> xPtr(new Struct2(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(1, Struct2::_mNbInstances);
    Struct2* pX = xPtr.get();

    // Create another unique_ptr
    unique_ptr<Struct2> yPtr(new Struct2(234));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(2, Struct2::_mNbInstances);

    EXPECT_EQ(true, yPtr);
    EXPECT_NE((void*)NULL, yPtr.get());
    EXPECT_EQ(234, yPtr->mVal);
    Struct2* pY = yPtr.get();

    EXPECT_NE(xPtr, yPtr);
    if (pX < pY)
    {
        EXPECT_LT(xPtr, yPtr);
        EXPECT_LE(xPtr, yPtr);
        EXPECT_GT(yPtr, xPtr);
        EXPECT_GE(yPtr, xPtr);
    }
    else // (pX > pY)
    {
        EXPECT_GT(xPtr, yPtr);
        EXPECT_GE(xPtr, yPtr);
        EXPECT_LT(yPtr, xPtr);
        EXPECT_LE(yPtr, xPtr);
    }
}

TEST(unique_ptr, swap_ptr)
{
    // Create a unique_ptr
    unique_ptr<Struct2> xPtr(new Struct2(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(1, Struct2::_mNbInstances);
    Struct2* pX = xPtr.get();

    // Create another unique_ptr
    unique_ptr<Struct2> yPtr(new Struct2(234));

    EXPECT_EQ(true, yPtr);
    EXPECT_NE((void*)NULL, yPtr.get());
    EXPECT_EQ(234, yPtr->mVal);
    EXPECT_EQ(2, Struct2::_mNbInstances);
    Struct2* pY = yPtr.get();

    if (pX < pY)
    {
        EXPECT_LT(xPtr, yPtr);
        xPtr.swap(yPtr);
        EXPECT_GT(xPtr, yPtr);
        EXPECT_EQ(true, xPtr);
        EXPECT_EQ(true, yPtr);
    }
    else // (pX > pY)
    {
        EXPECT_GT(xPtr, yPtr);
        xPtr.swap(yPtr);
        EXPECT_LT(xPtr, yPtr);
        EXPECT_EQ(true, xPtr);
        EXPECT_EQ(true, yPtr);
    }
}

TEST(unique_ptr, std_container)
{
    // Create a shared_ptr
    unique_ptr<Struct2> xPtr(new Struct2(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123, xPtr->mVal);
    EXPECT_EQ(1, Struct2::_mNbInstances);
    Struct2* pX = xPtr.get();

    {
        std::vector<unique_ptr<Struct2> > PtrList;

        // Move-it inside a container, transfering ownership
        PtrList.push_back(xPtr);

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ(true,  PtrList.back());
        EXPECT_EQ(pX,    PtrList.back().get());
        EXPECT_EQ(1, Struct2::_mNbInstances);

    } // Destructor of the vector releases the last pointer thus destroying the object

    EXPECT_EQ(0, Struct2::_mNbInstances);
}

