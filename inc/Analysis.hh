/*
*   File : CRTest/inc/Analysis.hh
*   Brief: Selection of the analysis technology
*   Description:
*       Geant4 Data Output Header Definition
*       Main class - G4VAnalysisManager
*       Data format - H1/H2/H3/P1/P2/Ntuple
*       Key methods
*           GetInstance
*           OpenFile/Write/Close/SetFileName
*           Create/Fill(H1/H2/H3/P1/P2)
*           Create/FillNtuple<T>Column
*           CreateNtuple, AddNtupleRow, FinishNtuple
*/

#ifndef CRTest_Analysis_h
#define CRTest_Analysis_h

#include "g4root.hh"

#include "OpRecorder.hh"

#include "G4Track.hh"
#include "G4Event.hh"
#include "G4VProcess.hh"

#include<vector>
#include "globals.hh"
#include "G4SystemOfUnits.hh"

namespace ana {
	// TODO : Wrap by class Analysis with static & Instance()
	extern int CURRENT_NTUPLE;

	enum RunCol {
		PRIMARY_E = 0,
		PRIMARY_X = 1,
		PRIMARY_Y = 2,
		PRIMARY_Z = 3,
		PRIMARY_PX = 4,
		PRIMARY_PY = 5,
		PRIMARY_PZ = 6,
		NUM_SCINT = 7,
		NUM_WLS = 8,
		NUM_DETECTION = 9,
		END = NUM_DETECTION +1,
		EnergyDeposit = 10 // TODO : Create new method to register SD col
	};

	static const char* RunColName[] =
	{
		"pri.E",
		"pri.x",
		"pri.y",
		"pri.z",
		"pri.px",
		"pri.py",
		"pri.pz",
		"op.scint",
		"op.wls",
		"op.det",
		"sd.Edep"
	};

	enum VertexType{
		Primary = 0,
		Final = 1,
		Scintillation = 2,
		Scint2Groove = 3,
		Groove2Cladding = 4,
		Cladding2Core = 5,
		OpWLS = 6,
		Fiber2Pmt = 7,
		PmtSD = 8
	};

	enum VertexInfoID {
		Type, // 0
		TrackID,
		Energy,
		Time,
		PositionX,
		PositionY,
		PositionZ,
		DirectionX,
		DirectionY,
		DirectionZ	// 9
	};

	static const char* VertexInfoName[] =
	{
		"type",		// VertexType == VertexID
		"trackID",	// TrackID to idendtify track
		"Ek",		// Kinetic Energy
		"time",		// Global Time
		"x",		// PositionX
		"y",		// PositionY
		"z",		// PositionZ
		"px",		// DirectionX
		"py",		// DirectionY
		"pz"		// DirectionZ
	};

