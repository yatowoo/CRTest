/**
*   FILE : CRTest/src/CryHit.cc
*   Brief: Implementation of class CryHit
*/

#include "CryHit.hh"

#include "G4VVisManager.hh"
#include "G4UnitsTable.hh"
#include "G4Colour.hh"
#include "G4Circle.hh"
#include "G4VisAttributes.hh"
#include "G4ParticleTable.hh"
#include "G4ios.hh"

CryHit::CryHit()
:   G4VHit(), fTime(0.),
    fPosition(G4ThreeVector()), fMomentum(G4ThreeVector()),
    fParticleID(-1), fDetectorID(-1), fTrackID(-1),
    fEdep(0.), fIonEdep(0.), fStepLength(0.)
{}

CryHit::~CryHit()
{}

void CryHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    // TODO: draw/view pram. & options
    if(pVVisManager){
        G4Circle circle(fPosition);
        circle.SetScreenSize(4.); 
        circle.SetFillStyle(G4Circle::filled);
        G4VisAttributes visAttr(G4Colour::Red);
        circle.SetVisAttributes(visAttr);

        pVVisManager->Draw(circle);
    }
}

void CryHit::Print()
{
    G4String particleName = 
        G4ParticleTable::GetParticleTable()
        ->FindParticle(fParticleID)->GetParticleName();
    G4cout << "TrackID : " << fTrackID << "\tDetectorID :" << fDetectorID
        << G4endl
        << "Particle : " << particleName << G4endl
        << "Position : " << G4BestUnit(fPosition,"Length") << G4endl
        << "Momentum : " << G4BestUnit(fMomentum, "Momentum") << G4endl
        << "Edep : " << G4BestUnit(fEdep,"Energy") << G4endl
        << "Ionization Edep : " << G4BestUnit(fIonEdep,"Energy")
        << G4endl;
}