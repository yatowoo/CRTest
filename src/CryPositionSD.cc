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

G4bool CryPositionSD::ProcessHits(G4Step *theStep, G4TouchableHistory *)
{
	// Sensitive only for Primary track
	if(theStep->GetTrack()->GetParentID() != 0)
		return false;
    G4double edep = theStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;

	if(!fNphysvol)
		CalculateNoPhysvols(theStep->GetPreStepPoint());
    
    CryHit* newHit = new CryHit();
    newHit->SetEdep(edep);
	newHit->SetDetectorID(CalculateCopyNo(theStep->GetPreStepPoint()));

    fHC->insert(newHit);

    return true;
}

void CryPositionSD::EndOfEvent(G4HCofThisEvent*){

   	for(int i = 0 ; i < fNvolume ; i++)
		fEdep->push_back(0.);

    for (int i = 0; i < fHC->entries(); i++)
    {
		(*fEdep)[(*fHC)[i]->GetDetectorID()]
			+= (*fHC)[i]->GetEdep();
    }

}

void CryPositionSD::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID =
		rootData->CreateNtupleDColumn(ntupleID, "sd.Edep", *fEdep);
}

void CryPositionSD::FillEntry(
	G4int, G4RootAnalysisManager*)
{}