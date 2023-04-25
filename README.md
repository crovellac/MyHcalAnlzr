# MyHcalAnlzr

The setup was changed to make everything more automated. When running by yourself, just make sure that outputs are being written/read in accessible places! (I.e., not on my personal EOS space. Just `grep` for `dmroy`.)

The HcalNano setup requires the HcalNano package: `https://gitlab.cern.ch/cmshcal/hcalpfg/hcalnanoprod`. At this time (End of April), a manual fix is required: The path `HLT_HcalCalibration_v5` was replaced for `HLT_HcalCalibration_v6` (for collisions), and so this change needs to be made in the HcalNano configurations.

In the file `doAbsolutelyEverything.sh` one should document the days and the integrated luminosity at the time. As an optional argument, add the run number of a COLLISIONS run, ideally close to the corresponding recorded luminosity at the time. Comment out already processed lines and run the script to process new files.

In case some steps need to be redone, uncomment everything in `doAbsolutelyEverything.sh`, and comment intermediate steps not required to be redone (such as running `cmsRun`) in the script `doEverything.sh`
