dir=$1

mkdir ${dir}
python3 Plotting.py hist_LocalOutput_hadd.root daysince
mv hist_LocalOutput_hadd_daysince ${dir}/byDay
python3 Plotting.py hist_LocalOutput_hadd.root lumi
mv hist_LocalOutput_hadd_lumi ${dir}/byLumi
mkdir ${dir}/histograms
mv ${dir}/*/Ped* ${dir}/histograms
mv ${dir}/*/Ped* ${dir}/histograms

mv Table*txt /afs/cern.ch/user/d/dmroy/public/PED_tables/
mv ${dir}/byDay/* /eos/user/d/dmroy/www/plots_archive/PED_plots/byDay/
mv ${dir}/byLumi/* /eos/user/d/dmroy/www/plots_archive/PED_plots/byLumi/
mv ${dir}/histograms/* /eos/user/d/dmroy/www/plots_archive/PED_plots/histograms/
rm -r ${dir}
