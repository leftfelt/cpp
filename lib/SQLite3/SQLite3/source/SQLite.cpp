#include <SQLite.h>

std::vector<Row> SQLite::result;

int SQLite::callback(void *arg ,int argc, char **argv, char **column){
	Row row;
	for(int i = 0 ; i < argc ; i++) {
		row[column[i]] = (argv[i] != NULL) ? argv[i] : "NULL";
	}
	SQLite::result.push_back(row);
	return SQLITE_OK;
}

SQLite::SQLite(){
	this->sqlite = NULL;
}

void SQLite::open(std::string db_name){
	int status;
	status = sqlite3_open(db_name.c_str(),&this->sqlite);
	if(status != SQLITE_OK) throw std::exception(std::string("sqlite3 open failed '"+db_name+"'").c_str());
}

void SQLite::prepare(){
}

std::vector<Row> SQLite::execute(std::string query){
	int status;
	char *errmsg;
	//Œ‹‰Ê‚ðƒŠƒZƒbƒg‚·‚é
	SQLite::result.clear();
	status = sqlite3_exec(this->sqlite,query.c_str(),callback,NULL,&errmsg);
	if(status != SQLITE_OK) throw std::exception(std::string(query + "\n" + errmsg).c_str());
	return SQLite::result;
}

void SQLite::close(){
	int status;
	status = sqlite3_close(this->sqlite);
	if(status != SQLITE_OK) throw std::exception(std::string("sqlite3 close failed").c_str());
}

void SQLite::start_transaction(){
	this->execute("BEGIN;");
}

void SQLite::commit_transaction(){
	this->execute("COMMIT;");
}

void SQLite::rollback_transaction(){
	this->execute("ROLLBACK;");
}