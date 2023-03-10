# MyHcalAnlzr

The setup was changed to make everything more automated. When running by yourself, just make sure that outputs are being written/read in accessible places! (I.e., not on my personal EOS space. Just `grep` for `dmroy`.)

At this point, with the HcalNano setup, one still needs to manually point at a input file correcponding to a certain run.

In the file `doAbsolutelyEverything.sh` one should document the days at which local runs were taken, and what the local runs are and the integrated luminosity at the time. Comment out already processed lines and run the script to process new files

In case some steps need to be redone, uncomment everything in `doAbsolutelyEverything.sh`, and comment intermediate steps not required to be redone (such as running `cmsRun`) in the script `doEverything.sh`
