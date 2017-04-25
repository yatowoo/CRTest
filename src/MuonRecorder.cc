/**
*   FILE : CRTest/src/MuonRecorder.cc
*   Brief: Implementation of class MuonRecorder
*/

#include "MuonRecorder.hh"

#include "G4SystemOfUnits.hh"

MuonRecorder* MuonRecorder::fgInstance = NULL;

MuonRecorder::MuonRecorder()
	: VirtualRecorder()
{
	fEk = new std::vector<double>;
	fTime = new std::vector<double>;
	fX = new std::vector<double>;
	fY = new std::vector<double>;
	fZ = new std::vector<double>;
	fPX = new std::vector<double>;
	fPY = new std::vector<double>;
	fPZ = new std::vector<double>;
}

MuonRecorder::~MuonRecorder(){
	fEk->clear();delete fEk;
	fTime->clear();delete fTime;
	fX->clear();delete fX;
	fY->clear();delete fY;
	fZ->clear();delete fZ;
	fPX->clear();delete fPX;
	fPY->clear();delete fPY;
	fPZ->clear();delete fPZ;
}

MuonRecorder* MuonRecorder::Instance(){
	if(!fgInstance)
		fgInstance = new MuonRecorder;
	return fgInstance;
}

void MuonRecorder::Reset()
{
	fEk->clear();
	fTime->clear();
	fX->clear();
	fY->clear();
	fZ->clear();
	fPX->clear();
	fPY->clear();
	fPZ->clear();
}

void MuonRecorder::CreateEntry(
	G4int ntupleID, G4RootAnalysisManager* rootData)
{
	fFirstColID =
		rootData->CreateNtupleDColumn(ntupleID, "mu.E", *fEk);
	rootData->CreateNtupleDColumn(ntupleID, "mu.t", *fTime);
	rootData->CreateNtupleDColumn(ntupleID, "mu.x", *fX);
	rootData->CreateNtupleDColumn(ntupleID, "mu.y", *fY);
	rootData->CreateNtupleDColumn(ntupleID, "mu.z", *fZ);
	rootData->CreateNtupleDColumn(ntupleID, "mu.px", *fPX);
	rootData->CreateNtupleDColumn(ntupleID, "mu.py", *fPY);
	rootData->CreateNtupleDColumn(ntupleID, "mu.pz", *fPZ);
}

void MuonRecorder::FillEntry(G4int,G4RootAnalysisManager*)
{}

G4bool MuonRecorder::Record(const G4Track* theMuon){
	if(theMuon->GetParentID() != 0)
		return false;
	
	fEk->push_back(theMuon->GetKineticEnergy() / GeV );
	fTime->push_back(theMuon->GetGlobalTime() / ns );

	G4ThreeVector pos = theMuon->GetPosition();
	fX->push_back(pos.x() / cm );
	fY->push_back(pos.y() / cm );
	fZ->push_back(pos.z() / cm );

	G4ThreeVector pmu = theMuon->GetMomentumDirection();
	fPX->push_back(pmu.x());
	fPY->push_back(pmu.y());
	fPZ->push_back(pmu.z());
}