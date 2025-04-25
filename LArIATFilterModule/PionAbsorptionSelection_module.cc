///////////////////////////////////////////////////////////////////////
// Class:       PionAbsorptionSelection
// Module Type: filter
// File:        PionAbsorptionSelection_module.cc
//
// Created on January 2025 by Emilio Peláez
//
// Selection for pion absorption interactions among data events
// 
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

class PionAbsorptionSelection : public art::EDFilter {
    public:
        explicit PionAbsorptionSelection(fhicl::ParameterSet const &p); 

        PionAbsorptionSelection(PionAbsorptionSelection const &) = delete;
        PionAbsorptionSelection(PionAbsorptionSelection &&) = delete;
        PionAbsorptionSelection & operator = (PionAbsorptionSelection const &) = delete;
        PionAbsorptionSelection & operator = (PionAbsorptionSelection &&) = delete;

        // Required functions
        bool filter(art::Event &e) override;
        void beginJob() override;
        void endJob() override;
        void reconfigure(fhicl::ParameterSet const &p);

        // Helper methods
        void resetTree();
        bool isWithinActiveVolume(double x, double y, double z);
        bool isWithinReducedVolume(double x, double y, double z);
        bool isWithinReducedVolume(simb::MCParticle *track);
        int lastPointInTPC(simb::MCParticle *track);
        double meanDEDX(art::FindManyP<anab::Calorimetry> fmcal, unsigned int trackKey, bool isThisTrackReversed);
        std::tuple<double, double> computeCurvature(recob::Track track);
        double curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3);
        double distance(double x1, double x2, double y1, double y2, double z1, double z2);
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
        double       fVertexRadius;
        double       SmallTrackLength;
        int          MaxSmallTracks;
        double       MeanCurvatureThreshold;
        float        PROTON_ENERGY_LOWER_BOUND;
        float        PROTON_ENERGY_UPPER_BOUND;

        // Histograms
        TH1D* hTotalEvents;
        TH1D* hTotalEvents0pSignal;
        TH1D* hTotalEventsNpSignal;
        TH1D* hTotalBackground;

        TH1D* hWCExists;
        TH1D* hWCExists0pSignal;
        TH1D* hWCExistsNpSignal;
        TH1D* hWCExistsBackground;

        TH1D* hPionInRedVolume;
        TH1D* hPionInRedVolume0pSignal;
        TH1D* hPionInRedVolumeNpSignal;
        TH1D* hPionInRedVolumeBackground;

        // For this, filling with 0.5 indicates 0p reco event
        // and filling with 1.5 indicates Np reco event
        // For the background histos, 0pBackground contains
        // the events that are reco'ed as 0p but are not at
        // truth-level
        TH1D* hNoOutgoingPion;
        TH1D* hNoOutgoingPion0pSignal;
        TH1D* hNoOutgoingPionNpSignal;
        TH1D* hNoOutgoingPion0pBackground;
        TH1D* hNoOutgoingPionNpBackground;

        TH1D* hSmallTracks;
        TH1D* hSmallTracks0pSignal;
        TH1D* hSmallTracksNpSignal;
        TH1D* hSmallTracks0pBackground;
        TH1D* hSmallTracksNpBackground;

        TH1D* hMeanCurvature;
        TH1D* hMeanCurvature0pSignal;
        TH1D* hMeanCurvatureNpSignal;
        TH1D* hMeanCurvature0pBackground;
        TH1D* hMeanCurvatureNpBackground;

        TH1D* hFinalRecoEvents;
        TH1D* hFinalRecoEvents0pSignal;
        TH1D* hFinalRecoEventsNpSignal;
        TH1D* hFinalRecoEvents0pBackground;
        TH1D* hFinalRecoEventsNpBackground;

        // Cut variables
        double fMeanDEDXThreshold;

        // Output tree
        TTree* PionAbsTree;

        // Event metadata
        int run;
        int subrun;
        int event;

        // WC variables
        int    WC2TPCtrkID = -99999;
        double WCTrackMomentum;
        double WC2TPCPionBeginX;
        double WC2TPCPionBeginY;
        double WC2TPCPionBeginZ;
        double WC2TPCPionEndX;
        double WC2TPCPionEndY;
        double WC2TPCPionEndZ;
        double WC2TPCPionLength;
        double WC3PionX;
        double WC3PionY;
        double WC3PionZ;
        double WC4PionX;
        double WC4PionY;
        double WC4PionZ;
        double WCTheta;
        double WCPhi;
        bool   isPionReversed = false;

        // Wire chamber match truth information
        int                      wcMatchPDG;
        std::string              wcMatchProcess = "";
        std::vector<int>         wcMatchDaughtersPDG;
        std::vector<std::string> wcMatchDaughtersProcess;

        // Truth primary information
        int                      truthPrimaryPDG;
        float                    truthPrimaryVertexX;
        float                    truthPrimaryVertexY;
        float                    truthPrimaryVertexZ;
        std::vector<int>         truthPrimaryDaughtersPDG;
        std::vector<std::string> truthPrimaryDaughtersProcess;
        std::vector<double>      truthPrimaryDaughtersKE;

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

        // Proton tracks
        int                      protonCount = 0;
        std::vector<double>      protonBeginX;
        std::vector<double>      protonBeginY;
        std::vector<double>      protonBeginZ;
        std::vector<double>      protonEndX;
        std::vector<double>      protonEndY;
        std::vector<double>      protonEndZ;
        std::vector<double>      protonTrkID;
        std::vector<double>      protonLength;
        std::vector<bool>        isProtonInverted;
        std::vector<bool>        isProtonStopping;
        std::vector<std::string> protonTrueProcess;

        // Curvature of WC track
        double WCMeanCurvature;
        double WCMaxCurvature;

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
        
        // Reduced volume for pion vertex
        const double RminX =  5.0;
        const double RmaxX = 42.0;
        const double RminY =-15.0; 
        const double RmaxY = 15.0; 
        const double RminZ =  8.0;
        const double RmaxZ = 82.0;
};

