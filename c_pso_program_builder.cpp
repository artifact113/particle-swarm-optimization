#include "c_pso_program_builder.h"
#include "c_pso_algorithm.h"

CPSOProgramBuilder::CPSOProgramBuilder() {
	_program = NULL;
}


void CPSOProgramBuilder::BuildProgram() {
	_program = new Program();
}

void CPSOProgramBuilder::BuildAlgorithm() {
	Algorthm* p = new CPSO()
	_program->SetAlgorithm();
}