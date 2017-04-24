/*
*   File : CRTest/inc/Generator.hh
*
*   Brief: Generate primary event vertex and define messenger commands
*
*/

#ifndef CRTest_Generator_h
#define CRTest_Generator_h

#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4Event.hh"

class Generator : public G4VUserPrimaryGeneratorAction {

public:
  Generator();
  virtual ~Generator();
public:
  virtual void GeneratePrimaries(G4Event* anEvent);

protected:
  virtual void Generate();
  virtual void GeneratePosition();
  virtual void GenerateDirection();
  virtual void GenerateKineticEnergy();

  G4ThreeVector GetWorldBoundary();
  G4bool CheckEndpoint(G4double boundaryZ);
protected:
  G4ParticleGun* fParticleGun;

  // Trigger Mode
  G4bool fTriggerMode;
  G4double fXmin;
  G4double fXmax;
  G4double fYmin;
  G4double fYmax;

public: // Get & Set method
  G4bool IsTriggerMode(){return fTriggerMode;};
  void SetTriggerMode(G4bool trigger){fTriggerMode = trigger;};
  G4double GetXmin(){return fXmin;};
  void SetXmin(G4double xmin){fXmin = xmin;};
  G4double GetXmax(){return fXmax;};
  void SetXmax(G4double xmax){fXmax = xmax;};
  G4double GetYmin(){return fYmin;};
  void SetYmin(G4double Ymin){fYmin = Ymin;};
  G4double GetYmax(){return fYmax;};
  void SetYmax(G4double Ymax){fYmax = Ymax;};
};

#endif /*CRTest_Generator_h*/