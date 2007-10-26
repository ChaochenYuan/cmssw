#ifndef TrackAssociatorByChi2_h
#define TrackAssociatorByChi2_h

/** \class TrackAssociatorByChi2
 *  Class that performs the association of reco::Tracks and TrackingParticles evaluating the chi2 of reco tracks parameters and sim tracks parameters. The cut can be tuned from the config file: see data/TrackAssociatorByChi2.cfi. Note that the Association Map is filled with -ch2 and not chi2 because it is ordered using std::greater: the track with the lowest association chi2 will be the first in the output map.It is possible to use only diagonal terms (associator by pulls) seeting onlyDiagonal = true in the PSet 
 *
 *  $Date: 2007/06/29 15:49:54 $
 *  $Revision: 1.16 $
 *  \author cerati, magni
 */

#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "MagneticField/Engine/interface/MagneticField.h" 
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "FWCore/Framework/interface/EventSetup.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include<map>

//Note that the Association Map is filled with -ch2 and not chi2 because it is ordered using std::greater:
//the track with the lowest association chi2 will be the first in the output map.

class TrackAssociatorByChi2 : public TrackAssociatorBase {

 public:
  typedef std::map<double,  SimTrack> Chi2SimMap;
  typedef std::pair< reco::Track, Chi2SimMap> RecoToSimPair;
  typedef std::vector< RecoToSimPair > RecoToSimPairAssociation;

  /// Constructor with PSet
  TrackAssociatorByChi2(const edm::ESHandle<MagneticField> mF, edm::ParameterSet conf):
    chi2cut(conf.getParameter<double>("chi2cut")),
    onlyDiagonal(conf.getParameter<bool>("onlyDiagonal")){
    theMF=mF;  
    if (onlyDiagonal)
      edm::LogInfo("TrackAssociator") << " ---- Using Off Diagonal Covariance Terms = 0 ---- " <<  "\n";
    else 
      edm::LogInfo("TrackAssociator") << " ---- Using Off Diagonal Covariance Terms != 0 ---- " <<  "\n";
  }

  /// Constructor with double and bool
  TrackAssociatorByChi2(const edm::ESHandle<MagneticField> mF, double chi2Cut, bool onlyDiag){
    chi2cut=chi2Cut;
    onlyDiagonal=onlyDiag;
    theMF=mF;  
  }

  /// Destructor
  ~TrackAssociatorByChi2(){}

  /// compare reco::TrackCollection and edm::SimTrackContainer iterators: returns the chi2
  double compareTracksParam(reco::TrackCollection::const_iterator, 
			    edm::SimTrackContainer::const_iterator, 
			    const HepLorentzVector, 
			    GlobalVector,
			     reco::TrackBase::CovarianceMatrix) const;

  /// compare collections reco to sim
  RecoToSimPairAssociation compareTracksParam(const reco::TrackCollection&, 
					      const edm::SimTrackContainer&, 
					      const edm::SimVertexContainer&) const;

  /// compare reco::TrackCollection and TrackingParticleCollection iterators: returns the chi2
  double associateRecoToSim(reco::TrackCollection::const_iterator,
			    TrackingParticleCollection::const_iterator) const;

  /// compare reco to sim the handle of reco::Track and TrackingParticle collections
  reco::RecoToSimCollection associateRecoToSim(edm::Handle<reco::TrackCollection>&, 
					       edm::Handle<TrackingParticleCollection>&, 
					       const edm::Event * event = 0) const;
  
  /// compare reco to sim the handle of reco::Track and TrackingParticle collections
  reco::SimToRecoCollection associateSimToReco(edm::Handle<reco::TrackCollection>&, 
					       edm::Handle<TrackingParticleCollection>& ,
					       const edm::Event * event = 0) const;
  
  /// propagate the track parameters of TrackinParticle from production vertex to the point of closest approach to the beam line. 
  reco::TrackBase::ParameterVector parametersAtClosestApproach(Basic3DVector<double>,// vertex
							       Basic3DVector<double>,// momAtVtx
							       float) const;// charge
 private:
  edm::ESHandle<MagneticField> theMF;
  double chi2cut;
  bool onlyDiagonal;
};

#endif
