import ROOT
import os, sys
from array import array

##### Start
ROOT.gStyle.SetOptFit(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)

#runid = sys.argv[1] # Integer as string
days = sys.argv[1]
lumi = sys.argv[2]
floatday = sys.argv[3] # e.g. "05.07"
WholeRun = True if lumi=="WholeRun" else False
# if WholeRun, then "days" is actually run number, and "floatday" is actually input filename

fileName = [f for f in os.listdir(".") if f.endswith("_"+floatday+".root")][0]
if not WholeRun:
  runid = fileName[20:26]
else:
  runid = days
  lumi = fileName.split("LS")[1].split("_")[0]
fin=ROOT.TFile.Open(fileName, "READ")

histos = {}
histos["FC"] = {}
histos["ADC"] = {}
finalhistos = {}
subdets = ["HB", "HE", "HF", "HO"]
for subdet in subdets:
  histos["FC"][subdet] = {}
  histos["ADC"][subdet] = {}

# Load
for key in fin.GetListOfKeys():
  hname = key.GetName()
  if "_ADC" in hname:
    histos["ADC"][hname[hname.find("subdet")+6:hname.find("subdet")+8]][hname] = fin.Get(hname)
  elif "_FC" in hname:
    histos["FC"][hname[hname.find("subdet")+6:hname.find("subdet")+8]][hname] = fin.Get(hname)

# Define everything to process
pedtrend = ["HB_sipmSmall", "HB_sipmLarge", "HE_sipmSmall", "HE_sipmLarge", "HF", "HO"]
for depth in range(4):
  pedtrend.append("HB_depth"+str(depth+1))
for depth in range(7):
  pedtrend.append("HE_depth"+str(depth+1))
#pedtrend += ["HB_sipmSmall_phi,1,72", "HB_sipmLarge_phi,1,72", "HE_sipmSmall_phi,1,72", "HE_sipmLarge_phi,1,72"]
#pedtrend += ["HB_sipmSmall_phi,18,19", "HB_sipmLarge_phi,18,19", "HE_sipmSmall_phi,18,19", "HE_sipmLarge_phi,18,19"]
#pedtrend += ["HB_sipmSmall_phi,36,37", "HB_sipmLarge_phi,36,37", "HE_sipmSmall_phi,36,37", "HE_sipmLarge_phi,36,37"]
pedtrend += ["HB_sipmSmall_HBP14RM1", "HB_sipmLarge_HBP14RM1", "HB_sipmSmall_HBM09RM3", "HB_sipmLarge_HBM09RM3"]
pedtrend += ["HB_sipmSmall_HBM04RM3", "HB_sipmLarge_HBM04RM3"]

def IsHBP14RM1(hname):
  if "iphi51" in hname and "ieta-" not in hname: return True
  return False
def IsHBM09RM3(hname):
  if "iphi32" in hname and "ieta-" in hname: return True
  return False
def IsHBM04RM3(hname):
  if "iphi12" in hname and "ieta-" in hname: return True
  return False

# Process
for p in pedtrend:
  subdet = p.split("_")[0]
  finalhistos[p+"_ADC_Mean"] = ROOT.TH1F(p+"_pedADCMean_run"+runid, "Pedestal Mean; ADC; Entries",  960, 0, 32)
  finalhistos[p+"_ADC_RMS"] = ROOT.TH1F(p+"_pedADCRMS_run"+runid, "Pedestal RMS; ADC; Entries",  960, 0, 32)
  finalhistos[p+"_FC_Mean"] = ROOT.TH1F(p+"_pedFCMean_run"+runid, "Pedestal Mean; fC; Entries", 10000, 0, 1000)
  finalhistos[p+"_FC_RMS"] = ROOT.TH1F(p+"_pedFCRMS_run"+runid, "Pedestal RMS; fC; Entries", 10000, 0, 1000)
  for unit in ["ADC", "FC"]:
    for hname in histos[unit][subdet]:
      skip = False
      for cut in p.split("_")[1:]:
        if "phi" in cut: # Special phi cuts
          if all("phi"+phicut+"_" not in hname for phicut in cut.split(",")[1:]): skip = True
        elif "HBP14RM1" in cut:
          if not IsHBP14RM1(hname): skip=True
        elif "HBM09RM3" in cut:
          if not IsHBM09RM3(hname): skip=True
        elif "HBM04RM3" in cut:
          if not IsHBM04RM3(hname): skip=True
        elif cut not in hname: skip = True
      if ("HB" in p) and ("HBP14RM1" not in p) and (IsHBP14RM1(hname)): skip=True
      if ("HB" in p) and ("HBM09RM3" not in p) and (IsHBM09RM3(hname)): skip=True
      if ("HB" in p) and ("HBM04RM3" not in p) and (IsHBM04RM3(hname)): skip=True
      if not skip:
        mean = histos[unit][subdet][hname].GetMean()
        rms = histos[unit][subdet][hname].GetRMS()
        if mean!=0.0 and rms!=0.0:
          finalhistos[p+"_"+unit+"_Mean"].Fill(mean)
          finalhistos[p+"_"+unit+"_RMS"].Fill(rms)
        #if mean<1 or rms<0.2: print("Low values for",subdet,hname,": Mean =",mean,", RMS =",rms)
        #if mean>7 and "HE" in p: print("High values for",subdet,hname,": Mean =",mean,", RMS =",rms)

# Write in text file
savefilename = "SaveFile.txt" if not WholeRun else "SaveFile_"+runid+".txt"
with open(savefilename, "a") as file:
  # To be written:
  # RUN LUMI DAYSINCE FLOATDAY TRENDNAME MeanMean/MeanRMS/RMSMean value
  idname = runid+" "+lumi+" "+days+" "+floatday if not WholeRun else runid+" "+lumi+" X X"
  for trend in finalhistos:
    if trend.endswith("RMS"): continue
    if finalhistos[trend].GetMean()==0: continue # HO in Full runs is empty
    trend = trend[:-5]
    file.write(idname+" "+trend+" MeanMean "+str(finalhistos[trend+"_Mean"].GetMean()) + "\n")
    file.write(idname+" "+trend+" MeanRMS "+str(finalhistos[trend+"_RMS"].GetMean()) + "\n")
    file.write(idname+" "+trend+" RMSMean "+str(finalhistos[trend+"_Mean"].GetRMS()) + "\n")
    file.write(idname+" "+trend+" RMSRMS "+str(finalhistos[trend+"_RMS"].GetRMS()) + "\n")

# Write histograms
rootfilename = "hist_CalibOutputSummary_run"+runid+".root" if not WholeRun else "hist_CalibOutputSummary_run"+runid+"_"+floatday+".root"
fout=ROOT.TFile.Open(rootfilename, "RECREATE")
for fhist in finalhistos:
  finalhistos[fhist].Write()
fout.Close()
fin.Close()
print("Done!")
exit()
