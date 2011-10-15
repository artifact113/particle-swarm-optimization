#ifndef DATATYPEOPERATOR_H
#define DATATYPEOPERATOR_H

#include <vector>
#include <string>  
#include <sstream>
#include <cstdlib>

using namespace std;


class DatatypeOperator
{
public:
	/*******************************************************************/
	/// 释放vector里的内存
	template <typename T>
	static void DestroyVector(vector<T> &a)
	{
		vector<T>().swap(a);
	}

	/*******************************************************************/
	/// int\double->string
	template <typename K>
	static string ConvertTostring(K value)
	{
		stringstream strStream;
		strStream<<value;
		return strStream.str();		
	}

	/*******************************************************************/
	/// string->int
	static int stringToint(string value)
	{
		return atoi(value.c_str());
	}

	/*******************************************************************/
	/// string->double
	static double stringTodouble(string value)
	{
		return atof(value.c_str());
	}

}


#endif // DATATYPEOPERATOR_H