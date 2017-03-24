/**
*   FILE : CRTest/src/OpRecorder.cc
*   Brief: Implementation of class OpRecorder
*/

#include "OpRecorder.hh"

OpRecorder* OpRecorder::fgInstance = 0;

OpRecorder::OpRecorder()
    : nScintTotal(0), nScintToFiber(0), nWlsEmit(0)
{

}

OpRecorder::~OpRecorder() {}

OpRecorder *OpRecorder::Instance()
{
    if (fgInstance == NULL)
        fgInstance = new OpRecorder();
    return fgInstance;
}

void OpRecorder::Reset(){
    nScintTotal = 0;
    nScintToFiber = 0;
    nWlsEmit = 0;
}