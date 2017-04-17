/**
*   FILE : CRTest/src/OpRecorder.cc
*   Brief: Implementation of class OpRecorder
*/

#include "OpRecorder.hh"

OpRecorder *OpRecorder::fgInstance = 0;

OpRecorder::OpRecorder()
    : VirtualRecorder(),
	nScintTotal(0), nScint2Groove(0), nGroove2Cladding(0),
      nCladding2Core(0), nWlsEmit(0),nCore2PMT(0),nDetection(0),
      nBoundaryReflection(0), nBoundaryAbsorption(0), nBoundaryTransmission(0),
      nDebug(0)
{
	boundaryName = "NULL";
}

OpRecorder::~OpRecorder() {
}

OpRecorder *OpRecorder::Instance()
{
    if (fgInstance == NULL)
        fgInstance = new OpRecorder();
    return fgInstance;
}

void OpRecorder::Reset()
{
    nScintTotal = 0;
    nScint2Groove = 0;
    nGroove2Cladding = 0;
    nCladding2Core = 0;
	nWlsEmit = 0;
	nCore2PMT = 0;
	nDetection = 0;

    nBoundaryReflection = 0;
    nBoundaryAbsorption = 0;
    nBoundaryTransmission = 0;
    nBoundaryUndefined = 0;
    nBoundaryWARNNING = 0;

    nDebug = 0;
}

void OpRecorder::SetBoundaryName(G4String name){
	boundaryName = name;
}

void OpRecorder::Print()
{
    G4cout << " | + Scintillation Total Count\t: " << nScintTotal << G4endl
           << " | + Scint. to Groove Boundary\t: " << nScint2Groove << G4endl
		   << " | + Groove to Cladding\t\t: " << nGroove2Cladding << G4endl
           << " | + Cladding to Core\t\t: " << nCladding2Core << G4endl
           << " | + Emitted by OpWLS\t\t: " << nWlsEmit << G4endl
		   << " | + Core to PMT\t\t: " << nCore2PMT << G4endl
           << " | + Detected by PMT\t\t: " << nDetection << G4endl
		   << " | + Boundary Details for " << boundaryName <<G4endl
           << " | + + Boundary Reflection\t: " << nBoundaryReflection << G4endl
           << " | + + Boundary Absorption\t: " << nBoundaryAbsorption << G4endl
           << " | + + Boundary Transmission\t: " << nBoundaryTransmission << G4endl
           << " | + + Boundary Undefined\t: " << nBoundaryUndefined << G4endl
           << " | + + Boundary WARNNING\t: " << nBoundaryWARNNING << G4endl
           << " | + X Count for Debug\t\t: " << nDebug << G4endl;
}

void OpRecorder::CreateEntry(G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID = 
		rootData->CreateNtupleIColumn(ntupleID, "op.scint");
	rootData->CreateNtupleIColumn(ntupleID, "op.s2g");
	rootData->CreateNtupleIColumn(ntupleID, "op.g2c");
	rootData->CreateNtupleIColumn(ntupleID, "op.c2c");
	rootData->CreateNtupleIColumn(ntupleID, "op.wls");
	rootData->CreateNtupleIColumn(ntupleID, "op.c2p");
	rootData->CreateNtupleIColumn(ntupleID, "op.det");
	rootData->CreateNtupleIColumn(ntupleID, "op.dbg");
}

void OpRecorder::FillEntry(G4int ntupleID, G4RootAnalysisManager* rootData)
{
	rootData->FillNtupleIColumn(ntupleID, fFirstColID, nScintTotal);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+1, nScint2Groove);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+2, nGroove2Cladding);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+3, nCladding2Core);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+4, nWlsEmit);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+5, nCore2PMT);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+6, nDetection);
	rootData->FillNtupleIColumn(ntupleID, fFirstColID+7, nDebug);
}

G4bool OpRecorder::Record(const G4Track*)
{}