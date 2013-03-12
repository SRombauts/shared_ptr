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

#include "../src/shared_ptr.hpp"


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

void func(void)
{
    // Create an empty (ie. NULL) shared_ptr
    shared_ptr<Xxx> xPtr;

    std::cout << "func: in\n";

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

    } // yPtr is destroyed, but xPtr retains the ownership of the object

    std::cout << "func: out\n";

    // Same as :
    //xPtr.reset();
} // xPtr is destroyed, the reference counter drops to 0 thus the object is destroyed and the memory freed

int main(void)
{
    std::cout << "main\n";

    func ();

    return EXIT_SUCCESS;
}
