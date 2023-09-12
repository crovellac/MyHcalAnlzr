# MyHcalAnlzr

This is the setup for GLOBAL pedestal studies! For the LOCAL setup, check out the "master" branch!


## General info

The setup was changed to make everything more automated. When running by yourself, just make sure that outputs are being written/read in accessible places! (I.e., not on my personal EOS space. Just `grep` for `dmroy`.)

~~The HcalNano setup requires the HcalNano package: `https://gitlab.cern.ch/cmshcal/hcalpfg/hcalnanoprod`. At this time (End of April), a manual fix is required: The path `HLT_HcalCalibration_v5` was replaced for `HLT_HcalCalibration_v6` (for collisions), and so this change needs to be made in the HcalNano configurations.~~

HcalNano is automatically incoorperated in new CMSSW releases, so nothing else is needed besides this repository. An up-to-date CMSSW version is anyhow required to process global runs. At this time (Run2023F), CMSSW_13_2_2 works.

In the file `doAbsolutelyEverything.sh` one should document the days and the integrated luminosity at the time. As an optional argument, add the run number of a COLLISIONS run, ideally close to the corresponding recorded luminosity at the time. Comment out already processed lines and run the script to process new files.

In case some steps need to be redone, uncomment everything in `doAbsolutelyEverything.sh`, and comment intermediate steps not required to be redone (such as running `cmsRun`) in the script `doEverything.sh`

Before running it the first time, remember to compile first!


## How to get new data

Reminder that this specific setup is for global runs! You can find global runs on OMS (link [here](https://cmsoms.cern.ch/cms/run_3/index)). Make sure you pick a run that you want (with "collisions" HLT mode if you want to have events from the calibration gap). Also make sure the run as adequately long: The script will ignore input files that are too small (to ensure that there are enough event in the one single file that's being processed). The corresponding (recorded) luminosity can also be obtained from OMS.

The scripts will automatically find a good (enough) input file to measure the pedestals from. The only thing to pay attention to, is when the era changes. In the file `doAbsolutelyEverything.sh`, the `path` should be changes accordingly. Previous paths are commented as an example, and for easy bookkeeping of the previous eras.


## How to make new ZS thresholds

For every run, an xml file is automatically created, which contains Zero Suppression (ZS) thresholds. They are set to the mean pedestals of each channel, plus three times the standard deviation. This is based on the measured charge in fC, but is then converted to ADC counts, and rounded up to the next ADC count. All of these options can be set in the `digi_fromNano.cc` file, if there is need for something to be changed (usually not, unless something is to be checked).

However, for the best ZS tables, you shouldn't just use any global run as input. Or more specifically: The input file from which the pedestals are measured shouldn't just be any random file from a random part of the run. This is because the pedestals measured in global in the calibration gap strongly depend on the current instantaneous luminosity (or PileUp) at the time, and this changes throughout a single fill (At first leveled up to a certain PU, then decreasing). We (Ops+DPG) agreed, that the pedestals should measured at the point, where the instantaneous luminosity takes an average value throughout the fill.

In short: When an update in ZS thresholds is requested, a very specific input file should be used to obtain the pedestals and get the ZS xml file. Here is how to find that input:
- First, check on OMS (link [here](https://cmsoms.cern.ch/cms/run_3/index)) the most recent, completed fill
- If there are interuptions in the plot over the instantaneous luminosity, or if the fill ended early, then don't use this fill: Either wait for the next one, or use a previous one.
- Run `python3 GetAvgPU.py *fill number*`, this will give you the average instantaneous luminosity of that fill
- Now check the instantaneous luminosity plot for the fill on OMS, and find the approximate time (ideally in the range where it's decreasing) when the instantaneous luminosity was the average value. (Remember: The default timezone on OMS is UTC, not CERN time! The difference is 2 hours!)
- Now use this template: `https://cmsoms.cern.ch/agg/api/v1/diplogger/dip/CMS/BRIL/Luminosity?fields=run,lumi_section,dip_time,inst_lumi&page[offset]=0&page[limit]=10000&filter[dip_time][GE]=2023-05-18%2020:00:00&filter[dip_time][LE]=2023-05-18%2022:00:00&sort=dip_time`. The two dates/times near the end of this url are a time range. Change these accordingly, so your previously determined time is within the range.
- This will give you a list of BRIL luminosity measurements. Go on "data", then search for random entries `*number*/attributes` until you find one with a close enough instantaneous luminosity. Note the corresponding run number and luminosity section (LS).
- Now run `python3 GetAvgPU.py *fill number* *run number* *LS*`, that's the same command as before, just add the run number and LS as additional argument. This will tell you which input file you need to use
- Add this root file into the `whitelist_file` list in the file `FindDatasetToRun.py`
- Now you can run `doAbsolutelyEverything.sh` as normal.
