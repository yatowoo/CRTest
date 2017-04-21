/**
*   FILE : CRTest/src/StepAction.cc
*   Brief: Implementation of class StepAction
*/

#include "StepAction.hh"

#include "Analysis.hh"
#include "OpRecorder.hh"
#include "MuonRecorder.hh"

#include "CryPositionSD.hh"

#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4SteppingManager.hh"
#include "G4VPhysicalVolume.hh"

#include "G4ParticleDefinition.hh"
#include "G4OpticalPhoton.hh"
#include "G4OpBoundaryProcess.hh"

#include "G4SystemOfUnits.hh"

StepAction::StepAction()
{
}

StepAction::~StepAction()
{
    G4cout << "[-] INFO - StepAction deleted. " << G4endl;
}

void StepAction::UserSteppingAction(const G4Step *aStep)
{

    OpRecorder *Recorder = OpRecorder::Instance();

    G4Track *theTrack = aStep->GetTrack();
    G4StepPoint *thePrePoint = aStep->GetPreStepPoint();
    G4StepPoint *thePostPoint = aStep->GetPostStepPoint();
    G4VPhysicalVolume *thePrePV = thePrePoint->GetPhysicalVolume();
    G4VPhysicalVolume *thePostPV = thePostPoint->GetPhysicalVolume();

    const G4VProcess *theProcess = fpSteppingManager->GetfCurrentProcess();

	// for Muon (primary track)
	if (theTrack->GetParentID() == 0){
		MuonRecorder::Instance()->Record(theTrack);
		
		// TODO : #ifndef CRTest_Optical_Test -> primary == OpticalPhoton
		// Check if Muon in CryPositionSD
		G4TouchableHistory* touchable
			= (G4TouchableHistory*)(thePrePoint->GetTouchable());
		for(int i = 0 ; i < 10 ; i ++){
			G4VPhysicalVolume* pv = touchable->GetVolume(i);
			G4LogicalVolume* lv = pv->GetLogicalVolume();
			if(lv->GetName() == "World") break;
			
			G4VSensitiveDetector* sd = lv->GetSensitiveDetector();
			if(sd && sd->GetName() == "CryPositionSD")
				static_cast<CryPositionSD*>(sd)->ProcessHits_more(aStep,pv);
		}
	}

    //  for Optical
    if (theTrack->GetParticleDefinition() !=
        G4OpticalPhoton::OpticalPhotonDefinition())
        return;

	// Step Numbers CUT - to avoid endless OpBoundary process
	if(theTrack->GetCurrentStepNumber() > 5000){
		theTrack->SetTrackStatus(G4TrackStatus::fStopAndKill);
		Analysis::Instance()->FillOpPhotonTrackForDebug(theTrack, OpDebug);
		return;
	}
	// DEBUG - ABSLENGTH
	if(theProcess->GetProcessName() == "OpAbsorption")
		Recorder->nDebug += 1;

    //
    // Boundary Check
    //
	OpPhotonType type = OpPhotonType::Nothing;
    if (thePostPoint->GetStepStatus() == fGeomBoundary)
	{
        assert(theProcess->GetProcessName() == "OpBoundary");
        G4OpBoundaryProcess *boundary = (G4OpBoundaryProcess *)theProcess;
		G4OpBoundaryProcessStatus status = boundary->GetStatus();
		G4bool gotThrough = 
			(status == Transmission || status == FresnelRefraction);
		G4bool fromWLS = false;
		if(theTrack->GetParentID() != 0)
			fromWLS = (theTrack->GetCreatorProcess()->GetProcessName()
			== "OpWLS");
		if(gotThrough && !fromWLS){
			// OpPthoton got through boundary
			if (thePrePV->GetName() == "Scintillator_PV" &&
				thePostPV->GetName() == "Groove_PV")
			{
				type = Scint2Groove;
				Recorder->nScint2Groove ++;

			}
			else if (thePrePV->GetName() == "Groove_PV" &&
				(thePostPV->GetName() == "Fiber_PV"
				|| thePostPV->GetName() == "Cladding2_PV"))
			{
				type = Groove2Cladding;
				Recorder->nGroove2Cladding += 1;
			}
			else if (thePrePV->GetName() == "Cladding1_PV" &&
				thePostPV->GetName() == "Core_PV")
			{
				type = Cladding2Core;
				Recorder->nCladding2Core += 1;
			}
			// For Debug boundary details
			else if ((thePrePV->GetName() == "Cladding1_PV" || 
				thePrePV->GetName() == "Cladding2_PV" )&&
				thePostPV->GetName() == "Groove_PV")
			{
				Recorder->SetBoundaryName("Fiber2Groove");
				BoundaryStats(boundary);
				return;
			}
		}
        else if (thePrePV->GetName() == "Fiber_PV" &&
                 thePostPV->GetName() == "PMT_PV")
        {
			// OpPhoton hit PMT photocathode
            type = Fiber2Pmt;
            Recorder->nCore2PMT += 1;
            if (status == Detection){
				type = Detected;
				Recorder->nDetection += 1;
			}
        }
    }
	Analysis::Instance()->FillOpPhotonTrackForDebug(theTrack, type);
}

G4bool StepAction::BoundaryStats(G4OpBoundaryProcess *boundary)
{
    OpRecorder *Recorder = OpRecorder::Instance();
    switch (boundary->GetStatus())
    {
    case FresnelRefraction:;
    case Transmission:
        Recorder->nBoundaryTransmission++;
        break;
    case Absorption:;
    case Detection:
        Recorder->nBoundaryAbsorption++;
        break;
    case FresnelReflection:;
    case TotalInternalReflection:;
    case LambertianReflection:;
    case LobeReflection:;
    case SpikeReflection:;
    case BackScattering:
        Recorder->nBoundaryReflection++;
        break;
    case Undefined:
        Recorder->nBoundaryUndefined++;
        break;
    case NotAtBoundary:;
    case SameMaterial:;
    case StepTooSmall:;
    case NoRINDEX:;
        Recorder->nBoundaryWARNNING++;
        break;
    default:
        return false;
    }
    return true;
}