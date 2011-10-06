#ifndef SHAPEFILEOPERATOR_H
#define SHAPEFILEOPERATOR_H

#include <string>
#include "ogrsf_frmts.h"

using namespace std;


class ShapefileReader
{
	OGRDataSource* DataSource(string filePath);

	string GetFieldValue(string fieldName);

	OGRPolygon* GetPolygon();

}

class ShapefileWriter
{


}


#endif	// SHAPEFILEOPERATOR_H