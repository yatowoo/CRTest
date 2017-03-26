/**
*   FILE : CRTest/src/OpRecorder.cc
*   Brief: Implementation of class OpRecorder
*/

#include "OpRecorder.hh"

OpRecorder *OpRecorder::fgInstance = 0;

OpRecorder::OpRecorder()
    : nScintTotal(0), nScintToFiber(0), nWlsEmit(0),
      nFiberToCore(0), nCoreToPMT(0),
      nBoundaryReflection(0), nBoundaryAbsorption(0), nBoundaryTransmission(0),
      nDebug(0)
{
}

OpRecorder::~OpRecorder() {}

OpRecorder *OpRecorder::Instance()
{
    if (fgInstance == NULL)
        fgInstance = new OpRecorder();
    return fgInstance;
}

void OpRecorder::Reset()
{
    nScintTotal = 0;
    nScintToFiber = 0;
    nWlsEmit = 0;
    nFiberToCore = 0;
    nCoreToPMT = 0;
    nBoundaryReflection = 0;
    nBoundaryAbsorption = 0;
    nBoundaryTransmission = 0;

    nDebug = 0;
}

void OpRecorder::Print()
{
    G4cout << "    - Scintillation Total Count\t: " << nScintTotal << G4endl
           << "    - Scint. to Fiber Boundary\t: " << nScintToFiber << G4endl
           << "    - - Boundary Reflection\t: " << nBoundaryReflection << G4endl
           << "    - - Boundary Absorption\t: " << nBoundaryAbsorption << G4endl
           << "    - - Boundary Transmission\t: " << nBoundaryTransmission << G4endl
           << "    - Fiber to Core\t\t: " << nFiberToCore << G4endl
           << "    - Emitted by OpWLS\t\t: " << nWlsEmit << G4endl
           << "    - Detected by PMT\t\t: " << nCoreToPMT << G4endl
           << "    X Count for Debug\t\t: " << nDebug << G4endl;
}