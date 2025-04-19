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

        bool isPosterityOfPrimary(simb::MCParticle *particle, const sim::ParticleList& plist);
        double trackMagnitude(simb::MCParticle *track);
        double trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2);
        double trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2);
        bool isWithinActiveVolume(double x, double y, double z);
        bool isWithinReducedVolume(double x, double y, double z);
        bool isWithinReducedVolume(simb::MCParticle *track);

        // Get data about event signature
        void fillSignalInformation(int pdg, float vx, float vy, float vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void fillBackgroundInformation(int pdg, float vx, float vy, float vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);

    private: 
        // Produce's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string simulation_producer_label_;
        std::string recotrackmcparticlematching_label_;

        // fcl parameters
        bool         bVerbose;
        unsigned int MeanDEDXNumberTrajPoints;
        double       TrackStitchingThreshold;
        float        PROTON_ENERGY_LOWER_BOUND;
        float        PROTON_ENERGY_UPPER_BOUND;

        // Output tree
        TTree *RecoEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;

        // Signal information
        bool isPionAbsorptionSignal;
        int  numVisibleProtons;

        // Background information
        int backgroundType; 
        int NUM_BACKGROUND_TYPES = 11;
        // Background types:
        //    0:  0p pion absorption
        //    1:  Np pion absorption
        //    2:  primary muon event
        //    3:  primary electron event
        //    4:  other primary event
        //    5:  primary pion outside reduced volume
        //    6:  pion inelastic scattering
        //    7:  charge exchange
        //    8:  double charge exchange
        //    9:  capture at rest
        //    10: decay
        //    11: other

        // Truth primary information
        int                      truthPrimaryPDG;
        float                    truthPrimaryVertexX;
        float                    truthPrimaryVertexY;
        float                    truthPrimaryVertexZ;
        std::vector<int>         truthPrimaryDaughtersPDG;
        std::vector<std::string> truthPrimaryDaughtersProcess;
        std::vector<double>      truthPrimaryDaughtersKE;

        // WC variables
        int WC2TPCtrkID;
        double WCTrackMomentum;
        double WC2TPCPrimaryBeginX;
        double WC2TPCPrimaryBeginY;
        double WC2TPCPrimaryBeginZ;
        double WC3PrimaryX;
        double WC3PrimaryY;
        double WC3PrimaryZ;
        double WC4PrimaryX;
        double WC4PrimaryY;
        double WC4PrimaryZ;
        double WCTheta;
        double WCPhi;

        // Reco variables
        std::vector<bool>   isTrackInverted;
        std::vector<double> recoBeginX;
        std::vector<double> recoBeginY;
        std::vector<double> recoBeginZ;
        std::vector<double> recoEndX;
        std::vector<double> recoEndY;
        std::vector<double> recoEndZ;
        std::vector<int>    recoTrkID;

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
        std::vector<double> matchedRealEndX;
        std::vector<double> matchedRealEndY;
        std::vector<double> matchedRealEndZ;
        std::vector<double> matchedLength;

        std::vector<double> matchedKEnergy;
        std::vector<double> matchedEndingKEnergy;

        std::vector<double> matchedInitialPx;
        std::vector<double> matchedInitialPy;
        std::vector<double> matchedInitialPz;
        std::vector<double> matchedEndingPx;
        std::vector<double> matchedEndingPy;
        std::vector<double> matchedEndingPz;
        
        // Calorimetry variables for tracks
        std::vector<std::vector<double>> recoPitch;
        std::vector<std::vector<double>> recoDEDX;
        std::vector<std::vector<double>> recoEDep;
        std::vector<std::vector<double>> recoResR;
        std::vector<std::vector<double>> recoZPos;
        std::vector<double> recoMeanDEDX;

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

    //////////////
    // Get MC data
    //////////////

    // Get simulated particles
    auto particle_handle = e.getValidHandle<std::vector<simb::MCParticle>>(simulation_producer_label_);
    std::vector<art::Ptr<simb::MCParticle>> particle_vector;
    art::fill_ptr_vector(particle_vector, particle_handle);

    // Get particle list
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();

    // Initialize particle map (typedef at top) and fill it
    ParticleMap particle_map;
    for (auto const& particle : particle_vector) {
        particle_map[particle->TrackId()] = particle;
    }

    // Identify true-level primary pion and get its information
    std::vector<int> primaryDaughtersIDs;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (part->Process() == "primary") {
            truthPrimaryPDG = part->PdgCode();
            for (int i = 0; i < part->NumberDaughters(); ++i) primaryDaughtersIDs.push_back(part->Daughter(i));
            truthPrimaryVertexX = part->EndX();
            truthPrimaryVertexY = part->EndY();
            truthPrimaryVertexZ = part->EndZ(); 
            break;
        }
    }

    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (std::find(primaryDaughtersIDs.begin(), primaryDaughtersIDs.end(), part->TrackId()) != primaryDaughtersIDs.end()) {
            truthPrimaryDaughtersProcess.push_back(part->Process());
            truthPrimaryDaughtersPDG.push_back(part->PdgCode());
            truthPrimaryDaughtersKE.push_back(part->E() - part->Mass());
        }
    }

    fillSignalInformation(
        truthPrimaryPDG, 
        truthPrimaryVertexX,
        truthPrimaryVertexY,
        truthPrimaryVertexZ,
        truthPrimaryDaughtersPDG,
        truthPrimaryDaughtersProcess,
        truthPrimaryDaughtersKE
    );

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
    if (numWCtrks != 1) return; 

    // Get wcTrack momentum
    WCTrackMomentum = wctrack[0]->Momentum() * 0.001; // Mev to GeV
    WC3PrimaryX = wctrack[0]->HitPosition(2,0);
    WC3PrimaryY = wctrack[0]->HitPosition(2,1);
    WC3PrimaryZ = wctrack[0]->HitPosition(2,2);
    WC4PrimaryX = wctrack[0]->HitPosition(3,0);
    WC4PrimaryY = wctrack[0]->HitPosition(3,1);
    WC4PrimaryZ = wctrack[0]->HitPosition(3,2);
    WCTheta     = wctrack[0]->Theta();
    WCPhi       = wctrack[0]->Phi();

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
            WC2TPCPrimaryBeginX = recoWC2TPCBeginning.X();
            WC2TPCPrimaryBeginY = recoWC2TPCBeginning.Y();
            WC2TPCPrimaryBeginZ = recoWC2TPCBeginning.Z();
        } // end trackWC2TPC loop
    } // end if fWC2TPC.isValid()

    /////////////////////
    // MC particle tracks
    /////////////////////

    // Geometry service
    art::ServiceHandle<geo::Geometry> geom;
    // Get the backtracer to recover true quantities
    art::ServiceHandle<cheat::BackTrackerService> bt;

    ////////////////////////////////////
    // Truth matching using Johnny's Alg
    ////////////////////////////////////

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

    // Useful to order protons based on furthest-right pion track later on
    double maxPrimaryEndZ    = -9999;
    double primaryRecoEndX   = -9999;
    double primaryRecoEndY   = -9999;
    double primaryRecoEndZ   = -9999;
    int primaryRecoTrackIndex = -9999;

    // Do track matching
    for (int particleSpeciesIndex = 0; particleSpeciesIndex <= 2; particleSpeciesIndex++) {
        // This first loop goes over the types of particle that we want to match in order of
        // their priority: primary, protons, everything else

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
            if (bVerbose) std::cout << "Cleanliness * completeness of matched tracks:" << std::endl;
            for (unsigned int i = 0; i < btdata_vector.size(); ++i) {
                double this_cleanliness = btdata_vector[i]->cleanliness;
                double this_completeness = btdata_vector[i]->completeness;
                if (bVerbose) std::cout << this_cleanliness * this_completeness << std::endl;
            }
            if (bVerbose) std::cout << std::endl;

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

                if (recoEnd.Z() > maxPrimaryEndZ) {
                    maxPrimaryEndZ  = recoEnd.Z();
                    primaryRecoEndX = recoEnd.X();
                    primaryRecoEndY = recoEnd.Y();
                    primaryRecoEndZ = recoEnd.Z();
                }
            } // end pion matching

            // Proton matching and ordering
            if (particleSpeciesIndex == 1) {
                if (sqrt(
                    pow((thisTrack->Start()).X() - primaryRecoEndX, 2) + 
                    pow((thisTrack->Start()).Y() - primaryRecoEndY, 2) + 
                    pow((thisTrack->Start()).Z() - primaryRecoEndZ, 2)
                ) < sqrt(
                    pow((thisTrack->End()).X() - primaryRecoEndX, 2) + 
                    pow((thisTrack->End()).Y() - primaryRecoEndY, 2) + 
                    pow((thisTrack->End()).Z() - primaryRecoEndZ, 2)
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
            matchedRealEndX.push_back(particle->Vx(particle->NumberTrajectoryPoints()));
            matchedRealEndY.push_back(particle->Vy(particle->NumberTrajectoryPoints()));
            matchedRealEndZ.push_back(particle->Vz(particle->NumberTrajectoryPoints()));
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
                        std::reverse(recoDEDX_v.begin(), recoDEDX_v.end());
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
            unsigned int bound = MeanDEDXNumberTrajPoints;
            if (MeanDEDXNumberTrajPoints > recoDEDX_v.size()) bound = recoDEDX_v.size();
            for (unsigned int i = 0; i < bound; ++i) meanDEDX += recoDEDX_v.at(i);
            meanDEDX /= bound;
            recoMeanDEDX.push_back(meanDEDX);
            if (bVerbose) std::cout << "Mean DEDX for track: " << meanDEDX << std::endl;
            if (bVerbose) std::cout << std::endl;

            // Clear inner vectors
            recoPitch_v.clear();
            recoDEDX_v.clear();
            recoEDep_v.clear();
            recoResR_v.clear();
            recoZPos_v.clear();

        } // end loop over tracks
    } // end loop over particle species

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

    RecoEvalTree->Branch("isPionAbsorptionSignal", &isPionAbsorptionSignal, "isPionAbsorptionSignal/O");
    RecoEvalTree->Branch("numVisibleProtons", &numVisibleProtons, "numVisibleProtons/I");
    RecoEvalTree->Branch("backgroundType", &backgroundType, "backgroundType/I");

    RecoEvalTree->Branch("truthPrimaryPDG", &truthPrimaryPDG, "truthPrimaryPDG/I");
    RecoEvalTree->Branch("truthPrimaryVertexX", &truthPrimaryVertexX, "truthPrimaryVertexX/F");
    RecoEvalTree->Branch("truthPrimaryVertexY", &truthPrimaryVertexY, "truthPrimaryVertexY/F");
    RecoEvalTree->Branch("truthPrimaryVertexZ", &truthPrimaryVertexZ, "truthPrimaryVertexZ/F");
    RecoEvalTree->Branch("truthPrimaryDaughtersPDG", "std::vector<int>", &truthPrimaryDaughtersPDG);
    RecoEvalTree->Branch("truthPrimaryDaughtersProcess", "std::vector<std::string>", &truthPrimaryDaughtersProcess);
    RecoEvalTree->Branch("truthPrimaryDaughtersKE", "std::vector<double>", &truthPrimaryDaughtersKE);

    RecoEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    RecoEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginX", &WC2TPCPrimaryBeginX, "WC2TPCPrimaryBeginX/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginY", &WC2TPCPrimaryBeginY, "WC2TPCPrimaryBeginY/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginZ", &WC2TPCPrimaryBeginZ, "WC2TPCPrimaryBeginZ/D");
    RecoEvalTree->Branch("WC3PrimaryX", &WC3PrimaryX, "WC3PrimaryX/D");
    RecoEvalTree->Branch("WC3PrimaryY", &WC3PrimaryY, "WC3PrimaryY/D");
    RecoEvalTree->Branch("WC3PrimaryZ", &WC3PrimaryZ, "WC3PrimaryZ/D");
    RecoEvalTree->Branch("WC4PrimaryX", &WC4PrimaryX, "WC4PrimaryX/D");
    RecoEvalTree->Branch("WC4PrimaryY", &WC4PrimaryY, "WC4PrimaryY/D");
    RecoEvalTree->Branch("WC4PrimaryZ", &WC4PrimaryZ, "WC4PrimaryZ/D");
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
    RecoEvalTree->Branch("matchedRealEndX", "std::vector<double>", &matchedRealEndX);
    RecoEvalTree->Branch("matchedRealEndY", "std::vector<double>", &matchedRealEndY);
    RecoEvalTree->Branch("matchedRealEndZ", "std::vector<double>", &matchedRealEndZ);
    RecoEvalTree->Branch("matchedLength", "std::vector<double>", &matchedLength);

    RecoEvalTree->Branch("matchedKEnergy", "std::vector<double>", &matchedKEnergy);
    RecoEvalTree->Branch("matchedEndingKEnergy", "std::vector<double>", &matchedEndingKEnergy);

    RecoEvalTree->Branch("matchedInitialPx", "std::vector<double>", &matchedInitialPx);
    RecoEvalTree->Branch("matchedInitialPy", "std::vector<double>", &matchedInitialPy);
    RecoEvalTree->Branch("matchedInitialPz", "std::vector<double>", &matchedInitialPz);
    RecoEvalTree->Branch("matchedEndingPx", "std::vector<double>", &matchedEndingPx);
    RecoEvalTree->Branch("matchedEndingPy", "std::vector<double>", &matchedEndingPy);
    RecoEvalTree->Branch("matchedEndingPz", "std::vector<double>", &matchedEndingPz);

    RecoEvalTree->Branch("recoPitch","std::vector<std::vector<double>>",&recoPitch);
    RecoEvalTree->Branch("recoDEDX","std::vector<std::vector<double>>",&recoDEDX);
    RecoEvalTree->Branch("recoEDep","std::vector<std::vector<double>>",&recoEDep);
    RecoEvalTree->Branch("recoResR","std::vector<std::vector<double>>",&recoResR);
    RecoEvalTree->Branch("recoZPos","std::vector<std::vector<double>>",&recoZPos);
    RecoEvalTree->Branch("recoMeanDEDX","std::vector<double>",&recoMeanDEDX);
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

bool RecoEval::isWithinReducedVolume(simb::MCParticle *track) {
    return (
      (track->EndX()>RminX) && (track->EndX()<RmaxX) && 
      (track->EndY()>RminY) && (track->EndY()<RmaxY) && 
      (track->EndZ()>RminZ) && (track->EndZ()<RmaxZ)
    );
}

bool RecoEval::isWithinReducedVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}

