/**
*   FILE : CRTest/src/PduGenerator.cc
*   Brief: Implementation of class PduGenerator
*/

#include "PduGenerator.hh"

#include "G4ParticleDefinition.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"

#include "TMath.h"
#include "TF1.h"

PduGenerator::PduGenerator() : Generator(), fTheta(0.)
{
  // Initilized muon spectrum function
  fEmin = 0.1; // GeV - [TODO] change to G4SystemOfUnits?
  fEmax = 1000.;
  fPduModel = new TF1("pdu", PduModel, fEmin, fEmax, 1);
  fPduModel->FixParameter(0, fTheta);
}

PduGenerator::~PduGenerator()
{
  delete fPduModel;
}

void PduGenerator::GeneratePosition(){

  G4ThreeVector position = GetWorldBoundary();
  position.setX(position.x() * (1 - 2 * G4UniformRand()));
  position.setY(position.y() * (1 - 2 * G4UniformRand()));
  position.setZ(-position.z());

  if(fTriggerMode){
    position.setX(fXmin + (fXmax-fXmin) * G4UniformRand());
    position.setY(fYmin + (fYmax-fYmin) * G4UniformRand());
  }

  fParticleGun->SetParticlePosition(position);
}

void PduGenerator::GenerateDirection()
{

  // muon momentum direction
  fTheta = this->GetTheta();

  G4double phi = G4UniformRand() * TMath::Pi() * 2.;
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(
      TMath::Sin(fTheta) * TMath::Cos(phi),
      TMath::Sin(fTheta) * TMath::Sin(phi),
      TMath::Cos(fTheta)));
}

void PduGenerator::GenerateKineticEnergy()
{
  // muon kinetic energy
  fPduModel->FixParameter(0, fTheta);
  fParticleGun->SetParticleEnergy(fPduModel->GetRandom() * GeV);
}

void PduGenerator::GeneratePrimaries(G4Event *anEvent)
{
  Generate();
  if(fTriggerMode){
    G4int count = 1;
    G4ThreeVector boundary = GetWorldBoundary();
    while(!CheckEndpoint(boundary.z()) 
		&& count < 1000)
	{
      Generate();
      count ++;
    }
	// TODO : #ifdef CRTest_VERBOSE/DEBUG
    if(count > 100)
      G4cout << "[+] WARNING - BAD trigger efficiency. "
        << "Samples : " << count << "- by " << __func__ << G4endl;
  }
  fParticleGun->GeneratePrimaryVertex(anEvent);
}

// Accept-Reject method to sample muon zenith angle
G4double PduGenerator::GetTheta()
{
	using namespace TMath;
  // [TODO] create a general A-R method in class Generator
  G4double rnd, theta, val;
  do
  {
    // [TODO] change to CLHEP or G4
    theta = G4UniformRand() * TMath::Pi() / 2.;
		// Zenith angular distribution
    val = Sin(theta) * Cos(theta) * Cos(theta) * Cos(theta);
    rnd = G4UniformRand();
  } while (rnd > val);
  return theta;
}

G4ThreeVector PduGenerator::GetDirection()
{
  // [TODO] create a general method in class Generator
  using namespace TMath;

  G4double theta = this->GetTheta();
  G4double phi = G4UniformRand() * Pi() * 2.;

  return G4ThreeVector(
      Cos(theta) * Cos(phi),
      Cos(theta) * Sin(phi),
      Sin(theta));
}

// Ref. : Muon_generaotr_v3.m
G4double PduGenerator::PduModel(G4double *x, G4double *par)
{
  using namespace TMath;
  G4double Emu = x[0]; // Unit: GeV
  G4double cosX = Cos(par[0]);

  const G4double Alpha = 0.002382; // constant A
  const G4double lambda = 120;     // absorption mean free path 120 g/cm2
  const G4double kappa = 2.645;    // exponent (-)
  const G4double bp = 0.771;
  const G4double jp = 148.16;
  // correction factor (-); factor (GeV)
  const G4double alpha = 0.0025; // muon energy loss in GeV/g/cm2
  const G4double rho = 0.76;
  // fraction of pion energy that is transferred to muon
  const G4double y0 = 1000; // atmoshperic depth g/cm2
  const G4double bm = 0.8;
  const G4double Bm = 1.041231831; // correction factor (-);

  // Energy of Pion
  G4double Ep = (1 / rho) * (Emu + alpha * y0 * (1 / cosX - 0.1));

  // Prob. of Muon Production
  G4double Pmu = 0.1 * cosX * (1 - alpha / rho / Ep * (y0 / cosX - 100));
  Pmu = Power(Pmu, Bm / (rho * Ep + 100 * alpha) / cosX);

  G4double upper = Alpha * Pmu * Power(Ep, -kappa) * lambda * bp * jp;
  G4double lower = Ep * cosX + bp * jp;

  return (upper / lower);
}
