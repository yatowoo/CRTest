/**
*   FILE : CRTest/src/PmtSD.cc
*   Brief: Implementation of class PmtSD
*/

#include "PmtSD.hh"

#include "G4SDManager.hh"

#include "G4String.hh"
#include "G4ios.hh"

PmtSD::PmtSD(G4String &name)
	: VirtualSD(name), fHCID(0), fFirstColID(-1),
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

	if(fCounter) 
		fCounter->clear();
	else
		fCounter = new std::vector<int>;
}

G4bool PmtSD::ProcessHits(G4Step *aStep, G4TouchableHistory *roHist)
{
	
    G4double edep = aStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;
    
	PmtHit* newHit = new PmtHit;

    fHC->insert(newHit);

    return true;
}

void PmtSD::EndOfEvent(G4HCofThisEvent* hce){
    PmtHC *pmtHC = static_cast<PmtHC *>(hce->GetHC(fHCID));

    G4int count = 0;
    
	for (int i = 0; i < pmtHC->entries(); i++)
    {
        count +=1;// (*pmtHC)[i]->GetPmtID();
    }

	fCounter->push_back(count);
}

void PmtSD::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID = 
		rootData->CreateNtupleIColumn(ntupleID, "sd.pmt", *fCounter);
}

void PmtSD::FillEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{}