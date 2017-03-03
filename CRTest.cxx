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
#include "ActionRegister.hh"
#include "SysMessenger.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIExecutive.hh"
#include "G4VModularPhysicsList.hh"
#include "FTFP_BERT.hh"
#include "G4StepLimiterPhysics.hh"

#include "globals.hh"
#include "G4ios.hh"

int main (int argc, char** argv){
    
    G4cout << G4endl << "CRTest Start!"
        << G4endl << G4endl;

    // UI Session
    G4UIExecutive* ui = new G4UIExecutive(argc, argv);
    
    // Random
    G4Random::setTheEngine(new CLHEP::RanecuEngine);
    
    // Run manager
    G4RunManager* runManager = new G4RunManager;
    // User defined classes 
    runManager->SetUserInitialization(new SysConstruction());
    
    G4VModularPhysicsList* physicsList = new FTFP_BERT;
    physicsList->RegisterPhysics(new G4StepLimiterPhysics());
    runManager->SetUserInitialization(physicsList);
    
    runManager->SetUserInitialization(new ActionRegister);
    
    SysMessenger* messenger = new SysMessenger(runManager);
    // Visualization Manager
    G4VisManager* visManager = new G4VisExecutive;
    visManager->Initialize();
    // UIManager
    G4UImanager* uiManager = G4UImanager::GetUIpointer();
    // Execute Initialization Macro by UIManager
    uiManager->ApplyCommand("/control/execute ./mac/init.mac");
    if(ui->IsGUI())
        uiManager->ApplyCommand("/control/execute ./mac/gui.mac");
    // Start Session
    ui->SessionStart();

    // delete
    delete messenger; 
    delete uiManager;
    delete visManager;
    delete physicsList;
    delete runManager;
    delete ui;

    return 0;
}