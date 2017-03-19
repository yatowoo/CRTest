/**
*   FILE : CRTest/src/PhysicsList.cc
*   Brief: Implementation of class PhysicsList
*/

#include "PhysicsList.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4OpticalPhysics.hh"
#include "G4StepLimiterPhysics.hh"

#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList()
    : G4VModularPhysicsList()
{
    RegisterPhysics(new G4EmStandardPhysics());
    RegisterPhysics(new G4EmLivermorePhysics());
    RegisterPhysics(new G4OpticalPhysics());

    RegisterPhysics(new G4StepLimiterPhysics());
    
    SetDefaultCutValue(0.7*mm);
}

PhysicsList::~PhysicsList()
{}