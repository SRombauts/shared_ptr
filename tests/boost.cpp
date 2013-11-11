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

#include "../src/shared_ptr.hpp"


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



BOOST_AUTO_TEST_CASE (empty_ptr)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Struct> xPtr;

    BOOST_CHECK(false   == xPtr);
    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());

    if (xPtr)
    {
        BOOST_ERROR("bool cast operator error");
    }

    // Reset to NULL (ie. do nothing)
    xPtr.reset();

    BOOST_CHECK(false   == xPtr);
    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());

    // sub-scope
    {
        // Copy construct the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> yPtr(xPtr);

        BOOST_CHECK(false   == xPtr);
        BOOST_CHECK(false   == xPtr.unique());
        BOOST_CHECK(0       == xPtr.use_count());
        BOOST_CHECK(NULL    == xPtr.get());
        BOOST_CHECK(false   == yPtr.unique());
        BOOST_CHECK(0       == yPtr.use_count());
        BOOST_CHECK(NULL    == yPtr.get());

        // Assign the empty (ie. NULL) shared_ptr
        shared_ptr<Struct> zPtr;
        zPtr = xPtr;

        BOOST_CHECK(false   == xPtr);
        BOOST_CHECK(false   == xPtr.unique());
        BOOST_CHECK(0       == xPtr.use_count());
        BOOST_CHECK(NULL    == xPtr.get());
        BOOST_CHECK(false   == zPtr.unique());
        BOOST_CHECK(0       == zPtr.use_count());
        std::cout << "zPtr.get()=" << zPtr.get() << std::endl;
        BOOST_CHECK(NULL    == zPtr.get());
    }
    // end of scope

    BOOST_CHECK(false   == xPtr);
    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());
}

