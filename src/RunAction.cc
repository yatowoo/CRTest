/**
*   FILE : CRTest/src/RunAction.cc
*   Brief: Implementation of class RunAction
*/

#include "RunAction.hh"

#include "Analysis.hh"

#include "G4RunManager.hh"

RunAction::RunAction()
    : G4UserRunAction()
{
    G4RunManager::GetRunManager()->SetPrintProgress(1);

    // Initialize .root file
    G4AnalysisManager* rootData = G4AnalysisManager::Instance();

    rootData->SetFileName("Cry.root");
    rootData->CreateNtuple("Cry","Event");
    rootData->CreateNtupleDColumn("Edep");  // id = 0
    rootData->CreateNtupleIColumn("pri.num"); // id = 1
    rootData->CreateNtupleIColumn("pri.id");
    rootData->CreateNtupleDColumn("pri.x");
    rootData->CreateNtupleDColumn("pri.y");
    rootData->CreateNtupleDColumn("pri.z");
    rootData->CreateNtupleDColumn("pri.px");
    rootData->CreateNtupleDColumn("pri.py");
    rootData->CreateNtupleDColumn("pri.pz");
    rootData->CreateNtupleDColumn("pri.E");
    rootData->CreateNtupleIColumn("scint.N"); // id = 10
    rootData->CreateNtupleDColumn("scint.E"); 
    rootData->CreateNtupleDColumn("scint.px");
    rootData->CreateNtupleDColumn("scint.py");
    rootData->CreateNtupleDColumn("scint.pz");
    rootData->CreateNtupleIColumn("fiber.N"); // id = 15
    rootData->CreateNtupleDColumn("fiber.E");
    rootData->CreateNtupleDColumn("fiber.px");
    rootData->CreateNtupleDColumn("fiber.py");
    rootData->CreateNtupleDColumn("fiber.pz");
    rootData->FinishNtuple();

}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();
	G4cout << "[-] INFO - RunAction deleted. " << G4endl;
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << G4endl << "[-] INFO - Run " << aRun->GetRunID()
        << " begins. - by RunAction" << G4endl;
    
    // Initialize .root file
    G4AnalysisManager* rootData = G4AnalysisManager::Instance();

    rootData->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    // Write data to file
    G4AnalysisManager* rootData = G4AnalysisManager::Instance();
    rootData->Write();
    rootData->CloseFile();

    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " ends. - by RunAction" << G4endl;
}
