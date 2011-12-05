#ifndef SHAPEFILEOPERATOR_H
#define SHAPEFILEOPERATOR_H

#include <vector>
#include <QString>
#include "ogrsf_frmts.h"
using namespace std;


class ShapefileReader
{
public:
	/*********************************************************************************/
	// 图斑数量
	static int GetFeatureCount(const QString &fileName);
	
	/*********************************************************************************/
	// 获取指定文件、指定字段的一列值
	static vector<QString> GetFieldValues(const QString &fileName, const QString &fieldName);

	/*********************************************************************************/
	// 获取指定文件、指定字段、指定记录的值
	static QString GetValue(const QString &fileName, const QString &fieldName,long featureID);

	/*********************************************************************************/
	// 获取所有图斑的邻接矩阵
	static vector<vector<int>> GetAdjacency(const QString &fileName);

	/*********************************************************************************/
	// 获取所有的字段名
	static vector<QString> GetFieldNames(const QString &fileName);

	/*********************************************************************************/
	// 获取指定文件、指定字段的唯一值
	static vector<QString> GetUniqueValues(const QString &fileName, const QString &fieldName);

};


class ShapefileWriter
{
public:
	/*********************************************************************************/
	// 向指定文件、指定字段写值
	static bool WriteToFile(const QString &fileName, const QString &fieldName, const vector<QString> &values);

};


#endif	// SHAPEFILEOPERATOR_H