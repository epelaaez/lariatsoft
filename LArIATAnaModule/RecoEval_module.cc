//////////////////////////////////////////////////////////////////////////
// Class:       RecoEval
// Module Type: analyzer
// File:        RecoEval_module.cc
//
// Written by Emilio Peláez, created on January 2025. Adapted from XSAnalysis module
// and RecoEval module by Matt King.
////////////////////////////////////////////////////////////////////////

// ########################
// ### LArSoft includes ###
// ########################
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"

// #######################
// ### LArIAT includes ###
// #######################
#include "Utilities/DatabaseUtilityT1034.h"
#include "LArIATRecoAlg/TriggerFilterAlg.h"
#include "LArIATDataProducts/WCTrack.h"

// ##########################
// ### Framework includes ###
// ##########################
#include "canvas/Utilities/InputTag.h"
#include "art/Framework/Core/EDAnalyzer.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOneP.h" 
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
//#include "art/Framework/Services/Optional/TFileService.h" 
//#include "art/Framework/Services/Optional/TFileDirectory.h"
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "canvas/Utilities/InputTag.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "cetlib/maybe_ref.h"
#include "fhiclcpp/ParameterSet.h" 

// ########################
// ### LArSoft includes ###
// ########################
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" 
#include "larcore/Geometry/Geometry.h"
#include "larcorealg/Geometry/CryostatGeo.h"
#include "larcorealg/Geometry/TPCGeo.h"
#include "larcorealg/Geometry/PlaneGeo.h"
#include "larcorealg/Geometry/WireGeo.h"
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/Utilities/AssociationUtil.h"
#include "lardataobj/RawData/ExternalTrigger.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"
#include "larsim/MCCheater/BackTracker.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "larevt/Filters/ChannelFilter.h"
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/AnalysisBase/ParticleID.h"
#include "larreco/RecoAlg/TrackMomentumCalculator.h"
#include "LArIATDataProducts/WCTrack.h"
#include "RawDataUtilities/TriggerDigitUtility.h"

#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "LArIATDataProducts/WCTrack.h"
#include "canvas/Persistency/Common/FindOneP.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h"
#include "cetlib/maybe_ref.h" 
//#include "art/Utilities/InputTag.h"
#include "fhiclcpp/ParameterSet.h"
#include "messagefacility/MessageLogger/MessageLogger.h"
#include "lardataobj/RecoBase/Track.h"
#include <memory>
#include "lardataobj/RecoBase/PFParticle.h"
#include "larsim/MCCheater/ParticleInventoryService.h"


// #####################
// ### ROOT includes ###
// #####################
#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TTree.h>

// ####################
// ### C++ includes ###
// ####################
#include <map>
#include <memory>
#include <fstream>
#include "math.h"
#include <algorithm>

// ##########################
// ### Framework includes ###
// ##########################
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h" 
#include "art/Framework/Principal/Event.h" 
#include "fhiclcpp/ParameterSet.h" 
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Principal/Handle.h" 
#include "canvas/Persistency/Common/Ptr.h" 
#include "canvas/Persistency/Common/PtrVector.h" 
#include "art/Framework/Services/Registry/ServiceHandle.h" 
#include "canvas/Persistency/Common/FindOneP.h" 
#include "canvas/Persistency/Common/FindManyP.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 
//#include "cetlib/maybe_ref.h"

// ########################
// ### LArSoft includes ###
// ########################
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "larcore/Geometry/Geometry.h"
#include "larcorealg/Geometry/CryostatGeo.h"
#include "larcorealg/Geometry/TPCGeo.h"
#include "larcorealg/Geometry/PlaneGeo.h"
#include "larcorealg/Geometry/WireGeo.h"
#include "lardataobj/RecoBase/Wire.h"
#include "lardataobj/RecoBase/Hit.h"
#include "lardataobj/RecoBase/Cluster.h"
#include "lardataobj/RecoBase/Track.h"
#include "lardataobj/RecoBase/TrackHitMeta.h"
#include "lardataobj/RecoBase/Vertex.h"
#include "lardataobj/RecoBase/SpacePoint.h"
#include "lardata/ArtDataHelper/TrackUtils.h" // lar::util::TrackPitchInView()
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/Utilities/AssociationUtil.h"

//#include "RawData/ExternalTrigger.h"
#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"
#include "larsim/MCCheater/BackTrackerService.h"
#include "larsim/MCCheater/ParticleInventoryService.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "larevt/Filters/ChannelFilter.h"
#include "lardataobj/AnalysisBase/Calorimetry.h"
#include "lardataobj/AnalysisBase/ParticleID.h"
#include "larreco/RecoAlg/TrackMomentumCalculator.h"
#include "LArIATDataProducts/WCTrack.h"
#include "LArIATDataProducts/TOF.h"
#include "LArIATDataProducts/AGCounter.h"
#include "RawDataUtilities/TriggerDigitUtility.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/EndPoint2D.h"
#include "lardataobj/MCBase/MCShower.h"
#include "lardataobj/MCBase/MCStep.h"
#include "larreco/Calorimetry/CalorimetryAlg.h"

// #####################
// ### ROOT includes ###
// #####################
#include "TComplex.h"
#include "TFile.h"
#include "TH2D.h"
#include "TF1.h"
#include "TTree.h"
#include "TTimeStamp.h"

// Type definitions
typedef std::map<int, art::Ptr<simb::MCParticle>> ParticleMap;

class RecoEval : public art::EDAnalyzer {
    public: 
        explicit RecoEval(fhicl::ParameterSet const &p);
        RecoEval(RecoEval const &) = delete;
        RecoEval(RecoEval &&) = delete;
        RecoEval & operator = (RecoEval const &) = delete;
        RecoEval & operator = (RecoEval &&) = delete;

        // Required functions
        void analyze(art::Event const &e) override;

        // Optional functions
        void beginJob() override;
        void endJob() override;
        void reconfigure(fhicl::ParameterSet const &p);

