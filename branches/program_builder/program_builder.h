#ifndef PROGRAM_BUILDER_H_
#define PROGRAM_BUILDER_H_

#include "program.h"

class ProgramBuilder {
public:
	virtual void BuildProgram() { }
	virtual void BuildAlgorithm() { }
	virtual void BuildProblem() { }

	virtual Program* GetProgram() { return 0; }
};


#endif // PROGRAM_BUILDER_H_