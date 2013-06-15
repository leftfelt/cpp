#ifndef INCLUDE_SQLITE
#define INCLUDE_SQLITE

#ifdef _DEBUG
	#pragma comment(lib,"sqlite3d.lib")
#else
	#pragma comment(lib,"sqlite3.lib")
#endif

#include <string>
#include <sqlite3.h>
#include <map>
#include <vector>

//çs
typedef std::map<std::string, std::string> Row;

class SQLite{
private:
	static std::vector<Row> result;
	sqlite3 *sqlite;
	static int callback(void *arg ,int argc, char **argv, char **column);
public:
	SQLite();
	void open(std::string db_name);
	void prepare();
	std::vector<Row> execute(std::string query);
	void close();
	void start_transaction();
	void commit_transaction();
	void rollback_transaction();
};

#endif