#include <SQLite.h>
#include <algorithm>
#include <iostream>

struct hoge{
	int id;
	double name;
};

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

	sqlite.truncate("test");
	
	sqlite.execute("INSERT INTO test(data) VALUES('hoge')");

	result = sqlite.execute("SELECT * FROM test");


	std::for_each(result.begin(), result.end(),[](Row row){
		std::cout << row["id"] << ":" << row["data"] << std::endl;
	});
	
	//===========================================================================
	sqlite.execute(
		"CREATE TABLE IF NOT EXISTS hoges ("
		"id INTEGER PRIMARY KEY AUTOINCREMENT,"
		"data BLOB NOT NULL);"
	);

	sqlite.truncate("hoges");

	struct hoge h;

	std::cout << sizeof(struct hoge) << std::endl;

	h.id = 11;
	h.name = 1.5;

	try{
		sqlite.prepare("INSERT INTO hoges(data) VALUES(?);");
		sqlite.bindBlob(h, sizeof(h));
		sqlite.executeStatement<struct hoge>();
	}catch(std::exception e){
		std::cout << e.what() << std::endl;
	}

	sqlite.prepare("SELECT data FROM hoges WHERE id = 1;");
	std::vector<struct hoge> results = sqlite.executeStatement<struct hoge>(0);
	std::for_each(results.begin(), results.end(), [&](struct hoge h){
		std::cout << "id:" << h.id << " name:" << h.name << std::endl;
	});

	sqlite.close();

	getchar();
}