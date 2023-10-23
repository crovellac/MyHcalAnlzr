run=$1
pedrun=$2
days=$3
lumi=$4
floatday=$5

#run="360463" #$1
#pedrun="360468" #$2
#days="103" #$3
#lumi="16.907" #$4
#floatday="16.10" #$5

#run="355079"
#pedrun="XXX"
#days="0"
#lumi="0.001"
#floatday="05.07"

#run="359299"
#pedrun="359309"
#days="83"
#lumi="9.654"
#floatday="26.09"


# cmsRun step -> Make ntuples
cd python
sed -i "s/XXXXXX/${run}/g" localrun_singleFull.py
cmsRun localrun_singleFull.py
sed -i "s/${run}/XXXXXX/g" localrun_singleFull.py
if [[ "$pedrun" != "XXX" ]]; then
  sed -i "s/XXXXXX/${pedrun}/g" localrun_singlePed.py
  cmsRun localrun_singlePed.py
  sed -i "s/${pedrun}/XXXXXX/g" localrun_singlePed.py
fi
cd ..


# Digi step -> Make per-channel plots, table, xml
source rundigi_single.sh Full ${days} ${lumi} ${floatday} ${run}
if [[ "$pedrun" != "XXX" ]]; then
  source rundigi_single.sh Ped ${days} ${lumi} ${floatday} ${pedrun}
fi


# Process step
python3 digi_process.py Full ${days} ${lumi} ${floatday} ${run}
if [[ "$pedrun" != "XXX" ]]; then
  python3 digi_process.py Ped ${days} ${lumi} ${floatday} ${pedrun}
fi
mv hist_LocalOutput_hadd.root hist_LocalOutput_hadd.root_old
hadd hist_LocalOutput_hadd.root hist_LocalOutputSummary_run*.root


# Make plots
#python3 Plotting.py hist_LocalOutput_hadd.root daysince
source makePlots.sh PED_plots
# 2023: Removed for beginning
#mv /eos/user/c/ccrovell/www/plots_archive/PED_plots/extrapolations/ExtrapolationFC_H* /eos/user/c/ccrovell/www/plots_archive/PED_plots/extrapolations_FC/
