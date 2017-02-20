/*
*   File : CRTest/inc/ParticleGenertaor.hh
*
*   Brief: Generate primary event vertex and define messenger commands
*
*/

#ifndef CRTest_ParticleGenertaor_h
#define CRTest_ParticleGenertaor_h

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"

class ParticleGenertaor : public G4VUserPrimaryGeneratorAction {

public:
    ParticleGenertaor();
    virtual ~ParticleGenertaor();
public:
    virtual void GeneratePrimaries(G4Event* anEvent);
private:
    G4ParticleGun* fParticleGun;
};

#endif /*CRTest_ParticleGenertaor_h*/