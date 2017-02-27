/*
*   File : CRTest/inc/SysConstruction.hh
*
*   Brief: Definition of Detector System for simulationi
*
*   - Geometry
*       - Solid
*       - Logical Volume // Main Detector Object
*       - Physical Volume // Placement instantce
*   - Material
*   - Sensitive Detector
*   - Magnetic Field
*   - Visual Attribute
*   - Messenger
*   - Limits
*/

#ifndef CRTest_SysConstruction_h
#define CRTest_SysConstruction_h

#include "G4VUserDetectorConstruction.hh"

#include "G4LogicalVolume.hh"
#include "G4Material.hh"
#include "G4VSensitiveDetector.hh"
#include "G4FieldManager.hh"
#include "G4VisAttributes.hh"
#include "G4GenericMessenger.hh"
#include "G4UserLimits.hh"

class SysConstruction : public G4VUserDetectorConstruction
{

public:
    SysConstruction();
    virtual ~SysConstruction();

    virtual G4VPhysicalVolume* Construct();
    virtual void ConstructSDandField();

private:
    //void ConstructMaterial();

private:

    G4LogicalVolume* fWorld;
    G4LogicalVolume* fDetector;
    G4LogicalVolume* fTarget;

    G4UserLimits* fStepLimit;

    G4double fPosZTarget;
    G4double fPosZDetector;

};

#endif //CRTest_SysConstruction_h