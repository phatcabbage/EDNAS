#ifndef __LOGGING__SQLITE_MANAGER_HXX__
#define __LOGGING__SQLITE_MANAGER_HXX__

struct sqlite3;

namespace Logging 
{
    class SQLiteManager
    {
    private:
	static const char *DEF_FILE_NAME;
	const char* _fileName;
	sqlite3* _db;

	void
	handleOpenResult(int result);
	
	void
	openDB();

	void
	cleanupPreparedStatements();

	void
	closeDB();

    public:
	SQLiteManager(const char *fileName);
	~SQLiteManager();
    };
}
	
#endif // __LOGGING__SQLITE_MANAGER_HXX__
