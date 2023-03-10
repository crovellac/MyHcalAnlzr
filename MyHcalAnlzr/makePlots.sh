dir="PED_plots_2023" # $1

mkdir ${dir}
python3 Plotting.py hist_CalibOutput_hadd.root daysince
mv hist_CalibOutput_hadd_daysince ${dir}/byDay
#python3 Plotting.py hist_LocalOutput_hadd.root lumi
#mv hist_LocalOutput_hadd_lumi ${dir}/byLumi
mkdir ${dir}/histograms
mv ${dir}/byDay/Ped* ${dir}/histograms #rm ${dir}/byDay/Ped*
#mv ${dir}/byLumi/Ped* ${dir}/histograms
#mkdir ${dir}/extrapolations
##rm ${dir}/byDay/Ex*
#mv ${dir}/byLumi/Ex* ${dir}/extrapolations

mv Table*txt /afs/cern.ch/user/d/dmroy/public/PED_tables_2023/
mv ${dir}/byDay/* /eos/user/d/dmroy/www/plots_archive/PED_plots_2023/byDay/
#mv ${dir}/byLumi/* /eos/user/d/dmroy/www/plots_archive/PED_plots_2023/byLumi/
mv ${dir}/histograms/* /eos/user/d/dmroy/www/plots_archive/PED_plots_2023/histograms/
#mv ${dir}/extrapolations/* /eos/user/d/dmroy/www/plots_archive/PED_plots_2023/extrapolations/
rm -r ${dir}
