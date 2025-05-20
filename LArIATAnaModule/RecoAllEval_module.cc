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
#include "TLorentzVector.h"

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
        double meanDEDX(art::FindManyP<anab::Calorimetry> fmcal, unsigned int trackKey, bool isThisTrackReversed, std::vector<double>& trackDEDX, std::vector<double>& trackResR, std::vector<double>& trackXPos, std::vector<double>& trackYPos, std::vector<double>& trackZPos);
        double distance(double x1, double x2, double y1, double y2, double z1, double z2);
        double curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3);
        std::tuple<double, double> computeCurvature(recob::Track track);
        void fillSignalInformation(int pdg, double vx, double vy, double vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void fillBackgroundInformation(int pdg, double vx, double vy, double vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void initializeProtonPoints(TGraph *gProton);
        void initializePionPoints(TGraph *gPion);
        double computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints);

    private: 
        // Product's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string simulation_producer_label_;
        std::string recotrackmcparticlematching_label_;

        // Histograms
        TH1D* hTotalEvents;

        // fcl parameters
        bool         bVerbose;
        unsigned int MeanDEDXNumberTrajPoints;
        double       TrackStitchingThreshold;
        double       fMeanDEDXThreshold;
        double       fVertexRadius;
        double       SmallTrackLength;
        int          MaxSmallTracks;
        double       MeanCurvatureThreshold;
        float        PROTON_ENERGY_LOWER_BOUND;
        float        PROTON_ENERGY_UPPER_BOUND;
        double       PION_CHI2_PION_VALUE;
        double       PION_CHI2_PROTON_VALUE;
        double       PROTON_CHI2_PION_VALUE;
        double       PROTON_CHI2_PROTON_VALUE;

        // For chi^2 cuts
        TGraph* gProton = new TGraph();
        TGraph* gPion   = new TGraph();

        // Output tree
        TTree *RecoEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;

        // Signal information
        bool isPionAbsorptionSignal;
        int  numVisibleProtons;

        // Cut information
        bool passesPionInRedVolume;
        bool passesNoOutgoingPion;
        bool passesSmallTracksCut;
        bool passesMeanCurvatureCut;

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
        double                   truthPrimaryVertexX;
        double                   truthPrimaryVertexY;
        double                   truthPrimaryVertexZ;
        std::vector<int>         truthPrimaryDaughtersPDG;
        std::vector<std::string> truthPrimaryDaughtersProcess;
        std::vector<double>      truthPrimaryDaughtersKE;

        // If pion inelastic scattered, want more information
        double                   truthScatteringAngle;
        double                   truthScatteredPionLength;
        double                   truthSecondaryVertexX;
        double                   truthSecondaryVertexY;
        double                   truthSecondaryVertexZ;
        std::vector<int>         truthSecondaryPionDaughtersPDG;
        std::vector<std::string> truthSecondaryPionDaughtersProcess;
        std::vector<double>      truthSecondaryPionDaughtersKE;

        // WC variables
        int    WC2TPCtrkID;
        double WCTrackMomentum;
        double WC2TPCPrimaryBeginX;
        double WC2TPCPrimaryBeginY;
        double WC2TPCPrimaryBeginZ;
        double WC2TPCPrimaryEndX;
        double WC2TPCPrimaryEndY;
        double WC2TPCPrimaryEndZ;
        double WC2TPCPrimaryLength;

        double WC3PrimaryX;
        double WC3PrimaryY;
        double WC3PrimaryZ;
        double WC4PrimaryX;
        double WC4PrimaryY;
        double WC4PrimaryZ;
        double WCTheta;
        double WCPhi;
        double WCMeanCurvature;
        double WCMaxCurvature;

        // Wire chamber match truth information
        int                      wcMatchPDG;
        std::string              wcMatchProcess = "";
        std::vector<int>         wcMatchDaughtersPDG;
        std::vector<std::string> wcMatchDaughtersProcess;
        std::vector<double>      wcMatchResR;
        std::vector<double>      wcMatchDEDX;
        std::vector<double>      wcMatchXPos;
        std::vector<double>      wcMatchYPos;
        std::vector<double>      wcMatchZPos;

        // Reco variables
        std::vector<bool>   isTrackInverted;
        std::vector<int>    recoTaggedAs;
        std::vector<double> recoBeginX;
        std::vector<double> recoBeginY;
        std::vector<double> recoBeginZ;
        std::vector<double> recoEndX;
        std::vector<double> recoEndY;
        std::vector<double> recoEndZ;
        std::vector<int>    recoTrkID;
        std::vector<bool>   isTrackNearVertex;
        std::vector<double> recoPionChi2;
        std::vector<double> recoProtonChi2;

        int numTaggedAsPions;
        int numTaggedAsProton;
        int numNotTagged;

        // Truth variables for particles matched to tracks
        std::vector<int>         matchedIdentity;
        std::vector<double>      matchedCleanliness;
        std::vector<double>      matchedCompleteness;
        std::vector<int>         matchedTrkID;
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
        std::vector<std::vector<double>> recoDEDX;
        std::vector<std::vector<double>> recoResR;
        std::vector<std::vector<double>> recoXPos;
        std::vector<std::vector<double>> recoYPos;
        std::vector<std::vector<double>> recoZPos;
        std::vector<double>              recoMeanDEDX;

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

    // Identify true-level primary particle and get its information
    std::vector<int> primaryDaughtersIDs;
    TLorentzVector primaryStart, primaryEnd;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (part->Process() == "primary") {
            truthPrimaryPDG = part->PdgCode();
            for (int i = 0; i < part->NumberDaughters(); ++i) primaryDaughtersIDs.push_back(part->Daughter(i));
            truthPrimaryVertexX = part->EndX();
            truthPrimaryVertexY = part->EndY();
            truthPrimaryVertexZ = part->EndZ(); 
            primaryStart = part->Position(); primaryEnd = part->EndPosition();
            break;
        }
    }

    std::vector<int> secondaryPionDaughtersIDs;
    TLorentzVector scatteredPionStart, scatteredPionEnd;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (std::find(primaryDaughtersIDs.begin(), primaryDaughtersIDs.end(), part->TrackId()) != primaryDaughtersIDs.end()) {
            truthPrimaryDaughtersProcess.push_back(part->Process());
            truthPrimaryDaughtersPDG.push_back(part->PdgCode());
            truthPrimaryDaughtersKE.push_back(part->E() - part->Mass());

            // Save information for secondary pions
            if (part->PdgCode() == -211) {
                // Get daughters of scattered pion
                truthScatteredPionLength = trackMagnitude(part);
                for (int i = 0; i < part->NumberDaughters(); ++i) secondaryPionDaughtersIDs.push_back(part->Daughter(i));
                scatteredPionStart = part->Position(); scatteredPionEnd = part->EndPosition();
            }
        }
    }


    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (std::find(secondaryPionDaughtersIDs.begin(), secondaryPionDaughtersIDs.end(), part->TrackId()) != secondaryPionDaughtersIDs.end()) {
            truthSecondaryPionDaughtersPDG.push_back(part->PdgCode());
            truthSecondaryPionDaughtersProcess.push_back(part->Process());
            truthSecondaryPionDaughtersKE.push_back(part->E() - part->Mass());
        }
    }

    TVector3 incomingPrimary(primaryEnd.X() - primaryStart.X(), primaryEnd.Y() - primaryStart.Y(), primaryEnd.Z() - primaryStart.Z());
    TVector3 scatteredPion(scatteredPionEnd.X() - scatteredPionStart.X(), scatteredPionEnd.Y() - scatteredPionStart.Y(), scatteredPionEnd.Z() - scatteredPionStart.Z());
    truthScatteringAngle  = incomingPrimary.Angle(scatteredPion);
    truthSecondaryVertexX = scatteredPionEnd.X();
    truthSecondaryVertexY = scatteredPionEnd.Y();
    truthSecondaryVertexZ = scatteredPionEnd.Z();

    fillSignalInformation(
        truthPrimaryPDG,
        truthPrimaryVertexX,
        truthPrimaryVertexY,
        truthPrimaryVertexZ,
        truthPrimaryDaughtersPDG,
        truthPrimaryDaughtersProcess,
        truthPrimaryDaughtersKE
    );

    if (isPionAbsorptionSignal) {
        if (numVisibleProtons == 0) backgroundType = 0;
        if (numVisibleProtons > 0)  backgroundType = 1;
    }
    hTotalEvents->Fill(backgroundType);

    //////////////////////
    // Wire chamber tracks
    //////////////////////

    if (bVerbose) std::cout << "Getting wire chamber track information" << std::endl;

    art::Handle<std::vector<ldp::WCTrack>> wctrackHandle;
    std::vector<art::Ptr<ldp::WCTrack>>    wctrack;
    // If there is no wire chamber tracks for label, return
    if (!e.getByLabel(strWCTrackBuilderLabel, wctrackHandle)) return;
    art::fill_ptr_vector(wctrack, wctrackHandle);

    int numWCtrks = wctrack.size(); // number of wire chamber tracks
    if (numWCtrks != 1) return; 

    // Get wcTrack momentum
    WCTrackMomentum = wctrack[0]->Momentum() * 0.001; // Mev to GeV
    WC3PrimaryX     = wctrack[0]->HitPosition(2,0);
    WC3PrimaryY     = wctrack[0]->HitPosition(2,1);
    WC3PrimaryZ     = wctrack[0]->HitPosition(2,2);
    WC4PrimaryX     = wctrack[0]->HitPosition(3,0);
    WC4PrimaryY     = wctrack[0]->HitPosition(3,1);
    WC4PrimaryZ     = wctrack[0]->HitPosition(3,2);
    WCTheta         = wctrack[0]->Theta();
    WCPhi           = wctrack[0]->Phi();

    if (bVerbose) std::cout << "WCTrackMomentum: " << WCTrackMomentum << std::endl;
    if (bVerbose) std::cout << std::endl;

    /////////////
    // TPC tracks
    /////////////

    if (bVerbose) std::cout << "Getting TPC track information" << std::endl;

    art::Handle<std::vector<recob::Track>> tpcTrackHandle;
    std::vector<art::Ptr<recob::Track>> tracklist;
    // If there are no tpc chamber tracks for label, return
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

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

    
    // Check that there is a WC to TPC match, and check if pion stops inside fiducial volume
    if (WC2TPCtrkID != -99999) {
        // Found match, now find track
        for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
            auto thisTrack = tracklist.at(trk_idx);
            recob::TrackTrajectory::Point_t recoWC2TPCBeginning;
            recob::TrackTrajectory::Point_t recoWC2TPCEnd;

            if (thisTrack->ID() == WC2TPCtrkID) {
                // Get curvature
                auto [meanCurvature, maxCurvature] = computeCurvature(*thisTrack);
                WCMeanCurvature = meanCurvature;
                WCMaxCurvature  = maxCurvature;

                // Reverse primary if needed 
                bool isPrimaryReversed = false;
                if ((thisTrack->Start()).Z() < (thisTrack->End()).Z()) {
                    recoWC2TPCBeginning = thisTrack->Start();
                    recoWC2TPCEnd = thisTrack->End();
                } else {
                    recoWC2TPCEnd = thisTrack->Start();
                    recoWC2TPCBeginning = thisTrack->End();
                    isPrimaryReversed = true;
                }

                // Grab primary position data
                WC2TPCPrimaryBeginX = recoWC2TPCBeginning.X();
                WC2TPCPrimaryBeginY = recoWC2TPCBeginning.Y();
                WC2TPCPrimaryBeginZ = recoWC2TPCBeginning.Z();
                WC2TPCPrimaryEndX   = recoWC2TPCEnd.X();
                WC2TPCPrimaryEndY   = recoWC2TPCEnd.Y();
                WC2TPCPrimaryEndZ   = recoWC2TPCEnd.Z();
                WC2TPCPrimaryLength = thisTrack->Length();

                // Get calo data
                std::vector<double> thisTrackDEDX; std::vector<double> thisTrackResR; std::vector<double> thisTrackXPos; std::vector<double> thisTrackYPos; std::vector<double> thisTrackZPos; 
                double thisMeanDEDX = meanDEDX(fmcal, thisTrack.key(), isPrimaryReversed, thisTrackDEDX, thisTrackResR, thisTrackXPos, thisTrackYPos, thisTrackZPos);
                wcMatchResR = thisTrackResR; wcMatchDEDX = thisTrackDEDX; wcMatchXPos = thisTrackXPos; wcMatchYPos = thisTrackYPos; wcMatchZPos = thisTrackZPos;

                // Check primary track is inside reduced volume
                if (!(isWithinReducedVolume(WC2TPCPrimaryEndX, WC2TPCPrimaryEndY, WC2TPCPrimaryEndZ))) {
                    if (thisMeanDEDX > fMeanDEDXThreshold) {
                        // What to do with these??
                        passesPionInRedVolume = true;
                    } else {
                        passesPionInRedVolume = false;
                    }
                } else {
                    passesPionInRedVolume = true;
                }

                std::vector<art::Ptr<simb::MCParticle>> const& particles               = find_many_mcparticles_from_tracks.at(trk_idx);
                std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trk_idx);
                
                auto const& particle = particles.front();
                wcMatchPDG           = particle->PdgCode();
                wcMatchProcess       = particle->Process();
                
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
    }

    int numSmallTracks    = 0;
    for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
        auto thisTrack = tracklist.at(trk_idx);
        recob::TrackTrajectory::Point_t recoBeginning;
        recob::TrackTrajectory::Point_t recoEnd;

        // Continue if ID is that of matched beamline particle
        if (thisTrack->ID() == WC2TPCtrkID) continue;

        bool isThisTrackReversed = false;

        // Order track
        double startDistance = distance(thisTrack->Start().X(), WC2TPCPrimaryEndX, thisTrack->Start().Y(), WC2TPCPrimaryEndY, thisTrack->Start().Z(), WC2TPCPrimaryEndZ);             
        double endDistance = distance(thisTrack->End().X(), WC2TPCPrimaryEndX, thisTrack->End().Y(), WC2TPCPrimaryEndY, thisTrack->End().Z(), WC2TPCPrimaryEndZ);

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

        // Calo data
        std::vector<double> thisTrackDEDX; std::vector<double> thisTrackResR; std::vector<double> thisTrackXPos; std::vector<double> thisTrackYPos; std::vector<double> thisTrackZPos; 
        double thisMeanDEDX = meanDEDX(fmcal, thisTrack.key(), isThisTrackReversed, thisTrackDEDX, thisTrackResR, thisTrackXPos, thisTrackYPos, thisTrackZPos);
        recoDEDX.push_back(thisTrackDEDX); recoResR.push_back(thisTrackResR); recoMeanDEDX.push_back(thisMeanDEDX); recoXPos.push_back(thisTrackXPos); recoYPos.push_back(thisTrackYPos); recoZPos.push_back(thisTrackZPos);

        // Get chi^2 values
        int    caloPoints = thisTrackDEDX.size(); 
        double protonChi2 = computeReducedChi2(gProton, thisTrackResR, thisTrackDEDX, caloPoints);
        double pionChi2   = computeReducedChi2(gPion, thisTrackResR, thisTrackDEDX, caloPoints);

        // Find tracks near vertex
        bool thisTrackNearVertex = false;
        if ((startDistance < fVertexRadius) || (endDistance < fVertexRadius)) {
            thisTrackNearVertex = true;

            // Classify track as either pion or proton with chi^2
            if ((pionChi2 < PION_CHI2_PION_VALUE) && (protonChi2 > PROTON_CHI2_PION_VALUE)) {
                // Tagged as pion
                recoTaggedAs.push_back(0);
                numTaggedAsPions++;
            } else if ((pionChi2 > PION_CHI2_PROTON_VALUE) && (protonChi2 < PROTON_CHI2_PROTON_VALUE)) {
                // Tagged as proton
                recoTaggedAs.push_back(1);
                numTaggedAsProton++;
            } else {
                // Not tagged as either
                recoTaggedAs.push_back(2);
                numNotTagged++;
            }

            // // Classify track as either pion or proton with mean dE/dx
            // // Reject events with outgoing pions
            // if (thisMeanDEDX <= fMeanDEDXThreshold) {
            //     // TODO: pion stitching?
            //     return false;
            // }
        } // end if track begins or ends near pion
        else {
            recoTaggedAs.push_back(-1);
        }

        // Fill info about truth-matched particle

        // Get MCParticle objects and metadata
        std::vector<art::Ptr<simb::MCParticle>> const& particles = find_many_mcparticles_from_tracks.at(trk_idx);
        std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trk_idx);

        isTrackInverted.push_back(isThisTrackReversed);
        isTrackNearVertex.push_back(thisTrackNearVertex);
        recoBeginX.push_back(recoBeginning.X());
        recoBeginY.push_back(recoBeginning.Y());
        recoBeginZ.push_back(recoBeginning.Z());
        recoEndX.push_back(recoEnd.X());
        recoEndY.push_back(recoEnd.Y());
        recoEndZ.push_back(recoEnd.Z());
        recoTrkID.push_back(thisTrack->ID());
        recoPionChi2.push_back(pionChi2);
        recoProtonChi2.push_back(protonChi2);

        // If reco track is not matched to anything, add dummy values
        if (btdata_vector.size() == 0) {
            matchedIdentity.push_back(-99999);
            matchedCleanliness.push_back(-99999);
            matchedCompleteness.push_back(-99999);
            matchedTrkID.push_back(-99999);
            matchedProcess.push_back("");
            matchedBeginX.push_back(-99999);
            matchedBeginY.push_back(-99999);
            matchedBeginZ.push_back(-99999);
            matchedEndX.push_back(-99999);
            matchedEndY.push_back(-99999);
            matchedEndZ.push_back(-99999);
            matchedRealEndX.push_back(-99999);
            matchedRealEndY.push_back(-99999);
            matchedRealEndZ.push_back(-99999);
            matchedLength.push_back(-99999);
            matchedKEnergy.push_back(-99999);
            matchedEndingKEnergy.push_back(-99999);
            matchedInitialPx.push_back(-99999);
            matchedInitialPy.push_back(-99999);
            matchedInitialPz.push_back(-99999);
            matchedEndingPx.push_back(-99999);
            matchedEndingPy.push_back(-99999);
            matchedEndingPz.push_back(-99999);
        }

        // Get MCParticle object and data
        auto const& particle = particles.front();
        int const pdg_code   = particle->PdgCode();
        int const g4_trk_id  = particle->TrackId();
        std::string process  = particle->Process();

        double const cleanliness  = btdata_vector.front()->cleanliness;
        double const completeness = btdata_vector.front()->completeness;

        auto partTrackBegin = firstPointInTPC(particle);
        auto partTrackEnd   = lastPointInTPC(particle);
        auto bestEnd        = std::min(partTrackEnd, particle->NumberTrajectoryPoints() - 2);

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
        matchedCompleteness.push_back(completeness);
        matchedCleanliness.push_back(cleanliness);
        matchedTrkID.push_back(g4_trk_id);
        matchedProcess.push_back(process);
        matchedInitialPx.push_back(particle->Px(partTrackBegin));
        matchedInitialPy.push_back(particle->Py(partTrackBegin));
        matchedInitialPz.push_back(particle->Pz(partTrackBegin));
        matchedEndingPx.push_back(particle->Px(bestEnd));
        matchedEndingPy.push_back(particle->Py(bestEnd));
        matchedEndingPz.push_back(particle->Pz(bestEnd));
        matchedKEnergy.push_back(particle->E(partTrackBegin) - particle->Mass());
        matchedEndingKEnergy.push_back(particle->E(bestEnd) - particle->Mass());
    }
    
    if (numTaggedAsPions > 0) {
        passesNoOutgoingPion = false;
    } else {
        passesNoOutgoingPion = true;
    }

    if (numSmallTracks > MaxSmallTracks) {
        passesSmallTracksCut = false;
    } else {
        passesSmallTracksCut = true;
    }

    if (WCMeanCurvature > MeanCurvatureThreshold) {
        passesMeanCurvatureCut = false;
    } else {
        passesMeanCurvatureCut = true;
    }

    if (bVerbose) std::cout << std::endl;
    RecoEvalTree->Fill();
}

