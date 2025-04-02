//////////////////////////////////////////////////////////////////////////
// Class:       ShowerRecoEval
// Module Type: analyzer
// File:        ShowerRecoEval_module.cc
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
#include "TMatrixD.h"
#include "TDecompSVD.h"
#include "TVectorD.h"

// ####################
// ### C++ includes ###
// ####################
#include <map>
#include <memory>
#include <fstream>
#include <tuple>
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

class ShowerRecoEval : public art::EDAnalyzer {
    public: 
        explicit ShowerRecoEval(fhicl::ParameterSet const &p);
        ShowerRecoEval(ShowerRecoEval const &) = delete;
        ShowerRecoEval(ShowerRecoEval &&) = delete;
        ShowerRecoEval & operator = (ShowerRecoEval const &) = delete;
        ShowerRecoEval & operator = (ShowerRecoEval &&) = delete;

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
        double trackMagnitude(const art::Ptr<simb::MCParticle> track);
        double trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2);
        double trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2);
        bool isWithinActiveVolume(double x, double y, double z);
        bool isWithinReducedVolume(double x, double y, double z);
        std::tuple<double, double> computeCurvature(recob::Track track);
        double curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3);
        void fillSignalInformation(int pdg, float vx, float vy, float vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);

    private: 
        // Produce's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string simulation_producer_label_;
        std::string recotrackmcparticlematching_label_;

        // File for saving event data
        std::ofstream outFile;

        // fcl parameters
        bool bVerbose;
        unsigned int MeanDEDXNumberTrajPoints;
        double TrackStitchingThreshold;
        float PROTON_ENERGY_LOWER_BOUND;
        float PROTON_ENERGY_UPPER_BOUND;

        // Output tree
        TTree *ShowerRecoEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;

        // Truth information
        int primaryParticleTrackID;
        int primaryParticlePDG;
        int numEmmitedElectrons = 0;
        int numEmmitedPhotons = 0;
        std::vector<double> truthElectronsLength;

        // Signal information
        bool isPionAbsorptionSignal;
        int  numVisibleProtons;

        // WC variables
        int WC2TPCtrkID = -99999;
        double WCTrackMomentum;
        double WC2TPCPrimaryBeginX;
        double WC2TPCPrimaryBeginY;
        double WC2TPCPrimaryBeginZ;

        // Reco variables 
        std::vector<double> recoLength;
        std::vector<int> recoTrkID;
        double WCMeanCurvature;
        double WCMaxCurvature;

        // Truth variables for particles matched to tracks
        std::vector<int>    matchedIdentity;
        std::vector<int>    matchedTrkID;
        std::vector<std::string> matchedProcess;
        std::vector<double> matchedLength;

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

ShowerRecoEval::ShowerRecoEval(fhicl::ParameterSet const &p) : EDAnalyzer(p) {
    this->reconfigure(p);
}

