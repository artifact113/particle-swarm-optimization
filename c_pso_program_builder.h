#ifndef C_PSO_PROGRAM_BUILDER_H_
#define C_PSO_PROGRAM_BUILDER_H_

#include "program_builder.h"


class CPSOProgramBuilder : public ProgramBuilder {
public:
	CPSOProgramBuilder();

	virtual void BuildProgram();
	virtual void BuildAlgorithm();
	virtual void BuildProblem();

	virtual Program* GetProgram();

private:
	Program* _program;
};

#endif // C_PSO_PROGRAM_BUILDER_H_