void RecoEval::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    art::ServiceHandle<art::TFileService> tfs;

    // Initialize chi^2 graphs
    initializeProtonPoints(gProton);
    initializePionPoints(gPion);

    // Make histograms and tree branches
    hTotalEvents = tfs->make<TH1D>("hTotalEvents", "hTotalEvents", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    RecoEvalTree = tfs->make<TTree>("RecoEvalTree", "RecoEvalTree");

    RecoEvalTree->Branch("run", &run, "run/I");
    RecoEvalTree->Branch("subrun", &subrun, "subrun/I");
    RecoEvalTree->Branch("event", &event, "event/I");

    RecoEvalTree->Branch("isPionAbsorptionSignal", &isPionAbsorptionSignal, "isPionAbsorptionSignal/O");
    RecoEvalTree->Branch("numVisibleProtons", &numVisibleProtons, "numVisibleProtons/I");
    RecoEvalTree->Branch("backgroundType", &backgroundType, "backgroundType/I");

    RecoEvalTree->Branch("truthPrimaryPDG", &truthPrimaryPDG, "truthPrimaryPDG/I");
    RecoEvalTree->Branch("truthPrimaryVertexX", &truthPrimaryVertexX, "truthPrimaryVertexX/D");
    RecoEvalTree->Branch("truthPrimaryVertexY", &truthPrimaryVertexY, "truthPrimaryVertexY/D");
    RecoEvalTree->Branch("truthPrimaryVertexZ", &truthPrimaryVertexZ, "truthPrimaryVertexZ/D");
    RecoEvalTree->Branch("truthPrimaryDaughtersPDG", "std::vector<int>", &truthPrimaryDaughtersPDG);
    RecoEvalTree->Branch("truthPrimaryDaughtersProcess", "std::vector<std::string>", &truthPrimaryDaughtersProcess);
    RecoEvalTree->Branch("truthPrimaryDaughtersKE", "std::vector<double>", &truthPrimaryDaughtersKE);

    RecoEvalTree->Branch("truthScatteringAngle", &truthScatteringAngle, "truthScatteringAngle/D");
    RecoEvalTree->Branch("truthScatteredPionLength", &truthScatteredPionLength, "truthScatteredPionLength/D");
    RecoEvalTree->Branch("truthSecondaryVertexX", &truthSecondaryVertexX, "truthSecondaryVertexX/D");
    RecoEvalTree->Branch("truthSecondaryVertexY", &truthSecondaryVertexY, "truthSecondaryVertexY/D");
    RecoEvalTree->Branch("truthSecondaryVertexZ", &truthSecondaryVertexZ, "truthSecondaryVertexZ/D");
    RecoEvalTree->Branch("truthSecondaryPionDaughtersPDG", "std::vector<int>", &truthSecondaryPionDaughtersPDG); 
    RecoEvalTree->Branch("truthSecondaryPionDaughtersProcess", "std::vector<std::string>", &truthSecondaryPionDaughtersProcess); 
    RecoEvalTree->Branch("truthSecondaryPionDaughtersKE", "std::vector<double>", &truthSecondaryPionDaughtersKE); 

    RecoEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    RecoEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginX", &WC2TPCPrimaryBeginX, "WC2TPCPrimaryBeginX/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginY", &WC2TPCPrimaryBeginY, "WC2TPCPrimaryBeginY/D");
    RecoEvalTree->Branch("WC2TPCPrimaryBeginZ", &WC2TPCPrimaryBeginZ, "WC2TPCPrimaryBeginZ/D");
    RecoEvalTree->Branch("WC2TPCPrimaryEndX", &WC2TPCPrimaryEndX, "WC2TPCPrimaryEndX/D");
    RecoEvalTree->Branch("WC2TPCPrimaryEndY", &WC2TPCPrimaryEndY, "WC2TPCPrimaryEndY/D");
    RecoEvalTree->Branch("WC2TPCPrimaryEndZ", &WC2TPCPrimaryEndZ, "WC2TPCPrimaryEndZ/D");
    RecoEvalTree->Branch("WC2TPCPrimaryLength", &WC2TPCPrimaryLength, "WC2TPCPrimaryLength/D");

    RecoEvalTree->Branch("WC3PrimaryX", &WC3PrimaryX, "WC3PrimaryX/D");
    RecoEvalTree->Branch("WC3PrimaryY", &WC3PrimaryY, "WC3PrimaryY/D");
    RecoEvalTree->Branch("WC3PrimaryZ", &WC3PrimaryZ, "WC3PrimaryZ/D");
    RecoEvalTree->Branch("WC4PrimaryX", &WC4PrimaryX, "WC4PrimaryX/D");
    RecoEvalTree->Branch("WC4PrimaryY", &WC4PrimaryY, "WC4PrimaryY/D");
    RecoEvalTree->Branch("WC4PrimaryZ", &WC4PrimaryZ, "WC4PrimaryZ/D");
    RecoEvalTree->Branch("WCMeanCurvature", &WCMeanCurvature, "WCMeanCurvature/D");
    RecoEvalTree->Branch("WCMaxCurvature", &WCMaxCurvature, "WCMaxCurvature/D");
    RecoEvalTree->Branch("WCTheta", &WCTheta, "WCTheta/D");
    RecoEvalTree->Branch("WCPhi", &WCPhi, "WCPhi/D");

    RecoEvalTree->Branch("wcMatchPDG", &wcMatchPDG, "wcMatchPDG/I");
    RecoEvalTree->Branch("wcMatchProcess", "std::string", &wcMatchProcess);
    RecoEvalTree->Branch("wcMatchDaughtersPDG", "std::vector<int>", &wcMatchDaughtersPDG);
    RecoEvalTree->Branch("wcMatchDaughtersProcess", "std::vector<std::string>", &wcMatchDaughtersProcess);
    RecoEvalTree->Branch("wcMatchResR", "std::vector<double>", &wcMatchResR);
    RecoEvalTree->Branch("wcMatchDEDX", "std::vector<double>", &wcMatchDEDX);
    RecoEvalTree->Branch("wcMatchXPos", "std::vector<double>", &wcMatchXPos);
    RecoEvalTree->Branch("wcMatchYPos", "std::vector<double>", &wcMatchYPos);
    RecoEvalTree->Branch("wcMatchZPos", "std::vector<double>", &wcMatchZPos);

    RecoEvalTree->Branch("isTrackInverted", "std::vector<bool>", &isTrackInverted);
    RecoEvalTree->Branch("isTrackNearVertex", "std::vector<bool>", &isTrackNearVertex);
    RecoEvalTree->Branch("recoTaggedAs", "std::vector<int>", &recoTaggedAs);
    RecoEvalTree->Branch("recoBeginX", "std::vector<double>", &recoBeginX);
    RecoEvalTree->Branch("recoBeginY", "std::vector<double>", &recoBeginY);
    RecoEvalTree->Branch("recoBeginZ", "std::vector<double>", &recoBeginZ);
    RecoEvalTree->Branch("recoEndX", "std::vector<double>", &recoEndX);
    RecoEvalTree->Branch("recoEndY", "std::vector<double>", &recoEndY);
    RecoEvalTree->Branch("recoEndZ", "std::vector<double>", &recoEndZ);
    RecoEvalTree->Branch("recoTrkID", "std::vector<int>", &recoTrkID);
    RecoEvalTree->Branch("recoPionChi2", "std::vector<double>", &recoPionChi2);
    RecoEvalTree->Branch("recoProtonChi2", "std::vector<double>", &recoProtonChi2);

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

    RecoEvalTree->Branch("recoDEDX","std::vector<std::vector<double>>",&recoDEDX);
    RecoEvalTree->Branch("recoResR","std::vector<std::vector<double>>",&recoResR);
    RecoEvalTree->Branch("recoXPos","std::vector<std::vector<double>>",&recoXPos);
    RecoEvalTree->Branch("recoYPos","std::vector<std::vector<double>>",&recoYPos);
    RecoEvalTree->Branch("recoZPos","std::vector<std::vector<double>>",&recoZPos);
    RecoEvalTree->Branch("recoMeanDEDX","std::vector<double>",&recoMeanDEDX);

    RecoEvalTree->Branch("passesPionInRedVolume", &passesPionInRedVolume, "passesPionInRedVolume/O");
    RecoEvalTree->Branch("passesNoOutgoingPion", &passesNoOutgoingPion, "passesNoOutgoingPion/O");
    RecoEvalTree->Branch("passesSmallTracksCut", &passesSmallTracksCut, "passesSmallTracksCut/O");
    RecoEvalTree->Branch("passesMeanCurvatureCut", &passesMeanCurvatureCut, "passesMeanCurvatureCut/O");

    RecoEvalTree->Branch("numTaggedAsPions", &numTaggedAsPions, "numTaggedAsPions/I");
    RecoEvalTree->Branch("numTaggedAsProton", &numTaggedAsProton, "numTaggedAsProton/I");
    RecoEvalTree->Branch("numNotTagged", &numNotTagged, "numTaggenumNotTaggeddAsPions/I");
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

double RecoEval::distance(double x1, double x2, double y1, double y2, double z1, double z2) {
    return sqrt(
        pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)
    );
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
    double vx, double vy, double vz,
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
    double vx, double vy, double vz,
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

std::tuple<double, double> RecoEval::computeCurvature(recob::Track track) {
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

double RecoEval::curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3) {
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

double RecoEval::meanDEDX(
    art::FindManyP<anab::Calorimetry> fmcal, 
    unsigned int trackKey, 
    bool isThisTrackReversed,
    std::vector<double>& trackDEDX,
    std::vector<double>& trackResR,
    std::vector<double>& trackXPos, 
    std::vector<double>& trackYPos, 
    std::vector<double>& trackZPos
) {
    // Temporary storage for this reco track
    std::vector<double> recoPitch_v; 
    std::vector<double> recoDEDX_v;
    std::vector<double> recoEDep_v;
    std::vector<double> recoResR_v;
    std::vector<double> recoXPos_v;
    std::vector<double> recoYPos_v;
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
                recoXPos_v.push_back(calos[j]->XYZ()[k].X());
                recoYPos_v.push_back(calos[j]->XYZ()[k].Y());
                recoZPos_v.push_back(calos[j]->XYZ()[k].Z());
            } // end loop on calo points

            if (bVerbose) std::cout << "Filled calorimetry vectors" << std::endl;
            if (isThisTrackReversed) {
                std::reverse(recoResR_v.begin(), recoResR_v.end());
                std::reverse(recoDEDX_v.begin(), recoDEDX_v.end());
                std::reverse(recoXPos_v.begin(), recoXPos_v.end());
                std::reverse(recoYPos_v.begin(), recoYPos_v.end());
                std::reverse(recoZPos_v.begin(), recoZPos_v.end());
            }
        } // end loop over planes
    }

    // Compute and save mean dedx
    double meanDEDX = 0;
    unsigned int bound = MeanDEDXNumberTrajPoints;
    if (MeanDEDXNumberTrajPoints > recoDEDX_v.size()) bound = recoDEDX_v.size();
    for (unsigned int i = 0; i < bound; ++i) meanDEDX += recoDEDX_v.at(i);
    meanDEDX /= bound;

    trackDEDX = recoDEDX_v;
    trackResR = recoResR_v;
    trackXPos = recoXPos_v;
    trackYPos = recoYPos_v;
    trackZPos = recoZPos_v;
    
    return meanDEDX;
}

