#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
//#include "TH3.h"
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

using namespace std;

int main(int argc, char *argv[])
{
  if(argc!=3){
    cerr << "Invalid arguments provided, correct format is: ./main finame foname\n";
    exit(0);
  }

  string finame = argv[1];
  string foname = argv[2];

  // Days since 5th July:
  // 0  = 05.07. (355079 / 0.001)
  // 7  = 12.07. (355538 / 0.030)
  // 8  = 13.07. (355573 / 0.052)
  // 9  = 14.07. (355669 / 0.053)
  // 10 = 15.07. (355710 / 0.101)
  // 13 = 18.07. (355776 / 0.120)
  // 14 = 19.07. (355838 / 0.1205)
  // 15 = 20.07. (355882 / 0.178)
  // 16 = 21.07. (355947 / 0.206)
  // 17 = 22.07. (356016 / 0.272)
  // 20 = 25.07. (356115 / 0.425)
  // 25 = 30.07. (356457 / 1.059)
  // 26 = 31.07.
  // 27 = 01.08. (356538 / 1.487)
  // 28 = 02.08. (356590 / 1.820)
  // 29 = 03.08. (356646 / 2.077)
  // 31 = 05.08. (356829 / 2.786)
  // 32 = 06.08. (356926 / 2.820)
  // 33 = 07.08. (356958 / 3.312)
  // 34 = 08.08. (357008 / 3.643)
  // 36 = 10.08. (357142 / 4.408)
  // 37 = 11.08. (357287 / 4.837)
  // 38 = 12.08. (357337 / 5.245)
  // 39 = 13.08. (357415 / 5.442)
  // 40 = 14.08. (357456 / 5.950)
  // 41 = 15.08. (357501 / 6.333)
  // 42 = 16.08. (357564 / 6.434)
  // 43 = 17.08. (357622 / 6.788)
  // 44 = 18.08. (357646 / 6.7885)
  // 46 = 20.08. (357743 / 7.950)
  // 47 = 21.08. (357787 / 8.585)
  // 48 = 22.08. (357845 / 9.165)
  // 49 = 23.08. (357968 / 9.649)
  // 50 = 24.08. (357996 / 9.649)
  // 51 = 25.08. (358087 / 9.649)
  // 52 = 26.08. (358101 / 9.649)
  // 54 = 28.08. (358160 / 9.649)
  // 55 = 29.08. (358179 / 9.649)
  // 56 = 30.08. (358222 / 9.649)
  // 57 = 31.08. (358277 / 9.649)
  // 58 = 01.09. (358303 / 9.649)
  // 59 = 02.09. (358338 / 9.649)
  // 62 = 05.09. (358430 / 9.649)
  // 63 = 06.09. (358488 / 9.649)
  // 64 = 07.09. (358543 / 9.649)
  // 66 = 09.09. (358595 / 9.649)
  // 69 = 12.09. (358663 / 9.649)
  // 72 = 15.09. (358812 / 9.649)
  // 74 = 17.09. (358902 / 9.649)
  // 75 = 18.09. (358928 / 9.649)
  // 76 = 19.09. (358997 / 9.649)
  // 77 = 20.09. (359026 / 9.651)
  // 78 = 21.09. (359092 / 9.651)
  // 79 = 22.09. (359168 / 9.651)
  // 80 = 23.09. (359236 / 9.651)
  // 83 = 26.09. (359299 / 9.654) (PED: 359309)
  // 84 = 27.09. (359378 / 9.654) (PED: 359382)
  // 85 = 28.09. (359483 / 9.654) (PED: 359489)
  // 86 = 29.09. (359549 / 9.654) (PED: 359554)
  // 87 = 30.09. (359652 / 9.702) (PED: 359656)
  // 89 = 02.10. (359703 / 10.791) (PED: 359707)
  // 90 = 03.10. (359730 / 10.996) (PED: 359735)
  // 91 = 04.10. (359767 / 11.836) (PED: 359772)
  // 92 = 05.10. (359822 / 12.234) (PED: 359827)
  // 93 = 06.10. (359892 / 12.346) (PED: 359912)
  // 94 = 07.10. (359954 / 12.346) (PED: 359967)
  // 95 = 08.10. (360006 / 12.901) (PED: 360010)
  // 96 = 09.10. (360040 / 13.363) (PED: 360043)
  // 97 = 10.10. (360096 / 13.952) (PED: 360102)
  // 98 = 11.10. (360144 / 14.821) (PED: 360145)
  // 99 = 12.10. (360242 / 15.048) (PED: 360244)
  // 100 = 13.10. (360302 / 15.653) (PED: 360310)
  // 101 = 14.10. (360362 / 15.810) (PED: 360363)
  // 102 = 15.10. (360417 / 15.965) (PED: 360419)
  // 103 = 16.10. (360463 / 16.907) (PED: 360468)

  // New files to process:
  // #1:
  //vector<float> days = {0, 7, 8, 9, 10};
  //vector<float> lumi = {0.001, 0.030, 0.052, 0.053, 0.101};
  //vector<string> floatday = {"05.07", "12.07", "13.07", "14.07", "15.07"};
  //vector<string> runid = {"355079", "355538", "355573", "355669", "355710"};

  // #2:
  //vector<float> days = {13, 14, 15, 16, 17};
  //vector<float> lumi = {0.120, 0.1205, 0.178, 0.206, 0.272};
  //vector<string> floatday = {"18.07", "19.07", "20.07", "21.07", "22.07"};
  //vector<string> runid = {"355776", "355838", "355882", "355947", "356016"};

  // #3:
  //vector<float> days = {20, 25, 27, 28, 29};
  //vector<float> lumi = {0.425, 1.059, 1.487, 1.820, 2.077};
  //vector<string> floatday = {"25.07", "30.07", "01.08", "02.08", "03.08"};
  //vector<string> runid = {"356115", "356457", "356538", "356590", "356646"};

  // #4:
  //vector<float> days = {31, 32, 33, 34, 36};
  //vector<float> lumi = {2.786, 2.820, 3.312, 3.643, 4.408};
  //vector<string> floatday = {"05.08", "06.08", "07.08", "08.08", "10.08"};
  //vector<string> runid = {"356829", "356926", "356958", "357008", "357142"};

  // #5:
  //vector<float> days = {37, 38, 39, 40, 41};
  //vector<float> lumi = {4.837, 5.245, 5.442, 5.950, 6.333};
  //vector<string> floatday = {"11.08", "12.08", "13.08", "14.08", "15.08"};
  //vector<string> runid = {"357287", "357337", "357415", "357456", "357501"};

  // #6:
  //vector<float> days = {42, 43, 44, 46, 47};
  //vector<float> lumi = {6.434, 6.788, 6.7885, 7.950, 8.585};
  //vector<string> floatday = {"16.08", "17.08", "18.08", "20.08", "21.08"};
  //vector<string> runid = {"357564", "357622", "357646", "357743", "357787"};

  // #7:
  //vector<float> days = {48, 49, 50, 51, 52};
  //vector<float> lumi = {9.165, 9.6485, 9.649, 9.649, 9.649};
  //vector<string> floatday = {"22.08", "23.08", "24.08", "25.08", "26.08"};
  //vector<string> runid = {"357845", "357968", "357996", "358087", "358101"};

  // #8:
  //vector<float> days = {54, 55, 56, 57, 58};
  //vector<float> lumi = {9.649, 9.649, 9.649, 9.649, 9.649};
  //vector<string> floatday = {"28.08", "29.08", "30.08", "31.08", "01.09"};
  //vector<string> runid = {"358160", "358179", "358222", "358277", "358303"};

  // #9:
  //vector<float> days = {59, 62, 63, 64, 66};
  //vector<float> lumi = {9.649, 9.649, 9.649, 9.649, 9.649};
  //vector<string> floatday = {"02.09", "05.09", "06.09", "07.09", "09.09"};
  //vector<string> runid = {"358338", "358430", "358488", "358543", "358595"};

  // #10:
  //vector<float> days = {69, 72, 74, 75, 76};
  //vector<float> lumi = {9.649, 9.649, 9.649, 9.649, 9.649};
  //vector<string> floatday = {"12.09", "15.09", "17.09", "18.09", "19.09"};
  //vector<string> runid = {"358663", "358812", "358902", "358928", "358997"};

  // #11:
  //vector<float> days = {77, 78, 79, 80, 83};
  //vector<float> lumi = {9.651, 9.651, 9.651, 9.651, 9.654};
  //vector<string> floatday = {"20.09", "21.09", "22.09", "23.09", "26.09"};
  //vector<string> runid = {"359026", "359092", "359168", "359236", "359299"};

  // #12:
  //vector<float> days = {84, 85, 86, 87, 89};
  //vector<float> lumi = {9.654, 9.654, 9.654, 9.702, 10.791};
  //vector<string> floatday = {"27.09", "28.09", "29.09", "30.09", "02.10"};
  //vector<string> runid = {"359378", "359483", "359549", "359652", "359703"};

  // #13:
  //vector<float> days = {90, 91, 92, 93, 94};
  //vector<float> lumi = {10.996, 11.836, 12.234, 12.346, 12.346};
  //vector<string> floatday = {"03.10", "04.10", "05.10", "06.10", "07.10"};
  //vector<string> runid = {"359730", "359767", "359822", "359892", "359954"};

  // #14:
  //vector<float> days = {95, 96, 97, 98, 99};
  //vector<float> lumi = {12.901, 13.363, 13.952, 14.821, 15.048};
  //vector<string> floatday = {"08.10", "09.10", "10.10", "11.10", "12.10"};
  //vector<string> runid = {"360006", "360040", "360096", "360144", "360242"};

  // #15:
  //vector<float> days = {100, 101, 102, 103};
  //vector<float> lumi = {15.653, 15.810, 15.965, 16.907};
  //vector<string> floatday = {"13.10", "14.10", "15.10", "16.10"};
  //vector<string> runid = {"360302", "360362", "360417", "360463"};


  // PEDonly #1:
  //vector<float> days = {83, 84, 85, 86, 87};
  //vector<float> lumi = {9.654, 9.654, 9.654, 9.654, 9.702};
  //vector<string> floatday = {"26.09", "27.09", "28.09", "29.09", "30.09"};
  //vector<string> runid = {"359309", "359382", "359489", "359554", "359656"};

  // PEDonly #2:
  //vector<float> days = {89, 90, 91, 92, 93};
  //vector<float> lumi = {10.791, 10.996, 11.836, 12.234, 12.346};
  //vector<string> floatday = {"02.10", "03.10", "04.10", "05.10", "06.10"};
  //vector<string> runid = {"359707", "359735", "359772", "359827", "359912"};

  // PEDonly #3:
  //vector<float> days = {94, 95, 96, 97, 98};
  //vector<float> lumi = {12.346, 12.901, 13.363, 13.952, 14.821};
  //vector<string> floatday = {"07.10", "08.10", "09.10", "10.10", "11.10"};
  //vector<string> runid = {"359967", "360010", "360043", "360102", "360145"};

  // PEDonly #4:
  vector<float> days = {99, 100, 101, 102, 103};
  vector<float> lumi = {15.048, 15.653, 15.810, 15.965, 16.907};
  vector<string> floatday = {"12.10", "13.10", "14.10", "15.10", "16.10"};
  vector<string> runid = {"360244", "360310", "360363", "360419", "360468"};


  // Global:
  //vector<float> days = {42};
  //vector<float> lumi = {6.434};
  //vector<string> floatday = {"16.08"};
  //vector<string> runid = {"357559"};

  /*reverse(days.begin(), days.end());
  reverse(lumi.begin(), lumi.end());
  reverse(floatday.begin(), floatday.end());
  reverse(runid.begin(), runid.end());*/

  assert(days.size()==runid.size());

  int nruns = days.size();

  TFile *f = new TFile((finame+".root").c_str(), "read");
  TNtuple* qiedigi = (TNtuple*)f->Get("MyHcalAnlzr/qiedigi");
  int ntot = qiedigi->GetEntries();
  std::cout << "Reading in input file, total " << ntot << " Entries." << std::endl;
  float RunNum, LumiNum, EvtNum, ieta, iphi, depth, sumADC, type, shunt;
  qiedigi->SetBranchAddress("RunNum", &RunNum);
  qiedigi->SetBranchAddress("LumiNum", &LumiNum);
  qiedigi->SetBranchAddress("EvtNum", &EvtNum);
  qiedigi->SetBranchAddress("ieta", &ieta);
  qiedigi->SetBranchAddress("iphi", &iphi);
  qiedigi->SetBranchAddress("depth", &depth);
  qiedigi->SetBranchAddress("sumADC", &sumADC);
  qiedigi->SetBranchAddress("type", &type);
  qiedigi->SetBranchAddress("shunt", &shunt);

  TFile *ofile = new TFile(("hist_"+foname+"_digi.root").c_str(), "recreate");

  std::cout << "Creating histograms..." << std::endl;

  TH1F ****** histarray;
  histarray = new TH1F*****[nruns];
  for(int n=0; n<nruns; n++){ // Number of runs
    histarray[n] = new TH1F****[3];
    for(int t=0; t<3; t++){ // 0=Small SiPM, 1=Large SiPM, 2=HF&HO
      histarray[n][t] = new TH1F***[82];
      for(int i=0; i<82; i++){ // Number of ieta bins -41..41, excluding 0
        if (t!=2 && (i<12 || i>69)) continue; //// Remove HF range from HBHE
        if (t==2 && ((i>12 && i<26) || (i>55 && i<69))) continue; //// HF between ieta 29 and 41 (= i 12/69 and 0/81), HO between ieta -15 and 15 (= i 26 and 55)
        histarray[n][t][i] = new TH1F**[72];
        for(int j=0; j<72; j++){ // Number of iphi bins 1..72
          //if (((i>12 && i<14) || (i>67 && i<69)) && (t==1 || (j%2==1))) continue; //// HE not in all size/phi
          //if (((i<2) || (i>79)) && j%4!=2) continue; //// HF not in all phi
          //if (((i<12) || (i>69)) && j%2==1) continue; //// HF not in all phi
          histarray[n][t][i][j] = new TH1F*[7];
          for(int k=0; k<7; k++){ // Number of depths 1..7
            if ((i>23 && i<58) && k>3) break; //// HB (and a bit of HE, ieta 16&17) depth up to 4
            if (t==2 && k>3) break; //// HF depth up to 4
            if (t==2 && (i>=26 && i<=55) && k!=3) continue; //// HO only has depth 4
            //if ((i==12 || i==69) && (k>3 || j%2==1)) break; //// HE not in all depth/phi
            histarray[n][t][i][j][k] = new TH1F(("hist_run"+runid.at(n)+"_sipmT"+to_string(t)+"_ieta"+to_string(i<=41?i-41:i-40)+"_iphi"+to_string(j+1)+"_depth"+to_string(k+1)).c_str(), "Pedestal per Channel; ADC; Entries", 1600, 0, 40);
          }
        }
      }
    }
  }

  TH1F **** pedMean;
  TH1F **** pedRMS;
  TH1F **** PEDMeanDepth;
  TH1F **** PEDRMSDepth;
  TH1F ***** PEDMeanPhi;
  TH1F ***** PEDRMSPhi;
  pedMean = new TH1F***[4];
  pedRMS = new TH1F***[4];
  PEDMeanDepth = new TH1F***[4];
  PEDRMSDepth = new TH1F***[4];
  PEDMeanPhi = new TH1F****[4];
  PEDRMSPhi = new TH1F****[4];
  string det;
  string size;
  for(int nh=0; nh<4; nh++){ // HB, HE, HF and HO
    //string det = nh==0?"HB":"HE";
    if (nh==0) det = "HB";
    else if (nh==1) det = "HE";
    else det = nh==2?"HF":"HO";

    pedMean[nh]=new TH1F**[5];
    pedRMS[nh]=new TH1F**[5];
    for(int nt=0; nt<5; nt++){ // HBHE: 0=Small SiPM, 1=Large SiPM; HF/HO: 2=All, Special: 3+
      if (nh<2 && nt==2) continue; // HBHE
      if (nh>1 && nt!=2) continue; // HFHO
      pedMean[nh][nt]=new TH1F*[nruns];
      pedRMS[nh][nt]=new TH1F*[nruns];
      for(int n=0; n<nruns; n++){ // One per run
        pedMean[nh][nt][n] = new TH1F((det+"_sipmT"+to_string(nt)+"pedMean_run"+runid.at(n)).c_str(), "Pedestal Mean; ADC; Entries", 1600, 0, 40);
        pedRMS[nh][nt][n] = new TH1F((det+"_sipmT"+to_string(nt)+"pedRMS_run"+runid.at(n)).c_str(), "Pedestal RMS; ADC; Entries", 1600, 0, 40);
      }
    }
    PEDMeanDepth[nh]=new TH1F**[7];
    PEDRMSDepth[nh]=new TH1F**[7];
    for(int k=0; k<7; k++){ // Depth
      if (nh!=1 && k>3) break; // Only HE has depth 5,6,7
      if (nh==3 && k!=3) continue; // HO only depth 4
      PEDMeanDepth[nh][k]=new TH1F*[nruns];
      PEDRMSDepth[nh][k]=new TH1F*[nruns];
      for(int n=0; n<nruns; n++){ // One per run
        PEDMeanDepth[nh][k][n] = new TH1F((det+"_depth"+to_string(k)+"pedMean_run"+runid.at(n)).c_str(), "Pedestal Mean; ADC; Entries", 1600, 0, 40);
        PEDRMSDepth[nh][k][n] = new TH1F((det+"_depth"+to_string(k)+"pedRMS_run"+runid.at(n)).c_str(), "Pedestal RMS; ADC; Entries", 1600, 0, 40);
      }
    }
    if(nh>1) continue; // HBHE only
    PEDMeanPhi[nh]=new TH1F***[2];
    PEDRMSPhi[nh]=new TH1F***[2];
    for(int nt=0; nt<2; nt++){ // HBHE: 0=Small SiPM, 1=Large SiPM
      PEDMeanPhi[nh][nt]=new TH1F**[2];
      PEDRMSPhi[nh][nt]=new TH1F**[2];
      for(int k=0; k<2; k++){ // Phi slice 1/2
        PEDMeanPhi[nh][nt][k]=new TH1F*[nruns];
        PEDRMSPhi[nh][nt][k]=new TH1F*[nruns];
        for(int n=0; n<nruns; n++){ // One per run
          PEDMeanPhi[nh][nt][k][n] = new TH1F((det+"_sipmT"+to_string(nt)+"_phi"+to_string(k)+"pedMean_run"+runid.at(n)).c_str(), "Pedestal Mean; ADC; Entries", 1600, 0, 40);
          PEDRMSPhi[nh][nt][k][n] = new TH1F((det+"_sipmT"+to_string(nt)+"_phi"+to_string(k)+"pedRMS_run"+runid.at(n)).c_str(), "Pedestal RMS; ADC; Entries", 1600, 0, 40);
        }
      }
    }
  }

  std::cout << "Looping over input events..." << std::endl;

  for(int i=0; i<ntot; i++){
    if(i%50000000==0) std::cout << i << "-th entry." << std::endl;
    qiedigi->GetEntry(i);

    if(find(runid.begin(), runid.end(), to_string((int)RunNum))==runid.end()) continue;

    if(shunt!=-1.0 and shunt!=6.0) continue; //-1.0 for global, 6.0 for local

    int runidx = distance(runid.begin(), find(runid.begin(), runid.end(), to_string((int)RunNum)) );
    int ietaidx = ieta<0?ieta+41:ieta+40;
    int iphiidx = iphi-1;
    int depthidx = depth-1;
    int sipmidx = -1;
    if(type==3||type==5) sipmidx=0; // Small SiPM
    else if(type==4||type==6) sipmidx=1; // Large SiPM
    else if(type==0||type==2) sipmidx=2; // HF/HO
    // HF (type 0) can be assinged "Large" type, because there's overlap in HE/HF in ieta 29, depths 1-3.
    // In this range, all HE are small. HF has only one size, so put those as large.
    // Put HO (type 2) in separate histograms

    //std::cout << runidx << ", " << sipmidx << ", " << ietaidx << ", " << iphiidx << ", " << depthidx << std::endl;

    if (type==3 || type==4 || type==5 || type==6){
      histarray[runidx][sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/8.0); // Average ADC of all 8 TSs, HBHE
    }else if (!(i>=26 && i<=55)){
      histarray[runidx][sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/6.0); // Average ADC of all 6 TSs, HF
    }else{
      histarray[runidx][sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/10.0); // Average ADC of all 10 TSs, HO
    }

  }


  std::cout << "Postprocessing histograms..." << std::endl;

  //TH3F* h3 = new TH3F("h3", "h3", 83, -41, 42, 72, 0, 72, 7, 1, 8);
  ofstream tablefile;
  ofstream delme;
  delme.open("delmehistos.txt");
  for(int n=0; n<nruns; n++){
    tablefile.open("Table_Run"+runid[n]+"_"+floatday[n]+".2022.txt");
    tablefile << setw(8) << "SubDet" << setw(8) << "SiPM" << setw(8) << "ieta" << setw(8) << "iphi" << setw(8) << "depth" << setw(12) << "Mean" << setw(12) << "RMS" << "\n";
    for(int t=0; t<5; t++){
      for(int i=0; i<82; i++){
        if (t!=2 && (i<12 || i>69)) continue; // HF range from HBHE
        if (t==2 && ((i>12 && i<26) || (i>55 && i<69))) continue; // Empty space between HF and HO
        for(int j=0; j<72; j++){
          for(int k=0; k<7; k++){
            if ((i>23 && i<58) && k>3) break; // HB depth up to 4
            if (t==2 && k>3) break; // HF depth up to 4
            if (t==2 && (i>=26 && i<=55) && k!=3) continue; // HO only depth 4
            //std::cout << n << ", " << t << ", " << i << ", " << j << ", " << k << std::endl;
            if (t<3){ // Regular
              if(histarray[n][t][i][j][k]->GetEntries()>0){
                int nh=-1;
                //if((i>=14 && i<=43) || (i==13 && k<3) || (i==44 && k<3)) nh=0; // HB
                //else if((i<=12 || i>=45 ) || (i==13 && k>=3) || (i==44 && k>=3)) nh=1; // HE
                if(t<2 && ((i>=26 && i<=55) || (i==25 && k<3) || (i==56 && k<3))) nh=0; // HB
                else if(t<2 && ((i>=12 && i<=24) || (i>=57 && i<=69) || (i==25 && k>=3) || (i==56 && k>=3))) nh=1; // HE, incl ieta=29 (i=12, 69)
                else if(t==2 && (i<13 || i>68)) nh=2; // HF
                else if(t==2 && (i>25 && i<56)) nh=3; // HO
                else{
                  std::cout << "ERROR! Check this!" << "\n";
                  std::cout << n << ", " << t << ", " << i << ", " << j << ", " << k << std::endl;
                }
                pedMean[nh][t][n]->Fill(histarray[n][t][i][j][k]->GetMean());
                pedRMS[nh][t][n]->Fill(histarray[n][t][i][j][k]->GetRMS());
                PEDMeanDepth[nh][k][n]->Fill(histarray[n][t][i][j][k]->GetMean());
                PEDRMSDepth[nh][k][n]->Fill(histarray[n][t][i][j][k]->GetRMS());
                if (nh==0) det = "HB";
                else if (nh==1) det = "HE";
                else det = nh==2?"HF":"HO";
                if (nh>1) size = "";
                else size = t==0?"Small":"Large";
                tablefile << setw(8) << det << setw(8) << size << setw(8) << (i<41?i-41:i-40) << setw(8) << (j+1) << setw(8) << (k+1) << setw(12) << pedMean[nh][t][n]->GetMean() << setw(12) << pedRMS[nh][t][n]->GetRMS() << "\n";
                //if(t==1 && nh==0 && histarray[n][t][i][j][k]->GetMean() < 5.6) h3->Fill(i<41?i-41:i-40, j, k+1); // Large SiPM, HB, Small mean
              }else{
                if (n==0) delme << "Empty:" << t << ", " << i << ", " << j << ", " << k << "\n";
              }
            }else{ // Special
              if ((t==3 && (j>33 && j<38)) || (t==4 && (j<2 || j>69)) || (t==4 && (j>9 && j<13))){ // iphi's 35-38 and 71,72,1,2 ...and 11-14
                int nh=-1;
                if(((i>=26 && i<=55) || (i==25 && k<3) || (i==56 && k<3))) nh=0; // HB
                else if(((i>=12 && i<=24) || (i>=57 && i<=69) || (i==25 && k>=3) || (i==56 && k>=3))) nh=1; // HE, incl ieta=29 (i=12, 69)
                else{
                  std::cout << "ERROR! Check this!" << "\n";
                  std::cout << n << ", " << t << ", " << i << ", " << j << ", " << k << std::endl;
                }
                for(int t2=0; t2<2; t2++){
                  //std::cout << n << ", " << t2 << ", " << i << ", " << j << ", " << k << std::endl;
                  if(histarray[n][t2][i][j][k]->GetEntries()>0){
                    if ((t==3 && (j>33 && j<38)) || (t==4 && (j<2 || j>69))){ // iphi's 35-38 and 71,72,1,2
                      if (t2!=0) { // Do only large SiPMs for now -> Small and Large shouldn't mix, make the separate later (TODO).
                      pedMean[nh][t][n]->Fill(histarray[n][t2][i][j][k]->GetMean());
                      pedRMS[nh][t][n]->Fill(histarray[n][t2][i][j][k]->GetRMS());
                      }
                    }
                    if ((t==3 && (j>33 && j<38)) || (t==4 && (j>9 && j<13))){ // iphi's 35-38 and 11-14
                      PEDMeanPhi[nh][t2][t-3][n]->Fill(histarray[n][t2][i][j][k]->GetMean());
                      PEDRMSPhi[nh][t2][t-3][n]->Fill(histarray[n][t2][i][j][k]->GetRMS());
                    }
                  }
                }
              }
            }
          }
        }
      }
    }
    delme.close();
    tablefile.close();
  }

  std::cout << "part2..." << std::endl;
  vector<float> MeanofMeanPedVal[4][4];
  vector<float> RMSofMeanPedVal[4][4];
  vector<float> MeanofRMSPedVal[4][4];
  ofstream savefile;
  savefile.open("SaveFile.txt", ios::app);
  for(int nh=0; nh<4; nh++){ // HB / HE / HF / HO
    for(int nt=0; nt<5; nt++){ // Small / Large SiPM / HFHO / Special
      if (nh<2 && nt==2) continue;
      if (nh>1 && nt!=2) continue;
      for(int n=0; n<nruns; n++){ // Runs
        MeanofMeanPedVal[nh][nt].push_back(pedMean[nh][nt][n]->GetMean());
        RMSofMeanPedVal[nh][nt].push_back(pedMean[nh][nt][n]->GetRMS());
        MeanofRMSPedVal[nh][nt].push_back(pedRMS[nh][nt][n]->GetMean());
        if(shunt!=-1.0){ // Only save for non-global runs
          if(pedMean[nh][nt][n]->GetMean()!=0) savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " MeanMean " << pedMean[nh][nt][n]->GetMean() << "\n";
          if(pedMean[nh][nt][n]->GetRMS()!=0) savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " RMSMean " << pedMean[nh][nt][n]->GetRMS() << "\n";
          if(pedRMS[nh][nt][n]->GetMean()!=0) savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " MeanRMS " << pedRMS[nh][nt][n]->GetMean() << "\n";
        }
      }
    }
  }
  savefile.close();

  std::cout << "part3..." << std::endl;
  TGraph *** MeanofMeanvsLumi;
  TGraph *** RMSofMeanvsLumi;
  TGraph *** MeanofRMSvsLumi;
  MeanofMeanvsLumi = new TGraph**[4];
  RMSofMeanvsLumi = new TGraph**[4];
  MeanofRMSvsLumi = new TGraph**[4];
  for(int nh=0; nh<4; nh++){
    if (nh==0) det = "HB";
    else if (nh==1) det = "HE";
    else det = nh==2?"HF":"HO";
    MeanofMeanvsLumi[nh]=new TGraph*[3];
    RMSofMeanvsLumi[nh]=new TGraph*[3];
    MeanofRMSvsLumi[nh]=new TGraph*[3];
    for(int nt=0; nt<5; nt++){
      if (nh<2 && nt==2) continue;
      if (nh>1 && nt!=2) continue;
      MeanofMeanvsLumi[nh][nt]=new TGraph(days.size(), &(days[0]), &(MeanofMeanPedVal[nh][nt][0]));
      RMSofMeanvsLumi[nh][nt]=new TGraph(days.size(), &(days[0]), &(RMSofMeanPedVal[nh][nt][0]));
      MeanofRMSvsLumi[nh][nt]=new TGraph(days.size(), &(days[0]), &(MeanofRMSPedVal[nh][nt][0]));
      MeanofMeanvsLumi[nh][nt]->SetTitle(det+"Mean_of_pedMean_sipmT"+nt);
      RMSofMeanvsLumi[nh][nt]->SetTitle(det+"RMS_of_pedMean_sipmT"+nt);
      MeanofRMSvsLumi[nh][nt]->SetTitle(det+"Mean_of_pedRMS_sipmT"+nt);
      MeanofMeanvsLumi[nh][nt]->SetName(det+"Mean_of_pedMean_sipmT"+nt);
      RMSofMeanvsLumi[nh][nt]->SetName(det+"RMS_of_pedMean_sipmT"+nt);
      MeanofRMSvsLumi[nh][nt]->SetName(det+"Mean_of_pedRMS_sipmT"+nt);
    }
  }



  std::cout << "Saving results..." << std::endl;

  ofile->cd();
  //h3->Write();

  for(int nh=0; nh<4; nh++){
    for(int nt=0; nt<2; nt++){
      if (nh<2 && nt==2) continue;
      if (nh>1 && nt!=2) continue;
      MeanofMeanvsLumi[nh][nt]->Write();
      RMSofMeanvsLumi[nh][nt]->Write();
      MeanofRMSvsLumi[nh][nt]->Write();
    }
  }

  for(int nh=0; nh<4; nh++){
    for(int nt=0; nt<2; nt++){
      if (nh<2 && nt==2) continue;
      if (nh>1 && nt!=2) continue;
      for(int n=0; n<nruns; n++){
        pedMean[nh][nt][n]->Write();
        pedRMS[nh][nt][n]->Write();
      }
    }
    for(int k=0; k<7; k++){
      if (nh!=1 && k>3) break;
      if (nh==3 && k!=3) continue;
      for(int n=0; n<nruns; n++){
         PEDMeanDepth[nh][k][n]->Write();
         PEDRMSDepth[nh][k][n]->Write();
      }
    }
    if(nh>1) continue;
    for(int nt=0; nt<2; nt++){
      for(int k=0; k<2; k++){
        for(int n=0; n<nruns; n++){
          PEDMeanPhi[nh][nt][k][n]->Write();
          PEDRMSPhi[nh][nt][k][n]->Write();
        }
      }
    }
  }


  std::cout << "Finished writing." << std::endl;
  ofile->Close(); // Sometimes crashes when closing; probably some memory issue. Other times it crashes after "End Job" instead.
  // HOWEVER: Output is fine after this step.

  std::cout << "End Job." << std::endl;
  return 0;

}


