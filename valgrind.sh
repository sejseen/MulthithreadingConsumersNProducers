#!/usr/bin/env bash

if [ $1 = "C" ]  
then  
    /usr/bin/clang-3.8 -ggdb main.c -std=c99 -Wall -Werror -o program && /usr/local/bin/valgrind ./program
elif [ $1 = "Cpp" ]  
then  
    /usr/bin/clang++-3.8 -std=c++11 -stdlib=libc++ main.cpp -Wall -Werror -o program && /usr/local/bin/valgrind ./program
fi
