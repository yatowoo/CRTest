/**
*   FILE : CRTest/src/Argument.cc
*   Brief: Implementation of class Argument
*/

#include "Argument.hh"

#ifdef __unix
#include "unistd.h"
#include "time.h"
#else
#include "direct.h"
#endif

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
	char buf[1024];
	G4String dir = getcwd(buf, 1024);
	gdmlFileName = dir + "/mac/default.gdml";
    macroFileName = dir + "/mac/run.mac";
	rootFileName = dir + "/CRTest.root";
	rndFactor = 1;
#ifdef __unix
   rndSeed = time(NULL) * rndFactor;
#else
   rndSeed = 1.;
#endif
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
	G4cout << " Usage : ./CRTest [.gdml] [.mac] [output] [seed]"
		<< G4endl;
}

G4bool Argument::Validate(){
	// TODO : check arguments / data member effective
	if(!gdmlFileName.contains(".gdml"))
		return false;
	if(!macroFileName.contains(".mac"))
		return false;
	if(!rootFileName.contains(".root"))
		rootFileName.append(".root");
	return true;
}

void Argument::Print(){
	G4cout << "[-] CONFIG - Arguments set by user : " << G4endl
		<< " | + GDML File : " << gdmlFileName << G4endl
		<< " | + Execute File : " << macroFileName << G4endl
		<< " | + ROOT File : " << rootFileName << G4endl
    << " | + Random Seed : " << rndSeed << G4endl
		<< " | + Random Seed Factor : " << rndFactor << G4endl
		<< " | + Enable UI : " << (uiUse?"True":"False") << G4endl
		<< " | + Enable Visualization : " << (visUse?"True":"False") << G4endl
		<< G4endl;
}