	static G4bool FillOutputForRun(const G4Event* theEvent, G4double sdEdep)
	{
		if(CURRENT_NTUPLE <= 0) return false;
		G4int ntupleID = 0;

		G4AnalysisManager* rootData = G4AnalysisManager::Instance();

		G4PrimaryVertex *priV = theEvent->GetPrimaryVertex();
		G4PrimaryParticle *priP = priV->GetPrimary();
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_E, priP->GetKineticEnergy() / MeV);
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_X, priV->GetX0() / cm);
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_X, priV->GetX0() / cm);
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_Y, priV->GetY0() / cm);
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_Z, priV->GetZ0() / cm);
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_PX, priP->GetPx() );
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_PY, priP->GetPy() );
		rootData->FillNtupleDColumn(ntupleID, PRIMARY_PZ, priP->GetPz() );

		OpRecorder* Recorder = OpRecorder::Instance();
		rootData->FillNtupleIColumn(ntupleID, NUM_SCINT, Recorder->nScintTotal);
		rootData->FillNtupleIColumn(ntupleID, NUM_WLS, Recorder->nWlsEmit);
		rootData->FillNtupleIColumn(ntupleID, NUM_DETECTION, Recorder->nDetection);

		// TODO : SD method
		rootData->FillNtupleDColumn(ntupleID, EnergyDeposit, sdEdep / MeV);

		return rootData->AddNtupleRow(ntupleID);
	}

	static G4bool FillVertexForEvent(const G4Track* theTrack, VertexType type){
		if(CURRENT_NTUPLE <= 0)
			return false;
		G4int ntupleID = CURRENT_NTUPLE;

		G4AnalysisManager* rootData = G4AnalysisManager::Instance();

		rootData->FillNtupleIColumn(ntupleID, Type, type);
		rootData->FillNtupleIColumn(ntupleID, TrackID, theTrack->GetTrackID());

		rootData->FillNtupleDColumn(ntupleID, Energy, theTrack->GetKineticEnergy() / eV);
		rootData->FillNtupleDColumn(ntupleID, Time, theTrack->GetGlobalTime());
		
		G4ThreeVector position = theTrack->GetPosition();
		rootData->FillNtupleDColumn(ntupleID, PositionX, position.x()/ cm);
		rootData->FillNtupleDColumn(ntupleID, PositionY, position.y()/ cm);
		rootData->FillNtupleDColumn(ntupleID, PositionZ, position.z()/ cm);

		G4ThreeVector direction = theTrack->GetMomentumDirection();
		rootData->FillNtupleDColumn(ntupleID, DirectionX, direction.x()/ cm);
		rootData->FillNtupleDColumn(ntupleID, DirectionY, direction.y()/ cm);
		rootData->FillNtupleDColumn(ntupleID, DirectionZ, direction.z()/ cm);

		return rootData->AddNtupleRow(ntupleID);
	}

	static std::vector<double> vTest;
	static G4bool CreateNtupleForRun(){
		if(CURRENT_NTUPLE != -1) return false;
		
		G4AnalysisManager *rootData = G4AnalysisManager::Instance();
		
		G4int ntupleID = rootData->CreateNtuple(
			"Run", "Run sammary & SD output");

		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_E]);  // id = 0
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_X]); // id = 1
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_Y]);
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_Z]);	// eV
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_PX]);	// ns
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_PY]); // cm
		rootData->CreateNtupleDColumn(ntupleID, RunColName[PRIMARY_PZ]);	// cm
		rootData->CreateNtupleIColumn(ntupleID, RunColName[NUM_SCINT]);
		rootData->CreateNtupleIColumn(ntupleID, RunColName[NUM_WLS]);
		rootData->CreateNtupleIColumn(ntupleID, RunColName[NUM_DETECTION]);	// id = NUM_DETECTION

		// TODO : Replace with SD method
		rootData->CreateNtupleDColumn(ntupleID, RunColName[EnergyDeposit]);	

		// DEBUG
		vTest.push_back(1.0);
		vTest.push_back(2.0);

		rootData->CreateNtupleDColumn(ntupleID, "test", vTest);

		rootData->FinishNtuple(ntupleID);

		CURRENT_NTUPLE = ntupleID;		
		
		return true;
	}
	static G4int CreateNtupleForEvent(G4int eventID){

		G4AnalysisManager *rootData = G4AnalysisManager::Instance();
		
		G4int ntupleID = rootData->CreateNtuple(
			"Event"+std::to_string(eventID),
			"Storege Event Vertex.");

		rootData->CreateNtupleIColumn(ntupleID, VertexInfoName[Type]);  // id = 0
		rootData->CreateNtupleIColumn(ntupleID, VertexInfoName[TrackID]); // id = 1
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[Energy]);	// eV
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[Time]);	// ns
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[PositionX]); // cm
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[PositionY]);	// cm
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[PositionZ]);	// cm
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[DirectionX]);
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[DirectionY]);
		rootData->CreateNtupleDColumn(ntupleID, VertexInfoName[DirectionZ]);	// id = DirectionZ

		rootData->FinishNtuple(ntupleID);

		CURRENT_NTUPLE = ntupleID;
		return ntupleID;
	}

} // namespace ana

enum VertexType{
	Primary,
	Final,
	Scintillation,
	Scint2Groove,
	Groove2Cladding,
	Cladding2Core,
	OpWLS,
	Fiber2Pmt,
	Photocathode
};

class CryPostionSD;
//class PmtSD;

typedef struct MuonTrack{
	std::vector<double> Ek;
	std::vector<double> time;
	std::vector<double> x;
	std::vector<double> y;
	std::vector<double> z;
	std::vector<double> px;
	std::vector<double> py;
	std::vector<double> pz;
}_Muon;

class Analysis {
public:
	Analysis();
	~Analysis();
public:
	static Analysis* Instance();
	
	G4bool OpenFile();
	G4bool SaveFile();

	G4int CreateNtupleForEvent(G4int eventID);
	G4bool CreateNtupleForRun();

	G4bool FillMuonTrackForRun(const G4Track* theMuon);

	G4bool FillVertexForEvent(
		const G4Track* theTrack, VertexType type);
	G4bool FillEntryForRun(const G4Event* theEvent);
	
	//G4bool RegisterSD(CrySD*);

private:
	static Analysis* fgInstance;

private:
	G4RootAnalysisManager* rootData;
	G4int fCurrentNtuple;
	//std::vector<CrySD*> fCrySD;
	_Muon* fMuon;
};

#endif // CRTest_Analysis_h