/*
*   File : CRTest/src/SysMessenger.cc
*   Brief: Implementation of class SysMessenger
*/

#include "SysMessenger.hh"

#include "SysConstruction.hh"

#include "G4UIdirectory.hh"
#include "G4UIcommand.hh"
#include "G4UIcmdWithADoubleAndUnit.hh"

#include "G4String.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

SysMessenger::SysMessenger(G4RunManager* runManager)
 :  G4UImessenger(),
    fRunManager(runManager)
{
    fCRTestDir = new G4UIdirectory("/CRTest/");
    fCRTestDir->SetGuidance("CRTest UI commands");

    fGeomDir = new G4UIdirectory("/CRTest/geom/");
    fGeomDir->SetGuidance("System Construction");

    fSpacingCmd = new G4UIcmdWithADoubleAndUnit("/CRTest/geom/spacing", this);
    fSpacingCmd->SetGuidance("Set spacing between target & detector");
    fSpacingCmd->SetParameterName("spacing",false);
    fSpacingCmd->SetUnitCategory("Length");
    fSpacingCmd->AvailableForStates(G4State_Idle);
}

SysMessenger::~SysMessenger()
{
    delete fCRTestDir;
    delete fGeomDir;
    delete fSpacingCmd;
}

void SysMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
    if(cmd == fSpacingCmd){
        
        G4cout << "[+] CMD - Set Spacing between target & detector - "
            << fSpacingCmd->GetNewDoubleValue(val) / cm
            << " cm - by SysMessenger."
            << G4endl;

        SysConstruction* sysGeom = 
            (SysConstruction*)fRunManager->GetUserDetectorConstruction();
        if(sysGeom)
            sysGeom->SetSpacing(fSpacingCmd->GetNewDoubleValue(val));
    
    }
}

G4String SysMessenger::GetCurrentValue(G4UIcommand* cmd)
{
    G4String val;
    return val;
}