PionAbsorptionSelection::PionAbsorptionSelection(fhicl::ParameterSet const &p) : EDFilter(p) {
    this->reconfigure(p);
}

bool PionAbsorptionSelection::filter(art::Event &e) {
    resetTree();

    // Get event metadata
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

    // Initialize particle map and fill it
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

    // Events before any selection cut
    hTotalEvents->Fill(0.5);
    if (isPionAbsorptionSignal) {
        if (numVisibleProtons == 0) hTotalEvents0pSignal->Fill(0.5);
        if (numVisibleProtons > 0)  hTotalEventsNpSignal->Fill(0.5);
    }
    hTotalBackground->Fill(backgroundType);

    // First, we get all the reco data products we are going to need to make cuts

    //////////////////////
    // Wire chamber tracks
    //////////////////////

    if (bVerbose) std::cout << "Getting wire chamber track information" << std::endl;

    art::Handle<std::vector<ldp::WCTrack>> wctrackHandle;
    std::vector<art::Ptr<ldp::WCTrack>>    wctrack;
    // If there is no wire chamber tracks for right label, return
    if (!e.getByLabel(strWCTrackBuilderLabel, wctrackHandle)) return false;
    art::fill_ptr_vector(wctrack, wctrackHandle);

    int numWCtrks = wctrack.size(); // number of wire chamber tracks
    if (numWCtrks != 1) return false; 

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
    if (!e.getByLabel(strTPCTrackHandleLabel, tpcTrackHandle)) return false; 
    art::fill_ptr_vector(tracklist, tpcTrackHandle);

    int numTracksReco = tracklist.size();
    if (!numTracksReco) return false; // If no TPC tracks, return
    
    if (bVerbose) std::cout << "Number of TPC reco tracks: " << numTracksReco << std::endl;
    if (bVerbose) std::cout << std::endl;

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

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

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

    //////////////////////
    // Selection algorithm
    //////////////////////

    // Check that there is a WC to TPC match, and check if pion stops inside fiducial volume
    if (WC2TPCtrkID != -99999) {
        // There is WC match
        hWCExists->Fill(0.5);
        if (isPionAbsorptionSignal) {
            if (numVisibleProtons == 0) hWCExists0pSignal->Fill(0.5);
            if (numVisibleProtons > 0)  hWCExistsNpSignal->Fill(0.5);
        }
        hWCExistsBackground->Fill(backgroundType);

        // Identify pion among reco tracks
        for (size_t trk_idx = 0; trk_idx < tpcTrackHandle -> size(); ++trk_idx) {
            auto thisTrack = tracklist.at(trk_idx);
            recob::TrackTrajectory::Point_t recoWC2TPCBeginning;
            recob::TrackTrajectory::Point_t recoWC2TPCEnd;

            // Check if this track matches ID of particle matched to WC (i.e., pion)
            if (thisTrack->ID() == WC2TPCtrkID) {
                // Get curvature for track
                auto [meanCurvature, maxCurvature] = computeCurvature(*thisTrack);
                WCMeanCurvature = meanCurvature;
                WCMaxCurvature  = maxCurvature;

                // Reverse pion if needed 
                if ((thisTrack->Start()).Z() < (thisTrack->End()).Z()) {
                    recoWC2TPCBeginning = thisTrack->Start();
                    recoWC2TPCEnd = thisTrack->End();
                } else {
                    recoWC2TPCEnd = thisTrack->Start();
                    recoWC2TPCBeginning = thisTrack->End();
                    isPionReversed = true;
                }

                // Grab pion position data
                WC2TPCPionBeginX = recoWC2TPCBeginning.X();
                WC2TPCPionBeginY = recoWC2TPCBeginning.Y();
                WC2TPCPionBeginZ = recoWC2TPCBeginning.Z();
                WC2TPCPionEndX = recoWC2TPCEnd.X();
                WC2TPCPionEndY = recoWC2TPCEnd.Y();
                WC2TPCPionEndZ = recoWC2TPCEnd.Z();
                WC2TPCPionLength = thisTrack->Length();

                // Check pion track end is inside reduced volume
                if (!(isWithinReducedVolume(WC2TPCPionEndX, WC2TPCPionEndY, WC2TPCPionEndZ))) {
                    // We can have cases in which a proton going in the same (or close) direction to the pion 
                    // gets reconstructed as the same track; we identify these events with dE/dx
                    if (meanDEDX(fmcal, thisTrack.key(), isPionReversed) > fMeanDEDXThreshold) {
                        // TODO: do something else with these events?
                    } else {
                        return false;
                    }
                }

                // If we accept this pion, we want information about its process
                std::vector<art::Ptr<simb::MCParticle>> const& particles = find_many_mcparticles_from_tracks.at(trk_idx);
                std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trk_idx);
                
                auto const& particle = particles.front();
                wcMatchPDG     = particle->PdgCode();
                wcMatchProcess = particle->Process();
                
                // Get daughters IDs
                std::vector<int> daughterIDs;
                for (int i = 0; i < particle->NumberDaughters(); ++i) daughterIDs.push_back(particle->Daughter(i));

                for (size_t p = 0; p < plist.size(); ++p) {
                    auto part = plist.Particle(p);
                    if (std::find(daughterIDs.begin(), daughterIDs.end(), part->TrackId()) != daughterIDs.end()) {
                        wcMatchDaughtersProcess.push_back(part->Process());
                        wcMatchDaughtersPDG.push_back(part->PdgCode());
                    }
                }
                break;
            }
        }
    } // end of WC2TPCtrkID if statement
    else {
        // if no match
        return false;
    }

    // If we made it here, pion vertex is inside reduced volume
    hPionInRedVolume->Fill(0.5);
    if (isPionAbsorptionSignal) {
        if (numVisibleProtons == 0) hPionInRedVolume0pSignal->Fill(0.5);
        if (numVisibleProtons > 0)  hPionInRedVolumeNpSignal->Fill(0.5);
    }
    hPionInRedVolumeBackground->Fill(backgroundType);

    // Count small tracks for shower cut
    int numSmallTracks = 0;

    // Now we want to look at near pion
    for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
        auto thisTrack = tracklist.at(trk_idx);
        recob::TrackTrajectory::Point_t recoBeginning;
        recob::TrackTrajectory::Point_t recoEnd;

        // Continue if ID is that of matched beamline particle
        if (thisTrack->ID() == WC2TPCtrkID) continue;

        bool isThisTrackReversed = false;
        bool isThisTrackStopping = false;

        // Order track
        double startDistance = distance(thisTrack->Start().X(), WC2TPCPionEndX, thisTrack->Start().Y(), WC2TPCPionEndY, thisTrack->Start().Z(), WC2TPCPionEndZ);             
        double endDistance = distance(thisTrack->End().X(), WC2TPCPionEndX, thisTrack->End().Y(), WC2TPCPionEndY, thisTrack->End().Z(), WC2TPCPionEndZ);

        if (startDistance < endDistance) {
            recoBeginning = thisTrack->Start();
            recoEnd = thisTrack->End();
        } else {
            isThisTrackReversed = true;
            recoBeginning = thisTrack->End();
            recoEnd = thisTrack->Start();
        }

        // Check if track counts towards count of small tracks
        double thisTrackLength = sqrt(
            pow(recoBeginning.X() - recoEnd.X(), 2) +
            pow(recoBeginning.Y() - recoEnd.Y(), 2) + 
            pow(recoBeginning.Z() - recoEnd.Z(), 2)
        );
        if (thisTrackLength < SmallTrackLength) numSmallTracks++;

        if (bVerbose) std::cout << "Looking at track with start distance: " << startDistance << " and end distance: " << endDistance << std::endl;
        if (bVerbose) std::cout << std::endl;

        // Find tracks near pion
        if ((startDistance < fVertexRadius) || (endDistance < fVertexRadius)) {
            // Check if track stops in fiducial volume
            isThisTrackStopping = isWithinReducedVolume(recoEnd.X(), recoEnd.Y(), recoEnd.Z());

            // Reject events with outgoing pions
            double thisMeanDEDX = meanDEDX(fmcal, thisTrack.key(), isThisTrackReversed);
            if (thisMeanDEDX <= fMeanDEDXThreshold) {
                // TODO: pion stitching?
                return false;
            }

            // If we will accept this proton, we want truth information about its
            // mother particle and the process that generated the particle
            std::vector<art::Ptr<simb::MCParticle>> const& particles = find_many_mcparticles_from_tracks.at(trk_idx);
            std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trk_idx);

            std::string protonProcess = "";
            if (btdata_vector.size() > 0) {
                auto const& particle = particles.front();
                protonProcess = particle->Process();
            }
            
            protonCount++;
            protonBeginX.push_back(recoBeginning.X());
            protonBeginY.push_back(recoBeginning.Y());
            protonBeginZ.push_back(recoBeginning.Z());
            protonEndX.push_back(recoEnd.X());
            protonEndY.push_back(recoEnd.Y());
            protonEndZ.push_back(recoEnd.Z());
            protonTrkID.push_back(thisTrack->ID());
            protonLength.push_back(thisTrack->Length());
            isProtonInverted.push_back(isThisTrackReversed);
            isProtonStopping.push_back(isThisTrackStopping);

            protonTrueProcess.push_back(protonProcess);

            if (bVerbose) std::cout << "Found proton track near pion with DEDX: " << thisMeanDEDX << std::endl;
            if (bVerbose) std::cout << std::endl;
        } // end if track begins or ends near pion
    } // end loop over tracks near pion

    // At this point, found event with no outgoing pions
    if (protonCount == 0) hNoOutgoingPion->Fill(0.5);
    if (protonCount > 0)  hNoOutgoingPion->Fill(1.5);
    if (isPionAbsorptionSignal) {
        if ((numVisibleProtons == 0) && (protonCount == 0)) hNoOutgoingPion0pSignal->Fill(0.5);
        if ((numVisibleProtons == 0) && (protonCount > 0))  { hNoOutgoingPion0pSignal->Fill(1.5); hNoOutgoingPionNpBackground->Fill(0); }
        if ((numVisibleProtons > 0) && (protonCount == 0))  { hNoOutgoingPionNpSignal->Fill(0.5); hNoOutgoingPion0pBackground->Fill(1); }
        if ((numVisibleProtons > 0) && (protonCount > 0))   hNoOutgoingPionNpSignal->Fill(1.5);
    } else {
        if (protonCount == 0)     hNoOutgoingPion0pBackground->Fill(backgroundType);
        else if (protonCount > 0) hNoOutgoingPionNpBackground->Fill(backgroundType);
    }

    // Shower cut
    if (numSmallTracks > MaxSmallTracks) {
        return false;
    }
    if (protonCount == 0) hSmallTracks->Fill(0.5);
    if (protonCount > 0)  hSmallTracks->Fill(1.5);
    if (isPionAbsorptionSignal) {
        if ((numVisibleProtons == 0) && (protonCount == 0)) hSmallTracks0pSignal->Fill(0.5);
        if ((numVisibleProtons == 0) && (protonCount > 0))  { hSmallTracks0pSignal->Fill(1.5); hSmallTracksNpBackground->Fill(0); }
        if ((numVisibleProtons > 0) && (protonCount == 0))  { hSmallTracksNpSignal->Fill(0.5); hSmallTracks0pBackground->Fill(1); }
        if ((numVisibleProtons > 0) && (protonCount > 0))   hSmallTracksNpSignal->Fill(1.5);
    } else {
        if (protonCount == 0)     hSmallTracks0pBackground->Fill(backgroundType);
        else if (protonCount > 0) hSmallTracksNpBackground->Fill(backgroundType);
    }

    // Curvature cut
    if (WCMeanCurvature > MeanCurvatureThreshold) {
        return false;
    }
    if (protonCount == 0) hMeanCurvature->Fill(0.5);
    if (protonCount > 0)  hMeanCurvature->Fill(1.5);
    if (isPionAbsorptionSignal) {
        if ((numVisibleProtons == 0) && (protonCount == 0)) hMeanCurvature0pSignal->Fill(0.5);
        if ((numVisibleProtons == 0) && (protonCount > 0))  { hMeanCurvature0pSignal->Fill(1.5); hMeanCurvatureNpBackground->Fill(0); }
        if ((numVisibleProtons > 0) && (protonCount == 0))  { hMeanCurvatureNpSignal->Fill(0.5); hMeanCurvature0pBackground->Fill(1); }
        if ((numVisibleProtons > 0) && (protonCount > 0))   hMeanCurvatureNpSignal->Fill(1.5);
    } else {
        if (protonCount == 0)     hMeanCurvature0pBackground->Fill(backgroundType);
        else if (protonCount > 0) hMeanCurvatureNpBackground->Fill(backgroundType);
    }

    // If we got to here, we passed all selection criteria
    if (bVerbose) std::cout << "Found pion absorption event with:"  << std::endl;
    if (bVerbose) std::cout << "    Number of protons: " << protonCount << std::endl;
    if (bVerbose) std::cout << std::endl;
    PionAbsTree->Fill();

    // Fill final histograms
    if (protonCount == 0) hFinalRecoEvents->Fill(0.5);
    if (protonCount > 0)  hFinalRecoEvents->Fill(1.5);
    if (isPionAbsorptionSignal) {
        if ((numVisibleProtons == 0) && (protonCount == 0)) hFinalRecoEvents0pSignal->Fill(0.5);
        if ((numVisibleProtons == 0) && (protonCount > 0))  { hFinalRecoEvents0pSignal->Fill(1.5); hFinalRecoEventsNpBackground->Fill(0); }
        if ((numVisibleProtons > 0) && (protonCount == 0))  { hFinalRecoEventsNpSignal->Fill(0.5); hFinalRecoEvents0pBackground->Fill(1); }
        if ((numVisibleProtons > 0) && (protonCount > 0))   hFinalRecoEventsNpSignal->Fill(1.5);
    } else {
        if (protonCount == 0)     hFinalRecoEvents0pBackground->Fill(backgroundType);
        else if (protonCount > 0) hFinalRecoEventsNpBackground->Fill(backgroundType);
    }

    return true;
}

