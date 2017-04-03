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

#include "SysConstruction.hh"
#include "GdmlConstruction.hh"
#include "ActionRegister.hh"
#include "SysMessenger.hh"
#include "PhysicsList.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"

#include "G4VModularPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4GDMLParser.hh"

#include "globals.hh"
#include "G4ios.hh"

int main (int argc, char** argv){
    
    G4cout << G4endl << "CRTest Start!"
        << G4endl << G4endl;
    
    G4String gdmlFileName = "./mac/default.gdml";
    G4String rootFileName = "CRTest";

    // UI Session
#ifdef G4UI_USE
    G4UIExecutive* ui = NULL;
    if (argc < 3)
        ui = new G4UIExecutive(argc, argv);
    // else in Batch mode
    if(argc > 1)
        gdmlFileName = argv[1];
#endif

    // Random
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    srand(time(NULL));
    G4Random::setTheSeed(rand(),3);

    // Run manager
    G4RunManager* runManager = new G4RunManager;
    // User defined classes 
        // Detector Construction
    runManager->SetUserInitialization(
        new GdmlConstruction(gdmlFileName));
    //runManager->SetUserInitialization(new SysConstruction());
    
    //G4VModularPhysicsList* physicsList = new FTFP_BERT;
    //physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    G4VModularPhysicsList* physicsList = new PhysicsList;
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserInitialization(new ActionRegister);
    
    SysMessenger* messenger = new SysMessenger(runManager);
    // Visualization Manager
		// TODO : ADD verbosiry option and if/else by arg. mode
#ifdef G4VIS_USE
    G4VisManager* visManager = new G4VisExecutive("error");
    visManager->Initialize();
#endif

    // UIManager
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    if( argc > 2){
        // Execute macro from argument
        G4String command = "/control/execute ";
        G4String fileName = argv[2];
        uiManager->ApplyCommand(command+fileName);
    }
    else{
        // Execute Initialization Macro by UIManager
        uiManager->ApplyCommand("/control/execute ./mac/init.mac");
#ifdef G4UI_USE
        if(ui->IsGUI())
            uiManager->ApplyCommand("/control/execute ./mac/gui.mac");
        // Start Session
        ui->SessionStart();
#endif
    }// mode case
    
    // delete
#ifdef G4UI_USE
    delete ui;
#endif
    delete messenger;
#ifdef G4VIS_USE
    delete visManager;
#endif
    delete runManager;

    return 0;
}