void ShowerRecoEval::analyze(art::Event const &e) {
    resetTree();

    run = e.run(); subrun = e.subRun(); event = e.event();

    if (event == 200004) {
      outFile << "Event: " << event << std::endl;
    }

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
    if (numWCtrks != 1) return; 

    // Get wcTrack momentum
    WCTrackMomentum = wctrack[0]->Momentum() * 0.001; // Mev to GeV

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
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();

    // Pointer to assign primary particle to
    simb::MCParticle *primaryParticle = NULL;
    std::vector<simb::MCParticle*> primaryDaughters; 
    std::vector<int>               primaryDaughtersTrackIds;

    // Loop over all Geant4 particles using the BackTracker
    for (size_t p = 0; p < plist.size(); ++p) {
        auto mcPart = plist.Particle(p);

        // Get true particle
        std::string particleProcess = mcPart->Process();

        // If particle not primary, continue
        if (!(particleProcess.find("primary") != std::string::npos)) continue;

        primaryParticle = mcPart; // assign primary particle pointer
        primaryParticlePDG = primaryParticle->PdgCode();
        primaryParticleTrackID = primaryParticle->TrackId();
        if (bVerbose) std::cout << "Found primary particle!" << std::endl;
        if (bVerbose) std::cout << std::endl;

        // Get primary daughters
        int numDaughters = primaryParticle->NumberDaughters();
        std::vector<int>         primaryParticleDaughtersPDG;
        std::vector<std::string> primaryParticleDaughtersProcess;
        std::vector<double>      primaryParticleDaughtersKE;

        for (int iDaughter = 0; iDaughter < numDaughters; ++iDaughter) {
            int thisDaughterTrackId = primaryParticle->Daughter(iDaughter);
            simb::MCParticle *thisDaughter = NULL;

            // Grab daughter with thisDaughterTrackId
            for (size_t q = 0; q < plist.size(); ++q) {
                if (plist.Particle(q)->TrackId() == thisDaughterTrackId) {
                    thisDaughter = plist.Particle(q);
                    break;
                }
            }

            if (thisDaughter->Process() == "eBrem" && thisDaughter->PdgCode() == 22) numEmmitedPhotons++;
            if (thisDaughter->Process() == "eIoni" && thisDaughter->PdgCode() == 11) {
              truthElectronsLength.push_back(trackMagnitude(thisDaughter));
              numEmmitedElectrons++;
            }

            primaryParticleDaughtersPDG.push_back(thisDaughter->PdgCode());
            primaryParticleDaughtersProcess.push_back(thisDaughter->Process());
            primaryParticleDaughtersKE.push_back(thisDaughter->E() - thisDaughter->Mass());
        } // end daughters loop
        
        fillSignalInformation(
          primaryParticlePDG,
          primaryParticle->EndX(), 
          primaryParticle->EndY(), 
          primaryParticle->EndZ(),
          primaryParticleDaughtersPDG,
          primaryParticleDaughtersProcess,
          primaryParticleDaughtersKE
        );

        break; // Break once we find primary particle
    } // end Geant4 particle loop

    // If for some reason we did not find the primary particle, flag and return
    if (primaryParticle == NULL) {
        std::cout << "WARNING: primary particle not found, aborting event" << std::endl;
        return;
    }

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

    // Loop over tracks
    for (size_t trkIdx = 0; trkIdx < tpcTrackHandle->size(); ++trkIdx) {
        // Now, we loop over the TPC tracks

        // Get MCParticle objects and metadata
        std::vector<art::Ptr<simb::MCParticle>> const& particles = find_many_mcparticles_from_tracks.at(trkIdx);
        std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trkIdx);

        // Get TPC track
        auto thisTrack = tracklist.at(trkIdx);

        // Track matched to WC
        if (thisTrack->ID() == WC2TPCtrkID) {
          // Compute curvature measure
          auto [meanCurvature, maxCurvature] = computeCurvature(*thisTrack);
          WCMeanCurvature = meanCurvature;
          WCMaxCurvature  = maxCurvature;
        }

        recob::TrackTrajectory::Point_t recoBeginning = thisTrack->Start();
        recob::TrackTrajectory::Point_t recoEnd = thisTrack->End();

        // Fill in data to vectors for tree
        double thisTrackLength = sqrt(
            pow(recoBeginning.X() - recoEnd.X(), 2) + 
            pow(recoBeginning.Y() - recoEnd.Y(), 2) +
            pow(recoBeginning.Z() - recoEnd.Z(), 2)
        );
        recoLength.push_back(thisTrackLength);
        recoTrkID.push_back(thisTrack->ID());

        if (event == 200004) {
          outFile << "Starting new reco track:" << std::endl;
          outFile << "    Start X: " << recoBeginning.X() << std::endl;
          outFile << "    Start Y: " << recoBeginning.Y() << std::endl;
          outFile << "    Start Z: " << recoBeginning.Z() << std::endl;
          outFile << "    End X: " << recoEnd.X() << std::endl;
          outFile << "    End Y: " << recoEnd.Y() << std::endl;
          outFile << "    End Z: " << recoEnd.Z() << std::endl;
          outFile << "    Length: " << thisTrackLength << std::endl;
        }
        // If reco track is not matched to anything, continue
        if (btdata_vector.size() == 0) continue;

        // Get MCParticle object and data
        auto const& particle = particles.front();
        int const pdg_code   = particle->PdgCode();
        int const g4_trk_id  = particle->TrackId();
        std::string process  = particle->Process();

        if (event == 200004) {
          outFile << "  Truth match info:" << std::endl;
          outFile << "    PDG: " << pdg_code << std::endl;
          outFile << "    Process: " << process << std::endl;
          outFile << "    Length: " << trackMagnitude(particle) << std::endl;
        }

        // At this point, we have:
        //     particle: matched MCParticle
        //     thisTrack: reconstructed track

        matchedIdentity.push_back(pdg_code);
        matchedLength.push_back(trackMagnitude(particle));
        matchedTrkID.push_back(g4_trk_id);
        matchedProcess.push_back(process);

        if (event == 200004) {
          outFile << "Ending track." << std::endl;
          outFile << std::endl;
        }
    } // end loop over tracks

    if (bVerbose) std::cout << std::endl;
    ShowerRecoEvalTree->Fill();
}