        // Helper functions
        void resetTree();
        unsigned int lastPointInTPC(simb::MCParticle *track);
        unsigned int lastPointInTPC(const art::Ptr<simb::MCParticle> track);
        unsigned int firstPointInTPC(simb::MCParticle *track);
        unsigned int firstPointInTPC(const art::Ptr<simb::MCParticle> track);

        void fillPionTruthData(simb::MCParticle *pion);
        void fillProtonDaughtersTruthData(std::vector<simb::MCParticle*> daughterProtons);

        bool isPosterityOfPrimary(simb::MCParticle *particle, const sim::ParticleList& plist);
        double trackMagnitude(simb::MCParticle *track);
        double trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2);
        double trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2);
        bool isWithinActiveVolume(double x, double y, double z);

    private: 
        // Produce's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string simulation_producer_label_;
        std::string recotrackmcparticlematching_label_;

        // fcl parameters
        bool bVerbose;
        unsigned int MeanDEDXNumberTrajPoints;
        double TrackStitchingThreshold;

        // Output tree
        TTree *RecoEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;

        int pionTrackId;
        int numDaughterProtons = 0;
        int numDaughterNeutrons = 0;

        // Truth-level data
        double truthPionInitialEnergy;
        double truthPionInitialKEnergy;
        double truthPionInitialMomentum;

        double truthPionVertexEnergy;
        double truthPionVertexKEnergy;
        double truthPionVertexMomentum;

        double truthPionVertexPx;
        double truthPionVertexPy;
        double truthPionVertexPz;

        std::vector<double> truthProtonsEnergy;
        std::vector<double> truthProtonsKEnergy;
        std::vector<double> truthProtonsInitialMomentum;
        std::vector<double> truthProtonsInitialPx;
        std::vector<double> truthProtonsInitialPy;
        std::vector<double> truthProtonsInitialPz;

        // WC variables
        int WC2TPCtrkID;
        double WCTrackMomentum;
        double WC2TPCPionBeginX;
        double WC2TPCPionBeginY;
        double WC2TPCPionBeginZ;
        double WC3PionX;
        double WC3PionY;
        double WC3PionZ;
        double WC4PionX;
        double WC4PionY;
        double WC4PionZ;
        double WCTheta;
        double WCPhi;

        // Reco variables
        std::vector<bool> isTrackInverted;
        std::vector<double> recoBeginX;
        std::vector<double> recoBeginY;
        std::vector<double> recoBeginZ;
        std::vector<double> recoEndX;
        std::vector<double> recoEndY;
        std::vector<double> recoEndZ;
        std::vector<int> recoTrkID;

        // Truth variables for particles matched to tracks
        std::vector<int>    matchedIdentity;
        std::vector<double> matchedCleanliness;
        std::vector<double> matchedCompleteness;
        std::vector<int>    matchedTrkID;
        std::vector<std::string> matchedProcess;

        std::vector<double> matchedBeginX;
        std::vector<double> matchedBeginY;
        std::vector<double> matchedBeginZ;
        std::vector<double> matchedEndX;
        std::vector<double> matchedEndY;
        std::vector<double> matchedEndZ;
        std::vector<double> matchedLength;

        std::vector<double> matchedKEnergy;
        std::vector<double> matchedEndingKEnergy;

        std::vector<double> matchedInitialPx;
        std::vector<double> matchedInitialPy;
        std::vector<double> matchedInitialPz;
        std::vector<double> matchedEndingPx;
        std::vector<double> matchedEndingPy;
        std::vector<double> matchedEndingPz;
        
        std::vector<bool>   matchedIsPosterityProton;

        // Calorimetry variables for tracks
        std::vector<std::vector<double>> recoPitch;
        std::vector<std::vector<double>> recoDEDX;
        std::vector<std::vector<double>> recoEDep;
        std::vector<std::vector<double>> recoResR;
        std::vector<std::vector<double>> recoZPos;
        std::vector<double> recoMeanDEDX;

        // Pion reconstruction
        bool isPrimaryPionReco  = false;
        bool hasBrokenPionTrack = false;
        std::vector<double> distancePrimaryPionOtherPion;

        // Masses
        const double PionMass    = .13957018;    // in GeV
        const double ProtonMass  = .93827208816; // in GeV
        const double NeutronMass = .9395654133;  // in GeV

        // Detector dimensions
        const double minX =  0.0;
        const double maxX = 47.0;
        const double minY =-20.0; 
        const double maxY = 20.0; 
        const double minZ =  3.0;
        const double maxZ = 87.0;
        
        // Fiducial volume for interactions
        const double RminX =  5.0;
        const double RmaxX = 42.0;
        const double RminY =-15.0; 
        const double RmaxY = 15.0; 
        const double RminZ =  8.0;
        const double RmaxZ = 82.0;
};

RecoEval::RecoEval(fhicl::ParameterSet const &p) : EDAnalyzer(p) {
    this->reconfigure(p);
}

