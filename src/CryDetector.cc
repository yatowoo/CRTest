/**
*   FILE : CRTest/src/CryDetector.cc
*   Brief: Implementation of class CryDetector
*/

#include "CryDetector.hh"

#include "G4LogicalVolume.hh"

CryDetector::CryDetector()
    : fTotalBox(NULL)
{}

CryDetector::~CryDetector()
{
    delete fTotalBox;
}
