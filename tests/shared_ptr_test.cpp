/**
 * @file  shared_ptr_test.hpp
 * @brief Complete Unit Test of this shared_ptr minimal implementation using Google Test library.
 *
 * Copyright (c) 2013-2014 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */

#include "shared_ptr.hpp"

#include <vector>

#include <gtest/gtest.h>

struct Struct
{
    explicit Struct(int aVal) :
        mVal(aVal)
    {
        ++_mNbInstances;
    }
    ~Struct(void)
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

int Struct::_mNbInstances = 0;



TEST(shared_ptr, empty_ptr)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Struct> xPtr;

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(0,     xPtr.use_count());
    EXPECT_EQ((void*)NULL,  xPtr.get());

    if (xPtr)
    {
        GTEST_FATAL_FAILURE_("bool cast operator error");
    }

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(0,     xPtr.use_count());
    EXPECT_EQ((void*)NULL,  xPtr.get());

    // sub-scope
    {
        // Copy construct the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> yPtr(xPtr);

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ(false, xPtr.unique());
        EXPECT_EQ(0,     xPtr.use_count());
        EXPECT_EQ((void*)NULL,  xPtr.get());
        EXPECT_EQ(false, yPtr.unique());
        EXPECT_EQ(0,     yPtr.use_count());
        EXPECT_EQ((void*)NULL,  yPtr.get());

        // Assign the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> zPtr;
        zPtr = xPtr;

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ(false, xPtr.unique());
        EXPECT_EQ(0,     xPtr.use_count());
        EXPECT_EQ((void*)NULL,  xPtr.get());
        EXPECT_EQ(false, zPtr.unique());
        EXPECT_EQ(0,     zPtr.use_count());
        EXPECT_EQ((void*)NULL,  zPtr.get());
    }
    // end of scope

    EXPECT_EQ(false, xPtr);
    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(0,     xPtr.use_count());
    EXPECT_EQ((void*)NULL,  xPtr.get());
}

TEST(shared_ptr, basic_ptr)
{
    {
        // Create a shared_ptr
        shared_ptr<Struct> xPtr(new Struct(123));

        EXPECT_EQ(true, xPtr);
        EXPECT_EQ(true, xPtr.unique());
        EXPECT_EQ(1,    xPtr.use_count());
        EXPECT_NE((void*)NULL, xPtr.get());

        if (xPtr)
        {
            EXPECT_EQ(123,  xPtr->mVal);
            EXPECT_EQ(1,    xPtr->_mNbInstances);
            EXPECT_EQ(1,    Struct::_mNbInstances);

            // call a function
            xPtr->incr();
            EXPECT_EQ(124,  xPtr->mVal);
            (*xPtr).incr();
            EXPECT_EQ(125,  (*xPtr).mVal);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the shared_ptr
            shared_ptr<Struct> yPtr(xPtr);

            EXPECT_EQ(xPtr,  yPtr);
            EXPECT_EQ(true,  xPtr);
            EXPECT_EQ(false, xPtr.unique());
            EXPECT_EQ(2,     xPtr.use_count());
            EXPECT_NE((void*)NULL,  xPtr.get());
            EXPECT_EQ(123,   xPtr->mVal);
            EXPECT_EQ(1,     Struct::_mNbInstances);
            EXPECT_EQ(true,  yPtr);
            EXPECT_EQ(false, yPtr.unique());
            EXPECT_EQ(2,     yPtr.use_count());
            EXPECT_NE((void*)NULL,  yPtr.get());
            EXPECT_EQ(123,   yPtr->mVal);
            EXPECT_EQ(1,     Struct::_mNbInstances);

            if (yPtr)
            {
                // Assign the shared_ptr
                shared_ptr<Struct> zPtr;
                zPtr = xPtr;

                EXPECT_EQ(xPtr,  zPtr);
                EXPECT_EQ(true,  xPtr);
                EXPECT_EQ(false, xPtr.unique());
                EXPECT_EQ(3,     xPtr.use_count());
                EXPECT_NE((void*)NULL,  xPtr.get());
                EXPECT_EQ(123,   xPtr->mVal);
                EXPECT_EQ(1,     Struct::_mNbInstances);
                EXPECT_EQ(true,  yPtr);
                EXPECT_EQ(false, yPtr.unique());
                EXPECT_EQ(3,     yPtr.use_count());
                EXPECT_NE((void*)NULL,  yPtr.get());
                EXPECT_EQ(123,   yPtr->mVal);
                EXPECT_EQ(1,     Struct::_mNbInstances);
                EXPECT_EQ(true,  zPtr);
                EXPECT_EQ(false, zPtr.unique());
                EXPECT_EQ(3,     zPtr.use_count());
                EXPECT_NE((void*)NULL,  zPtr.get());
                EXPECT_EQ(123,   zPtr->mVal);
                EXPECT_EQ(1,     Struct::_mNbInstances);
            }

            EXPECT_EQ(true,  xPtr);
            EXPECT_EQ(false, xPtr.unique());
            EXPECT_EQ(2,     xPtr.use_count());
            EXPECT_NE((void*)NULL,  xPtr.get());
            EXPECT_EQ(123,   xPtr->mVal);
            EXPECT_EQ(1,     Struct::_mNbInstances);
        }
        else
        {
            GTEST_FATAL_FAILURE_("bool cast operator error");
        }

        EXPECT_EQ(true, xPtr);
        EXPECT_EQ(true, xPtr.unique());
        EXPECT_EQ(1,    xPtr.use_count());
        EXPECT_NE((void*)NULL, xPtr.get());
        EXPECT_EQ(123,  xPtr->mVal);
        EXPECT_EQ(1,    Struct::_mNbInstances);
    }

    EXPECT_EQ(0,     Struct::_mNbInstances);
}

