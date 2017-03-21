/*
*   File : CRTest/inc/PhysicsList.hh
*
*   Brief: Define particle and process in physics simulation
*
*/

#ifndef CRTest_PhysicsList_h
#define CRTest_PhysicsList_h

#include "G4VModularPhysicsList.hh"




class PhysicsList : public G4VModularPhysicsList {

public:
    PhysicsList();
    virtual ~PhysicsList();
private:
};

#endif /*CRTest_PhysicsList_h*/