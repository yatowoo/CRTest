/**
*   FILE : CRTest/src/VirtualRecorder.cc
*   Brief: Implementation of class VirtualRecorder
*/

#include "VirtualRecorder.hh"

VirtualRecorder* VirtualRecorder::fgInstance = NULL;

VirtualRecorder::VirtualRecorder()
	: fFirstColID(-1)
{}

VirtualRecorder* VirtualRecorder::Instance(){
	if(!fgInstance)
		fgInstance = new VirtualRecorder;
}

VirtualRecorder::~VirtualRecorder()
{}