// -*- C++ -*-
//
// Package:    HcalDPG/MyHcalAnlzr
// Class:      MyHcalAnlzr
//
/**\class MyHcalAnlzr MyHcalAnlzr.cc HcalDPG/MyHcalAnlzr/plugins/MyHcalAnlzr.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Long Wang
//         Created:  Wed, 05 Aug 2020 08:12:27 GMT
//
//

// system include files
#include <memory>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/one/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/Utilities/interface/EDGetToken.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/HcalDetId/interface/HcalSubdetector.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include "DataFormats/HcalRecHit/interface/CaloRecHitAuxSetter.h"
//#include "DataFormats/HcalRecHit/test/HcalRecHitDump.cc"
#include "DataFormats/HcalDigi/interface/HcalQIESample.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalCoderDb.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "TH1.h"
#include <TNtuple.h>
//
// class declaration
//

// If the analyzer does not use TFileService, please remove
// the template argument to the base class so the class inherits
// from  edm::one::EDAnalyzer<>
// This will improve performance in multithreaded jobs.

class MyHcalAnlzr : public edm::one::EDAnalyzer<edm::one::SharedResources> {
public:
  explicit MyHcalAnlzr(const edm::ParameterSet&);
  ~MyHcalAnlzr();

  static void fillDescriptions(edm::ConfigurationDescriptions& descriptions);

private:
  void beginJob() override;
  void analyze(const edm::Event&, const edm::EventSetup&) override;
  void endJob() override;

  // ----------member data ---------------------------
  //edm::EDGetTokenT<HBHERecHitCollection> HBHERecHitToken_;
  edm::EDGetTokenT<QIE11DigiCollection> qie11digisToken_;
  edm::EDGetTokenT<QIE10DigiCollection> qie10digisToken_;
  edm::EDGetTokenT<HODigiCollection> HOdigisToken_;
  //edm::EDGetTokenT<EEDigiCollection> eeDigiCollectionToken_;
  //edm::EDGetTokenT<EcalRecHitCollection> EERecHitCollectionT_;

  std::string runtype_;

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  edm::ESGetToken<SetupData, SetupRecord> setupToken_;
#endif
  edm::ESGetToken<HcalDbService, HcalDbRecord> hcalDbServiceToken_;
  //edm::ESGetToken<CaloGeometry, CaloGeometryRecord> caloGeomToken_;
  const HcalDbService* conditions;
  //const CaloGeometry* caloGeom_;

  //TNtuple* tup_rh;
  TNtuple* tup_qie;
  //TNtuple* tup_ecal;

  TTree* evttree;
  //std::vector<float> qieinfo;
  //std::vector<std::vector<float>> qielist;

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
MyHcalAnlzr::MyHcalAnlzr(const edm::ParameterSet& iConfig)
   :   //HBHERecHitToken_(consumes<HBHERecHitCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagRechit", edm::InputTag("hbheprereco")))),
       qie11digisToken_(consumes<QIE11DigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagQIE11", edm::InputTag("hcalDigis")))),
       qie10digisToken_(consumes<QIE10DigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagQIE10", edm::InputTag("hcalDigis")))),
       HOdigisToken_(consumes<HODigiCollection>(iConfig.getUntrackedParameter<edm::InputTag>("tagHO", edm::InputTag("hcalDigis")))),
       //eeDigiCollectionToken_(consumes<EEDigiCollection>(iConfig.getParameter<edm::InputTag>("EEdigiCollection"))),
       //EERecHitCollectionT_(consumes<EcalRecHitCollection>(iConfig.getParameter<edm::InputTag>("EERecHitCollection"))),
       runtype_(iConfig.getUntrackedParameter<std::string>("runtype"))
{
#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  setupDataToken_ = esConsumes<SetupData, SetupRecord>();
#endif
  hcalDbServiceToken_ = esConsumes<HcalDbService, HcalDbRecord>();
  //caloGeomToken_ = esConsumes<CaloGeometry, CaloGeometryRecord>();

  //now do what ever initialization is needed
  usesResource("TFileService");
  edm::Service<TFileService> fs;
  //tup_rh = fs->make<TNtuple>("rechit", "rechit", "RunNum:LumiNum:EvtNum:Energy:Time:TDC0:TDC1:TDC2:TDC3:TDC4:IEta:IPhi:Depth");
  tup_qie= fs->make<TNtuple>("qiedigi", "qiedigi", "RunNum:LumiNum:EvtNum:ieta:iphi:depth:sumADC:sumFC:type:shunt");
  //tup_ecal=fs->make<TNtuple>("eedigi", "eedigi", "ADC0:ADC1:ADC2:ADC3:ADC4:ADC5:ADC6:ADC7:ADC8:ADC9:e_eerec:ix:iy:iz");

  //evttree = fs->make<TTree>("evttree", "evttree");
  //evttree->Branch("qieinfo", &qieinfo);


}

MyHcalAnlzr::~MyHcalAnlzr() {
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  //
  // please remove this method altogether if it would be left empty
}

//
// member functions
//

// ------------ method called for each event  ------------
void MyHcalAnlzr::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  using namespace edm;

#ifdef THIS_IS_AN_EVENTSETUP_EXAMPLE
  // if the SetupData is always needed
  auto setup = iSetup.getData(setupToken_);
  // if need the ESHandle to check if the SetupData was there or not
  auto pSetup = iSetup.getHandle(setupToken_);
#endif
  conditions = &iSetup.getData(hcalDbServiceToken_);
  //caloGeom_ = &iSetup.getData(caloGeomToken_);

  //const CaloSubdetectorGeometry* geomEE = caloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalEndcap);
  //const CaloSubdetectorGeometry* geomHE = caloGeom_->getSubdetectorGeometry(DetId::Hcal, HcalEndcap);

  long runid   = iEvent.id().run();
  long eventid = iEvent.id().event();
  long lumiid  = iEvent.id().luminosityBlock();

  
  float shunt = -1;
  //if(runtype_=="Local"){
  //  if(eventid > 1000 && eventid < 2000) shunt = 6; // HBHE, Gsel1
  //  else if(eventid > 4000 && eventid < 5000) shunt = 1; // HBHE, Gsel0
  //  else return;
  //} else assert(runtype_=="Global");

  //edm::Handle<HBHERecHitCollection> hcalRecHits;
  edm::Handle<QIE11DigiCollection> qie11Digis;
  edm::Handle<QIE10DigiCollection> qie10Digis;
  edm::Handle<HODigiCollection> HODigis;
  //edm::Handle<EEDigiCollection> pEEDigis;
  //edm::Handle<EcalRecHitCollection> EERecHits;

  //bool gotRecHits = iEvent.getByToken(HBHERecHitToken_, hcalRecHits);
  bool gotQIE11Digis = iEvent.getByToken(qie11digisToken_, qie11Digis);
  bool gotQIE10Digis = iEvent.getByToken(qie10digisToken_, qie10Digis);
  bool gotHODigis = iEvent.getByToken(HOdigisToken_, HODigis);
  //bool gotEEDigis = iEvent.getByToken(eeDigiCollectionToken_, pEEDigis);
  //bool gotEERecHits = iEvent.getByToken(EERecHitCollectionT_, EERecHits);

  //if (!gotRecHits)
    //std::cout << "Could not find HCAL RecHits with tag: hbhereco" << std::endl;
  if (!gotQIE11Digis)
    std::cout << "Could not find HCAL QIE11Digis with tag: qie11Digis" << std::endl;
  if (!gotQIE10Digis)
    std::cout << "Could not find HCAL QIE10Digis with tag: qie10Digis" << std::endl;
  if (!gotHODigis)
    std::cout << "Could not find HCAL HODigis with tag: HODigis" << std::endl;
  //if (!gotEEDigis)
    //std::cout << "Could not find ECAL EEDigis with tag: EEdigiCollection" << std::endl;
  //if (!gotEERecHits)
    //std::cout << "Could not find ECAL EERecHits with tag: EERecHitCollection" << std::endl;


/////////////////////////////////
// Access ecal digi and rechits
/////////////////////////////////
  /*const EEDigiCollection* eeDigis = pEEDigis.product(); 
  for (EEDigiCollection::const_iterator it = eeDigis->begin(); it != eeDigis->end(); ++it) {
    const EEDataFrame digi = static_cast<const EEDataFrame>(*it);
    if(!( digi.id().zside()>0 && digi.id().ix()>=19 && digi.id().ix()<=22 && digi.id().iy()>=46 && digi.id().iy()<=50 )) continue;
    float E_ee = 0.;

    tup_ecal->Fill(digi[0].adc(), digi[1].adc(), digi[2].adc(), digi[3].adc(), digi[4].adc(), digi[5].adc(), digi[6].adc(), digi[7].adc(), digi[8].adc(), digi[9].adc(), E_ee, digi.id().ix(), digi.id().iy(), digi.id().zside());

  }
  for (auto const& h : (*EERecHits)) {
  }
  */

