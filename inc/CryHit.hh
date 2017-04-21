/*
*   File : CRTest/inc/CryHit.hh
*
*   Brief: Definition of Specific Detector Hit for Cosmic-ray or others
*
*   Description:
*       Snapshot of Cosmic-ray particle track(step) in CryDetector
*/

#ifndef CRTest_CryHit_h
#define CRTest_CryHit_h

#include "G4VHit.hh"
#include "G4THitsCollection.hh"

#include "G4ThreeVector.hh"

class CryHit : public G4VHit
{
public: // Con(De)structor
    CryHit();
    virtual ~CryHit();
public: // Some Methods
    virtual void Draw();
    virtual void Print();
private: // Data Member
	G4double fTime;
    G4ThreeVector fPosition;
    G4ThreeVector fMomentum;
    G4int fParticleID;
    G4int fDetectorID;
    G4int fTrackID;
    G4double fEdep; // Total Energy Depoist
    G4double fIonEdep; // Ionization Energy Depoist
    G4double fStepLength;
public: // Getter & Setter
	G4double GetTime() const {return fTime;};
	void SetTime(G4double time) {fTime = time;};
    G4ThreeVector GetPositon() const {return fPosition;};
    void SetPosition(G4ThreeVector position) {fPosition = position;};
    G4ThreeVector GetMomentum() const {return fMomentum;};
    void SetMomentum(G4ThreeVector momentum) {fMomentum = momentum;};
    G4int GetParticleID() const {return fParticleID;};
    void SetParticleID(G4int pid) {fParticleID = pid;};
    G4int GetDetectorID() const {return fDetectorID;};
    void SetDetectorID(G4int detectorID) {fDetectorID = detectorID;};
    G4int GetTrackID() const {return fTrackID;};
    void SetTrackID(G4int trackID) {fTrackID = trackID;};
    G4double GetEdep() const {return fEdep;};
    void SetEdep(G4double edep) {fEdep = edep;};
    G4double GetIonEdep() const {return fIonEdep;};
    void SetIonEdep(G4double ionEdep) {fIonEdep = ionEdep;};
    G4double GetStepLength() const {return fStepLength;};
    void SetStepLength(G4double stepLength) {fStepLength = stepLength;};
};

// HitCollection
typedef G4THitsCollection<CryHit> CryHC;

#endif // CRTest_CryHit_h