//////////////////////////////////////////////////////////////////////////
// Class:       RecoNNDataEval
// Module Type: analyzer
// File:        RecoNNDataEval_module.cc
//
// Written by Emilio Peláez, created on January 2025. Adapted from XSAnalysis module
// and RecoNNDataEval module by Matt King.
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
#include "LArIATDataProducts/TOF.h"
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

class RecoNNDataEval : public art::EDAnalyzer {
    public: 
        explicit RecoNNDataEval(fhicl::ParameterSet const &p);
        RecoNNDataEval(RecoNNDataEval const &) = delete;
        RecoNNDataEval(RecoNNDataEval &&) = delete;
        RecoNNDataEval & operator = (RecoNNDataEval const &) = delete;
        RecoNNDataEval & operator = (RecoNNDataEval &&) = delete;

        // Required functions
        void analyze(art::Event const &e) override;

        // Optional functions
        void beginJob() override;
        void endJob() override;
        void reconfigure(fhicl::ParameterSet const &p);

        // Helper functions
        void resetTree();

        bool isWithinActiveVolume(double x, double y, double z);
        bool isWithinReducedVolume(double x, double y, double z);
        double meanDEDX(art::FindManyP<anab::Calorimetry> fmcal, unsigned int trackKey, bool isThisTrackReversed, std::vector<double>& trackDEDX, std::vector<double>& trackResR, std::vector<double>& trackEDep, std::vector<double>& trackXPos, std::vector<double>& trackYPos, std::vector<double>& trackZPos);
        double distance(double x1, double x2, double y1, double y2, double z1, double z2);
        double curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3);
        std::tuple<double, double> computeCurvature(recob::Track track);
        void initializeProtonPoints(TGraph *gProton);
        void initializePionPoints(TGraph *gPion);
        double computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints);

    private: 
        // Product's names
        std::string strWCTrackBuilderLabel;
        std::string strTPCTrackHandleLabel;
        std::string strWC2TPCModuleLabel;
        std::string strCalorimetryModuleLabel;
        std::string fHitsModule;
        std::string fHitsInstance;
        std::string fNNetModuleLabel;
        std::string fTOFModuleLabel;

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
        TTree *RecoNNDataEvalTree;

        // Event metadata
        int run; 
        int subrun;
        int event;
        bool isData;

        // Cut information
        bool passesPionInRedVolume;
        bool passesNoOutgoingPion;
        bool passesSmallTracksCut;
        bool passesMeanCurvatureCut;

        // Background information
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

        std::vector<double>      wcMatchResR;
        std::vector<double>      wcMatchEDep;
        std::vector<double>      wcMatchDEDX;
        std::vector<double>      wcMatchXPos;
        std::vector<double>      wcMatchYPos;
        std::vector<double>      wcMatchZPos;

        // TOF variables
        double tofObject;
        double distanceTraveled;
        double TOFMass;

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

RecoNNDataEval::RecoNNDataEval(fhicl::ParameterSet const &p) : EDAnalyzer(p) ,fCaloAlg(p.get<fhicl::ParameterSet>("CaloAlg")) {
    this->reconfigure(p);

    // Initialize detprop pointer
    fDetProp = lar::providerFrom<detinfo::DetectorPropertiesService>();
}

