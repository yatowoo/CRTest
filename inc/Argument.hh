/*
*   File : CRTest/inc/Argument.hh
*   Brief: Handle argumetns for executable file
*   Description:
*		Initilize, build, print and some exception or warnnings
*/

#ifndef CRTest_Argument_h
#define CRTest_Argument_h

#include "G4String.hh"
#include "G4ios.hh"

#include<cstdlib>

class Argument {
public:
	Argument();
	Argument(int argc, char* argv[]);
	~Argument();
public:
	G4bool Build(int argc, char* argv[]);
	void Print();
	void Usage();
private:	
	void Initialize();
	G4bool Validate();

private:
    G4String gdmlFileName;
    G4String macroFileName;
	G4String rootFileName;
	G4int rndFactor;
  G4int rndSeed;
	G4bool uiUse;
	G4bool visUse;

public: // Getters
	G4String Gdml(){return gdmlFileName;}
	G4String Macro(){return macroFileName;}
	G4String Root(){return rootFileName;}
	G4int RndFactor(){return rndFactor;}
  G4int RndSeed(){return rndSeed;}
	G4bool Ui(){return uiUse;}
	G4bool Vis(){return visUse;}
};

#endif // CRTest_Argument_h