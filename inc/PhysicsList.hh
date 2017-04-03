/*
*   File : CRTest/inc/PhysicsList.hh
*
*   Brief: Define particle and process in physics simulation
*
*/

#ifndef CRTest_PhysicsList_h
#define CRTest_PhysicsList_h

#include "G4VModularPhysicsList.hh"

#include "G4VPhysicsConstructor.hh"

class PhysicsList : public G4VModularPhysicsList {

public:
    PhysicsList();
    virtual ~PhysicsList();
	void Print();

private:
	G4VPhysicsConstructor* fEmPhys;
	G4VPhysicsConstructor* fOpPhys;
	G4VPhysicsConstructor* fLimiterPhys;
	G4VPhysicsConstructor* fDecayPhys;
};

#endif /*CRTest_PhysicsList_h*/