void ShowerRecoEval::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    outFile.open("Events.txt");
  
    art::ServiceHandle<art::TFileService> tfs;

    // Make histograms and tree branches
    ShowerRecoEvalTree = tfs->make<TTree>("ShowerRecoEvalTree", "ShowerRecoEvalTree");

    ShowerRecoEvalTree->Branch("run", &run, "run/I");
    ShowerRecoEvalTree->Branch("subrun", &subrun, "subrun/I");
    ShowerRecoEvalTree->Branch("event", &event, "event/I");

    ShowerRecoEvalTree->Branch("primaryParticleTrackID", &primaryParticleTrackID, "primaryParticleTrackID/I");
    ShowerRecoEvalTree->Branch("primaryParticlePDG", &primaryParticlePDG, "primaryParticlePDG/I");
    ShowerRecoEvalTree->Branch("numEmmitedElectrons", &numEmmitedElectrons, "numEmmitedElectrons/I");
    ShowerRecoEvalTree->Branch("numEmmitedPhotons", &numEmmitedPhotons, "numEmmitedPhotons/I");
    ShowerRecoEvalTree->Branch("truthElectronsLength", "std::vector<double>", &truthElectronsLength);

    ShowerRecoEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    ShowerRecoEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    ShowerRecoEvalTree->Branch("WC2TPCPrimaryBeginX", &WC2TPCPrimaryBeginX, "WC2TPCPrimaryBeginX/D");
    ShowerRecoEvalTree->Branch("WC2TPCPrimaryBeginY", &WC2TPCPrimaryBeginY, "WC2TPCPrimaryBeginY/D");
    ShowerRecoEvalTree->Branch("WC2TPCPrimaryBeginZ", &WC2TPCPrimaryBeginZ, "WC2TPCPrimaryBeginZ/D");

    ShowerRecoEvalTree->Branch("recoTrkID", "std::vector<int>", &recoTrkID);
    ShowerRecoEvalTree->Branch("recoLength", "std::vector<double>", &recoLength);
    ShowerRecoEvalTree->Branch("WCMeanCurvature", &WCMeanCurvature, "WCMeanCurvature/D");
    ShowerRecoEvalTree->Branch("WCMaxCurvature", &WCMaxCurvature, "WCMaxCurvature/D");

    ShowerRecoEvalTree->Branch("isPionAbsorptionSignal", &isPionAbsorptionSignal, "isPionAbsorptionSignal/O");
    ShowerRecoEvalTree->Branch("numVisibleProtons", &numVisibleProtons, "numVisibleProtons/I");

    ShowerRecoEvalTree->Branch("matchedIdentity", "std::vector<int>", &matchedIdentity);
    ShowerRecoEvalTree->Branch("matchedTrkID", "std::vector<int>", &matchedTrkID);
    ShowerRecoEvalTree->Branch("matchedProcess", "std::vector<std::string>", &matchedProcess);
    ShowerRecoEvalTree->Branch("matchedLength", "std::vector<double>", &matchedLength);
}

