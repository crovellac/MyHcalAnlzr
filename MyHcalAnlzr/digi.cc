#include "TROOT.h"
#include "TFile.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TH3.h"
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
  // 87 = 30.09.

  // New files to process:
  // #1:
  //vector<float> days = {0, 7, 8, 9, 10};
  //vector<float> lumi = {0.001, 0.030, 0.052, 0.053, 0.101};
  //vector<float> floatday = {05.07, 12.07, 13.07, 14.07, 15.07};
  //vector<string> runid = {"355079", "355538", "355573", "355669", "355710"};

  // #2:
  //vector<float> days = {13, 14, 15, 16, 17};
  //vector<float> lumi = {0.120, 0.1205, 0.178, 0.206, 0.272};
  //vector<float> floatday = {18.07, 19.07, 20.07, 21.07, 22.07};
  //vector<string> runid = {"355776", "355838", "355882", "355947", "356016"};

  // #3:
  //vector<float> days = {20, 25, 27, 28, 29};
  //vector<float> lumi = {0.425, 1.059, 1.487, 1.820, 2.077};
  //vector<float> floatday = {25.07, 30.07, 01.08, 02.08, 03.08};
  //vector<string> runid = {"356115", "356457", "356538", "356590", "356646"};

  // #4:
  //vector<float> days = {31, 32, 33, 34, 36};
  //vector<float> lumi = {2.786, 2.820, 3.312, 3.643, 4.408};
  //vector<float> floatday = {05.08, 06.08, 07.08, 08.08, 10.08};
  //vector<string> runid = {"356829", "356926", "356958", "357008", "357142"};

  // #5:
  //vector<float> days = {37, 38, 39, 40, 41};
  //vector<float> lumi = {4.837, 5.245, 5.442, 5.950, 6.333};
  //vector<float> floatday = {11.08, 12.08, 13.08, 14.08, 15.08};
  //vector<string> runid = {"357287", "357337", "357415", "357456", "357501"};

  // #6:
  //vector<float> days = {42, 43, 44, 46, 47};
  //vector<float> lumi = {6.434, 6.788, 6.7885, 7.950, 8.585};
  //vector<float> floatday = {16.08, 17.08, 18.08, 20.08, 21.08};
  //vector<string> runid = {"357564", "357622", "357646", "357743", "357787"};

  // #7:
  //vector<float> days = {48, 49, 50, 51, 52};
  //vector<float> lumi = {9.165, 9.6485, 9.649, 9.649, 9.649};
  //vector<float> floatday = {22.08, 23.08, 24.08, 25.08, 26.08};
  //vector<string> runid = {"357845", "357968", "357996", "358087", "358101"};

  // #8:
  //vector<float> days = {54, 55, 56, 57, 58};
  //vector<float> lumi = {9.649, 9.649, 9.649, 9.649, 9.649};
  //vector<float> floatday = {28.08, 29.08, 30.08, 31.08, 01.09};
  //vector<string> runid = {"358160", "358179", "358222", "358277", "358303"};

  // #9:
  //vector<float> days = {59, 62, 63, 64, 66};
  //vector<float> lumi = {9.649, 9.649, 9.649, 9.649, 9.649};
  //vector<float> floatday = {02.09, 05.09, 06.09, 07.09, 09.09};
  //vector<string> runid = {"358338", "358430", "358488", "358543", "358595"};

  // #10:
  vector<float> days = {69};
  vector<float> lumi = {9.649};
  vector<float> floatday = {12.09};
  vector<string> runid = {"358663"};


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
    histarray[n] = new TH1F****[2];
    for(int t=0; t<2; t++){ // 0=Small SiPM, 1=Large SiPM
      histarray[n][t] = new TH1F***[58];
      for(int i=0; i<58; i++){ // Number of ieta bins -29..29, excluding 0
        histarray[n][t][i] = new TH1F**[72];
        for(int j=0; j<72; j++){ // Number of iphi bins 1..72
          histarray[n][t][i][j] = new TH1F*[7];
          for(int k=0; k<7; k++){ // Number of depths 1..7
            histarray[n][t][i][j][k] = new TH1F(("hist_run"+runid.at(n)+"_sipmT"+to_string(t)+"_ieta"+to_string(i<=29?i-29:i-28)+"_iphi"+to_string(j+1)+"_depth"+to_string(k+1)).c_str(), "Pedestal per Channel; ADC; Entries", 1600, 0, 40);
          }
        }
      }
    }
  }

  TH1F **** pedMean;
  TH1F **** pedRMS;
  TH1F **** PEDMeanDepth;
  TH1F **** PEDRMSDepth;
  pedMean = new TH1F***[2];
  pedRMS = new TH1F***[2];
  PEDMeanDepth = new TH1F***[2];
  PEDRMSDepth = new TH1F***[2];
  for(int nh=0; nh<2; nh++){ // HB and HE
    string det = nh==0?"HB":"HE";
    pedMean[nh]=new TH1F**[2];
    pedRMS[nh]=new TH1F**[2];
    PEDMeanDepth[nh]=new TH1F**[2];
    PEDRMSDepth[nh]=new TH1F**[2];
    for(int nt=0; nt<2; nt++){ // 0=Small SiPM, 1=Large SiPM
      pedMean[nh][nt]=new TH1F*[nruns];
      pedRMS[nh][nt]=new TH1F*[nruns];
      for(int n=0; n<nruns; n++){ // One per run
        pedMean[nh][nt][n] = new TH1F((det+"_sipmT"+to_string(nt)+"pedMean_run"+runid.at(n)).c_str(), "Pedestal Mean; ADC; Entries", 1600, 0, 40);
        pedRMS[nh][nt][n] = new TH1F((det+"_sipmT"+to_string(nt)+"pedRMS_run"+runid.at(n)).c_str(), "Pedestal RMS; ADC; Entries", 1600, 0, 40);
      }
    }
    for(int k=0; k<7; k++){ // Depth
      PEDMeanDepth[nh][k]=new TH1F*[nruns];
      PEDRMSDepth[nh][k]=new TH1F*[nruns];
      for(int n=0; n<nruns; n++){ // One per run
        PEDMeanDepth[nh][k][n] = new TH1F((det+"_depth"+to_string(k)+"pedMean_run"+runid.at(n)).c_str(), "Pedestal Mean; ADC; Entries", 1600, 0, 40);
        PEDRMSDepth[nh][k][n] = new TH1F((det+"_depth"+to_string(k)+"pedRMS_run"+runid.at(n)).c_str(), "Pedestal RMS; ADC; Entries", 1600, 0, 40);
      }
    }
  }

  std::cout << "Looping over input events..." << std::endl;

  for(int i=0; i<ntot; i++){
    if(i%50000000==0) std::cout << i << "-th entry." << std::endl;
    qiedigi->GetEntry(i);

    if(find(runid.begin(), runid.end(), to_string((int)RunNum))==runid.end()) continue;

    if(shunt!=6.0) continue;

    int runidx = distance(runid.begin(), find(runid.begin(), runid.end(), to_string((int)RunNum)) );
    int ietaidx = ieta<0?ieta+29:ieta+28;
    int iphiidx = iphi-1;
    int depthidx = depth-1;
    int sipmidx = -1;
    if(type==3||type==5) sipmidx=0; // Small SiPM
    else if(type==4||type==6) sipmidx=1; // Large SiPM

    //std::cout << runidx << ", " << sipmidx << ", " << ietaidx << ", " << iphiidx << ", " << depthidx << std::endl;

    histarray[runidx][sipmidx][ietaidx][iphiidx][depthidx]->Fill(sumADC/8.0); // Average ADC of all 8 TSs
 

  }


  std::cout << "Postprocessing histograms..." << std::endl;

  TH3F* h3 = new TH3F("h3", "h3", 59, -29, 30, 72, 0, 72, 7, 1, 8);

  for(int n=0; n<nruns; n++){
    for(int t=0; t<2; t++){
      for(int i=0; i<58; i++){
        for(int j=0; j<72; j++){
          for(int k=0; k<7; k++){
            if(histarray[n][t][i][j][k]->GetEntries()>0){
              int nh=-1;
              if((i>=14 && i<=43) || (i==13 && k<3) || (i==44 && k<3)) nh=0; // HB
              else if((i<=12 || i>=45 ) || (i==13 && k>=3) || (i==44 && k>=3)) nh=1; // HE
              pedMean[nh][t][n]->Fill(histarray[n][t][i][j][k]->GetMean());
              pedRMS[nh][t][n]->Fill(histarray[n][t][i][j][k]->GetRMS());
              PEDMeanDepth[nh][k][n]->Fill(histarray[n][t][i][j][k]->GetMean());
              PEDRMSDepth[nh][k][n]->Fill(histarray[n][t][i][j][k]->GetRMS());
              if(t==1 && nh==0 && histarray[n][t][i][j][k]->GetMean() < 5.6) h3->Fill(i<29?i-29:i-28, j, k+1); // Large SiPM, HB, Small mean
            }
          }
        }
      }
    }
  }

  //ofstream myfile;
  //myfile.open ("meanRMS.txt");  
  //myfile << "Mean = " << GetMean() << "RMS = " << GetRMS() <<endl;
  //myfile.close();

  vector<float> MeanofMeanPedVal[2][2];
  vector<float> RMSofMeanPedVal[2][2];
  vector<float> MeanofRMSPedVal[2][2];
  ofstream savefile;
  savefile.open("SaveFile.txt", ios::app);
  for(int nh=0; nh<2; nh++){ // HB / HE
    for(int nt=0; nt<2; nt++){ // Small / Large SiPM
      for(int n=0; n<nruns; n++){ // Runs
        MeanofMeanPedVal[nh][nt].push_back(pedMean[nh][nt][n]->GetMean());
        RMSofMeanPedVal[nh][nt].push_back(pedMean[nh][nt][n]->GetRMS());
        MeanofRMSPedVal[nh][nt].push_back(pedRMS[nh][nt][n]->GetMean());
        savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " MeanMean " << pedMean[nh][nt][n]->GetMean() << "\n";
        savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " RMSMean " << pedMean[nh][nt][n]->GetRMS() << "\n";
        savefile << runid[n] << " " << lumi[n] << " " << days[n] << " " << floatday[n] << " " << nh << " " << nt << " MeanRMS " << pedRMS[nh][nt][n]->GetMean() << "\n";
      }
    }
  }
  savefile.close();

  TGraph *** MeanofMeanvsLumi;
  TGraph *** RMSofMeanvsLumi;
  TGraph *** MeanofRMSvsLumi;
  MeanofMeanvsLumi = new TGraph**[2];
  RMSofMeanvsLumi = new TGraph**[2];
  MeanofRMSvsLumi = new TGraph**[2];
  for(int nh=0; nh<2; nh++){
    string det = nh==0?"HB":"HE";
    MeanofMeanvsLumi[nh]=new TGraph*[2];
    RMSofMeanvsLumi[nh]=new TGraph*[2];
    MeanofRMSvsLumi[nh]=new TGraph*[2];
    for(int nt=0; nt<2; nt++){
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
  h3->Write();

  for(int nh=0; nh<2; nh++){
    for(int nt=0; nt<2; nt++){
      MeanofMeanvsLumi[nh][nt]->Write();
      RMSofMeanvsLumi[nh][nt]->Write();
      MeanofRMSvsLumi[nh][nt]->Write();
    }
  }

  for(int nh=0; nh<2; nh++){
    for(int nt=0; nt<2; nt++){
      for(int n=0; n<nruns; n++){
        pedMean[nh][nt][n]->Write();
        pedRMS[nh][nt][n]->Write();
      }
    }
    for(int k=0; k<7; k++){
      for(int n=0; n<nruns; n++){
         PEDMeanDepth[nh][k][n]->Write();
         PEDRMSDepth[nh][k][n]->Write();
      }
    }
  }

  ofile->Close();


  std::cout << "End Job." << std::endl;
  return 0;

}


