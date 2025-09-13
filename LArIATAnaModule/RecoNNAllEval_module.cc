//////////////////////////////////////////////////////////////////////////
// Class:       RecoNNAllEval
// Module Type: analyzer
// File:        RecoNNAllEval_module.cc
//
// Written by Emilio Peláez, created on January 2025. Adapted from XSAnalysis module
// and RecoNNAllEval module by Matt King.
////////////////////////////////////////////////////////////////////////

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
#include "art/Framework/Core/EDFilter.h"
#include "art/Framework/Core/ModuleMacros.h"
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
#include "Utilities/DatabaseUtilityT1034.h"
#include "LArIATRecoAlg/TriggerFilterAlg.h"
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
#include "lardataobj/AnalysisBase/BackTrackerMatchingData.h"
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
#include "lardata/ArtDataHelper/MVAReader.h"

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
#include "lardata/ArtDataHelper/TrackUtils.h"
#include "lardata/DetectorInfoServices/LArPropertiesService.h"
#include "lardata/DetectorInfoServices/DetectorPropertiesService.h"
#include "lardata/Utilities/AssociationUtil.h"

#include "larsim/MCCheater/BackTrackerService.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "lardataobj/RecoBase/Shower.h"
#include "lardataobj/RecoBase/EndPoint2D.h"
#include "lardataobj/MCBase/MCShower.h"
#include "lardataobj/MCBase/MCStep.h"
#include "larreco/Calorimetry/CalorimetryAlg.h"
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
#include "TRandom2.h"
#include "TComplex.h"
#include "TFile.h"
#include "TH2D.h"
#include "TTimeStamp.h"
#include "TLorentzVector.h"

// ####################
// ### C++ includes ###
// ####################
#include <map>
#include <memory>
#include <fstream>
#include "math.h"
#include <algorithm>

class RecoNNAllEval : public art::EDAnalyzer {
    public: 
        explicit RecoNNAllEval(fhicl::ParameterSet const &p);
        RecoNNAllEval(RecoNNAllEval const &) = delete;
        RecoNNAllEval(RecoNNAllEval &&) = delete;
        RecoNNAllEval & operator = (RecoNNAllEval const &) = delete;
        RecoNNAllEval & operator = (RecoNNAllEval &&) = delete;

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
        double meanDEDX(art::FindManyP<anab::Calorimetry> fmcal, unsigned int trackKey, bool isThisTrackReversed, std::vector<double>& trackDEDX, std::vector<double>& trackResR, std::vector<double>& trackEDep, std::vector<double>& trackXPos, std::vector<double>& trackYPos, std::vector<double>& trackZPos);
        double distance(double x1, double x2, double y1, double y2, double z1, double z2);
        double curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3);
        std::tuple<double, double> computeCurvature(recob::Track track);
        void fillSignalInformation(int pdg, double vx, double vy, double vz, bool interactionInTrajectory, std::string trajectoryInteractionLabel, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void fillBackgroundInformation(int pdg, double vx, double vy, double vz, bool interactionInTrajectory, std::string trajectoryInteractionLabel, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void initializeProtonPoints(TGraph *gProton);
        void initializePionPoints(TGraph *gPion);
        double computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints);
        double energyLossCalculation(double x, double px);

    private: 
        // Product's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string simulation_producer_label_;
        std::string recotrackmcparticlematching_label_;
        std::string fHitsModule;
        std::string fHitsInstance;
        std::string fNNetModuleLabel;

        // Detector properties
        detinfo::DetectorProperties const* fDetProp;
        float fEfield;
        float fDriftVelocity[3]; 
        float fSamplingRate; 
        float fXTicksOffset[2]; 
        float fTriggerOffset;
        float fElectronLifeTime;

        // Calorimetry algorithm
        calo::CalorimetryAlg fCaloAlg;

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
        TTree *RecoNNAllEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;
        bool isData;

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
        int NUM_BACKGROUND_TYPES = 15;
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
        //    12: elastic scattering
        //    13: 0p scattering
        //    14: Np scattering

        // Shower probabilities information
        double trackProb;
        double showerProb;
        bool   obtainedProbabilities;

        double showerNoBoxProb;
        bool   obtainedNoBoxProbabilities;

        double showerOutsideBoxProb;
        bool   obtainedOutsideBoxProbabilities;

        // Truth primary information
        int                      truthPrimaryPDG;
        int                      truthPrimaryID;
        double                   truthPrimaryVertexX;
        double                   truthPrimaryVertexY;
        double                   truthPrimaryVertexZ;
        double                   truthPrimaryIncidentKE;
        double                   truthPrimaryVertexKE;
        std::vector<int>         truthPrimaryDaughtersID;
        std::vector<int>         truthPrimaryDaughtersPDG;
        std::vector<std::string> truthPrimaryDaughtersProcess;
        std::vector<double>      truthPrimaryDaughtersKE;

        // Truth information about interactions in trajectory
        bool        interactionInTrajectory;
        double      trajectoryInitialMomentumX;
        std::string trajectoryInteractionLabel;
        double      trajectoryInteractionAngle;
        double      trajectoryInteractionX;
        double      trajectoryInteractionY;
        double      trajectoryInteractionZ;
        double      trajectoryInteractionKE;

        // If pion inelastic scattered, want more information
        double                   truthScatteringAngle;
        double                   truthScatteredPionLength;
        double                   truthScatteredPionKE;
        double                   truthSecondaryVertexX;
        double                   truthSecondaryVertexY;
        double                   truthSecondaryVertexZ;
        std::vector<int>         truthSecondaryPionDaughtersPDG;
        std::vector<std::string> truthSecondaryPionDaughtersProcess;
        std::vector<double>      truthSecondaryPionDaughtersKE;

        // Shower product information for charge exchange events
        std::vector<int>         chExchShowerIDs;
        std::vector<std::string> chExchShowerProcesses;
        std::vector<int>         chExchShowerPDGs;
        std::vector<double>      chExchShowerLengths;

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

        std::vector<double> WC2TPCLocationsX;
        std::vector<double> WC2TPCLocationsY;
        std::vector<double> WC2TPCLocationsZ;

        // WC2TPC truth information
        int                      wcMatchPDG;
        std::string              wcMatchProcess = "";
        std::vector<int>         wcMatchDaughtersPDG;
        std::vector<std::string> wcMatchDaughtersProcess;
        std::vector<double>      wcMatchResR;
        std::vector<double>      wcMatchEDep;
        std::vector<double>      wcMatchDEDX;
        std::vector<double>      wcMatchXPos;
        std::vector<double>      wcMatchYPos;
        std::vector<double>      wcMatchZPos;

        // True incident KE information
        bool                validTrueIncidentKE;
        std::vector<double> trueIncidentKEContributions;

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
        std::vector<std::vector<double>> recoEDep;
        std::vector<std::vector<double>> recoXPos;
        std::vector<std::vector<double>> recoYPos;
        std::vector<std::vector<double>> recoZPos;
        std::vector<double>              recoMeanDEDX;

        // Vectors to fill with individual hit information
        std::vector<art::Ptr<recob::Hit>> fHitlist;
        std::vector<int>                  fHitKey;
        std::vector<int>                  fHitPlane;
        std::vector<float>                fHitT;
        std::vector<float>                fHitX;
        std::vector<float>                fHitW;
        std::vector<float>                fHitCharge;
        std::vector<float>                fHitChargeCol;
        std::vector<int>                  hitRecoAsTrackKey;
        std::vector<int>                  hitWC2TPCKey;
        std::vector<int>                  hitThroughTrack;
        double                            primaryEndPointHitX;
        double                            primaryEndPointHitW;

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
        
        // Reduced volume for interactions
        const double RminX =  5.0;
        const double RmaxX = 42.0;
        const double RminY =-15.0;
        const double RmaxY = 15.0;
        const double RminZ =  8.0;
        const double RmaxZ = 82.0;

        // Random generator
        int       fRandSeed = 1989;
        TRandom2* fRand     = new TRandom2(fRandSeed);

};

RecoNNAllEval::RecoNNAllEval(fhicl::ParameterSet const &p) : EDAnalyzer(p) ,fCaloAlg(p.get<fhicl::ParameterSet>("CaloAlg")) {
    this->reconfigure(p);

    // Initialize detprop pointer
    fDetProp = lar::providerFrom<detinfo::DetectorPropertiesService>();
}

