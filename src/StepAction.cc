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
		static G4int n2ndTotal = 0;
    //This is a primary track

    G4TrackVector *fSecondary = fpSteppingManager->GetfSecondary();
    G4int tN2ndariesTot =
        fpSteppingManager->GetfN2ndariesAtRestDoIt() + fpSteppingManager->GetfN2ndariesAlongStepDoIt() + fpSteppingManager->GetfN2ndariesPostStepDoIt();
    if (tN2ndariesTot == 0)
        return;
		n2ndTotal += tN2ndariesTot;
		if( theTrack->GetNextVolume()->GetName() == "World_PV"){
    	G4AnalysisManager *rootData = G4AnalysisManager::Instance();
      rootData->FillNtupleIColumn(10, n2ndTotal);
			n2ndTotal = 0;
		}
}
