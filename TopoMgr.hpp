#ifndef TOPOMGR_HPP
#define TOPOMGR_HPP

#define PRIMARY 0
#define SIGNAL 1

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>


class SubDecay
{
private:
  long mcIDX;
  long mcLund;
  long mcMotherLund;
  long mcMotherIDX;
  friend bool operator<( const SubDecay& lhs, const SubDecay& rhs ) { return lhs.mcIDX < rhs.mcIDX; }
public:
  SubDecay(){
    mcIDX = 0;
    mcLund = 0;
    mcMotherLund = 0;
    mcMotherIDX = 0;
  }	
  SubDecay(long _mcIDX, long _mcLund, long _mcMotherIDX, long _mcMotherLund){
    mcIDX = _mcIDX;
    mcLund = _mcLund;
    mcMotherLund = _mcMotherLund;
    mcMotherIDX = _mcMotherIDX;
  }
  long Idx(){
    return mcIDX;
  }
  long Lund(){
    return mcLund;
  }
  long MotherIdx(){
    return mcMotherIDX;
  }
  long MotherLund(){
    return mcMotherLund;
  }
	
};

class Decay
{
private:
  long mother;
  std::vector<long> daughters;
  friend bool operator<( const Decay& lhs, const Decay& rhs ){ 
    if(lhs.mother < rhs.mother) return true; 
    else if(lhs.mother > rhs.mother) return false;
    return lhs.daughters < rhs.daughters;
  }
  
public:
  Decay(){
    mother = 0;
    daughters.resize(0);
  }
  Decay(long _motherIn, std::vector<long> _daughtersIn){
    mother = _motherIn;
    daughters = _daughtersIn;
  }
  std::vector<long> GetDaughters(){
    return daughters;
  }
  long GetMother(){
    return mother;
  }
  void Print(){
    std::cout << mother << " ->";
    for(unsigned int i = 0; i < daughters.size(); i++){
      std::cout << " " << daughters[i];
    }
    return;
  }
  bool operator==(Decay _inDecay){
    if(mother != _inDecay.mother)return false;
    if(daughters == _inDecay.daughters)return true;
    return false;
  }
};

class DecayLayer
{
private:
  std::vector<Decay> decays;
  int layerN;
  friend bool operator<( const DecayLayer& lhs, const DecayLayer& rhs ){
    return lhs.layerN < rhs.layerN;
  }
public:
  DecayLayer(){
    decays.resize(0);
  }
  DecayLayer(std::vector<Decay> _decaysIn, int _inlayerN){
    layerN = _inlayerN;
    decays = _decaysIn;
  }
  void SetLayerN(int _inlayerN){
    layerN = _inlayerN;
    return;
  }
  void AddDecay(Decay _decayIn){
    decays.push_back(_decayIn);
    return;
  }
  std::vector<Decay> GetDecays(){
    return decays;
  }
  int GetNDecays(){
    return decays.size();
  }
  bool operator==(DecayLayer _inDecayLayer){
    std::sort (_inDecayLayer.decays.begin(), _inDecayLayer.decays.end());
    if(decays.size() != _inDecayLayer.decays.size()) return false;
    do{
      bool somethingDifferent = false;
      for(unsigned int i = 0; i < decays.size(); i++){
        if(!(decays[i] == _inDecayLayer.decays[i])) somethingDifferent = true;
      }
      if(!somethingDifferent)return true;
    } 
    while(std::next_permutation(_inDecayLayer.decays.begin(), _inDecayLayer.decays.end()));
    return false;
  }
};

