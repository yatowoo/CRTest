/*
*   File : CRTest/src/SysMessenger.cc
*   Brief: Implementation of class SysMessenger
*/

#include "SysMessenger.hh"

#include "SysConstruction.hh"
#include "GdmlConstruction.hh"

#include "Generator.hh"
#include "CryGenerator.hh"
#include "PduGenerator.hh"

#include "G4UImanager.hh"
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
  // Geometry
  fGeomDir = new G4UIdirectory("/CRTest/geom/");
  fGeomDir->SetGuidance("System Construction");
    // GDML
  fGdmlFile = new G4UIcmdWithAString("/CRTest/geom/setGdmlFile", this);
  fGdmlFile->SetGuidance("Set .gdml file for construction");
  fGdmlFile->SetParameterName("file_name",false);
  fGdmlFile->SetDefaultValue("mac/default.gdml");
  fGdmlFile->AvailableForStates(G4State_Idle);
    // Logical Volume - Material
  fMaterialCmd = new G4UIcmdWithAString("/CRTest/geom/setMaterial", this);
  fMaterialCmd->SetGuidance("Set material for logical volume : lvName matName");
  fMaterialCmd->SetParameterName("lvName:matName",true);
  fMaterialCmd->AvailableForStates(G4State_Idle);
    // Logical Volume - Skin surface
  fSkinCmd = new G4UIcmdWithAString("/CRTest/geom/setSkin", this);
  fSkinCmd->SetGuidance("Set skin surface for logical volume : lvName surfName");
  fSkinCmd->SetParameterName("lvName:surfName",true);
  fSkinCmd->AvailableForStates(G4State_Idle);
    // Logical Volume - Dump material & skin surface
  fMatSkinDumpCmd = new G4UIcmdWithoutParameter("/CRTest/geom/dumpMaterial", this);
  fMatSkinDumpCmd->SetGuidance("Dump logical volume material & skin surface");
  fMatSkinDumpCmd->AvailableForStates(G4State_Idle);
    // Dump surface list
  fSurfaceDumpCmd = new G4UIcmdWithoutParameter("/CRTest/geom/dumpSurface", this);
  fSurfaceDumpCmd->SetGuidance("Dump surface list");
  fSurfaceDumpCmd->AvailableForStates(G4State_Idle);
  
  // Generator
  fGeneratorDir = new G4UIdirectory("/CRTest/generator/");
  fGeneratorDir->SetGuidance("Set Generator");

  fGeneratorType = new G4UIcmdWithAString("/CRTest/generator/set", this);
  fGeneratorType->SetGuidance("Types: beam, CRY, Pdu");
  fGeneratorType->SetCandidates("beam CRY Pdu");
  fGeneratorType->SetParameterName("type",true);
  fGeneratorType->SetDefaultValue("beam");
  fGeneratorType->AvailableForStates(G4State_Idle);
}

SysMessenger::~SysMessenger()
{
  delete fCRTestDir;
  delete fGeomDir;
  delete fGeneratorDir;
  delete fGdmlFile;
}

#include<sstream>

void SysMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
  const G4String cmdPath = cmd->GetCommandPath();
  if(cmdPath.contains(fGeneratorDir->GetCommandPath())){
    if(cmd == fGeneratorType){
      G4cout << "[+] CMD - Set generator type - " << val
        << " - by SysMessenger." << G4endl;
      G4RunManager* runManager = G4RunManager::GetRunManager();
      if(val != fGeneratorType->GetCurrentValue())
      {
        delete runManager->GetUserPrimaryGeneratorAction();
        if(val == "beam")
          runManager->SetUserAction(new Generator);
        else if(val == "CRY")
          runManager->SetUserAction(new CryGenerator("./mac/setup.file"));
        else if(val == "Pdu")
          runManager->SetUserAction(new PduGenerator);
      }            
      G4cout << "[+] CMD - Current Type : " << fGeneratorType->GetCurrentValue()
        << " - by SysMessenger." << G4endl;
    }
  }
  else if(cmdPath.contains(fGeomDir->GetCommandPath())){
    G4RunManager* runMgr = G4RunManager::GetRunManager();
    GdmlConstruction* geom = 
      (GdmlConstruction*)(runMgr->GetUserDetectorConstruction());
    if(cmd == fGdmlFile){
      G4cout << "[+] CMD - Current GDML file - "
        << geom->GetGdmlFileName() << " - by SysMessenger." << G4endl;
      geom->SetGdmlFileName(val);
      runMgr->DefineWorldVolume(geom->Construct());
      G4cout << "[+] CMD - NEW GDML file - "
        << val << " - by SysMessenger." << G4endl;
    }else if(cmd == fMaterialCmd){
      G4String lvName, matName;
      std::stringstream sstr(val);
      sstr >> lvName >> matName;
      if(geom->SetMaterial(lvName, matName))
        G4cout << "[+] CMD - NEW Material updated - "
        << lvName << " - " << matName << " - by SysMessenger." << G4endl;
    }else if(cmd == fSkinCmd){
      G4String lvName, surfName;
      std::stringstream sstr(val);
      sstr >> lvName >> surfName;
      if(geom->SetSkinSurface(lvName,surfName))
        G4cout << "[+] CMD - NEW Skin surface updated - "
        << lvName << " - " << surfName << " - by SysMessenger." << G4endl;
    }else if(cmd == fMatSkinDumpCmd){
      geom->DumpMaterialAndSurface();
    }else if(cmd == fSurfaceDumpCmd){
      geom->DumpOpticalSurface();
    }
  }
}

G4String SysMessenger::GetCurrentValue(G4UIcommand*)
{
  G4String val;
  return val;
}