// ====================================================================================================================
// Copyright (C) 2015  Lukas Georgieff
// Last modified: 04/11/2015
// Description: Implements the RESTful server for the translation service.
// ====================================================================================================================

// ====================================================================================================================
// This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public
// License as published by the Free Software Foundation in version 2.
//
// This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied
// warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with this program; if not, write to the
// Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
// ====================================================================================================================

#include "../utils/command_line_exception.hpp"
#include "db_exception.hpp"
#include "connection_string.hpp"
#include "../utils/helper.hpp"

#include <pqxx/pqxx>
#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>


using lgeorgieff::translate::server::ConnectionString;
using lgeorgieff::translate::server::DB_Query;
using lgeorgieff::translate::server::DB_Exception;
using lgeorgieff::translate::utils::CommandLineException;
using lgeorgieff::translate::utils::string_to_size_t;

ConnectionString connection_string;
size_t service_port{8885};
std::string service_address{"127.0.0.1"};

// Returns the usage instractions for this programme.
std::string get_usage(const string &programme_name) {
  return programme_name +
         " starts a web service that offers a RESTful API for a\n"
         "translation service\n\n"
         "synopsis:\n"
         "-h | --help                        Prints this dialogue\n"
         "-u | --db-username <username>      Sets the DB user name\n"
         "-c | --db-password <password>      Sets the DB password\n"
         "-d | --db                          Sets the data base name\n"
         "-l | --db-host <host name>         Sets the host name (DNS) of the data\n"
         "                                   base server\n"
         "-a | --db-address <address>        Sets the host address (IP) of the data\n"
         "                                   base server\n"
         "-p | --db-port <port>              Sets the port of the data base server\n"
         "-P | --service-port <port>         Sets the port of this RESTful service\n"
         "-L | --service-address <address>   Sets the host address (IP) of this\n"
         "                                   RESTful service\n";
}

// Processes all command line arguments and sets the corresponding coniguration values.
void process_cmd_arguments(const int argc, const char **argv) {
  bool db_host_set{false};
  bool db_addr_set{false};
  for(int pos{1}; pos < argc; ++pos) {
    if(!strcmp("-h", argv[pos]) || !strcmp("--help", argv[pos])) {
      std::cout << get_usage(argv[0]) << std::endl;
    } else if((!strcmp("-u", argv[pos]) || !strcmp("--db-username", argv[pos])) && pos != argc - 1) {
      connection_string.user(argv[++pos]);
    } else if((!strcmp("-c", argv[pos]) || !strcmp("--db-password", argv[pos])) && pos != argc - 1) {
      connection_string.password(argv[++pos]);
    } else if((!strcmp("-l", argv[pos]) || !strcmp("--db-host", argv[pos])) && pos != argc - 1) {
      if (db_addr_set)
        throw CommandLineException(
            "The option \"-l|--db-host\" cannot be used together with the option \"-a|--db-address\"!");
      // The default setting of ConnectionString is to set the hostaddr value to "127.0.0.1", so we have to unset it
      // before setting the host value.
      connection_string.hostaddr("");
      connection_string.host(argv[++pos]);
      db_host_set = true;
    } else if((!strcmp("-a", argv[pos]) || !strcmp("--db-address", argv[pos])) && pos != argc - 1) {
      if (db_host_set)
        throw CommandLineException(
            "The option \"-a|--db-address\" cannot be used together with the option \"-l|--db-host\"!");
      connection_string.hostaddr(argv[++pos]);
      db_addr_set = true;
    } else if ((!strcmp("-d", argv[pos]) || !strcmp("--db", argv[pos])) && pos != argc - 1) {
      connection_string.dbname(argv[++pos]);
    } else if ((!strcmp("-p", argv[pos]) || !strcmp("--db-port", argv[pos])) && pos != argc - 1) {
      try {
        size_t port{string_to_size_t(argv[++pos])};
        connection_string.port(port);
      } catch (std::invalid_argument err) {
        throw CommandLineException(std::string("The value \"") + argv[pos] + "\" is not a valid port value!");
      }
    } else if((!strcmp("-P", argv[pos]) || !strcmp("--service-port", argv[pos])) && pos != argc - 1) {
      try {
        service_port = string_to_size_t(argv[++pos]);
      } catch (std::invalid_argument err) {
        throw CommandLineException(std::string("The value \"") + argv[pos] + "\" is not a valid port value!");
      }
    } else if((!strcmp("-L", argv[pos]) || !strcmp("--service-address", argv[pos])) && pos != argc - 1) {
      service_address = argv[++pos];
    } else {
      throw CommandLineException(std::string("The option \"") + argv[pos] + "\" is not supported!");
    }
  }
}

// The entry point for this programme.
int main(const int argc, const char **argv) {
  try {
    process_cmd_arguments(argc, argv);
  } catch (CommandLineException err) {
    std::cerr << err.what() << std::endl;
    std::cerr << "Use \"" << argv[0] << " -h\" to see the usage instructions for " << argv[0] << std::endl;
    return 1;
  } catch (DB_Exception err) {
    std::cerr << err.what() << std::endl;
    return 1;
  }

  // TODO: start service
  return 0;
}
