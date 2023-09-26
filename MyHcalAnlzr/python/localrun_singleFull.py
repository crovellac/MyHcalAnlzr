import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing

from Configuration.StandardSequences.Eras import eras
process = cms.Process('TDC',eras.Run3)

process.load("FWCore.MessageService.MessageLogger_cfi")
process.load('Configuration.StandardSequences.Services_cff')
process.maxEvents = cms.untracked.PSet( input = cms.untracked.int32(-1) )
process.MessageLogger.cerr.FwkReport.reportEvery = cms.untracked.int32(1000)

process.load('Configuration.EventContent.EventContent_cff')
process.load("Configuration.StandardSequences.GeometryDB_cff")
process.load("Configuration.StandardSequences.MagneticField_cff")
process.load("Configuration.StandardSequences.RawToDigi_Data_cff")
#process.load("EventFilter.HcalRawToDigi.HcalRawToDigi_cfi")
process.load("Configuration.StandardSequences.Reconstruction_cff")
process.load("RecoLocalCalo.Configuration.hcalLocalReco_cff")
process.load('Configuration.StandardSequences.EndOfProcess_cff')
#process.load("RecoLuminosity.LumiProducer.bunchSpacingProducer_cfi")

process.load("RecoLocalTracker.SiPixelRecHits.SiPixelRecHits_cfi")

RUN = "XXXXXX"

process.source = cms.Source("HcalTBSource",
	firstLuminosityBlockForEachRun = cms.untracked.VLuminosityBlockID([]),
	fileNames = cms.untracked.vstring(
		#"file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run358080/USC_358080.root",
		'file:/eos/cms/store/group/dpg_hcal/comm_hcal/USC/run' + RUN + '/USC_' + RUN + '.root'
        )
)

process.options = cms.untracked.PSet(
#	SkipEvent = cms.untracked.vstring('ProductNotFound')
)

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
from Configuration.AlCa.autoCond import autoCond
process.GlobalTag.globaltag = autoCond['run3_hlt']

#Setup FWK for multithreaded
process.options.numberOfThreads=cms.untracked.uint32(8)
process.options.numberOfStreams=cms.untracked.uint32(0)
process.options.numberOfConcurrentLuminosityBlocks=cms.untracked.uint32(1)


process.MyHcalAnlzr = cms.EDAnalyzer('MyHcalAnlzr',
#        tagRecHit = cms.untracked.InputTag("hbheprereco"),
#	tagQIE11 = cms.InputTag("simHcalDigis", "HBHEQIE11DigiCollection")
	tagQIE11 = cms.untracked.InputTag("hcalDigis"),
#        EEdigiCollection = cms.InputTag("ecalDigis","eeDigis"),
#	EERecHitCollection = cms.InputTag("ecalRecHit","EcalRecHitsEE")
        runtype = cms.untracked.string("Local")
)

process.TFileService = cms.Service("TFileService",
      fileName = cms.string("/eos/user/c/ccrovell/HCAL/MyHcalAnlzr/output_LocalRuns_Full_Run"+RUN+".root"),
      closeFileFast = cms.untracked.bool(True)
)

#process.ecalRecoSequence = cms.Sequence((process.ecalMultiFitUncalibRecHit+process.ecalDetIdToBeRecovered+process.ecalRecHit))

process.tbunpacker = cms.EDProducer(
        "HcalTBObjectUnpacker",
        IncludeUnmatchedHits    = cms.untracked.bool(False),
        HcalTriggerFED                  = cms.untracked.int32(1)
)
process.tbunpacker.fedRawDataCollectionTag = cms.InputTag("source")
process.hcalDigis.InputLabel = cms.InputTag("source")


process.p = cms.Path(
#	process.tbunpacker*
#	process.ecalDigis*
#	process.ecalRecoSequence*
        process.hcalDigis*
#	process.hcalLocalRecoSequence*
	process.MyHcalAnlzr
)

#process.outpath = cms.EndPath(process.out)
