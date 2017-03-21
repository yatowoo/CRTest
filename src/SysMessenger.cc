/*
*   File : CRTest/src/SysMessenger.cc
*   Brief: Implementation of class SysMessenger
*/

#include "SysMessenger.hh"

#include "SysConstruction.hh"
#include "Generator.hh"
#include "CryGenerator.hh"

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

    fGeneratorDir = new G4UIdirectory("/CRTest/generator/");
    fGeneratorDir->SetGuidance("Set Generator");

    fGeneratorType = new G4UIcmdWithAString("/CRTest/generator/set", this);
    fGeneratorType->SetGuidance("Types: beam, CRY");
    fGeneratorType->SetCandidates("beam CRY");
    fGeneratorType->SetParameterName("type",true);
    fGeneratorType->SetDefaultValue("beam");
    fGeneratorType->AvailableForStates(G4State_Idle);
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
    
    }else if(cmd == fGeneratorType){
        G4cout << "[+] CMD - Set generator type - "
            << val
            << " - by SysMessenger."
            << G4endl;
        G4RunManager* runManager = G4RunManager::GetRunManager();
        if(val != fGeneratorType->GetCurrentValue())
        {
            delete runManager->GetUserPrimaryGeneratorAction();
            if(val == "beam")
                runManager->SetUserAction(new Generator);
            else if(val == "CRY")
                runManager->SetUserAction(new CryGenerator("./mac/setup.file"));
        }            
        G4cout << "[+] CMD - Current Type : "
            << fGeneratorType->GetCurrentValue()
            << " - by SysMessenger."
            << G4endl;
    }
}

G4String SysMessenger::GetCurrentValue(G4UIcommand* cmd)
{
    G4String val;
    return val;
}