/////////////////////////////////
// Hcal rechits
/////////////////////////////////
  /*for (HBHERecHitCollection::const_iterator it = hcalRecHits->begin(); it != hcalRecHits->end(); ++it) {
    //	Explicit check on the DetIds present in the Collection
    HcalDetId did = it->id();
    if(did.subdet() != HcalEndcap) continue;

    unsigned rawTDCValues[5] = {0,};
    bool unpackedTDCData = false;

    const uint32_t auxTDC = it->auxTDC();
    if (auxTDC) {
      const unsigned six_bits_mask = 0x3f;
      for (unsigned ts = 0; ts < 5; ++ts)
        rawTDCValues[ts] = CaloRecHitAuxSetter::getField(auxTDC, six_bits_mask, ts * 6);
      unpackedTDCData = true;
    }

    if(!unpackedTDCData) std::cout << "Found rechit with unpackedTDCData" << std::endl;

    tup_rh->Fill(runid, lumiid, eventid, it->energy(), it->time(), rawTDCValues[0], rawTDCValues[1], rawTDCValues[2], rawTDCValues[3], rawTDCValues[4], did.ieta(), did.iphi(), did.depth());

  }
  */

/////////////////////////////////
// Hcal QIE11Digis
/////////////////////////////////
  if (runtype_=="Global" || (runtype_=="Local" && ((eventid > 1000 && eventid < 2000) || (eventid > 4000 && eventid < 5000)))){ // HBHE in combined health check
  if(eventid > 1000 && eventid < 2000) shunt = 6; // HBHE, Gsel1
  else if(eventid > 4000 && eventid < 5000) shunt = 1; // HBHE, Gsel0
  //qieinfo.clear();
  //qielist.clear();
  for (QIE11DigiCollection::const_iterator it = qie11Digis->begin(); it != qie11Digis->end(); ++it) {
    const QIE11DataFrame digi = static_cast<const QIE11DataFrame>(*it);

    //	Explicit check on the DetIds present in the Collection
    HcalDetId const& did = digi.detid();
    if(!(did.subdet() == HcalEndcap || did.subdet() == HcalBarrel)) continue;
    int type = conditions->getHcalSiPMParameter(did)->getType();

    const HcalQIECoder *channelCoder = conditions->getHcalCoder(did);
    const HcalQIEShape *shape = conditions->getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder, *shape);
    CaloSamples cs;
    coder.adc2fC(digi, cs);
    //CaloSamples cs = hcaldqm::utilities::loadADC2fCDB<QIE11DataFrame>(setup, did, *digi);

    //const HcalQIECoder* channelCoder = conditions -> getHcalCoder(did);
    //const HcalQIEShape* shape = conditions -> getHcalShape(channelCoder);
    //HcalCoderDb coder(*channelCoder,*shape);
    //CaloSamples cs; coder.adc2fC(digi,cs);
    //HcalCalibrations calibrations = conditions->getHcalCalibrations(did);

    float FC_=0;
    int ADC_=0;
    for(int i=0; i<digi.samples(); i++){
      ADC_ += digi[i].adc();
      FC_ += cs[i];
    }

    /*float sumE = 0.;
    for (auto const& h : (*EERecHits)) {
      if(!( EEDetId(h.detid()).zside()>0 && EEDetId(h.detid()).ix()>=19 && EEDetId(h.detid()).ix()<=22 && EEDetId(h.detid()).iy()>=46 && EEDetId(h.detid()).iy()<=50 )) continue;
      auto cellGeometry = geomEE->getGeometry(h.detid());
      const GlobalPoint& gp(cellGeometry->getPosition());
      HcalDetId closestCell(geomHE->getClosestCell(gp));
      if(closestCell.ieta()==did.ieta() && closestCell.iphi()==did.iphi()) {
        sumE += h.energy();
      }
    }*/

    float vars[10] = {(float) runid, (float) lumiid, (float) eventid, (float) did.ieta(), (float) did.iphi(), (float) did.depth(), (float) ADC_, (float) FC_, (float) type, shunt};

    tup_qie->Fill(vars);

    //qieinfo={};
    //evttree->Fill();
  }
  }