TEST(shared_ptr, reset_ptr)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Struct> xPtr;

    // Reset it with a new pointer
    xPtr.reset(new Struct(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1,    xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,  xPtr->mVal);
    EXPECT_EQ(1,    Struct::_mNbInstances);
    Struct* pX  = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Struct(234));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1,    xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(234,  xPtr->mVal);
    EXPECT_EQ(1,    Struct::_mNbInstances);
    EXPECT_NE(pX,   xPtr.get());

    // Copy-construct a new shared_ptr to the same object
    shared_ptr<Struct> yPtr(xPtr);

    EXPECT_EQ(xPtr,  yPtr);
    EXPECT_EQ(true,  xPtr);
    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(2,     xPtr.use_count());
    EXPECT_NE((void*)NULL,  xPtr.get());
    EXPECT_EQ(234,   xPtr->mVal);
    EXPECT_EQ(true,  yPtr);
    EXPECT_EQ(false, yPtr.unique());
    EXPECT_EQ(2,     yPtr.use_count());
    EXPECT_NE((void*)NULL,  yPtr.get());
    EXPECT_EQ(234,   yPtr->mVal);
    EXPECT_EQ(1,     Struct::_mNbInstances);

    // Reset to NULL
    yPtr.reset();

    EXPECT_EQ(false, yPtr.unique());
    EXPECT_EQ(0,     yPtr.use_count());
    EXPECT_EQ((void*)NULL,  yPtr.get());
    EXPECT_EQ(true,  xPtr);
    EXPECT_EQ(true,  xPtr.unique());
    EXPECT_EQ(1,     xPtr.use_count());
    EXPECT_NE((void*)NULL,  xPtr.get());
    EXPECT_EQ(234,   xPtr->mVal);
    EXPECT_EQ(1,     Struct::_mNbInstances);

    // Copy-construct a new shared_ptr to the same object
    shared_ptr<Struct> zPtr(xPtr);

    EXPECT_EQ(xPtr,  zPtr);
    EXPECT_EQ(true,  xPtr);
    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(2, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(234, xPtr->mVal);
    EXPECT_EQ(true, zPtr);
    EXPECT_EQ(false, zPtr.unique());
    EXPECT_EQ(2, zPtr.use_count());
    EXPECT_NE((void*)NULL, zPtr.get());
    EXPECT_EQ(234, zPtr->mVal);
    EXPECT_EQ(1, Struct::_mNbInstances);

    // Reset it with another new pointer : now xPtr and yPtr each manage a different instance
    zPtr.reset(new Struct(345));

    EXPECT_NE(xPtr, zPtr);
    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(234, xPtr->mVal);
    EXPECT_EQ(true, zPtr);
    EXPECT_EQ(true, zPtr.unique());
    EXPECT_EQ(1, zPtr.use_count());
    EXPECT_NE((void*)NULL, zPtr.get());
    EXPECT_EQ(345, zPtr->mVal);
    EXPECT_EQ(2, Struct::_mNbInstances);

    // Reset to NULL
    zPtr.reset();

    EXPECT_EQ(false, zPtr.unique());
    EXPECT_EQ(0,     zPtr.use_count());
    EXPECT_EQ((void*)NULL,  zPtr.get());
    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(234, xPtr->mVal);
    EXPECT_EQ(1, Struct::_mNbInstances);

    // Reset to NULL
    xPtr.reset();

    EXPECT_EQ(false, xPtr.unique());
    EXPECT_EQ(0,     xPtr.use_count());
    EXPECT_EQ((void*)NULL,  xPtr.get());
    EXPECT_EQ(0,     Struct::_mNbInstances);
}

