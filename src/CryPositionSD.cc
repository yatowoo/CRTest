/**
*   FILE : CRTest/src/CryPositionSD.cc
*   Brief: Implementation of class CryPositionSD
*/

#include "CryPositionSD.hh"

#include "G4SDManager.hh"

#include "G4String.hh"
#include "G4ios.hh"

CryPositionSD::CryPositionSD(G4String &name)
    : G4VSensitiveDetector(name), fHCID(0)
{
    collectionName.insert("CryHC");
}

CryPositionSD::~CryPositionSD()
{
}

void CryPositionSD::Initialize(G4HCofThisEvent *hce)
{
    G4cout << "[-] INFO - CryPositionSD Initialized."
           << " - by CryPositionSD" << G4endl;
    fHC = new CryHC(SensitiveDetectorName, collectionName[0]);

    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(fHCID, fHC);
}

G4bool CryPositionSD::ProcessHits(G4Step *aStep, G4TouchableHistory *roHist)
{
    G4cout << "[+] Hit - Processing by CryPositionSD"
           << G4endl;

    return true;
}