void RecoEval::analyze(art::Event const &e) {
    resetTree();

    run = e.run(); subrun = e.subRun(); event = e.event();
    if (bVerbose) std::cout << "Run: " << run << ", subrun: " << subrun << ", event: " << event << std::endl;
    if (bVerbose) std::cout << std::endl;

    //////////////////////
    // Wire chamber tracks
    //////////////////////

    if (bVerbose) std::cout << "Getting wire chamber track information" << std::endl;

    art::Handle<std::vector<ldp::WCTrack>> wctrackHandle;
    std::vector<art::Ptr<ldp::WCTrack>>    wctrack;
    // If there is no wire chamber tracks for right label, return
    if (!e.getByLabel(strWCTrackBuilderLabel, wctrackHandle)) return;
    art::fill_ptr_vector(wctrack, wctrackHandle);

    int numWCtrks = wctrack.size(); // number of wire chamber tracks
    if (numWCtrks != 1) return; // TODO: needed?

    // Get wcTrack momentum
    WCTrackMomentum = wctrack[0]->Momentum() * 0.001; // Mev to GeV
    WC3PionX = wctrack[0]->HitPosition(2,0);
    WC3PionY = wctrack[0]->HitPosition(2,1);
    WC3PionZ = wctrack[0]->HitPosition(2,2);
    WC4PionX = wctrack[0]->HitPosition(3,0);
    WC4PionY = wctrack[0]->HitPosition(3,1);
    WC4PionZ = wctrack[0]->HitPosition(3,2);
    WCTheta  = wctrack[0]->Theta();
    WCPhi    = wctrack[0]->Phi();

    if (bVerbose) std::cout << "WCTrackMomentum: " << WCTrackMomentum << std::endl;
    if (bVerbose) std::cout << std::endl;

    /////////////
    // TPC tracks
    /////////////

    if (bVerbose) std::cout << "Getting TPC track information" << std::endl;

    art::Handle<std::vector<recob::Track>> tpcTrackHandle;
    std::vector<art::Ptr<recob::Track>> tracklist;
    // If there are no tpc chamber tracks for the label, return
    if (!e.getByLabel(strTPCTrackHandleLabel, tpcTrackHandle)) return; 
    art::fill_ptr_vector(tracklist, tpcTrackHandle);

    int numTracksReco = tracklist.size();
    if (!numTracksReco) return; // If no TPC tracks, return
    
    if (bVerbose) std::cout << "Number of TPC reco tracks: " << numTracksReco << std::endl;
    if (bVerbose) std::cout << std::endl;

    //////////////////
    // WC2TPC Matching
    //////////////////

    art::FindOneP<recob::Track> fWC2TPC(wctrackHandle, e, strWC2TPCModuleLabel);

    if (fWC2TPC.isValid()) {
        if (bVerbose) std::cout << "Wire chamber to TPC is valid" << std::endl;
        for (unsigned int i = 0; i < fWC2TPC.size(); ++i) {
            cet::maybe_ref<recob::Track const> trackWC2TPC(*fWC2TPC.at(i));
            if (!trackWC2TPC) continue;
            recob::Track const& aTrack(trackWC2TPC.ref());
            WC2TPCtrkID = aTrack.ID();

            if (bVerbose) std::cout << "Wire chamber to TPC track id: " << WC2TPCtrkID << std::endl;
            if (bVerbose) std::cout << std::endl;

            auto recoWC2TPCBeginning = aTrack.Start();

            if ((aTrack.Start()).Z() < (aTrack.End()).Z()) {
                recoWC2TPCBeginning = aTrack.Start();
            } else {
                recoWC2TPCBeginning = aTrack.End();
            }

            WC2TPCPionBeginX = recoWC2TPCBeginning.X();
            WC2TPCPionBeginY = recoWC2TPCBeginning.Y();
            WC2TPCPionBeginZ = recoWC2TPCBeginning.Z();

        } // end trackWC2TPC loop
    } // end if fWC2TPC.isValid()

    /////////////////////
    // MC particle tracks
    /////////////////////

    // Geometry service
    art::ServiceHandle<geo::Geometry> geom;
    // Get the backtracer to recover true quantities
    art::ServiceHandle<cheat::BackTrackerService> bt;
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();

    // Pointer to assign pion to
    simb::MCParticle *primaryPion = NULL;
    std::vector<simb::MCParticle*> daughterProtons; 
    std::vector<int>               daughterProtonsTrackIds;
    std::vector<simb::MCParticle*> daughterNeutrons; 

    // Loop over all Geant4 particles using the BackTracker
    for (size_t p = 0; p < plist.size(); ++p) {
        auto mcPart = plist.Particle(p);

        // Get true particle
        std::string particleProcess = mcPart->Process();
        if (
            !(particleProcess.find("primary") != std::string::npos) && // If particle is not primary
            !(mcPart->PdgCode() == -211) // Check pdg code indicates negative pion
        ) continue;

        primaryPion = mcPart; // assign primary pion pointer
        pionTrackId = primaryPion->TrackId();
        std::cout << "Found primary pion!" << std::endl;
        if (bVerbose) std::cout << std::endl;

        // Get pion daughters
        int numPionDaughters = primaryPion->NumberDaughters();
        for (int iDaughter = 0; iDaughter < numPionDaughters; ++iDaughter) {
            int thisDaughterTrackId = primaryPion->Daughter(iDaughter);
            simb::MCParticle *thisDaughter = NULL;

            // Grab daughter with thisDaughterTrackId
            for (size_t q = 0; q < plist.size(); ++q) {
                if (plist.Particle(q)->TrackId() == thisDaughterTrackId) {
                    thisDaughter = plist.Particle(q);
                    break;
                }
            }

            if (thisDaughter->Process() == "pi-Inelastic") {
                if (thisDaughter->PdgCode() == 2112) {
                    numDaughterNeutrons++; 
                    daughterNeutrons.push_back(thisDaughter);
                }
                if (thisDaughter->PdgCode() == 2212) {
                    numDaughterProtons++; 
                    daughterProtons.push_back(thisDaughter);
                    daughterProtonsTrackIds.push_back(thisDaughter->TrackId());
                }
            }
        } // end pion daughters loop
        break; // Break once we find primary pion
    } // end Geant4 particle loop

    // If for some reason we did not find the pion, flag and return
    if (primaryPion == NULL) {
        std::cout << "WARNING: primary pion not found, aborting event" << std::endl;
        return;
    }

    if (bVerbose) std::cout << "Number of protons before looking for posterity protons: " << daughterProtons.size() << std::endl;

    // Get posterity protons
    for (size_t p = 0; p < plist.size(); ++p) {
        auto mcPart = plist.Particle(p);
        int partTrackId = mcPart->TrackId();

        // If track id is of proton we are already tracking or primary pion, continue
        // Also continue if particle is not a proton
        if (
            (std::find(daughterProtonsTrackIds.begin(), daughterProtonsTrackIds.end(), partTrackId) != daughterProtonsTrackIds.end()) ||
            (partTrackId == pionTrackId) ||
            (mcPart->PdgCode() != 2212)
        ) continue;

        // Check if proton is in TPC
        unsigned int protonTrackBegin = firstPointInTPC(mcPart);
        unsigned int protonTrackEnd   = lastPointInTPC(mcPart);
        if (protonTrackBegin == 9999 || protonTrackEnd == 9999) continue;

        // Check if proton is in pion's family tree
        if (isPosterityOfPrimary(mcPart, plist)) {
            if (bVerbose) std::cout << "Found posterity proton with track id: " << partTrackId << std::endl;

            // numDaughterProtons++; // Would this double count protons?
            daughterProtons.push_back(mcPart);
            daughterProtonsTrackIds.push_back(partTrackId);
        }
    }

    if (bVerbose) std::cout << "Number of protons after looking for posterity protons: " << daughterProtons.size() << std::endl;
    if (bVerbose) std::cout << std::endl;

    // Get relevant pion truth information
    fillPionTruthData(primaryPion); 
    fillProtonDaughtersTruthData(daughterProtons);

    // Ignore pions not in TPC, should not be needed since in our pion 
    // Redundant since we check containment in fiducial volume in signal
    // definition, but good sanity check anyways
    unsigned int trackBeginIndex = firstPointInTPC(primaryPion);
    unsigned int trackEndIndex   = lastPointInTPC(primaryPion);
    if (trackBeginIndex == 9999 || trackEndIndex == 9999) return;

    ////////////////////////////////////
    // Truth matching using Johnny's Alg
    ////////////////////////////////////

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

    // Get simulated particles
    auto particle_handle = e.getValidHandle<std::vector<simb::MCParticle>>(simulation_producer_label_);
    std::vector<art::Ptr<simb::MCParticle>> particle_vector;
    art::fill_ptr_vector(particle_vector, particle_handle);

    // Initialize particle map (typedef at top) and fill it
    ParticleMap particle_map;
    for (auto const& particle : particle_vector) {
        particle_map[particle->TrackId()] = particle;
    }

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

    // Useful to order protons based on furthest-right pion track later on
    double maxPionEndZ    = -9999;
    double pionRecoEndX   = -9999;
    double pionRecoEndY   = -9999;
    double pionRecoEndZ   = -9999;
    int primaryPionRecoTrackIndex = -9999;

    // Do track matching
    for (int particleSpeciesIndex = 0; particleSpeciesIndex <= 2; particleSpeciesIndex++) {
        // This first loop goes over the types of particle that we want to match in order of
        // their priority: primary pion, protons, everything else

        for (size_t trkIdx = 0; trkIdx < tpcTrackHandle->size(); ++trkIdx) {
            // Now, we loop over the TPC tracks

            // Get MCParticle objects and metadata
            std::vector<art::Ptr<simb::MCParticle>> const& particles = find_many_mcparticles_from_tracks.at(trkIdx);
            std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trkIdx);

            // If reco track is not matched to anything, continue
            if (btdata_vector.size() == 0) continue;

            // Get MCParticle object and data
            auto const& particle = particles.front();
            int const pdg_code   = particle->PdgCode();
            int const g4_trk_id  = particle->TrackId();
            std::string process  = particle->Process();

            // Get TPC track
            auto thisTrack = tracklist.at(trkIdx);

            // Matching priority
            if ((pdg_code != -211) && (particleSpeciesIndex == 0)) continue;
            if ((pdg_code != 2212) && (particleSpeciesIndex == 1)) continue;
            if (((pdg_code == -211) || (pdg_code == 2212)) && (particleSpeciesIndex == 2)) continue;

            // Get cleanliness/completeness of track from metadata
            double const cleanliness  = btdata_vector.front()->cleanliness;
            double const completeness = btdata_vector.front()->completeness;

            // At this point, we have:
            //     particle: matched MCParticle
            //     thisTrack: reconstructed track

            recob::TrackTrajectory::Point_t recoBeginning;
            recob::TrackTrajectory::Point_t recoEnd;

            bool isThisTrackReversed = false;

            // Check that interactino is inside the TPC
            auto partTrackBegin = firstPointInTPC(particle);
            auto partTrackEnd   = lastPointInTPC(particle);
            if (partTrackBegin == 9999 || partTrackEnd == 9999) continue;

            // Pion matching and ordering
            if (particleSpeciesIndex == 0) {
                // Check if track is reversed
                if ((thisTrack->Start()).Z() < (thisTrack->End()).Z()) {
                    recoBeginning = thisTrack->Start();
                    recoEnd       = thisTrack->End();
                } else {
                    recoEnd       = thisTrack->Start();
                    recoBeginning = thisTrack->End();
                    isThisTrackReversed = true;
                }

                if (recoEnd.Z() > maxPionEndZ) {
                    maxPionEndZ    = recoEnd.Z();
                    pionRecoEndX   = recoEnd.X();
                    pionRecoEndY   = recoEnd.Y();
                    pionRecoEndZ   = recoEnd.Z();
                }
            } // end pion matching

            // Proton matching and ordering
            if (particleSpeciesIndex == 1) {
                if (sqrt(
                    pow((thisTrack->Start()).X() - pionRecoEndX, 2) + 
                    pow((thisTrack->Start()).Y() - pionRecoEndY, 2) + 
                    pow((thisTrack->Start()).Z() - pionRecoEndZ, 2)
                ) < sqrt(
                    pow((thisTrack->End()).X() - pionRecoEndX, 2) + 
                    pow((thisTrack->End()).Y() - pionRecoEndY, 2) + 
                    pow((thisTrack->End()).Z() - pionRecoEndZ, 2)
                )) {
                    isThisTrackReversed = false;
                    recoBeginning       = thisTrack->Start();
                    recoEnd             = thisTrack->End();
                } else {
                    isThisTrackReversed = true;
                    recoBeginning       = thisTrack->End();
                    recoEnd             = thisTrack->Start();
                }
            } // end proton matching

            isTrackInverted.push_back(isThisTrackReversed);

            // Check if track is primary pion
            if (
                (thisTrack->ID() == WC2TPCtrkID) &&
                (pdg_code == -211) &&
                (process == "primary") &&
                particleSpeciesIndex == 0
            ) {
                isPrimaryPionReco = true;
                // Set index of primary pion to current size of vector before 
                // adding it to the vector
                primaryPionRecoTrackIndex = (int) recoBeginX.size();
            }

            // Fill in data to vectors for tree
            recoBeginX.push_back(recoBeginning.X());
            recoBeginY.push_back(recoBeginning.Y());
            recoBeginZ.push_back(recoBeginning.Z());
            recoEndX.push_back(recoEnd.X());
            recoEndY.push_back(recoEnd.Y());
            recoEndZ.push_back(recoEnd.Z());
            recoTrkID.push_back(thisTrack->ID());

            matchedIdentity.push_back(pdg_code);
            matchedBeginX.push_back(particle->Vx(partTrackBegin));
            matchedBeginY.push_back(particle->Vy(partTrackBegin));
            matchedBeginZ.push_back(particle->Vz(partTrackBegin));
            matchedEndX.push_back(particle->Vx(partTrackEnd));
            matchedEndY.push_back(particle->Vy(partTrackEnd));
            matchedEndZ.push_back(particle->Vz(partTrackEnd));
            matchedLength.push_back(trackMagnitude(particle, partTrackBegin, partTrackEnd));

            // If particle ends in TPC, best end is two indices before actual last point
            auto bestEnd = std::min(partTrackEnd, particle->NumberTrajectoryPoints() - 2);

            // Get energies depending on particle type
            if (pdg_code == -211) {
                matchedKEnergy.push_back(particle->E(partTrackBegin) - PionMass);
                matchedEndingKEnergy.push_back(particle->E(bestEnd) - PionMass);
            } else if (pdg_code == 2212) {
                matchedKEnergy.push_back(particle->E(partTrackBegin) - ProtonMass);
                matchedEndingKEnergy.push_back(particle->E(bestEnd) - ProtonMass);
            } else {
                matchedKEnergy.push_back(particle->E(partTrackBegin) - particle->Mass());
                matchedEndingKEnergy.push_back(particle->E(bestEnd) - particle->Mass());
            }

            matchedInitialPx.push_back(particle->Px(partTrackBegin));
            matchedInitialPy.push_back(particle->Py(partTrackBegin));
            matchedInitialPz.push_back(particle->Pz(partTrackBegin));
            matchedEndingPx.push_back(particle->Px(bestEnd));
            matchedEndingPy.push_back(particle->Py(bestEnd));
            matchedEndingPz.push_back(particle->Pz(bestEnd));

            matchedCompleteness.push_back(completeness);
            matchedCleanliness.push_back(cleanliness);
            matchedTrkID.push_back(g4_trk_id);
            matchedProcess.push_back(process);

            // Tag track if it matches to posterity proton
            bool isThisPosterityProton = false;
            if(std::find(daughterProtonsTrackIds.begin(), daughterProtonsTrackIds.end(), g4_trk_id) != daughterProtonsTrackIds.end()) isThisPosterityProton = true;
            matchedIsPosterityProton.push_back(isThisPosterityProton);

            ////////////////////////
            // Calorimetry 
            // (still in track loop)
            ////////////////////////

            if (bVerbose) std::cout << "Starting calorimetry" << std::endl;

            std::vector<double> recoPitch_v; 
            std::vector<double> recoDEDX_v;
            std::vector<double> recoEDep_v;
            std::vector<double> recoResR_v;
            std::vector<double> recoZPos_v;

            // Check if calorimetry is valid 
            if (fmcal.isValid()) {
                if (bVerbose) std::cout << "fmcal is valid" << std::endl;

                // Get calorimetry for this track
                std::vector<art::Ptr<anab::Calorimetry>> calos = fmcal.at(thisTrack.key());
                
                // Loop over calo data
                for (size_t j = 0; j < calos.size(); ++j) {
                    if (!calos[j]->PlaneID().isValid) continue;
                    if (calos[j]->PlaneID().Plane == 0) continue; // Induction plane

                    if (bVerbose) std::cout << "Number of hits at plane " << j << ": " << calos[j]->dEdx().size() << std::endl;
                    
                    // Loop over hits in plane
                    for (size_t k = 0; k < calos[j]->dEdx().size(); ++k) {
                        // If the following happens, there is a mess in calorimetry module
                        if (calos[j]->XYZ()[k].Z() < 0 || calos[j]->XYZ()[k].Z() > 90.) continue;

                        // If point not in active volume, continue
                        if (!isWithinActiveVolume(calos[j]->XYZ()[k].X(), calos[j]->XYZ()[k].Y(), calos[j]->XYZ()[k].Z())) continue;

                        recoPitch_v.push_back(calos[j]->TrkPitchVec()[k]);
                        recoDEDX_v.push_back(calos[j]->dEdx()[k]);
                        recoEDep_v.push_back(calos[j]->dEdx()[k] * calos[j]->TrkPitchVec()[k]);
                        recoResR_v.push_back(calos[j]->ResidualRange()[k]);
                        recoZPos_v.push_back(calos[j]->XYZ()[k].Z());
                    } // end loop on calo points

                    if (bVerbose) std::cout << "Filled calorimetry vectors" << std::endl;
                    if (isThisTrackReversed) {
                        std::reverse(recoResR_v.begin(), recoResR_v.end());
                    }
                } // end loop over planes
            } // end if calorimetry is valid

            recoPitch.push_back(recoPitch_v);
            recoDEDX.push_back(recoDEDX_v);
            recoEDep.push_back(recoEDep_v);
            recoResR.push_back(recoResR_v);
            recoZPos.push_back(recoZPos_v);

            // Compute and save mean dedx
            double meanDEDX = 0;
            unsigned int bound = 0;
            if (recoDEDX_v.size() > MeanDEDXNumberTrajPoints) {
                bound = (int)(recoDEDX_v.size() - MeanDEDXNumberTrajPoints);
            }
            for (unsigned int i = bound; i < recoDEDX_v.size(); ++i) meanDEDX += recoDEDX_v.at(i);
            meanDEDX /= (recoDEDX_v.size() - bound);
            recoMeanDEDX.push_back(meanDEDX);
            if (bVerbose) std::cout << "Mean DEDX for track: " << meanDEDX << std::endl;

            // Clear inner vectors
            recoPitch_v.clear();
            recoDEDX_v.clear();
            recoEDep_v.clear();
            recoResR_v.clear();
            recoZPos_v.clear();

        } // end loop over tracks
    } // end loop over particle species

    // Test for broken pion tracks
    if (isPrimaryPionReco) {
        for (unsigned int i = 0; i < recoBeginX.size(); ++i) {
            // Check if there is other pion that is not primary
            if (((int) i != primaryPionRecoTrackIndex) && (matchedIdentity.at(i) == -211)) {
                double pionDistance = sqrt(
                    pow(recoBeginX.at(i) - recoEndX.at(primaryPionRecoTrackIndex), 2) + 
                    pow(recoBeginY.at(i) - recoEndY.at(primaryPionRecoTrackIndex), 2) + 
                    pow(recoBeginZ.at(i) - recoEndZ.at(primaryPionRecoTrackIndex), 2)
                );
                distancePrimaryPionOtherPion.push_back(pionDistance);
                if (pionDistance < TrackStitchingThreshold) {
                    if (bVerbose) std::cout << "Pion track broken" << std::endl;
                    hasBrokenPionTrack = true;
                }
            } // if we find pion that is not primary
        } // loop over reco tracks
    }

    if (bVerbose) std::cout << std::endl;
    RecoEvalTree->Fill();
}

