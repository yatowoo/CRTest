/**
*   FILE : CRTest/src/Analysis.cc
*   Brief: Implementation of class Analysis
*/

#include "Analysis.hh"

int ana::CURRENT_NTUPLE = -1;

Analysis* Analysis::fgInstance = NULL;

Analysis::Analysis()
	: fCurrentNtuple(-1)
{
	rootData = G4RootAnalysisManager::Instance();

	G4cout << "[+] INFO - CRTest_Analysis created." 
		<< G4endl;
}

Analysis* Analysis::Instance(){
	if(!fgInstance)
		fgInstance = new Analysis;
	return fgInstance;
}