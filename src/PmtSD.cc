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

void PmtSD::CalculateNoPhysvols(G4Step* theStep){
	// VERBOSE
	G4cout << "[+] VERBOSE - Physical Volume : Numbers" << G4endl;
	fNphysvol = new std::vector<int>;
	fNvolume = 1;
		// for PMT or other SD invoked by G4OpBoundaryStatus::Detection
		// Get the POST step point but not 'pre'
	G4TouchableHistory* touchable
		= (G4TouchableHistory*)(theStep->GetPostStepPoint()->GetTouchable());
	for(int i = 0 ; i < 10 ; i++){
		G4VPhysicalVolume* thePV = touchable->GetVolume(i);
		G4String pvName = thePV->GetName();
		if(pvName == "World_PV")
			break;
		int nSibling = 
			thePV->GetMotherLogical()->GetNoDaughters();
		int nPV = 0;
		for(int idxPV = 0 ; idxPV < nSibling ; idxPV++)
			if(thePV->GetMotherLogical()->GetDaughter(idxPV)->GetName()
				== pvName)
				nPV ++;
		fNphysvol->push_back(nPV);
		fNvolume *= nPV;
		// VERBOSE
		G4cout << "\t" << pvName << "\t" << nPV << G4endl;
	}
	// VERBOSE
	G4cout << "Total - " << SensitiveDetectorName << " x" << fNvolume << G4endl;
}

int PmtSD::CalculateCopyNo(G4Step* theStep){
	int copyNo = 0;
		// for PMT or other SD invoked by G4OpBoundaryStatus::Detection
		// Get the POST step point but not 'pre'
    G4TouchableHistory* touchable
		= (G4TouchableHistory*)(theStep->GetPostStepPoint()->GetTouchable());
	int factor = 1;
	for(int i = 0 ; i < 10 ; i++){
		G4VPhysicalVolume* thePV = touchable->GetVolume(i);
		if(thePV->GetName() == "World_PV")
			break;
		copyNo += thePV->GetCopyNo() * factor;
		factor *= fNphysvol->at(i);
	}

	return copyNo;
}

G4bool PmtSD::ProcessHits(G4Step *theStep, G4TouchableHistory *)
{
	
    G4double edep = theStep->GetTotalEnergyDeposit();
    if(edep <= 0) return false;
    
	if(!fNphysvol)
		CalculateNoPhysvols(theStep);

	PmtHit* newHit = new PmtHit;

	newHit->SetPmtID(CalculateCopyNo(theStep));

    fHC->insert(newHit);

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
}

void PmtSD::FillEntry(
	G4int, G4RootAnalysisManager*)
{}