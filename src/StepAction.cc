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

    OpRecorder *Recorder = OpRecorder::Instance();

    G4Track *theTrack = aStep->GetTrack();

    if (theTrack->GetParentID() == 0 && theTrack->GetVolume()->GetName() == "Detector_PV")
    {
        //A primary track in Detector/Scint
        G4int tN2ndariesTot =
            fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
        if (tN2ndariesTot > 0)
            Recorder->nScintTotal += tN2ndariesTot;

        return;
    }

    if (theTrack->GetParticleDefinition() !=
        G4OpticalPhoton::OpticalPhotonDefinition())
        return;

    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    if (thePrePV && thePostPV)
    {
        if (thePrePV->GetName() == "Detector_PV" &&
            thePostPV->GetName() == "Fiber_PV")
        {
            Recorder->nScintToFiber += 1;
            return;
        }
        if (thePrePV->GetName() == "Fiber_PV" &&
            thePostPV->GetName() == "Core_PV")
        {
            Recorder->nFiberToCore += 1;
            return;
        }
        if (thePrePV->GetName() == "Core_PV" &&
            thePostPV->GetName() == "World_PV")
        {
            Recorder->nCoreToPMT += 1;
            return;
        }
    }

    const G4VProcess *theProcess = thePostPoint->GetProcessDefinedStep();
    if (theProcess && theProcess->GetProcessName() == "OpWLS")
    {
        Recorder->nWlsEmit++;
        //theTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);
        return;
    }
}