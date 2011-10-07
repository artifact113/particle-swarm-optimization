#include "ShapefileOperator.h"
#include <iostream>
#include <string>
#include "ogrsf_frmts.h"

using namespace std;


/****************************************** ShapefileReader ***********************************/

ShapefileReader::ShapefileReader(string fileName)
{
	FilePath = fileName;
}


ShapefileReader::~ShapefileReader()
{


}


int ShapefileReader::GetFeatureCount()
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(FilePath.c_str());
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


string ShapefileReader::GetFieldValue(string fieldName, long featureID)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(FilePath.c_str());
    if( poDS == NULL )
    {
		cout<<"Open file failed"<<endl;
		return NULL;
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


OGRPolygon* ShapefileReader::GetPolygon(long featureID)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(FilePath.c_str());
    if( poDS == NULL )
    {
		cout<<"Open file failed"<<endl;
		return NULL;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);

	// 获取要素
	OGRFeature* poFeature;
	poFeature = poLayer->GetFeature(featureID);

	// 获取几何对象
	OGRGeometry* poGeometry;
	poGeometry = poFeature->GetGeometryRef();
	OGRPolygon* poPolygon = new OGRPolygon;
	if( poGeometry != NULL && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon )
	{
		poPolygon = (OGRPolygon*)poGeometry;
	}
	else
	{
		cout<<"No polygon geometry!"<<endl;
		poPolygon = NULL;
	}

	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);

	return poPolygon;
}


/****************************************** ShapefileWriter ***********************************/