void PionAbsorptionSelection::reconfigure(fhicl::ParameterSet const &p) {
    bVerbose = p.get<bool>("Verbose", false);
    strWCTrackBuilderLabel             = p.get<std::string>("WCTrackBuilderLabel", "wctrack");
    strTPCTrackHandleLabel             = p.get<std::string>("TPCTrackHandleLabel", "pmtrack");
    strWC2TPCModuleLabel               = p.get<std::string>("WC2TPModuleLabel", "wctracktpctrackmatch");
    strCalorimetryModuleLabel          = p.get<std::string>("CalorimetryModuleLabel", "calo");
    simulation_producer_label_         = p.get<std::string>("SimulationLabel", "largeant");
    recotrackmcparticlematching_label_ = p.get<std::string>("RecoTrackMCMatchLabel", "recotrackmcmatching");

    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 20);
    fMeanDEDXThreshold                 = p.get<double>("MeanDEDXThreshold", 5.0);
    fVertexRadius                      = p.get<double>("VertexRadius", 4);
    SmallTrackLength                   = p.get<double>("SmallTrackLength", 35);
    MaxSmallTracks                     = p.get<int>("MaxSmallTracks", 5);
    MeanCurvatureThreshold             = p.get<double>("MeanCurvatureThreshold", 0.015);
    PROTON_ENERGY_LOWER_BOUND          = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND          = p.get<float>("ProtonEnergyUpperBound", 1.0);
}

