/*
*   File : CRTest/inc/VirtualRecorder.hh
*   Brief: link Analysis & G4VSensitiveDetector 
*   Description:
*       Recorder virtual base
*/

#ifndef CRTest_VirtualRecorder_h
#define CRTest_VirtualRecorder_h

#include "G4RootAnalysisManager.hh"
#include "G4Track.hh"

#include "globals.hh"
#include<vector>

class VirtualRecorder {

public:
	VirtualRecorder();
	virtual ~VirtualRecorder();

public:
	static VirtualRecorder* Instance();

	void Reset(){};
	void Print(){};

public: // for class Analysis
	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager*){};
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager*){};

	virtual G4bool Record(const G4Track*){};

protected:
	G4int fFirstColID;

public:
	static VirtualRecorder* fgInstance;
};

#endif /*CRTest_VirtualRecorder_h*/