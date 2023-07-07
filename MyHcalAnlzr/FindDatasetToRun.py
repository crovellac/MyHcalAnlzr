import os, sys
import glob
import time
import json

date = sys.argv[1]
day = str(int(date.split(".")[0]))
month = str(int(date.split(".")[1]))
whitelistrun = sys.argv[2] if len(sys.argv)>2 else ""
WholeRun = True if len(sys.argv)>3 and sys.argv[3]=="WholeRun" else False # Will only work for a given run!
MakeSmallerFiles = True

def MakeSmall(path):
  patht1 = path.replace(".root", "_temp.root")
  patht2 = path.replace(".root", "_temp2.root")
  while True:
    # Remove HLT branches and filter non-eventtype 1 events
    os.system('rooteventselector -s "(uMNio_EventType == 1)" -e "HLT_*" '+path+':Events '+patht1)
    # Re-compress
    success = os.system('python3 haddnano.py '+patht2+' '+patht1)
    os.system('rm '+patht1)
    if success==0:
      break
    else:
      os.system('rm '+patht2)
  os.system('mv '+path+' '+path.replace(".root", "_FULL.root"))
  os.system('mv '+patht2+' '+path)

#path = "/eos/cms/tier0/store/data/Commissioning2023/TestEnablesEcalHcal/*/*/*"
#path = "/eos/cms/tier0/store/data/Run2023A/TestEnablesEcalHcal/*/*/*"  # 06.04.-20.04.
#path = "/eos/cms/tier0/store/data/Run2023B/TestEnablesEcalHcal/*/*/*"  # 21.04.-05.05.
#path = "/eos/cms/tier0/store/data/Run2023C/TestEnablesEcalHcal/*/*/*"  # 06.05.-29.06.
path = "/eos/cms/tier0/store/data/Run2023D/TestEnablesEcalHcal/*/*/*"

blacklist_file = ["244aa98d-1bc6-4c3f-bf02-36032473b104.root"]
whitelist_file = ["0bc49ae3-69bc-439c-8dde-526a71ca1e39.root", "4e32fe18-85ca-4c35-9790-821dd051f4cf.root", "abbda151-86ea-4609-a0ba-820fd14fa9e9.root", "3afeb82e-a911-46c2-bbcb-dc74f90022cd.root", "663ffe17-ddf0-43b5-ba49-24594331baaf.root", "0ba626ee-a60a-436a-bd10-35d05934908f.root", "5f0f2e56-2c6f-4943-95fb-49c79ef053f7.root", "d0497690-6393-44c9-a08f-ec046e5f5cf7.root"] # Use this file, to get ZS threshold at certain inst. lumi.

allfiles_list = [f for f in glob.glob(path+"/*/*/*/*") if f.endswith(".root") and f.split("/")[-1] not in blacklist_file]
print("There are",len(allfiles_list),"files total")
allfiles_done = []
if os.path.isfile('allFilesByDate.json.json'):
  with open('allFilesByDate.json') as jfile:
    allfiles_date = json.load(jfile)
  with open('allRunsByDate.json') as jfile:
    allruns_date = json.load(jfile)
  for dm in allfiles_date:
    allfiles_done += allfiles_date[dm]
else:
  allfiles_date = {}
  allruns_date = {}
for f in allfiles_list:
  if f in allfiles_done: continue
  run = f.split("/")[11]+f.split("/")[12]
  ftime = os.path.getmtime(f)
  fdate = time.gmtime(ftime)
  fday = fdate[2]
  fmonth = fdate[1]
  dm = str(fday)+"."+str(fmonth)
  if dm not in allfiles_date:
    allfiles_date[dm] = []
    allruns_date[dm] = []
  allfiles_date[dm].append(f)
  if run not in allruns_date[dm]: allruns_date[dm].append(run)

# Save file dict
with open('allFilesByDate.json', 'w') as jfile:
  json.dump(allfiles_date, jfile)
with open('allRunsByDate.json', 'w') as jfile:
  json.dump(allruns_date, jfile)

