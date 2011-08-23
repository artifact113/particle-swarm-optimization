#include "Tools.h"
#include <cstdlib>
#include <ctime>



Random::Random()
{
	state = (unsigned)time(NULL);
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