TEST(shared_ptr, compare_ptr)
{
    // Create a shared_ptr
    shared_ptr<Struct> xPtr(new Struct(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(1, Struct::_mNbInstances);
    Struct* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Struct> yPtr(new Struct(234));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(2, Struct::_mNbInstances);

    EXPECT_EQ(true, yPtr);
    EXPECT_EQ(true, yPtr.unique());
    EXPECT_EQ(1, yPtr.use_count());
    EXPECT_NE((void*)NULL, yPtr.get());
    EXPECT_EQ(234, yPtr->mVal);
    Struct* pY = yPtr.get();

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

    // Copy a shared_ptr
    shared_ptr<Struct> zPtr = xPtr;
    Struct* pZ = zPtr.get();

    EXPECT_EQ(pX, pZ);
    EXPECT_EQ(xPtr, zPtr);
    EXPECT_EQ(zPtr, xPtr);
    EXPECT_GE(xPtr, zPtr);
    EXPECT_LE(xPtr, zPtr);
}

TEST(shared_ptr, std_container)
{
    // Create a shared_ptr
    shared_ptr<Struct> xPtr(new Struct(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1,    xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,  xPtr->mVal);
    EXPECT_EQ(1, Struct::_mNbInstances);
    Struct* pX = xPtr.get();

    {
        std::vector<shared_ptr<Struct> > PtrList;

        // Copy-it inside a container
        PtrList.push_back(xPtr);

        EXPECT_EQ(true,  xPtr);
        EXPECT_EQ(false, xPtr.unique());
        EXPECT_EQ(2,     xPtr.use_count());
        EXPECT_EQ(2,     PtrList.back().use_count());
        EXPECT_EQ(xPtr,  PtrList.back());
        EXPECT_EQ(pX,    PtrList.back().get());
        EXPECT_EQ(1, Struct::_mNbInstances);

        // And copy-it again
        PtrList.push_back(xPtr);

        EXPECT_EQ(true,  xPtr);
        EXPECT_EQ(false, xPtr.unique());
        EXPECT_EQ(3,     xPtr.use_count());
        EXPECT_EQ(3,     PtrList.back().use_count());
        EXPECT_EQ(xPtr,  PtrList.back());
        EXPECT_EQ(pX,    PtrList.back().get());
        EXPECT_EQ(1, Struct::_mNbInstances);

        // Remove the second pointer from the vector
        PtrList.pop_back();

        EXPECT_EQ(true,  xPtr);
        EXPECT_EQ(false, xPtr.unique());
        EXPECT_EQ(2,     xPtr.use_count());
        EXPECT_EQ(2,     PtrList.back().use_count());
        EXPECT_EQ(xPtr,  PtrList.back());
        EXPECT_EQ(pX,    PtrList.back().get());
        EXPECT_EQ(1, Struct::_mNbInstances);

        // Reset the original pointer, leaving 1 last pointer in the vector
        xPtr.reset();

        EXPECT_EQ(false, xPtr);
        EXPECT_EQ(1,     PtrList.back().use_count());
        EXPECT_EQ(pX,    PtrList.back().get());
        EXPECT_EQ(1, Struct::_mNbInstances);

    } // Destructor of the vector releases the last pointer thus destroying the object

    EXPECT_EQ(0, Struct::_mNbInstances);
}

TEST(shared_ptr, swap_ptr)
{
    // Create a shared_ptr
    shared_ptr<Struct> xPtr(new Struct(123));

    EXPECT_EQ(true, xPtr);
    EXPECT_EQ(true, xPtr.unique());
    EXPECT_EQ(1, xPtr.use_count());
    EXPECT_NE((void*)NULL, xPtr.get());
    EXPECT_EQ(123,xPtr->mVal);
    EXPECT_EQ(1, Struct::_mNbInstances);
    Struct* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Struct> yPtr(new Struct(234));

    EXPECT_EQ(true, yPtr);
    EXPECT_EQ(true, yPtr.unique());
    EXPECT_EQ(1, yPtr.use_count());
    EXPECT_NE((void*)NULL, yPtr.get());
    EXPECT_EQ(234, yPtr->mVal);
    EXPECT_EQ(2, Struct::_mNbInstances);
    Struct* pY = yPtr.get();

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


class A
{
public:
   A(){++_mNbInstances;};
   virtual ~A(){--_mNbInstances;};
   static int _mNbInstances;
};
int A::_mNbInstances = 0;

class B : public A
{
public:
   B(){++_mNbInstances;};
   virtual ~B(){--_mNbInstances;};
   static int _mNbInstances;
};
int B::_mNbInstances = 0;



TEST(shared_ptr, pointer_conv)
{
   shared_ptr<A> a0Ptr;
   EXPECT_EQ(false, a0Ptr);

   {
      shared_ptr<B> bPtr(new B);
      EXPECT_EQ(true, bPtr);
      EXPECT_EQ(true, bPtr.unique());
      EXPECT_EQ(1, bPtr.use_count());
      EXPECT_NE((void*)NULL, bPtr.get());
      EXPECT_EQ(1, A::_mNbInstances);
      EXPECT_EQ(1, B::_mNbInstances);

      // copy with conversion
      shared_ptr<A> aPtr = bPtr;
      EXPECT_EQ(true, aPtr);
      EXPECT_EQ(false, aPtr.unique());
      EXPECT_EQ(2, aPtr.use_count());
      EXPECT_NE((void*)NULL, aPtr.get());
      EXPECT_EQ(1, A::_mNbInstances);
      EXPECT_EQ(1, B::_mNbInstances);

      // assignment with conversion
      a0Ptr = bPtr;
      EXPECT_EQ(3, a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only bPtr converted to a0Ptr survived
   EXPECT_EQ(true, a0Ptr);
   EXPECT_EQ(true, a0Ptr.unique());
   EXPECT_EQ(1, a0Ptr.use_count());
   EXPECT_NE((void*)NULL, a0Ptr.get());
   EXPECT_EQ(1, A::_mNbInstances);
   EXPECT_EQ(1, B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   EXPECT_EQ(false, a0Ptr);
   EXPECT_EQ(false, a0Ptr.unique());
   EXPECT_EQ(0,     a0Ptr.use_count());
   EXPECT_EQ((void*)NULL,  a0Ptr.get());
   EXPECT_EQ(0,     A::_mNbInstances);
   EXPECT_EQ(0,     B::_mNbInstances);
}

TEST(shared_ptr, stat_pointer_cast)
{
   shared_ptr<A> a0Ptr;
   EXPECT_EQ(false, a0Ptr);

   {
      shared_ptr<A> aPtr(new A);
      EXPECT_EQ(true, aPtr);
      EXPECT_EQ(true, aPtr.unique());
      EXPECT_EQ(1, aPtr.use_count());
      EXPECT_NE((void*)NULL, aPtr.get());
      EXPECT_EQ(1, A::_mNbInstances);

      shared_ptr<A> abPtr(new B);
      EXPECT_EQ(true, abPtr);
      EXPECT_EQ(true, abPtr.unique());
      EXPECT_EQ(1, abPtr.use_count());
      EXPECT_NE((void*)NULL, abPtr.get());
      EXPECT_EQ(2, A::_mNbInstances);
      EXPECT_EQ(1, B::_mNbInstances);

      shared_ptr<B> bPtr(new B);
      EXPECT_EQ(true, bPtr);
      EXPECT_EQ(true, bPtr.unique());
      EXPECT_EQ(1, bPtr.use_count());
      EXPECT_NE((void*)NULL, bPtr.get());
      EXPECT_EQ(3, A::_mNbInstances);
      EXPECT_EQ(2, B::_mNbInstances);

      // static cast
      shared_ptr<A> a2Ptr = static_pointer_cast<A>(bPtr);
      EXPECT_EQ(true, a2Ptr);
      EXPECT_EQ(false, a2Ptr.unique());
      EXPECT_EQ(2, a2Ptr.use_count());
      EXPECT_NE((void*)NULL, a2Ptr.get());
      EXPECT_EQ(3, A::_mNbInstances);
      EXPECT_EQ(2, B::_mNbInstances);

      // memorize a2Ptr
      a0Ptr = a2Ptr;
      EXPECT_EQ(3, a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only abPtr converted to a2Ptr survived through a0Ptr
   EXPECT_EQ(true, a0Ptr);
   EXPECT_EQ(true, a0Ptr.unique());
   EXPECT_EQ(1, a0Ptr.use_count());
   EXPECT_NE((void*)NULL, a0Ptr.get());
   EXPECT_EQ(1, A::_mNbInstances);
   EXPECT_EQ(1, B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   EXPECT_EQ(false, a0Ptr);
   EXPECT_EQ(false, a0Ptr.unique());
   EXPECT_EQ(0,     a0Ptr.use_count());
   EXPECT_EQ((void*)NULL,  a0Ptr.get());
   EXPECT_EQ(0,     A::_mNbInstances);
   EXPECT_EQ(0,     B::_mNbInstances);
}

TEST(shared_ptr, dyn_pointer_cast)
{
   shared_ptr<A> a0Ptr;
   EXPECT_EQ(false, a0Ptr);

   {
      shared_ptr<A> aPtr(new A);
      EXPECT_EQ(true, aPtr);
      EXPECT_EQ(true, aPtr.unique());
      EXPECT_EQ(1, aPtr.use_count());
      EXPECT_NE((void*)NULL, aPtr.get());
      EXPECT_EQ(1, A::_mNbInstances);

      shared_ptr<A> abPtr(new B);
      EXPECT_EQ(true, abPtr);
      EXPECT_EQ(true, abPtr.unique());
      EXPECT_EQ(1, abPtr.use_count());
      EXPECT_NE((void*)NULL, abPtr.get());
      EXPECT_EQ(2, A::_mNbInstances);
      EXPECT_EQ(1, B::_mNbInstances);

      shared_ptr<B> bPtr(new B);
      EXPECT_EQ(true, bPtr);
      EXPECT_EQ(true, bPtr.unique());
      EXPECT_EQ(1, bPtr.use_count());
      EXPECT_NE((void*)NULL, bPtr.get());
      EXPECT_EQ(3, A::_mNbInstances);
      EXPECT_EQ(2, B::_mNbInstances);

      // dynamic cast
      shared_ptr<A> a2Ptr = dynamic_pointer_cast<A>(bPtr);
      EXPECT_EQ(true, a2Ptr);
      EXPECT_EQ(false, a2Ptr.unique());
      EXPECT_EQ(2, a2Ptr.use_count());
      EXPECT_NE((void*)NULL, a2Ptr.get());
      EXPECT_EQ(3, A::_mNbInstances);
      EXPECT_EQ(2, B::_mNbInstances);

      // memorize a2Ptr
      a0Ptr = a2Ptr;
      EXPECT_EQ(3, a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only abPtr converted to a2Ptr survived through a0Ptr
   EXPECT_EQ(true, a0Ptr);
   EXPECT_EQ(true, a0Ptr.unique());
   EXPECT_EQ(1, a0Ptr.use_count());
   EXPECT_NE((void*)NULL, a0Ptr.get());
   EXPECT_EQ(1, A::_mNbInstances);
   EXPECT_EQ(1, B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   EXPECT_EQ(false, a0Ptr);
   EXPECT_EQ(false, a0Ptr.unique());
   EXPECT_EQ(0,     a0Ptr.use_count());
   EXPECT_EQ((void*)NULL,  a0Ptr.get());
   EXPECT_EQ(0,     A::_mNbInstances);
   EXPECT_EQ(0,     B::_mNbInstances);
}

