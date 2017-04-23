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

    fGeomDir = new G4UIdirectory("/CRTest/geom/");
    fGeomDir->SetGuidance("System Construction");

	fGdmlFile = new G4UIcmdWithAString("/CRTest/geom/setGdmlFile", this);
    fGdmlFile->SetGuidance("Set .gdml file for construction");
    fGdmlFile->SetParameterName("file_name",true);
    fGdmlFile->SetDefaultValue("mac/default.gdml");
    fGdmlFile->AvailableForStates(G4State_Idle);

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

void SysMessenger::SetNewValue(G4UIcommand* cmd, G4String val)
{
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
	else if(cmd == fGdmlFile){
		G4RunManager* runMgr = G4RunManager::GetRunManager();
		GdmlConstruction* geom = (GdmlConstruction*)(runMgr->GetUserDetectorConstruction());
		G4cout << "[+] CMD - Current GDML file - "
			<< geom->GetGdmlFileName() << " - by SysMessenger." << G4endl;
		geom->SetGdmlFileName(val);
		runMgr->DefineWorldVolume(geom->Construct());
		G4cout << "[+] CMD - NEW GDML file - "
			<< val << " - by SysMessenger." << G4endl;
	}
}

G4String SysMessenger::GetCurrentValue(G4UIcommand*)
{
    G4String val;
    return val;
}