void RecoEval::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;

    art::ServiceHandle<art::TFileService> tfs;

    // Make histograms and tree branches
    RecoEvalTree = tfs->make<TTree>("RecoEvalTree", "RecoEvalTree");

    RecoEvalTree->Branch("run", &run, "run/I");
    RecoEvalTree->Branch("subrun", &subrun, "subrun/I");
    RecoEvalTree->Branch("event", &event, "event/I");

    RecoEvalTree->Branch("truthPionInitialEnergy", &truthPionInitialEnergy, "truthPionInitialEnergy/D");
    RecoEvalTree->Branch("truthPionInitialKEnergy", &truthPionInitialKEnergy, "truthPionInitialKEnergy/D");
    RecoEvalTree->Branch("truthPionInitialMomentum", &truthPionInitialMomentum, "truthPionInitialMomentum/D");

    RecoEvalTree->Branch("truthPionVertexEnergy", &truthPionVertexEnergy, "truthPionVertexEnergy/D");
    RecoEvalTree->Branch("truthPionVertexKEnergy", &truthPionVertexKEnergy, "truthPionVertexKEnergy/D");
    RecoEvalTree->Branch("truthPionVertexMomentum", &truthPionVertexMomentum, "truthPionVertexMomentum/D");

    RecoEvalTree->Branch("truthProtonsEnergy", "std::vector<double>", &truthProtonsEnergy);
    RecoEvalTree->Branch("truthProtonsKEnergy", "std::vector<double>", &truthProtonsKEnergy);
    RecoEvalTree->Branch("truthProtonsInitialMomentum", "std::vector<double>", &truthProtonsInitialMomentum);
    RecoEvalTree->Branch("truthProtonsInitialPx", "std::vector<double>", &truthProtonsInitialPx);
    RecoEvalTree->Branch("truthProtonsInitialPy", "std::vector<double>", &truthProtonsInitialPy);
    RecoEvalTree->Branch("truthProtonsInitialPz", "std::vector<double>", &truthProtonsInitialPz);

    RecoEvalTree->Branch("truthPionVertexPx", &truthPionVertexPx, "truthPionVertexPx/D");
    RecoEvalTree->Branch("truthPionVertexPy", &truthPionVertexPy, "truthPionVertexPy/D");
    RecoEvalTree->Branch("truthPionVertexPz", &truthPionVertexPz, "truthPionVertexPz/D");

    RecoEvalTree->Branch("pionTrackId", &pionTrackId, "pionTrackId/I");
    RecoEvalTree->Branch("numDaughterProtons", &numDaughterProtons, "numDaughterProtons/I");
    RecoEvalTree->Branch("numDaughterNeutrons", &numDaughterNeutrons, "numDaughterNeutrons/I");

    RecoEvalTree->Branch("isPrimaryPionReco", &isPrimaryPionReco, "isPrimaryPionReco/B");
    RecoEvalTree->Branch("hasBrokenPionTrack", &hasBrokenPionTrack, "hasBrokenPionTrack/B");
    RecoEvalTree->Branch("distancePrimaryPionOtherPion", "std::vector<double>", &distancePrimaryPionOtherPion);

    RecoEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    RecoEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    RecoEvalTree->Branch("WC2TPCPionBeginX", &WC2TPCPionBeginX, "WC2TPCPionBeginX/D");
    RecoEvalTree->Branch("WC2TPCPionBeginY", &WC2TPCPionBeginY, "WC2TPCPionBeginY/D");
    RecoEvalTree->Branch("WC2TPCPionBeginZ", &WC2TPCPionBeginZ, "WC2TPCPionBeginZ/D");
    RecoEvalTree->Branch("WC3PionX", &WC3PionX, "WC3PionX/D");
    RecoEvalTree->Branch("WC3PionY", &WC3PionY, "WC3PionY/D");
    RecoEvalTree->Branch("WC3PionZ", &WC3PionZ, "WC3PionZ/D");
    RecoEvalTree->Branch("WC4PionX", &WC4PionX, "WC4PionX/D");
    RecoEvalTree->Branch("WC4PionY", &WC4PionY, "WC4PionY/D");
    RecoEvalTree->Branch("WC4PionZ", &WC4PionZ, "WC4PionZ/D");
    RecoEvalTree->Branch("WCTheta", &WCTheta, "WCTheta/D");
    RecoEvalTree->Branch("WCPhi", &WCPhi, "WCPhi/D");

    RecoEvalTree->Branch("isTrackInverted", "std::vector<bool>", &isTrackInverted);
    RecoEvalTree->Branch("recoBeginX", "std::vector<double>", &recoBeginX);
    RecoEvalTree->Branch("recoBeginY", "std::vector<double>", &recoBeginY);
    RecoEvalTree->Branch("recoBeginZ", "std::vector<double>", &recoBeginZ);
    RecoEvalTree->Branch("recoEndX", "std::vector<double>", &recoEndX);
    RecoEvalTree->Branch("recoEndY", "std::vector<double>", &recoEndY);
    RecoEvalTree->Branch("recoEndZ", "std::vector<double>", &recoEndZ);
    RecoEvalTree->Branch("recoTrkID", "std::vector<int>", &recoTrkID);

    RecoEvalTree->Branch("matchedIdentity", "std::vector<int>", &matchedIdentity);
    RecoEvalTree->Branch("matchedCleanliness", "std::vector<double>", &matchedCleanliness);
    RecoEvalTree->Branch("matchedCompleteness", "std::vector<double>", &matchedCompleteness);
    RecoEvalTree->Branch("matchedTrkID", "std::vector<int>", &matchedTrkID);
    RecoEvalTree->Branch("matchedProcess", "std::vector<std::string>", &matchedProcess);

    RecoEvalTree->Branch("matchedBeginX", "std::vector<double>", &matchedBeginX);
    RecoEvalTree->Branch("matchedBeginY", "std::vector<double>", &matchedBeginY);
    RecoEvalTree->Branch("matchedBeginY", "std::vector<double>", &matchedBeginY);
    RecoEvalTree->Branch("matchedEndX", "std::vector<double>", &matchedEndX);
    RecoEvalTree->Branch("matchedEndY", "std::vector<double>", &matchedEndY);
    RecoEvalTree->Branch("matchedEndZ", "std::vector<double>", &matchedEndZ);
    RecoEvalTree->Branch("matchedLength", "std::vector<double>", &matchedLength);

    RecoEvalTree->Branch("matchedKEnergy", "std::vector<double>", &matchedKEnergy);
    RecoEvalTree->Branch("matchedEndingKEnergy", "std::vector<double>", &matchedEndingKEnergy);

    RecoEvalTree->Branch("matchedInitialPx", "std::vector<double>", &matchedInitialPx);
    RecoEvalTree->Branch("matchedInitialPy", "std::vector<double>", &matchedInitialPy);
    RecoEvalTree->Branch("matchedInitialPz", "std::vector<double>", &matchedInitialPz);
    RecoEvalTree->Branch("matchedEndingPx", "std::vector<double>", &matchedEndingPx);
    RecoEvalTree->Branch("matchedEndingPy", "std::vector<double>", &matchedEndingPy);
    RecoEvalTree->Branch("matchedEndingPz", "std::vector<double>", &matchedEndingPz);

    RecoEvalTree->Branch("matchedIsPosterityProton", "std::vector<bool>", &matchedIsPosterityProton);

    RecoEvalTree->Branch("recoPitch","std::vector<std::vector<double>>",&recoPitch);
    RecoEvalTree->Branch("recoDEDX","std::vector<std::vector<double>>",&recoDEDX);
    RecoEvalTree->Branch("recoEDep","std::vector<std::vector<double>>",&recoEDep);
    RecoEvalTree->Branch("recoResR","std::vector<std::vector<double>>",&recoResR);
    RecoEvalTree->Branch("recoZPos","std::vector<std::vector<double>>",&recoZPos);
    RecoEvalTree->Branch("recoMeanDEDX","std::vector<double>",&recoMeanDEDX);
}

