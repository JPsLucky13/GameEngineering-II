/*
The main() function is where the program starts execution
*/

// Include Files
//==============

#include "cMeshBuilder.h"

// Entry Point
//============

int main(int i_argumentCount, char** i_arguments)
{
	return eae6320::Assets::Build<eae6320::Assets::cMeshBuilder>(i_arguments, i_argumentCount);
}