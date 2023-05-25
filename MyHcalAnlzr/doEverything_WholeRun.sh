floatday=$1
run=$2

#run="363427"
#days="39"
#lumi="0.0"
#floatday="08.02"


# HcalNano step + Digi step + Process step
python3 FindDatasetToRun.py ${floatday} ${run} WholeRun


# Make plots: Make trends from values saved in savefile, and get example histograms from rootfile
python3 Plotting.py ${run} wholerun
mv SaveFile_${run}.txt* WholeRunOutput_${run}
mkdir PED_run${run}
mv WholeRunOutput_${run}/*png PED_run${run}
mv WholeRunOutput_${run}/*pdf PED_run${run}
#mv /eos/user/d/dmroy/www/plots_archive/PED_plots/extrapolations/ExtrapolationFC_H* /eos/user/d/dmroy/www/plots_archive/PED_plots/extrapolations_FC/
