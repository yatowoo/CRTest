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

    //G4AnalysisManager* rootData = G4AnalysisManager::Instance();
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
    // TODO : Parameters, data file name
    rootData->SetFileName("Cry.root");
    rootData->CreateNtuple("Cry","Event");
    rootData->CreateNtupleDColumn("Edep");
    rootData->FinishNtuple();

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