void RecoEval::fillSignalInformation(
    int pdg,
    float vx, float vy, float vz,
    std::vector<int> daughtersPDG, 
    std::vector<std::string> daughtersProcess, 
    std::vector<double> daughtersKE
) {
    bool isPionAbsorptionSignalTemp = true;

    if (pdg != -211) isPionAbsorptionSignalTemp = false;
    if (!isWithinReducedVolume(vx, vy, vz)) isPionAbsorptionSignalTemp = false;

    int numDaughters = daughtersPDG.size();
    int tempNumProtons = 0;
    for (int iDaughter = 0; iDaughter < numDaughters; iDaughter++) {
        if ((daughtersPDG[iDaughter] == 11) && (daughtersProcess[iDaughter] == "hIoni")) continue;
        if ((daughtersPDG[iDaughter] == 111) || (daughtersPDG[iDaughter] == 211) || (daughtersPDG[iDaughter] == -211)) isPionAbsorptionSignalTemp = false;
        if ((daughtersProcess[iDaughter] == "Decay") || (daughtersProcess[iDaughter] == "hBertiniCaptureAtRest")) isPionAbsorptionSignalTemp = false;

        if (daughtersProcess[iDaughter] == "pi-Inelastic") {
            if ((daughtersPDG[iDaughter] == 13) || (daughtersPDG[iDaughter] == -13)) { isPionAbsorptionSignalTemp = false; } // muon
            else if ((daughtersPDG[iDaughter] == 321) || (daughtersPDG[iDaughter] == -321) || (daughtersPDG[iDaughter] == 311)) { isPionAbsorptionSignalTemp = false; } // kaon
            else if (daughtersPDG[iDaughter] == 2212) {
                if ((daughtersKE[iDaughter] >= PROTON_ENERGY_LOWER_BOUND) && (daughtersKE[iDaughter] <= PROTON_ENERGY_UPPER_BOUND)) {
                    tempNumProtons++;
                }
            }
        }
    }

    if (isPionAbsorptionSignalTemp) {
        // Event is signal!
        numVisibleProtons      = tempNumProtons;
        isPionAbsorptionSignal = true;
    } else {
        // Event is background, classify it
        fillBackgroundInformation(
            pdg,
            vx, vy, vz,
            daughtersPDG,
            daughtersProcess,
            daughtersKE
        );
    }

    return;
}

