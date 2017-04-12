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

#include "globals.hh"
#include<vector>

class VirtualSD : public G4VSensitiveDetector {

public:
	VirtualSD(G4String name)
		:G4VSensitiveDetector(name),fNvolume(0),fNphysvol(NULL){};
	virtual ~VirtualSD(){};

public: // for Geant4

	virtual void Initialize(G4HCofThisEvent* hc){};
	virtual void EndOfEvent(G4HCofThisEvent* hc){};
	virtual G4bool ProcessHits(
		G4Step*, G4TouchableHistory*){return true;};

public: // for class Analysis
	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager*) = 0;
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager*) = 0;

private:
	virtual void CalculateNoPhysvols(G4Step*){return;};
	virtual int CalculateCopyNo(G4Step*){return 0;};

protected:
	int fNvolume;
	std::vector<int>* fNphysvol;

public:
	virtual int GetNoVolumes(){return fNvolume;};
};

#endif /*CRTest_VirtualSD_h*/