#ifndef PROGRAM_DIRECTOR_H_
#define PROGRAM_DIRECTOR_H_

#include "program_builder.h"

class ProgramDirector {
public:
	virtual Program* CreateProgram(ProgramBuilder &builder) {}
};


#endif // PROGRAM_DIRECTOR_H_