/////////////////////////////////
// Hcal QIE10Digis
/////////////////////////////////
  if (runtype_=="Global" || (runtype_=="Local" && (eventid > 6000 && eventid < 11000))){ // HF in combined health check
  shunt = 6; // HF, Gsel1
  //qie10info.clear();
  //qie10list.clear();
  for (QIE10DigiCollection::const_iterator it = qie10Digis->begin(); it != qie10Digis->end(); ++it) {
    const QIE10DataFrame digi10 = static_cast<const QIE10DataFrame>(*it);

    //  Explicit check on the DetIds present in the Collection
    HcalDetId const& did10 = digi10.detid();
    if(!(did10.subdet() == HcalForward)) continue;
    int type = conditions->getHcalSiPMParameter(did10)->getType();

    const HcalQIECoder *channelCoder = conditions->getHcalCoder(did10);
    const HcalQIEShape *shape = conditions->getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder, *shape);
    CaloSamples cs10;
    coder.adc2fC(digi10, cs10);

    int ADC_=0;
    float FC_=0;
    for(int i=0; i<digi10.samples(); i++){
      ADC_ += digi10[i].adc();
      FC_ += cs10[i];
    }


    float vars10[10] = {(float) runid, (float) lumiid, (float) eventid,(float) did10.ieta(), (float) did10.iphi(), (float) did10.depth(), (float) ADC_, (float) FC_, (float) type, shunt};

    tup_qie->Fill(vars10);

    //qieinfo={};
    //evttree->Fill();
  }
  }


