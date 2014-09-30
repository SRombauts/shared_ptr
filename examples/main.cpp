/**
 * @file  main.hpp
 * @brief sample demonstrating how to use this shared_ptr minimal implementation
 *
 * Copyright (c) 2013 Sebastien Rombauts (sebastien.rombauts@gmail.com)
 *
 * Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
 * or copy at http://opensource.org/licenses/MIT)
 */
#include <iostream>  // std::cout
#include <cstring>   // memset
#include <cstdlib>   // abort

#include "shared_ptr.hpp"

class Xxx
{
public:
    explicit Xxx(size_t len = 0) :
        size(len),
        buffer(NULL)
    {
        if (0 < size)
        {
            buffer = new char[len];
            memset(buffer, 0, size);
            std::cout << "new buffer(" << size << ")\n";
        }
    };
    ~Xxx(void)
    {
        if (0 < size)
        {
            delete [] buffer;
            std::cout << "delete buffer(" << size << ")\n";
        }
    }
    void doSomething(void)
    {
        if (0 < size)
        {
            memset(buffer, 'x', size);
            std::cout << "doSomething buffer(" << size << ")\n";
        }
    };

private:
    size_t  size;
    char*   buffer;
};

void shared_ptr_test(void)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Xxx> xPtr;

    std::cout << "shared_ptr_test: in\n";

    if (xPtr) // empty pointer
    {
        abort(); // impossible
    }
    else
    {
        // Create a new Xxx object, and give its ownership to the yPtr shared_ptr
        shared_ptr<Xxx> yPtr(new Xxx(1024));
        // Same as :
        //shared_ptr<Xxx> yPtr;
        //yPtr.reset(new Xxx(1024));

        std::cout << "yPtr=" << yPtr.get() << std::endl;
        if (yPtr) // valid pointer
        {
            // Access members functions/variables like with a raw pointer
            yPtr->doSomething();
        }
        else
        {
            abort(); // impossible
        }

        // Share ownership by making a copy of the shared_ptr (the reference counter reachs 2)
        xPtr = yPtr;
        std::cout << "xPtr=" << xPtr.get() << std::endl;
        std::cout << "yPtr=" << yPtr.get() << std::endl;

    } // yPtr is destroyed, but xPtr retains the ownership of the object

    std::cout << "xPtr=" << xPtr.get() << std::endl;
    std::cout << "shared_ptr_test: out\n";

    // Same as :
    //xPtr.reset();
} // xPtr is destroyed, the reference counter drops to 0 thus the object is destroyed and the memory freed

// TODO
#include <memory>

void unique_ptr_test(void)
{
    // Create an empty (ie. NULL) unique_ptr
    std::unique_ptr<Xxx> xPtr;

    std::cout << "unique_ptr_test: in\n";

    if (xPtr) // empty pointer
    {
        abort(); // impossible
    }
    else
    {
        // Create a new Xxx object, and give its ownership to the yPtr unique_ptr
        std::unique_ptr<Xxx> yPtr(new Xxx(1024));
        // Same as :
        //unique_ptr<Xxx> yPtr;
        //yPtr.reset(new Xxx(1024));

        std::cout << "yPtr=" << yPtr.get() << std::endl;
        if (yPtr) // valid pointer
        {
            // Access members functions/variables like with a raw pointer
            yPtr->doSomething();
        }
        else
        {
            abort(); // impossible
        }

        // Transfert ownership by making a copy of the unique_ptr
        xPtr = std::move(yPtr);
        std::cout << "xPtr=" << xPtr.get() << std::endl;
        std::cout << "yPtr=" << yPtr.get() << std::endl;

    } // yPtr is destroyed, but xPtr retains the ownership of the object

    std::cout << "xPtr=" << xPtr.get() << std::endl;
    std::cout << "unique_ptr_test: out\n";

    // Same as :
    //xPtr.reset();
} // xPtr is destroyed, thus the object is destroyed and the memory freed

int main(void)
{
    shared_ptr_test();
    std::cout << std::endl;
    unique_ptr_test();

    return EXIT_SUCCESS;
}
