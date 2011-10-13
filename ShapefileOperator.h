#ifndef SHAPEFILEOPERATOR_H
#define SHAPEFILEOPERATOR_H

#include <vector>
#include <string>
#include "ogrsf_frmts.h"

using namespace std;


class ShapefileReader
{
public:
	/*********************************************************************************/
	// 图斑数量
	static int GetFeatureCount(string filePath);		
	
	/*********************************************************************************/
	// 获取指定文件、指定字段的一列值
	static vector<string> GetFieldValues(string filePath, string fieldName);

	/*********************************************************************************/
	// 获取指定文件、指定字段、指定记录的值
	static string GetValue(string filePath, string fieldName,long featureID);

	/*********************************************************************************/
	// 获取所有图斑的邻接矩阵
	static vector<vector<int>> GetAdjacency(string filePath);

};

class ShapefileWriter
{
public:
	/*********************************************************************************/
	// 向指定文件、指定字段写值
	static bool WriteToFile(string filePath, string fieldName, vector<string> &values);

};


#endif	// SHAPEFILEOPERATOR_H