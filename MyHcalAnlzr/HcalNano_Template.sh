cmsDriver.py NANO \
    -s RAW2DIGI,RECO,USER:DPGAnalysis/HcalNanoAOD/hcalNano_cff.hcalNanoTask \
    --processName=PFG \
    --datatier NANOAOD \
    --eventcontent NANOAOD \
    --filein root://eoscms.cern.ch//store/group/dpg_hcal/comm_hcal/PFG/Commissioning2023/TestEnablesEcalHcal/RAW/Express-v1/000/363/427/00000/516760f5-86e8-472b-acd9-f186012f4bee.root \
    --fileout /eos/user/d/dmroy/HCAL/MyHcalAnlzr_Nano/output_CalibRuns_Nano_Run363427.root \
    -n 5000 \
    --nThreads 4 \
    --conditions auto:run3_data_prompt \
    --era Run3 \
    --python_filename cmsdriver_cfg_MWGR2.py \
    --no_exec \
    --customise DPGAnalysis/HcalNanoAOD/customiseHcalCalib_cff.customiseHcalCalib
cmsRun cmsdriver_cfg_MWGR2.py
