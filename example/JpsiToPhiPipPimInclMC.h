/**************************************************************
 *                                                            *            
 *  TopoMgr                                                   *
 *                                                            *
 *  Topology handling in BESIII                               *
 *                                                            *
 *  Author: Markus Kuhlmann, Marvin Richter                   *
 *   email: mkuhlmann@ep1.rub.de, mrichter@ep1.rub.de         *
 *                                                            *
 *  Copyright (C) 2016  Markus Kuhlmann, Marvin Richter       *
 *                                                            *
 *                                                            *
 *  Description: Topology handling in BESIII                  *
 *                                                            *
 *  License:                                                  *
 *                                                            *
 *  This file is part of TopoMgr                              *
 *                                                            *
 *  TopoMgr is free software: you can redistribute it and/or  *
 *  modify it under the terms of the GNU General Public       *
 *  License as published by the Free Software Foundation,     *
 *  either version 3 of the License, or (at your option) any  *
 *  later version.                                            *
 *                                                            *
 *  TopoMgr is distributed in the hope that it will be useful,*
 *  but WITHOUT ANY WARRANTY; without even the implied        *
 *  warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR   *
 *  PURPOSE. See the GNU General Public License for more      *
 *  details.                                                  *
 *                                                            *
 *  You should have received a copy of the GNU General        *
 *  Public License along with TopoMgr (license.txt). If not,  *
 *                                                            *
 *                                                            *
 *  see <http://www.gnu.org/licenses/>.                       *
 *                                                            *
 *************************************************************/

#ifndef JpsiToPhiPipPimInclMC_h
#define JpsiToPhiPipPimInclMC_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include "../TopoMgr.hpp" //Include the TopoMgr object here like this


class JpsiToPhiPipPimInclMC : public TSelector {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  TopoMgr *mgr;

  

   // Declaration of leaf types...
   
   // List of branches...
   
   JpsiToPhiPipPimInclMC(TTree * /*tree*/ =0) : fChain(0) { }
   virtual ~JpsiToPhiPipPimInclMC() { }
   virtual Int_t   Version() const { return 2; }
   virtual void    Begin(TTree *tree);
   virtual void    SlaveBegin(TTree *tree);
   virtual void    Init(TTree *tree);
   virtual Bool_t  Notify();
   virtual Bool_t  Process(Long64_t entry);
   virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
   virtual void    SetOption(const char *option) { fOption = option; }
   virtual void    SetObject(TObject *obj) { fObject = obj; }
   virtual void    SetInputList(TList *input) { fInput = input; }
   virtual TList  *GetOutputList() const { return fOutput; }
   virtual void    SlaveTerminate();
   virtual void    Terminate();

   ClassDef(JpsiToPhiPipPimInclMC,0);
};

#endif

#ifdef JpsiToPhiPipPimInclMC_cxx
void JpsiToPhiPipPimInclMC::Init(TTree *tree)
{
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  // Set branch addresses and branch pointers...
}

Bool_t JpsiToPhiPipPimInclMC::Notify()
{
  return kTRUE;
}

#endif // #ifdef JpsiToPhiPipPimInclMC_cxx