void RecoEval::fillPionTruthData(simb::MCParticle *pion) {
    unsigned int pionTrackBegin = firstPointInTPC(pion);
    int vertexIndex = pion->NumberTrajectoryPoints() - 2;

    TLorentzVector initialMomentum = pion->Momentum(pionTrackBegin);
    TLorentzVector vertexMomentum  = pion->Momentum(vertexIndex);

    truthPionInitialEnergy   = initialMomentum.E();
    truthPionInitialKEnergy  = initialMomentum.E() - PionMass;
    truthPionInitialMomentum = initialMomentum.P();

    truthPionVertexEnergy   = vertexMomentum.E();
    truthPionVertexKEnergy  = vertexMomentum.E() - PionMass;
    truthPionVertexMomentum = vertexMomentum.P();

    truthPionVertexPx = vertexMomentum.Px();
    truthPionVertexPy = vertexMomentum.Py();
    truthPionVertexPz = vertexMomentum.Pz();

    if (bVerbose) std::cout << "Pion initial momentum: " << truthPionInitialMomentum << ", pion vertex momentum: " << truthPionVertexMomentum << std::endl;
    if (bVerbose) std::cout << "Pion initial energy: " << truthPionInitialEnergy << ", pion vertex energy: " << truthPionVertexEnergy << std::endl;
    if (bVerbose) std::cout << std::endl;
}

