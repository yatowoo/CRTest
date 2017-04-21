/*
*   File : CRTest/inc/Analysis.hh
*   Brief: Selection of the analysis technology
*   Description:
*       Geant4 Data Output Header Definition
*       Main class - G4VAnalysisManager
*       Data format - H1/H2/H3/P1/P2/Ntuple
*       Key methods
*           GetInstance
*           OpenFile/Write/Close/SetFileName
*           Create/Fill(H1/H2/H3/P1/P2)
*           Create/FillNtuple<T>Column
*           CreateNtuple, AddNtupleRow, FinishNtuple
*/

#ifndef CRTest_Analysis_h
#define CRTest_Analysis_h

#include "g4root.hh"

#include "VirtualRecorder.hh"
#include "OpRecorder.hh"
#include "VirtualSD.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"

#include<vector>
#include "globals.hh"
#include "G4SystemOfUnits.hh"

class Analysis {
public:
	Analysis();
	~Analysis();
public:
	static Analysis* Instance();
	
	G4bool OpenFile();
	G4bool SaveFile();

	G4int CreateNtupleForRun();
	G4int CreateNtupleForOpDebug();

	G4bool FillOpPhotonTrackForDebug(
		const G4Track* theTrack, OpPhotonType type);
	G4bool FillEntryForRun();
	
	G4bool RegisterSD(VirtualSD*);
	G4bool RegisterRecorder(VirtualRecorder*);
private:
	static Analysis* fgInstance;

private:
	G4RootAnalysisManager* rootData;
	G4int fRunNtuple;
	G4int fOpDebugNtuple;
	G4int fCurrentEvent;
	
	std::vector<VirtualSD*>* fSD;
	std::vector<VirtualRecorder*>* fRecorder;

public:
	void SetCurrentEvent(G4int eventID){fCurrentEvent=eventID;};
	G4int GetCurrentEvent(){return fCurrentEvent;};
};

#endif // CRTest_Analysis_h