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
    rootData->CreateNtupleDColumn("Edep");
    rootData->CreateNtupleIColumn("pri.num");
    rootData->CreateNtupleIColumn("pri.id");
    rootData->CreateNtupleDColumn("pri.x");
    rootData->CreateNtupleDColumn("pri.y");
    rootData->CreateNtupleDColumn("pri.z");
    rootData->CreateNtupleDColumn("pri.px");
    rootData->CreateNtupleDColumn("pri.py");
    rootData->CreateNtupleDColumn("pri.pz");
    rootData->CreateNtupleDColumn("pri.E");
    rootData->FinishNtuple();

}

RunAction::~RunAction()
{
    delete G4AnalysisManager::Instance();    
}

void RunAction::BeginOfRunAction(const G4Run* aRun)
{
    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " begin. - by RunAction" << G4endl;
    
    // Initialize .root file
    G4AnalysisManager* rootData = G4AnalysisManager::Instance();

    rootData->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run* aRun)
{
    G4cout << "[-] INFO - Run " << aRun->GetRunID()
        << " end. - by RunAction" << G4endl;

    // Write data to file
    G4AnalysisManager* rootData = G4AnalysisManager::Instance();
    rootData->Write();
    rootData->CloseFile();
}
