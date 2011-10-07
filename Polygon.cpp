#include "Polygon.h"
#include <vector>
#include "ogrsf_frmts.h"

using namespace std;


/*********************************LandUsePolygon****************************/
LandUsePolygon::LandUsePolygon(long id, double area, int oldUseCode)
{
	ID = id;
	Area = area;
	OldUseCode = oldUseCode;
	NewUseCode = oldUseCode;
}


LandUsePolygon::~LandUsePolygon()
{



}







/*********************************LandUseLayer******************************/




/*********************************LayerAssessor*****************************/
