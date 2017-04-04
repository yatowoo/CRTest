/*
*   File : CRTest/inc/OpRecorder.hh
*   Brief: Record optical process information
*   Description:
*       Record optical photon : count, energy, direction
*       Scintillation, ScintToFiber, WLS, PMT
*/

#ifndef CRTest_OpRecorder_h
#define CRTest_OpRecorder_h

#include "globals.hh"

class OpRecorder{
public:
    OpRecorder();
    ~OpRecorder();
    static OpRecorder* Instance();
    void Reset();
    void Print();
	void SetBoundaryName(G4String);
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