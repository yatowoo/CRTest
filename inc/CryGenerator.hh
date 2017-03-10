/*
*   File : CRTest/inc/CryGenerator.hh
*
*   Brief: Generator with CRY@LLNL package
*
*/

#ifndef CRTest_CryGenerator_h
#define CRTest_CryGenerator_h

#include "Generator.hh"

#include "CRYSetup.h"
#include "CRYUtils.h"
#include "CRYParticle.h"
#include "CRYGenerator.h"

#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4Event.hh"
#include "G4String.hh"

#include<vector>

class CryGenerator : public Generator {

public:
    CryGenerator(G4String);
    virtual ~CryGenerator();
public:
    virtual void GeneratePrimaries(G4Event* anEvent);
public:
    // For Messenger
    void InputCRY();
    void UpdateCRY(G4String);
    void CRYFromFile(G4String);

private:
    G4ParticleGun* fParticleGun;
    G4ParticleTable* fParticleTable;
    
    G4int fInputState;
    CRYGenerator* fCryGenerator;
    std::vector<CRYParticle*> *fParticles;
};

#endif /*CRTest_CryGenerator_h*/