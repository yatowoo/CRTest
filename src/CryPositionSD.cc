/**
*   FILE : CRTest/src/CryPositionSD.cc
*   Brief: Implementation of class CryPositionSD
*/

#include "CryPositionSD.hh"

#include "G4SDManager.hh"

#include "G4String.hh"
#include "G4ios.hh"

CryPositionSD::CryPositionSD(G4String &name)
    : VirtualSD(name), fHCID(0)
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
	// Sensitive only for Primary track
	if(aStep->GetTrack()->GetParentID() != 0)
		return false;
	/* For further : verbose > 1 
    G4cout << "[+] Hit - Processing by CryPositionSD"
           << G4endl;
    */

    G4double edep = aStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;
    
    CryHit* newHit = new CryHit();
    newHit->SetEdep(edep);

    fHC->insert(newHit);

    return true;
}