class Topology
{
private:
  std::vector<SubDecay> subDecays;
  std::vector<DecayLayer> layers;
  bool chainbuilt;
  long IdToLund(long _inID){
    long Lund = 0;
    for(unsigned int i = 0; i < subDecays.size(); i++){
        if(_inID == subDecays[i].Idx()) Lund = subDecays[i].Lund();
    }
    return Lund;
  }
  std::vector<long> GetPrimaries(){
    std::vector<long> primaries;
    for(unsigned int i = 0; i < subDecays.size(); i++){
      bool motherFound = false;
      for(unsigned int j = 0; j < subDecays.size(); j++){
        if(subDecays[i].MotherIdx() == subDecays[j].Idx()) motherFound = true;
      }
      if(!motherFound) primaries.push_back(subDecays[i].Idx());
    }
    return primaries;
  }
  std::vector<long> GetDaughters(long _inID){
    std::vector<long> daughters;
    for(unsigned int i = 0; i < subDecays.size(); i++){
      if(subDecays[i].MotherIdx() == _inID) daughters.push_back(subDecays[i].Idx());
    }
    return daughters;
  }
  bool buildChain(){
    int layerNhere = 0;
    std::vector<long> prim = GetPrimaries();
    std::vector<long> primLunds;
    for(unsigned int i = 0; i < prim.size(); i++){
      primLunds.push_back(IdToLund(prim[i]));
    }
    DecayLayer primLayer;
    std::sort (primLunds.begin(), primLunds.end()); 
    Decay PrimDecay(0, primLunds);
    primLayer.SetLayerN(layerNhere);
    primLayer.AddDecay(PrimDecay);
    layers.push_back(primLayer);
    while(true){
      layerNhere++;
      std::vector<long> tmp;
      tmp.resize(0);
      DecayLayer tmpLayer;
      tmpLayer.SetLayerN(layerNhere);
      bool foundSomething = false;
      for(unsigned int i = 0; i < prim.size(); i++){
        std::vector<long> tmpDaughters = GetDaughters(prim[i]);
        if(tmpDaughters.size() != 0){
          tmp.insert(tmp.begin(),tmpDaughters.begin(),tmpDaughters.end());
          for(unsigned int j = 0; j < tmpDaughters.size(); j++){
            tmpDaughters[j] = IdToLund(tmpDaughters[j]);
          }
          std::sort (tmpDaughters.begin(), tmpDaughters.end());
          Decay tmpDecay(IdToLund(prim[i]), tmpDaughters);
          tmpLayer.AddDecay(tmpDecay);
          foundSomething = true;
        }
        
      }
      if(!foundSomething)break;
      layers.push_back(tmpLayer);
      prim = tmp;
    }
    
    
    return true;
  }
public:
	
  Topology(){
    subDecays.resize(0);
    layers.resize(0);
    chainbuilt = false;
  }
  Topology(std::vector<SubDecay> _inVec){
    subDecays = _inVec;
    chainbuilt = buildChain();
  }
  bool GetStatus(){
    return chainbuilt;
  }
  int RetSubDecCount(){
    return subDecays.size();
  }
  SubDecay RetSubDec(int _nr){
    return subDecays[_nr];
  }
  void AddSubDec(SubDecay _inSubDec){
    subDecays.push_back(_inSubDec);
    return;
  }
  std::vector<SubDecay> GetVec(){
    return subDecays;
  }
  std::vector<DecayLayer> GetLayers(){
    return layers;
  }
  std::vector<long> findDerived(long _inID){
    std::vector<long> result;
    result.clear();
    std::vector<long> lookForMotherID;
    lookForMotherID.clear();
    lookForMotherID.push_back(_inID);
    std::vector<long> resultTmp;
    resultTmp.clear();
    while(true){
      bool foundSomething = false;
      for(unsigned int i = 0; i < lookForMotherID.size(); i++){
	for(unsigned int j = 0; j < subDecays.size(); j++){
	  if(subDecays[j].MotherIdx() == lookForMotherID[i]){
	    result.push_back(subDecays[j].Idx());
	    resultTmp.push_back(subDecays[j].Idx());
	    foundSomething = true;
	  }
	}
      }
      lookForMotherID = resultTmp;
      resultTmp.clear();
      if(!foundSomething) break;		
    }
    lookForMotherID.clear();
    resultTmp.clear();
		
    return result;
  }
  bool operator==(Topology _inTopo){
    if(layers.size() != _inTopo.GetLayers().size()) return false;
    do{
      bool somethingDifferent = false;
      for(unsigned int i = 0; i < layers.size(); i++){
        if(!(layers[i] == _inTopo.GetLayers()[i])) somethingDifferent = true;
      }
      if(!somethingDifferent)return true;
    } 
    while(std::next_permutation(layers.begin(), layers.end()));
    return false;
  }
  //THIS IS THE OLD WORKING OPERATOR
  /*bool operator==(Topology _inTopo){
    std::vector<SubDecay> inVec = _inTopo.GetVec();
    if(subDecays.size() != inVec.size()){
      return false;
    }
    std::vector<int> test, testIn;
    for(unsigned int i = 0; i < subDecays.size(); i++){
      test.push_back(subDecays[i].Lund());
      testIn.push_back(inVec[i].Lund());
    }
    std::sort (test.begin(), test.end());
    std::sort (testIn.begin(), testIn.end());
    if(test!=testIn)return false;
    bool different = true;
    do{
      bool combinationdifferent = false;
      for(unsigned int i = 0; i < subDecays.size(); i++){
        if(subDecays[i].Lund() != inVec[i].Lund() || subDecays[i].MotherLund() != inVec[i].MotherLund()){
	  combinationdifferent = true;
	}
      }
      if(combinationdifferent == false) return true;
    } while(std::next_permutation(subDecays.begin(), subDecays.end()));
    return false;
  }*/
};