pureRuns = [] # Run lasted only during that day
mixedRuns = [] # Run started on previous day or ended on next day
for run in allruns_date[day+"."+month]:
  mixed = False
  for dm in allruns_date:
    if run in allruns_date[dm] and dm != day+"."+month:
      mixed = True
      break
  if mixed:
    mixedRuns.append(run)
  else:
    pureRuns.append(run)

#Consider using mixed runs only when there are no pure runs
if whitelistrun in pureRuns or whitelistrun in mixedRuns:
  print("Using given run")
  runs = [whitelistrun]
elif pureRuns!=[]:
  print("Using run from given day")
  runs = pureRuns
elif mixedRuns!=[]:
  print("Using run from given day (overlapping with previous or next day)")
  runs = mixedRuns
else:
  print("There are no available runs for this day!")
  exit()

#run = "365482"#sys.argv[1]
#runstr = run[:3]+"/"+run[3:]
#
# Get all files related to run
files = []
for run in runs:
  runstr = run[:3]+"/"+run[3:]
  files += [f for f in glob.glob(path+"/"+runstr+"/*/*") if f.endswith(".root")]

myfile = ""
if any([fstr in f for fstr in whitelist_file for f in files]) and not WholeRun: 
  myfile = [f for fstr in whitelist_file for f in files if fstr in f][0]
else:
  # Get all large files, sort by time, then process the median file
  largefiles = {}
  for f in files:
    fs = os.path.getsize(f)
    if fs > 3758096384: # 3.5G
      largefiles[os.path.getmtime(f)] = f
  if largefiles != {}:
    myfile = largefiles[ sorted(list(largefiles.keys()))[int(len(largefiles)/2.0)] ]
#else:
#  # Process the largest file
#  size = 0
#  for f in files:
#    fs = os.path.getsize(f)
#    if fs > size:
#      size = fs
#      myfile = f

if myfile == "":
  print("NO FILES FOUND!")
  exit()
else:
  print("Processing",myfile,"...")
run = myfile.split("/")[11]+myfile.split("/")[12]

# Run
if not WholeRun:
  os.system("cp HcalNano_Template.sh HcalNano_"+run+".sh")
  filein = myfile.replace("/eos/cms/tier0", "").replace("/", "\/")
  os.system('sed -i "s/FILEIN/'+filein+'/g" HcalNano_'+run+'.sh')
  os.system('sed -i "s/XXXXXX/'+run+'/g" HcalNano_'+run+'.sh')
  os.system('sed -i "s/DAY/'+date+'/g" HcalNano_'+run+'.sh')
  os.system('. ./HcalNano_'+run+'.sh') # "source" somehow doesn't work here, but it works when you just replace it with "."
  if MakeSmallerFiles: MakeSmall('/eos/user/d/dmroy/HCAL/MyHcalAnlzr_Nano/output_CalibRuns_Nano_Run'+run+'_'+date+'.root')
else:
  files = [largefiles[f] for f in largefiles]
  os.system("mkdir WholeRunOutput_"+run)
  for myfile in files:
    fname = myfile.split("/")[-1].split(".")[0]
    os.system("cp HcalNano_Template.sh HcalNano_"+run+"_"+fname+".sh")
    filein = myfile.replace("/eos/cms/tier0", "").replace("/", "\/")
    os.system('sed -i "s/FILEIN/'+filein+'/g" HcalNano_'+run+'_'+fname+'.sh')
    os.system('sed -i "s/XXXXXX/'+run+'_'+fname+'/g" HcalNano_'+run+'_'+fname+'.sh')
    os.system('sed -i "s/_DAY//g" HcalNano_'+run+'_'+fname+'.sh')
    os.system('sed -i "s/-n 5000/-n 300/g" HcalNano_'+run+"_"+fname+'.sh')
    os.system('. ./HcalNano_'+run+'_'+fname+'.sh')
    if MakeSmallerFiles: MakeSmall('/eos/user/d/dmroy/HCAL/MyHcalAnlzr_Nano/output_CalibRuns_Nano_Run'+run+'_'+fname+'.root')
    os.system('./macro_nano '+fname+' 1')
    os.system('python3 digi_process.py '+run+' WholeRun '+fname)
    os.system('mv *'+fname+'* WholeRunOutput_'+run)

print("Done making NanoTuple!")
exit()