/////////////////////////////////
// Hcal HODigis
/////////////////////////////////
  if (runtype_=="Global") { // No HO in combined health check
  //HOdigiinfo.clear();
  //HOdigilist.clear();
  for (HODigiCollection::const_iterator it = HODigis->begin(); it != HODigis->end(); ++it) {
    const HODataFrame HODigis = static_cast<const HODataFrame>(*it);

    //  Explicit check on the DetIds present in the Collection
    //HcalDetId didHO = HODigis.id();
    HcalDetId const& didHO = HODigis.id();
    if(!(didHO.subdet() == HcalOuter)) continue;
    int type = conditions->getHcalSiPMParameter(didHO)->getType();

    const HcalQIECoder *channelCoder = conditions->getHcalCoder(didHO);
    const HcalQIEShape *shape = conditions->getHcalShape(channelCoder);
    HcalCoderDb coder(*channelCoder, *shape);
    CaloSamples HOcs;
    coder.adc2fC(HODigis, HOcs);

    int ADC_=0;
    float FC_=0;
    for(int i=0; i<HODigis.size(); i++){
      ADC_ += HODigis[i].adc();
      FC_ += HOcs[i];
    }

    float varsHO[10] = {(float) runid, (float) lumiid, (float) eventid,(float) didHO.ieta(), (float) didHO.iphi(), (float) didHO.depth(), (float) ADC_, (float) FC_, (float) type, shunt};

    tup_qie->Fill(varsHO);

    //qieinfo={};
    //evttree->Fill();
  }
  }


}

// ------------ method called once each job just before starting event loop  ------------
void MyHcalAnlzr::beginJob() {
  // please remove this method if not needed
}

// ------------ method called once each job just after ending the event loop  ------------
void MyHcalAnlzr::endJob() {
  // please remove this method if not needed
}

// ------------ method fills 'descriptions' with the allowed parameters for the module  ------------
void MyHcalAnlzr::fillDescriptions(edm::ConfigurationDescriptions& descriptions) {
  //The following says we do not know what parameters are allowed so do no validation
  // Please change this to state exactly what you do use, even if it is no parameters
  edm::ParameterSetDescription desc;
  desc.setUnknown();
  descriptions.addDefault(desc);

  //Specify that only 'tracks' is allowed
  //To use, remove the default given above and uncomment below
  //ParameterSetDescription desc;
  //desc.addUntracked<edm::InputTag>("tracks","ctfWithMaterialTracks");
  //descriptions.addDefault(desc);
}

//define this as a plug-in
DEFINE_FWK_MODULE(MyHcalAnlzr);
