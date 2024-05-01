dir="PED_plots_2024" # $1

mkdir ${dir}
python3 Plotting.py hist_CalibOutput_hadd.root daysince
mv hist_CalibOutput_hadd_daysince ${dir}/byDay
python3 Plotting.py hist_CalibOutput_hadd.root lumi
mv hist_CalibOutput_hadd_lumi ${dir}/byLumi
mkdir ${dir}/histograms
rm ${dir}/byDay/Ped*
mv ${dir}/byLumi/Ped* ${dir}/histograms
#mkdir ${dir}/extrapolations
##rm ${dir}/byDay/Ex*
#mv ${dir}/byLumi/Ex* ${dir}/extrapolations

mv Table*txt /afs/cern.ch/user/c/ccrovell/public/PED_tables_2024/
mv PedestalTable*txt /afs/cern.ch/user/c/ccrovell/public/PED_DPGcalibration/
mv ${dir}/byDay/* /eos/user/c/ccrovell/www/plots_archive/PED_plots_2024/byDay/
mv ${dir}/byLumi/* /eos/user/c/ccrovell/www/plots_archive/PED_plots_2024/byLumi/
mv ${dir}/histograms/* /eos/user/c/ccrovell/www/plots_archive/PED_plots_2024/histograms/
#mv ${dir}/extrapolations/* /eos/user/c/ccrovell/www/plots_archive/PED_plots_2024/extrapolations/
rm -r ${dir}
