# Copyright (c) 2014 Sébastien Rombauts (sebastien.rombauts@gmail.com)
# 
# Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt
# or copy at http://opensource.org/licenses/MIT)

mkdir -p build
cd build
# generate solution for GCC
cmake ..
cmake --build .

# launch tests
ctest --output-on-failure