void RecoNNAllEval::analyze(art::Event const &e) {
    resetTree();

    run = e.run(); subrun = e.subRun(); event = e.event(); isData = e.isRealData();
    if (bVerbose) std::cout << "Run: " << run << ", subrun: " << subrun << ", event: " << event << ", is real data: " << isData << std::endl;
    if (bVerbose) std::cout << std::endl;

    if (
        event == 91686 ||
        event == 91707 || 
        event == 115702 || 
        event == 172759 ||
        event == 264599
    ) return; // Bad event, crashes the code for some reason

    // Get detector properties
    fEfield           = fDetProp->Efield(0);
    fDriftVelocity[0] = fDetProp->DriftVelocity(fEfield, fDetProp->Temperature());
    fDriftVelocity[1] = fDetProp->DriftVelocity(fDetProp->Efield(1), fDetProp->Temperature());
    fDriftVelocity[2] = fDetProp->DriftVelocity(fDetProp->Efield(2), fDetProp->Temperature());
    fTriggerOffset    = fDetProp->TriggerOffset();
    fElectronLifeTime = fDetProp->ElectronLifetime();
    fXTicksOffset[0]  = fDetProp->GetXTicksOffset(0,0,0);
    fXTicksOffset[1]  = fDetProp->GetXTicksOffset(1,0,0);
    fSamplingRate     = fDetProp->SamplingRate() * 1e-3;

    //////////////////////////
    // Get NN data products //
    //////////////////////////

    anab::MVAReader<recob::Hit, 4> hitResults(e, fNNetModuleLabel);
    std::vector<anab::FeatureVector<4>> featVec = hitResults.outputs();

    /////////////////
    // Get MC data //
    /////////////////

    // Get simulated particles
    auto particle_handle = e.getValidHandle<std::vector<simb::MCParticle>>(simulation_producer_label_);
    std::vector<art::Ptr<simb::MCParticle>> particle_vector;
    art::fill_ptr_vector(particle_vector, particle_handle);

    // Get particle list
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();

    // Get hits information
    art::Handle< std::vector<recob::Hit> > hitListHandle;
    if (e.getByLabel(fHitsModule, fHitsInstance, hitListHandle)) { art::fill_ptr_vector(fHitlist, hitListHandle); }
    size_t nWireHits = fHitlist.size();

    // Identify true-level primary particle and get its information
    std::vector<int> primaryDaughtersIDs;
    TLorentzVector primaryStart, primaryEnd;
    TLorentzVector vertexMomentum;
    simb::MCTrajectory primaryTrajectory;
    double primaryMass = 0.; int primaryPartID;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (part->Process() == "primary") {
            truthPrimaryPDG = part->PdgCode();
            truthPrimaryID  = part->TrackId();
            for (int i = 0; i < part->NumberDaughters(); ++i) primaryDaughtersIDs.push_back(part->Daughter(i));
            truthPrimaryVertexX    = part->EndX();
            truthPrimaryVertexY    = part->EndY();
            truthPrimaryVertexZ    = part->EndZ(); 
            primaryStart           = part->Position(); primaryEnd = part->EndPosition();
            primaryMass            = part->Mass();
            primaryPartID          = part->TrackId();
            truthPrimaryIncidentKE = part->E() - primaryMass;
            if (part->NumberTrajectoryPoints() > 1) {
                vertexMomentum         = part->Momentum(part->NumberTrajectoryPoints() - 2);
                truthPrimaryVertexKE   = part->E(part->NumberTrajectoryPoints() - 2) - primaryMass;
            }
            primaryTrajectory = part->Trajectory();
            break;
        }
    }

    // Get momentum for energy loss
    if (primaryTrajectory.TotalLength() > 1) {
        auto firstPrimaryPoint          = primaryTrajectory.begin();
        auto initialPrimaryTrajMomentum = firstPrimaryPoint->second;
        trajectoryInitialMomentumX      = 1000 * initialPrimaryTrajMomentum.X();
    }

    // We want to find last point in trajectory
    auto finalTPCPoint = std::prev(primaryTrajectory.end());
    // If there is an interesting interaction in the trajectory, that is the final point in the TPC
    // If no interaction in trajectory, we find vertex by looking at the daughters
    // If no interaction, it simply is the last point in the TPC

    // Look at interactions through primary trajectory
    auto primaryTrajectoryProcessMap = primaryTrajectory.TrajectoryProcesses();
    TLorentzVector momBeforeInteraction, momAfterInteraction;
    if (primaryTrajectory.size()) {
        for (auto const& couple: primaryTrajectoryProcessMap) {
            // Each couple is pair of the form (index, process key)

            // We do not concern ourselves with Coulomb scattering
            if ((primaryTrajectory.KeyToProcess(couple.second)).find("CoulombScat") != std::string::npos) continue;

            // Check position is inside reduced volume
            auto interactionPosition = (primaryTrajectory.at(couple.first)).first; // .at() returns (pos, mom), we grab pos
            if (!isWithinReducedVolume(interactionPosition.X(), interactionPosition.Y(), interactionPosition.Z())) continue;

            // If we do not have Coulomb scattering, and the interaction happens in the reduced volume,
            // we have an interesting interaction, so we want to save the information 
            interactionInTrajectory    = true;
            finalTPCPoint              = primaryTrajectory.begin() + couple.first;
            trajectoryInteractionLabel = primaryTrajectory.KeyToProcess(couple.second);

            trajectoryInteractionX = interactionPosition.X();
            trajectoryInteractionY = interactionPosition.Y();
            trajectoryInteractionZ = interactionPosition.Z();

            trajectoryInteractionKE = primaryTrajectory.E(couple.first) - primaryMass;
            
            // Get momentum before and after interaction
            if (couple.first - 1 >= 0) {
                momBeforeInteraction = (primaryTrajectory.at(couple.first - 1)).second;
            } else {
                momBeforeInteraction = (primaryTrajectory.at(couple.first)).second;
            }
            momAfterInteraction  = (primaryTrajectory.at(couple.first)).second;
        }
    }

    // If no interaction in trajectory, last traj point is found by looping backwards
    if (!interactionInTrajectory) {
        for (auto point = std::prev(primaryTrajectory.end()); point != primaryTrajectory.begin(); point--) {
            if (isWithinActiveVolume(point->first.X(), point->first.Y(), point->first.Z())) {
                finalTPCPoint = point;
                break;
            }
        }
    }

    if (interactionInTrajectory) {
        trajectoryInteractionAngle = momBeforeInteraction.Angle(momAfterInteraction.Vect());
        if (bVerbose) std::cout << "Interaction found in primary trajectory" << std::endl;
        if (bVerbose) std::cout << "  Interaction in trajectory: " << trajectoryInteractionLabel << std::endl;
        if (bVerbose) std::cout << "  Elastic scattering angle: " << trajectoryInteractionAngle << std::endl;
        if (bVerbose) std::cout << std::endl;
    }

    // Look at daughters of primary particle
    std::vector<int> secondaryPionDaughtersIDs;
    TLorentzVector scatteredPionStart, scatteredPionEnd;
    TLorentzVector outgoingScatterMomentum;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (std::find(primaryDaughtersIDs.begin(), primaryDaughtersIDs.end(), part->TrackId()) != primaryDaughtersIDs.end()) {
            truthPrimaryDaughtersProcess.push_back(part->Process());
            truthPrimaryDaughtersPDG.push_back(part->PdgCode());
            truthPrimaryDaughtersID.push_back(part->TrackId());
            truthPrimaryDaughtersKE.push_back(part->E() - part->Mass());

            // Save information for secondary pions
            if (part->PdgCode() == -211) {
                // Get daughters of scattered pion
                truthScatteredPionLength = trackMagnitude(part);
                truthScatteredPionKE     = part->E() - part->Mass();
                for (int i = 0; i < part->NumberDaughters(); ++i) secondaryPionDaughtersIDs.push_back(part->Daughter(i));
                scatteredPionStart = part->Position(); scatteredPionEnd = part->EndPosition();
                outgoingScatterMomentum = part->Momentum();
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

    truthScatteringAngle  = vertexMomentum.Angle(outgoingScatterMomentum.Vect());
    truthSecondaryVertexX = scatteredPionEnd.X();
    truthSecondaryVertexY = scatteredPionEnd.Y();
    truthSecondaryVertexZ = scatteredPionEnd.Z();

    fillSignalInformation(
        truthPrimaryPDG,
        truthPrimaryVertexX,
        truthPrimaryVertexY,
        truthPrimaryVertexZ,
        interactionInTrajectory,
        trajectoryInteractionLabel,
        truthPrimaryDaughtersPDG,
        truthPrimaryDaughtersProcess,
        truthPrimaryDaughtersKE
    );

    if (isPionAbsorptionSignal) {
        if (numVisibleProtons == 0) backgroundType = 0;
        if (numVisibleProtons > 0)  backgroundType = 1;
    } else if (backgroundType == 12 || (backgroundType == 6 && numVisibleProtons == 0)) {
        hTotalEvents->Fill(13);
    } else if (backgroundType == 6 && numVisibleProtons > 0) {
        hTotalEvents->Fill(14);
    }
    hTotalEvents->Fill(backgroundType);

    ////////////////////////////////////////////////////
    // Extra information about charge exchange events //
    ////////////////////////////////////////////////////

    if (backgroundType == 7) {
        for (size_t p = 0; p < plist.size(); ++p) {
            auto part = plist.Particle(p);
            if (
                std::find(primaryDaughtersIDs.begin(), primaryDaughtersIDs.end(), part->TrackId()) != primaryDaughtersIDs.end() &&
                part->PdgCode() == 111
            ) {
                // Found neutral pion from charge exchange
                chExchShowerIDs.push_back(part->TrackId());
                chExchShowerProcesses.push_back(part->Process());
                chExchShowerPDGs.push_back(part->PdgCode());
                chExchShowerLengths.push_back(trackMagnitude(part));

                // Recursively collect all electrons and photons from the shower
                std::function<void(int)> collectShowerParticles = [&](int trackId) {
                    for (size_t pidx = 0; pidx < plist.size(); ++pidx) {
                        auto showerPart = plist.Particle(pidx);
                        if (showerPart->Mother() == trackId) {
                            int pdg = showerPart->PdgCode();

                            // Photons, electrons, positrons
                            if (
                                (pdg == 22 || pdg == 11 || pdg == -11) &&
                                isWithinActiveVolume(showerPart->EndX(), showerPart->EndY(), showerPart->EndZ())
                            ) {
                                chExchShowerIDs.push_back(showerPart->TrackId());
                                chExchShowerProcesses.push_back(showerPart->Process());
                                chExchShowerPDGs.push_back(showerPart->PdgCode());
                                chExchShowerLengths.push_back(trackMagnitude(showerPart));
                            }

                            // Continue recursion for all daughters
                            collectShowerParticles(showerPart->TrackId());
                        }
                    }
                };

                // Start recursion from each direct daughter of the neutral pion
                collectShowerParticles(part->TrackId());
                for (int i = 0; i < part->NumberDaughters(); ++i) {
                    collectShowerParticles(part->Daughter(i));
                }

                break;
            }
        }
    }

    /////////////////////////////////////////////////
    // Truth-level data about WC match incident KE //
    /////////////////////////////////////////////////

    // Setup services
    art::ServiceHandle<geo::Geometry> geom;
    art::ServiceHandle<cheat::BackTrackerService> bt;
    geo::View_t view = geom->View(0);
    auto simIDE_Prim = bt->TrackIdToSimIDEs_Ps(primaryPartID, view);
    std::map<double, sim::IDE> orderedSimIDE;
    for (auto ide : simIDE_Prim) orderedSimIDE[ide->z] = *ide;

    // Constants
    const double trackPitch = 0.47;

    // Find first point in TPC
    auto firstTPCPoint = primaryTrajectory.begin();
    for (auto point = primaryTrajectory.begin(); point != std::prev(primaryTrajectory.end()); point++) {
        if (isWithinActiveVolume(point->first.X(), point->first.Y(), point->first.Z())) {
            firstTPCPoint = point;
            break;
        }
    }

    validTrueIncidentKE = true;
    if (firstTPCPoint == primaryTrajectory.begin()) validTrueIncidentKE = false;
    if (firstTPCPoint == finalTPCPoint) validTrueIncidentKE = false;
    if (truthPrimaryPDG != -211) validTrueIncidentKE = false;

    double totalLength = distance(firstTPCPoint->first.X(), finalTPCPoint->first.X(), firstTPCPoint->first.Y(), finalTPCPoint->first.Y(), firstTPCPoint->first.Z(), finalTPCPoint->first.Z());
    if (totalLength < trackPitch) validTrueIncidentKE = false; // less than separation between two wires

    // Chop up points between first and last uniformly and ordered increasing in Z
    std::map<double, TVector3> orderedUniformTrjPts;

    auto positionVector0 = (firstTPCPoint->first).Vect();
    auto positionVector1 = (finalTPCPoint->first).Vect();
    orderedUniformTrjPts[positionVector0.Z()] = positionVector0;
    orderedUniformTrjPts[positionVector1.Z()] = positionVector1;

    int numberPts = (int) (totalLength / trackPitch);
    for (int iPoint = 1; iPoint <= numberPts; ++iPoint) {
        auto newPoint = positionVector0 + iPoint * (trackPitch / totalLength) * (positionVector1 - positionVector0);
        orderedUniformTrjPts[newPoint.Z()] = newPoint;
    }

    // If distance between last point and second to last is less than 0.235, eliminate second to last
    auto lastPt         = (orderedUniformTrjPts.rbegin())->second;
    auto secondtoLastPt = (std::next(orderedUniformTrjPts.rbegin()))->second;
    double lastDist     = distance(lastPt.X(), secondtoLastPt.X(), lastPt.Y(), secondtoLastPt.Y(), lastPt.Z(), secondtoLastPt.Z());
    if (lastDist < 0.235) orderedUniformTrjPts.erase((std::next(orderedUniformTrjPts.rbegin()))->first);

    // Initial true KE
    auto initialMomentum = firstTPCPoint->second;
    double trueInitialKE = 1000 * (
        TMath::Sqrt(
            initialMomentum.X() * initialMomentum.X() + 
            initialMomentum.Y() * initialMomentum.Y() + 
            initialMomentum.Z() * initialMomentum.Z() + 
            primaryMass * primaryMass
        ) - primaryMass
    );
    double trueKineticEnergy = trueInitialKE;

    // Get contributions to truth incident KE
    for (auto it = std::next(orderedUniformTrjPts.begin()), old_it = orderedUniformTrjPts.begin(); it != orderedUniformTrjPts.end(); it++, old_it++) {
        auto oldPos     = old_it->second;
        auto currentPos = it->second;

        double uniformDist = (currentPos - oldPos).Mag();

        // Calculate energy deposited in this slice
        auto old_iter           = orderedSimIDE.begin();
        double currentDepEnergy = 0.;
        for (auto iter = orderedSimIDE.begin(); iter != orderedSimIDE.end(); iter++, old_iter++) {
            auto currentIDE = iter->second;
            if (currentIDE.z < oldPos.Z()) continue;
            if (currentIDE.z > currentPos.Z()) continue;
            currentDepEnergy += currentIDE.energy;
        }

        // Skip tiny energy depositions
        if (currentDepEnergy / uniformDist < 0.1) continue;

        // Calculate current kinetic energy
        trueKineticEnergy -= currentDepEnergy;

        if (isWithinReducedVolume(currentPos.X(), currentPos.Y(), currentPos.Z())) {
            trueIncidentKEContributions.push_back(trueKineticEnergy);
        }
    }

    //////////////////////
    // Wire chamber tracks
    //////////////////////

    if (bVerbose) std::cout << "Getting wire chamber track information" << std::endl;

    art::Handle<std::vector<ldp::WCTrack>> wctrackHandle;
    std::vector<art::Ptr<ldp::WCTrack>>    wctrack;
    // If there is no wire chamber tracks for label, return
    if (!e.getByLabel(strWCTrackBuilderLabel, wctrackHandle)) { RecoNNAllEvalTree->Fill(); return; }
    art::fill_ptr_vector(wctrack, wctrackHandle);

    int numWCtrks = wctrack.size(); // number of wire chamber tracks
    if (numWCtrks != 1) { RecoNNAllEvalTree->Fill(); return; }

    // Get wcTrack momentum
    WCTrackMomentum = wctrack[0]->Momentum();
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

    ////////////////
    // TPC tracks //
    ////////////////

    if (bVerbose) std::cout << "Getting TPC track information" << std::endl;

    art::Handle<std::vector<recob::Track>> tpcTrackHandle;
    std::vector<art::Ptr<recob::Track>> tracklist;
    // If there are no tpc chamber tracks for label, return
    if (!e.getByLabel(strTPCTrackHandleLabel, tpcTrackHandle)) { RecoNNAllEvalTree->Fill(); return; }
    art::fill_ptr_vector(tracklist, tpcTrackHandle);

    // Get hits associated with TPC tracks
    art::FindManyP<recob::Hit> HitsInTrack(tpcTrackHandle, e, strTPCTrackHandleLabel);

    int numTracksReco = tracklist.size();
    if (!numTracksReco) { RecoNNAllEvalTree->Fill(); return; } // If no TPC tracks, return
    
    if (bVerbose) std::cout << "Number of TPC reco tracks: " << numTracksReco << std::endl;
    if (bVerbose) std::cout << std::endl;

    //////////////////
    // WC2TPC Matching
    //////////////////

    art::FindOneP<recob::Track> fWC2TPC(wctrackHandle, e, strWC2TPCModuleLabel);

    if (fWC2TPC.isValid()) {
        if (bVerbose) std::cout << "Wire chamber to TPC is valid with size: " << fWC2TPC.size() << std::endl;
        for (unsigned int i = 0; i < fWC2TPC.size(); ++i) {
            cet::maybe_ref<recob::Track const> trackWC2TPC(*fWC2TPC.at(i));
            if (!trackWC2TPC) continue;
            recob::Track const& aTrack(trackWC2TPC.ref());
            WC2TPCtrkID = aTrack.ID();

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
    } else {
        if (bVerbose) std::cout << "Wire chamber to TPC is NOT valid!" << std::endl;
    }
    if (bVerbose) std::cout << "Wire chamber to TPC track ID: " << WC2TPCtrkID << std::endl;
    if (bVerbose) std::cout << std::endl;

    /////////////////////
    // MC particle tracks
    /////////////////////

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

    // Get MCParticles from tracks
    const art::FindManyP<simb::MCParticle, anab::BackTrackerMatchingData>
        find_many_mcparticles_from_tracks(tpcTrackHandle, e, recotrackmcparticlematching_label_);

    // Check that there is a WC to TPC match, and check if pion stops inside fiducial volume
    int WC2TPCtrackIndex = -1;
    if (WC2TPCtrkID != -99999) {
        // Found match, now find track
        for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
            auto thisTrack = tracklist.at(trk_idx);
            recob::TrackTrajectory::Point_t recoWC2TPCBeginning;
            recob::TrackTrajectory::Point_t recoWC2TPCEnd;

            if (thisTrack->ID() == WC2TPCtrkID) {
                WC2TPCtrackIndex = trk_idx;

                // Get primary track coordinates
                int numCoordPoints = thisTrack->NPoints();
                if (bVerbose) std::cout << "Primary track num coords: " << numCoordPoints << std::endl;
                for (int iCoord = 0; iCoord < numCoordPoints; ++iCoord) {
                    TVector3 p = thisTrack->LocationAtPoint<TVector3>(iCoord);
                    WC2TPCLocationsX.push_back(p.X()); WC2TPCLocationsY.push_back(p.Y()); WC2TPCLocationsZ.push_back(p.Z());
                    // if (bVerbose) std::cout << "  x: " << p.X() << " y: " << p.Y() << " z: " << p.Z() << std::endl;
                }
                if (bVerbose) std::cout << std::endl;

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
                std::vector<double> thisTrackDEDX; std::vector<double> thisTrackResR; std::vector<double> thisTrackEDep; std::vector<double> thisTrackXPos; std::vector<double> thisTrackYPos; std::vector<double> thisTrackZPos; 
                double thisMeanDEDX = meanDEDX(fmcal, thisTrack.key(), isPrimaryReversed, thisTrackDEDX, thisTrackResR, thisTrackEDep, thisTrackXPos, thisTrackYPos, thisTrackZPos);
                wcMatchResR = thisTrackResR; wcMatchDEDX = thisTrackDEDX; wcMatchEDep = thisTrackEDep; wcMatchXPos = thisTrackXPos; wcMatchYPos = thisTrackYPos; wcMatchZPos = thisTrackZPos;

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

    ///////////////////////////////////////////
    // Neural net primary electron detection //
    ///////////////////////////////////////////

    double total_shower_prob = 0.0;
    int num_hits = 0;

    double total_shower_prob_no_box = 0.0;
    int num_hits_no_box = 0;

    double total_shower_prob_outside_box = 0.0;
    int num_hits_outside_box = 0;

    if (HitsInTrack.isValid() && WC2TPCtrackIndex != -1 && WC2TPCtrkID != -99999) {
        int lowest_hit = -1;
        std::vector<art::Ptr<recob::Hit>> trackhits = HitsInTrack.at(WC2TPCtrackIndex);
        for (size_t iHit = 0; iHit < trackhits.size(); ++iHit) {
            if (trackhits[iHit]->View() != 1) continue;
            if (lowest_hit == -1) lowest_hit = iHit;
            if (trackhits[iHit]->WireID().Wire < trackhits[lowest_hit]->WireID().Wire) lowest_hit = iHit;
        }

        for (size_t iHit = 0; iHit < nWireHits; ++iHit) {
            if (fHitlist[iHit]->WireID().Plane != 1) continue;
            
            int wireID  = fHitlist[iHit]->WireID().Wire;
            int hitTime = fHitlist[iHit]->PeakTime();

            // Get NN output
            total_shower_prob_no_box += featVec[iHit][1] / (featVec[iHit][0] + featVec[iHit][1]); 
            num_hits_no_box++;
        }
        total_shower_prob_no_box /= double(num_hits_no_box);

        showerNoBoxProb = total_shower_prob_no_box;
        obtainedNoBoxProbabilities = true;

        if (lowest_hit != -1) {
            double inter  = trackhits[lowest_hit]->PeakTime();
            double offset = trackhits[lowest_hit]->WireID().Wire;

            if (!(offset > 100. || inter > 3000.)) {
                for (size_t iHit = 0; iHit < nWireHits; ++iHit) {
                    if (fHitlist[iHit]->WireID().Plane != 1) continue;

                    int wireID  = fHitlist[iHit]->WireID().Wire;
                    int hitTime = fHitlist[iHit]->PeakTime();
                    
                    if(
                        wireID > (offset + 100.0) || 
                        wireID < offset ||
                        hitTime > (inter + 200.0) || 
                        hitTime < (inter - 200.0)
                    ) {
                        total_shower_prob_outside_box += featVec[iHit][1] / (featVec[iHit][0] + featVec[iHit][1]);
                        num_hits_outside_box++;
                        continue;
                    }

                    if (hitTime > 3000.) continue;
                    if (wireID > 240.) continue;

                    // Get NN output
                    total_shower_prob += featVec[iHit][1] / (featVec[iHit][0] + featVec[iHit][1]); 
                    num_hits++;
                }
                total_shower_prob /= double(num_hits);

                showerProb = total_shower_prob;
                trackProb  = 1. - showerProb;
                obtainedProbabilities = true;

                showerOutsideBoxProb = total_shower_prob_outside_box / double(num_hits_outside_box);
                obtainedOutsideBoxProbabilities = true;
            }
        }
    }

    //////////////////////////////////////
    // Small tracks cut and track chi^2 //
    //////////////////////////////////////

    int numSmallTracks = 0;
    for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
        auto thisTrack = tracklist.at(trk_idx);
        recob::TrackTrajectory::Point_t recoBeginning;
        recob::TrackTrajectory::Point_t recoEnd;

        // Continue if ID is that of matched beamline particle
        // if (thisTrack->ID() == WC2TPCtrkID) continue;
        if (bVerbose) std::cout << "Looking at track with ID: " << thisTrack->ID() << std::endl;

        bool isThisTrackReversed = false;

        // Order track
        double startDistance = distance(thisTrack->Start().X(), WC2TPCPrimaryEndX, thisTrack->Start().Y(), WC2TPCPrimaryEndY, thisTrack->Start().Z(), WC2TPCPrimaryEndZ);             
        double endDistance   = distance(thisTrack->End().X(), WC2TPCPrimaryEndX, thisTrack->End().Y(), WC2TPCPrimaryEndY, thisTrack->End().Z(), WC2TPCPrimaryEndZ);

        if (startDistance < endDistance) {
            recoBeginning = thisTrack->Start();
            recoEnd       = thisTrack->End();
        } else {
            isThisTrackReversed = true;
            recoBeginning = thisTrack->End();
            recoEnd       = thisTrack->Start();
        }

        // Check if track counts towards count of small tracks
        double thisTrackLength = sqrt(
            pow(recoBeginning.X() - recoEnd.X(), 2) +
            pow(recoBeginning.Y() - recoEnd.Y(), 2) + 
            pow(recoBeginning.Z() - recoEnd.Z(), 2)
        );
        if (thisTrackLength < SmallTrackLength) numSmallTracks++;

        // Calo data
        std::vector<double> thisTrackDEDX; std::vector<double> thisTrackResR; std::vector<double> thisTrackEDep; std::vector<double> thisTrackXPos; std::vector<double> thisTrackYPos; std::vector<double> thisTrackZPos; 
        double thisMeanDEDX = meanDEDX(fmcal, thisTrack.key(), isThisTrackReversed, thisTrackDEDX, thisTrackResR, thisTrackEDep, thisTrackXPos, thisTrackYPos, thisTrackZPos);
        recoDEDX.push_back(thisTrackDEDX); recoResR.push_back(thisTrackResR); recoEDep.push_back(thisTrackEDep); recoMeanDEDX.push_back(thisMeanDEDX); recoXPos.push_back(thisTrackXPos); recoYPos.push_back(thisTrackYPos); recoZPos.push_back(thisTrackZPos);

        // Get chi^2 values
        if (bVerbose) std::cout << "Computing chi^2 values: " << std::endl;
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

        // Fill reco-level data
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

        // Fill info about truth-matched particle

        // Get MCParticle objects and metadata
        if (bVerbose) std::cout << "Filling truth data" << std::endl;
        std::vector<art::Ptr<simb::MCParticle>> const& particles               = find_many_mcparticles_from_tracks.at(trk_idx);
        std::vector<const anab::BackTrackerMatchingData*> const& btdata_vector = find_many_mcparticles_from_tracks.data(trk_idx);

        // If reco track is not matched to anything, add dummy values
        if (btdata_vector.size() == 0 || particles.size() == 0) {
            if (bVerbose) std::cout << "No truth match " << std::endl;
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
        } else {
            if (bVerbose) std::cout << "Found truth match " << std::endl;
            // Get MCParticle object and data
            auto const& particle = particles.front();
            int const pdg_code   = particle->PdgCode();
            int const g4_trk_id  = particle->TrackId();
            std::string process  = particle->Process();

            double const cleanliness  = btdata_vector.front()->cleanliness;
            double const completeness = btdata_vector.front()->completeness;

            auto partTrackBegin = firstPointInTPC(particle);
            if (partTrackBegin == 9999) partTrackBegin = 0;
            auto partTrackEnd   = lastPointInTPC(particle);
            auto bestEnd        = std::min(partTrackEnd, particle->NumberTrajectoryPoints() - 2);

            matchedIdentity.push_back(pdg_code);
            matchedBeginX.push_back(particle->Vx(partTrackBegin));
            matchedBeginY.push_back(particle->Vy(partTrackBegin));
            matchedBeginZ.push_back(particle->Vz(partTrackBegin));
            matchedEndX.push_back(particle->Vx(bestEnd));
            matchedEndY.push_back(particle->Vy(bestEnd));
            matchedEndZ.push_back(particle->Vz(bestEnd));
            matchedRealEndX.push_back(particle->Vx(particle->NumberTrajectoryPoints() - 1));
            matchedRealEndY.push_back(particle->Vy(particle->NumberTrajectoryPoints() - 1));
            matchedRealEndZ.push_back(particle->Vz(particle->NumberTrajectoryPoints() - 1));
            matchedLength.push_back(trackMagnitude(particle, partTrackBegin, bestEnd));
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
        if (bVerbose) std::cout << "Moving on to next track" << std::endl;
        if (bVerbose) std::cout << std::endl;
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

    //////////////////
    // Clustering hits
    //////////////////

    // Reserve space for hit information
    fHitKey.reserve(nWireHits);
    fHitPlane.reserve(nWireHits);
    fHitT.reserve(nWireHits);
    fHitX.reserve(nWireHits);
    fHitW.reserve(nWireHits);
    fHitCharge.reserve(nWireHits);
    fHitChargeCol.reserve(nWireHits);

    // First, we get all the information about our hits
    for (size_t iHit = 0; iHit < nWireHits; ++iHit) {
        int   hitPlane = fHitlist[iHit]->WireID().Plane;
        float hitTime0 = fSamplingRate * (fHitlist[iHit]->PeakTime() - fTriggerOffset);
        float hitTime  = fSamplingRate * (fHitlist[iHit]->PeakTime() - fXTicksOffset[hitPlane]);
        float ltCorFac = 1.;
        if (hitTime0 >= 0) { ltCorFac = exp(hitTime0 / fElectronLifeTime); }

        fHitKey.push_back(iHit);
        fHitPlane.push_back(hitPlane); // collection == 1, induction == 0
        fHitT.push_back(hitTime); // us
        fHitX.push_back(hitTime * fDriftVelocity[0]);
        if (fHitlist[iHit]->Channel() < 240) {
            fHitW.push_back(fHitlist[iHit]->Channel() * 0.4);
        } else {
            fHitW.push_back((fHitlist[iHit]->Channel() - 240) * 0.4);
        }
        fHitChargeCol.push_back(fCaloAlg.ElectronsFromADCArea(fHitlist[iHit]->Integral(), hitPlane));
        fHitCharge.push_back(fHitChargeCol.at(iHit) * ltCorFac);

        // Add stochasticity for MC data
        if (!isData) {
            float prob  = 1. / exp(hitTime0 / fElectronLifeTime);
            float mean  = fHitCharge.at(iHit) * prob;
            float sigma = sqrt(mean*(1. - prob));
            float fac   = (fRand->Gaus(mean, sigma) * ltCorFac) / fHitCharge.at(iHit);
            fHitCharge.at(iHit)   *= fac;
        }
    }

    if (bVerbose) std::cout << "Hits found for this event: " << fHitKey.size() << std::endl;

    // Associations between tracks and hits
    art::FindManyP<recob::Hit, recob::TrackHitMeta> fmthm(tpcTrackHandle, e, strTPCTrackHandleLabel);
    
    // We now want to loop through tracks and find what hits are already associated to tracks
    int totalNHits = 0;
    for (size_t trk_idx = 0; trk_idx < tpcTrackHandle->size(); ++trk_idx) {
        auto thisTrack = tracklist.at(trk_idx);
        int  nHits     = 0;
        if (bVerbose) std::cout << "  Track id: " << thisTrack->ID() << std::endl;

        bool isThroughGoing = (
            (!isWithinReducedVolume(thisTrack->Start().X(), thisTrack->Start().Y(), thisTrack->Start().Z())) &&
            (!isWithinReducedVolume(thisTrack->End().X(), thisTrack->End().Y(), thisTrack->End().Z()))
        );

        if (fmthm.isValid()) {
            auto vhit = fmthm.at(thisTrack->ID());
            for (size_t h = 0; h < vhit.size(); ++h) {
                if (thisTrack->ID() == WC2TPCtrkID) { hitWC2TPCKey.push_back(vhit[h].key()); }
                else if (isThroughGoing) hitThroughTrack.push_back(vhit[h].key());
                hitRecoAsTrackKey.push_back(vhit[h].key());
                ++nHits;
            }
        }
        totalNHits += nHits;
        if (bVerbose) std::cout << "    Hits for this track: " << nHits << std::endl;
    }
    if (bVerbose) std:: cout << "Total hits for all tracks: " << totalNHits << std::endl;

    // First, we find the end hit for the main TPC track
    float maxHitTime = -1e9;
    int   endpointHitIdx = -1;
    for (size_t i = 0; i < hitWC2TPCKey.size(); ++i) {
        int hitIdx = hitWC2TPCKey[i];
        if (fHitPlane[hitIdx] != 0) continue; // induction plane only

        float hitTime = fHitT[hitIdx];
        if (hitTime > maxHitTime) {
            maxHitTime = hitTime;
            endpointHitIdx = hitIdx;
        }
    }

    // Use the hit position in X-W of this endpoint
    primaryEndPointHitX = fHitX[endpointHitIdx];
    primaryEndPointHitW = fHitW[endpointHitIdx];

    if (bVerbose) std::cout << "End hit X: " << primaryEndPointHitX << " W: " << primaryEndPointHitW << std::endl;
    if (bVerbose) std::cout << std::endl;

    RecoNNAllEvalTree->Fill();
}

void RecoNNAllEval::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    art::ServiceHandle<art::TFileService> tfs;

    // Initialize chi^2 graphs
    initializeProtonPoints(gProton);
    initializePionPoints(gPion);

    // Make histograms and tree branches
    hTotalEvents = tfs->make<TH1D>("hTotalEvents", "hTotalEvents", NUM_BACKGROUND_TYPES, 0, NUM_BACKGROUND_TYPES);

    RecoNNAllEvalTree = tfs->make<TTree>("RecoNNAllEvalTree", "RecoNNAllEvalTree");

    RecoNNAllEvalTree->Branch("run", &run, "run/I");
    RecoNNAllEvalTree->Branch("subrun", &subrun, "subrun/I");
    RecoNNAllEvalTree->Branch("event", &event, "event/I");
    RecoNNAllEvalTree->Branch("isData", &isData, "isData/O");

    RecoNNAllEvalTree->Branch("isPionAbsorptionSignal", &isPionAbsorptionSignal, "isPionAbsorptionSignal/O");
    RecoNNAllEvalTree->Branch("numVisibleProtons", &numVisibleProtons, "numVisibleProtons/I");
    RecoNNAllEvalTree->Branch("backgroundType", &backgroundType, "backgroundType/I");

    RecoNNAllEvalTree->Branch("trackProb", &trackProb, "trackProb/D");
    RecoNNAllEvalTree->Branch("showerProb", &showerProb, "showerProb/D");
    RecoNNAllEvalTree->Branch("obtainedProbabilities", &obtainedProbabilities, "obtainedProbabilities/O");

    RecoNNAllEvalTree->Branch("showerNoBoxProb", &showerNoBoxProb, "showerNoBoxProb/D");
    RecoNNAllEvalTree->Branch("obtainedNoBoxProbabilities", &obtainedNoBoxProbabilities, "obtainedNoBoxProbabilities/O");

    RecoNNAllEvalTree->Branch("showerOutsideBoxProb", &showerOutsideBoxProb, "showerOutsideBoxProb/D");
    RecoNNAllEvalTree->Branch("obtainedOutsideBoxProbabilities", &obtainedOutsideBoxProbabilities, "obtainedOutsideBoxProbabilities/O");

    RecoNNAllEvalTree->Branch("truthPrimaryPDG", &truthPrimaryPDG, "truthPrimaryPDG/I");
    RecoNNAllEvalTree->Branch("truthPrimaryID", &truthPrimaryID, "truthPrimaryID/I");
    RecoNNAllEvalTree->Branch("truthPrimaryIncidentKE", &truthPrimaryIncidentKE, "truthPrimaryIncidentKE/D");
    RecoNNAllEvalTree->Branch("truthPrimaryVertexKE", &truthPrimaryVertexKE, "truthPrimaryVertexKE/D");
    RecoNNAllEvalTree->Branch("truthPrimaryVertexX", &truthPrimaryVertexX, "truthPrimaryVertexX/D");
    RecoNNAllEvalTree->Branch("truthPrimaryVertexY", &truthPrimaryVertexY, "truthPrimaryVertexY/D");
    RecoNNAllEvalTree->Branch("truthPrimaryVertexZ", &truthPrimaryVertexZ, "truthPrimaryVertexZ/D");
    RecoNNAllEvalTree->Branch("truthPrimaryDaughtersID", "std::vector<int>", &truthPrimaryDaughtersID);
    RecoNNAllEvalTree->Branch("truthPrimaryDaughtersPDG", "std::vector<int>", &truthPrimaryDaughtersPDG);
    RecoNNAllEvalTree->Branch("truthPrimaryDaughtersProcess", "std::vector<std::string>", &truthPrimaryDaughtersProcess);
    RecoNNAllEvalTree->Branch("truthPrimaryDaughtersKE", "std::vector<double>", &truthPrimaryDaughtersKE);

    RecoNNAllEvalTree->Branch("truthScatteringAngle", &truthScatteringAngle, "truthScatteringAngle/D");
    RecoNNAllEvalTree->Branch("truthScatteredPionLength", &truthScatteredPionLength, "truthScatteredPionLength/D");
    RecoNNAllEvalTree->Branch("truthScatteredPionKE", &truthScatteredPionKE, "truthScatteredPionKE/D");
    RecoNNAllEvalTree->Branch("truthSecondaryVertexX", &truthSecondaryVertexX, "truthSecondaryVertexX/D");
    RecoNNAllEvalTree->Branch("truthSecondaryVertexY", &truthSecondaryVertexY, "truthSecondaryVertexY/D");
    RecoNNAllEvalTree->Branch("truthSecondaryVertexZ", &truthSecondaryVertexZ, "truthSecondaryVertexZ/D");
    RecoNNAllEvalTree->Branch("truthSecondaryPionDaughtersPDG", "std::vector<int>", &truthSecondaryPionDaughtersPDG); 
    RecoNNAllEvalTree->Branch("truthSecondaryPionDaughtersProcess", "std::vector<std::string>", &truthSecondaryPionDaughtersProcess); 
    RecoNNAllEvalTree->Branch("truthSecondaryPionDaughtersKE", "std::vector<double>", &truthSecondaryPionDaughtersKE); 

    RecoNNAllEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    RecoNNAllEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryBeginX", &WC2TPCPrimaryBeginX, "WC2TPCPrimaryBeginX/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryBeginY", &WC2TPCPrimaryBeginY, "WC2TPCPrimaryBeginY/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryBeginZ", &WC2TPCPrimaryBeginZ, "WC2TPCPrimaryBeginZ/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryEndX", &WC2TPCPrimaryEndX, "WC2TPCPrimaryEndX/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryEndY", &WC2TPCPrimaryEndY, "WC2TPCPrimaryEndY/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryEndZ", &WC2TPCPrimaryEndZ, "WC2TPCPrimaryEndZ/D");
    RecoNNAllEvalTree->Branch("WC2TPCPrimaryLength", &WC2TPCPrimaryLength, "WC2TPCPrimaryLength/D");

    RecoNNAllEvalTree->Branch("WC3PrimaryX", &WC3PrimaryX, "WC3PrimaryX/D");
    RecoNNAllEvalTree->Branch("WC3PrimaryY", &WC3PrimaryY, "WC3PrimaryY/D");
    RecoNNAllEvalTree->Branch("WC3PrimaryZ", &WC3PrimaryZ, "WC3PrimaryZ/D");
    RecoNNAllEvalTree->Branch("WC4PrimaryX", &WC4PrimaryX, "WC4PrimaryX/D");
    RecoNNAllEvalTree->Branch("WC4PrimaryY", &WC4PrimaryY, "WC4PrimaryY/D");
    RecoNNAllEvalTree->Branch("WC4PrimaryZ", &WC4PrimaryZ, "WC4PrimaryZ/D");
    RecoNNAllEvalTree->Branch("WCMeanCurvature", &WCMeanCurvature, "WCMeanCurvature/D");
    RecoNNAllEvalTree->Branch("WCMaxCurvature", &WCMaxCurvature, "WCMaxCurvature/D");
    RecoNNAllEvalTree->Branch("WCTheta", &WCTheta, "WCTheta/D");
    RecoNNAllEvalTree->Branch("WCPhi", &WCPhi, "WCPhi/D");

    RecoNNAllEvalTree->Branch("wcMatchPDG", &wcMatchPDG, "wcMatchPDG/I");
    RecoNNAllEvalTree->Branch("wcMatchProcess", "std::string", &wcMatchProcess);
    RecoNNAllEvalTree->Branch("wcMatchDaughtersPDG", "std::vector<int>", &wcMatchDaughtersPDG);
    RecoNNAllEvalTree->Branch("wcMatchDaughtersProcess", "std::vector<std::string>", &wcMatchDaughtersProcess);
    RecoNNAllEvalTree->Branch("wcMatchResR", "std::vector<double>", &wcMatchResR);
    RecoNNAllEvalTree->Branch("wcMatchEDep", "std::vector<double>", &wcMatchEDep);
    RecoNNAllEvalTree->Branch("wcMatchDEDX", "std::vector<double>", &wcMatchDEDX);
    RecoNNAllEvalTree->Branch("wcMatchXPos", "std::vector<double>", &wcMatchXPos);
    RecoNNAllEvalTree->Branch("wcMatchYPos", "std::vector<double>", &wcMatchYPos);
    RecoNNAllEvalTree->Branch("wcMatchZPos", "std::vector<double>", &wcMatchZPos);

    RecoNNAllEvalTree->Branch("WC2TPCLocationsX", "std::vector<double>", &WC2TPCLocationsX);
    RecoNNAllEvalTree->Branch("WC2TPCLocationsY", "std::vector<double>", &WC2TPCLocationsY);
    RecoNNAllEvalTree->Branch("WC2TPCLocationsZ", "std::vector<double>", &WC2TPCLocationsZ);

    RecoNNAllEvalTree->Branch("isTrackInverted", "std::vector<bool>", &isTrackInverted);
    RecoNNAllEvalTree->Branch("isTrackNearVertex", "std::vector<bool>", &isTrackNearVertex);
    RecoNNAllEvalTree->Branch("recoTaggedAs", "std::vector<int>", &recoTaggedAs);
    RecoNNAllEvalTree->Branch("recoBeginX", "std::vector<double>", &recoBeginX);
    RecoNNAllEvalTree->Branch("recoBeginY", "std::vector<double>", &recoBeginY);
    RecoNNAllEvalTree->Branch("recoBeginZ", "std::vector<double>", &recoBeginZ);
    RecoNNAllEvalTree->Branch("recoEndX", "std::vector<double>", &recoEndX);
    RecoNNAllEvalTree->Branch("recoEndY", "std::vector<double>", &recoEndY);
    RecoNNAllEvalTree->Branch("recoEndZ", "std::vector<double>", &recoEndZ);
    RecoNNAllEvalTree->Branch("recoTrkID", "std::vector<int>", &recoTrkID);
    RecoNNAllEvalTree->Branch("recoPionChi2", "std::vector<double>", &recoPionChi2);
    RecoNNAllEvalTree->Branch("recoProtonChi2", "std::vector<double>", &recoProtonChi2);

    RecoNNAllEvalTree->Branch("matchedIdentity", "std::vector<int>", &matchedIdentity);
    RecoNNAllEvalTree->Branch("matchedCleanliness", "std::vector<double>", &matchedCleanliness);
    RecoNNAllEvalTree->Branch("matchedCompleteness", "std::vector<double>", &matchedCompleteness);
    RecoNNAllEvalTree->Branch("matchedTrkID", "std::vector<int>", &matchedTrkID);
    RecoNNAllEvalTree->Branch("matchedProcess", "std::vector<std::string>", &matchedProcess);

    RecoNNAllEvalTree->Branch("matchedBeginX", "std::vector<double>", &matchedBeginX);
    RecoNNAllEvalTree->Branch("matchedBeginY", "std::vector<double>", &matchedBeginY);
    RecoNNAllEvalTree->Branch("matchedBeginY", "std::vector<double>", &matchedBeginY);
    RecoNNAllEvalTree->Branch("matchedEndX", "std::vector<double>", &matchedEndX);
    RecoNNAllEvalTree->Branch("matchedEndY", "std::vector<double>", &matchedEndY);
    RecoNNAllEvalTree->Branch("matchedEndZ", "std::vector<double>", &matchedEndZ);
    RecoNNAllEvalTree->Branch("matchedRealEndX", "std::vector<double>", &matchedRealEndX);
    RecoNNAllEvalTree->Branch("matchedRealEndY", "std::vector<double>", &matchedRealEndY);
    RecoNNAllEvalTree->Branch("matchedRealEndZ", "std::vector<double>", &matchedRealEndZ);
    RecoNNAllEvalTree->Branch("matchedLength", "std::vector<double>", &matchedLength);

    RecoNNAllEvalTree->Branch("matchedKEnergy", "std::vector<double>", &matchedKEnergy);
    RecoNNAllEvalTree->Branch("matchedEndingKEnergy", "std::vector<double>", &matchedEndingKEnergy);

    RecoNNAllEvalTree->Branch("matchedInitialPx", "std::vector<double>", &matchedInitialPx);
    RecoNNAllEvalTree->Branch("matchedInitialPy", "std::vector<double>", &matchedInitialPy);
    RecoNNAllEvalTree->Branch("matchedInitialPz", "std::vector<double>", &matchedInitialPz);
    RecoNNAllEvalTree->Branch("matchedEndingPx", "std::vector<double>", &matchedEndingPx);
    RecoNNAllEvalTree->Branch("matchedEndingPy", "std::vector<double>", &matchedEndingPy);
    RecoNNAllEvalTree->Branch("matchedEndingPz", "std::vector<double>", &matchedEndingPz);

    RecoNNAllEvalTree->Branch("recoDEDX","std::vector<std::vector<double>>",&recoDEDX);
    RecoNNAllEvalTree->Branch("recoResR","std::vector<std::vector<double>>",&recoResR);
    RecoNNAllEvalTree->Branch("recoEDep","std::vector<std::vector<double>>",&recoEDep);
    RecoNNAllEvalTree->Branch("recoXPos","std::vector<std::vector<double>>",&recoXPos);
    RecoNNAllEvalTree->Branch("recoYPos","std::vector<std::vector<double>>",&recoYPos);
    RecoNNAllEvalTree->Branch("recoZPos","std::vector<std::vector<double>>",&recoZPos);
    RecoNNAllEvalTree->Branch("recoMeanDEDX","std::vector<double>",&recoMeanDEDX);

    RecoNNAllEvalTree->Branch("passesPionInRedVolume", &passesPionInRedVolume, "passesPionInRedVolume/O");
    RecoNNAllEvalTree->Branch("passesNoOutgoingPion", &passesNoOutgoingPion, "passesNoOutgoingPion/O");
    RecoNNAllEvalTree->Branch("passesSmallTracksCut", &passesSmallTracksCut, "passesSmallTracksCut/O");
    RecoNNAllEvalTree->Branch("passesMeanCurvatureCut", &passesMeanCurvatureCut, "passesMeanCurvatureCut/O");

    RecoNNAllEvalTree->Branch("numTaggedAsPions", &numTaggedAsPions, "numTaggedAsPions/I");
    RecoNNAllEvalTree->Branch("numTaggedAsProton", &numTaggedAsProton, "numTaggedAsProton/I");
    RecoNNAllEvalTree->Branch("numNotTagged", &numNotTagged, "numTaggenumNotTaggeddAsPions/I");

    RecoNNAllEvalTree->Branch("fHitlist", "std::vector<art::Ptr<recob::Hit>>", &fHitlist);
    RecoNNAllEvalTree->Branch("fHitKey", "std::vector<int>", &fHitKey);
    RecoNNAllEvalTree->Branch("fHitPlane", "std::vector<int>", &fHitPlane);
    RecoNNAllEvalTree->Branch("fHitT", "std::vector<float>", &fHitT);
    RecoNNAllEvalTree->Branch("fHitX", "std::vector<float>", &fHitX);
    RecoNNAllEvalTree->Branch("fHitW", "std::vector<float>", &fHitW);
    RecoNNAllEvalTree->Branch("fHitCharge", "std::vector<float>", &fHitCharge);
    RecoNNAllEvalTree->Branch("fHitChargeCol", "std::vector<float>", &fHitChargeCol);

    RecoNNAllEvalTree->Branch("hitRecoAsTrackKey", "std::vector<int>", &hitRecoAsTrackKey);
    RecoNNAllEvalTree->Branch("hitWC2TPCKey", "std::vector<int>", &hitWC2TPCKey);
    RecoNNAllEvalTree->Branch("hitThroughTrack", "std::vector<int>", &hitThroughTrack);
    RecoNNAllEvalTree->Branch("primaryEndPointHitX", &primaryEndPointHitX, "primaryEndPointHitX/D");
    RecoNNAllEvalTree->Branch("primaryEndPointHitW", &primaryEndPointHitW, "primaryEndPointHitW/D");

    RecoNNAllEvalTree->Branch("interactionInTrajectory", &interactionInTrajectory, "interactionInTrajectory/O");
    RecoNNAllEvalTree->Branch("trajectoryInteractionLabel", "std::string", &trajectoryInteractionLabel);
    RecoNNAllEvalTree->Branch("trajectoryInteractionAngle", &trajectoryInteractionAngle, "trajectoryInteractionAngle/D");

    RecoNNAllEvalTree->Branch("trajectoryInteractionX", &trajectoryInteractionX, "trajectoryInteractionX/D");
    RecoNNAllEvalTree->Branch("trajectoryInteractionY", &trajectoryInteractionY, "trajectoryInteractionY/D");
    RecoNNAllEvalTree->Branch("trajectoryInteractionZ", &trajectoryInteractionZ, "trajectoryInteractionZ/D");
    RecoNNAllEvalTree->Branch("trajectoryInteractionKE", &trajectoryInteractionKE, "trajectoryInteractionKE/D");
    RecoNNAllEvalTree->Branch("trajectoryInitialMomentumX", &trajectoryInitialMomentumX, "trajectoryInitialMomentumX/D");

    RecoNNAllEvalTree->Branch("validTrueIncidentKE", &validTrueIncidentKE, "validTrueIncidentKE/O");
    RecoNNAllEvalTree->Branch("trueIncidentKEContributions", "std::vector<double>", &trueIncidentKEContributions);

    RecoNNAllEvalTree->Branch("chExchShowerIDs", "std::vector<int>", &chExchShowerIDs);
    RecoNNAllEvalTree->Branch("chExchShowerProcesses", "std::vector<std::string>", &chExchShowerProcesses);
    RecoNNAllEvalTree->Branch("chExchShowerPDGs", "std::vector<int>", &chExchShowerPDGs);
    RecoNNAllEvalTree->Branch("chExchShowerLengths", "std::vector<double>", &chExchShowerLengths);
}

unsigned int RecoNNAllEval::lastPointInTPC(simb::MCParticle *track) {
  for (int i=track->NumberTrajectoryPoints()-1; i >=0; i--) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return last point
  } 
  return 9999;
}

unsigned int RecoNNAllEval::lastPointInTPC(const art::Ptr<simb::MCParticle> track) {
  for (int i=track->NumberTrajectoryPoints()-1; i >=0; i--) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return last point
  } 
  return 9999;
}

unsigned int RecoNNAllEval::firstPointInTPC(simb::MCParticle *track) {
  for (unsigned int i=0; i < track->NumberTrajectoryPoints(); ++i) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return first point
  } 
  return 9999;
}

unsigned int RecoNNAllEval::firstPointInTPC(const art::Ptr<simb::MCParticle> track) {
  for (unsigned int i=0; i < track->NumberTrajectoryPoints(); ++i) {
    if (
      (track->Vx(i)>minX) && (track->Vx(i)<maxX) && 
      (track->Vy(i)>minY) && (track->Vy(i)<maxY) && 
      (track->Vz(i)>minZ) && (track->Vz(i)<maxZ)
    ) return i; // If present in TPC, return first point
  } 
  return 9999;
}

bool RecoNNAllEval::isPosterityOfPrimary(simb::MCParticle *particle, const sim::ParticleList& plist) {
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

double RecoNNAllEval::distance(double x1, double x2, double y1, double y2, double z1, double z2) {
    return sqrt(
        pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)
    );
}

double RecoNNAllEval::trackMagnitude(const art::Ptr<simb::MCParticle> track, unsigned int cut1, unsigned int cut2) {
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double RecoNNAllEval::trackMagnitude(simb::MCParticle *track, unsigned int cut1, unsigned int cut2) {
  return sqrt(
    pow(track->Vx(cut2)-track->Vx(cut1),2) + 
    pow(track->Vy(cut2)-track->Vy(cut1),2) + 
    pow(track->Vz(cut2)-track->Vz(cut1),2)
  );
}

double RecoNNAllEval::trackMagnitude(simb::MCParticle *track) {
  return sqrt(
    pow(track->EndX()-track->Vx(0),2) + 
    pow(track->EndY()-track->Vy(0),2) + 
    pow(track->EndZ()-track->Vz(0),2)
  );
}

bool RecoNNAllEval::isWithinActiveVolume(double x, double y, double z) {
    if (x < minX ) return false; 
    if (x > maxX ) return false;
    if (y < minY ) return false; 
    if (y > maxY ) return false;
    if (z < minZ ) return false; 
    if (z > maxZ ) return false;
    return true;
}

bool RecoNNAllEval::isWithinReducedVolume(simb::MCParticle *track) {
    return (
      (track->EndX()>RminX) && (track->EndX()<RmaxX) && 
      (track->EndY()>RminY) && (track->EndY()<RmaxY) && 
      (track->EndZ()>RminZ) && (track->EndZ()<RmaxZ)
    );
}

bool RecoNNAllEval::isWithinReducedVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}
double RecoNNAllEval::energyLossCalculation(double x, double px) {
    // x in cm and px in MeV
    double discriminant = 0.0733 * px + 1.3 * x - 31; 
    if (discriminant > 0) {
        // particles going through the halo hole
        return 24.5;
    } else {
        // particles going through the halo paddle
        return 32.5;
    }
}

void RecoNNAllEval::fillSignalInformation(
    int pdg,
    double vx, double vy, double vz,
    bool interactionInTrajectory,
    std::string trajectoryInteractionLabel,
    std::vector<int> daughtersPDG, 
    std::vector<std::string> daughtersProcess, 
    std::vector<double> daughtersKE
) {
    bool isPionAbsorptionSignalTemp = true;

    if (pdg != -211) isPionAbsorptionSignalTemp = false;
    if (!isWithinReducedVolume(vx, vy, vz)) isPionAbsorptionSignalTemp = false;

    int numDaughters = daughtersPDG.size();
    numVisibleProtons = 0;
    for (int iDaughter = 0; iDaughter < numDaughters; iDaughter++) {
        if ((daughtersPDG[iDaughter] == 11) && (daughtersProcess[iDaughter] == "hIoni")) { continue; }
        else if ((daughtersPDG[iDaughter] == 111) || (daughtersPDG[iDaughter] == 211) || (daughtersPDG[iDaughter] == -211)) { isPionAbsorptionSignalTemp = false; }
        else if ((daughtersPDG[iDaughter] == 13) || (daughtersPDG[iDaughter] == -13)) { isPionAbsorptionSignalTemp = false; } // muon
        else if ((daughtersPDG[iDaughter] == 321) || (daughtersPDG[iDaughter] == -321) || (daughtersPDG[iDaughter] == 311)) { isPionAbsorptionSignalTemp = false; } // kaon
        else if ((daughtersProcess[iDaughter] == "Decay") || (daughtersProcess[iDaughter] == "hBertiniCaptureAtRest")) { isPionAbsorptionSignalTemp = false; }
        else if (
            daughtersProcess[iDaughter] == "pi-Inelastic" &&
            daughtersPDG[iDaughter] == 2212 &&
            daughtersKE[iDaughter] >= PROTON_ENERGY_LOWER_BOUND &&
            daughtersKE[iDaughter] <= PROTON_ENERGY_UPPER_BOUND
        ) {
            numVisibleProtons++;
        }
    }

    // If we have elastic scattering in trajectory, we do not consider it a signal
    if (interactionInTrajectory && trajectoryInteractionLabel == "hadElastic") isPionAbsorptionSignalTemp = false;

    if (isPionAbsorptionSignalTemp) {
        // Event is signal!
        isPionAbsorptionSignal = true;
    } else {
        // Event is background, classify it
        fillBackgroundInformation(
            pdg,
            vx, vy, vz,
            interactionInTrajectory,
            trajectoryInteractionLabel,
            daughtersPDG,
            daughtersProcess,
            daughtersKE
        );
    }
    return;
}

void RecoNNAllEval::fillBackgroundInformation(
    int pdg,
    double vx, double vy, double vz,
    bool interactionInTrajectory,
    std::string trajectoryInteractionLabel,
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

    // Check for elastic scattering in trajectory (already checked it's inside reduced volume)
    if (interactionInTrajectory && trajectoryInteractionLabel == "hadElastic") { backgroundType = 12; return; }

    // If interaction not in reduced volume, flag as outside reduced volume
    if (!isWithinReducedVolume(vx, vy, vz)) { backgroundType = 5; return; }

    // If interaction is inside reduced volume, check daughters
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

    // If not flagged at this point, label as other
    if (backgroundType == -1) backgroundType = 11;
}

std::tuple<double, double> RecoNNAllEval::computeCurvature(recob::Track track) {
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

double RecoNNAllEval::curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3) {
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

double RecoNNAllEval::meanDEDX(
    art::FindManyP<anab::Calorimetry> fmcal, 
    unsigned int trackKey, 
    bool isThisTrackReversed,
    std::vector<double>& trackDEDX,
    std::vector<double>& trackResR,
    std::vector<double>& trackEDep,
    std::vector<double>& trackXPos, 
    std::vector<double>& trackYPos,
    std::vector<double>& trackZPos
) {
    // Temporary storage for this reco track
    // std::vector<double> recoPitch_v; 
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

                // recoPitch_v.push_back(calos[j]->TrkPitchVec()[k]);
                recoDEDX_v.push_back(calos[j]->dEdx()[k]);
                recoEDep_v.push_back(calos[j]->dEdx()[k] * calos[j]->TrkPitchVec()[k]);
                recoResR_v.push_back(calos[j]->ResidualRange()[k]);
                recoXPos_v.push_back(calos[j]->XYZ()[k].X());
                recoYPos_v.push_back(calos[j]->XYZ()[k].Y());
                recoZPos_v.push_back(calos[j]->XYZ()[k].Z());
            } // end loop on calo points

            if (bVerbose) std::cout << "Filled calorimetry vectors" << std::endl;
            if (isThisTrackReversed) {
                // std::reverse(recoPitch_v.begin(), recoPitch_v.end());
                std::reverse(recoResR_v.begin(), recoResR_v.end());
                std::reverse(recoDEDX_v.begin(), recoDEDX_v.end());
                std::reverse(recoEDep_v.begin(), recoEDep_v.end());
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
    if (bound != 0) meanDEDX /= bound;

    trackDEDX = recoDEDX_v;
    trackResR = recoResR_v;
    trackEDep = recoEDep_v;
    trackXPos = recoXPos_v;
    trackYPos = recoYPos_v;
    trackZPos = recoZPos_v;
    
    return meanDEDX;
}

double RecoNNAllEval::computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints) {
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

void RecoNNAllEval::initializeProtonPoints(TGraph* gProton) {
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

void RecoNNAllEval::initializePionPoints(TGraph* gPion) {
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


void RecoNNAllEval::resetTree() {
    numTaggedAsPions  = 0;
    numTaggedAsProton = 0;
    numNotTagged      = 0;

    passesPionInRedVolume  = false;
    passesNoOutgoingPion   = false;
    passesSmallTracksCut   = false;
    passesMeanCurvatureCut = false;

    trackProb             = 1.;
    showerProb            = 0.;
    obtainedProbabilities = false;

    showerNoBoxProb = 0.;
    obtainedNoBoxProbabilities = false;

    showerOutsideBoxProb = 0.;
    obtainedOutsideBoxProbabilities = false;

    WC2TPCtrkID = -99999;

    wcMatchPDG = -99999;
    wcMatchProcess = "";
    wcMatchDaughtersPDG.clear();
    wcMatchDaughtersProcess.clear();
    wcMatchDEDX.clear();
    wcMatchResR.clear();
    wcMatchEDep.clear();
    wcMatchXPos.clear();
    wcMatchYPos.clear();
    wcMatchZPos.clear();

    WC2TPCLocationsX.clear();
    WC2TPCLocationsY.clear();
    WC2TPCLocationsZ.clear();
    
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
    recoEDep.clear();
    recoXPos.clear();
    recoYPos.clear();
    recoZPos.clear();
    recoMeanDEDX.clear();

    truthPrimaryID         = -99999;
    truthPrimaryPDG        = -99999;
    truthPrimaryIncidentKE = -99999;
    truthPrimaryVertexKE   = -99999;
    truthPrimaryVertexX    = -99999;
    truthPrimaryVertexY    = -99999;
    truthPrimaryVertexZ    = -99999;
    truthPrimaryDaughtersID.clear();
    truthPrimaryDaughtersPDG.clear();
    truthPrimaryDaughtersProcess.clear();
    truthPrimaryDaughtersKE.clear();
    
    truthScatteringAngle     = -99999;
    truthScatteredPionLength = -99999;
    truthScatteredPionKE     = -99999;
    truthSecondaryVertexX    = -99999;
    truthSecondaryVertexY    = -99999;
    truthSecondaryVertexZ    = -99999;
    truthSecondaryPionDaughtersPDG.clear();
    truthSecondaryPionDaughtersProcess.clear();
    truthSecondaryPionDaughtersKE.clear();

    isPionAbsorptionSignal = false;
    numVisibleProtons      = 0;
    backgroundType         = -1;

    fHitlist.clear();
    fHitKey.clear();
    fHitPlane.clear();
    fHitT.clear();
    fHitX.clear();
    fHitW.clear();
    fHitCharge.clear();
    fHitChargeCol.clear();

    hitRecoAsTrackKey.clear();
    hitWC2TPCKey.clear();
    hitThroughTrack.clear();
    primaryEndPointHitX = 0.;
    primaryEndPointHitW = 0.;

    interactionInTrajectory    = false;
    trajectoryInteractionLabel = "";
    trajectoryInteractionAngle = 0.0;
    trajectoryInteractionX     = -99999.;
    trajectoryInteractionY     = -99999.;
    trajectoryInteractionZ     = -99999.;
    trajectoryInteractionKE    = -99999.;
    trajectoryInitialMomentumX = -99999.;

    validTrueIncidentKE = false;
    trueIncidentKEContributions.clear();
    
    chExchShowerIDs.clear();
    chExchShowerProcesses.clear();
    chExchShowerPDGs.clear();
    chExchShowerLengths.clear();
}

void RecoNNAllEval::endJob() {
    delete gProton;
    delete gPion;
    delete fRand;
}

void RecoNNAllEval::reconfigure(fhicl::ParameterSet const & p) {
    bVerbose = p.get<bool>("Verbose", false);
    strWCTrackBuilderLabel             = p.get<std::string>("WCTrackBuilderLabel", "wctrack");
    strTPCTrackHandleLabel             = p.get<std::string>("TPCTrackHandleLabel", "pmtrack");
    strWC2TPCModuleLabel               = p.get<std::string>("WC2TPModuleLabel", "wctracktpctrackmatch");
    strCalorimetryModuleLabel          = p.get<std::string>("CalorimetryModuleLabel", "calo");
    simulation_producer_label_         = p.get<std::string>("SimulationLabel", "largeant");
    recotrackmcparticlematching_label_ = p.get<std::string>("RecoTrackMCMatchLabel", "recotrackmcmatching");
    fHitsModule                        = p.get< std::string>("HitsModule", "gaushit");
    fHitsInstance                      = p.get< std::string>("HitsInstance", "");
    MeanDEDXNumberTrajPoints           = p.get<unsigned int>("MeanDEDXNumberTrajPoints", 20);
    TrackStitchingThreshold            = p.get<double> ("TrackStitchingThreshold",4);
    fMeanDEDXThreshold                 = p.get<double>("MeanDEDXThreshold", 5.0);
    fVertexRadius                      = p.get<double>("VertexRadius", 4);
    SmallTrackLength                   = p.get<double>("SmallTrackLength", 35);
    MaxSmallTracks                     = p.get<int>("MaxSmallTracks", 5);
    MeanCurvatureThreshold             = p.get<double>("MeanCurvatureThreshold", 0.015);
    fNNetModuleLabel                   = p.get<std::string>("NNetModuleLabel", "emtrkmichelid:emtrkmichel:EmTrackId");
    PROTON_ENERGY_LOWER_BOUND = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND = p.get<float>("ProtonEnergyUpperBound", 1.0);
    PION_CHI2_PION_VALUE     = p.get<double>("PionChi2PionValue", 3.);
    PION_CHI2_PROTON_VALUE   = p.get<double>("PionChi2ProtonValue", 1.);
    PROTON_CHI2_PION_VALUE   = p.get<double>("ProtonChi2PionValue", 3.);
    PROTON_CHI2_PROTON_VALUE = p.get<double>("ProtonChi2ProtonValue", 5.);
}

DEFINE_ART_MODULE(RecoNNAllEval)