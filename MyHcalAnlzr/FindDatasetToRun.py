import os, sys
import glob
import time
import json

date = sys.argv[1]
day = str(int(date.split(".")[0]))
month = str(int(date.split(".")[1]))

#path = "/eos/cms/tier0/store/data/Commissioning2023/TestEnablesEcalHcal/*/*/*"
path = "/eos/cms/tier0/store/data/Run2023A/TestEnablesEcalHcal/*/*/*"

blacklist = ["244aa98d-1bc6-4c3f-bf02-36032473b104.root"]

allfiles_list = [f for f in glob.glob(path+"/*/*/*/*") if f.endswith(".root") and f.split("/")[-1] not in blacklist]
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
if pureRuns!=[]:
  runs = pureRuns
elif mixedRuns!=[]:
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

# Process the largest file
size = 0
myfile = ""
for f in files:
  fs = os.path.getsize(f)
  if fs > size:
    size = fs
    myfile = f
if myfile == "":
  print("NO FILES FOUND!")
else:
  print("Processing",myfile,"...")
run = myfile.split("/")[11]+myfile.split("/")[12]

# Run
os.system("cp HcalNano_Template.sh HcalNano_"+run+".sh")
filein = myfile.replace("/eos/cms/tier0", "").replace("/", "\/")
os.system('sed -i "s/FILEIN/'+filein+'/g" HcalNano_'+run+'.sh')
os.system('sed -i "s/XXXXXX/'+run+'/g" HcalNano_'+run+'.sh')
os.system('sed -i "s/DAY/'+date+'/g" HcalNano_'+run+'.sh')
os.system('. ./HcalNano_'+run+'.sh') # "source" somehow doesn't work here, but it works when you just replace it with "."

print("Done making NanoTuple!")
exit()
