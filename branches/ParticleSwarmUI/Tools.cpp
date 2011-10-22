#include "Tools.h"
#include <cstdlib>
#include <ctime>

using namespace std;


Random::Random()
{
	state = GetCycleCount();
    if( state == 0 )
	{
        state = 1;
	}

}


double Random::NextDouble()
{
	long int tmpState = A*( state%Q ) - R*( state/Q );

    if( tmpState >= 0 )
        state = tmpState;
    else
        state = tmpState + M;
   
	return (double)state/M;
}