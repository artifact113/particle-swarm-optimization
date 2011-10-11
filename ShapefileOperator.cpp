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
		cout<<"Open file failed"<<endl;
		return 0;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);

	OGRDataSource::DestroyDataSource(poDS);

	return poLayer->GetFeatureCount();

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
		cout<<"Open file failed"<<endl;
		return results;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	poLayer->ResetReading();

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());


	OGRFeature* poFeature;
	while ( (poFeature = poLayer->GetNextFeature()) != NULL )
	{
		string value(poFeature->GetFieldAsString(fieldIndex));
		results.push_back(value);
	}

	OGRFeature::DestroyFeature(poFeature);
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
		cout<<"Open file failed"<<endl;
		return "";
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);

	// 获取要素
	OGRFeature* poFeature;
	poFeature = poLayer->GetFeature(featureID);

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.c_str());

	// 获取值
	string value(poFeature->GetFieldAsString(fieldIndex));

	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);

	return value;
}


/****************************************** ShapefileWriter ***********************************/
bool ShapefileWriter::WriteToFile(string filePath, string fieldName, vector<string> &values)
{

	return true;
}