void PionAbsorptionSelection::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    art::ServiceHandle<art::TFileService> tfs;

    // Make histograms and tree branches
    hTotalEvents         = tfs->make<TH1D>("hTotalEvents", "hTotalEvents", 1, 0, 1);
    hTotalEvents0pSignal = tfs->make<TH1D>("hTotalEvents0pSignal", "hTotalEvents0pSignal", 1, 0, 1);
    hTotalEventsNpSignal = tfs->make<TH1D>("hTotalEventsNpSignal", "hTotalEventsNpSignal", 1, 0, 1);
    hTotalBackground     = tfs->make<TH1D>("hTotalBackground", "hTotalBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hWCExists           = tfs->make<TH1D>("hWCExists", "hWCExists", 1, 0, 1);
    hWCExists0pSignal   = tfs->make<TH1D>("hWCExists0pSignal", "hWCExists0pSignal", 1, 0, 1);
    hWCExistsNpSignal   = tfs->make<TH1D>("hWCExistsNpSignal", "hWCExistsNpSignal", 1, 0, 1);
    hWCExistsBackground = tfs->make<TH1D>("hWCExistsBackground", "hWCExistsBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hPionInRedVolume           = tfs->make<TH1D>("hPionInRedVolume", "hPionInRedVolume", 1, 0, 1);
    hPionInRedVolume0pSignal   = tfs->make<TH1D>("hPionInRedVolume0pSignal", "hPionInRedVolume0pSignal", 1, 0, 1);
    hPionInRedVolumeNpSignal   = tfs->make<TH1D>("hPionInRedVolumeNpSignal", "hPionInRedVolumeNpSignal", 1, 0, 1);
    hPionInRedVolumeBackground = tfs->make<TH1D>("hPionInRedVolumeBackground", "hPionInRedVolumeBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hNoOutgoingPion             = tfs->make<TH1D>("hNoOutgoingPion", "hNoOutgoingPion", 2, 0, 2);
    hNoOutgoingPion0pSignal     = tfs->make<TH1D>("hNoOutgoingPion0pSignal", "hNoOutgoingPion0pSignal", 2, 0, 2);
    hNoOutgoingPionNpSignal     = tfs->make<TH1D>("hNoOutgoingPionNpSignal", "hNoOutgoingPionNpSignal", 2, 0, 2);
    hNoOutgoingPion0pBackground = tfs->make<TH1D>("hNoOutgoingPion0pBackground", "hNoOutgoingPion0pBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);
    hNoOutgoingPionNpBackground = tfs->make<TH1D>("hNoOutgoingPionNpBackground", "hNoOutgoingPionNpBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hSmallTracks             = tfs->make<TH1D>("hSmallTracks", "hSmallTracks", 2, 0, 2);
    hSmallTracks0pSignal     = tfs->make<TH1D>("hSmallTracks0pSignal", "hSmallTracks0pSignal", 2, 0, 2); 
    hSmallTracksNpSignal     = tfs->make<TH1D>("hSmallTracksNpSignal", "hSmallTracksNpSignal", 2, 0, 2);
    hSmallTracks0pBackground = tfs->make<TH1D>("hSmallTracks0pBackground", "hSmallTracks0pBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);
    hSmallTracksNpBackground = tfs->make<TH1D>("hSmallTracksNpBackground", "hSmallTracksNpBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hMeanCurvature             = tfs->make<TH1D>("hMeanCurvature", "hMeanCurvature", 2, 0, 2);
    hMeanCurvature0pSignal     = tfs->make<TH1D>("hMeanCurvature0pSignal", "hMeanCurvature0pSignal", 2, 0, 2);
    hMeanCurvatureNpSignal     = tfs->make<TH1D>("hMeanCurvatureNpSignal", "hMeanCurvatureNpSignal", 2, 0, 2);
    hMeanCurvature0pBackground = tfs->make<TH1D>("hMeanCurvature0pBackground", "hMeanCurvature0pBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);
    hMeanCurvatureNpBackground = tfs->make<TH1D>("hMeanCurvatureNpBackground", "hMeanCurvatureNpBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    hFinalRecoEvents             = tfs->make<TH1D>("hFinalRecoEvents", "hFinalRecoEvents", 2, 0, 2);
    hFinalRecoEvents0pSignal     = tfs->make<TH1D>("hFinalRecoEvents0pSignal", "hFinalRecoEvents0pSignal", 2, 0, 2);
    hFinalRecoEventsNpSignal     = tfs->make<TH1D>("hFinalRecoEventsNpSignal", "hFinalRecoEventsNpSignal", 2, 0, 2);
    hFinalRecoEvents0pBackground = tfs->make<TH1D>("hFinalRecoEvents0pBackground", "hFinalRecoEvents0pBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);
    hFinalRecoEventsNpBackground = tfs->make<TH1D>("hFinalRecoEventsNpBackground", "hFinalRecoEventsNpBackground", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    PionAbsTree = tfs->make<TTree>("PionAbsorptionSelectionTree", "PionAbsorptionSelectionTree");

    PionAbsTree->Branch("Run", &run, "run/I");
    PionAbsTree->Branch("Subrun", &subrun, "subrun/I");
    PionAbsTree->Branch("Event", &event, "event/I");

    PionAbsTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    PionAbsTree->Branch("wcMatchPDG", &wcMatchPDG, "wcMatchPDG/I");
    PionAbsTree->Branch("wcMatchProcess", "std::string", &wcMatchProcess);
    PionAbsTree->Branch("wcMatchDaughtersPDG", "std::vector<int>", &wcMatchDaughtersPDG);
    PionAbsTree->Branch("wcMatchDaughtersProcess", "std::vector<std::string>", &wcMatchDaughtersProcess);

    PionAbsTree->Branch("truthPrimaryPDG", &truthPrimaryPDG, "truthPrimaryPDG/I");
    PionAbsTree->Branch("truthPrimaryVertexX", &truthPrimaryVertexX, "truthPrimaryVertexX/F");
    PionAbsTree->Branch("truthPrimaryVertexY", &truthPrimaryVertexY, "truthPrimaryVertexY/F");
    PionAbsTree->Branch("truthPrimaryVertexZ", &truthPrimaryVertexZ, "truthPrimaryVertexZ/F");
    PionAbsTree->Branch("truthPrimaryDaughtersPDG", "std::vector<int>", &truthPrimaryDaughtersPDG);
    PionAbsTree->Branch("truthPrimaryDaughtersProcess", "std::vector<std::string>", &truthPrimaryDaughtersProcess);
    PionAbsTree->Branch("truthPrimaryDaughtersKE", "std::vector<double>", &truthPrimaryDaughtersKE);

    PionAbsTree->Branch("isPionAbsorptionSignal", &isPionAbsorptionSignal, "isPionAbsorptionSignal/O");
    PionAbsTree->Branch("numVisibleProtons", &numVisibleProtons, "numVisibleProtons/I");
    PionAbsTree->Branch("backgroundType", &backgroundType, "backgroundType/I");

    PionAbsTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    PionAbsTree->Branch("WC2TPCPionBeginX", &WC2TPCPionBeginX, "WC2TPCPionBeginX/D");
    PionAbsTree->Branch("WC2TPCPionBeginY", &WC2TPCPionBeginY, "WC2TPCPionBeginY/D");
    PionAbsTree->Branch("WC2TPCPionBeginZ", &WC2TPCPionBeginZ, "WC2TPCPionBeginZ/D");
    PionAbsTree->Branch("WC2TPCPionEndX", &WC2TPCPionEndX, "WC2TPCPionEndX/D");
    PionAbsTree->Branch("WC2TPCPionEndY", &WC2TPCPionEndY, "WC2TPCPionEndY/D");
    PionAbsTree->Branch("WC2TPCPionEndZ", &WC2TPCPionEndZ, "WC2TPCPionEndZ/D");
    PionAbsTree->Branch("WC2TPCPionLength", &WC2TPCPionLength, "WC2TPCPionLength/D");

    PionAbsTree->Branch("protonCount", &protonCount, "protonCount/I");
    PionAbsTree->Branch("protonBeginX", "std::vector<double>", &protonBeginX);
    PionAbsTree->Branch("protonBeginY", "std::vector<double>", &protonBeginY);
    PionAbsTree->Branch("protonBeginZ", "std::vector<double>", &protonBeginZ);
    PionAbsTree->Branch("protonEndX", "std::vector<double>", &protonEndX);
    PionAbsTree->Branch("protonEndY", "std::vector<double>", &protonEndY);
    PionAbsTree->Branch("protonEndZ", "std::vector<double>", &protonEndZ);
    PionAbsTree->Branch("protonTrkID", "std::vector<double>", &protonTrkID);
    PionAbsTree->Branch("protonLength", "std::vector<double>", &protonLength);
    PionAbsTree->Branch("isProtonInverted", "std::vector<bool>", &isProtonInverted);
    PionAbsTree->Branch("isProtonStopping", "std::vector<bool>", &isProtonStopping);
    PionAbsTree->Branch("protonTrueProcess", "std::vector<std::string>", &protonTrueProcess);
}

void PionAbsorptionSelection::endJob() {
    std::cout << std::endl;
    int total0pSignalEvents = hTotalEvents0pSignal->Integral();
    int totalNpSignalEvents = hTotalEventsNpSignal->Integral();
    int totalSignalEvents   = total0pSignalEvents + totalNpSignalEvents;
    std::cout << "Total events: " << hTotalEvents->Integral() << std::endl;
    std::cout << "Total signal events: " << totalSignalEvents << std::endl;
    std::cout << "Total 0p signal events: " << total0pSignalEvents << std::endl;
    std::cout << "Total Np signal events: " << totalNpSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "Cut statistics:" << std::endl;
    std::cout << "  WC to TPC match: " << std::endl;
    std::cout << "    Total reco events: " << hWCExists->Integral() << std::endl;
    std::cout << "    0p reco signal events: " << hWCExists0pSignal->Integral() << std::endl;
    std::cout << "    Np reco signal events: " << hWCExistsNpSignal->Integral() << std::endl;
    std::cout << "    Overall purity: " << (hWCExists0pSignal->Integral() + hWCExistsNpSignal->Integral()) / hWCExists->Integral() << " and efficiency: " << (hWCExists0pSignal->Integral() + hWCExistsNpSignal->Integral()) / totalSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "  Pion vertex in reduced volume: " << std::endl;
    std::cout << "    Total reco events: " << hPionInRedVolume->Integral() << std::endl;
    std::cout << "    0p signal events: " << hPionInRedVolume0pSignal->Integral() << std::endl;
    std::cout << "    Np signal events: " << hPionInRedVolumeNpSignal->Integral() << std::endl;
    std::cout << "    Overall purity: " << (hPionInRedVolume0pSignal->Integral() + hPionInRedVolumeNpSignal->Integral()) / hPionInRedVolume->Integral() << " and efficiency: " << (hPionInRedVolume0pSignal->Integral() + hPionInRedVolumeNpSignal->Integral()) / totalSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "  No outgoing pion: " << std::endl;
    std::cout << "    Total reco events: " << hNoOutgoingPion->Integral() << std::endl;
    std::cout << "    0p signal events: " << hNoOutgoingPion0pSignal->Integral() << std::endl;
    std::cout << "    Np signal events: " <<  hNoOutgoingPionNpSignal->Integral() << std::endl;
    std::cout << "    Overall purity: " << (hNoOutgoingPion0pSignal->Integral() + hNoOutgoingPionNpSignal->Integral()) / hNoOutgoingPion->Integral() << " and efficiency: " << (hNoOutgoingPion0pSignal->Integral() + hNoOutgoingPionNpSignal->Integral()) / totalSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco 0p events: " << hNoOutgoingPion->Integral(1, 1) << std::endl;
    std::cout << "    Reco 0p true signal events: " << hNoOutgoingPion0pSignal->Integral(1, 1) << std::endl;
    std::cout << "    0p purity: " << hNoOutgoingPion0pSignal->Integral(1, 1) / hNoOutgoingPion->Integral(1,1) << " and efficiency: " << hNoOutgoingPion0pSignal->Integral(1, 1) / total0pSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco Np events: " << hNoOutgoingPion->Integral(2, 2) << std::endl;
    std::cout << "    Reco Np true signal events: " << hNoOutgoingPionNpSignal->Integral(2, 2) << std::endl;
    std::cout << "    Np purity: " << hNoOutgoingPionNpSignal->Integral(2, 2) / hNoOutgoingPion->Integral(2,2) << " and efficiency: " << hNoOutgoingPionNpSignal->Integral(2, 2) / totalNpSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "  No small tracks: " << std::endl;
    std::cout << "    Total reco events: " << hSmallTracks->Integral() << std::endl;
    std::cout << "    0p signal events: " << hSmallTracks0pSignal->Integral() << std::endl;
    std::cout << "    Np signal events: " <<  hSmallTracksNpSignal->Integral() << std::endl;
    std::cout << "    Overall purity: " << (hSmallTracks0pSignal->Integral() + hSmallTracksNpSignal->Integral()) / hSmallTracks->Integral() << " and efficiency: " << (hSmallTracks0pSignal->Integral() + hSmallTracksNpSignal->Integral()) / totalSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco 0p events: " << hSmallTracks->Integral(1, 1) << std::endl;
    std::cout << "    Reco 0p true signal events: " << hSmallTracks0pSignal->Integral(1,1) << std::endl;
    std::cout << "    0p purity: " << hSmallTracks0pSignal->Integral(1, 1) / hSmallTracks->Integral(1, 1) << " and efficiency: " << hSmallTracks0pSignal->Integral(1, 1) / total0pSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco Np events: " << hSmallTracks->Integral(2, 2) << std::endl;
    std::cout << "    Reco Np true signal events: " << hSmallTracksNpSignal->Integral(2,2) << std::endl;
    std::cout << "    Np purity: " << hSmallTracksNpSignal->Integral(2, 2) / hSmallTracks->Integral(2, 2) << " and efficiency: " << hSmallTracksNpSignal->Integral(2, 2) / totalNpSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "  Curvature: " << std::endl;
    std::cout << "    Total reco events: " << hMeanCurvature->Integral() << std::endl;
    std::cout << "    0p signal events: " << hMeanCurvature0pSignal->Integral() << std::endl;
    std::cout << "    Np signal events: " <<  hMeanCurvatureNpSignal->Integral() << std::endl;
    std::cout << "    Overall purity: " << (hMeanCurvature0pSignal->Integral() + hMeanCurvatureNpSignal->Integral()) / hMeanCurvature->Integral() << " and efficiency: " << (hMeanCurvature0pSignal->Integral() + hMeanCurvatureNpSignal->Integral()) / totalSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco 0p events: " << hMeanCurvature->Integral(1, 1) << std::endl;
    std::cout << "    Reco 0p true signal events: " << hMeanCurvature0pSignal->Integral(1,1) << std::endl;
    std::cout << "    0p purity: " << hMeanCurvature0pSignal->Integral(1, 1) / hMeanCurvature->Integral(1, 1) << " and efficiency: " << hMeanCurvature0pSignal->Integral(1, 1) / total0pSignalEvents << std::endl;
    std::cout << std::endl;
    std::cout << "    Reco Np events: " << hMeanCurvature->Integral(2, 2) << std::endl;
    std::cout << "    Reco Np true signal events: " << hMeanCurvatureNpSignal->Integral(2,2) << std::endl;
    std::cout << "    Np purity: " << hMeanCurvatureNpSignal->Integral(2, 2) / hMeanCurvature->Integral(2, 2) << " and efficiency: " << hMeanCurvatureNpSignal->Integral(2, 2) / totalNpSignalEvents << std::endl;
    std::cout << std::endl;

    int totalFinalSignal0p = hFinalRecoEvents0pSignal->Integral();
    int totalFinalSignalNp = hFinalRecoEventsNpSignal->Integral();
    int totalFinalSignal   = totalFinalSignal0p + totalFinalSignalNp;
    std::cout << "Final stats: " << std::endl;
    std::cout << "  Reco events: " << hFinalRecoEvents->Integral() << std::endl;
    std::cout << "  Overall purity: " << totalFinalSignal / hFinalRecoEvents->Integral() << " and efficiency: " << (double)totalFinalSignal / totalSignalEvents << std::endl;
    std::cout << "  Reco 0p events: " << hFinalRecoEvents->Integral(1,1) << std::endl;
    std::cout << "  0p channel purity: " << hFinalRecoEvents0pSignal->Integral(1,1) / hFinalRecoEvents->Integral(1,1) << " and efficiency: " << hFinalRecoEvents0pSignal->Integral(1,1) / total0pSignalEvents << std::endl;
    std::cout << "  Reco Np events: " << hFinalRecoEvents->Integral(2,2) << std::endl;
    std::cout << "  0p channel purity: " << hFinalRecoEventsNpSignal->Integral(2,2) / hFinalRecoEvents->Integral(2,2) << " and efficiency: " << hFinalRecoEventsNpSignal->Integral(2,2) / total0pSignalEvents << std::endl;
    std::cout << std::endl;
}

void PionAbsorptionSelection::resetTree() {
    WC2TPCtrkID = -99999;
    isPionReversed = false;
    wcMatchPDG = -99999;
    wcMatchProcess = "";
    wcMatchDaughtersPDG.clear();
    wcMatchDaughtersProcess.clear();

    truthPrimaryPDG = -99999;
    truthPrimaryDaughtersPDG.clear();
    truthPrimaryDaughtersProcess.clear();
    truthPrimaryDaughtersKE.clear();
    truthPrimaryVertexX = -99999;
    truthPrimaryVertexY = -99999;
    truthPrimaryVertexZ = -99999;

    protonCount = 0;
    protonBeginX.clear();
    protonBeginY.clear();
    protonBeginZ.clear();
    protonEndX.clear();
    protonEndY.clear();
    protonEndZ.clear();
    protonTrkID.clear();
    protonLength.clear();
    isProtonInverted.clear();
    isProtonStopping.clear();
    protonTrueProcess.clear();

    isPionAbsorptionSignal = false;
    numVisibleProtons      = 0;
    backgroundType         = -1;
}

void PionAbsorptionSelection::fillSignalInformation(
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

void PionAbsorptionSelection::fillBackgroundInformation(
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

std::tuple<double, double> PionAbsorptionSelection::computeCurvature(recob::Track track) {
    double meanCurvature = 0;
    double maxCurvature  = 0;
    for (size_t iPoint = 0; iPoint < track.NPoints() - 2; iPoint++) {
        recob::TrackTrajectory::Point_t p1_ = track.LocationAtPoint(iPoint);
        recob::TrackTrajectory::Point_t p2_ = track.LocationAtPoint(iPoint + 1);
        recob::TrackTrajectory::Point_t p3_ = track.LocationAtPoint(iPoint + 2);
        TVector3 p1, p2, p3;
        p1(0) = p1_.X(); p1(1) = p1_.Y(); p1(2) = p1_.Z();
        p2(0) = p2_.X(); p2(1) = p2_.Y(); p2(2) = p2_.Z();
        p3(0) = p3_.X(); p3(1) = p3_.Y(); p3(2) = p3_.Z();
  
        double curvatureAtPoint = curvatureForThreePoints(p1, p2, p3);
        meanCurvature += curvatureAtPoint / (track.NPoints() - 2);
        if (curvatureAtPoint > maxCurvature) maxCurvature = curvatureAtPoint;
    }
  
    return std::make_tuple(meanCurvature, maxCurvature);
}

double PionAbsorptionSelection::curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3) {
    // From: https://en.wikipedia.org/wiki/Circumcircle#Cartesian_coordinates_from_cross-_and_dot-products 
  
    // Edges of a triangle
    TVector3 t = p1 - p2;
    TVector3 u = p3 - p1;
    TVector3 v = p2 - p3;
  
    // Normal to the triangle
    TVector3 w = t.Cross(v);
  
    double tt = TMath::Sqrt(t * t);
    double uu = TMath::Sqrt(u * u);
    double vv = TMath::Sqrt(v * v);
    double ww = TMath::Sqrt(w * w);
  
    // If area of triangle is too small, no curvature
    if (ww < 10e-14) return 0;
  
    return (2 * ww) / (tt * uu * vv);
  }

double PionAbsorptionSelection::meanDEDX(art::FindManyP<anab::Calorimetry> fmcal, unsigned int trackKey, bool isThisTrackReversed) {
    // Temporary storage for this reco track
    // TODO: add temporary vectors to tree vectors
    std::vector<double> recoPitch_v; 
    std::vector<double> recoDEDX_v;
    std::vector<double> recoEDep_v;
    std::vector<double> recoResR_v;
    std::vector<double> recoZPos_v;

    if (fmcal.isValid()) {
        // Get calorimetry for this track
        std::vector<art::Ptr<anab::Calorimetry>> calos = fmcal.at(trackKey);

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
    }

    // Compute and save mean dedx
    double meanDEDX = 0;
    unsigned int bound = MeanDEDXNumberTrajPoints;
    if (MeanDEDXNumberTrajPoints > recoDEDX_v.size()) bound = recoDEDX_v.size();
    for (unsigned int i = 0; i < bound; ++i) meanDEDX += recoDEDX_v.at(i);
    meanDEDX /= bound;
    
    return meanDEDX;
}

double PionAbsorptionSelection::distance(double x1, double x2, double y1, double y2, double z1, double z2) {
    return sqrt(
        pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)
    );
}

bool PionAbsorptionSelection::isWithinActiveVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}

bool PionAbsorptionSelection::isWithinReducedVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}

bool PionAbsorptionSelection::isWithinReducedVolume(simb::MCParticle *track) {
    return (
        (track->EndX() > RminX) && (track->EndX() < RmaxX) && 
        (track->EndY() > RminY) && (track->EndY() < RmaxY) && 
        (track->EndZ() > RminZ) && (track->EndZ() < RmaxZ)
    );
}

int PionAbsorptionSelection::lastPointInTPC(simb::MCParticle *track) {
    for (int i=track->NumberTrajectoryPoints()-1; i >=0; i--) {
        if (
            (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
            (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
            (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
        ) return i; // If present in TPC, return last point
    }
    return 9999;
}

DEFINE_ART_MODULE(PionAbsorptionSelection)