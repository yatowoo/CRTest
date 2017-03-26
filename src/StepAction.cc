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
#include "G4VPhysicalVolume.hh"

#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"

#include "G4ProcessManager.hh"
#include "G4Scintillation.hh"
#include "G4OpBoundaryProcess.hh"

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
    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    //  for Optical
    if (theTrack->GetParticleDefinition() !=
        G4OpticalPhoton::OpticalPhotonDefinition())
        return;

    //
    // Boundary Check
    //
    if (thePostPoint->GetStepStatus() == fGeomBoundary)
    {
        assert(theProcess->GetProcessName() == "OpBoundary");
        G4OpBoundaryProcess *boundary = (G4OpBoundaryProcess*)theProcess;
        if (thePrePV->GetName() == "Detector_PV" &&
            thePostPV->GetName() == "Fiber_PV")
        {
            Recorder->nScintToFiber += 1;
            switch (boundary->GetStatus())
            {
            case Transmission:
                Recorder->nBoundaryTransmission++;
                break;
            case Absorption:;
            case Detection:
                Recorder->nBoundaryAbsorption++;
                break;
            case FresnelRefraction:;
            case FresnelReflection:;
            case TotalInternalReflection:;
            case LambertianReflection:;
            case LobeReflection:;
            case SpikeReflection:;
            case BackScattering:
                Recorder->nBoundaryReflection++;
                break;
            default:;
            }
            return;
        }
        else if (thePrePV->GetName() == "Fiber_PV" &&
                 thePostPV->GetName() == "Core_PV")
        {
            Recorder->nFiberToCore += 1;
            return;
        }
        else if (thePrePV->GetName() == "Core_PV" &&
                 thePostPV->GetName() == "World_PV")
        {
            Recorder->nCoreToPMT += 1;
            return;
        }
    }
}