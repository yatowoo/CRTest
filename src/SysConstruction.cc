/*
*   File : CRTest/src/SysConstruction.cc
*   Brief: Implementation of class SysConstruction
*/

#include "SysConstruction.hh"

#include "CryPositionSD.hh"

#include "G4Material.hh"
#include "G4NistManager.hh"
#include "G4GeometryManager.hh"
#include "G4GeometryTolerance.hh"
#include "G4PVPlacement.hh"

#include "G4Box.hh"

#include "G4SystemOfUnits.hh"

SysConstruction::SysConstruction()
    : fWorld(NULL), fDetector(NULL), fTarget(NULL),
      fStepLimit(NULL),
      fPosZTarget(0), fPosZDetector(0)
{
    G4cout << "[-] CRTest Detector System Construction Start"
           << G4endl;
}

SysConstruction::~SysConstruction()
{
    delete fStepLimit;
}

G4VPhysicalVolume *SysConstruction::Construct()
{
    // Default Material
    G4NistManager* nistManager = G4NistManager::Instance();
    G4Material *air  = nistManager->FindOrBuildMaterial("G4_AIR");
    G4Material *lead = nistManager->FindOrBuildMaterial("G4_Pb");
    G4Material *silicon = nistManager->FindOrBuildMaterial("G4_Si");

    // Default Size - not half
    G4double targetZ = 6 * cm;
    G4double targetW = 1 * cm;
    G4double detectorZ = 1 * mm;
    G4double detectorW = 10 * cm;
    G4double spacing = 5 * cm;

    G4double worldZ = 2.4 * (targetZ / 2. + detectorZ + spacing); // 21.6cm
    G4double worldW = 1.2 * detectorW;

    // Position
    G4ThreeVector worldPos = G4ThreeVector(0,0,0);
    G4ThreeVector targetPos = G4ThreeVector(0,0,0);
    G4ThreeVector detectorPos
        = G4ThreeVector(0, 0, targetZ / 2. + spacing + detectorZ / 2.);

    // Visualization
    G4VisAttributes *worldVis = new G4VisAttributes(true, G4Colour::White());
    G4VisAttributes *targetVis = new G4VisAttributes(true, G4Colour::Red());
    G4VisAttributes *detectorVis = worldVis;

    // Limit - by example/.../B2b
    G4GeometryManager::GetInstance()->SetWorldMaximumExtent(worldZ);
    G4cout << "Computed tolerance = "
           << G4GeometryTolerance::GetInstance()->GetSurfaceTolerance() / mm
           << " mm" << G4endl;
    fStepLimit = new G4UserLimits(detectorZ / 2.);

    // Construction
    // World
    G4Box *worldS = new G4Box("WorldS", worldW / 2., worldW / 2., worldZ / 2.);
    fWorld = new G4LogicalVolume(worldS, air, "World");
    G4VPhysicalVolume *worldPV =
        new G4PVPlacement(NULL, worldPos, fWorld, "WorldPV", NULL, false, 0, false);
    fWorld->SetVisAttributes(worldVis);
    // Target
    G4Box *targetS = new G4Box("TargetS", targetW / 2., targetW / 2., targetZ / 2.);
    fTarget = new G4LogicalVolume(targetS, lead, "Target");
    G4VPhysicalVolume *targetPV =
        new G4PVPlacement(NULL, targetPos, fTarget, "TargetPV", fWorld, false, 0, false);
    fTarget->SetVisAttributes(targetVis);
    fTarget->SetUserLimits(fStepLimit);
    // Detector
    G4Box *detectorS = new G4Box("DetectorS", detectorW / 2., detectorW / 2., detectorZ / 2.);
    fDetector = new G4LogicalVolume(detectorS, silicon, "Detector");
    G4VPhysicalVolume *detectorPV =
        new G4PVPlacement(NULL, detectorPos, fDetector, "DetectorPV",fWorld, false, 0, false);
    fDetector->SetVisAttributes(detectorVis);

    // ISSUE : print volume info.

    return worldPV;
}

void SysConstruction::ConstructSDandField()
{
    G4String sdName = "CryPositionSD";
    CryPositionSD* crySD = new CryPositionSD(sdName);

    SetSensitiveDetector(fDetector, crySD);

    return;
}

G4bool SysConstruction::SetSpacing(G4double spacing)
{
    if(spacing < 0*mm){
        G4cout << "[X] WARNNING - Illegal Value - by SetSpacing."
            << G4endl;
        return false;
    }

    G4double worldZ = ((G4Box*)fWorld->GetSolid())->GetZHalfLength();
    G4double targetZ = ((G4Box*)fTarget->GetSolid())->GetZHalfLength();
    G4double detectorZ = ((G4Box*)fDetector->GetSolid())->GetZHalfLength();

    G4double newWorldZ = 1.1 * (spacing + targetZ + 2*detectorZ);
    ((G4Box*)(fWorld->GetSolid()))->SetZHalfLength(newWorldZ);

    G4double newDetectorPosZ = targetZ + spacing + detectorZ;

    G4PVPlacement* detectorPV = 
        (G4PVPlacement*)(fWorld->GetDaughter(1));

    if(detectorPV)
        detectorPV->SetTranslation(G4ThreeVector(0,0,newDetectorPosZ));

    return true;
}