void RecoNNDataEval::analyze(art::Event const &e) {
    resetTree();

    run = e.run(); subrun = e.subRun(); event = e.event(); isData = e.isRealData();
    if (bVerbose) std::cout << "Run: " << run << ", subrun: " << subrun << ", event: " << event << ", is real data: " << isData << std::endl;
    if (bVerbose) std::cout << std::endl;

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

    ///////////////////////
    // Get data products //
    ///////////////////////

    // Get NN data
    anab::MVAReader<recob::Hit, 4> hitResults(e, fNNetModuleLabel);
    std::vector<anab::FeatureVector<4>> featVec = hitResults.outputs();

    // Get hits information
    art::Handle< std::vector<recob::Hit> > hitListHandle;
    if (e.getByLabel(fHitsModule, fHitsInstance, hitListHandle)) { art::fill_ptr_vector(fHitlist, hitListHandle); }
    size_t nWireHits = fHitlist.size();

    // Get wire chamber data
    art::Handle<std::vector<ldp::WCTrack>> wctrackHandle;
    std::vector<art::Ptr<ldp::WCTrack>>    wctrack;
    // If there is no wire chamber tracks for label, return
    if (!e.getByLabel(strWCTrackBuilderLabel, wctrackHandle)) return;
    art::fill_ptr_vector(wctrack, wctrackHandle);

    int numWCtrks = wctrack.size(); // number of wire chamber tracks
    if (numWCtrks != 1) return; 

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

    if (bVerbose) std::cout << "WC Track Momentum: " << WCTrackMomentum << std::endl;

    ////////////////
    // TPC tracks //
    ////////////////

    art::Handle<std::vector<recob::Track>> tpcTrackHandle;
    std::vector<art::Ptr<recob::Track>> tracklist;
    // If there are no tpc chamber tracks for label, return
    if (!e.getByLabel(strTPCTrackHandleLabel, tpcTrackHandle)) return; 
    art::fill_ptr_vector(tracklist, tpcTrackHandle);

    // Get hits associated with TPC tracks
    art::FindManyP<recob::Hit> HitsInTrack(tpcTrackHandle, e, strTPCTrackHandleLabel);

    int numTracksReco = tracklist.size();
    if (!numTracksReco) return; // If no TPC tracks, return
    
    if (bVerbose) std::cout << "Number of TPC reco tracks: " << numTracksReco << std::endl;
    if (bVerbose) std::cout << std::endl;

    /////////////////////
    // WC2TPC Matching //
    /////////////////////

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

    // Geometry service
    art::ServiceHandle<geo::Geometry> geom;

    // Load geometry
    double USTOF_Center[3];
    double WC1_Center[3];
    double WC2_Center[3];
    double WC3_Center[3];
    double WC4_Center[3];
    double DSTOF_Center[3];

    for(size_t iDet = 0; iDet < geom->NAuxDets(); ++iDet){
        geo::AuxDetGeo const& anAuxDetGeo = geom->AuxDet(iDet);
        std::string detName = anAuxDetGeo.Name();
        if(detName == "volAuxDetTOFUS")        anAuxDetGeo.GetCenter(USTOF_Center);
        if(detName == "volAuxDetSensitiveWC1") anAuxDetGeo.GetCenter(WC1_Center);
        if(detName == "volAuxDetSensitiveWC2") anAuxDetGeo.GetCenter(WC2_Center);
        if(detName == "volAuxDetSensitiveWC3") anAuxDetGeo.GetCenter(WC3_Center);
        if(detName == "volAuxDetSensitiveWC4") anAuxDetGeo.GetCenter(WC4_Center);
        if(detName == "volAuxDetTOFDS")        anAuxDetGeo.GetCenter(DSTOF_Center);
    }

    // Get TOF data products
    art::Handle<std::vector<ldp::TOF>> TOFColHandle;
    std::vector<art::Ptr<ldp::TOF>> tof;
    if (!e.getByLabel(fTOFModuleLabel, TOFColHandle)) return;
    art::fill_ptr_vector(tof, TOFColHandle);
    if (tof.size() != 1) return;
    else if (tof[0]->NTOF() != 1) return;

    // Compute mass with TOF
    tofObject        = tof[0]->SingleTOF(0);
    distanceTraveled = TMath::Sqrt(
        (USTOF_Center[0] - DSTOF_Center[0]) * (USTOF_Center[0] - DSTOF_Center[0]) + 
        (USTOF_Center[1] - DSTOF_Center[1]) * (USTOF_Center[1] - DSTOF_Center[1]) + 
        (USTOF_Center[2] - DSTOF_Center[2]) * (USTOF_Center[2] - DSTOF_Center[2])
    );

    double radical = tofObject * 29.9792458 * 29.9792458 * tofObject / (distanceTraveled * distanceTraveled) - 1;
    if (tofObject > 0) {
        if (radical < 0) {
            TOFMass = -WCTrackMomentum * pow(-radical, 0.5);
        } else {
            TOFMass = WCTrackMomentum * pow(radical, 0.5);
        }
    }

    if (bVerbose) std::cout << "TOF Mass: " << TOFMass << std::endl;

    // Define calorimetry
    art::FindManyP<anab::Calorimetry> fmcal(tpcTrackHandle, e, strCalorimetryModuleLabel);

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
        if (thisTrack->ID() == WC2TPCtrkID) continue;
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

    RecoNNDataEvalTree->Fill();
}

