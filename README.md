# MyHcalAnlzr

The setup was changed to make everything more automated.

In the file `doAbsolutelyEverything.sh` one should document the days at which local runs were taken, and what the local runs are and the integrated luminosity at the time. Comment out already processed lines and run the script to process new files

In case some steps need to be redone, uncomment everything in `doAbsolutelyEverything.sh`, and comment intermediate steps not required to be redone (such as running `cmsRun`) in the script `doEverything_v2.sh`
