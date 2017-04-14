/*
*   File : CRTest/inc/PmtSD.hh
*
*   Brief: Definition of PMT Sensitive Detector for OpPhoton
*
*   Description:
*		Count OpPhoton hits for each PMT physical volume
*/

#ifndef CRTest_PmtSD_h
#define CRTest_PmtSD_h

#include "VirtualSD.hh"
#include "PmtHit.hh"

class PmtSD : public VirtualSD {

public:
	PmtSD(G4String&);
    virtual ~PmtSD();
public:
    virtual void Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(
		G4Step* aStep, G4TouchableHistory* roHist);
	virtual void EndOfEvent(G4HCofThisEvent* hce);

	virtual void CreateEntry(
		G4int ntupleID, G4RootAnalysisManager* rootData);
	virtual void FillEntry(
		G4int ntupleID, G4RootAnalysisManager* rootData);

private:
    G4int fHCID;
    PmtHC* fHC;

	G4int fFirstColID;

	std::vector<int>* fCounter;
};

#endif /*CRTest_PmtSD_h*/