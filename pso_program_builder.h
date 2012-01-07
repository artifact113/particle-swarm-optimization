#ifndef PSO_PROGRAM_BUILDER_H_
#define PSO_PROGRAM_BUILDER_H_

#include "program_builder.h"


class PSOProgramBuilder : public ProgramBuilder {
public:
	PSOProgramBuilder();

	virtual void BuildProgram();
	virtual void BuildAlgorithm();
	virtual void BuildProblem();

	virtual Program* GetProgram();

private:
	Program* _program;
};

#endif // PSO_PROGRAM_BUILDER_H_