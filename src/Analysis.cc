/**
*   FILE : CRTest/src/Analysis.cc
*   Brief: Implementation of class Analysis
*/

#include "Analysis.hh"

#include "globals.hh"

// REMOVE after class Analysis COMPLETED
int ana::CURRENT_NTUPLE = -1;

Analysis* Analysis::fgInstance = NULL;

Analysis::Analysis()
	: fCurrentNtuple(-1), fMuon(NULL)
{
	rootData = G4RootAnalysisManager::Instance();
	rootData->SetFileName("CRTest");

	fMuon = new _Muon;

	G4cout << "[+] INFO - CRTest_Analysis created." 
		<< G4endl;
}

Analysis::~Analysis(){
	delete rootData;
	delete fMuon;
}

Analysis* Analysis::Instance(){
	if(!fgInstance)
		fgInstance = new Analysis;
	return fgInstance;
}

G4bool Analysis::OpenFile(){
	return rootData->OpenFile();
}

G4bool Analysis::SaveFile(){
	rootData->Write();
	return rootData->CloseFile();
}

G4bool Analysis::CreateNtupleForRun(){
	if(fCurrentNtuple != -1)
		return false;
	fCurrentNtuple =
		rootData->CreateNtuple("Run","Muon track & SD outputs");
	assert(fCurrentNtuple == 0);

	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.E" , fMuon->Ek);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.t" , fMuon->time);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.x" , fMuon->x);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.y" , fMuon->y);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.z" , fMuon->z);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.px" , fMuon->px);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.py" , fMuon->py);
	rootData->CreateNtupleDColumn(fCurrentNtuple, "mu.pz" , fMuon->pz);

	// #for each sd in fCrySD (std::vector)
	// sd->CreateEntry(rootData, fCurrentNtuple)

	// #ifdef CRTest_DEBUG_OPTICAL
	// CreateNtupleIColumn(fCurrentNtuple, "op.[scint,wls,det]")
		// or Call OpRecorder::CreateEntry ?

	return true;
}

G4bool Analysis::FillMuonTrackForRun(G4Track* theMuon){

	if(theMuon->GetParentID() != 0)
		return false;
	
	(fMuon->Ek).push_back(theMuon->GetKineticEnergy());
	(fMuon->time).push_back(theMuon->GetGlobalTime());

	G4ThreeVector pos = theMuon->GetPosition();
	(fMuon->x).push_back(pos.x());
	(fMuon->y).push_back(pos.y());
	(fMuon->z).push_back(pos.z());

	G4ThreeVector pmu = theMuon->GetMomentum();
	(fMuon->px).push_back(pmu.x());
	(fMuon->py).push_back(pmu.y());
	(fMuon->pz).push_back(pmu.z());

	return true;
}

G4bool Analysis::FillEntryForRun(const G4Event* theEvent){

	// # fMuon filled with StepAction call Analysis::FillEntryForMuon

	// #for each sd in fCrySD (std::vector)
	// sd->FillEntry(fCurrentNtuple)

	// #ifdef CRTest_DEBUG_OPTICAL
	// FillNtupleIColumn(fCurrentNtuple, op_debug->nCol[scint,wls,det])
		// or Call OpRecorder::FillEntry ?

	rootData->AddNtupleRow();

	// Clear/Reset fMuon
	delete fMuon;
	fMuon = new _Muon;

	return true;
}