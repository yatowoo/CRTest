/*
*   File : CRTest/inc/VirtualSD.hh
*   Brief: link Analysis & G4VSensitiveDetector 
*   Description:
*       Sensitive Detector virtual base
*/

#ifndef CRTest_VirtualSD_h
#define CRTest_VirtualSD_h

#include "G4VSensitiveDetector.hh"

#include "G4RootAnalysisManager.hh"

class VirtualSD : public G4VSensitiveDetector {

public:
	VirtualSD();
	VirtualSD(G4String name):G4VSensitiveDetector(name){};
	virtual ~VirtualSD(){};

public:

	virtual void Initialize(G4HCofThisEvent* hc){};
	virtual void EndOfEvent(G4HCofThisEvent* hc){};
	virtual G4bool ProcessHits(
		G4Step* theStep, G4TouchableHistory* roHist){};

	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager* rootData) = 0;
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager* rootData) = 0;

};

#endif /*CRTest_VirtualSD_h*/