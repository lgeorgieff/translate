#include "connection_data.hpp"

#include <pqxx/pqxx>
#include <iostream>
#include <fstream>
#include <string>

#include <cctype>
#include <algorithm>

using lgeorgieff::translate::server::Connection_Data;

using std::cout;
using std::cin;
using std::string;
using std::endl;



std::string read_sql_file() {
	std::ifstream sql("create_schema.sql");
	std::string result;
	if(sql.is_open()) {
		std::string line;
		while(getline(sql, line)){
			result += "\n" + line;
		}
	}
	sql.close();
	return result;
}

int main(const int argc, const char **argv) {
	try {
		Connection_Data connection_data{};
		connection_data.user(argv[1]);
		connection_data.password(argv[2]);
		connection_data.dbname("translate");
		connection_data.hostaddr("127.0.0.1");		
		cout << connection_data << endl;
		
		pqxx::connection c(connection_data.to_string());
		pqxx::work w(c);
		pqxx::result r = w.exec(read_sql_file());
		w.commit();
		std::cout << r.size() << std::endl;
	} catch (const std::exception &e) {
		std::cerr << e.what() << std::endl;
		return 1;
	}


	return 0;
}
