/*
*   File : CRTest/inc/CryPositionSD.hh
*
*   Brief: Postion-sensitive Detector for Cosmic-ray or others
*
*   Description:
*       Derive from G4LogicalVolume
*       Data/Physical Member // G4LogicalVolume(s)
*           TotalBox - Hold all components
*           Components - Detector detail structure
*       Inner Class?
*           CryHit, CrySD
*/

#ifndef CRTest_CryPositionSD_h
#define CRTest_CryPositionSD_h

#include "G4VSensitiveDetector.hh"

#include "G4Step.hh"
#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"

#include "G4String.hh"

class CryPositionSD : public G4VSensitiveDetector {

public:
    CryPositionSD(G4String& name);
    virtual ~CryPositionSD();
public:
    virtual void Initialize(G4HCofThisEvent* hce);
    virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* roHist);
private:
    G4int fHCID;
};

#endif /*CRTest_CryPositionSD_h*/