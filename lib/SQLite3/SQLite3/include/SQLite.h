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
class Row : public std::map<std::string, std::string>{
public:
	bool isEmptySet();
};


class SQLite{
private:
	static std::vector<Row> result;
	sqlite3 *sqlite;
	sqlite3_stmt *statement;
	int bindPos;
	static int callback(void *arg ,int argc, char **argv, char **column);
public:
	SQLite();
	~SQLite();
	void open(std::string db_name);
	std::vector<Row> execute(std::string query);
	void close();
	void startTransaction();
	void commitTransaction();
	void rollbackTransaction();
	void truncate(std::string table_name);
	void prepare(std::string query);
	template<typename T> void bindBlob(T data, int size){
		int status;
		if(this->statement == NULL) throw std::exception("sqlite3 prepare unperformed");
		status = sqlite3_bind_blob(this->statement, this->bindPos, &data, size, SQLITE_TRANSIENT);
		if(status != SQLITE_OK) throw std::exception("sqlite3 bind blob failed");
		this->bindPos++;
	}
	template<typename T> std::vector<T> executeStatement(int column_num = 0){
		int status;
		std::vector<T> results;
		if(this->statement == NULL) throw std::exception("sqlite3 prepare unperformed");
		do{
			status = sqlite3_step(this->statement);
			if(status == SQLITE_ROW){
				T *result = (T*)sqlite3_column_blob(this->statement, column_num);
				results.push_back(*result);
			}
		}while(status != SQLITE_DONE);

		sqlite3_finalize(this->statement);
		this->statement = NULL;
		this->bindPos = 1;

		return results;
	}
};

#endif