unsigned int ShowerRecoEval::lastPointInTPC(simb::MCParticle *track)
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

unsigned int ShowerRecoEval::lastPointInTPC(const art::Ptr<simb::MCParticle> track)
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

unsigned int ShowerRecoEval::firstPointInTPC(simb::MCParticle *track)
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

unsigned int ShowerRecoEval::firstPointInTPC(const art::Ptr<simb::MCParticle> track)
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

bool ShowerRecoEval::isPosterityOfPrimary(simb::MCParticle *particle, const sim::ParticleList& plist) {
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

std::tuple<double, double> ShowerRecoEval::computeCurvature(recob::Track track) {
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

double ShowerRecoEval::curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3) {
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

void ShowerRecoEval::fillSignalInformation(
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
      if ((daughtersProcess[iDaughter] == "Decay") || (daughtersProcess[iDaughter] == "hBertiniCaptureAtRest")) return;

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


double ShowerRecoEval::trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2)
{
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double ShowerRecoEval::trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2)
{
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double ShowerRecoEval::trackMagnitude(simb::MCParticle *track)
{
    return sqrt(
        pow(track->EndX()-track->Vx(0),2) + 
        pow(track->EndY()-track->Vy(0),2) + 
        pow(track->EndZ()-track->Vz(0),2)
    );
}

double ShowerRecoEval::trackMagnitude(const art::Ptr<simb::MCParticle> track) {
    return sqrt(
        pow(track->EndX()-track->Vx(0),2) + 
        pow(track->EndY()-track->Vy(0),2) + 
        pow(track->EndZ()-track->Vz(0),2)
    );
}

bool ShowerRecoEval::isWithinActiveVolume(double x, double y, double z) {
    if (x < minX ) return false; 
    if (x > maxX ) return false;
    if (y < minY ) return false; 
    if (y > maxY ) return false;
    if (z < minZ ) return false; 
    if (z > maxZ ) return false;
    return true;
}

bool ShowerRecoEval::isWithinReducedVolume(double x, double y, double z) {
  return (
      (x > RminX) && (x < RmaxX) && 
      (y > RminY) && (y < RmaxY) && 
      (z > RminZ) && (z < RmaxZ)
  );
}

void ShowerRecoEval::resetTree() {
    numEmmitedElectrons = 0;
    numEmmitedPhotons = 0;
    truthElectronsLength.clear();
    
    recoLength.clear();
    recoTrkID.clear();
    
    matchedIdentity.clear();
    matchedTrkID.clear();
    matchedProcess.clear();
    matchedLength.clear();

    WCMeanCurvature = -99999;
    WCMaxCurvature  = -99999;
    WC2TPCtrkID = -99999;

    isPionAbsorptionSignal = false;
    numVisibleProtons = 0;
}

void ShowerRecoEval::endJob() {
    
}

void ShowerRecoEval::reconfigure(fhicl::ParameterSet const & p) {
    bVerbose = p.get<bool>("Verbose", false);
    strWCTrackBuilderLabel             = p.get<std::string>("WCTrackBuilderLabel", "wctrack");
    strTPCTrackHandleLabel             = p.get<std::string>("TPCTrackHandleLabel", "pmtrack");
    strWC2TPCModuleLabel               = p.get<std::string>("WC2TPModuleLabel", "wctracktpctrackmatch");
    strCalorimetryModuleLabel          = p.get<std::string>("CalorimetryModuleLabel", "calo");
    simulation_producer_label_         = p.get<std::string>("SimulationLabel", "largeant");
    recotrackmcparticlematching_label_ = p.get<std::string>("RecoTrackMCMatchLabel", "recotrackmcmatching");
    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 60);
    TrackStitchingThreshold            = p.get<double> ("TrackStitchingThreshold",4);
    PROTON_ENERGY_LOWER_BOUND          = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND          = p.get<float>("ProtonEnergyUpperBound", 1.0);
}

DEFINE_ART_MODULE(ShowerRecoEval)