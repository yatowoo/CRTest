/*
*   File : CRTest/inc/OpRecorder.hh
*   Brief: Record optical process information
*   Description:
*       Record optical photon : count, energy, direction
*       Scintillation, ScintToFiber, WLS, PMT
*/

#ifndef CRTest_OpRecorder_h
#define CRTest_OpRecorder_h

#include "VirtualRecorder.hh"

#include "G4RootAnalysisManager.hh"
#include "G4Track.hh"
#include "globals.hh"

enum OpPhotonType{
	Nothing = 0,
	Scintillation,
	Scint2Groove,
	Groove2Cladding,
	Cladding2Core,
	OpWLS,
	Fiber2Pmt,
	Detected,
	OpDebug
};

class OpRecorder : public VirtualRecorder{
public:
    OpRecorder();
    ~OpRecorder();
    static OpRecorder* Instance();
    void Reset();
    void Print();
	void SetBoundaryName(G4String);

public: // for class Analysis
	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager*);
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager*);

	G4bool Record(const G4Track*);

// TODO : Convert to 'private' and add Increment method
public:
    G4int nScintTotal;
    G4int nScint2Groove;
	G4int nGroove2Cladding;
	G4int nCladding2Core;
    G4int nWlsEmit;
    G4int nCore2PMT;
	G4int nDetection; // Detected by PmtSD
// For deatil probe
    G4int nBoundaryReflection;
    G4int nBoundaryAbsorption;
    G4int nBoundaryTransmission;
    G4int nBoundaryUndefined;
    G4int nBoundaryWARNNING;

    G4int nDebug; // use for debug
private:
    static OpRecorder* fgInstance;
	G4String boundaryName;
};


#endif // CRTest_OpRecorder_h