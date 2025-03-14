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
        double distance(double x1, double x2, double y1, double y2, double z1, double z2);
        void fillSignalInformation(int pdg, float vx, float vy, float vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);

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
        float        PROTON_ENERGY_LOWER_BOUND;
        float        PROTON_ENERGY_UPPER_BOUND;

        // Cut variables
        double fMeanDEDXThreshold;

        // Output tree
        TTree* PionAbsTree;

        // Event metadata
        int run;
        int subrun;
        int event;

        // Statistics
        int totalEventCount = 0;
        int pionVertexInRedVolEventCount = 0;
        int WC2TPCTrackBecomesProton = 0;
        int onlyOutgoingProtonsEventCount = 0;
        int showerEvents = 0;

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
    totalEventCount++;

    // Get event metadata
    run = e.run(); subrun = e.subRun(); event = e.event();
    if (bVerbose) std::cout << "Run: " << run << ", subrun: " << subrun << ", event: " << event << std::endl;
    if (bVerbose) std::cout << std::endl;

    // First, we get all the data products we are going to need to make cuts

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

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

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
    // Selection algorithm
    //////////////////////

    // Check that there is a WC to TPC match, and check if pion stops inside fiducial volume
    if (WC2TPCtrkID != -99999) {
        // Identify pion among reco tracks
        for (size_t trk_idx = 0; trk_idx < tpcTrackHandle -> size(); ++trk_idx) {
            auto thisTrack = tracklist.at(trk_idx);
            recob::TrackTrajectory::Point_t recoWC2TPCBeginning;
            recob::TrackTrajectory::Point_t recoWC2TPCEnd;

            // Check if this track matches ID of particle matched to WC (i.e., pion)
            if (thisTrack->ID() == WC2TPCtrkID) {
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

                // Check pion track end is inside fiducial volume
                if (!(isWithinReducedVolume(WC2TPCPionEndX, WC2TPCPionEndY, WC2TPCPionEndZ))) {
                    // We can have cases in which a proton going in the same (or close) direction to the pion 
                    // gets reconstructed as the same track; we identify these events with dE/dx
                    if (meanDEDX(fmcal, thisTrack.key(), isPionReversed) > fMeanDEDXThreshold) {
                        // TODO: do something else with these events?
                        WC2TPCTrackBecomesProton++;
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
            }
        }
    } // end of WC2TPCtrkID if statement
    else {
        // if no match
        return false;
    }

    // If we made it here, pion vertex is inside reduced volume
    pionVertexInRedVolEventCount++;

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
            if (thisMeanDEDX < fMeanDEDXThreshold) {
                // TODO: keep track of these events somewhere?
                // TODO: pion stitching?
                // TODO: do something different btw stopping and non-stopping protons?
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

    // Shower cut
    if (numSmallTracks > MaxSmallTracks) {
        showerEvents++;
        return false;
    }

    // We found an event with only outgoing protons (near pion vertex)
    onlyOutgoingProtonsEventCount++;

    // If we got to here, we passed all selection criteria
    if (bVerbose) std::cout << "Found pion absorption event with:"  << std::endl;
    if (bVerbose) std::cout << "    Number of protons: " << protonCount << std::endl;
    if (bVerbose) std::cout << std::endl;
    PionAbsTree->Fill();
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

    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 60);
    fMeanDEDXThreshold                 = p.get<double>("MeanDEDXThreshold", 3.4);
    fVertexRadius                      = p.get<double>("VertexRadius", 4);
    SmallTrackLength                   = p.get<double>("SmallTrackLength", 35);
    MaxSmallTracks                     = p.get<int>("MaxSmallTracks", 5);
    PROTON_ENERGY_LOWER_BOUND          = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND          = p.get<float>("ProtonEnergyUpperBound", 1.0);
}

void PionAbsorptionSelection::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    art::ServiceHandle<art::TFileService> tfs;

    // Make histograms and tree branches
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

    PionAbsTree->Branch("totalEventCount", &totalEventCount, "totalEventCount/I");
    PionAbsTree->Branch("pionVertexInRedVolEventCount", &pionVertexInRedVolEventCount, "pionVertexInRedVolEventCount/I");
    PionAbsTree->Branch("WC2TPCTrackBecomesProton", &WC2TPCTrackBecomesProton, "WC2TPCTrackBecomesProton/I");
    PionAbsTree->Branch("onlyOutgoingProtonsEventCount", &onlyOutgoingProtonsEventCount, "onlyOutgoingProtonsEventCount/I");
    PionAbsTree->Branch("showerEvents", &showerEvents, "showerEvents/I");

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
    std::cout << "Cut statistics:" << std::endl;
    std::cout << "Total event count: " << totalEventCount << std::endl;
    std::cout << "Events with pion vertex in fiducial volume: " << pionVertexInRedVolEventCount << std::endl;
    std::cout << "Events with only protons coming out of vertex: " << onlyOutgoingProtonsEventCount << std::endl;
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
    numVisibleProtons = 0;
}

void PionAbsorptionSelection::fillSignalInformation(
    int pdg,
    float vx, float vy, float vz,
    std::vector<int> daughtersPDG, 
    std::vector<std::string> daughtersProcess, 
    std::vector<double> daughtersKE
) {
    if (pdg != -211) return;
    if (!isWithinReducedVolume(vx, vy, vz)) return;

    int numDaughters = daughtersPDG.size();
    int tempNumProtons = 0;
    for (int iDaughter = 0; iDaughter < numDaughters; iDaughter++) {
        if ((daughtersPDG[iDaughter] == 11) && (daughtersProcess[iDaughter] == "hIoni")) continue;
        if ((daughtersPDG[iDaughter] == 111) || (daughtersPDG[iDaughter] == 211) || (daughtersPDG[iDaughter] == -211)) return;
        if (daughtersProcess[iDaughter] == "Decay") return;

        if (daughtersProcess[iDaughter] == "pi-Inelastic") {
            if ((daughtersPDG[iDaughter] == 13) || (daughtersPDG[iDaughter] == -13)) { return; } // muon
            else if ((daughtersPDG[iDaughter] == 321) || (daughtersPDG[iDaughter] == -321) || (daughtersPDG[iDaughter] == 311)) { return; } // kaon
            else if (daughtersPDG[iDaughter] == 2212) {
                if ((daughtersKE[iDaughter] >= PROTON_ENERGY_LOWER_BOUND) && (daughtersKE[iDaughter] <= PROTON_ENERGY_UPPER_BOUND)) {
                    tempNumProtons++;
                }
            }
        }
    }

    numVisibleProtons = tempNumProtons;
    isPionAbsorptionSignal = true;
    return;
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
            }
        } // end loop over planes
    }

    // TODO: take meanDEDX over last/first track points, currently doing over whole track
    // Compute and save mean dedx
    double meanDEDX = 0;
    unsigned int bound = 0;
    if (recoDEDX_v.size() > MeanDEDXNumberTrajPoints) {
        bound = (int)(recoDEDX_v.size() - MeanDEDXNumberTrajPoints);
    }
    for (unsigned int i = bound; i < recoDEDX_v.size(); ++i) meanDEDX += recoDEDX_v.at(i);
    meanDEDX /= (recoDEDX_v.size() - bound);
    
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