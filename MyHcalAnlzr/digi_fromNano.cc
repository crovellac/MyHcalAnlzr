#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH2.h"
#include "TF1.h"
#include "TProfile.h"
#include "TLegend.h"
#include "TGraph.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <assert.h> 
#include <iomanip>
#include <filesystem>
#include <numeric>

#include <stdlib.h>

using namespace std;

int main(int argc, char *argv[])
{
  /*if(argc!=6){
    cerr << "Invalid arguments provided, correct format is: ./main kind(Full/Ped) days lumi floatday runid\n";
    exit(0);
  }*/

  //string runid = argv[1]; // Integer
  string floatday = argv[1]; // String, e.g. "22.02"
  bool WholeRun = false;
  if(argc>1) WholeRun = argv[2]; // When doing a whole run, floatday will actually be the input filename. LS shall be added to output name
  float XtimesRMS = 3.0; // Set new ZS thresholds to "Mean + X * RMS + Y"
  string StrXtimesRMS = "3"; // Same as above, as string (to_string doesn't work because there's trailing zeros)
  float RMSplusY = 0.0; // Set new ZS thresholds to "Mean + X * RMS + Y"
  float LessForHBM09RM3 = 0.0;
  int ZSminimum = 6; // Define minimum ZS, specifically for HBP14 RM1
  int maxevents = -1; // -1 for all
  

  // New: Get input based on date in name, then find run number
  string fileName;
  for (const auto & entry : filesystem::directory_iterator("/eos/user/c/ccrovell/HCAL/MyHcalAnlzr_Nano/")){
    fileName = entry.path().filename().string();
    if(fileName.length() <= (floatday+".root").length()) continue;
    //if (fileName.find(floatday+".root") != string::npos){
    if (0 == fileName.compare (fileName.length() - (floatday+".root").length(), (floatday+".root").length(), (floatday+".root"))){
      break;
    }
  }
  string runid = fileName.substr(25, 6);

  // There is a DISGUSTING amount of hardcoding here; unfortunately I wasn't able to loop over subdets or time slices.

  cout << "Opening " << fileName << endl;
  TFile *f = new TFile(("/eos/user/c/ccrovell/HCAL/MyHcalAnlzr_Nano/"+fileName).c_str(), "read");
  TNtuple* qiedigi = (TNtuple*)f->Get("Events");
  int ntot = qiedigi->GetEntries();
  cout << "Reading in input file, total " << ntot << " Entries." << endl;
  //float RunNum, LumiNum, EvtNum;
  //float ieta, iphi, depth, sumADC, type, shunt;
  int nDigiHB, nDigiHE, nDigiHF, nDigiHO;
  qiedigi->SetBranchAddress("nDigiHB", &nDigiHB); // always 9072 ?
  qiedigi->SetBranchAddress("nDigiHE", &nDigiHE); // always 6768 ?
  qiedigi->SetBranchAddress("nDigiHF", &nDigiHF); // always 3456 ?
  qiedigi->SetBranchAddress("nDigiHO", &nDigiHO); // always 2160 ?
  int DigiHB_ieta[9072], DigiHE_ieta[6768], DigiHF_ieta[3456], DigiHO_ieta[2160];
  qiedigi->SetBranchAddress("DigiHB_ieta", &DigiHB_ieta);
  qiedigi->SetBranchAddress("DigiHE_ieta", &DigiHE_ieta);
  qiedigi->SetBranchAddress("DigiHF_ieta", &DigiHF_ieta);
  qiedigi->SetBranchAddress("DigiHO_ieta", &DigiHO_ieta);
  int DigiHB_iphi[9072], DigiHE_iphi[6768], DigiHF_iphi[3456], DigiHO_iphi[2160];
  qiedigi->SetBranchAddress("DigiHB_iphi", &DigiHB_iphi);
  qiedigi->SetBranchAddress("DigiHE_iphi", &DigiHE_iphi);
  qiedigi->SetBranchAddress("DigiHF_iphi", &DigiHF_iphi);
  qiedigi->SetBranchAddress("DigiHO_iphi", &DigiHO_iphi);
  int DigiHB_depth[9072], DigiHE_depth[6768], DigiHF_depth[3456], DigiHO_depth[2160];
  qiedigi->SetBranchAddress("DigiHB_depth", &DigiHB_depth);
  qiedigi->SetBranchAddress("DigiHE_depth", &DigiHE_depth);
  qiedigi->SetBranchAddress("DigiHF_depth", &DigiHF_depth);
  qiedigi->SetBranchAddress("DigiHO_depth", &DigiHO_depth);
  int DigiHB_rawId[9072], DigiHE_rawId[6768], DigiHF_rawId[3456], DigiHO_rawId[2160];
  qiedigi->SetBranchAddress("DigiHB_rawId", &DigiHB_rawId);
  qiedigi->SetBranchAddress("DigiHE_rawId", &DigiHE_rawId);
  qiedigi->SetBranchAddress("DigiHF_rawId", &DigiHF_rawId);
  qiedigi->SetBranchAddress("DigiHO_rawId", &DigiHO_rawId);
  float DigiHB_fc0[9072], DigiHB_fc1[9072], DigiHB_fc2[9072], DigiHB_fc3[9072];
  float DigiHB_fc4[9072], DigiHB_fc5[9072], DigiHB_fc6[9072], DigiHB_fc7[9072];
  qiedigi->SetBranchAddress("DigiHB_fc0", &DigiHB_fc0);
  qiedigi->SetBranchAddress("DigiHB_fc1", &DigiHB_fc1);
  qiedigi->SetBranchAddress("DigiHB_fc2", &DigiHB_fc2);
  qiedigi->SetBranchAddress("DigiHB_fc3", &DigiHB_fc3);
  qiedigi->SetBranchAddress("DigiHB_fc4", &DigiHB_fc4);
  qiedigi->SetBranchAddress("DigiHB_fc5", &DigiHB_fc5);
  qiedigi->SetBranchAddress("DigiHB_fc6", &DigiHB_fc6);
  qiedigi->SetBranchAddress("DigiHB_fc7", &DigiHB_fc7);
  int DigiHB_adc0[9072], DigiHB_adc1[9072], DigiHB_adc2[9072], DigiHB_adc3[9072];
  int DigiHB_adc4[9072], DigiHB_adc5[9072], DigiHB_adc6[9072], DigiHB_adc7[9072];
  qiedigi->SetBranchAddress("DigiHB_adc0", &DigiHB_adc0);
  qiedigi->SetBranchAddress("DigiHB_adc1", &DigiHB_adc1);
  qiedigi->SetBranchAddress("DigiHB_adc2", &DigiHB_adc2);
  qiedigi->SetBranchAddress("DigiHB_adc3", &DigiHB_adc3);
  qiedigi->SetBranchAddress("DigiHB_adc4", &DigiHB_adc4);
  qiedigi->SetBranchAddress("DigiHB_adc5", &DigiHB_adc5);
  qiedigi->SetBranchAddress("DigiHB_adc6", &DigiHB_adc6);
  qiedigi->SetBranchAddress("DigiHB_adc7", &DigiHB_adc7);
  int DigiHB_capid0[9072], DigiHB_capid1[9072], DigiHB_capid2[9072], DigiHB_capid3[9072];
  int DigiHB_capid4[9072], DigiHB_capid5[9072], DigiHB_capid6[9072], DigiHB_capid7[9072];
  qiedigi->SetBranchAddress("DigiHB_capid0", &DigiHB_capid0);
  qiedigi->SetBranchAddress("DigiHB_capid1", &DigiHB_capid1);
  qiedigi->SetBranchAddress("DigiHB_capid2", &DigiHB_capid2);
  qiedigi->SetBranchAddress("DigiHB_capid3", &DigiHB_capid3);
  qiedigi->SetBranchAddress("DigiHB_capid4", &DigiHB_capid4);
  qiedigi->SetBranchAddress("DigiHB_capid5", &DigiHB_capid5);
  qiedigi->SetBranchAddress("DigiHB_capid6", &DigiHB_capid6);
  qiedigi->SetBranchAddress("DigiHB_capid7", &DigiHB_capid7);
  float DigiHE_fc0[6768], DigiHE_fc1[6768], DigiHE_fc2[6768], DigiHE_fc3[6768];
  float DigiHE_fc4[6768], DigiHE_fc5[6768], DigiHE_fc6[6768], DigiHE_fc7[6768];
  qiedigi->SetBranchAddress("DigiHE_fc0", &DigiHE_fc0);
  qiedigi->SetBranchAddress("DigiHE_fc1", &DigiHE_fc1);
  qiedigi->SetBranchAddress("DigiHE_fc2", &DigiHE_fc2);
  qiedigi->SetBranchAddress("DigiHE_fc3", &DigiHE_fc3);
  qiedigi->SetBranchAddress("DigiHE_fc4", &DigiHE_fc4);
  qiedigi->SetBranchAddress("DigiHE_fc5", &DigiHE_fc5);
  qiedigi->SetBranchAddress("DigiHE_fc6", &DigiHE_fc6);
  qiedigi->SetBranchAddress("DigiHE_fc7", &DigiHE_fc7);
  int DigiHE_adc0[6768], DigiHE_adc1[6768], DigiHE_adc2[6768], DigiHE_adc3[6768];
  int DigiHE_adc4[6768], DigiHE_adc5[6768], DigiHE_adc6[6768], DigiHE_adc7[6768];
  qiedigi->SetBranchAddress("DigiHE_adc0", &DigiHE_adc0);
  qiedigi->SetBranchAddress("DigiHE_adc1", &DigiHE_adc1);
  qiedigi->SetBranchAddress("DigiHE_adc2", &DigiHE_adc2);
  qiedigi->SetBranchAddress("DigiHE_adc3", &DigiHE_adc3);
  qiedigi->SetBranchAddress("DigiHE_adc4", &DigiHE_adc4);
  qiedigi->SetBranchAddress("DigiHE_adc5", &DigiHE_adc5);
  qiedigi->SetBranchAddress("DigiHE_adc6", &DigiHE_adc6);
  qiedigi->SetBranchAddress("DigiHE_adc7", &DigiHE_adc7);
  int DigiHE_capid0[6768], DigiHE_capid1[6768], DigiHE_capid2[6768], DigiHE_capid3[6768];
  int DigiHE_capid4[6768], DigiHE_capid5[6768], DigiHE_capid6[6768], DigiHE_capid7[6768];
  qiedigi->SetBranchAddress("DigiHE_capid0", &DigiHE_capid0);
  qiedigi->SetBranchAddress("DigiHE_capid1", &DigiHE_capid1);
  qiedigi->SetBranchAddress("DigiHE_capid2", &DigiHE_capid2);
  qiedigi->SetBranchAddress("DigiHE_capid3", &DigiHE_capid3);
  qiedigi->SetBranchAddress("DigiHE_capid4", &DigiHE_capid4);
  qiedigi->SetBranchAddress("DigiHE_capid5", &DigiHE_capid5);
  qiedigi->SetBranchAddress("DigiHE_capid6", &DigiHE_capid6);
  qiedigi->SetBranchAddress("DigiHE_capid7", &DigiHE_capid7);
  float DigiHF_fc0[3456], DigiHF_fc1[3456], DigiHF_fc2[3456];
  qiedigi->SetBranchAddress("DigiHF_fc0", &DigiHF_fc0);
  qiedigi->SetBranchAddress("DigiHF_fc1", &DigiHF_fc1);
  qiedigi->SetBranchAddress("DigiHF_fc2", &DigiHF_fc2);
  int DigiHF_adc0[3456], DigiHF_adc1[3456], DigiHF_adc2[3456];
  qiedigi->SetBranchAddress("DigiHF_adc0", &DigiHF_adc0);
  qiedigi->SetBranchAddress("DigiHF_adc1", &DigiHF_adc1);
  qiedigi->SetBranchAddress("DigiHF_adc2", &DigiHF_adc2);
  int DigiHF_capid0[3456], DigiHF_capid1[3456], DigiHF_capid2[3456];
  qiedigi->SetBranchAddress("DigiHF_capid0", &DigiHF_capid0);
  qiedigi->SetBranchAddress("DigiHF_capid1", &DigiHF_capid1);
  qiedigi->SetBranchAddress("DigiHF_capid2", &DigiHF_capid2);
  float DigiHO_fc0[2160], DigiHO_fc1[2160], DigiHO_fc2[2160], DigiHO_fc3[2160], DigiHO_fc4[2160];
  float DigiHO_fc5[2160], DigiHO_fc6[2160], DigiHO_fc7[2160], DigiHO_fc8[2160], DigiHO_fc9[2160];
  qiedigi->SetBranchAddress("DigiHO_fc0", &DigiHO_fc0);
  qiedigi->SetBranchAddress("DigiHO_fc1", &DigiHO_fc1);
  qiedigi->SetBranchAddress("DigiHO_fc2", &DigiHO_fc2);
  qiedigi->SetBranchAddress("DigiHO_fc3", &DigiHO_fc3);
  qiedigi->SetBranchAddress("DigiHO_fc4", &DigiHO_fc4);
  qiedigi->SetBranchAddress("DigiHO_fc5", &DigiHO_fc5);
  qiedigi->SetBranchAddress("DigiHO_fc6", &DigiHO_fc6);
  qiedigi->SetBranchAddress("DigiHO_fc7", &DigiHO_fc7);
  qiedigi->SetBranchAddress("DigiHO_fc8", &DigiHO_fc8);
  qiedigi->SetBranchAddress("DigiHO_fc9", &DigiHO_fc9);
  int DigiHO_adc0[2160], DigiHO_adc1[2160], DigiHO_adc2[2160], DigiHO_adc3[2160], DigiHO_adc4[2160];
  int DigiHO_adc5[2160], DigiHO_adc6[2160], DigiHO_adc7[2160], DigiHO_adc8[2160], DigiHO_adc9[2160];
  qiedigi->SetBranchAddress("DigiHO_adc0", &DigiHO_adc0);
  qiedigi->SetBranchAddress("DigiHO_adc1", &DigiHO_adc1);
  qiedigi->SetBranchAddress("DigiHO_adc2", &DigiHO_adc2);
  qiedigi->SetBranchAddress("DigiHO_adc3", &DigiHO_adc3);
  qiedigi->SetBranchAddress("DigiHO_adc4", &DigiHO_adc4);
  qiedigi->SetBranchAddress("DigiHO_adc5", &DigiHO_adc5);
  qiedigi->SetBranchAddress("DigiHO_adc6", &DigiHO_adc6);
  qiedigi->SetBranchAddress("DigiHO_adc7", &DigiHO_adc7);
  qiedigi->SetBranchAddress("DigiHO_adc8", &DigiHO_adc8);
  qiedigi->SetBranchAddress("DigiHO_adc9", &DigiHO_adc9);
  int DigiHO_capid0[2160], DigiHO_capid1[2160], DigiHO_capid2[2160], DigiHO_capid3[2160], DigiHO_capid4[2160];
  int DigiHO_capid5[2160], DigiHO_capid6[2160], DigiHO_capid7[2160], DigiHO_capid8[2160], DigiHO_capid9[2160];
  qiedigi->SetBranchAddress("DigiHO_capid0", &DigiHO_capid0);
  qiedigi->SetBranchAddress("DigiHO_capid1", &DigiHO_capid1);
  qiedigi->SetBranchAddress("DigiHO_capid2", &DigiHO_capid2);
  qiedigi->SetBranchAddress("DigiHO_capid3", &DigiHO_capid3);
  qiedigi->SetBranchAddress("DigiHO_capid4", &DigiHO_capid4);
  qiedigi->SetBranchAddress("DigiHO_capid5", &DigiHO_capid5);
  qiedigi->SetBranchAddress("DigiHO_capid6", &DigiHO_capid6);
  qiedigi->SetBranchAddress("DigiHO_capid7", &DigiHO_capid7);
  qiedigi->SetBranchAddress("DigiHO_capid8", &DigiHO_capid8);
  qiedigi->SetBranchAddress("DigiHO_capid9", &DigiHO_capid9);
  UChar_t eventtype, DigiHB_sipmTypes[9072], DigiHE_sipmTypes[6768];
  qiedigi->SetBranchAddress("uMNio_EventType", &eventtype);
  qiedigi->SetBranchAddress("DigiHB_sipmTypes", &DigiHB_sipmTypes);
  qiedigi->SetBranchAddress("DigiHE_sipmTypes", &DigiHE_sipmTypes);

  uint LS;
  qiedigi->SetBranchAddress("luminosityBlock", &LS);
  if(WholeRun){
    qiedigi->GetEntry(0);
    floatday = "LS"+to_string(LS)+"_"+floatday;
  }

  TFile *ofile = new TFile(("hist_CalibOutput_run"+runid+"_"+floatday+".root").c_str(), "recreate");

  cout << "Creating base histograms..." << endl;

  vector<string> subdets{"HB", "HE", "HF", "HO"};
  map<string, map<string, map<int, map<int, map<int, TH1F*>>>>> histarrayFC, histarrayADC; // Subdet, SiPM size, ieta, iphi, depth;  value is histogram filled with fC/ADC values
  map<string, map<int, map<int, map<int, map<int, vector<float>>>>>> CapIDarrayFC; // Subdet, ieta, iphi, depth, capid;  value is vector of fC values
  map<string, map<int, map<int, map<int, int>>>> rawIDarray; // Subdet, ieta, iphi, depth;  value is int of RawId
  map<string, map<int, map<int, map<int, int>>>> CheckMissing; // Subdet, ieta, iphi, depth;  value is int of RawId

  TH2F* ADCvsFC = new TH2F("ADC vs FC", "Conversion; ADC; FC", 32, 0, 32, 10000, 0, 1000);
  double fC_vals[256] = { // https://github.com/cms-sw/cmssw/blob/f5b4310413558919869e0dfa7c9c231e4b2b03fc/DQM/HcalCommon/interface/Constants.h#L253
        1.58,   4.73,   7.88,   11.0,   14.2,   17.3,   20.5,   23.6,   26.8,   29.9,   33.1,   36.2,   39.4,
        42.5,   45.7,   48.8,   53.6,   60.1,   66.6,   73.0,   79.5,   86.0,   92.5,   98.9,   105,    112,
        118,    125,    131,    138,    144,    151,    157,    164,    170,    177,    186,    199,    212,
        225,    238,    251,    264,    277,    289,    302,    315,    328,    341,    354,    367,    380,
        393,    406,    418,    431,    444,    464,    490,    516,    542,    568,    594,    620,    569,
        594,    619,    645,    670,    695,    720,    745,    771,    796,    821,    846,    871,    897,
        922,    947,    960,    1010,   1060,   1120,   1170,   1220,   1270,   1320,   1370,   1430,   1480,
        1530,   1580,   1630,   1690,   1740,   1790,   1840,   1890,   1940,   2020,   2120,   2230,   2330,
        2430,   2540,   2640,   2740,   2850,   2950,   3050,   3150,   3260,   3360,   3460,   3570,   3670,
        3770,   3880,   3980,   4080,   4240,   4450,   4650,   4860,   5070,   5280,   5490,

        5080,   5280,   5480,   5680,   5880,   6080,   6280,   6480,   6680,   6890,   7090,   7290,   7490,
        7690,   7890,   8090,   8400,   8810,   9220,   9630,   10000,  10400,  10900,  11300,  11700,  12100,
        12500,  12900,  13300,  13700,  14100,  14500,  15000,  15400,  15800,  16200,  16800,  17600,  18400,
        19300,  20100,  20900,  21700,  22500,  23400,  24200,  25000,  25800,  26600,  27500,  28300,  29100,
        29900,  30700,  31600,  32400,  33200,  34400,  36100,  37700,  39400,  41000,  42700,  44300,  41100,
        42700,  44300,  45900,  47600,  49200,  50800,  52500,  54100,  55700,  57400,  59000,  60600,  62200,
        63900,  65500,  68000,  71300,  74700,  78000,  81400,  84700,  88000,  91400,  94700,  98100,  101000,
        105000, 108000, 111000, 115000, 118000, 121000, 125000, 128000, 131000, 137000, 145000, 152000, 160000,
        168000, 176000, 183000, 191000, 199000, 206000, 214000, 222000, 230000, 237000, 245000, 253000, 261000,
        268000, 276000, 284000, 291000, 302000, 316000, 329000, 343000, 356000, 370000, 384000};
  double ADC_vals[256];
  for (int i = 0; i < 256; i++){
    ADC_vals[i] = i;
    fC_vals[i] = fC_vals[i]*6.0; // Table is for shunt 1, but measurements are shunt 6
  }
  TGraph* adc2fc = new TGraph(256, ADC_vals, fC_vals);
  TGraph* fc2adc = new TGraph(256, fC_vals, ADC_vals);
  adc2fc->SetTitle("ADC to FC; ADC; FC");
  fc2adc->SetTitle("FC to ADC; FC; ADC");


  // Get list of ALL channels that SHOULD exist (mainly for DPG table)
  ifstream DPGinfile("ref_table.txt"); 
  int meta, mphi, mdep, mrawid;
  string mline, msubdet;
  float mgarbage;
  while (getline(DPGinfile, mline))
  {
    istringstream iss(mline);
    iss >> dec >> meta;
    iss >> mphi;
    iss >> mdep;
    iss >> msubdet;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> mgarbage;
    iss >> hex >> mrawid;
    CheckMissing[msubdet][meta][mphi][mdep] = mrawid;
  }

  cout << "Looping over input events..." << endl;

  string size;
  int N, ieta, iphi, depth, evtype, sipmtype;
  float fcsum, adcsum;
  int doneevents=0;
  for(int i=0; i<ntot; i++){
    if(i%100==0) cout << i << "-th event." << endl;
    qiedigi->GetEntry(i);
    evtype = eventtype; // UChar_t -> Int conversion
    if(evtype!=1) continue; // PED events only
    doneevents++;
    if(doneevents==maxevents+1) break;

    for (auto const& subdet : subdets){
      if(subdet=="HB") N = nDigiHB;
      else if(subdet=="HE") N = nDigiHE;
      else if(subdet=="HF") N = nDigiHF;
      else if(subdet=="HO") N = nDigiHO;
      for(int j=0; j<N; j++){
        size = "";
        if(subdet=="HB"){
          ieta = DigiHB_ieta[j];
          iphi = DigiHB_iphi[j];
          depth = DigiHB_depth[j];
          sipmtype = DigiHB_sipmTypes[j]; // UChar_t -> Int conversion
          if(sipmtype==5) size = "_sipmSmall";
          if(sipmtype==6) size = "_sipmLarge";
        }else if (subdet=="HE"){
          ieta = DigiHE_ieta[j];
          iphi = DigiHE_iphi[j];
          depth = DigiHE_depth[j];
          sipmtype = DigiHE_sipmTypes[j]; // UChar_t -> Int conversion
          if(sipmtype==3) size = "_sipmSmall";
          if(sipmtype==4) size = "_sipmLarge";
        }else if (subdet=="HF"){
          ieta = DigiHF_ieta[j];
          iphi = DigiHF_iphi[j];
          depth = DigiHF_depth[j];
        }else if (subdet=="HO"){
          ieta = DigiHO_ieta[j];
          iphi = DigiHO_iphi[j];
          depth = DigiHO_depth[j];
        }
        if (ieta==0) continue;
        //if (histarrayFC.find(subdet.first) == histarrayFC.end()) histarrayFC[subdet.first];
        //if (histarrayFC[subdet.first].find(size) == histarrayFC[subdet.first].end()) histarrayFC[subdet.first][size];
        //if (histarrayFC[subdet.first][size].find(ieta) == histarrayFC[subdet.first][size].end()) histarrayFC[subdet.first][size][ieta];
        //if (histarrayFC[subdet.first][size][ieta].find(iphi) == histarrayFC[subdet.first][size][ieta].end()) histarrayFC[subdet.first][size][ieta][iphi];
        if (histarrayFC[subdet][size][ieta][iphi].find(depth) == histarrayFC[subdet][size][ieta][iphi].end()){
          histarrayFC[subdet][size][ieta][iphi][depth] = new TH1F(("hist_run"+runid+"_subdet"+subdet+size+"_ieta"+to_string(ieta)+"_iphi"+to_string(iphi)+"_depth"+to_string(depth)+"_FC").c_str(), "Pedestal per Channel; fC; Entries", 10000, 0, 1000); // TODO
          histarrayADC[subdet][size][ieta][iphi][depth] = new TH1F(("hist_run"+runid+"_subdet"+subdet+size+"_ieta"+to_string(ieta)+"_iphi"+to_string(iphi)+"_depth"+to_string(depth)+"_ADC").c_str(), "Pedestal per Channel; ADC; Entries", 960, 0, 32);
        }

        if(subdet=="HB"){
          fcsum = (DigiHB_fc0[j]+DigiHB_fc1[j]+DigiHB_fc2[j]+DigiHB_fc3[j]+DigiHB_fc4[j]+DigiHB_fc5[j]+DigiHB_fc6[j]+DigiHB_fc7[j]) / 8.0;
          adcsum = (DigiHB_adc0[j]+DigiHB_adc1[j]+DigiHB_adc2[j]+DigiHB_adc3[j]+DigiHB_adc4[j]+DigiHB_adc5[j]+DigiHB_adc6[j]+DigiHB_adc7[j]) / 8.0;
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid0[j]].push_back(DigiHB_fc0[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid1[j]].push_back(DigiHB_fc1[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid2[j]].push_back(DigiHB_fc2[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid3[j]].push_back(DigiHB_fc3[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid4[j]].push_back(DigiHB_fc4[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid5[j]].push_back(DigiHB_fc5[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid6[j]].push_back(DigiHB_fc6[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHB_capid7[j]].push_back(DigiHB_fc7[j]);
          rawIDarray[subdet][ieta][iphi][depth] = DigiHB_rawId[j];
          ADCvsFC->Fill(DigiHB_adc0[j], DigiHB_fc0[j]);
          ADCvsFC->Fill(DigiHB_adc1[j], DigiHB_fc1[j]);
          ADCvsFC->Fill(DigiHB_adc2[j], DigiHB_fc2[j]);
          ADCvsFC->Fill(DigiHB_adc3[j], DigiHB_fc3[j]);
          ADCvsFC->Fill(DigiHB_adc4[j], DigiHB_fc4[j]);
          ADCvsFC->Fill(DigiHB_adc5[j], DigiHB_fc5[j]);
          ADCvsFC->Fill(DigiHB_adc6[j], DigiHB_fc6[j]);
          ADCvsFC->Fill(DigiHB_adc7[j], DigiHB_fc7[j]);
        }else if (subdet=="HE"){
          fcsum = (DigiHE_fc0[j]+DigiHE_fc1[j]+DigiHE_fc2[j]+DigiHE_fc3[j]+DigiHE_fc4[j]+DigiHE_fc5[j]+DigiHE_fc6[j]+DigiHE_fc7[j]) / 8.0;
          adcsum = (DigiHE_adc0[j]+DigiHE_adc1[j]+DigiHE_adc2[j]+DigiHE_adc3[j]+DigiHE_adc4[j]+DigiHE_adc5[j]+DigiHE_adc6[j]+DigiHE_adc7[j]) / 8.0;
          if(ieta==-19 and iphi==16 and depth==5){ // There's a dead CapID (0) in this channel! Ignore it
            if(DigiHE_capid0[j]==0){
              fcsum = ((fcsum*8.0) - DigiHE_fc0[j] - DigiHE_fc4[j]) / 6.0;
              adcsum = ((adcsum*8.0) - DigiHE_adc0[j] - DigiHE_adc4[j]) / 6.0;
            }else if(DigiHE_capid1[j]==0){
              fcsum = ((fcsum*8.0) - DigiHE_fc1[j] - DigiHE_fc5[j]) / 6.0;
              adcsum = ((adcsum*8.0) - DigiHE_adc1[j] - DigiHE_adc5[j]) / 6.0;
            }else if(DigiHE_capid2[j]==0){
              fcsum = ((fcsum*8.0) - DigiHE_fc2[j] - DigiHE_fc6[j]) / 6.0;
              adcsum = ((adcsum*8.0) - DigiHE_adc2[j] - DigiHE_adc6[j]) / 6.0;
            }else if(DigiHE_capid3[j]==0){
              fcsum = ((fcsum*8.0) - DigiHE_fc3[j] - DigiHE_fc7[j]) / 6.0;
              adcsum = ((adcsum*8.0) - DigiHE_adc3[j] - DigiHE_adc7[j]) / 6.0;
            }
            //cout << DigiHE_fc0[j] << ", " << DigiHE_fc1[j] << ", " << DigiHE_fc2[j] << ", " << DigiHE_fc3[j] << ", " << DigiHE_fc4[j] << ", " << DigiHE_fc5[j] << ", " << DigiHE_fc6[j] << ", " << DigiHE_fc7[j] << "; AvgFC: " << fcsum << "; AvgADC: " << adcsum << endl;
          }
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid0[j]].push_back(DigiHE_fc0[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid1[j]].push_back(DigiHE_fc1[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid2[j]].push_back(DigiHE_fc2[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid3[j]].push_back(DigiHE_fc3[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid4[j]].push_back(DigiHE_fc4[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid5[j]].push_back(DigiHE_fc5[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid6[j]].push_back(DigiHE_fc6[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHE_capid7[j]].push_back(DigiHE_fc7[j]);
          rawIDarray[subdet][ieta][iphi][depth] = DigiHE_rawId[j];
          ADCvsFC->Fill(DigiHE_adc0[j], DigiHE_fc0[j]);
          ADCvsFC->Fill(DigiHE_adc1[j], DigiHE_fc1[j]);
          ADCvsFC->Fill(DigiHE_adc2[j], DigiHE_fc2[j]);
          ADCvsFC->Fill(DigiHE_adc3[j], DigiHE_fc3[j]);
          ADCvsFC->Fill(DigiHE_adc4[j], DigiHE_fc4[j]);
          ADCvsFC->Fill(DigiHE_adc5[j], DigiHE_fc5[j]);
          ADCvsFC->Fill(DigiHE_adc6[j], DigiHE_fc6[j]);
          ADCvsFC->Fill(DigiHE_adc7[j], DigiHE_fc7[j]);
        }else if (subdet=="HF"){
          fcsum = (DigiHF_fc0[j]+DigiHF_fc1[j]+DigiHF_fc2[j]) / 3.0;
          adcsum = (DigiHF_adc0[j]+DigiHF_adc1[j]+DigiHF_adc2[j]) / 3.0;
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHF_capid0[j]].push_back(DigiHF_fc0[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHF_capid1[j]].push_back(DigiHF_fc1[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHF_capid2[j]].push_back(DigiHF_fc2[j]);
          rawIDarray[subdet][ieta][iphi][depth] = DigiHF_rawId[j];
        }else if (subdet=="HO"){
          fcsum = (DigiHO_fc0[j]+DigiHO_fc1[j]+DigiHO_fc2[j]+DigiHO_fc3[j]+DigiHO_fc4[j]+DigiHO_fc5[j]+DigiHO_fc6[j]+DigiHO_fc7[j]+DigiHO_fc8[j]+DigiHO_fc9[j]) / 10.0;
          adcsum = (DigiHO_adc0[j]+DigiHO_adc1[j]+DigiHO_adc2[j]+DigiHO_adc3[j]+DigiHO_adc4[j]+DigiHO_adc5[j]+DigiHO_adc6[j]+DigiHO_adc7[j]+DigiHO_adc8[j]+DigiHO_adc9[j]) / 10.0;
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid0[j]].push_back(DigiHO_fc0[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid1[j]].push_back(DigiHO_fc1[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid2[j]].push_back(DigiHO_fc2[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid3[j]].push_back(DigiHO_fc3[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid4[j]].push_back(DigiHO_fc4[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid5[j]].push_back(DigiHO_fc5[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid6[j]].push_back(DigiHO_fc6[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid7[j]].push_back(DigiHO_fc7[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid8[j]].push_back(DigiHO_fc8[j]);
          CapIDarrayFC[subdet][ieta][iphi][depth][DigiHO_capid9[j]].push_back(DigiHO_fc9[j]);
          rawIDarray[subdet][ieta][iphi][depth] = DigiHO_rawId[j];
        }
        histarrayFC[subdet][size][ieta][iphi][depth]->Fill(fcsum);
        histarrayADC[subdet][size][ieta][iphi][depth]->Fill(adcsum);
      }
    }

  }

  // Test for comparing measured FC-ADC conversion and table
  /*cout << "Average FC per ADC bin:" << endl;
  for(int adc=0; adc<ADCvsFC->GetXaxis()->GetNbins(); adc++){
    ADCvsFC->GetXaxis()->SetRange(adc+1,adc+1);
    cout << "ADC " << adc << " : Table fC = " << adc2fc->Eval(adc) << ", Measured fC = " <<  ADCvsFC->GetMean(2) << endl;
  }
  ADCvsFC->GetXaxis()->SetRange(0,0); // Reset*/

  if(!WholeRun){

  cout << "Writing table..." << endl;

  ofstream tablefile;
  tablefile.open("Table_Run"+runid+"_"+floatday+".2024.txt");
  tablefile << setw(8) << "SubDet" << setw(8) << "SiPM" << setw(8) << "ieta" << setw(8) << "iphi" << setw(8) << "depth" << setw(12) << "ADC Mean" << setw(12) << "ADC RMS" << setw(12) << "fC Mean" << setw(12) << "fC RMS" << "\n";
  for (auto const& subdet : subdets){
    for (auto const& siz : histarrayFC[subdet]){
      if(siz.first=="_sipmSmall") size = "Small";
      if(siz.first=="_sipmLarge") size = "Large";
      for (auto const& eta : histarrayFC[subdet][siz.first]){
        for (auto const& phi : histarrayFC[subdet][siz.first][eta.first]){
          for (auto const& dep : histarrayFC[subdet][siz.first][eta.first][phi.first]){
            if (histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetEntries() > 0){
              tablefile << setw(8) << subdet << setw(8) << size << setw(8) << eta.first << setw(8) << phi.first << setw(8) << dep.first;
              tablefile << setw(12) << histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->GetMean() << setw(12) << histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->GetRMS();
              tablefile << setw(12) << histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetMean() << setw(12) << histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->GetRMS() << "\n";
            }
          }
        }
      }
    }
  }
  tablefile.close();


  cout << "Writing DPG pedestal tables..." << endl;

  float pedsum, pedmean, pedsqsum, pedstd;
  float CapIDMean[4], CapIDStd[4];
  int pedsize;
  ofstream DPGfile;
  ofstream DPGfileWidth;
  DPGfile.open("PedestalTable_Run"+runid+"_"+floatday+".2024.txt");
  DPGfileWidth.open("PedestalTableWidth_Run"+runid+"_"+floatday+".2024.txt");
  //DPGfile << "# Unit is fC" << "\n";
  DPGfile << "#U fC  << this is the unit" << "\n";
  DPGfile << "#" << setw(16) << "ieta" << setw(16) << "iphi" << setw(16) << "depth" << setw(16) << "SubDet" << setw(12) << "CapId0" << setw(12) << "CapId1" << setw(12) << "CapId2" << setw(12) << "CapId3" << setw(12) << "WidthId0" << setw(12) << "WidthId1" << setw(12) << "WidthId2" << setw(12) << "WidthId3" << setw(11) << "RawId" << "\n";
  //DPGfileWidth << "# Unit is fC^2" << "\n";
  DPGfileWidth << "#U fC  << this is the unit" << "\n";
  DPGfileWidth << "#" << setw(16) << "ieta" << setw(16) << "iphi" << setw(16) << "depth" << setw(16) << "SubDet" << setw(12) << "Width0/0" << setw(12) << "Width0/1" << setw(12) << "Width0/2" << setw(12) << "Width0/3" << setw(12) << "Width1/0" << setw(12) << "Width1/1" << setw(12) << "Width1/2" << setw(12) << "Width1/3" << setw(12) << "Width2/0" << setw(12) << "Width2/1" << setw(12) << "Width2/2" << setw(12) << "Width2/3" << setw(12) << "Width3/0" << setw(12) << "Width3/1" << setw(12) << "Width3/2" << setw(12) << "Width3/3" << setw(11) << "RawId" << "\n";
  for (auto const& subdet : subdets){
    for (auto const& eta : CapIDarrayFC[subdet]){
      for (auto const& phi : CapIDarrayFC[subdet][eta.first]){
        for (auto const& dep : CapIDarrayFC[subdet][eta.first][phi.first]){
          DPGfile << setw(17) << dec << eta.first << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
          DPGfileWidth << setw(17) << dec << eta.first << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
          for (int capid=0; capid<4; capid++) { // for (auto const& capid : CapIDarrayFC[subdet][eta.first][phi.first][dep.first]){
            CapIDMean[capid] = 0.0;
            CapIDStd[capid] = 0.0;
            if (!CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].empty()){
              pedsum = std::accumulate(CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].begin(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].end(), 0.0);
              pedmean = pedsum / CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].size();
              pedsqsum = std::inner_product(CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].begin(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].end(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].begin(), 0.0);
              pedstd = sqrt(pedsqsum / CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capid].size() - pedmean * pedmean);
            }else{ // For HF, one CapID is empty: Get average of other entries
              //cout << "Averaging: det=" << subdet << ", eta=" << eta.first << ", phi=" << phi.first << ", depth=" << dep.first << ", capid=" << capid << endl;
              pedsum = 0.0;
              pedsqsum = 0.0;
              pedsize = 0;
              for (int capidtemp=0; capidtemp<4; capidtemp++) { // for (auto const& capidtemp : CapIDarrayFC[subdet][eta.first][phi.first][dep.first]){
                if (!CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].empty()){
                  pedsum = pedsum + std::accumulate(CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].begin(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].end(), 0.0);
                  pedsqsum = pedsqsum + std::inner_product(CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].begin(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].end(), CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].begin(), 0.0);
                  pedsize = pedsize + CapIDarrayFC[subdet][eta.first][phi.first][dep.first][capidtemp].size();
                }
              }
              // if(pedsize==0) break; // No CapID has entries: It's missing channel! Fill below by interpolation. (But code shouldn't pass through here, because it wouldn't even enter the capid loop above.)
              pedmean = pedsum / pedsize;
              pedstd = sqrt(pedsqsum / pedsize - pedmean * pedmean);
            }
            //if(subdet=="HE" and eta.first==-19 and phi.first==16 and dep.first==5 and capid==0) cout << "HE,-19,16,5,0: " << pedmean << ", " << pedstd << endl;
            //if(subdet=="HB" and phi.first==7 and dep.first==3) cout << "HB,phi7,depth3: eta" << eta.first << ", capid" << capid << ": " << pedmean << ", " << pedstd << endl;
            CapIDMean[capid] = pedmean;
            CapIDStd[capid] = pedstd;
          }
          DPGfile << setw(12) << CapIDMean[0] << setw(12) << CapIDMean[1] << setw(12) << CapIDMean[2] << setw(12) << CapIDMean[3];
          DPGfile << setw(12) << CapIDStd[0] << setw(12) << CapIDStd[1] << setw(12) << CapIDStd[2] << setw(12) << CapIDStd[3];
          DPGfileWidth << setw(12) << CapIDStd[0]*CapIDStd[0] << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << CapIDStd[1]*CapIDStd[1] << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << CapIDStd[2]*CapIDStd[2] << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << CapIDStd[3]*CapIDStd[3];
          DPGfile << setw(11) << hex << rawIDarray[subdet][eta.first][phi.first][dep.first];
          DPGfile << "\n";
          DPGfileWidth << setw(11) << hex << rawIDarray[subdet][eta.first][phi.first][dep.first];
          DPGfileWidth << "\n";
          CheckMissing[subdet][eta.first][phi.first][dep.first] = 0;
        }
      }
    }
  }

  // Now get values for missing channels: Average over all (available) adjacent eta and phi +/- 1
  int myeta, myphi;
  for (auto const& subdet : subdets){
    for (auto const& eta : CheckMissing[subdet]){
      for (auto const& phi : CheckMissing[subdet][eta.first]){
        for (auto const& dep : CheckMissing[subdet][eta.first][phi.first]){
          if(CheckMissing[subdet][eta.first][phi.first][dep.first]!=0){
            //cout << "Interpolating: det=" << subdet << ", eta=" << eta.first << ", phi=" << phi.first << ", depth=" << dep.first << endl;
            DPGfile << setw(17) << dec << eta.first << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
            DPGfileWidth << setw(17) << dec << eta.first << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
            for (int capid=0; capid<4; capid++) {
              pedsum = 0.0;
              pedsqsum = 0.0;
              pedsize = 0;
              CapIDMean[capid] = 0.0;
              CapIDStd[capid] = 0.0;
              for (int etavar=-1; etavar<2; etavar++) {
                for (int phivar=-1; phivar<2; phivar++) {
                  myeta = eta.first+etavar;
                  myphi = phi.first+phivar;
                  if (myeta==0) myeta=-eta.first; // either eta in "0,1,2"->"-1,1,2" or "-2,-1,0"->"-2,-1,1"
                  if (myphi==0) myphi=72;
                  if (myphi==73) myphi=1;
                  if (!CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].empty()){
                    pedsum = pedsum + std::accumulate(CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].begin(), CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].end(), 0.0);
                    pedsqsum = pedsqsum + std::inner_product(CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].begin(), CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].end(), CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].begin(), 0.0);
                    pedsize = pedsize + CapIDarrayFC[subdet][myeta][myphi][dep.first][capid].size();
                  }
                }
              }
              pedmean = pedsum / pedsize;
              pedstd = sqrt(pedsqsum / pedsize - pedmean * pedmean);
              CapIDMean[capid] = pedmean;
              CapIDStd[capid] = pedstd;
            }
            DPGfile << setw(12) << CapIDMean[0] << setw(12) << CapIDMean[1] << setw(12) << CapIDMean[2] << setw(12) << CapIDMean[3];
            DPGfile << setw(12) << CapIDStd[0] << setw(12) << CapIDStd[1] << setw(12) << CapIDStd[2] << setw(12) << CapIDStd[3];
            DPGfileWidth << setw(12) << CapIDStd[0]*CapIDStd[0] << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
            DPGfileWidth << setw(12) << 0.0 << setw(12) << CapIDStd[1]*CapIDStd[1] << setw(12) << 0.0 << setw(12) << 0.0;
            DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << CapIDStd[2]*CapIDStd[2] << setw(12) << 0.0;
            DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << CapIDStd[3]*CapIDStd[3];
            DPGfile << setw(11) << hex << CheckMissing[subdet][eta.first][phi.first][dep.first];
            DPGfile << "\n";
            DPGfileWidth << setw(11) << hex << CheckMissing[subdet][eta.first][phi.first][dep.first];
            DPGfileWidth << "\n";
          }
        }
      }
    }
  }

  // Add ZDC channels
  vector<int> pm{-1, 1};
  vector<string> ZDC{"ZDC_EM", "ZDC_HAD", "ZDC_LUM"};
  for (auto const& eta : pm){
    for (auto const& subdet : ZDC){
      for (auto const& phi : CheckMissing[subdet][eta]){
        for (auto const& dep : CheckMissing[subdet][eta][phi.first]){
          DPGfile << setw(17) << dec << eta << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
          DPGfile << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfile << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfile << setw(11) << hex << CheckMissing[subdet][eta][phi.first][dep.first];
          DPGfile << "\n";
          DPGfileWidth << setw(17) << dec << eta << setw(16) << phi.first << setw(16) << dep.first << setw(16) << subdet;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0 << setw(12) << 0.0;
          DPGfileWidth << setw(11) << hex << CheckMissing[subdet][eta][phi.first][dep.first];
          DPGfileWidth << "\n";
        }
      }
    }
  }
  
  DPGfile.close();
  DPGfileWidth.close();


  ////
  cout << "Preparing matching map..." << endl; // Find eta/phi/depth matching to crate/slot/fiber/channel, for HBHE only (Reminder that some eta/phi/depth overlap between HBHE and HFHO)
  map<string, string> matching;
  map<string, int> matchingHO;
  ifstream infile("lmap_complete.txt"); 
  string line, sumone, sumtwo, strgarbage;
  int side, eta, phi, dep, crate, slot, fiber, channel, garbage;
  //vector<string> ishbhe{ "HB", "HBX", "HE", "HEX" };
  vector<string> ishbhe{"HB", "HBX", "HE", "HEX", "HO", "HOX"};
  vector<string> isho{ "HO", "HOX"};
  bool itsHBHE;
  while (getline(infile, line))
  {
    itsHBHE = false;
    istringstream iss(line);
    iss >> side;
    iss >> eta;
    iss >> phi;
    iss >> garbage; // dPhi
    iss >> dep;
    iss >> garbage; // CH_TYPE
    iss >> strgarbage; // Det
    if(find(ishbhe.begin(), ishbhe.end(), strgarbage) == ishbhe.end()) continue;
    if(find(isho.begin(), isho.end(), strgarbage) == isho.end()) itsHBHE = true;
    iss >> strgarbage; // RBX
    iss >> garbage; // Wedge
    iss >> garbage; // MB_NO
    iss >> garbage; // BV
    iss >> garbage; // QIE
    iss >> garbage; // QIECH
    iss >> garbage; // RM
    iss >> garbage; // RM_FI
    iss >> channel; // FI_CH
    iss >> garbage; // Trunk
    iss >> garbage; // cpCol
    iss >> garbage; // cpRow
    iss >> garbage; // cpCpl
    iss >> garbage; // cpLC
    iss >> garbage; // cpOctFib
    iss >> garbage; // ribbon
    iss >> garbage; // ppCol
    iss >> garbage; // ppRow
    iss >> strgarbage; // ppCpl
    iss >> garbage; // ppLC
    iss >> garbage; // dodec
    iss >> crate; // Crate
    iss >> slot; // uHTR
    iss >> fiber; // uHTR_FI

    //eta = eta * side;
    sumone = to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel);
    //sumtwo = to_string(eta)+" "+to_string(phi)+" "+to_string(dep);
    //matching[sumone] = sumtwo;
    sumtwo = to_string(eta * side)+" "+to_string(phi)+" "+to_string(dep);
    if(itsHBHE){
      matching[sumone] = sumtwo;
    }else{
      matchingHO[sumone] = eta;
    }
  }

  cout << "Writing xml..." << endl;

  ofstream xmlfile;
  xmlfile.open(floatday+".2024.xml");
  //string tag = "hb12-13_he8_hf0_ho11_v14";
  string tag = floatday+".2024_MeanPlus"+StrXtimesRMS+"RMS";
  auto MakeNewBrick = [&xmlfile](int crate, int slot, int elements, string tag){
    xmlfile << "  <CFGBrick>" << "\n";
    xmlfile << "    <Parameter name=\"INFOTYPE\" type=\"string\">ZS</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"CREATIONTAG\" type=\"string\">" << tag << "</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"CRATE\" type=\"int\">" << dec << crate << "</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"SLOT\" type=\"int\">" << dec << slot << "</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"TOPBOTTOM\" type=\"string\">utca</Parameter>" << "\n";
    xmlfile << "    <Parameter name=\"ZS_TYPE\" type=\"string\">standard</Parameter>" << "\n";
    xmlfile << "    <Data elements=\"" << dec << elements << "\" encoding=\"hex\">";
  };
  auto EndBrick = [&xmlfile](){
    xmlfile << "</Data>" << "\n";
    xmlfile << "  </CFGBrick>" << "\n";
  };

  xmlfile << "<?xml version=\"1.0\" ?>" << "\n";
  xmlfile << "<CFGBrickSet>" << "\n";

  // HBHE
  vector<int> HBHEcrateVec{ 20, 21, 24, 25, 30, 31, 34, 35, 37 };
  vector<int> FibNotInMod2{ 0, 1, 10, 11 };
  vector<int> FibNotInMod0{ 0, 11, 12, 23 };
  bool isHE;
  string subdet;
  for(int crate: HBHEcrateVec){
    for(int slot=1; slot<13; slot++){
      int modulo = slot%3; // mod 1 (1,4,7...) = HB only,    mod 2 (2,5,8...) = HBHE mix,    mod 0 (3,6,9...) = HE only
      int elements, maxchannel, ZS, smallEntries, largeEntries, ietaidx, depthidx; // , iphiidx
      float offset;
      string mystring;
      int filled = 0;
      if(modulo==0){
        maxchannel = 6;
      }else{
        maxchannel = 8;
      }
      elements = 24 * maxchannel; // 192 for HB, 144 for HE
      MakeNewBrick(crate, slot, elements, tag);
      for(int fiber=0; fiber<24; fiber++){
        bool FiberNotInUHTR = (modulo==2 and find(FibNotInMod2.begin(), FibNotInMod2.end(), fiber) != FibNotInMod2.end()) or (modulo==0 and find(FibNotInMod0.begin(), FibNotInMod0.end(), fiber) != FibNotInMod0.end());
        for(int channel=0; channel<maxchannel; channel++){
          // (Note on ranges: RM 1-4, RMFI 1-8, CH 0-5 HE, CH 0-7 HB)
          // When trying to find value for given crate&slot: Need to know uHTR_FI and FI_CH: nth value where n = HTRFI*8+CH+1
          if((modulo==2 and fiber==10 and (channel==4 || channel==5)) || (modulo==0 and fiber==11 and (channel==0 || channel==1))){ // Calibration unit fibers/channels (pin diodes). We don't use all of them but only a couple of channels, so they have ZS 0 instead of 255
            ZS = 0;
          }else if(modulo==2 and ((fiber==14 and channel==4) || (fiber==15 and channel==0) || (fiber==20 and channel==4) || (fiber==21 and channel==0))){ // Masked HE channels:
            // RM 1, RMFI 6, CH 4 -> slot 2, FI 14, CH 4   (eta 18, depth 1)
            // RM 2, RMFI 4, CH 0 -> slot 2, FI 15, CH 0   (eta 18, depth 1)
            // RM 3, RMFI 6, CH 4 -> slot 2, FI 20, CH 4   (eta 18, depth 1)
            // RM 4, RMFI 4, CH 0 -> slot 2, FI 21, CH 0   (eta 18, depth 1)
            ZS = 255;
          /*}else if((modulo==2 and (fiber==2 || fiber==6) and channel==2) || (modulo==1 and (fiber==21 || fiber==23) and channel==0)){ // Masked HB channels in local DQM, but not actually masked in runs
            // RM 1, RMFI 2, CH 2 -> slot 2, FI 2,  CH 2   (eta 14, depth 1)
            // RM 2, RMFI 8, CH 0 -> slot 1, FI 21, CH 0   (eta 12, depth 4)
            // RM 3, RMFI 2, CH 2 -> slot 2, FI 6,  CH 2   (eta 14, depth 1)
            // RM 4, RMFI 8, CH 0 -> slot 1, FI 23, CH 0   (eta 12, depth 4)
            ZS = 255;*/
          }else if(FiberNotInUHTR){ // Nonexistent channels
            ZS = 255;
          /*}else if(modulo==2 and (fiber==12 || fiber==13 || fiber==22 || fiber==23) and (channel==6 || channel==7)){ // Some of these are CU fibers, there are normal (?). During first tests, I determined that these were placeholders for HE fibers, all have depth "-999" and would get 255 at a leter check, but were regular HE ZS 8 for some reason.
            ZS = 8;*/
          //}else if(crate==20 and slot==2 and fiber==6){ // Masked HBM04 RM3 fiber
          //  ZS = 255;
          } else {
            if(crate==20 and slot==2 and fiber==6) {
	      cout << "Doing the normal thing for HBM04 RM3" << endl;
	    }
            mystring = matching[to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel)];
            //if(crate==20 && slot==2) cout << to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel) << " -> " << mystring << endl;
            istringstream iss(mystring);
            eta = 0;
            phi = 0;
            dep = 0;
            depthidx=-1000;
            while( iss >> eta >> phi >> dep ){
              ietaidx = eta<0?eta+41:eta+40;
              //iphiidx = phi-1;
              depthidx = dep-1;
            }
            if(depthidx==-1000){ // No valid depth: Not connected to any Megatiles for CU. Some in particular, appear like masked in local DQM for HE:
              // RM 2, RMFI 5, CH 5 -> slot 2, FI 16, CH 5   (depth -999)
              // RM 2, RMFI 7, CH 1 -> slot 2, FI 17, CH 1   (depth -999)
              // RM 3, RMFI 2, CH 1 -> slot 2, FI 18, CH 1   (depth -999)
              // RM 3, RMFI 4, CH 0 -> slot 2, FI 19, CH 0   (depth -999)
              // Alternatively, channels there no eta/phi/depth match was found for given crate/slot/fiber/channel also pass through here
              ZS = 255;
            }else{
              isHE = (((ietaidx>=12 && ietaidx<=24) || (ietaidx>=57 && ietaidx<=69) || (ietaidx==25 && depthidx>=3) || (ietaidx==56 && depthidx>=3)));
              subdet = isHE?"HE":"HB";
              smallEntries = 0;
              largeEntries = 0;
              if(false){
                // Example configuration: HE 8, HB small 9, HB large 10
                if (((ietaidx>=26 && ietaidx<=55) || (ietaidx==25 && depthidx<3) || (ietaidx==56 && depthidx<3)) && (depthidx>1 && depthidx<4)) ZS = 13;
                else if (((ietaidx>=26 && ietaidx<=55) || (ietaidx==25 && depthidx<3) || (ietaidx==56 && depthidx<3)) && (depthidx>-1 && depthidx<2)) ZS = 12;
                else if (isHE) ZS = 8;
                // Horizontal HB:
                if ((((crate==30) and (slot==4 || slot==5 || slot==10 || slot==11)) || ((crate==31) and (slot==1 || slot==2 || slot==7 || slot==8))) and ZS>8) ZS++;
              }else if(false){
                // ZS is Mean+RMS of each channel, rounded up
                // SiPM: Large or Small? Check both
                if(histarrayADC[subdet]["_sipmLarge"][eta][phi].find(dep) != histarrayADC[subdet]["_sipmLarge"][eta][phi].end()){
                  largeEntries = histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetEntries();
                }
                if(histarrayADC[subdet]["_sipmSmall"][eta][phi].find(dep) != histarrayADC[subdet]["_sipmSmall"][eta][phi].end()){
                  smallEntries = histarrayADC[subdet]["_sipmSmall"][eta][phi][dep]->GetEntries();
                }
                if ((smallEntries > 0) and (largeEntries == 0)){
                  ZS = ceil(histarrayADC[subdet]["_sipmSmall"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayADC[subdet]["_sipmSmall"][eta][phi][dep]->GetRMS() + RMSplusY);
                }else if ((smallEntries == 0) and (largeEntries > 0)){
                  ZS = ceil(histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS() + RMSplusY);
                }else if ((smallEntries == 0) and (largeEntries == 0)){
                  cout << "WARNING! NONE! " << smallEntries << "/" << largeEntries << ": " << eta << " " << phi << " " << dep << endl;
                  ZS = 255; // Shouldn't happen. Set safety value anyway
                }else if ((smallEntries > 0) and (largeEntries > 0)){
                  cout << "WARNING! BOTH! " << smallEntries << "/" << largeEntries << ": " << eta << " " << phi << " " << dep << endl;
                  ZS = ceil(histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS() + RMSplusY); // Shouldn't happen. Use large value just in case
                }
              }else{
                offset = RMSplusY;
                if(crate==25 and ((slot==4 and (fiber==2 || fiber==6 || fiber==10 || fiber==14 || fiber==18 || fiber==22)) || (slot==5 and (fiber==6 || fiber==7)))) offset = offset - LessForHBM09RM3; // Lower threshold for HBM09 RM3
                // ZS as above, but derived by FC, and then translated into ADC
                if(histarrayFC[subdet]["_sipmLarge"][eta][phi].find(dep) != histarrayFC[subdet]["_sipmLarge"][eta][phi].end()){
                  largeEntries = histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetEntries();
                }
                if(histarrayFC[subdet]["_sipmSmall"][eta][phi].find(dep) != histarrayFC[subdet]["_sipmSmall"][eta][phi].end()){
                  smallEntries = histarrayFC[subdet]["_sipmSmall"][eta][phi][dep]->GetEntries();
                }
                if ((smallEntries > 0) and (largeEntries == 0)){
                  float fc = histarrayFC[subdet]["_sipmSmall"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayFC[subdet]["_sipmSmall"][eta][phi][dep]->GetRMS();
                  ZS = ceil(fc2adc->Eval(fc) + offset);
                }else if ((smallEntries == 0) and (largeEntries > 0)){
                  float fc = histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS();
                  ZS = ceil(fc2adc->Eval(fc) + offset);
                  if(eta==-9 and phi==32 and dep==4){
                    cout << "FC Mean: " << histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() << endl;
                    cout << "FC RMS: " << histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS() << endl;
                    cout << "ADC Mean: " << histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() << endl;
                    cout << "ADC RMS: " << histarrayADC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS() << endl;
                    cout << "ADC val of FC = " << fc << " is " << fc2adc->Eval(fc) << endl;
                    cout << "ADC val of FC = " << histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + (XtimesRMS-1.0)*histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS() << " is " << fc2adc->Eval(histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + (XtimesRMS-1.0)*histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS()) << endl;
                  }
                }else if ((smallEntries == 0) and (largeEntries == 0)){
                  cout << "WARNING! NONE! " << smallEntries << "/" << largeEntries << ": " << eta << " " << phi << " " << dep << " " << crate << " " << slot << " " << fiber << " " << channel << endl;
                  ZS = 255; // Shouldn't happen. Set safety value anyway
                }else if ((smallEntries > 0) and (largeEntries > 0)){
                  cout << "WARNING! BOTH! " << smallEntries << "/" << largeEntries << ": " << eta << " " << phi << " " << dep << endl;
                  float fc = histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetMean() + XtimesRMS*histarrayFC[subdet]["_sipmLarge"][eta][phi][dep]->GetRMS();
                  ZS = ceil(fc2adc->Eval(fc) + offset); // Shouldn't happen. Use large value just in case
                }
              }
            }
          }
          // Double check ZS of masked channels:
          /*if( ((modulo==2 and (fiber==2 || fiber==6) and channel==2) || (modulo==1 and (fiber==21 || fiber==23) and channel==0))  ||  (modulo==2 and ((fiber==14 and channel==4) || (fiber==15 and channel==0) || (fiber==20 and channel==4) || (fiber==21 and channel==0) || (fiber==16 and channel==5) || (fiber==17 and channel==1) || (fiber==18 and channel==1) || (fiber==19 and channel==0)))  ||  (crate==20 and slot==1 and fiber==2) ){
            cout << "Doublecheck mask: " << ZS;
            if(ZS!=0 and ZS!=255){
              cout << " -> CR " << crate << ", slot " << slot << ", FI " << fiber << ", CH " << channel << "; changed to 255";
              ZS = 255;
            }
            cout << endl;
          }*/
          if(ZS>0 and ZS<ZSminimum) ZS = ZSminimum; // Define minimum ZS, specifically for HBP14 RM1

          xmlfile << hex << ZS;
          filled++;
          if(elements != filled) xmlfile << " ";
        }
      }
      EndBrick();
    }
  }

  // HF
  vector<int> HFcrateVec{ 22, 29, 32 };
  for(int crate: HFcrateVec){
    for(int slot=1; slot<13; slot++){
      MakeNewBrick(crate, slot, 96, tag);
      xmlfile << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
      EndBrick();
    }
  }
  MakeNewBrick(38, 9, 96, tag);
  xmlfile << "0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff";
  EndBrick();

  // HO
  vector<int> HOcrateVec{ 23, 26, 27, 38 };
  for(int crate: HOcrateVec){
    for(int slot=1; slot<10; slot++){
      if(crate!=26 and slot==9) continue;
      //MakeNewBrick(crate, slot, 72, tag);
      //xmlfile << "b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b";
      int filled = 0;
      int ZS, eta;
      MakeNewBrick(crate, slot, 72, tag);
      //xmlfile << "b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b b";
      for(int fiber=0; fiber<24; fiber++){
        for(int channel=0; channel<3; channel++){
          eta = matchingHO[to_string(crate)+"_"+to_string(slot)+"_"+to_string(fiber)+"_"+to_string(channel)];
          ZS = 11; // HO-0
          if(eta >= 5 && eta <= 10){ // HO-1PM
            ZS = 14;
          }else if(eta >= 11 && eta <= 15){ // HO-2PM
            ZS = 16;
          }
          xmlfile << hex << ZS;
          filled++;
          if(72 != filled) xmlfile << " ";
        }
      }
      EndBrick();
    }
  }

  xmlfile << "</CFGBrickSet>" << "\n";
  xmlfile.close();
  ////

  } // end if(!WholeRun)

  cout << "Saving results..." << endl;

  ofile->cd();

  for (auto const& subdet : subdets){
    for (auto const& siz : histarrayFC[subdet]){
      for (auto const& eta : histarrayFC[subdet][siz.first]){
        for (auto const& phi : histarrayFC[subdet][siz.first][eta.first]){
          for (auto const& dep : histarrayFC[subdet][siz.first][eta.first][phi.first]){
            histarrayFC[subdet][siz.first][eta.first][phi.first][dep.first]->Write(); // dep.second->Write()
            histarrayADC[subdet][siz.first][eta.first][phi.first][dep.first]->Write();
          }
        }
      }
    }
  }
  ADCvsFC->Write();
  adc2fc->Write();
  fc2adc->Write();

  cout << "Finished writing." << endl;
  ofile->Close();

  cout << "End Job." << endl;
  return 0;

}


