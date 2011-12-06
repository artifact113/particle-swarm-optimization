#include "ShapefileOperator.h"
#include <vector>
#include <QString>
#include "ogrsf_frmts.h"
using namespace std;


/****************************************** ShapefileReader ***********************************/
int ShapefileReader::GetFeatureCount(const QString &fileName)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData());
    if(!poDS)
    {
		return 0;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if(!poLayer)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return 0;
	}

	int result = poLayer->GetFeatureCount();

	OGRDataSource::DestroyDataSource(poDS);

	return result;
}


vector<QString> ShapefileReader::GetFieldValues(const QString &fileName, const QString &fieldName)
{
	vector<QString> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData());
    if(!poDS)
    {
		return results;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if(!poLayer)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return results;
	}
	int featureCount = poLayer->GetFeatureCount();

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.toUtf8().constData());
	if (fieldIndex == -1)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return results;
	}

	poLayer->ResetReading();
	OGRFeature* poFeature;
	while ((poFeature = poLayer->GetNextFeature()))
	{
		QString value(poFeature->GetFieldAsString(fieldIndex));
		results.push_back(value);
		OGRFeature::DestroyFeature(poFeature);
	}
	
	OGRDataSource::DestroyDataSource(poDS);

	return results;
}


QString ShapefileReader::GetValue(const QString &fileName, const QString &fieldName, long featureID)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData());
    if(!poDS)
    {
		return "";
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if(!poLayer)
    {
		OGRDataSource::DestroyDataSource(poDS);
		return "";
    }

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.toUtf8().constData());
	if (fieldIndex == -1)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return "";
	}

	// 获取要素
	OGRFeature* poFeature;
	poFeature = poLayer->GetFeature(featureID);
	if (!poFeature)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return "";
	}

	// 获取值
	QString result(QString::fromUtf8(poFeature->GetFieldAsString(fieldIndex)));

	OGRFeature::DestroyFeature(poFeature);
	OGRDataSource::DestroyDataSource(poDS);

	return result;
}


vector<vector<int>> ShapefileReader::GetAdjacency(const QString &fileName)
{
	vector<vector<int>> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
    OGRDataSource* poDS;
    poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData());
    if(!poDS)
    {
		return results;
    }

	// 获取图层
    OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if(!poLayer)
    {
		OGRDataSource::DestroyDataSource(poDS);
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

	OGRDataSource::DestroyDataSource(poDS);
	return results;
}


// 获取所有的字段名
vector<QString> ShapefileReader::GetFieldNames(const QString &fileName)
{
	vector<QString> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
	OGRDataSource* poDS;
	poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData());
	if(!poDS)
	{
		return results;
	}

	// 获取图层
	OGRLayer* poLayer;
	poLayer = poDS->GetLayer(0);
	if(!poLayer)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return results;
	}

	// 获取字段
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	for (int i=0; i != poFeatureDefn->GetFieldCount(); ++i)
	{
		OGRFieldDefn* poFieldDefn = poFeatureDefn->GetFieldDefn(i);
		QString fieldName(QString::fromUtf8(poFieldDefn->GetNameRef()));
		results.push_back(fieldName);
	}

	OGRDataSource::DestroyDataSource(poDS);
	return results;
}


// 获取指定文件、指定字段的唯一值
vector<QString> ShapefileReader::GetUniqueValues(const QString &fileName, const QString &fieldName)
{
	vector<QString> results;

	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
	OGRDataSource* poDS;
	poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData(), true);
	if(!poDS)
	{
		return results;
	}

	// 获取图层
	OGRLayer* poLayer;
	poLayer = poDS->GetLayer(0);
	if(!poLayer)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return results;
	}

	QString layerName(QString::fromUtf8(poLayer->GetName()));

	QString strSQL = "SELECT DISTINCT " + fieldName + " FROM " + layerName;

	OGRLayer* myLayer = poDS->ExecuteSQL(strSQL.toUtf8().constData(), NULL, NULL);

	myLayer->ResetReading();
	OGRFeature* myFeature;
	while(myFeature = myLayer->GetNextFeature())
	{
		results.push_back(QString::fromUtf8(myFeature->GetFieldAsString(0)));
		OGRFeature::DestroyFeature(myFeature);
	}
	poDS->ReleaseResultSet(myLayer);

	OGRDataSource::DestroyDataSource(poDS);
	return results;
}


/****************************************** ShapefileWriter ***********************************/
bool ShapefileWriter::WriteToFile(const QString &fileName, const QString &fieldName, const vector<QString> &values)
{
	// 注册驱动器
	OGRRegisterAll();

	// 获取数据源
	OGRDataSource* poDS;
	poDS = OGRSFDriverRegistrar::Open(fileName.toUtf8().constData(), true);
	if(!poDS)
    {
		return false;
    }
	
	// 获取图层
	OGRLayer* poLayer;
    poLayer = poDS->GetLayer(0);
	if( poLayer == NULL )
    {
		OGRDataSource::DestroyDataSource(poDS);
		return false;
    }

	// 查找字段是否已存在
	OGRFeatureDefn* poFeatureDefn;
	poFeatureDefn = poLayer->GetLayerDefn();
	int fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.toUtf8().constData());
	if (fieldIndex != -1)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return false;
	}

	// 创建字段
	OGRFieldDefn poFieldDefn(fieldName.toUtf8().constData(), OFTString);
	poFieldDefn.SetWidth(32);
	if(poLayer->CreateField(&poFieldDefn) != OGRERR_NONE)
	{
		OGRDataSource::DestroyDataSource(poDS);
		return false;
	}

	// 写入值
	fieldIndex = poFeatureDefn->GetFieldIndex(fieldName.toUtf8().constData());
	int size = values.size();
	int count = poLayer->GetFeatureCount();
	for (int i=0; i != count; ++i)
	{
		if (i < size)
		{
			OGRFeature* poFeature = poLayer->GetFeature(i);
			poFeature->SetField(fieldIndex, values.at(i).toUtf8().constData());
			poLayer->SetFeature(poFeature);
			OGRFeature::DestroyFeature(poFeature);
		}
		else
		{
			break;
		}
	}

	OGRDataSource::DestroyDataSource(poDS);
	return true;
}