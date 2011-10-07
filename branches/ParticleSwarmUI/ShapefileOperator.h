#ifndef SHAPEFILEOPERATOR_H
#define SHAPEFILEOPERATOR_H

#include <string>
#include "ogrsf_frmts.h"

using namespace std;


class ShapefileReader
{
private:
	string FilePath;							// shp文件路径

public:
	/****************************************************************/
	int GetFeatureCount();						// 图斑数量

	/****************************************************************/
	// 获取指定记录、指定字段的值
	string GetFieldValue(string fieldName,long featureID);

	/****************************************************************/
	OGRPolygon* GetPolygon(long featureID);	// 获取图斑

	/****************************************************************/
	ShapefileReader(string fileName);			// 构造函数
	~ShapefileReader();							// 析构函数

};

class ShapefileWriter
{


};


#endif	// SHAPEFILEOPERATOR_H