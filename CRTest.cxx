/* Cosmic-ray Test Simulation Platform
*
*  Reference : example/basic/B5
*
*  Author : ytwu (torrence@mail.ustc.edu.cn)
*
*  Created : 2017.02.17
*
*/

/* file : CRTest.cxx
** desc.: Main Program for CRTest
*/

#include "Argument.hh"
#include "SysConstruction.hh"
#include "GdmlConstruction.hh"
#include "ActionRegister.hh"
#include "SysMessenger.hh"
#include "PhysicsList.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VisExecutive.hh"

#include "G4VModularPhysicsList.hh"
#include "G4StepLimiterPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"
#include<cstdlib>

int main (int argc, char** argv){
    // Handle Arguments
	Argument args;
	if(!args.Build(argc, argv)){
		args.Usage();
		return -1;
	}
    // UI Session
    G4UIExecutive* ui = NULL;
    if (args.Ui())
        ui = new G4UIExecutive(argc, argv);
    // Random
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    G4Random::setTheSeed(args.RndSeed(),3);
    // Run manager
    G4RunManager* runManager = new G4RunManager;
    // Welcome info.
    G4cout << G4endl << "[***] WELCOME - CRTest Start!" << G4endl << G4endl;
	// Print Arguments & Configuration Info.
	args.Print();
	// User defined classes 
        // Detector Construction
    runManager->SetUserInitialization(new GdmlConstruction(args.Gdml()));
		// Physics List
    runManager->SetUserInitialization(new PhysicsList);
		// User Actions
    runManager->SetUserInitialization(new ActionRegister);
		// User Messenger
    SysMessenger* messenger = new SysMessenger(runManager);
    
	// Visualization Manager
		// TODO : ADD verbosiry option and if/else by arg. mode
    G4VisManager* visManager = NULL;
	if(args.Vis()){
		visManager = new G4VisExecutive("error");
		visManager->Initialize();
	}
    // UIManager
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
	if( !args.Vis()){
        // Set output .root file name from argument
        G4String command = "/analysis/setFileName ";
		uiManager->ApplyCommand(command+args.Root());
        // Execute macro from argument
        command = "/control/execute ";
		uiManager->ApplyCommand(command+args.Macro());
    }
	else{
		// Execute Initialization Macro by UIManager
		uiManager->ApplyCommand("/control/execute ./mac/init.mac");
		if(args.Ui()){
			if(ui && ui->IsGUI())
				uiManager->ApplyCommand("/control/execute ./mac/gui.mac");
			// Start Session
			ui->SessionStart();
		}
	}// mode case
    
    // delete
	G4cout << G4endl << "[-] INFO - CRTest come to QUIT state." << G4endl;
	if(ui) delete ui;
	if(visManager)	delete visManager;
	delete messenger;
    delete runManager;
    
	// Exit info.
    G4cout << G4endl << "[***] All is well - CRTest Exit." << G4endl << G4endl;
    
	return 0;
}