void RecoEval::fillProtonDaughtersTruthData(std::vector<simb::MCParticle*> daughterProtons) {
    for (simb::MCParticle *proton : daughterProtons) {
        unsigned int protonTrackBegin = firstPointInTPC(proton);

        truthProtonsEnergy.push_back(proton->E(protonTrackBegin));
        truthProtonsKEnergy.push_back(proton->E(protonTrackBegin) - ProtonMass);
        truthProtonsInitialMomentum.push_back(proton->P(protonTrackBegin));
        truthProtonsInitialPx.push_back(proton->Px(protonTrackBegin));
        truthProtonsInitialPy.push_back(proton->Px(protonTrackBegin));
        truthProtonsInitialPz.push_back(proton->Px(protonTrackBegin));

        if (bVerbose) std::cout << "Filled information for proton with id: " << proton->TrackId() << std::endl;
        if (bVerbose) std::cout << std::endl;
    }
}

unsigned int RecoEval::lastPointInTPC(simb::MCParticle *track)
{
  for (int i=track->NumberTrajectoryPoints()-1; i >=0; i--) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return last point
  } 
  return 9999;
}

unsigned int RecoEval::lastPointInTPC(const art::Ptr<simb::MCParticle> track)
{
  for (int i=track->NumberTrajectoryPoints()-1; i >=0; i--) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return last point
  } 
  return 9999;
}

