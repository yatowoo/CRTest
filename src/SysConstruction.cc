/*
*   File : CRTest/src/SysConstruction.cc
*   Brief: Implementation of DetectorConstruction
*/

#include "SysConstruction.hh"

#include "G4SystemOfUnits.hh"
#include "G4Box.hh"

SysConstruction::SysConstruction()
: fMatWorld(NULL), fMatDetector(NULL),fMatTarget(NULL),
    fWorld(NULL), fDetector(NULL), fTarget(NULL),
    fStepLimit(NULL),
    fPosZTarget(0), fPosZDetector(0)
{
    G4cout << " [-] CRTest Detector System Construction Start" 
            << G4endl;
}

SysConstruction::~SysConstruction()
{
    
    if(fMatWorld) delete fMatWorld;
    if(fMatDetector) delete fMatDetector;
    if(fMatTarget) delete fMatTarget;

    if(fWorld) delete fWorld;
    if(fDetector) delete fDetector;
    if(fTarget) delete fTarget;

    if(fStepLimit) delete fStepLimit;

    fMatWorld = NULL;
    fMatDetector = NULL;
    fMatTarget = NULL;
    fWorld = NULL;
    fDetector = NULL;
    fTarget = NULL;
    fStepLimit = NULL;

}

G4VPhysicalVolume* SysConstruction::Construct()
{
    G4Box* worldBox
        = new G4Box();
    fWorld = new G4LogicalVolume();
    G4VPhysicalVolume* worldPV = 
        new G4VPhysicalVolume();
    
    
    return worldPV;
}