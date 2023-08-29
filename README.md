# MyHcalAnlzr

This is the setup for GLOBAL pedestal studies! For the LOCAL setup, check out the "master" branch!



The setup was changed to make everything more automated. When running by yourself, just make sure that outputs are being written/read in accessible places! (I.e., not on my personal EOS space. Just `grep` for `dmroy`.)

The HcalNano setup requires the HcalNano package: `https://gitlab.cern.ch/cmshcal/hcalpfg/hcalnanoprod`. At this time (End of April), a manual fix is required: The path `HLT_HcalCalibration_v5` was replaced for `HLT_HcalCalibration_v6` (for collisions), and so this change needs to be made in the HcalNano configurations.

In the file `doAbsolutelyEverything.sh` one should document the days and the integrated luminosity at the time. As an optional argument, add the run number of a COLLISIONS run, ideally close to the corresponding recorded luminosity at the time. Comment out already processed lines and run the script to process new files.

In case some steps need to be redone, uncomment everything in `doAbsolutelyEverything.sh`, and comment intermediate steps not required to be redone (such as running `cmsRun`) in the script `doEverything.sh`



Reminder that this specific setup is for global runs! You can find global runs on OMS (link [here](https://cmsoms.cern.ch/cms/run_3/index)). Make sure you pick a run that you want (with "collisions" HLT mode if you want to have events from the calibration gap). Also make sure the run as adequately long: The script will ignore input files that are too small (to ensure that there are enough event in the one single file that's being processed). The corresponding (recorded) luminosity can also be obtained from OMS.
