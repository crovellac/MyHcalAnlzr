from omsapi import OMSAPI
import datetime
import sys
from subprocess import Popen, PIPE, STDOUT

fill = 8786 if len(sys.argv)<2 else sys.argv[1]
if len(sys.argv)>3:
  run = sys.argv[2]
  lumi = sys.argv[3]

def dasgoclient(query):
  dascmd = 'dasgoclient --query="%s"'%(query)
  out = ""
  try:
    process = Popen(dascmd,stdout=PIPE,stderr=STDOUT,shell=True) # ,universal_newlines=True, bufsize=1
    for line in iter(process.stdout.readline,b""):
      if isinstance(line,bytes):
        line = line.decode('utf-8')
      out += line
    process.stdout.close()
    retcode = process.wait()
    out = out.strip()
  except Exception as e:
    return ""
  if retcode:
    return ""
  return out

omsapi = OMSAPI("https://cmsoms.cern.ch/agg/api", "v1", cert_verify=False)
omsapi.auth_krb()
query = omsapi.query("fills")
my_filters = [{"attribute_name": "fill_number", "value": fill, "operator": "EQ"}]
query.filters(my_filters)
response = query.data()
myjson = response.json()

start = myjson["data"][0]["attributes"]["start_stable_beam"]
end = myjson["data"][0]["attributes"]["end_stable_beam"]
integLumi = myjson["data"][0]["attributes"]["delivered_lumi"]
ts = datetime.datetime.strptime(start, "%Y-%m-%dT%H:%M:%SZ") # E.g.: 2023-05-19T04:15:57Z
te = datetime.datetime.strptime(end, "%Y-%m-%dT%H:%M:%SZ")
time = (te - ts).total_seconds()
avgInstLumi = integLumi / time
print("\n")
print("Average inst. lumi. in Fill",fill,":",avgInstLumi*1000000,"* 10^30 cm^-2 s^-1")
if len(sys.argv)<4:
  print("Now look for corresponding appoximate time range, to find exact run and LS!")
  print("https://cmsoms.cern.ch/agg/api/v1/diplogger/dip/CMS/BRIL/Luminosity?fields=run,lumi_section,dip_time,inst_lumi&page[offset]=0&page[limit]=10000&filter[dip_time][GE]=2023-05-18%2020:00:00&filter[dip_time][LE]=2023-05-18%2022:00:00&sort=dip_time")
  print("Then rerun this: 'python3 GetAvgPU.py %s *RUN* *LS*'"%(fill))
  #print("Then find needed input file on DAS:")
  #print("file dataset=/TestEnablesEcalHcal/Run2023C-Express-v2/RAW run=367696 lumi=1000")
else:
  print("I've been told this corresponds to run %s, LS %s ..."%(run,lumi))
  output = dasgoclient("dataset run="+run)
  datasets = output.split("\n")
  print(datasets)
  dataset = [d for d in datasets if "TestEnablesEcalHcal" in d][0]
  output = dasgoclient("file dataset="+dataset+" run="+run+" lumi="+lumi)
  print("Use this file:")
  print(output)
  print(output.split("/")[-1])

exit()
