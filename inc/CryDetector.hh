/*
*   File : CRTest/inc/CryDetector.hh
*
*   Brief: Definition of Specific Detector for Cosmic-ray or others
*
*   Description:
*       Derive from G4LogicalVolume
*       Data/Physical Member // G4LogicalVolume(s)
*           TotalBox - Hold all components
*           Components - Detector detail structure
*       Inner Class?
*           CryHit, CrySD
*/

#ifndef CRTest_CryDetector_h
#define CRTest_CryDetector_h

#include "G4LogicalVolume.hh"

class CryDetector {

public:
    CryDetector();
    virtual ~CryDetector();
private:
    G4LogicalVolume* fTotalBox;
};

#endif /*CRTest_CryDetector_h*/