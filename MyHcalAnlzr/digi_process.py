import ROOT
import os, sys
from array import array

##### Start
ROOT.gStyle.SetOptFit(1)
ROOT.gStyle.SetOptStat(0)
ROOT.gROOT.SetBatch(True)

kind = sys.argv[1] # "Full" or "Ped"
days = int(sys.argv[2])
lumi = float(sys.argv[3])
floatday = sys.argv[4] # e.g. "05.07"
runid = sys.argv[5] # Integer as string
#kind = "Full"
#days = 103
#lumi = 16.907
#floatday = "16.10"
#runid = "360463"


fin=ROOT.TFile.Open("hist_LocalOutput_run"+runid+".root", "READ")
histos = {}
finalhistos = {}
subdets = ["HB", "HE", "HF", "HO"]
for subdet in subdets:
  histos[subdet] = {}

# Load
for key in fin.GetListOfKeys():
  hname = key.GetName()
  histos[hname[hname.find("subdet")+6:hname.find("subdet")+8]][hname] = fin.Get(hname)

# Define everything to process
pedtrend = ["HB_sipmSmall", "HB_sipmLarge", "HE_sipmSmall", "HE_sipmLarge", "HF", "HO"]
for depth in range(4):
  pedtrend.append("HB_depth"+str(depth+1))
for depth in range(7):
  pedtrend.append("HE_depth"+str(depth+1))
pedtrend += ["HB_sipmSmall_phi,1,72", "HB_sipmLarge_phi,1,72", "HE_sipmSmall_phi,1,72", "HE_sipmLarge_phi,1,72"]
pedtrend += ["HB_sipmSmall_phi,12,13", "HB_sipmLarge_phi,12,13", "HE_sipmSmall_phi,12,13", "HE_sipmLarge_phi,12,13"]
pedtrend += ["HB_sipmSmall_phi,36,37", "HB_sipmLarge_phi,36,37", "HE_sipmSmall_phi,36,37", "HE_sipmLarge_phi,36,37"]
pedtrend += ["HB_sipmSmall_HBP14RM1", "HB_sipmLarge_HBP14RM1", "HB_sipmSmall_HBM09RM3", "HB_sipmLarge_HBM09RM3"]

def IsHBP14RM1(hname):
  if "iphi51" in hname and "ieta-" not in hname: return True
  return False
def IsHBM09RM3(hname):
  if "iphi32" in hname and "ieta-" in hname: return True
  return False

# Process
for p in pedtrend:
  subdet = p.split("_")[0]
  finalhistos[p+"Mean"] = ROOT.TH1F(p+"_pedMean_run"+runid, "Pedestal Mean; ADC; Entries", 4000, 0, 40)
  finalhistos[p+"RMS"] = ROOT.TH1F(p+"_pedRMS_run"+runid, "Pedestal RMS; ADC; Entries", 4000, 0, 40)
  for hname in histos[subdet]:
    skip = False
    for cut in p.split("_")[1:]:
      if "phi" in cut: # Special phi cuts
        if all("phi"+phicut+"_" not in hname for phicut in cut.split(",")[1:]): skip = True
      elif "HBP14RM1" in cut:
        if not IsHBP14RM1(hname): skip = True
      elif "HBM09RM3" in cut:
        if not IsHBM09RM3(hname): skip = True
      elif cut not in hname: skip = True
    if ("HB" in p) and ("HBP14RM1" not in p) and (IsHBP14RM1(hname)): skip = True
    if ("HB" in p) and ("HBM09RM3" not in p) and (IsHBM09RM3(hname)): skip = True
    if not skip:
      mean = histos[subdet][hname].GetMean()
      rms = histos[subdet][hname].GetRMS()
      if mean!=0.0 and rms!=0.0:
        finalhistos[p+"Mean"].Fill(mean)
        finalhistos[p+"RMS"].Fill(rms)
      #if mean<1 or rms<0.2: print("Low values for",subdet,hname,": Mean =",mean,", RMS =",rms)
      #if mean>7 and "HE" in p: print("High values for",subdet,hname,": Mean =",mean,", RMS =",rms)

# Write in text file
with open("SaveFile.txt", "a") as file:
  # To be written:
  # RUN LUMI DAYSINCE FLOATDAY TRENDNAME MeanMean/MeanRMS/RMSMean value
  for trend in finalhistos:
    if trend.endswith("RMS"): continue
    trend = trend[:-4]
    if finalhistos[trend+"Mean"].GetMean()==0: continue # HO in Full runs is empty
    file.write(runid+" "+str(lumi)+" "+str(days)+" "+floatday+" "+trend+" MeanMean "+str(finalhistos[trend+"Mean"].GetMean()) + "\n")
    file.write(runid+" "+str(lumi)+" "+str(days)+" "+floatday+" "+trend+" MeanRMS "+str(finalhistos[trend+"RMS"].GetMean()) + "\n")
    file.write(runid+" "+str(lumi)+" "+str(days)+" "+floatday+" "+trend+" RMSMean "+str(finalhistos[trend+"Mean"].GetRMS()) + "\n")
    file.write(runid+" "+str(lumi)+" "+str(days)+" "+floatday+" "+trend+" RMSRMS "+str(finalhistos[trend+"RMS"].GetRMS()) + "\n")

# Write histograms
ped = "PED" if kind=="Ped" else ""
fout=ROOT.TFile.Open("hist_LocalOutputSummary_"+ped+"run"+runid+".root", "RECREATE")
for fhist in finalhistos:
  finalhistos[fhist].Write()
fout.Close()
fin.Close()
print("Done!")
exit()

