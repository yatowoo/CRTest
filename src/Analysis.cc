/**
*   FILE : CRTest/src/Analysis.cc
*   Brief: Implementation of class Analysis
*/

#include "Analysis.hh"

#include "globals.hh"

Analysis* Analysis::fgInstance = NULL;

// add option for each type 
static std::vector<OpPhotonType> TypeList = {
	Fiber2Pmt,
	Detected,
	OpDebug
};
// #endif CRTest_DEBUG_OPTICAL_MORE

Analysis::Analysis()
	: fRunNtuple(-1), fOpDebugNtuple(-1), fCurrentEvent(-1)
{
	rootData = G4RootAnalysisManager::Instance();
	rootData->SetFileName("CRTest");

	fSD = new std::vector<VirtualSD*>;
	fRecorder = new std::vector<VirtualRecorder*>;

	G4cout << "[+] INFO - CRTest_Analysis created." 
		<< G4endl;
}

Analysis::~Analysis(){
	delete rootData;
	fRecorder->erase(fRecorder->begin(),fRecorder->end());
	delete fRecorder;
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

G4int Analysis::CreateNtupleForRun(){
	if(fRunNtuple != -1)
		return fRunNtuple;
	fRunNtuple =
		rootData->CreateNtuple("Run","Muon track & SD outputs");
	assert(fRunNtuple == 0);

	for(unsigned int i = 0 ; i < fSD->size() ; i++)
		(*fSD)[i]->CreateEntry(fRunNtuple, rootData);
	
	for(unsigned int i = 0 ; i < fRecorder->size() ; i++)
		(*fRecorder)[i]->CreateEntry(fRunNtuple, rootData);

	return fRunNtuple;
}

G4bool Analysis::FillEntryForRun(){

	for(unsigned int i = 0 ; i < fSD->size() ; i++)
		(*fSD)[i]->FillEntry(0, rootData);

	for(unsigned int i = 0 ; i < fRecorder->size() ; i++)
		(*fRecorder)[i]->FillEntry(0, rootData);

	rootData->AddNtupleRow();

	return true;
}

G4int Analysis::CreateNtupleForOpDebug(){
	
	if(fRunNtuple < 0 || fOpDebugNtuple > 0) return -1;
	
	G4int ntupleID = rootData->CreateNtuple(
		"OpDbg"," Storege Optical Photon for Debug");

	rootData->CreateNtupleIColumn(ntupleID, "type");  // id = 0
	rootData->CreateNtupleIColumn(ntupleID, "id"); // id = 1
	rootData->CreateNtupleDColumn(ntupleID, "E");	// eV
	rootData->CreateNtupleDColumn(ntupleID, "t");	// ns
	rootData->CreateNtupleDColumn(ntupleID, "x"); // cm
	rootData->CreateNtupleDColumn(ntupleID, "y");	// cm
	rootData->CreateNtupleDColumn(ntupleID, "z");	// cm
	rootData->CreateNtupleDColumn(ntupleID, "px");
	rootData->CreateNtupleDColumn(ntupleID, "py");
	rootData->CreateNtupleDColumn(ntupleID, "pz");
	rootData->CreateNtupleIColumn(ntupleID, "event");  // Event id

	rootData->FinishNtuple(ntupleID);

	fOpDebugNtuple = ntupleID;
	return fOpDebugNtuple;
}

G4bool Analysis::FillOpPhotonTrackForDebug(
	const G4Track* theTrack, OpPhotonType type)
{
	if(std::find(TypeList.begin(),TypeList.end(),type)
		== TypeList.end())
		return false;

	rootData->FillNtupleIColumn(fOpDebugNtuple, 0, type);
	rootData->FillNtupleIColumn(fOpDebugNtuple, 1, theTrack->GetTrackID());

	rootData->FillNtupleDColumn(fOpDebugNtuple, 2, theTrack->GetKineticEnergy() / eV);
	rootData->FillNtupleDColumn(fOpDebugNtuple, 3, theTrack->GetGlobalTime());

	G4ThreeVector position = theTrack->GetPosition();
	rootData->FillNtupleDColumn(fOpDebugNtuple, 4, position.x()/ cm);
	rootData->FillNtupleDColumn(fOpDebugNtuple, 5, position.y()/ cm);
	rootData->FillNtupleDColumn(fOpDebugNtuple, 6, position.z()/ cm);

	G4ThreeVector direction = theTrack->GetMomentumDirection();
	rootData->FillNtupleDColumn(fOpDebugNtuple, 7, direction.x());
	rootData->FillNtupleDColumn(fOpDebugNtuple, 8, direction.y());
	rootData->FillNtupleDColumn(fOpDebugNtuple, 9, direction.z());
	
	rootData->FillNtupleIColumn(fOpDebugNtuple, 10, fCurrentEvent);
	
	return rootData->AddNtupleRow(fOpDebugNtuple);
}

G4bool Analysis::RegisterSD(VirtualSD* sd){

	if(std::find(fSD->begin(),fSD->end(),sd)
		== fSD->end())
	{
		fSD->push_back(sd);
		return true;
	}
	else
		return false;
}

G4bool Analysis::RegisterRecorder(VirtualRecorder* recorder){

	if(std::find(fRecorder->begin(),fRecorder->end(),recorder)
		== fRecorder->end())
	{
		fRecorder->push_back(recorder);
		return true;
	}
	else
		return false;
}