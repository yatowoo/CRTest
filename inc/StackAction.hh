/*
*   File : CRTest/inc/StackAction.hh
*   Brief: User Action before G4Track tracking & processing
*   Description:
*       Record optical photon production
*/

#ifndef CRTest_StackAction_h
#define CRTest_StackAction_h 

#include "G4UserStackingAction.hh"

#include "G4Track.hh"

class StackAction : public G4UserStackingAction
{
 public:

    StackAction();
    virtual ~StackAction();
 
   virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track* aTrack);
    virtual void NewStage();
    virtual void PrepareNewEvent();
 
  private:
};

#endif /*CRTest_StackAction_h*/
