#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "types.h"
#include <iostream>

using namespace pathfinder_combat_simulator;

const string SqlTableName = "AttackScoreCalculation";

void print_error(SQLHANDLE handle, SQLSMALLINT handleType) {
	SQLSMALLINT i = 0;
	SQLINTEGER nativeError;
	SQLWCHAR state[7];
	SQLWCHAR text[256];
	SQLSMALLINT len;
	SQLRETURN ret;

	std::cerr << "Diagnostics:" << std::endl;
	do {
		ret = SQLGetDiagRec(handleType, handle, ++i, state, &nativeError, text, sizeof(text), &len);
		if (SQL_SUCCEEDED(ret))
		{
			std::wcerr << state << ": " << text << " (" << nativeError << ")" << std::endl;
		}
	} while (ret == SQL_SUCCESS);
}

data_access::~data_access() 
{
	close_connection();
}

void data_access::initialize_connection() 
{
	SQLRETURN retcode;

	// Allocate environment handle
	retcode = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		// Set the ODBC version environment attribute
		retcode = SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);

		// Allocate connection handle
		retcode = SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// Set connection string
			auto connStr = std::make_unique<std::string>("DRIVER={ODBC Driver 17 for SQL Server};Server=(localdb)\\MSSQLLocalDB;Database=PCS;Trusted_Connection=yes;");
			const std::string& strConnStr = *connStr;
			std::wstring convertedWstrConnStr(strConnStr.begin(), strConnStr.end());
			wchar_t* wideStrConnStr = const_cast<wchar_t*>(convertedWstrConnStr.c_str());

			retcode = SQLDriverConnect(hDbc, NULL, wideStrConnStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
			if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
				// Handle connection error
				print_error(hDbc, SQL_HANDLE_DBC);
			}
		}
	}
}

void data_access::close_connection() {
	if (hDbc) {
		SQLDisconnect(hDbc);
		SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
		hDbc = NULL;
	}
	if (hEnv) {
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
		hEnv = NULL;
	}
}

// when you rewrite this, just make it write to a CSV file instead of a database.  
void data_access::insert_attack_scenario(shared_ptr<attack_scenario> scenario) {
	_db_mutex->lock();

	SQLHSTMT hStmt = NULL;
	SQLRETURN retcode;

	// Allocate statement handle
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
	if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
		// Prepare the SQL insert statement
		auto sql = std::make_unique<std::string>("INSERT INTO [" + SqlTableName + "] (AttackersAttackBonus, DefendersAC, MinimumCrit, CritMultiplier, AttackRoll, DamageDiceCount, DamageDieMean, ExpectedDamageResult) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
		const std::string& str = *sql;
		std::wstring convertedWstrCommand(str.begin(), str.end());
		wchar_t* wideStrCommand = const_cast<wchar_t*>(convertedWstrCommand.c_str());

		retcode = SQLPrepare(hStmt, wideStrCommand, SQL_NTS);
		if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
			// Bind parameters
			SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->attack_bonus, 0, NULL);
			SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->armor_class, 0, NULL);
			SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->minimum_crit, 0, NULL);
			SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->crit_multiplier, 0, NULL);
			SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->unmodified_attack_roll, 0, NULL);
			SQLBindParameter(hStmt, 6, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &scenario->damage_dice_count, 0, NULL);
			SQLBindParameter(hStmt, 7, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL, 0, 0, &scenario->mean_damage_per_die, 0, NULL);
			SQLBindParameter(hStmt, 8, SQL_PARAM_INPUT, SQL_C_FLOAT, SQL_REAL, 0, 0, &scenario->expected_result, 0, NULL);

			// Execute the statement
			retcode = SQLExecute(hStmt);
			if (!(retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO)) {
				print_error(hStmt, SQL_HANDLE_STMT);
				// Handle error
			}
		}
		// Free statement handle
		SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
	}
	_db_mutex->unlock();
}