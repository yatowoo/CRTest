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

#include<vector>
#include "G4String.hh"

namespace ana {
	// TODO : Wrap by class Analysis with static & Instance()
	static int CURRENT_NTUPLE = 0;
	static int INDEX_OF_VERTEX_HEAD = 0;
	static int INDEX_OF_SD_HEAD = 0;

	namespace summary {
		static int PRIMARY_E = 0;
		static int PRIMARY_X = 0;
		static int PRIMARY_Y = 0;
		static int PRIMARY_Z = 0;
		static int PRIMARY_PX = 0;
		static int PRIMARY_PY = 0;
		static int PRIMARY_PZ = 0;
		static int NUM_SCINT = 0;
		static int NUM_WLS = 0;
		static int NUM_DETECTION = 0;
	}

	enum VertexID{
		Primary = 0,
		Final = 1,
		Scintillation = 2,
		Scint2Gloove = 3,
		Gloove2Cladding = 4,
		Cladding2Core = 5,
		Fiber2Pmt = 6,
		PmtSD = 7
	};

	static const std::vector<G4String> VertexName ={
		"pri",		// Primary
		"final",	// Final
		"scint",	// Scintillation
		"s2g",		// Scint2Gloove
		"g2c",		// Gloove2Cladding
		"c2c",		// Cladding2Core
		"f2p",		// Fiber2Pmt
		"pmt"		// PmtSD
	};

	enum VertexInfoID {
		ParticleType = 0,
		TrackStatus = 1,
		CreatorProcess = 2,
		Energy = 3,
		Time = 4,
		PositionX = 5,
		PositionY = 6,
		PositionZ = 7,
		DirectionX = 8,
		DirectionY = 9,
		DirectionZ = 10
	};

	static const std::vector<G4String> VertexInfoName =
	{
		"type",		// ParticleType
		"status",	// TrackStatus
		"creator",	// CreatorProcess
		"Ek",		// Energy
		"time",		// Time
		"x",		// PositionX
		"y",		// PositionY
		"z",		// PositionZ
		"px",		// DirectionX
		"py",		// DirectionY
		"pz"		// DirectionZ
	};

	static const std::vector<G4String> ProcessName =
	{
		"Scintillation",
		"OpWLS"
	};

	static int GetProcessID(G4String process)
	{
		for (unsigned int i = 0 ; i < ProcessName.size() ; i ++){
			if(process == ProcessName[i]) return i;
		}
		return -1;
	}

	static const int NUM_OF_VERTEX = VertexName.size();
	static const int NUM_OF_VERTEX_INFO = VertexInfoName.size();

	static int GetColumnIndex(VertexID vid, VertexInfoID viid)
	{
		return (INDEX_OF_VERTEX_HEAD + 1 + vid * NUM_OF_VERTEX + viid);
	}
	static G4String GetColumnName(VertexID vid, VertexInfoID viid){
		return (VertexName[vid] + "." + VertexInfoName[viid]);
	}
	

} // namespace ana

#endif // CRTest_Analysis_h