BOOST_AUTO_TEST_CASE (basic_ptr)
{
    {
        // Create a shared_ptr
        shared_ptr<Struct> xPtr(new Struct(123));

        BOOST_CHECK(true    == xPtr);
        BOOST_CHECK(true    == xPtr.unique());
        BOOST_CHECK(1       == xPtr.use_count());
        BOOST_CHECK(NULL    != xPtr.get());

        if (xPtr)
        {
            BOOST_CHECK(123     == xPtr->mVal);
            BOOST_CHECK(1       == xPtr->_mNbInstances);
            BOOST_CHECK(1       == Struct::_mNbInstances);

            // call a function
            xPtr->incr();
            BOOST_CHECK(124     == xPtr->mVal);
            (*xPtr).incr();
            BOOST_CHECK(125     == (*xPtr).mVal);
            xPtr->decr();
            xPtr->decr();

            // Copy construct the shared_ptr
            shared_ptr<Struct> yPtr(xPtr);

            BOOST_CHECK(xPtr    == yPtr);
            BOOST_CHECK(true    == xPtr);
            BOOST_CHECK(false   == xPtr.unique());
            BOOST_CHECK(2       == xPtr.use_count());
            BOOST_CHECK(NULL    != xPtr.get());
            BOOST_CHECK(123     == xPtr->mVal);
            BOOST_CHECK(1       == Struct::_mNbInstances);
            BOOST_CHECK(true    == yPtr);
            BOOST_CHECK(false   == yPtr.unique());
            BOOST_CHECK(2       == yPtr.use_count());
            BOOST_CHECK(NULL    != yPtr.get());
            BOOST_CHECK(123     == yPtr->mVal);
            BOOST_CHECK(1       == Struct::_mNbInstances);

            if (yPtr)
            {
                // Assign the shared_ptr
                shared_ptr<Struct> zPtr;
                zPtr = xPtr;

                BOOST_CHECK(xPtr    == zPtr);
                BOOST_CHECK(true    == xPtr);
                BOOST_CHECK(false   == xPtr.unique());
                BOOST_CHECK(3       == xPtr.use_count());
                BOOST_CHECK(NULL    != xPtr.get());
                BOOST_CHECK(123     == xPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
                BOOST_CHECK(true    == yPtr);
                BOOST_CHECK(false   == yPtr.unique());
                BOOST_CHECK(3       == yPtr.use_count());
                BOOST_CHECK(NULL    != yPtr.get());
                BOOST_CHECK(123     == yPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
                BOOST_CHECK(true    == zPtr);
                BOOST_CHECK(false   == zPtr.unique());
                BOOST_CHECK(3       == zPtr.use_count());
                BOOST_CHECK(NULL    != zPtr.get());
                BOOST_CHECK(123     == zPtr->mVal);
                BOOST_CHECK(1       == Struct::_mNbInstances);
            }

            BOOST_CHECK(true    == xPtr);
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

        BOOST_CHECK(true    == xPtr);
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

    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(123     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);
    Struct* pX  = xPtr.get();

    // Reset it with another new pointer
    xPtr.reset(new Struct(234));

    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);
    BOOST_CHECK(pX      != xPtr.get());

    // Copy-construct a new shared_ptr to the same object
    shared_ptr<Struct> yPtr(xPtr);

    BOOST_CHECK(xPtr    == yPtr);
    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(2       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(true    == yPtr);
    BOOST_CHECK(false   == yPtr.unique());
    BOOST_CHECK(2       == yPtr.use_count());
    BOOST_CHECK(NULL    != yPtr.get());
    BOOST_CHECK(234     == yPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);

    // Reset to NULL
    yPtr.reset();

    BOOST_CHECK(false   == yPtr.unique());
    BOOST_CHECK(0       == yPtr.use_count());
    BOOST_CHECK(NULL    == yPtr.get());
    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);

    // Copy-construct a new shared_ptr to the same object
    shared_ptr<Struct> zPtr(xPtr);

    BOOST_CHECK(xPtr    == zPtr);
    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(2       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(true    == zPtr);
    BOOST_CHECK(false   == zPtr.unique());
    BOOST_CHECK(2       == zPtr.use_count());
    BOOST_CHECK(NULL    != zPtr.get());
    BOOST_CHECK(234     == zPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);

    // Reset it with another new pointer : now xPtr and yPtr each manage a different instance
    zPtr.reset(new Struct(345));

    BOOST_CHECK(xPtr    != zPtr);
    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(true    == zPtr);
    BOOST_CHECK(true    == zPtr.unique());
    BOOST_CHECK(1       == zPtr.use_count());
    BOOST_CHECK(NULL    != zPtr.get());
    BOOST_CHECK(345     == zPtr->mVal);
    BOOST_CHECK(2       == Struct::_mNbInstances);

    // Reset to NULL
    zPtr.reset();

    BOOST_CHECK(false   == zPtr.unique());
    BOOST_CHECK(0       == zPtr.use_count());
    BOOST_CHECK(NULL    == zPtr.get());
    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(234     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);

    // Reset to NULL
    xPtr.reset();

    BOOST_CHECK(false   == xPtr.unique());
    BOOST_CHECK(0       == xPtr.use_count());
    BOOST_CHECK(NULL    == xPtr.get());
    BOOST_CHECK(0       == Struct::_mNbInstances);
}

BOOST_AUTO_TEST_CASE (compare_ptr)
{
    // Create a shared_ptr
    shared_ptr<Struct> xPtr(new Struct(123));

    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(123     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);
    Struct* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Struct> yPtr(new Struct(234));

    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(123     == xPtr->mVal);
    BOOST_CHECK(2       == Struct::_mNbInstances);

    BOOST_CHECK(true    == yPtr);
    BOOST_CHECK(true    == yPtr.unique());
    BOOST_CHECK(1       == yPtr.use_count());
    BOOST_CHECK(NULL    != yPtr.get());
    BOOST_CHECK(234     == yPtr->mVal);
    Struct* pY = yPtr.get();

    BOOST_CHECK(xPtr    != yPtr);
    if (pX < pY)
    {
        BOOST_CHECK(xPtr    <  yPtr);
        BOOST_CHECK(xPtr    <= yPtr);
        BOOST_CHECK(yPtr    >  xPtr);
        BOOST_CHECK(yPtr    >= xPtr);
    }
    else // (pX > pY)
    {
        BOOST_CHECK(xPtr    >  yPtr);
        BOOST_CHECK(xPtr    >= yPtr);
        BOOST_CHECK(yPtr    <  xPtr);
        BOOST_CHECK(yPtr    <= xPtr);
    }

    // Copy a shared_ptr
    shared_ptr<Struct> zPtr = xPtr;
    Struct* pZ = zPtr.get();

    BOOST_CHECK(pX      == pZ);
    BOOST_CHECK(xPtr    == zPtr);
    BOOST_CHECK(zPtr    == xPtr);
    BOOST_CHECK(xPtr    >= zPtr);
    BOOST_CHECK(xPtr    <= zPtr);
}

BOOST_AUTO_TEST_CASE (swap_ptr)
{
    // Create a shared_ptr
    shared_ptr<Struct> xPtr(new Struct(123));

    BOOST_CHECK(true    == xPtr);
    BOOST_CHECK(true    == xPtr.unique());
    BOOST_CHECK(1       == xPtr.use_count());
    BOOST_CHECK(NULL    != xPtr.get());
    BOOST_CHECK(123     == xPtr->mVal);
    BOOST_CHECK(1       == Struct::_mNbInstances);
    Struct* pX = xPtr.get();

    // Create another shared_ptr
    shared_ptr<Struct> yPtr(new Struct(234));

    BOOST_CHECK(true    == yPtr);
    BOOST_CHECK(true    == yPtr.unique());
    BOOST_CHECK(1       == yPtr.use_count());
    BOOST_CHECK(NULL    != yPtr.get());
    BOOST_CHECK(234     == yPtr->mVal);
    BOOST_CHECK(2       == Struct::_mNbInstances);
    Struct* pY = yPtr.get();

    if (pX < pY)
    {
        BOOST_CHECK(xPtr    < yPtr);
        xPtr.swap(yPtr);
        BOOST_CHECK(xPtr    > yPtr);
        BOOST_CHECK(true    == xPtr);
        BOOST_CHECK(true    == yPtr);
    }
    else // (pX > pY)
    {
        BOOST_CHECK(xPtr    > yPtr);
        xPtr.swap(yPtr);
        BOOST_CHECK(xPtr    < yPtr);
        BOOST_CHECK(true    == xPtr);
        BOOST_CHECK(true    == yPtr);
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



BOOST_AUTO_TEST_CASE (pointer_conv)
{
   shared_ptr<A> a0Ptr;
   BOOST_CHECK(false   == a0Ptr);

   {
      shared_ptr<B> bPtr(new B);
      BOOST_CHECK(true    == bPtr);
      BOOST_CHECK(true    == bPtr.unique());
      BOOST_CHECK(1       == bPtr.use_count());
      BOOST_CHECK(NULL    != bPtr.get());
      BOOST_CHECK(1       == A::_mNbInstances);
      BOOST_CHECK(1       == B::_mNbInstances);

      // copy with conversion
      shared_ptr<A> aPtr = bPtr;
      BOOST_CHECK(true    == aPtr);
      BOOST_CHECK(false   == aPtr.unique());
      BOOST_CHECK(2       == aPtr.use_count());
      BOOST_CHECK(NULL    != aPtr.get());
      BOOST_CHECK(1       == A::_mNbInstances);
      BOOST_CHECK(1       == B::_mNbInstances);

      // assignment with conversion
      a0Ptr = bPtr;
      BOOST_CHECK(3       == a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only bPtr converted to a0Ptr survived
   BOOST_CHECK(true    == a0Ptr);
   BOOST_CHECK(true    == a0Ptr.unique());
   BOOST_CHECK(1       == a0Ptr.use_count());
   BOOST_CHECK(NULL    != a0Ptr.get());
   BOOST_CHECK(1       == A::_mNbInstances);
   BOOST_CHECK(1       == B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   BOOST_CHECK(false   == a0Ptr);
   BOOST_CHECK(false   == a0Ptr.unique());
   BOOST_CHECK(0       == a0Ptr.use_count());
   BOOST_CHECK(NULL    == a0Ptr.get());
   BOOST_CHECK(0       == A::_mNbInstances);
   BOOST_CHECK(0       == B::_mNbInstances);
}

BOOST_AUTO_TEST_CASE (stat_pointer_cast)
{
   shared_ptr<A> a0Ptr;
   BOOST_CHECK(false   == a0Ptr);

   {
      shared_ptr<A> aPtr(new A);
      BOOST_CHECK(true    == aPtr);
      BOOST_CHECK(true    == aPtr.unique());
      BOOST_CHECK(1       == aPtr.use_count());
      BOOST_CHECK(NULL    != aPtr.get());
      BOOST_CHECK(1       == A::_mNbInstances);

      shared_ptr<A> abPtr(new B);
      BOOST_CHECK(true    == abPtr);
      BOOST_CHECK(true    == abPtr.unique());
      BOOST_CHECK(1       == abPtr.use_count());
      BOOST_CHECK(NULL    != abPtr.get());
      BOOST_CHECK(2       == A::_mNbInstances);
      BOOST_CHECK(1       == B::_mNbInstances);

      shared_ptr<B> bPtr(new B);
      BOOST_CHECK(true    == bPtr);
      BOOST_CHECK(true    == bPtr.unique());
      BOOST_CHECK(1       == bPtr.use_count());
      BOOST_CHECK(NULL    != bPtr.get());
      BOOST_CHECK(3       == A::_mNbInstances);
      BOOST_CHECK(2       == B::_mNbInstances);

      // dynamic cast
      shared_ptr<A> a2Ptr = static_pointer_cast<A>(bPtr);
      BOOST_CHECK(true    == a2Ptr);
      BOOST_CHECK(false   == a2Ptr.unique());
      BOOST_CHECK(2       == a2Ptr.use_count());
      BOOST_CHECK(NULL    != a2Ptr.get());
      BOOST_CHECK(3       == A::_mNbInstances);
      BOOST_CHECK(2       == B::_mNbInstances);

      // memorize a2Ptr
      a0Ptr = a2Ptr;
      BOOST_CHECK(3       == a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only abPtr converted to a2Ptr survived through a0Ptr
   BOOST_CHECK(true    == a0Ptr);
   BOOST_CHECK(true    == a0Ptr.unique());
   BOOST_CHECK(1       == a0Ptr.use_count());
   BOOST_CHECK(NULL    != a0Ptr.get());
   BOOST_CHECK(1       == A::_mNbInstances);
   BOOST_CHECK(1       == B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   BOOST_CHECK(false   == a0Ptr);
   BOOST_CHECK(false   == a0Ptr.unique());
   BOOST_CHECK(0       == a0Ptr.use_count());
   BOOST_CHECK(NULL    == a0Ptr.get());
   BOOST_CHECK(0       == A::_mNbInstances);
   BOOST_CHECK(0       == B::_mNbInstances);
}

BOOST_AUTO_TEST_CASE (dyn_pointer_cast)
{
   shared_ptr<A> a0Ptr;
   BOOST_CHECK(false   == a0Ptr);

   {
      shared_ptr<A> aPtr(new A);
      BOOST_CHECK(true    == aPtr);
      BOOST_CHECK(true    == aPtr.unique());
      BOOST_CHECK(1       == aPtr.use_count());
      BOOST_CHECK(NULL    != aPtr.get());
      BOOST_CHECK(1       == A::_mNbInstances);

      shared_ptr<A> abPtr(new B);
      BOOST_CHECK(true    == abPtr);
      BOOST_CHECK(true    == abPtr.unique());
      BOOST_CHECK(1       == abPtr.use_count());
      BOOST_CHECK(NULL    != abPtr.get());
      BOOST_CHECK(2       == A::_mNbInstances);
      BOOST_CHECK(1       == B::_mNbInstances);

      shared_ptr<B> bPtr(new B);
      BOOST_CHECK(true    == bPtr);
      BOOST_CHECK(true    == bPtr.unique());
      BOOST_CHECK(1       == bPtr.use_count());
      BOOST_CHECK(NULL    != bPtr.get());
      BOOST_CHECK(3       == A::_mNbInstances);
      BOOST_CHECK(2       == B::_mNbInstances);

      // dynamic cast
      shared_ptr<A> a2Ptr = dynamic_pointer_cast<A>(bPtr);
      BOOST_CHECK(true    == a2Ptr);
      BOOST_CHECK(false   == a2Ptr.unique());
      BOOST_CHECK(2       == a2Ptr.use_count());
      BOOST_CHECK(NULL    != a2Ptr.get());
      BOOST_CHECK(3       == A::_mNbInstances);
      BOOST_CHECK(2       == B::_mNbInstances);

      // memorize a2Ptr
      a0Ptr = a2Ptr;
      BOOST_CHECK(3       == a0Ptr.use_count());
   }
   // after release of the aPtr and bPtr : only abPtr converted to a2Ptr survived through a0Ptr
   BOOST_CHECK(true    == a0Ptr);
   BOOST_CHECK(true    == a0Ptr.unique());
   BOOST_CHECK(1       == a0Ptr.use_count());
   BOOST_CHECK(NULL    != a0Ptr.get());
   BOOST_CHECK(1       == A::_mNbInstances);
   BOOST_CHECK(1       == B::_mNbInstances);

   // release the last one
   a0Ptr.reset();
   BOOST_CHECK(false   == a0Ptr);
   BOOST_CHECK(false   == a0Ptr.unique());
   BOOST_CHECK(0       == a0Ptr.use_count());
   BOOST_CHECK(NULL    == a0Ptr.get());
   BOOST_CHECK(0       == A::_mNbInstances);
   BOOST_CHECK(0       == B::_mNbInstances);
}

