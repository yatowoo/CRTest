/*
*   File : CRTest/inc/Generator.hh
*
*   Brief: Generate primary event vertex and define messenger commands
*
*/

#ifndef CRTest_Generator_h
#define CRTest_Generator_h

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"

class Generator : public G4VUserPrimaryGeneratorAction {

public:
    Generator();
    virtual ~Generator();
public:
    virtual void GeneratePrimaries(G4Event* anEvent);
private:
    G4ParticleGun* fParticleGun;
};

#endif /*CRTest_Generator_h*/