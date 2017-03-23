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

    static G4int nScintTotal = 0;
    static G4int nScintToFiber = 0;

    G4Track *theTrack = aStep->GetTrack();

    if (theTrack->GetParentID() == 0 && theTrack->GetVolume()->GetName() == "Detector_PV")
    {

        //This is a primary track

        //G4TrackVector *fSecondary = fpSteppingManager->GetfSecondary();
        G4int tN2ndariesTot =
            fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();

        nScintTotal += tN2ndariesTot;

        if (theTrack->GetNextVolume()->GetName() == "World_PV")
        {
            G4cout << "[+] INFO - Primary Track Scintillation "
                   << "- OpPhotons Total Count : " << nScintTotal
                   << G4endl
                   << "- OpPhotons Scint. To Fiber : " << nScintToFiber
                   << " - by StepAction." << G4endl;
            nScintTotal = 0;
            nScintToFiber = 0;
        }

        return;
    }

    if (theTrack->GetParticleDefinition() != G4OpticalPhoton::OpticalPhotonDefinition())
        return;

    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    if (thePrePV && thePostPV &&
        thePrePV->GetName() == "Detector_PV" &&
        thePostPV->GetName() == "Fiber_PV")
    {

        nScintToFiber++;
        
        G4cout << "[X] DEBUG - OpPhoton from Scint. to Fiber "
               << G4endl
               << "\t- ParentTrackID : " << theTrack->GetParentID()
               << "\t- TrackID : " << theTrack->GetTrackID()
               << G4endl;
        theTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);
        /*
        G4cout << theTrack->GetTotalEnergy() / eV << " eV "
            << "Direction : " << theTrack->GetMomentumDirection()
            << G4endl;*/
    }

    /*  G4AnalysisManager *rootData = G4AnalysisManager::Instance();
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
    }*/
}