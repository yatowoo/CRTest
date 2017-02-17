# CRTest

Cosmic-Ray Test Platform for particle detectors

Based on Geant4 framework, version 4.10

# Structure

* CRTest.cxx
  * // Main function

* CRTestMessenger
  * // Deal with User Options

* CRTestDetectorConstruction

* CRTestVDetector
  * // Inherited from G4LogicalVolume
  
  * CRTestUnifiedDetector

  * CRTestScintDetector

* CRTestSensitiveDetector

* CRTestVParticleGenerator

  * CRTestPDGGenerator
 
  * CRTestCRYGenerator
  
  * CRTestPurdueGenerator

* CRTestPhysicsList

* CRTestRunAction

* CRTestRunInitialization

* CRTestEventAction

* CRTestTrackingAction

* CRTestSteppingAction

# Configuration

* Geometry

* Detector

* Target

* Generator

* Physicslist

* Readout

# Author

Yitao WU @ USTC-HEPG410

Email : torrecne@mail.ustc.edu.cn

All Rights Reserved &copy; 2017.