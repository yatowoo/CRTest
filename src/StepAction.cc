/**
*   FILE : CRTest/src/StepAction.cc
*   Brief: Implementation of class StepAction
*/

#include "StepAction.hh"

#include "Analysis.hh"
#include "OpRecorder.hh"

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

    OpRecorder *Recoder = OpRecorder::Instance();

    G4Track *theTrack = aStep->GetTrack();

    if (theTrack->GetParentID() == 0 && theTrack->GetVolume()->GetName() == "Detector_PV")
    {
        //A primary track in Detector/Scint
        G4int tN2ndariesTot =
            fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
        if (tN2ndariesTot > 0)
            Recoder->nScintTotal += tN2ndariesTot;

        return;
    }

    if (theTrack->GetParticleDefinition() != 
        G4OpticalPhoton::OpticalPhotonDefinition())
        return;

    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    if (thePrePV && thePostPV &&
        thePrePV->GetName() == "Detector_PV" &&
        thePostPV->GetName() == "Fiber_PV")
    {

        Recoder->nScintToFiber += 1;

        theTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);

        G4AnalysisManager *rootData = G4AnalysisManager::Instance();
        G4ThreeVector photonV =
            theTrack->GetMomentumDirection();
        rootData->FillNtupleDColumn(16,
                                    theTrack->GetTotalEnergy() / eV);
        rootData->FillNtupleDColumn(17, photonV[0]);
        rootData->FillNtupleDColumn(18, photonV[1]);
        rootData->FillNtupleDColumn(19, photonV[2]);
        rootData->AddNtupleRow();
    }
}