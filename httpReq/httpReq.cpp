// httpReq.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

void populateDB(string message);

int main()
{
	const string url = "http://protein.azurewebsites.net/Search/Single";

	curlpp::Cleanup cleaner;
	curlpp::Easy request;

	while (1) {
		try {
			request.setOpt(new curlpp::options::Url(url));
			ostringstream ss;
			ss << request;
			populateDB("success");
		}
		catch (exception e)
		{
			populateDB(e.what());
		}
		Sleep(1000 * 60 * 19);
	}
}


void populateDB(string message)
{
	char *errmsg = NULL;
	PGconn     *conn;
	PGresult   *res;

	auto t = time(nullptr);
	auto tm = *localtime(&t);

	conn = PQconnectdb("host=localhost port=5432 dbname=postgres user=yu password=yu");
	if (PQstatus(conn) != CONNECTION_OK)
	{
		std::cout << PQerrorMessage(conn) << '\n';
		return;
	}
	const char* paramVal[1];
	paramVal[0] = message.c_str();
	std::cout << put_time(&tm, "%Y-%m-%d %H:%M:%S") << '\t' << message;
	res = PQexecParams(conn,
		"insert into httpReq (msg) values($1);",
		1,
		NULL,
		paramVal,
		NULL,
		NULL,
		0
	);
	std::cout << PQerrorMessage(conn) << '\n';
	PQclear(res);
	if (errmsg)
		std::cout << errmsg << '\n';
	PQfinish(conn);
}
