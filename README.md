# CRTest

Cosmic-Ray Test Platform for particle detectors

Based on Geant4 framework, version 4.10

Dependencies : ROOT ()

# Introduction
* Usage : ./CRTest [.gdml] [.mac] [.root] [seed]
* Output - by G4cout/std::cout
  - Initialize : banner, config(Argument), geometry(GdmlConstruction), physics(PhysicsList)
  - Run/Em verbose
  - Event -begin & end : Recorder output in EndOfEvent
  - Exit & Run verbose
* ROOT file : Analysis & registered VirtualRecorder/SD

# Structure
* CRTest.cxx
  * Main function
* Argument
	- Read, Check & Handle exe arguments
* Analysis
	- wrapper for G4RootAnalysisManager
  - VirtualRecorder/SD : Create/FillEntry, Record
    - OpRecorder
    - MuonRecorder
    - CryPostionSD
    - PmtSD：sd.pmt[NUM]
* GdmlConstruction
	- G4VUserDetectorConstruction.hh
  * Link SensitiveDetector by LV name
  	* PmtSD <-> "PMT"
    * CryPositionSD <-> "Detector"
  * Link Surface MPT by LV name & Material name 
    * NEW GDML interface for optical surface / G4LogicalSkinSurface by \<userinfo\>
    * Add SurfaceProperty by Material
  * Dump geometry structure
  * UIcmd method : Set/Get material, skinsurface information
* Generator
	- G4VUserPrimaryGeneratorAction
	- Trigger Mode : Generate & Check in user-defined area
  * CryGenerator
  * PduGenerator
  * UIcmd Method
* User Action
	* RunAction：Initialize Analysis & VirtualRecorder/SD
	* EventAction：Reset Recorder, Output information
	* StackAction：Record OpticalPhoton before tracking start
	* TrackingAction
	* StepAction
* SysMessenger
	- G4UImessenger
	* /CRTest/geometry
	* /CRTest/generator
* CRTest_GUI - developing
  - ROOT-GUI

# Configuration
* Geometry : GDML file and UIcmd /CRTest/geometry
* Generator : UIcmd /CRTest/generator 

# Author
Yitao WU @ USTC-HEPG410

Email : torrecne@mail.ustc.edu.cn

License as Geant4 http://cern.ch/geant4/license/