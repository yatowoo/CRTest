/**
*   FILE : CRTest/src/StackAction.cc
*   Brief: Implementation of class StackAction
*/

#include "StackAction.hh"

#include "Analysis.hh"
#include "OpRecorder.hh"
#include "MuonRecorder.hh"

#include "G4Track.hh"
#include "G4VProcess.hh"

#include "G4OpticalPhoton.hh"

StackAction::StackAction()
    : G4UserStackingAction()
{
}

StackAction::~StackAction()
{
	G4cout << "[-] INFO - StackAction deleted. " << G4endl;
}

G4ClassificationOfNewTrack
StackAction::ClassifyNewTrack(const G4Track *theTrack)
{
	// Record muon
	if(theTrack->GetParentID() == 0){
		MuonRecorder::Instance()->Record(theTrack);
		return fUrgent;
	}


    OpRecorder *Recorder = OpRecorder::Instance();

	//Count what process generated the optical photons
	if (theTrack->GetDefinition() == G4OpticalPhoton::OpticalPhotonDefinition())
	{
		// particle is secondary
		if (theTrack->GetCreatorProcess()->GetProcessName() 
			== "Scintillation")
		{
			Analysis::Instance()->FillOpPhotonTrackForDebug(
				theTrack, OpPhotonType::Scintillation);
			Recorder->nScintTotal++;
		}
		else if (theTrack->GetCreatorProcess()->GetProcessName() 
			== "OpWLS")
		{
			Analysis::Instance()->FillOpPhotonTrackForDebug(
				theTrack, OpPhotonType::OpWLS);
			Recorder->nWlsEmit++;
		}

	}

    return fWaiting;
}

void StackAction::NewStage() {}

void StackAction::PrepareNewEvent() {}