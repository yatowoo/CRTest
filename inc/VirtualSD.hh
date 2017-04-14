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
	VirtualSD(G4String name);
	virtual ~VirtualSD();

public: // for Geant4

	virtual void Initialize(G4HCofThisEvent*){};
	virtual void EndOfEvent(G4HCofThisEvent*){};
	virtual G4bool ProcessHits(
		G4Step*, G4TouchableHistory*){return true;};

public: // for class Analysis
	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager*) = 0;
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager*) = 0;

protected:
	virtual void CalculateNoPhysvols(const G4StepPoint*);
	virtual int CalculateCopyNo(const G4StepPoint*);

protected:
	int fNvolume;
	std::vector<int>* fNphysvol;

// [TODO] : #ifdef CRTest_SD_MORE
	std::vector<int>* fHitCopyNo;
	std::vector<double>* fHitEk;
	std::vector<double>* fHitTime;
	std::vector<double>* fHitX;
	std::vector<double>* fHitY;
	std::vector<double>* fHitZ;
	std::vector<double>* fHitPX;
	std::vector<double>* fHitPY;
	std::vector<double>* fHitPZ;

public:
	virtual int GetNoVolumes(){return fNvolume;};
};

#endif /*CRTest_VirtualSD_h*/