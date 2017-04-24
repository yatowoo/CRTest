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
  : G4VUserPrimaryGeneratorAction(), 
  fTriggerMode(false), fXmin(0), fXmax(0), fYmin(0), fYmax(0)
{
    G4int nofParticles = 1;
    fParticleGun = new G4ParticleGun(nofParticles);

    // default particle kinematic
    G4ParticleDefinition *particleDefinition =
        G4ParticleTable::GetParticleTable()->FindParticle("mu-");
    fParticleGun->SetParticleDefinition(particleDefinition);

    Generate();
}

Generator::~Generator()
{
    delete fParticleGun;
	G4cout << "[-] INFO - Generator deleted. " << G4endl;
}

void Generator::Generate()
{
  GeneratePosition();
  GenerateDirection();
  GenerateKineticEnergy();
}

void Generator::GeneratePosition()
{
  G4ThreeVector position = GetWorldBoundary();
  position.setX(0.);
  position.setY(0.);
  
  if(fTriggerMode){
    position.setX(fXmin + (fXmax-fXmin) * G4UniformRand());
    position.setY(fYmin + (fYmax-fYmin) * G4UniformRand());
  }

  position.setZ(-position.z());

  fParticleGun->SetParticlePosition(position);
}
void Generator::GenerateDirection()
{
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0., 0., 1.));
}

void Generator::GenerateKineticEnergy()
{
  fParticleGun->SetParticleEnergy(3.0 * GeV);
}

void Generator::GeneratePrimaries(G4Event *anEvent)
{
  // for beam/optical test - generate position only
  if(fTriggerMode){
    G4int count = 0;
    G4ThreeVector boundary = GetWorldBoundary();
    do{
      GeneratePosition();
      count ++;
    }while(!CheckEndpoint(boundary.z()) && count < 10000);
    if(count > 5000)
      G4cout << "[+] WARNING - BAD trigger efficiency. "
        << "Samples : " << count << "- by Generator " << G4endl;
  }
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

G4bool Generator::CheckEndpoint(G4double boundaryZ)
{
  G4ThreeVector position = fParticleGun->GetParticlePosition();
  G4ThreeVector direction = fParticleGun->GetParticleMomentumDirection();

  G4ThreeVector end = position + (boundaryZ-position.z()) * direction;
  if(end.x() < fXmin || end.x() > fXmax 
    || end.y() < fYmin || end.y() > fYmax)
    return false;
  
  return true;
}

G4ThreeVector Generator::GetWorldBoundary(){
    G4double worldX, worldY, worldZ;
    G4LogicalVolume *worldLV = 
        G4LogicalVolumeStore::GetInstance()->GetVolume("World");
    G4Box *worldBox = NULL;
    if (worldLV)
        worldBox = dynamic_cast<G4Box *>(worldLV->GetSolid());
    if (worldBox){
        worldX = worldBox->GetXHalfLength();
        worldY = worldBox->GetYHalfLength();
        worldZ = worldBox->GetZHalfLength();
    }
    else
    {
        G4cout << "[X] ERROR - World volume of box not found."
        << G4endl <<"\t"
        << "Perhaps you have changed geometry." 
        << G4endl << "\t"
        << "The gun will be place in the center. - by Generator"
        << G4endl;
        return G4ThreeVector(0.,0.,0.);
    }
    return G4ThreeVector(worldX,worldY,worldZ);
}
