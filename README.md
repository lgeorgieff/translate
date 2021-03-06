# translate
A command line translator for Unix-like systems. It consists of a server part (backend) and a client part (frontend).

The backend is built on top of [PostgreSQL](http://www.postgresql.org/ ) and offers a [RESTful](http://en.wikipedia.org/wiki/Representational_state_transfer) API that is used by the frontend to get the requested translations.

The language material is originally taken from http://www.dict.cc/. Unfortunately it is not allowed to share it with the sources, i.e. for getting the language resources it is required to download them from [here](http://www1.dict.cc/translation_file_request.php?l=e).

A final remark: this is just a hobby project to play a little bit around with C++ and Unix. Please don't expect to get entire sentences translated! What you will get is an easy- and fast-to-use command line translator for single words.

# License
This project is open source under the [GNU GENERAL PUBLIC LICENSE Version 2 (GPL v2)](LICENSE.md). Feel free to use this and contribute to this project!

# Building translate
For building translate you need:
* [git](https://git-scm.com/)
* [cmake](http://www.cmake.org/) >= 2.8
* [g++](https://gcc.gnu.org/) or [clang++](http://clang.llvm.org/)
* [PostgreSQL](http://www.postgresql.org/)
* [libcurl](http://curl.haxx.se/libcurl/), in some distrubitions you need the development/dev package of libcurl, e.g. Ubuntu
* [libpqxx](http://pqxx.org/development/libpqxx/), in some distrubitions you need the development/dev package of libpqxx, e.g. Ubuntu
* [google test](https://code.google.com/p/googletest/), if compiling with -DWITH_TESTS=ON
 
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
 1. `./trlt Hallo` # will search for the English translation of the German word "Hallo"

You will get an empty result for the previous example, since you need to create and fill the data bsae with languge data before you can use it.

# Get Language Data
Unfortunately, it is not allowed to share the language data from [dict.cc](http://www.dict.cc/) directly within the source code. This is why we need a separate step to download it.
 1. Visit the [dict.cc](http://www1.dict.cc/translation_file_request.php?l=e) web page
 1. Download all language resources you need (you will get an email containing a download link)
 1. Create the database user translate and the database translate (example for arch linux)
   1. `pacman -S postgresql` # install postgresql
   1. `passwd postgres` # set password for postgres user
   1. `su - postgres` # change user id to postgres
   1. `initdb --locale en_US.UTF-8 -E UTF8 -D '/var/lib/postgres/data'` # create data base files
   1. `systemctl enable postgresql.service` # enable postgersql to be started automatically during startup
   1. `systemctl start postgresql.service` # start postgesql
   1. `createuser --interactive` # create a new database user
   1. `> Enter name of role to add: translate`
   1. `> Shall the new role be a superuser? (y/n) n`
   1. `> Shall the new role be allowed to create databases? (y/n) n`
   1. `> Shall the new role be allowed to create more new roles? (y/n) n`
   1. `createdb translate` # create a new data base
   1. `psql` # connect to data base
   1. `postgres=# GRANT ALL PRIVILEGES ON DATABASE translate to translate;` # give the data base user trnaslate access to the data base translate
   1. `postgres=# \q` # exit
 1. `cd <translate root folder>/src/scripts`
 1. `./process_language_resources.sh -Z <folder path containing downloaded language resources> -d <folder for resulting files>` # This script will unzip and rename all language resources
 1. `./populate_db.sh -d <the folder with the results from the previous step> -t <temporary directory>`
   1. # the need for defining a temporary directory is given since some language files are pretty huge (> 2.0 GB) and tmpfs is not big enough
   1. # if your database user and database name is not translate you can use the -p option for specifying different values
   1. # it will take a while to populate the databse with all language files

# Configuration
The trlt client uses a configuration file. Thus you need to copy the file <translate root folder>/src/client/configuration.json to /home/<user home>/.trlt/configuration.json.

This file contains a json string with some default values, such as the service address and some options. You can modify it to set some individual values and change the default behavior of the frontend.

# RESTful API
The RESTful API is documented in [help.html](https://github.com/lgeorgieff/translate/blob/master/src/server/www/help.html).
