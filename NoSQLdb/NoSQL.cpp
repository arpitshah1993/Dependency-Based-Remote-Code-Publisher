///////////////////////////////////////////////////////////////
// Cpp11-NoSQL.cpp - NoSQL database system                   //
// ver 1.0                                                   //
// Arpit Shah,  CSE687 - Object Oriented Design, Spring 2017 //
///////////////////////////////////////////////////////////////
#include"NoSQL.h"

#ifdef TEST_NoSQL
int main()
{
	NoSqlDb<string> db("NoSQLdb1");
	db.deleteKey("elem1");
	Element<string> elem1;
	elem1.setName("elem1");
	elem1.setCategory("test1");
	elem1.setData(("123Abc123"));
	elem1.setDescription("Description");
	db.saveOrUpdate(elem1.getName(), elem1);

	cout << "\n Displaying the whole Database:";
	for (Key key : db.keys())
	{
		cout << "\n For Key:" << key;
		cout << db.value(key).displayElementData() << "\n";
	}
	cout << "\n Exporting Database to XML file(Persisting)";
	db.ExportDatabaseToXMLDocument();
}
#endif