unsigned int RecoEval::firstPointInTPC(simb::MCParticle *track)
{
  for (unsigned int i=0; i < track->NumberTrajectoryPoints(); ++i) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return first point
  } 
  return 9999;
}

unsigned int RecoEval::firstPointInTPC(const art::Ptr<simb::MCParticle> track)
{
  for (unsigned int i=0; i < track->NumberTrajectoryPoints(); ++i) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return first point
  } 
  return 9999;
}

bool RecoEval::isPosterityOfPrimary(simb::MCParticle *particle, const sim::ParticleList& plist) {
    int motherTrackID = particle->Mother();
    int motherPosition = -1;
    for (size_t p = 0; p < plist.size(); ++p) {
        if (plist.Particle(p)->TrackId() == motherTrackID) motherPosition = p;
    }

    // Return false if no mother found/mother outside of scope
    if ((motherTrackID == -1) || (motherPosition == -1)) return false;

    // Check if we have reached primary particle
    if (plist.Particle(motherPosition)->Process() == "primary") {
        if (plist.Particle(motherPosition)->PdgCode() == -211) {
            return true; // Recursion reached primary pion, so proton is in pion's family tree
        } else {
            return false; // Recursion did not reach primary pion
        }
    }

    // Recursion for mother
    return isPosterityOfPrimary(plist.Particle(motherPosition), plist);
}

