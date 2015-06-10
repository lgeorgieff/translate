# translate
A command line translator for Unix-like systems. It consists of a server part (backend) and a client part (frontend).

The backend is built on top of [PostgreSQL](http://www.postgresql.org/ ) and offers a [RESTful](http://en.wikipedia.org/wiki/Representational_state_transfer) API that is used by the frontend to get the requested translations.

The language material is originally taken from http://www.dict.cc/. Unfortunatelly it is not allowed to share it with the sources, i.e. for getting the language resources it is required to download them from [here](http://www1.dict.cc/translation_file_request.php?l=e).

A final remark: this is just a hobby project to play a little bit around with C++ and Unix. Please don't expect to get entire sentences translated! What you will get is an easy- and fast-to-use command line translator for single words.

# License
This project is open source under the [GNU GENERAL PUBLIC LICENSE Version 2 (GPL v2)](LICENSE.md). Feel free to use this and contribute to this project!

# Building translate
For building translate you need:
* git
* [cmake](http://www.cmake.org/) >= 2.8
* [g++](https://gcc.gnu.org/) or [clang++](http://clang.llvm.org/)
* [PostgreSQL](http://www.postgresql.org/)
* [libcurl](http://curl.haxx.se/libcurl/), in some distrubitions you need the development/dev package of libcurl, e.g. Ubuntu
* [libpqxx](http://pqxx.org/development/libpqxx/), in some distrubitions you need the development/dev package of libpqxx, e.g. Ubuntu
 
To build it run:
 1. `git https://github.com/lgeorgieff/translate.git translate`
 1. `cd translate`
 1. `git submodule init`
 1. `git submodule update`
 1. `mkdir build`
 1. `cd build`
 1. `cmake .. -DWITH_TESTS=ON` # compiles also unit tests provided by translate # For additional cmake flags open the file translte/CMakeLists.txt
 1. `make -j 4`
 1. `cd ../bin`
 1. `./trlt.service -P 8885 -L localhost`
 1. #open a new terminal
 1. `cd <translate roo folder>/bin/`
 2. `./trlt Hallo` # will search for the English translation of the German word "Hallo"
