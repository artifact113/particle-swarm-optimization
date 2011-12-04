#include "ShapefileOperator.h"
#include <iostream>
#include <vector>
#include <string>
#include "ogrsf_frmts.h"

using namespace std;


/****************************************** ShapefileReader ***********************************/
int ShapefileReader::GetFeatureCount(string filePath)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(filePath.c_str());
    if( poDS == NULL )
    {
		return 0;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);

	int result = poLayer->GetFeatureCount();

	OGRDataSource::DestroyDataSource(poDS);

	return result;
}


vector<string> ShapefileReader::GetFieldValues(string filePath, string fieldName)
{
	vector<string> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(filePath.c_str());
    if( poDS == NULL )
    {
		return results;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	int featureCount = poLayer->GetFeatureCount();

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());
	if (fieldIndex == -1)
	{
		results.assign(featureCount, "");
		return results;
	}


	poLayer->ResetReading();
	OGRFeature* poFeature;
	while ( (poFeature = poLayer->GetNextFeature()) != NULL )
	{
		string value(poFeature->GetFieldAsString(fieldIndex));
		results.push_back(value);
		OGRFeature::DestroyFeature(poFeature);
	}
	
	OGRDataSource::DestroyDataSource(poDS);

	return results;
}


string ShapefileReader::GetValue(string filePath, string fieldName, long featureID)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(filePath.c_str());
    if( poDS == NULL )
    {
		return "";
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if( poLayer == NULL )
    {
		return "";
    }

	// 获取要素
	OGRFeature* poFeature;
	poFeature = poLayer->GetFeature(featureID);

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());
	if (fieldIndex == -1)
	{
		return "";
	}

	// 获取值
	string value(poFeature->GetFieldAsString(fieldIndex));

	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);

	return value;
}


vector<vector<int>> ShapefileReader::GetAdjacency(string filePath)
{
	vector<vector<int>> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(filePath.c_str());
    if( poDS == NULL )
    {
		return results;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if( poLayer == NULL )
    {
		return results;
    }

	int featureCount = poLayer->GetFeatureCount();
	vector<int> temp;
	results.assign(featureCount, temp);
	
	for (int i=0; i != featureCount; ++i)
	{
		OGRFeature* poFeature = poLayer->GetFeature(i);
		OGRGeometry* poGeometry = poFeature->GetGeometryRef();		
		for (int j=i+1; j != featureCount; ++j)
		{
			OGRFeature* otherFeature = poLayer->GetFeature(j);
			OGRGeometry* otherGeometry = otherFeature->GetGeometryRef();
			if (poGeometry->Touches(otherGeometry))
			{
				results.at(i).push_back(j);
				results.at(j).push_back(i);
			}
			OGRFeature::DestroyFeature(otherFeature);
		}		
		OGRFeature::DestroyFeature(poFeature);
	}

	OGRDataSource::DestroyDataSource( poDS );
	return results;
}


// 获取所有的字段名
vector<string> ShapefileReader::GetFieldNames(string filePath)
{
	vector<string> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
	OGRDataSource* poDS;
	poDS = OGRSFDriverRegistrar::Open( filePath.c_str(), true );
	if(!poDS)
	{
		return results;
	}

	// 获取图层
	OGRLayer* poLayer;
	poLayer = poDS->GetLayer(0);
	if(!poLayer)
	{
		return results;
	}

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	for (int i=0; i != poFeatureDefn->GetFieldCount(); ++i)
	{
		OGRFieldDefn* poFieldDefn = poFeatureDefn->GetFieldDefn(i);
		string strFieldName(poFieldDefn->GetNameRef());
		results.push_back(strFieldName);
	}

	OGRDataSource::DestroyDataSource( poDS );	
	return results;
}

/****************************************** ShapefileWriter ***********************************/
bool ShapefileWriter::WriteToFile(string filePath, string fieldName, vector<string> &values)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
	OGRDataSource* poDS;
	poDS = OGRSFDriverRegistrar::Open( filePath.c_str(), true );
	if( poDS == NULL )
    {
		return false;
    }
	
	// 获取图层
	OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if( poLayer == NULL )
    {
		return false;
    }

	// 查找字段是否已存在
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());
	if (fieldIndex == -1)
	{
		return false;
	}

	// 创建字段
	OGRFieldDefn poFieldDefn(fieldName.c_str(), OFTString);
	poFieldDefn.SetWidth(32);
	if(poLayer->CreateField(&poFieldDefn) != OGRERR_NONE)
	{
		return false;
	}

	// 写入值
	fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());
	int size = values.size();
	int count = poLayer->GetFeatureCount();
	for (int i=0; i != count; ++i)
	{
		if (i < size)
		{
			OGRFeature* poFeature = poLayer->GetFeature(i);
			poFeature->SetField(fieldIndex, values.at(i).c_str());
			poLayer->SetFeature(poFeature);
			OGRFeature::DestroyFeature(poFeature);
		}
	}

	OGRDataSource::DestroyDataSource( poDS );
	return true;
}