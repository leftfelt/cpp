#include <SQLite.h>
#include <algorithm>
#include <iostream>

void main(){
	SQLite sqlite;
	std::vector<Row> result;
	sqlite.open("testdb.db");
	sqlite.execute(
		"CREATE TABLE IF NOT EXISTS test ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"data TEXT NOT NULL"
		");"
	);
	
	sqlite.execute("INSERT INTO test(data) VALUES('hoge')");

	result = sqlite.execute("SELECT * FROM test");

	sqlite.close();

	std::for_each(result.begin(), result.end(),[](Row row){
		std::cout << row["id"] << ":" << row["data"] << std::endl;
	});
	getchar();
}