class Rule
{
private:
  long decaying;
  std::vector<long> decayingTo;
public:
  Rule(){}
  Rule(long _decaying, std::vector<long> _decayingTo){
    decaying = _decaying;
    decayingTo = _decayingTo;
  }
  long GetDecaying(){
    return decaying;
  }
  std::vector<long> GetDecayingTo(){
    return decayingTo;
  }	
};

class RuleConnection
{
private:
  Rule rule1;
  Rule rule2;
  bool type;
public:
  RuleConnection(){}
  RuleConnection(Rule _rule1, Rule _rule2, bool _type){
    rule1 = _rule1;
    rule2 = _rule2;
    type = _type;
  }	
};

class RuleManager
{
private:
  long primaryParticle;
  std::vector<Rule> rules;
  std::vector<RuleConnection> ruleConns;
	
public:
  RuleManager(){
    rules.resize(0);
    ruleConns.resize(0);
  }
  void RegisterRule(Rule _inRule){
    rules.push_back(_inRule);
  }
  void RegisterRuleConnection(RuleConnection _inRuleConnection){
    ruleConns.push_back(_inRuleConnection);
  }
  bool ProcessRule(Topology _inTopo, Rule _inRule){
    std::vector<SubDecay> subDecays = _inTopo.GetVec();
    std::vector<long> decayingCand;
    decayingCand.clear();
    for(unsigned int i = 0; i < subDecays.size(); i++){
      if(subDecays[i].Lund() == _inRule.GetDecaying()){
	decayingCand.push_back(subDecays[i].Idx());
				
      }
    }
    bool decayingIsDaughter = false;
    std::vector<long> candMotherIds;
    candMotherIds.clear();
    unsigned int cnt = 0;
    for(unsigned int i = 0; i < decayingCand.size(); i++){
      for(unsigned int j = 0; j < subDecays.size(); j++){
	for(unsigned int k = 0; k < _inRule.GetDecayingTo().size(); k++){
	  if(subDecays[j].Lund() == _inRule.GetDecayingTo()[k] && subDecays[j].MotherIdx() == decayingCand[i]){
	    cnt++;
	    candMotherIds.push_back(subDecays[j].MotherIdx());
	  }
	}
      }	
    }
    unsigned int candMotherTo = 0;
    for(unsigned int l = 0; l < subDecays.size(); l++){
      if(subDecays[l].MotherLund()==_inRule.GetDecaying()){
        candMotherTo++;
      }
    }	
    if(cnt == _inRule.GetDecayingTo().size() && candMotherTo == cnt) return true;
    else return false;
  }
};

