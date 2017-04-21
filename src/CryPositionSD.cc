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

	fEdep->clear();
	fHitTime->clear();
	fHitX->clear();
	fHitY->clear();
	fHitZ->clear();
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

G4bool CryPositionSD::ProcessHits_more
	(const G4Step* theStep, G4VPhysicalVolume* thePV)
{
	// Sensitive only for Primary track
	if(theStep->GetTrack()->GetParentID() != 0)
		return false;
    G4double edep = theStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;

	// fNvolume
	if(fNvolume == 0){
		for(int i = 0 ; 
			i < thePV->GetMotherLogical()->GetNoDaughters() ; i++)
		{
			if(thePV->GetMotherLogical()->GetDaughter(i)->GetName()
				== thePV->GetName())
				fNvolume++;
		}
	}

	CryHit* newHit = new CryHit();
	
	newHit->SetDetectorID(thePV->GetCopyNo());
	newHit->SetEdep(edep);

	G4Track* theTrack = theStep->GetTrack();
	newHit->SetTime(theTrack->GetGlobalTime());
	newHit->SetPosition(theTrack->GetPosition());

    fHC->insert(newHit);

    return true;
}

void CryPositionSD::EndOfEvent(G4HCofThisEvent*){

	std::vector<G4int> count;
   	for(int i = 0 ; i < fNvolume ; i++){
		fEdep->push_back(0.);
		fHitTime->push_back(0.);
		fHitX->push_back(0.);
		fHitY->push_back(0.);
		fHitZ->push_back(0.);
		count.push_back(0);
	}

    for (int i = 0; i < fHC->entries(); i++)
    {
		CryHit* hit = (*fHC)[i];
		G4int id = hit->GetDetectorID();

		count[id] ++;
		(*fEdep)[id] += hit->GetEdep();
		(*fHitTime)[id] += hit->GetTime();

		G4ThreeVector pos = hit->GetPositon();
		(*fHitX)[id] += pos.x();
		(*fHitY)[id] += pos.y();
		(*fHitZ)[id] += pos.z();
    }
   	for(int i = 0 ; i < fNvolume ; i++){
		(*fHitTime)[i] /= count[i];
		(*fHitX)[i] /= count[i];
		(*fHitY)[i] /= count[i];
		(*fHitZ)[i] /= count[i];
	}
}

void CryPositionSD::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID =
		rootData->CreateNtupleDColumn(ntupleID, "sd.Edep", *fEdep);
	rootData->CreateNtupleDColumn(ntupleID, "sd.t", *fHitTime);
	rootData->CreateNtupleDColumn(ntupleID, "sd.x", *fHitX);
	rootData->CreateNtupleDColumn(ntupleID, "sd.y", *fHitY);
	rootData->CreateNtupleDColumn(ntupleID, "sd.z", *fHitZ);
}

void CryPositionSD::FillEntry(
	G4int, G4RootAnalysisManager*)
{}