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

#include "OpRecorder.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"

#include<vector>
#include "globals.hh"
#include "G4SystemOfUnits.hh"

//
// [TODO] #ifdef CRTest_DEBUG_OPTICAL_MORE
enum OpPhotonType{
	Nothing = 0,
	Scintillation,
	Scint2Groove,
	Groove2Cladding,
	Cladding2Core,
	OpWLS,
	Fiber2Pmt,
	Photocathode
};

// add option for each type 
static std::vector<OpPhotonType> TypeList = {
	Cladding2Core,
	OpWLS,
	Fiber2Pmt,
	Photocathode
};
// #endif CRTest_DEBUG_OPTICAL_MORE
//

class CryPostionSD;
//[TODO] class PmtSD;

typedef struct MuonTrack{
	std::vector<double> Ek;	// GeV
	std::vector<double> time;	// ns
	std::vector<double> x;	// cm
	std::vector<double> y;
	std::vector<double> z;
	std::vector<double> px;	// direction
	std::vector<double> py;
	std::vector<double> pz;
}_Muon;

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

	G4bool FillMuonTrackForRun(const G4Track* theMuon);

	G4bool FillOpPhotonTrackForEvent(
		const G4Track* theTrack, OpPhotonType type);
	G4bool FillEntryForRun();
	
	// [TODO] G4bool RegisterSD(CrySD*);

private:
	static Analysis* fgInstance;

private:
	G4RootAnalysisManager* rootData;
	G4int fCurrentNtuple;
	// [TODO] std::vector<CRTestSD*> fCRTestSD;
	_Muon* fMuon;
};

#endif // CRTest_Analysis_h