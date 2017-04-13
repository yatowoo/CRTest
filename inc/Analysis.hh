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

// add option for each type 
static std::vector<OpPhotonType> TypeList = {
	Fiber2Pmt,
	Detected
};
// #endif CRTest_DEBUG_OPTICAL_MORE


class Analysis {
public:
	Analysis();
	~Analysis();
public:
	static Analysis* Instance();
	
	G4bool OpenFile();
	G4bool SaveFile();

	G4int CreateNtupleForEvent(G4int eventID);
	G4bool CreateNtupleForRun();

	G4bool FillOpPhotonTrackForEvent(
		const G4Track* theTrack, OpPhotonType type);
	G4bool FillEntryForRun();
	
	G4bool RegisterSD(VirtualSD*);
	G4bool RegisterRecorder(VirtualRecorder*);
private:
	static Analysis* fgInstance;

private:
	G4RootAnalysisManager* rootData;
	G4int fCurrentNtuple;

	std::vector<VirtualSD*>* fSD;
	std::vector<VirtualRecorder*>* fRecorder;

	G4int fOpticalFirstColID;
};

#endif // CRTest_Analysis_h