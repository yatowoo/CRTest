/**
*   FILE : CRTest/src/Generator.cc
*   Brief: Implementation of class Generator
*/

#include "Generator.hh"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"

#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4Box.hh"

#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

Generator::Generator()
    : G4VUserPrimaryGeneratorAction()
{
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);

    // default particle kinematic
    G4ParticleDefinition *particleDefinition =
        G4ParticleTable::GetParticleTable()->FindParticle("proton");
    fParticleGun->SetParticleDefinition(particleDefinition);
    fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
    fParticleGun->SetParticleEnergy(3.0 * GeV);
}

Generator::~Generator()
{
    delete fParticleGun;
}

void Generator::GeneratePrimaries(G4Event *anEvent)
{
    G4double worldZ = 0;
    G4LogicalVolume *worldLV = 
        G4LogicalVolumeStore::GetInstance()->GetVolume("WorldLV");
    G4Box *worldBox = NULL;
    if (worldLV)
        worldBox = dynamic_cast<G4Box *>(worldLV->GetSolid());
    if (worldBox)
        worldZ = worldBox->GetZHalfLength();
    else
    {
        G4cout << "[X] ERROR - World volume of box not found."
        << G4endl <<"\t"
        << "Perhaps you have changed geometry." 
        << G4endl << "\t"
        << "The gun will be place in the center. - by Generator"
        << G4endl;
    }
    fParticleGun->SetParticlePosition(G4ThreeVector(0., 0., -worldZ));

    fParticleGun->GeneratePrimaryVertex(anEvent);
}