double RecoEval::trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2)
{
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double RecoEval::trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2)
{
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double RecoEval::trackMagnitude(simb::MCParticle *track)
{
  return sqrt(
    pow(track->EndX()-track->Vx(0),2) + 
    pow(track->EndY()-track->Vy(0),2) + 
    pow(track->EndZ()-track->Vz(0),2)
  );
}

bool RecoEval::isWithinActiveVolume(double x, double y, double z) {
    if (x < minX ) return false; 
    if (x > maxX ) return false;
    if (y < minY ) return false; 
    if (y > maxY ) return false;
    if (z < minZ ) return false; 
    if (z > maxZ ) return false;
    return true;
}

void RecoEval::resetTree() {
    numDaughterProtons = 0;
    numDaughterNeutrons = 0;
    
    truthProtonsEnergy.clear();
    truthProtonsKEnergy.clear();
    truthProtonsInitialMomentum.clear();
    truthProtonsInitialPx.clear();
    truthProtonsInitialPy.clear();
    truthProtonsInitialPz.clear();

    isTrackInverted.clear();
    recoBeginX.clear();
    recoBeginY.clear();
    recoBeginZ.clear();
    recoEndX.clear();
    recoEndY.clear();
    recoEndZ.clear();
    recoTrkID.clear();

    matchedIdentity.clear();
    matchedCleanliness.clear();
    matchedCompleteness.clear();
    matchedTrkID.clear();
    matchedProcess.clear();

    matchedBeginX.clear();
    matchedBeginY.clear();
    matchedBeginZ.clear();
    matchedEndX.clear();
    matchedEndY.clear();
    matchedEndZ.clear();
    matchedLength.clear();

    matchedKEnergy.clear();
    matchedEndingKEnergy.clear();

    matchedInitialPx.clear();
    matchedInitialPy.clear();
    matchedInitialPz.clear();
    matchedEndingPx.clear();
    matchedEndingPy.clear();
    matchedEndingPz.clear();

    matchedIsPosterityProton.clear();
    
    recoPitch.clear();
    recoDEDX.clear();
    recoEDep.clear();
    recoResR.clear();
    recoZPos.clear();
    recoMeanDEDX.clear();

    isPrimaryPionReco = false;
    hasBrokenPionTrack = false;
    distancePrimaryPionOtherPion.clear();
}

void RecoEval::endJob() {
    
}

void RecoEval::reconfigure(fhicl::ParameterSet const & p) {
    bVerbose = p.get<bool>("Verbose", false);
    strWCTrackBuilderLabel             = p.get<std::string>("WCTrackBuilderLabel", "wctrack");
    strTPCTrackHandleLabel             = p.get<std::string>("TPCTrackHandleLabel", "pmtrack");
    strWC2TPCModuleLabel               = p.get<std::string>("WC2TPModuleLabel", "wctracktpctrackmatch");
    strCalorimetryModuleLabel          = p.get<std::string>("CalorimetryModuleLabel", "calo");
    simulation_producer_label_         = p.get<std::string>("SimulationLabel", "largeant");
    recotrackmcparticlematching_label_ = p.get<std::string>("RecoTrackMCMatchLabel", "recotrackmcmatching");
    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 60);
    TrackStitchingThreshold            = p.get<double> ("TrackStitchingThreshold",4);
}

DEFINE_ART_MODULE(RecoEval)