void RecoNNDataEval::beginJob() {
    if (bVerbose) std::cout << "Beginning job." << std::endl;
    art::ServiceHandle<art::TFileService> tfs;

    // Initialize chi^2 graphs
    initializeProtonPoints(gProton);
    initializePionPoints(gPion);

    RecoNNDataEvalTree = tfs->make<TTree>("RecoNNDataEvalTree", "RecoNNDataEvalTree");

    RecoNNDataEvalTree->Branch("run", &run, "run/I");
    RecoNNDataEvalTree->Branch("subrun", &subrun, "subrun/I");
    RecoNNDataEvalTree->Branch("event", &event, "event/I");
    RecoNNDataEvalTree->Branch("isData", &isData, "isData/O");

    RecoNNDataEvalTree->Branch("trackProb", &trackProb, "trackProb/D");
    RecoNNDataEvalTree->Branch("showerProb", &showerProb, "showerProb/D");
    RecoNNDataEvalTree->Branch("obtainedProbabilities", &obtainedProbabilities, "obtainedProbabilities/O");

    RecoNNDataEvalTree->Branch("showerNoBoxProb", &showerNoBoxProb, "showerNoBoxProb/D");
    RecoNNDataEvalTree->Branch("obtainedNoBoxProbabilities", &obtainedNoBoxProbabilities, "obtainedNoBoxProbabilities/O");

    RecoNNDataEvalTree->Branch("showerOutsideBoxProb", &showerOutsideBoxProb, "showerOutsideBoxProb/D");
    RecoNNDataEvalTree->Branch("obtainedOutsideBoxProbabilities", &obtainedOutsideBoxProbabilities, "obtainedOutsideBoxProbabilities/O");

    RecoNNDataEvalTree->Branch("WC2TPCtrkID", &WC2TPCtrkID, "WC2TPCtrkID/I");
    RecoNNDataEvalTree->Branch("WCTrackMomentum", &WCTrackMomentum, "WCTrackMomentum/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryBeginX", &WC2TPCPrimaryBeginX, "WC2TPCPrimaryBeginX/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryBeginY", &WC2TPCPrimaryBeginY, "WC2TPCPrimaryBeginY/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryBeginZ", &WC2TPCPrimaryBeginZ, "WC2TPCPrimaryBeginZ/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryEndX", &WC2TPCPrimaryEndX, "WC2TPCPrimaryEndX/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryEndY", &WC2TPCPrimaryEndY, "WC2TPCPrimaryEndY/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryEndZ", &WC2TPCPrimaryEndZ, "WC2TPCPrimaryEndZ/D");
    RecoNNDataEvalTree->Branch("WC2TPCPrimaryLength", &WC2TPCPrimaryLength, "WC2TPCPrimaryLength/D");

    RecoNNDataEvalTree->Branch("WC3PrimaryX", &WC3PrimaryX, "WC3PrimaryX/D");
    RecoNNDataEvalTree->Branch("WC3PrimaryY", &WC3PrimaryY, "WC3PrimaryY/D");
    RecoNNDataEvalTree->Branch("WC3PrimaryZ", &WC3PrimaryZ, "WC3PrimaryZ/D");
    RecoNNDataEvalTree->Branch("WC4PrimaryX", &WC4PrimaryX, "WC4PrimaryX/D");
    RecoNNDataEvalTree->Branch("WC4PrimaryY", &WC4PrimaryY, "WC4PrimaryY/D");
    RecoNNDataEvalTree->Branch("WC4PrimaryZ", &WC4PrimaryZ, "WC4PrimaryZ/D");
    RecoNNDataEvalTree->Branch("WCMeanCurvature", &WCMeanCurvature, "WCMeanCurvature/D");
    RecoNNDataEvalTree->Branch("WCMaxCurvature", &WCMaxCurvature, "WCMaxCurvature/D");
    RecoNNDataEvalTree->Branch("WCTheta", &WCTheta, "WCTheta/D");
    RecoNNDataEvalTree->Branch("WCPhi", &WCPhi, "WCPhi/D");

    RecoNNDataEvalTree->Branch("wcMatchResR", "std::vector<double>", &wcMatchResR);
    RecoNNDataEvalTree->Branch("wcMatchEDep", "std::vector<double>", &wcMatchEDep);
    RecoNNDataEvalTree->Branch("wcMatchDEDX", "std::vector<double>", &wcMatchDEDX);
    RecoNNDataEvalTree->Branch("wcMatchXPos", "std::vector<double>", &wcMatchXPos);
    RecoNNDataEvalTree->Branch("wcMatchYPos", "std::vector<double>", &wcMatchYPos);
    RecoNNDataEvalTree->Branch("wcMatchZPos", "std::vector<double>", &wcMatchZPos);

    RecoNNDataEvalTree->Branch("WC2TPCLocationsX", "std::vector<double>", &WC2TPCLocationsX);
    RecoNNDataEvalTree->Branch("WC2TPCLocationsY", "std::vector<double>", &WC2TPCLocationsY);
    RecoNNDataEvalTree->Branch("WC2TPCLocationsZ", "std::vector<double>", &WC2TPCLocationsZ);

    RecoNNDataEvalTree->Branch("isTrackInverted", "std::vector<bool>", &isTrackInverted);
    RecoNNDataEvalTree->Branch("isTrackNearVertex", "std::vector<bool>", &isTrackNearVertex);
    RecoNNDataEvalTree->Branch("recoTaggedAs", "std::vector<int>", &recoTaggedAs);
    RecoNNDataEvalTree->Branch("recoBeginX", "std::vector<double>", &recoBeginX);
    RecoNNDataEvalTree->Branch("recoBeginY", "std::vector<double>", &recoBeginY);
    RecoNNDataEvalTree->Branch("recoBeginZ", "std::vector<double>", &recoBeginZ);
    RecoNNDataEvalTree->Branch("recoEndX", "std::vector<double>", &recoEndX);
    RecoNNDataEvalTree->Branch("recoEndY", "std::vector<double>", &recoEndY);
    RecoNNDataEvalTree->Branch("recoEndZ", "std::vector<double>", &recoEndZ);
    RecoNNDataEvalTree->Branch("recoTrkID", "std::vector<int>", &recoTrkID);
    RecoNNDataEvalTree->Branch("recoPionChi2", "std::vector<double>", &recoPionChi2);
    RecoNNDataEvalTree->Branch("recoProtonChi2", "std::vector<double>", &recoProtonChi2);

    RecoNNDataEvalTree->Branch("recoDEDX","std::vector<std::vector<double>>",&recoDEDX);
    RecoNNDataEvalTree->Branch("recoResR","std::vector<std::vector<double>>",&recoResR);
    RecoNNDataEvalTree->Branch("recoEDep","std::vector<std::vector<double>>",&recoEDep);
    RecoNNDataEvalTree->Branch("recoXPos","std::vector<std::vector<double>>",&recoXPos);
    RecoNNDataEvalTree->Branch("recoYPos","std::vector<std::vector<double>>",&recoYPos);
    RecoNNDataEvalTree->Branch("recoZPos","std::vector<std::vector<double>>",&recoZPos);
    RecoNNDataEvalTree->Branch("recoMeanDEDX","std::vector<double>",&recoMeanDEDX);

    RecoNNDataEvalTree->Branch("passesPionInRedVolume", &passesPionInRedVolume, "passesPionInRedVolume/O");
    RecoNNDataEvalTree->Branch("passesNoOutgoingPion", &passesNoOutgoingPion, "passesNoOutgoingPion/O");
    RecoNNDataEvalTree->Branch("passesSmallTracksCut", &passesSmallTracksCut, "passesSmallTracksCut/O");
    RecoNNDataEvalTree->Branch("passesMeanCurvatureCut", &passesMeanCurvatureCut, "passesMeanCurvatureCut/O");

    RecoNNDataEvalTree->Branch("numTaggedAsPions", &numTaggedAsPions, "numTaggedAsPions/I");
    RecoNNDataEvalTree->Branch("numTaggedAsProton", &numTaggedAsProton, "numTaggedAsProton/I");
    RecoNNDataEvalTree->Branch("numNotTagged", &numNotTagged, "numTaggenumNotTaggeddAsPions/I");

    RecoNNDataEvalTree->Branch("fHitlist", "std::vector<art::Ptr<recob::Hit>>", &fHitlist);
    RecoNNDataEvalTree->Branch("fHitKey", "std::vector<int>", &fHitKey);
    RecoNNDataEvalTree->Branch("fHitPlane", "std::vector<int>", &fHitPlane);
    RecoNNDataEvalTree->Branch("fHitT", "std::vector<float>", &fHitT);
    RecoNNDataEvalTree->Branch("fHitX", "std::vector<float>", &fHitX);
    RecoNNDataEvalTree->Branch("fHitW", "std::vector<float>", &fHitW);
    RecoNNDataEvalTree->Branch("fHitCharge", "std::vector<float>", &fHitCharge);
    RecoNNDataEvalTree->Branch("fHitChargeCol", "std::vector<float>", &fHitChargeCol);

    RecoNNDataEvalTree->Branch("hitRecoAsTrackKey", "std::vector<int>", &hitRecoAsTrackKey);
    RecoNNDataEvalTree->Branch("hitWC2TPCKey", "std::vector<int>", &hitWC2TPCKey);
    RecoNNDataEvalTree->Branch("hitThroughTrack", "std::vector<int>", &hitThroughTrack);
    RecoNNDataEvalTree->Branch("primaryEndPointHitX", &primaryEndPointHitX, "primaryEndPointHitX/D");
    RecoNNDataEvalTree->Branch("primaryEndPointHitW", &primaryEndPointHitW, "primaryEndPointHitW/D");

    RecoNNDataEvalTree->Branch("tofObject", &tofObject, "tofObject/D");
    RecoNNDataEvalTree->Branch("distanceTraveled", &distanceTraveled, "distanceTraveled/D");
    RecoNNDataEvalTree->Branch("TOFMass", &TOFMass, "TOFMass/D");
}

