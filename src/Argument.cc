/**
*   FILE : CRTest/src/Argument.cc
*   Brief: Implementation of class Argument
*/

#include "Argument.hh"

Argument::Argument()
{
	Initialize();
}

Argument::Argument(int argc, char* argv[]){
	Initialize();
	Build(argc, argv);
}

Argument::~Argument(){}

void Argument::Initialize(){
	gdmlFileName = "./mac/default.gdml";
    macroFileName = "./mac/run.mac";
	rootFileName = "CRTest";
	rndFactor = 1;
	uiUse = true;
	visUse = true;
}

G4bool Argument::Build(int argc, char* argv[])
{
	// Handle Arguments
	switch(argc){
	case 5:
		rndFactor = std::atoi(argv[4]) + 1;
	case 4:
		rootFileName = argv[3];
	case 3:
		macroFileName = argv[2];
		visUse = false;
		uiUse = false;
	case 2:
		gdmlFileName = argv[1];
	case 1:break;
	default:
		return false;
	}
#ifndef G4UI_USE
	uiUse = false;
#endif
#ifndef G4VIS_USE
	visUse = false;
#endif

	return Validate();
}

void Argument::Usage(){
	G4cout << " Usage : ./CRTest [gdml] [mac] [output] [seed]"
		<< G4endl;
}

G4bool Argument::Validate(){
	// TODO : check arguments / data member effective
	return true;
}

void Argument::Print(){
	// TODO : print arguments info.
}