shared_ptr
----------

![shared_ptr build status](https://api.travis-ci.org/SRombauts/shared_ptr.png "shared_ptr build status")

[shared_ptr](include/shared_ptr.hpp) is a minimal implementation of smart pointer, a subset of the C++11 std::shared_ptr or boost::shared_ptr.

It comes with a fake implementation of a [unique_ptr](include/unique_ptr.hpp) for C++98.

### The goals of this minimal shared_ptr are:

- to replace the C++11 std::shared_ptr and boost::shared_ptr where they are not available
- to be a header only implementation
- to keep dependencies to a minimum (STL)
- to be portable
- to be light (minimizing code size, presently 92 bytes per template usage)
- to be fast and monothreaded (not thread-safe)
- to be well documented with Doxygen tags
- to have a perfect unit test coverage
- to use a permissive MIT license, similar to BSD or Boost, for proprietary/commercial usage

- to also provide a fake unique_ptr to be used with older compiler

### Limitations
- does not come with a weak_ptr implementation
- does not manage array type (does not call delete[] for array allocated with new[])
- does not manage the underlying raw pointer type separately from the template shared_ptr type : does not call delete on the right type, thus needing virtual destructor (as with raw pointer)
- does not distinguish between the stored pointer and the owned pointer : cannot store a pointer to object member while managing a pointer to the object itself
- not thread-safe

- the fake unique_ptr does not at all conform to the standard, and so is only a placeholder for use with older compilers

### Supported platforms:

Developments and tests are done under the following OSs :
- Debian 7 (testing)
- Ubuntu 12.10
- Windows XP/7/8
And following IDEs/Compilers
- GCC 4.7.x with a provided Makefile
- Eclipse CDT under Linux, using the provided Makefile
- Visual Studio Express 2008/2010/2012 for testing compatibility purpose

### Dependencies:

 - a STL implementation (even an old one, like those provided with VC6/eVC4 should work)

### Installation

To use this shared_ptr implementation, you only need to include the shared_ptr.hpp file
from the source code of your projects.

### Continuous Integration

This project is continuously tested under Ubuntu Linux with the gcc and clang compilers
using the Travis CI community service with the above CMake building and testing procedure.

Detailed results can be seen online: https://travis-ci.org/SRombauts/shared_ptr

### License

Copyright (c) 2013-2014 Sébastien Rombauts (sebastien.rombauts@gmail.com)

Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
or copy at http://opensource.org/licenses/MIT)

## Getting started
### About std::shared_ptr:
- http://www.cplusplus.com/reference/memory/shared_ptr/
- http://en.cppreference.com/w/cpp/memory/shared_ptr

### About boost::shared_ptr:
- http://www.boost.org/libs/smart_ptr/shared_ptr.htm

### About std::unique_ptr:
- http://www.cplusplus.com/reference/memory/unique_ptr/

### First sample demonstrates how to create a shared_ptr to a class Xxx:

The Xxx class:
```C++
class Xxx
{
public:
    Xxx(size_t len = 0);
    ~Xxx(void);
    ...
    void doSomething(void);
    ...
};
```

shared_ptr usage:
```C++
void func(void)
{
    // Create an empty (ie. NULL) p1 shared_ptr
    shared_ptr<Xxx> xPtr;

    if (xPtr) // empty pointer
    {
        // impossible
    }
    else
    {
        // Create a new Xxx object, and give its ownership to the yPtr shared_ptr
        shared_ptr<Xxx> yPtr(new Xxx(1024));

        if (yPtr) // valid pointer
        {
            // Access members functions/variables like with a raw pointer
            yPtr->doSomething();
        }
        else
        {
            // impossible
        }

        // Share ownership by making a copy of the shared_ptr (the reference counter reaches 2)
        xPtr = yPtr;

    } // yPtr is destroyed, but xPtr retains the ownership of the object

    ...

} // xPtr is destroyed, the reference counter drops to 0 thus the object is destroyed and the memory freed
```

## How to contribute
### GitHub website
The most efficient way to help and contribute to this wrapper project is to
use the tools provided by GitHub:
- please fill bug reports and feature requests here: https://github.com/SRombauts/shared_ptr/issues
- fork the repository, make some small changes and submit them with pull-request

### Contact
You can also email me directly, I will answer any questions and requests.

### Coding Style Guidelines
The source code use the CamelCase naming style variant where :
- type names (class, struct, typedef, enums...) begins with a capital letter
- files (.cpp/.h) are named like the class they contains
- function and variable names begins with a lower case letter
- member variables begins with a 'm', function arguments begins with a 'a', boolean with a 'b', pointers with a 'p'
- each file, class, method and member variable is documented using Doxygen tags
See also http://www.appinf.com/download/CppCodingStyleGuide.pdf for good guidelines