class TopoMgr
{
private:
  long primaryParticle;
  std::vector<long> idVector;
  std::vector<long> lundkcVector;
  std::vector<float> massVector;
  std::vector<float> widthVector;
  std::vector<float> lifetimeVector;
  std::vector<float> maxdmVector;
  std::vector<int> chargeVector;
  std::vector<int> spinVector;
  std::vector<std::string> nameVector;
  std::vector<std::string> typeVector;
  std::vector<long> stableParticles;
  std::vector<long> irrelevantParticles;
  std::vector<Topology> topos;
  std::vector<Topology> countedTopos;
  std::vector<int> toposCount;

  void init(){
    idVector.resize(0);
    nameVector.resize(0);
    typeVector.resize(0);
    stableParticles.resize(0);
    topos.resize(0);
    countedTopos.resize(0);
    toposCount.resize(0);
    primaryParticle=0;
    rmgr = RuleManager();
  }
  std::vector<std::string> readFile(std::string _inPath){
    std::vector<std::string> result;
    std::ifstream file(_inPath.c_str());
    if (file.is_open()){
      std::string line = "";
      while(getline (file,line)){
	if(line.at(0) != '#'){
	  result.push_back(line);
	}
      }
      file.close();
    }
    else{
      std::cout << "PDT table file not readable! Exiting!" << std::endl;
      exit(1);
    }
    return result;
  }
  std::vector<std::string> readLine(std::string _inLine){
    std::vector<std::string> result;
    std::string delim = " ";
    std::size_t delimfound;
    std::string remainingstring = _inLine;
    while(true){
      delimfound = remainingstring.find(delim);
      if(delimfound!=std::string::npos){
	result.push_back(remainingstring.substr(0, delimfound));
	remainingstring = remainingstring.substr(delimfound+1, remainingstring.length());
      }
      else{
	result.push_back(remainingstring);
	break;
      }
    }
    return result;
  }
  void PrintDecay(Decay _inDecay){
    std::cout << Conv(_inDecay.GetMother()) << " ->";
    for(unsigned int i = 0; i < _inDecay.GetDaughters().size(); i++){
      std::cout << " " << Conv(_inDecay.GetDaughters()[i]);
    }
    return;
  }
  void ResultDecay(Decay _inDecay, std::stringstream &_inStream){
    _inStream << Conv(_inDecay.GetMother()) << " ->";
    for(unsigned int i = 0; i < _inDecay.GetDaughters().size(); i++){
      _inStream << " " << Conv(_inDecay.GetDaughters()[i]);
    }
    return;
  }
  void PrintDecayLayer(DecayLayer _inLayer){
    for(unsigned int i = 0; i < _inLayer.GetDecays().size(); i++){
        PrintDecay(_inLayer.GetDecays()[i]);
        if(i != _inLayer.GetDecays().size()-1)std::cout << ", ";
      }
    return;
  }
  void ResultDecayLayer(DecayLayer _inLayer, std::stringstream &_inStream){
    for(unsigned int i = 0; i < _inLayer.GetDecays().size(); i++){
        ResultDecay(_inLayer.GetDecays()[i], _inStream);
        if(i != _inLayer.GetDecays().size()-1) _inStream << ", ";
      }
    return;
  }
  void readPdtTable(std::string _inPath){
    std::vector<std::string> lines = readFile(_inPath.c_str());
		
    for(unsigned int i = 0; i < lines.size(); i++){
      std::vector<std::string> lineContent = readLine(lines[i]);
      idVector.push_back(atol(lineContent[2].c_str()));
      nameVector.push_back(lineContent[1]);
      typeVector.push_back(lineContent[0]);
      lundkcVector.push_back(atol(lineContent[9].c_str()));
      massVector.push_back(atof(lineContent[3].c_str()));
      widthVector.push_back(atof(lineContent[4].c_str()));
      lifetimeVector.push_back(atof(lineContent[8].c_str()));
      maxdmVector.push_back(atof(lineContent[5].c_str()));
      chargeVector.push_back(atoi(lineContent[6].c_str()));
      spinVector.push_back(atoi(lineContent[7].c_str()));
    }
    return;
  }
  void credits(){
    std::cout << std::endl;
    std::cout << "Thank you for using TopoMgr" << std::endl;
    std::cout << "Sharing is caring: http://mkuhlmann@jollyj.ep1.rub.de/mkuhlmann/TopoMgr.git" << std::endl;
    std::cout << "Dont forget to cite! ;-)" << std::endl;
  }
  

 
public:
  RuleManager rmgr;
  TopoMgr(){
    init();
  }
  void PrepareTable(std::string _inPath){
    readPdtTable(_inPath);
  }
  std::string Conv(long inId){
    std::string result = "";
    for(unsigned int i = 0; i < idVector.size(); i++){
      if(idVector[i] == inId){
	result = nameVector[i];
      }
    }
    return result;
  }
  long Conv(std::string inName){
    long result = 0;
    for(unsigned int i = 0; i < nameVector.size(); i++){
      if(nameVector[i].compare(inName) == 0){
	result = idVector[i];
      }
    }
    return result;
  }
  void RegisterTopology(std::vector<SubDecay> _inVec){
    topos.push_back(Topology(_inVec));
    return;
  }
  void RegisterStable(long _inStable){
    stableParticles.push_back(_inStable);
    return;
  }
  void RegisterIrrelevant(long _inIrrelevant){
    irrelevantParticles.push_back(_inIrrelevant);
    return;
  }
  void RegisterPrimaryParticle(long _inPrimary){
    primaryParticle = _inPrimary;
    return;
  }
  long ToposRegistered(){
    return topos.size();
  }
  Topology GetTopo(int _in){
    return topos[_in];
  }
  Topology GetCountedTopo(int _in){
    return countedTopos[_in];
  }
  std::vector<Topology> GetCountedTopos(){
    return countedTopos;
  }
  int GetTopoCount(int _in){
    return toposCount[_in];
  }
  std::vector<int> GetTopoCounts(){
    return toposCount;
  }
  long GetPrimary(){
    return primaryParticle;
  }
  void TrimTopos(){
    std::cout << "Now attempting to trim topologies" << std::endl;
    for(unsigned int i = 0; i < topos.size(); i++){
      std::vector<SubDecay> tmpVec = topos[i].GetVec();
      std::vector<long> stable;
      std::vector<long> irrelevant;
      stable.resize(0);
      irrelevant.resize(0);
      for(unsigned int k = 0; k < tmpVec.size(); k++){
        for(unsigned int j = 0; j < irrelevantParticles.size(); j++){
          if(tmpVec[k].Lund() == irrelevantParticles[j]){
	    irrelevant.push_back(tmpVec[k].Idx());
	  }
        }
	for(unsigned int j = 0; j < tmpVec.size(); j++){
	  if(tmpVec[k].Lund() == stableParticles[j]){
	    stable.push_back(tmpVec[k].Idx());
	  }
					
	}
				
      }
      std::vector<long> toBeDeleted;
      toBeDeleted.resize(0);
      for(unsigned int l = 0; l < stable.size(); l++){
	std::vector<long> daughters = topos[i].findDerived(stable[l]);
	toBeDeleted.insert(toBeDeleted.begin(),daughters.begin(),daughters.end());
      }
      for(unsigned int l = 0; l < irrelevant.size(); l++){
	toBeDeleted.insert(toBeDeleted.begin(),irrelevant.begin(),irrelevant.end());
      }
      while(true){
	bool deleted = false;
	for(unsigned int m = 0; m < toBeDeleted.size(); m++){
	  for(unsigned int n = 0; n < tmpVec.size(); n++){
	    if(toBeDeleted[m] == tmpVec[n].Idx()){
	      tmpVec.erase(tmpVec.begin() + n);
	      deleted = true;
	    }
	    if(deleted) break;
	  }
	  if(deleted) break;				
	}
	if(!deleted) break;
      }
      topos[i]=Topology(tmpVec);
    }
    std::cout << "Finished trimming topologies" << std::endl;
    return;
  }
  void PrintTopo(Topology _inTopo){
    for(unsigned int i = 0; i < _inTopo.GetLayers().size(); i++){
      PrintDecayLayer(_inTopo.GetLayers()[i]);
      std::cout << std::endl;
    }
    return;
  }
  void PrintTopo(int _in){
    std::vector<SubDecay> tmpVec = topos[_in].GetVec();
    for(unsigned int i = 0; i < tmpVec.size(); i++){
      std::cout << "Particle with ID: " << tmpVec[i].Idx() << " in topology has stdhepid: " << tmpVec[i].Lund() << " and mother has stdhepid: " << tmpVec[i].MotherLund() << " and ID: " << tmpVec[i].MotherIdx() << std::endl;
    }
    std::cout << std::endl;
    return;
  }
  void countTopos(){
    std::cout << "Now attempting to count topologies" << std::endl;
    countedTopos.push_back(topos[0]);
    toposCount.push_back(1);
    for(unsigned int i = 1; i < topos.size(); i++){
      //if(i%1000 == 0)std::cout << "Processed Topologies: " << i << std::endl;
      bool found = false;
      for(unsigned int j = 0; j < countedTopos.size(); j++){
	if(topos[i]==countedTopos[j]){
	  toposCount[j] = toposCount[j]+1;
	  found = true;
	}
      }
      if(!found){
	countedTopos.push_back(topos[i]);
	toposCount.push_back(1);
      }
    }
    
    std::vector< std::pair<Topology,int> > intermediate;
    for(unsigned int i = 0; i < toposCount.size() ; i++){
      std::pair<Topology,int> indermediatePair(countedTopos[i],toposCount[i]);
      intermediate.push_back(indermediatePair);
    }
    std::sort (intermediate.begin(), intermediate.end(), compareFunc );
    std::reverse(intermediate.begin(), intermediate.end());
    for(unsigned int i = 0; i < toposCount.size() ; i++){
      toposCount[i]=intermediate[i].second;
      countedTopos[i]=intermediate[i].first;
    }
    std::cout << "Finished counting topologies" << std::endl;
    return;
  }
  static bool compareFunc(std::pair<Topology,int> &a, std::pair<Topology,int> &b){
    return a.second < b.second;
  }
  void Result(std::string _inPath){
    ofstream resultFile;
    std::stringstream tmpStream;
    int total = 0;
    for(unsigned int i = 0; i < toposCount.size(); i++){
      total = total + toposCount[i];
    }
    tmpStream << "Analysis finished!" << std::endl << "Events analyzed: " << total << std::endl << "Different Topologies found: " << toposCount.size() << std::endl << std::endl;
    resultFile.open (_inPath.c_str());
    if (!resultFile.is_open()){
      std::cout << "File could not be opened! Exiting!" << std::endl;
      return;
    }
    for(unsigned int i = 0; i < countedTopos.size(); i++){
      tmpStream << "#######################Topology Begin#######################" << std::endl;
      for(unsigned int j = 0; j < countedTopos[i].GetLayers().size(); j++){
        for(unsigned int k = 0; k < j; k++){
          tmpStream << "\t";
        }
        ResultDecayLayer(countedTopos[i].GetLayers()[j], tmpStream);
        tmpStream << std::endl;
      }
    tmpStream << "Count: " << toposCount[i] << " / " << 100.0*((double)toposCount[i]/(double)total) << "%" << std::endl;
    tmpStream << "########################Topology End########################"<< std::endl << std::endl;
    }
    resultFile << tmpStream.str();
    resultFile.close();
    std::cout << "Result file placed at: " <<  _inPath << std::endl;
    credits();
  }
    
};

#endif
//EOF