double RecoNNDataEval::distance(double x1, double x2, double y1, double y2, double z1, double z2) {
    return sqrt(
        pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 - z2, 2)
    );
}

bool RecoNNDataEval::isWithinActiveVolume(double x, double y, double z) {
    if (x < minX ) return false; 
    if (x > maxX ) return false;
    if (y < minY ) return false; 
    if (y > maxY ) return false;
    if (z < minZ ) return false; 
    if (z > maxZ ) return false;
    return true;
}

bool RecoNNDataEval::isWithinReducedVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}

std::tuple<double, double> RecoNNDataEval::computeCurvature(recob::Track track) {
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

double RecoNNDataEval::curvatureForThreePoints(TVector3 p1, TVector3 p2, TVector3 p3) {
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

double RecoNNDataEval::meanDEDX(
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

double RecoNNDataEval::computeReducedChi2(const TGraph* theory, std::vector<double> xData, std::vector<double> yData, int nPoints) {
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

void RecoNNDataEval::initializeProtonPoints(TGraph* gProton) {
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

void RecoNNDataEval::initializePionPoints(TGraph* gPion) {
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


void RecoNNDataEval::resetTree() {
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

    recoDEDX.clear();
    recoResR.clear();
    recoEDep.clear();
    recoXPos.clear();
    recoYPos.clear();
    recoZPos.clear();
    recoMeanDEDX.clear();

    fHitlist.clear();
    fHitKey.clear();
    fHitPlane.clear();
    fHitT.clear();
    fHitX.clear();
    fHitW.clear();
    fHitCharge.clear();
    fHitChargeCol.clear();

    wcMatchDEDX.clear();
    wcMatchResR.clear();
    wcMatchEDep.clear();
    wcMatchXPos.clear();
    wcMatchYPos.clear();
    wcMatchZPos.clear();

    hitRecoAsTrackKey.clear();
    hitWC2TPCKey.clear();
    hitThroughTrack.clear();
    primaryEndPointHitX = 0.;
    primaryEndPointHitW = 0.;

    tofObject        = -1.;
    distanceTraveled = -1.;
    TOFMass          = -1.;
}

void RecoNNDataEval::endJob() {
    delete gProton;
    delete gPion;
    delete fRand;
}

void RecoNNDataEval::reconfigure(fhicl::ParameterSet const & p) {
    bVerbose = p.get<bool>("Verbose", false);
    strWCTrackBuilderLabel             = p.get<std::string>("WCTrackBuilderLabel", "wctrack");
    strTPCTrackHandleLabel             = p.get<std::string>("TPCTrackHandleLabel", "pmtrack");
    strWC2TPCModuleLabel               = p.get<std::string>("WC2TPModuleLabel", "wctracktpctrackmatch");
    strCalorimetryModuleLabel          = p.get<std::string>("CalorimetryModuleLabel", "calo");
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
    fTOFModuleLabel                    = p.get<std::string>("TOFModuleLabel", "tof");
    PROTON_ENERGY_LOWER_BOUND = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND = p.get<float>("ProtonEnergyUpperBound", 1.0);
    PION_CHI2_PION_VALUE     = p.get<double>("PionChi2PionValue", 3.);
    PION_CHI2_PROTON_VALUE   = p.get<double>("PionChi2ProtonValue", 1.);
    PROTON_CHI2_PION_VALUE   = p.get<double>("ProtonChi2PionValue", 3.);
    PROTON_CHI2_PROTON_VALUE = p.get<double>("ProtonChi2ProtonValue", 5.);
}

DEFINE_ART_MODULE(RecoNNDataEval)