double RecoEval::computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints) {
    double chi2 = 0.0;

    for (int i = 0; i < nPoints; ++i) {
        double theoryY = theory->Eval(xData[i]); // interpolate the theory at xData[i]
        double deltaY = yData[i] - theoryY;
        chi2 += (deltaY * deltaY) / theoryY;
    }

    // Currently, no fixed parameters, so dof = nPoints
    int dof = nPoints;
    return dof > 0 ? chi2 / dof : 0.0; 
}

void RecoEval::initializeProtonPoints(TGraph* gProton) {
    double protonData[107][2] = {
        {31.95, 4.14}, {31.65, 4.16}, {31.35, 4.17}, {31.05, 4.18}, {30.75, 4.20},
        {30.45, 4.21}, {30.15, 4.23}, {29.85, 4.25}, {29.55, 4.26}, {29.25, 4.28},
        {28.95, 4.29}, {28.65, 4.31}, {28.35, 4.33}, {28.05, 4.34}, {27.75, 4.36},
        {27.45, 4.38}, {27.15, 4.40}, {26.85, 4.42}, {26.55, 4.43}, {26.25, 4.45},
        {25.95, 4.47}, {25.65, 4.49}, {25.35, 4.51}, {25.05, 4.53}, {24.75, 4.55},
        {24.45, 4.57}, {24.15, 4.60}, {23.85, 4.62}, {23.55, 4.64}, {23.25, 4.66},
        {22.95, 4.69}, {22.65, 4.71}, {22.35, 4.73}, {22.05, 4.76}, {21.75, 4.78},
        {21.45, 4.81}, {21.15, 4.83}, {20.85, 4.86}, {20.55, 4.89}, {20.25, 4.92},
        {19.95, 4.94}, {19.65, 4.97}, {19.35, 5.00}, {19.05, 5.03}, {18.75, 5.07},
        {18.45, 5.10}, {18.15, 5.13}, {17.85, 5.16}, {17.55, 5.20}, {17.25, 5.23},
        {16.95, 5.27}, {16.65, 5.31}, {16.35, 5.35}, {16.05, 5.39}, {15.75, 5.43},
        {15.45, 5.47}, {15.15, 5.51}, {14.85, 5.56}, {14.55, 5.60}, {14.25, 5.65},
        {13.95, 5.70}, {13.65, 5.75}, {13.35, 5.80}, {13.05, 5.85}, {12.75, 5.91},
        {12.45, 5.97}, {12.15, 6.03}, {11.85, 6.09}, {11.55, 6.15}, {11.25, 6.22},
        {10.95, 6.29}, {10.65, 6.36}, {10.35, 6.44}, {10.05, 6.52}, {9.75, 6.60},
        {9.45, 6.68}, {9.15, 6.77}, {8.85, 6.87}, {8.55, 6.97}, {8.25, 7.08},
        {7.95, 7.19}, {7.65, 7.30}, {7.35, 7.43}, {7.05, 7.56}, {6.75, 7.70},
        {6.45, 7.85}, {6.15, 8.02}, {5.85, 8.19}, {5.55, 8.38}, {5.25, 8.58},
        {4.95, 8.81}, {4.65, 9.05}, {4.35, 9.32}, {4.05, 9.61}, {3.75, 9.94},
        {3.45, 10.32}, {3.15, 10.74}, {2.85, 11.23}, {2.55, 11.80}, {2.25, 12.48},
        {1.95, 13.31}, {1.65, 14.35}, {1.35, 15.71}, {1.05, 17.59}, {0.75, 20.44},
        {0.45, 25.48}, {0.15, 38.12}
    };

    for (int i = 0; i < 107; ++i) {
        gProton->SetPoint(i, protonData[i][0], protonData[i][1]);
    }
}

