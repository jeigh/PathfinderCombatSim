#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include "types.h"

using namespace pathfinder_combat_simulator;

const string SqlTableName = "your_table";
const string SqlServerName = "your_server";
const string SqlDatabaseName = "your_database";
const string SqlUserName = "your_username";
const string SqlPassword = "your_password";


void data_access::insert_payload(unique_ptr<data_collection_payload> payload)
{
	SQLHENV hEnv = NULL;
	SQLHDBC hDbc = NULL;
	SQLHSTMT hStmt = NULL;
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
			auto connStr = make_unique<std::string>("DRIVER={SQL Server};SERVER=" + SqlServerName + ";DATABASE=" + SqlDatabaseName + ";UID=" + SqlUserName + ";PWD=" + SqlPassword + ";");
			const std::string& strConnStr = *connStr;
			std::wstring convertedWstrConnStr(strConnStr.begin(), strConnStr.end());
			wchar_t* wideStrConnStr = const_cast<wchar_t*>(convertedWstrConnStr.c_str());

			retcode = SQLDriverConnect(hDbc, NULL, wideStrConnStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

			if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
				// Allocate statement handle
				retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt);
				if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {

					// Prepare the SQL insert statement
					auto sql = make_unique<string>("INSERT INTO [" + SqlTableName + "] (AttackersAttackBonus, DefendersAC, MinimumCrit, CritMultiplier, AttackRoll) VALUES (?, ?, ?, ?, ?)");
					const std::string& str = *sql;
					std::wstring convertedWstrCommand(str.begin(), str.end());
					wchar_t* wideStrCommand = const_cast<wchar_t*>(convertedWstrCommand.c_str());

					retcode = SQLPrepare(hStmt, wideStrCommand, SQL_NTS);

					if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
						// Bind parameters
						SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &payload->attackers_momentary_attack_bonus, 0, NULL);
						SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &payload->defenders_momentary_ac, 0, NULL);
						SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &payload->attackers_weapons_minimum_crit, 0, NULL);
						SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &payload->attackers_weapons_crit_multipler, 0, NULL);
						SQLBindParameter(hStmt, 5, SQL_PARAM_INPUT, SQL_C_LONG, SQL_INTEGER, 0, 0, &payload->unmodified_attack_roll, 0, NULL);

						// Execute the statement
						retcode = SQLExecute(hStmt);
						if (retcode == SQL_SUCCESS || retcode == SQL_SUCCESS_WITH_INFO) {
							// Row inserted successfully
						}
						else {
							// Handle error
						}
					}
					// Free statement handle
					SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
				}
				// Disconnect and free connection handle
				SQLDisconnect(hDbc);
				SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
			}
		}
		// Free environment handle
		SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
	}
}
