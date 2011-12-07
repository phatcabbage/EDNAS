#include "Logging/SQLiteManager.h++"

#include <cstdio>
#include <sqlite3.h>

namespace Logging
{
    const char *
    SQLiteManager::DEF_FILE_NAME = "results/slaap.db";

    SQLiteManager::
    SQLiteManager(const char* fileName) 
	: _fileName(fileName),
	  _db(NULL)
    {
	printf("[LSQL]\tCreating SQLiteManager.\n");
	this->openDB();
    }

    SQLiteManager::
    ~SQLiteManager()
    {
	this->closeDB();
    }

    void
    SQLiteManager::
    handleOpenResult(int result)
    {
	printf("[LSQL]\tError opening database: %s\n", sqlite3_errmsg(_db));
    }

    void
    SQLiteManager::
    openDB()
    {
	printf("[LSQL]\tOpening DB (\'%s\').\n", _fileName);
	int openResult = sqlite3_open_v2(_fileName, &_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE, NULL);
	printf("[LSQL]\tsqlite3_open_v2 result: %d\n", openResult);
	if (openResult != SQLITE_OK)
	{
	    _db = NULL;
	    handleOpenResult(openResult);
	}
    }

    void
    SQLiteManager::
    cleanupPreparedStatements()
    {
	sqlite3_stmt* stmt;

	/* we can't call sqlite3_next_stmt on a NULL database pointer */
	if (!_db) return;


	while ((stmt = sqlite3_next_stmt(_db, 0)))
	    sqlite3_finalize(stmt);
    }

    void
    SQLiteManager::
    closeDB()
    {
	printf("[LSQL]\tClosing DB.\n");
	cleanupPreparedStatements();
	sqlite3_close(_db);
	_db = NULL;
	printf("[LSQL]\tClosed DB.\n");
    }
}