void RecoEval::initializePionPoints(TGraph* gPion) {
    double pionData[107][2] = {
        {31.95, 2.4}, {31.65, 2.4}, {31.35, 2.4}, {31.05, 2.4}, {30.75, 2.4},
        {30.45, 2.4}, {30.15, 2.4}, {29.85, 2.4}, {29.55, 2.4}, {29.25, 2.4},
        {28.95, 2.4}, {28.65, 2.4}, {28.35, 2.4}, {28.05, 2.4}, {27.75, 2.5},
        {27.45, 2.5}, {27.15, 2.5}, {26.85, 2.5}, {26.55, 2.5}, {26.25, 2.5},
        {25.95, 2.5}, {25.65, 2.5}, {25.35, 2.5}, {25.05, 2.5}, {24.75, 2.5},
        {24.45, 2.5}, {24.15, 2.5}, {23.85, 2.5}, {23.55, 2.5}, {23.25, 2.6},
        {22.95, 2.6}, {22.65, 2.6}, {22.35, 2.6}, {22.05, 2.6}, {21.75, 2.6},
        {21.45, 2.6}, {21.15, 2.6}, {20.85, 2.6}, {20.55, 2.6}, {20.25, 2.6},
        {19.95, 2.6}, {19.65, 2.7}, {19.35, 2.7}, {19.05, 2.7}, {18.75, 2.7},
        {18.45, 2.7}, {18.15, 2.7}, {17.85, 2.7}, {17.55, 2.7}, {17.25, 2.8},
        {16.95, 2.8}, {16.65, 2.8}, {16.35, 2.8}, {16.05, 2.8}, {15.75, 2.8},
        {15.45, 2.8}, {15.15, 2.9}, {14.85, 2.9}, {14.55, 2.9}, {14.25, 2.9},
        {13.95, 2.9}, {13.65, 2.9}, {13.35, 3.0}, {13.05, 3.0}, {12.75, 3.0},
        {12.45, 3.0}, {12.15, 3.0}, {11.85, 3.1}, {11.55, 3.1}, {11.25, 3.1},
        {10.95, 3.1}, {10.65, 3.2}, {10.35, 3.2}, {10.05, 3.2}, {9.75, 3.3},
        {9.45, 3.3}, {9.15, 3.3}, {8.85, 3.4}, {8.55, 3.4}, {8.25, 3.4},
        {7.95, 3.5}, {7.65, 3.5}, {7.35, 3.6}, {7.05, 3.6}, {6.75, 3.7},
        {6.45, 3.7}, {6.15, 3.8}, {5.85, 3.9}, {5.55, 3.9}, {5.25, 4.0},
        {4.95, 4.1}, {4.65, 4.2}, {4.35, 4.3}, {4.05, 4.4}, {3.75, 4.6},
        {3.45, 4.7}, {3.15, 4.9}, {2.85, 5.1}, {2.55, 5.3}, {2.25, 5.6},
        {1.95, 5.9}, {1.65, 6.4}, {1.35, 6.9}, {1.05, 7.7}, {0.75, 8.9},
        {0.45, 11.0}, {0.15, 16.5}
    };

    for (int i = 0; i < 107; ++i) {
        gPion->SetPoint(i, pionData[i][0], pionData[i][1]);
    }
}


