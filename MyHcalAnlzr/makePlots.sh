dir=$1

mkdir ${dir}
python3 Plotting.py hist_LocalOutput_hadd.root daysince
mv hist_LocalOutput_hadd_daysince ${dir}/byDay
python3 Plotting.py hist_LocalOutput_hadd.root lumi
mv hist_LocalOutput_hadd_lumi ${dir}/byLumi
mkdir ${dir}/histograms
rm ${dir}/byDay/Ped*
mv ${dir}/byLumi/Ped* ${dir}/histograms
mkdir ${dir}/extrapolations
mv ${dir}/byLumi/Ex* ${dir}/extrapolations
mkdir ${dir}/extrapolations_FC2ADC
mv ${dir}/extrapolations/*FC2ADC* ${dir}/extrapolations_FC2ADC

mv Table*txt /afs/cern.ch/user/c/ccrovell/public/PED_tables/
mv ${dir}/byDay/* /eos/user/c/ccrovell/www/plots_archive/PED_plots/byDay/
mv ${dir}/byLumi/* /eos/user/c/ccrovell/www/plots_archive/PED_plots/byLumi/
mv ${dir}/histograms/* /eos/user/c/ccrovell/www/plots_archive/PED_plots/histograms/
mv ${dir}/extrapolations/* /eos/user/c/ccrovell/www/plots_archive/PED_plots/extrapolations/
mv ${dir}/extrapolations_FC2ADC/* /eos/user/c/ccrovell/www/plots_archive/PED_plots/extrapolations_FC2ADC/
rm -r ${dir}
