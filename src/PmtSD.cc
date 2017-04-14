/**
*   FILE : CRTest/src/PmtSD.cc
*   Brief: Implementation of class PmtSD
*/

#include "PmtSD.hh"

#include "G4SDManager.hh"

#include "G4String.hh"
#include "G4ios.hh"

PmtSD::PmtSD(G4String &name)
	: VirtualSD(name), fHCID(0), fHC(NULL), fFirstColID(-1),
	fCounter(NULL)
{
    collectionName.insert("PmtHC");

	fCounter = new std::vector<int>;

}

PmtSD::~PmtSD()
{
	delete fCounter;

}

void PmtSD::Initialize(G4HCofThisEvent *hce)
{
    G4cout << "[-] INFO - PmtSD Initialized."
           << " - by PmtSD" << G4endl;
    fHC = new PmtHC(SensitiveDetectorName, collectionName[0]);

    fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(collectionName[0]);

    hce->AddHitsCollection(fHCID, fHC);

	fCounter->clear();
	fHitCopyNo->clear();
	fHitEk->clear();
	fHitTime->clear();
	fHitX->clear();
	fHitY->clear();
	fHitZ->clear();
	fHitPX->clear();
	fHitPY->clear();
	fHitPZ->clear();
}

G4bool PmtSD::ProcessHits(G4Step *theStep, G4TouchableHistory *)
{
	
    G4double edep = theStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;
    
	G4StepPoint* theParticle = theStep->GetPostStepPoint();

	if(!fNphysvol)
		CalculateNoPhysvols(theParticle);

	PmtHit* newHit = new PmtHit;

	G4int copyNo = CalculateCopyNo(theParticle);
	newHit->SetPmtID(copyNo);

    fHC->insert(newHit);
	
	// TODO : #ifdef CRTest_SD_MORE
	// TODO : move to DumpHit and call in EndOfEvent
	fHitCopyNo->push_back(copyNo);
	fHitEk->push_back(theParticle->GetKineticEnergy());
	fHitTime->push_back(theParticle->GetGlobalTime());

	G4ThreeVector pos = theParticle->GetPosition();
	fHitX->push_back(pos.x());
	fHitY->push_back(pos.y());
	fHitZ->push_back(pos.z());

	G4ThreeVector dir = theParticle->GetMomentumDirection();
	fHitPX->push_back(dir.x());
	fHitPY->push_back(dir.y());
	fHitPZ->push_back(dir.z());

    return true;
}

void PmtSD::EndOfEvent(G4HCofThisEvent*){

	for(int i = 0 ; i < fNvolume ; i++)
		fCounter->push_back(0);
    
	for (int i = 0; i < fHC->entries(); i++)
    {
        (*fCounter)[(*fHC)[i]->GetPmtID()]++;
    }

}

void PmtSD::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID = 
		rootData->CreateNtupleIColumn(ntupleID, "sd.pmt", *fCounter);
	// TODO : #ifdef CRTest_SD_MORE
	rootData->CreateNtupleIColumn(ntupleID, "pmt.id", *fHitCopyNo);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.E", *fHitEk);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.t", *fHitTime);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.x", *fHitX);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.y", *fHitY);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.z", *fHitZ);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.px", *fHitPX);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.py", *fHitPY);
	rootData->CreateNtupleDColumn(ntupleID, "pmt.pz", *fHitPZ);
}

void PmtSD::FillEntry(
	G4int, G4RootAnalysisManager*)
{}