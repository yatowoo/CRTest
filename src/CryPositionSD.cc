/**
*   FILE : CRTest/src/CryPositionSD.cc
*   Brief: Implementation of class CryPositionSD
*/

#include "CryPositionSD.hh"

#include "G4SDManager.hh"

#include "G4String.hh"
#include "G4ios.hh"

CryPositionSD::CryPositionSD(G4String &name)
	: VirtualSD(name), fHCID(0), fFirstColID(-1),
	fEdep(NULL)
{
    collectionName.insert("CryHC");

	fEdep = new std::vector<double>;
}

CryPositionSD::~CryPositionSD()
{
	delete fEdep;
}

void CryPositionSD::Initialize(G4HCofThisEvent *hce)
{
    G4cout << "[-] INFO - CryPositionSD Initialized."
           << " - by CryPositionSD" << G4endl;
    fHC = new CryHC(SensitiveDetectorName, collectionName[0]);

    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(fHCID, fHC);

	if(fEdep) 
		fEdep->clear();
	else
		fEdep = new std::vector<double>;
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

void CryPositionSD::EndOfEvent(G4HCofThisEvent* hce){
    CryHC *cryHC = static_cast<CryHC *>(hce->GetHC(fHCID));

    G4double sdEdep = 0.;
    for (int i = 0; i < cryHC->entries(); i++)
    {
        sdEdep += (*cryHC)[i]->GetEdep();
    }

	fEdep->push_back(sdEdep);
}

void CryPositionSD::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID =
		rootData->CreateNtupleDColumn(ntupleID, "sd.Edep", *fEdep);
}

void CryPositionSD::FillEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{}