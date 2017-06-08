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
#define JpsiToPhiPipPimInclMC_cxx

#include "JpsiToPhiPipPimInclMC.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>
#include <vector>

void JpsiToPhiPipPimInclMC::Begin(TTree * /*tree*/)
{
  //TOPOMGR THIS GOES TO THE BEGINNING OF YOUR BEGIN LOOP
  mgr = new TopoMgr(); //Instatiate TopoMgr Object
  mgr->PrepareTable("../pdtIDs.csv"); //Hand over path to pdtTable. This should not be altered
  mgr->RegisterStable(321); //Here you can exclude specific decays by marking particles by theri stdHEPID as stable. This should be widely used, since it reduces runtime significantly
  mgr->RegisterStable(-321);
  mgr->RegisterStable(211);
  mgr->RegisterStable(-211);
  mgr->RegisterIrrelevant(-22); //Here you can exclude specific particles by their stdHEPID like final state gammas
  //\TOPOMGR THIS GOES TO THE BEGINNING OF YOUR BEGIN LOOP
}

void JpsiToPhiPipPimInclMC::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
}

Bool_t JpsiToPhiPipPimInclMC::Process(Long64_t entry)
{
  //TOPOMGR THIS GOES TO THE END OF YOUR PROCESS LOOP
  std::vector<SubDecay> subs; //Create vecot of SubDecay type to temporarily hold topology structure
  subs.resize(0);
  
  for(int mcLength = 0; mcLength < mcLen; mcLength++){ //fill your vector with the components of the MC array. Please inspect your structures very carefully here and exclude positions in array, that you dont need (Like gamma from cluster)
    SubDecay tmpDec = SubDecay(mcIdx[mcLength],mcLund[mcLength],mcMothIdx[mcLength]+mcIdx[0],mcMothLund[mcLength]);
    subs.push_back(tmpDec);
  }
  mgr->RegisterTopology(subs); //Register Topology from your vector like this
  //\TOPOMGR THIS GOES TO THE END OF YOUR PROCESS LOOP
  return kTRUE;
}

void JpsiToPhiPipPimInclMC::SlaveTerminate()
{
}

void JpsiToPhiPipPimInclMC::Terminate()
{
  //TOPOMGR THIS GOES TO THE END OF YOUR TERMINATE LOOP
   mgr->TrimTopos(); //This will trim your topologies according to StableParticles and IrrelevantParticles
   mgr->countTopos(); //This is will characterize your topologies and count their occurences
   mgr->Result("TopoMgrResult.dat"); //This will write a result file to the path specified as argument
  //\TOPOMGR THIS GOES TO THE END OF YOUR TERMINATE LOOP
}

