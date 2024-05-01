days=$1
lumi=$2
floatday=$3
run=$4

# HcalNano step
python3 FindDatasetToRun.py ${floatday} ${run}


# Digi step: From nano tuple, make one histogram per channel
./macro_nano ${floatday}


# Process step: From previous histograms, combine kinds (e.g. all HB, all HE, ...)
python3 digi_process.py ${days} ${lumi} ${floatday}
mv hist_CalibOutput_hadd.root hist_CalibOutput_hadd.root_old
hadd hist_CalibOutput_hadd.root hist_CalibOutputSummary_run*.root


# Make plots: Make trends from values saved in savefile, and get example histograms from rootfile
source makePlots.sh
