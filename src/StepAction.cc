/**
*   FILE : CRTest/src/StepAction.cc
*   Brief: Implementation of class StepAction
*/

#include "StepAction.hh"

#include "Analysis.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"

#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4VPhysicalVolume.hh"
#include "G4Scintillation.hh"
#include "G4ProcessManager.hh"

#include "G4SystemOfUnits.hh"

StepAction::StepAction()
{
}

StepAction::~StepAction()
{
}

void StepAction::UserSteppingAction(const G4Step *aStep)
{
    G4Track *theTrack = aStep->GetTrack();

    if (theTrack->GetParentID() != 0)
        return;

    //This is a primary track

    G4TrackVector *fSecondary = fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot =
        fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
    if (tN2ndariesTot == 0)
        return;

    G4AnalysisManager *rootData = G4AnalysisManager::Instance();
    
    G4cout << "[+] INFO - Primary Track Scintillation "
           << "- 2ndaries Total Count : " << tN2ndariesTot
           << " - by StepAction." << G4endl;
    G4cout << "[+] INFO - Scintillation Optical Photon " << G4endl;
    for (size_t i = (*fSecondary).size() - tN2ndariesTot;
         i < (*fSecondary).size(); i++)
    {
        G4Track *newTrack = (*fSecondary)[i];
        if (newTrack->GetCreatorProcess()->GetProcessName() 
            == "Scintillation")
        {
            G4ThreeVector photonV = newTrack->GetMomentumDirection();
            rootData->FillNtupleDColumn(10,
                newTrack->GetTotalEnergy() / eV);
            rootData->FillNtupleDColumn(11, photonV[0]);
            rootData->FillNtupleDColumn(12, photonV[1]);
            rootData->FillNtupleDColumn(13, photonV[2]);
            rootData->AddNtupleRow();
        }
    }
    G4cout << " - by StepAction." << G4endl;
    // DEBUG
    /*
    

        G4cout << "[+] INFO - Scintillation Optical Photon "
               << "- GetTotalEnergy : " << thePostPoint->GetTotalEnergy() / eV
               << " - by StepAction." << G4endl;
        rootData->FillNtupleDColumn(10, thePostPoint->GetTotalEnergy() / eV);
        */
}