void RecoEval::fillBackgroundInformation(
    int pdg,
    float vx, float vy, float vz,
    std::vector<int> daughtersPDG, 
    std::vector<std::string> daughtersProcess, 
    std::vector<double> daughtersKE
) {
    if (pdg != -211) {
        if (pdg == 13) { backgroundType = 2; }
        else if (pdg == 11) { backgroundType = 3; }
        else { backgroundType = 4; }
        return; 
    } 

    int numDaughters = daughtersPDG.size();
    int numNegativePions = 0; int numNeutralPions = 0; int numPositivePions = 0;
    for (int iDaughter = 0; iDaughter < numDaughters; iDaughter++) {
        if (daughtersPDG[iDaughter] == -211) {
            numNegativePions++; 
        } else if (daughtersPDG[iDaughter] == 111) {
            numNeutralPions++;
        } else if (daughtersPDG[iDaughter] == 211) {
            numPositivePions++;
        } else if (daughtersProcess[iDaughter] == "hBertiniCaptureAtRest") {
            backgroundType = 9; return;
        } else if (daughtersProcess[iDaughter] == "Decay") {
            backgroundType = 10; return;
        }
    }

    if ((numNegativePions + numNeutralPions + numPositivePions) > 0) {
        if ((numNegativePions == 1) && (numNeutralPions == 0) && (numPositivePions == 0)) {
            backgroundType = 6;
        } else if ((numNegativePions == 0) && (numNeutralPions == 1) && (numPositivePions == 0)) {
            backgroundType = 7;
        } else if ((numNegativePions == 0) && (numNeutralPions == 0) && (numPositivePions == 1)) {
            backgroundType = 8;
        }
    }

    // Only flag as outside reduced volume if it is not anything else
    if ((backgroundType == -1) && (!isWithinReducedVolume(vx, vy, vz))) { backgroundType = 5; return; }

    // If not flagged at this point, label as other
    if (backgroundType == -1) backgroundType = 11;
}

void RecoEval::resetTree() {
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
    matchedRealEndX.clear();
    matchedRealEndY.clear();
    matchedRealEndZ.clear();
    matchedLength.clear();

    matchedKEnergy.clear();
    matchedEndingKEnergy.clear();

    matchedInitialPx.clear();
    matchedInitialPy.clear();
    matchedInitialPz.clear();
    matchedEndingPx.clear();
    matchedEndingPy.clear();
    matchedEndingPz.clear();
    
    recoPitch.clear();
    recoDEDX.clear();
    recoEDep.clear();
    recoResR.clear();
    recoZPos.clear();
    recoMeanDEDX.clear();
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
    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 20);
    TrackStitchingThreshold            = p.get<double> ("TrackStitchingThreshold",4);
    PROTON_ENERGY_LOWER_BOUND          = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND          = p.get<float>("ProtonEnergyUpperBound", 1.0);
}

DEFINE_ART_MODULE(RecoEval)