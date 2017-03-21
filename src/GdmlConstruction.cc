/*
*   File : CRTest/src/GdmlConstruction.cc
*   Brief: Implementation of class GdmlConstruction
*/

#include "GdmlConstruction.hh"

GdmlConstruction::GdmlConstruction(G4GDMLParser *gdml)
    : SysConstruction()
{
  assert(gdml != NULL);
  fWorldPV = gdml->GetWorldVolume();
  fWorld = gdml->GetVolume("World");
  fDetector = gdml->GetVolume("Detector");
  fTarget = gdml->GetVolume("Target");
}
GdmlConstruction::~GdmlConstruction()
{
}

G4VPhysicalVolume *GdmlConstruction::Construct()
{
  return fWorldPV;
}