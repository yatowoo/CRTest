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
public:
    G4int nScintTotal;
    G4int nScintToFiber;
    G4int nWlsEmit;
    G4int nFiberToCore;
    G4int nCoreToPMT;
    G4int nBoundaryReflection;
    G4int nBoundaryAbsorption;
    G4int nBoundaryTransmission;
    G4int nDebug; // use for debug
private:
    static OpRecorder* fgInstance;
};


#endif // CRTest_OpRecorder_h