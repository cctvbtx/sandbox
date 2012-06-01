//
// StatementExecutor.h
//
// $Id: //poco/1.4/Data/PgSQL/include/Poco/Data/PgSQL/StatementExecutor.h#1 $
//
// Library: Data
// Package: PgSQL
// Module:  StatementExecutor
//
// Definition of the StatementExecutor class.
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Data_PgSQL_StatementHandle_INCLUDED
#define Data_PgSQL_StatementHandle_INCLUDED

#include <libpq-fe.h>
#include <pg_config.h>
#include "Poco/Data/PgSQL/PgSQLException.h"

#include <vector>

namespace Poco {
namespace Data {
namespace PgSQL {

class StatementExecutor
	/// PgSQL statement executor.
{
public:
	enum State
	{
		STMT_INITED,
		STMT_COMPILED,
		STMT_EXECUTED
	};

	explicit StatementExecutor(PgSQL* conn);
		/// Creates the StatementExecutor.

	~StatementExecutor();
		/// Destroys the StatementExecutor.

	int state() const;
		/// Returns the current state.

	void prepare(const std::string& query, PgSQL_BIND* params, std::size_t count);
		/// Prepares the statement for execution.

	void bindResult(PgSQL_BIND* result);
		/// Binds result.

	void execute(PgSQL_BIND* params, std::size_t count);
		/// Executes the statement.

	bool fetch();
		/// Fetches the data.

	bool fetchColumn(std::size_t n, PgSQL_BIND *bind);
		/// Fetches the column.

	operator PgSQL_STMT* ();
		/// Cast operator to native handle type.

private:

	StatementExecutor(const StatementExecutor&);
	StatementExecutor& operator=(const StatementExecutor&);

	static std::string getRandomName();

private:
	pg_conn* _connection;
	pg_result* _result;
	int _state;
	std::string _query;
	std::string _stmtName;
};


//
// inlines
//

inline StatementExecutor::operator PgSQL_STMT* ()
{
	return _result;
}


}}}


#endif // Data_PgSQL_StatementHandle_INCLUDED