void RecoEval::resetTree() {
    numTaggedAsPions  = 0;
    numTaggedAsProton = 0;
    numNotTagged      = 0;

    passesPionInRedVolume  = false;
    passesNoOutgoingPion   = false;
    passesSmallTracksCut   = false;
    passesMeanCurvatureCut = false;

    WC2TPCtrkID = -99999;

    wcMatchPDG = -99999;
    wcMatchProcess = "";
    wcMatchDaughtersPDG.clear();
    wcMatchDaughtersProcess.clear();
    wcMatchDEDX.clear();
    wcMatchResR.clear();
    wcMatchXPos.clear();
    wcMatchYPos.clear();
    wcMatchZPos.clear();
    
    isTrackInverted.clear();
    recoBeginX.clear();
    recoBeginY.clear();
    recoBeginZ.clear();
    recoEndX.clear();
    recoEndY.clear();
    recoEndZ.clear();
    recoTrkID.clear();
    isTrackNearVertex.clear();
    recoPionChi2.clear();
    recoProtonChi2.clear();

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
    
    recoDEDX.clear();
    recoResR.clear();
    recoXPos.clear();
    recoYPos.clear();
    recoZPos.clear();
    recoMeanDEDX.clear();

    truthPrimaryPDG     = -99999;
    truthPrimaryVertexX = -99999;
    truthPrimaryVertexY = -99999;
    truthPrimaryVertexZ = -99999;
    truthPrimaryDaughtersPDG.clear();
    truthPrimaryDaughtersProcess.clear();
    truthPrimaryDaughtersKE.clear();
    
    truthScatteringAngle     = -99999;
    truthScatteredPionLength = -99999;
    truthSecondaryVertexX    = -99999;
    truthSecondaryVertexY    = -99999;
    truthSecondaryVertexZ    = -99999;
    truthSecondaryPionDaughtersPDG.clear();
    truthSecondaryPionDaughtersProcess.clear();
    truthSecondaryPionDaughtersKE.clear();

    isPionAbsorptionSignal = false;
    numVisibleProtons      = 0;
    backgroundType         = -1;
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
    fMeanDEDXThreshold        = p.get<double>("MeanDEDXThreshold", 5.0);
    fVertexRadius             = p.get<double>("VertexRadius", 4);
    SmallTrackLength          = p.get<double>("SmallTrackLength", 35);
    MaxSmallTracks            = p.get<int>("MaxSmallTracks", 5);
    MeanCurvatureThreshold    = p.get<double>("MeanCurvatureThreshold", 0.015);
    PROTON_ENERGY_LOWER_BOUND = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND = p.get<float>("ProtonEnergyUpperBound", 1.0);
    PION_CHI2_PION_VALUE     = p.get<double>("PionChi2PionValue", 3.);
    PION_CHI2_PROTON_VALUE   = p.get<double>("PionChi2ProtonValue", 1.);
    PROTON_CHI2_PION_VALUE   = p.get<double>("ProtonChi2PionValue", 3.);
    PROTON_CHI2_PROTON_VALUE = p.get<double>("ProtonChi2ProtonValue", 5.);
}

DEFINE_ART_MODULE(RecoEval)