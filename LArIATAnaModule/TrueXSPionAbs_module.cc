////////////////////////////////////////////////////////////////////////
// Class:       TrueXSPionAbs
// Module Type: analyzer
// File:        TrueXSPionAbs_module.cc
//
// Generated at Tue June 29 11:21:46 2017 by Elena Gramellini
////////////////////////////////////////////////////////////////////////

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
#include "art_root_io/TFileService.h"
#include "art_root_io/TFileDirectory.h"
#include "canvas/Persistency/Common/FindOneP.h" 
#include "canvas/Persistency/Common/FindManyP.h"
#include "messagefacility/MessageLogger/MessageLogger.h" 
//#include "cetlib/maybe_ref.h"
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "larcore/Geometry/Geometry.h"
#include "larcorealg/Geometry/CryostatGeo.h"
#include "larcorealg/Geometry/TPCGeo.h"
#include "larcorealg/Geometry/PlaneGeo.h"
#include "larcorealg/Geometry/WireGeo.h"
#include "larsim/MCCheater/BackTrackerService.h"
#include "lardataobj/Simulation/SimChannel.h"
#include "nusimdata/SimulationBase/MCTruth.h"
#include "larevt/Filters/ChannelFilter.h"
#include "RawDataUtilities/TriggerDigitUtility.h"

// ########################
// ### LArSoft includes ###
// ########################
#include "larcoreobj/SimpleTypesAndConstants/geo_types.h"
#include "larcoreobj/SimpleTypesAndConstants/RawTypes.h" // raw::ChannelID_t
#include "larcore/Geometry/Geometry.h"
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
#include "LArIATDataProducts/WCTrack.h"

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

#include "lardataobj/RawData/RawDigit.h"
#include "lardataobj/RawData/raw.h"

// #####################
// ### ROOT includes ###
// #####################
#include "TComplex.h"
#include "TFile.h"
#include "TVector3.h"
#include "TH2D.h"
#include "TMath.h"
#include "TF1.h"
#include "TTree.h"
#include "TTimeStamp.h"
#include <map>
#include <iostream>
#include <fstream>
#include <math.h>

// Type definitions
typedef std::map<int, art::Ptr<simb::MCParticle>> ParticleMap;

namespace lariat { class TrueXSPionAbs; }

class lariat::TrueXSPionAbs : public art::EDAnalyzer {
    public:
        explicit TrueXSPionAbs(fhicl::ParameterSet const & p);
        virtual ~TrueXSPionAbs();

        // Required functions.
        void analyze(art::Event const & e) override;

        // Selected optional functions.
        void beginJob();
        void endJob();
        void reconfigure(fhicl::ParameterSet const & p);
        double  distance(double, double, double, double, double, double );

        // Helper functions
        void resetTree();
        void fillSignalInformation(int pdg, double vx, double vy, double vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        void fillBackgroundInformation(int pdg, double vx, double vy, double vz, std::vector<int> daughtersPDG, std::vector<std::string> daughtersProcess, std::vector<double> daughtersKE);
        bool isWithinReducedVolume(double x, double y, double z);

    private:
        // Parameters
        bool verbose;
        float PROTON_ENERGY_LOWER_BOUND;
        float PROTON_ENERGY_UPPER_BOUND;
        std::string simulation_producer_label_;

        // Signal information
        bool isPionAbsorptionSignal = false;
        int  numVisibleProtons      = 0;
        int  backgroundType         = -1;

        // Truth primary information
        int                      truthPrimaryPDG;
        double                   truthPrimaryVertexX;
        double                   truthPrimaryVertexY;
        double                   truthPrimaryVertexZ;
        std::vector<int>         truthPrimaryDaughtersPDG;
        std::vector<std::string> truthPrimaryDaughtersProcess;
        std::vector<double>      truthPrimaryDaughtersKE;

        // Cross-section signal histograms
        TH1D *hCrossSection;
        TH1D *hCrossSectionEl;
        TH1D *hCrossSectionInel;
        TH1D *hCrossSectionPionAbs;
        TH1D *hCrossSectionPionAbs0p;
        TH1D *hCrossSectionPionAbsNp;

        // Cross-section background histograms
        TH1D *hCrossSectionOutsideRedVolume;
        TH1D *hCrossSectionPionInelastic;
        TH1D *hCrossSectionChargeExchange;
        TH1D *hCrossSectionDoubleChargeExchange;
        TH1D *hCrossSectionCaptureAtRest;
        TH1D *hCrossSectionDecay;
        TH1D *hCrossSectionOther;

        // Kinetic energy signal histograms
        TH1D *hIncidentKE;
        TH1D *hInteractingKE; 
        TH1D *hInteractingKEEl; 
        TH1D *hInteractingKEElDep; 
        TH1D *hInteractingKEInel; 
        TH1D *hInteractingKEPionAbs;
        TH1D *hInteractingKEPionAbs0p;
        TH1D *hInteractingKEPionAbsNp;

        // Kinetic energy background histograms
        TH1D *hInteractingKEOutsideRedVolume;
        TH1D *hInteractingKEPionInelastic;
        TH1D *hInteractingKEChargeExchange;
        TH1D *hInteractingKEDoubleChargeExchange;
        TH1D *hInteractingKECaptureAtRest;
        TH1D *hInteractingKEDecay;
        TH1D *hInteractingKEOther;

        TH1D*   h_DE   ;
        TH1D*   h_DX   ;
        TH1D*   h_DEDX ;

        TH1D*   h_DEUniform   ;
        TH1D*   h_DXUniform   ;
        TH1D*   h_DEDXUniform ;

        TH1D*   h_DeltaE ;
        TH1D*   h_SimIDEDist ;
        TH1D*   h_UniformDistances ;

        TH1D *hKEAtTPCFF; 
        TH1D *hInitialKE; 
        TH1D *hInitialPz; 

        TH2D *hXZ;
        TH2D *hYZ;
        TH2D *hXZPre;
        TH2D *hYZPre;

        TH2D *hdEVsdX; 
        TH2D *hdEVsKE; 

        bool    debug = false;

        TTree* fTree;
        int    run;
        int    subrun;
        int    eventN;

        bool keepInteraction;

        double trueVtxX ;
        double trueVtxY ;
        double trueVtxZ ;
        double trueEndX ;
        double trueEndY ;
        double trueEndZ ;

        double finalKE ;
        std::vector<std::string> G4Process; 

        // Keep track of event counts
        int evtsPreTPC  = 0;
        int evtsInTheMiddle = 0;
        int evtsPostTPC = 0;
        int throughgoing = 0;
        int interactingInTPC = 0;

        // Detector dimensions
        double minX =  0.0;
        double maxX = 47.0;
        double minY =-20.0;
        double maxY = 20.0;
        double minZ =  3.0;
        double maxZ = 90.0;

        // Reduced volume for interactions
        const double RminX =  5.0;
        const double RmaxX = 42.0;
        const double RminY =-15.0; 
        const double RmaxY = 15.0;
        const double RminZ =  8.0;
        const double RmaxZ = 82.0;
};


lariat::TrueXSPionAbs::TrueXSPionAbs(fhicl::ParameterSet const & pset) : EDAnalyzer(pset) {
  this->reconfigure(pset);
}

lariat::TrueXSPionAbs::~TrueXSPionAbs() {
  // Clean up dynamic memory and other resources here.
}

void lariat::TrueXSPionAbs::reconfigure(fhicl::ParameterSet const & p) {
    verbose                    = p.get<bool>("Verbose", false);
    PROTON_ENERGY_LOWER_BOUND  = p.get<float>("ProtonEnergyLowerBound", 0.075);
    PROTON_ENERGY_UPPER_BOUND  = p.get<float>("ProtonEnergyUpperBound", 1.0);
    simulation_producer_label_ = p.get<std::string>("SimulationLabel", "largeant");
}

void lariat::TrueXSPionAbs::analyze(art::Event const & evt) {
    resetTree();

    // Get potentially useful services
    art::ServiceHandle<geo::Geometry> geom;

    // Get simulated particles
    auto particle_handle = evt.getValidHandle<std::vector<simb::MCParticle>>(simulation_producer_label_);
    std::vector<art::Ptr<simb::MCParticle>> particle_vector;
    art::fill_ptr_vector(particle_vector, particle_handle);
 
    // Get the backtracker to recover true quantities
    art::ServiceHandle<cheat::BackTrackerService> bt;
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();

    // Set a bogus branch quantities, to be overwritten  
    run     = evt.run();
    subrun  = evt.subRun();
    eventN  = evt.event();

    /////////////////////////////////////////////
    // Tag interaction as signal or background //
    /////////////////////////////////////////////

    // Initialize particle map (typedef at top) and fill it
    ParticleMap particle_map;
    for (auto const& particle : particle_vector) {
        particle_map[particle->TrackId()] = particle;
    }

    // Identify true-level primary particle and get its information
    std::vector<int> primaryDaughtersIDs;
    for (size_t p = 0; p < plist.size(); ++p) {
        auto part = plist.Particle(p);
        if (part->Process() == "primary") {
            truthPrimaryPDG = part->PdgCode();
            for (int i = 0; i < part->NumberDaughters(); ++i) primaryDaughtersIDs.push_back(part->Daughter(i));
            truthPrimaryVertexX    = part->EndX();
            truthPrimaryVertexY    = part->EndY();
            truthPrimaryVertexZ    = part->EndZ();
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

    if (isPionAbsorptionSignal) {
        if (numVisibleProtons == 0) backgroundType = 0;
        if (numVisibleProtons > 0)  backgroundType = 1;
    }

    /////////////////////////////////////
    // Compute stuff for cross-section //
    /////////////////////////////////////

    // Looping over all the Geant4 particles from the BackTracker
    for (size_t p = 0; p < plist.size(); ++p) {
        // Only continue if it is a primary pion
        auto mcPart = plist.Particle(p);
        if (!((mcPart->Process().find("primary") != std::string::npos) && (mcPart->PdgCode() == -211))) continue;

        // Get the True Trajectory point
        simb::MCTrajectory truetraj = mcPart->Trajectory();
        // Make Sure we get the beamline primary
        if (((truetraj.begin())->first).Z() >  -50. ) continue;      

        // Get simIDE associated to the primary
        geo::View_t view = geom->View(0); 
        auto simIDE_Prim = bt->TrackIdToSimIDEs_Ps(mcPart->TrackId(), view);

        // Order them in order of increasing Z
        std::map<double, sim::IDE> orderedSimIDE;
        for (auto ide : simIDE_Prim) orderedSimIDE[ide->z] = *ide;

        //Let's store the interaction type, so we can use it later in the Cross Section
        std::string interactionLabel = "";

        // Get the mass (in GeV)
        double mass = mcPart->Mass();
      
        // Store the kinetic energy and momentum on z at WC4. Just for cross check 
        auto inTPCPoint  = truetraj.begin(); 
        auto Momentum0   = inTPCPoint->second;
        
        double KE0 = 1000*(TMath::Sqrt(Momentum0.X()*Momentum0.X() + Momentum0.Y()*Momentum0.Y() + Momentum0.Z()*Momentum0.Z() + mass*mass ) - mass); //I want this in MeV
        hInitialKE->Fill(KE0);
        hInitialPz->Fill(1000*Momentum0.Z());

        // Identify the first trajectory point inside the TPC
        // Loop From First TrajPoint --> First Point in TPC 
        // Stop when you get into the TPC
        for (auto t = truetraj.begin(); t != std::prev(truetraj.end()); t++) {
	        auto pos = t->first;
	        if (pos.Z() < minZ) continue;
	        else if (pos.X() < minX || pos.X() > maxX ) continue;
	        else if (pos.Y() < minY || pos.Y() > maxY ) continue;
	        else {
	            inTPCPoint = t;
	            break;
	        }
	    } // End search for first point in TPC
      
        // if the first point is not in the TPC, we're not interested in this event. 
        // Fill some histos for cross check
        evtsPreTPC++;
        hXZPre->Fill((inTPCPoint->first).Z(), (inTPCPoint->first).X());
        hYZPre->Fill((inTPCPoint->first).Z(), (inTPCPoint->first).Y());

        if (inTPCPoint == truetraj.begin()) continue;
        evtsPostTPC++;
        hXZ->Fill((inTPCPoint->first).Z(), (inTPCPoint->first).X());
        hYZ->Fill((inTPCPoint->first).Z(), (inTPCPoint->first).Y());
      
        // Identify the last interesting trajectory point in TPC
        auto finTPCPoint = std::prev(truetraj.end()); 
        // The last point is a bit more complicated:
        //  if there's no interaction, then it is simply the last point in the TPC
        //  if there's one or more interaction points, it's the first interaction point deemed interesting (no coulomb)
        // Take the interaction Map... check if there's something there
        auto thisTracjectoryProcessMap =  truetraj.TrajectoryProcesses();
        if (thisTracjectoryProcessMap.size()) {
	        for(auto const& couple: thisTracjectoryProcessMap) { 
	            // I'm not interested in the CoulombScattering, discart this case
	            if (verbose) std::cout<<(truetraj.KeyToProcess(couple.second))<<" Position "<< ((truetraj.at(couple.first)).first).Z() <<"\n";
	            if ((truetraj.KeyToProcess(couple.second)).find("CoulombScat")!= std::string::npos) continue;
	      
	            // Let's check if the interaction is in the the TPC
	            auto     interactionPos4D =  (truetraj.at(couple.first)).first ;	
	            if      (interactionPos4D.Z() <  minZ || interactionPos4D.Z() > maxZ ) continue;
	            else if (interactionPos4D.X() <  minX || interactionPos4D.X() > maxX ) continue;
	            else if (interactionPos4D.Y() <  minY || interactionPos4D.Y() > maxY ) continue;

	            // If we made it here, then this is the first interesting interaction in the TPC
	            // Our job is done!!! Great! Store the interaction label and the iterator for the final point
	            interactionLabel = truetraj.KeyToProcess(couple.second);
	            finTPCPoint = truetraj.begin() + couple.first; 
	            keepInteraction = true;
	            interactingInTPC++;
	            break;
	        } // Loop on interaction points
	    } // If there are G4 interactions
      

        // If I didn't find anything interesting in the interaction map, let's loop back!
        if (!keepInteraction) {
	        // Loop on the daughters
	        for(size_t d = 0; d < plist.size(); ++d) {
                auto mcDaught = plist.Particle(d);
                // We keep only the daughters of the primary not coming from elastic or inelastic scattering
                if (mcDaught->Mother() != 1) continue;
                if ((mcDaught->Process()).find("astic") != std::string::npos) continue;
                if ((mcDaught->Process()).find("CoulombScat")!= std::string::npos) continue;

                // Is the daughter born inside the TPC? If yes, store the process which created it 
                simb::MCTrajectory trueDaugthTraj = mcDaught->Trajectory();	      
                if (trueDaugthTraj.begin()->first.Z() < minZ || trueDaugthTraj.begin()->first.Z() > maxZ) continue;
                else if (trueDaugthTraj.begin()->first.X() <   minX || trueDaugthTraj.begin()->first.X() > maxX ) continue;
                else if (trueDaugthTraj.begin()->first.Y() <   minY || trueDaugthTraj.begin()->first.Y() > maxY ) continue;
                else {
                    interactionLabel = mcDaught->Process();
                    break;
                }
            }

	        for (auto t = std::prev(truetraj.end()); t != truetraj.begin(); t--) {
	            auto pos = t->first;
	            if (pos.Z() > maxZ) continue;
	            else if (pos.X() <   minX || pos.X() > maxX ) continue;
	            else if (pos.Y() <   minY || pos.Y() > maxY ) continue;
	            else {
		            finTPCPoint = t;
		            break;
	            }
	        }
	    }      
 
        if (finTPCPoint == inTPCPoint) continue;
        auto posFin = finTPCPoint->first;
        auto posIni = inTPCPoint->first;
        // Let's record what the initial and final points are.
        trueVtxX = posIni.X();
        trueVtxY = posIni.Y();
        trueVtxZ = posIni.Z();
        trueEndX = posFin.X();
        trueEndY = posFin.Y();
        trueEndZ = posFin.Z();
      
        auto totLength = distance(posFin.X(), posFin.Y(), posFin.Z(),posIni.X(), posIni.Y(), posIni.Z() );
        if (totLength < 0.47) continue;

        // Ok, now we have the first and last point in the TPC and they are reasonably distant
        // Let's use them!
        // But first, some stupid checks
        if (verbose) {
	        std::cout<<"True Vtx X: "<<posIni.X()<<" Y: "<<posIni.Y()<<" Z: "<<posIni.Z()<<"\n";
	        for (auto const& t : truetraj) {
                auto pos = t.first;
                std::cout<<"------------------> "<<pos.X()<<" "<<pos.Y() <<" "<<pos.Z()<<" \n";
	        }
	        std::cout<<"True End X: "<<posFin.X()<<" Y: "<<posFin.Y()<<" Z: "<<posFin.Z()<<"-----> "<< interactionLabel<<"\n\n";
	        std::cout<<"\n\n\n";
	    }

        // We want to chop up the points between the fist and list uniformely
        // and ordered by Z
        // Order them in order of increasing Z
        std::map<double, TVector3> orderedUniformTrjPts;
        // We want the first and last uniform point to coincide with the 
        // the first and last points we just found 
        auto positionVector0 = (inTPCPoint ->first).Vect(); 
        auto positionVector1 = (finTPCPoint->first).Vect(); 
        orderedUniformTrjPts[positionVector0.Z()] = positionVector0;
        orderedUniformTrjPts[positionVector1.Z()] = positionVector1;

        const double trackPitch = 0.47;
        // I do have space for at least one extra point, so let's put it there!
        // Calculate how many extra points I need to put between the new first point and the second TrajPoint
        int nPts = (int) (totLength/trackPitch);
        for (int iPt = 1; iPt <= nPts; iPt++) {
	        auto newPoint = positionVector0 + iPt*(trackPitch/totLength) * (positionVector1 - positionVector0);
	        orderedUniformTrjPts[newPoint.Z()] = newPoint;
	    }

        // If the distance between the last point and the second to last is less then 0.235
        // eliminate the second to last point
        auto lastPt         = (orderedUniformTrjPts.rbegin())->second;
        auto secondtoLastPt = (std::next(orderedUniformTrjPts.rbegin()))->second;
        double lastDist = distance(lastPt.X(),lastPt.Y(),lastPt.Z(),secondtoLastPt.X(),secondtoLastPt.Y(),secondtoLastPt.Z());

        if (lastDist < 0.235) {
	        orderedUniformTrjPts.erase((std::next(orderedUniformTrjPts.rbegin()))->first );
	    }

        // Some other stupid check
        if (verbose) {
            lastPt         = (orderedUniformTrjPts.rbegin())->second;
            secondtoLastPt = (std::next(orderedUniformTrjPts.rbegin()))->second;
            lastDist = distance(lastPt.X(),lastPt.Y(),lastPt.Z(),secondtoLastPt.X(),secondtoLastPt.Y(),secondtoLastPt.Z());
            
            std::cout<<"True End X: "<<posFin.X()<<" Y: "<<posFin.Y()<<" Z: "<<posFin.Z()<<"-----> "<< interactionLabel<<"\n";
            std::cout<<"True End X: "<<lastPt.X()<<" Y: "<<lastPt.Y()<<" Z: "<<lastPt.Z()<<"-----> "<< interactionLabel<<"\n";
            std::cout<<"True End X: "<<secondtoLastPt.X()<<" Y: "<<secondtoLastPt.Y()<<" Z: "<<secondtoLastPt.Z()<<"-----> "<< interactionLabel<<"\n";
            std::cout<<"lastDist "<<lastDist<<"\n\n\n";
	    }      


        // Calculate the initial kinetic energy
        auto   initialMom = inTPCPoint->second;
        double initialKE  = 1000 * (TMath::Sqrt(initialMom.X()*initialMom.X() + initialMom.Y()*initialMom.Y() + initialMom.Z()*initialMom.Z() + mass * mass) - mass); 
        hKEAtTPCFF->Fill(initialKE);
        double kineticEnergy = initialKE;

        auto old_it = orderedUniformTrjPts.begin();
        for (auto it = std::next(orderedUniformTrjPts.begin()); it != orderedUniformTrjPts.end(); it++, old_it++) {
	        if (verbose)  std::cout << it->first<<" : " << (it->second).Z() << std::endl ;

	        auto oldPos        = old_it->second;
	        auto currentPos    =     it->second;
	
	        double uniformDist =  (currentPos - oldPos).Mag();
	        h_UniformDistances->Fill(uniformDist);
	  
	        // Calculate the energy deposited in this slice	  
	        auto old_iter = orderedSimIDE.begin();
	        double currentDepEnergy = 0.;
	        for ( auto iter= orderedSimIDE.begin(); iter!= orderedSimIDE.end(); iter++, old_iter++) {
	            auto currentIde = iter->second;
	            if ( currentIde.z < oldPos.Z()) continue;
	            if ( currentIde.z > currentPos.Z()) continue;
	            currentDepEnergy += currentIde.energy;
	        } // Determing which simIDE is within the current slice

            // avoid overfilling super tiny energy depositions
            if (currentDepEnergy / uniformDist < 0.1 ) continue;
            // Calculate the current kinetic energy
            kineticEnergy -= currentDepEnergy;

            hdEVsdX->Fill(currentDepEnergy, (currentPos.Z() - oldPos.Z()));
            hdEVsKE->Fill(currentDepEnergy, kineticEnergy);
            hIncidentKE->Fill(kineticEnergy);
            h_DEUniform->Fill(currentDepEnergy);
            h_DXUniform->Fill(uniformDist);
            h_DEDXUniform->Fill(currentDepEnergy / uniformDist);
        } // Loop on OrderedPoints

        // Fill histogram considering ALL interactions
        if (interactionLabel.size()) {
            hInteractingKE->Fill(kineticEnergy);
        }

        if (interactionLabel.find("Inelastic") != std::string::npos) {
	        // std::cout<<"Interaction Label: "<<interactionLabel<<"\n";
	        hInteractingKEInel->Fill(kineticEnergy);
	    }
      
        // Fill the Elastic and Total Interacting with the last point
        if (interactionLabel.find("Elastic") != std::string::npos) {
            h_DeltaE->Fill(kineticEnergy - 1000 * ((finTPCPoint->second).E() - mass));
            hInteractingKEElDep->Fill(kineticEnergy);
            auto MomentumF = finTPCPoint->second;
            double KEF = 1000*(TMath::Sqrt(MomentumF.X()*MomentumF.X() + MomentumF.Y()*MomentumF.Y() + MomentumF.Z()*MomentumF.Z() + mass*mass ) - mass); // I want this in MeV
            hInteractingKEEl->Fill(KEF);
	    }


        // Fill pion absorption histograms
        if (isPionAbsorptionSignal) {
            hInteractingKEPionAbs->Fill(kineticEnergy);
            if (numVisibleProtons == 0) {
                hInteractingKEPionAbs0p->Fill(kineticEnergy);
            } else if (numVisibleProtons > 0) {
                hInteractingKEPionAbsNp->Fill(kineticEnergy);
            }
        }

        // Fill background histograms
        if (backgroundType == 5) {
            hInteractingKEOutsideRedVolume->Fill(kineticEnergy);
        } else if (backgroundType == 6) {
            hInteractingKEPionInelastic->Fill(kineticEnergy);
        } else if (backgroundType == 7) {
            hInteractingKEChargeExchange->Fill(kineticEnergy);
        } else if (backgroundType == 8) {
            hInteractingKEDoubleChargeExchange->Fill(kineticEnergy);
        } else if (backgroundType == 9) {
            hInteractingKECaptureAtRest->Fill(kineticEnergy);
        } else if (backgroundType == 10) {
            hInteractingKEDecay->Fill(kineticEnergy);
        } else if (backgroundType == 11) {
            hInteractingKEOther->Fill(kineticEnergy);
        }

        finalKE = kineticEnergy;
        if (!interactionLabel.size()) {
	        throughgoing++;
	        G4Process.push_back("throughgoing");
	    } else {
	        G4Process.push_back(interactionLabel);
	    }
      
    } // MC Particle Loop
  
    fTree->Fill();
}

void lariat::TrueXSPionAbs::endJob() {
    std::cout<<"-------------------------------------------"<<std::endl;
    std::cout<<"True Events pre-TPC .............. "<<evtsPreTPC<<std::endl;
    std::cout<<"True Events pre-TPC .............. "<<evtsInTheMiddle<<std::endl;
    std::cout<<"True Events post-TPC ............. "<<evtsPostTPC<<std::endl;
    std::cout<<"True Throughgoing    ............. "<<throughgoing<<std::endl;
    std::cout<<"True interactingInTPC ............ "<<interactingInTPC<<std::endl;
    std::cout<<"-------------------------------------------"<<std::endl;

    float rho            = 1396; // kg / m^3
    float molar_mass     = 39.95; // g / mol
    float g_per_kg       = 1000; 
    float avogadro       = 6.022e+23; // number/mol
    float number_density = rho*g_per_kg / molar_mass * avogadro;
    float slab_width     = 0.0047; // in m

    // Calculate the cross section, looping over exiting bins to extract cros-section
    for( int iBin = 1; iBin <= hInteractingKE->GetNbinsX(); ++iBin ) {
        // If an incident bin is equal to zero then skip that bin
        // Temporary fix to ensure that no Infinities are propagated to pad
        if(hIncidentKE->GetBinContent(iBin) == 0) continue; 
   
        // Cross-section = (Exit Bins / Incident Bins) * (1/Density) * (1/slab width) 
        float crossSection     = ((hInteractingKE->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float elCrossSection   = ((hInteractingKEEl->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float inelCrossSection = ((hInteractingKEInel->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
	
        // Cross-section for pion absorption
        float crossSectionPionAbs   = ((hInteractingKEPionAbs->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionPionAbs0p = ((hInteractingKEPionAbs0p->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionPionAbsNp = ((hInteractingKEPionAbsNp->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);

        // Cross-section for background interactions
        float crossSectionOutsideRedVolume     = ((hInteractingKEOutsideRedVolume->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionPionInelastic        = ((hInteractingKEPionInelastic->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionChargeExchange       = ((hInteractingKEChargeExchange->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionDoubleChargeExchange = ((hInteractingKEDoubleChargeExchange->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionCaptureAtRest        = ((hInteractingKECaptureAtRest->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionDecay                = ((hInteractingKEDecay->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);
        float crossSectionOther                = ((hInteractingKEOther->GetBinContent(iBin) / hIncidentKE->GetBinContent(iBin)) * (1 / number_density) * (1 / slab_width)) * (1 / 1e-28);

        // Putting the value on the histogram
        hCrossSection    ->SetBinContent(iBin, crossSection);
        hCrossSectionEl  ->SetBinContent(iBin, elCrossSection);
        hCrossSectionInel->SetBinContent(iBin, inelCrossSection);
        
        hCrossSectionPionAbs  ->SetBinContent(iBin, crossSectionPionAbs);
        hCrossSectionPionAbs0p->SetBinContent(iBin, crossSectionPionAbs0p);
        hCrossSectionPionAbsNp->SetBinContent(iBin, crossSectionPionAbsNp);

        hCrossSectionOutsideRedVolume    ->SetBinContent(iBin, crossSectionOutsideRedVolume);
        hCrossSectionPionInelastic       ->SetBinContent(iBin, crossSectionPionInelastic);
        hCrossSectionChargeExchange      ->SetBinContent(iBin, crossSectionChargeExchange);
        hCrossSectionDoubleChargeExchange->SetBinContent(iBin, crossSectionDoubleChargeExchange);
        hCrossSectionCaptureAtRest       ->SetBinContent(iBin, crossSectionCaptureAtRest);
        hCrossSectionDecay               ->SetBinContent(iBin, crossSectionDecay);
        hCrossSectionOther               ->SetBinContent(iBin, crossSectionOther);

        // Calculating the error on the numerator of the ratio
        float denomError = std::pow(hIncidentKE->GetBinContent(iBin), 0.5);
        float denom      = hIncidentKE->GetBinContent(iBin);
        if (denom == 0) continue; 
        float term2 = denomError / denom;

        float numError = std::pow(hInteractingKE->GetBinContent(iBin), 0.5);
        float num      = hInteractingKE->GetBinContent(iBin);

        float numErrorEl = std::pow(hInteractingKEEl->GetBinContent(iBin), 0.5);
        float numEl      = hInteractingKEEl->GetBinContent(iBin);

        float numErrorInel = std::pow(hInteractingKEInel->GetBinContent(iBin), 0.5);
        float numInel      = hInteractingKEInel->GetBinContent(iBin);

        float numErrorPionAbs = std::pow(hInteractingKEPionAbs->GetBinContent(iBin), 0.5);
        float numPionAbs      = hInteractingKEPionAbs->GetBinContent(iBin);

        float numErrorPionAbs0p = std::pow(hInteractingKEPionAbs0p->GetBinContent(iBin), 0.5);
        float numPionAbs0p      = hInteractingKEPionAbs0p->GetBinContent(iBin);

        float numErrorPionAbsNp = std::pow(hInteractingKEPionAbsNp->GetBinContent(iBin), 0.5);
        float numPionAbsNp      = hInteractingKEPionAbsNp->GetBinContent(iBin);

        float numErrorOutsideRedVolume = std::pow(hInteractingKEOutsideRedVolume->GetBinContent(iBin), 0.5);
        float numOutsideRedVolume      = hInteractingKEOutsideRedVolume->GetBinContent(iBin);

        float numErrorPionInelastic = std::pow(hInteractingKEPionInelastic->GetBinContent(iBin), 0.5);
        float numPionInelastic      = hInteractingKEPionInelastic->GetBinContent(iBin); 

        float numErrorChargeExchange = std::pow(hInteractingKEChargeExchange->GetBinContent(iBin), 0.5);
        float numChargeExchange      = hInteractingKEChargeExchange->GetBinContent(iBin);

        float numErrorDoubleChargeExchange = std::pow(hInteractingKEDoubleChargeExchange->GetBinContent(iBin), 0.5);
        float numDoubleChargeExchange      = hInteractingKEDoubleChargeExchange->GetBinContent(iBin);

        float numErrorCaptureAtRest = std::pow(hInteractingKECaptureAtRest->GetBinContent(iBin), 0.5);
        float numCaptureAtRest      = hInteractingKECaptureAtRest->GetBinContent(iBin);

        float numErrorDecay = std::pow(hInteractingKEDecay->GetBinContent(iBin), 0.5);
        float numDecay      = hInteractingKEDecay->GetBinContent(iBin);

        float numErrorOther = std::pow(hInteractingKEOther->GetBinContent(iBin), 0.5);
        float numOther      = hInteractingKEOther->GetBinContent(iBin);

        // Putting in a protection against dividing by zero 
        if (num != 0) {
            float term1      = numError / num;
            float totalError = (crossSection) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSection->SetBinError(iBin, totalError);
        }
      
        if (numEl != 0) {
            float term1      = numErrorEl / numEl;
            float totalError = (elCrossSection) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionEl->SetBinError(iBin,totalError);
        }
      
        if (numInel != 0){
            float term1      = numErrorInel / numInel;
            float totalError = (inelCrossSection) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width)  *(1e26);
            hCrossSectionInel->SetBinError(iBin,totalError);
        }

        if (numPionAbs != 0) {
            float term1      = numErrorPionAbs / numPionAbs;
            float totalError = (crossSectionPionAbs) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionPionAbs->SetBinError(iBin, totalError);
        }

        if (numPionAbs0p != 0) {
            float term1      = numErrorPionAbs0p / numPionAbs0p;
            float totalError = (crossSectionPionAbs0p) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionPionAbs0p->SetBinError(iBin, totalError);
        }

        if (numPionAbsNp != 0) {
            float term1      = numErrorPionAbsNp / numPionAbsNp;
            float totalError = (crossSectionPionAbsNp) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionPionAbsNp->SetBinError(iBin, totalError);
        }

        if (numOutsideRedVolume != 0) {
            float term1      = numErrorOutsideRedVolume / numOutsideRedVolume;
            float totalError = (crossSectionOutsideRedVolume) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionOutsideRedVolume->SetBinError(iBin, totalError);
        }

        if (numPionInelastic != 0) {
            float term1      = numErrorPionInelastic / numPionInelastic;
            float totalError = (crossSectionPionInelastic) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionPionInelastic->SetBinError(iBin, totalError);
        }

        if (numChargeExchange != 0) {
            float term1      = numErrorChargeExchange / numChargeExchange;
            float totalError = (crossSectionChargeExchange) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionChargeExchange->SetBinError(iBin, totalError);
        }

        if (numDoubleChargeExchange != 0) {
            float term1      = numErrorDoubleChargeExchange / numDoubleChargeExchange;
            float totalError = (crossSectionDoubleChargeExchange) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionDoubleChargeExchange->SetBinError(iBin, totalError);
        }

        if (numCaptureAtRest != 0) {
            float term1      = numErrorCaptureAtRest / numCaptureAtRest;
            float totalError = (crossSectionCaptureAtRest) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionCaptureAtRest->SetBinError(iBin, totalError);
        }

        if (numDecay != 0) {
            float term1      = numErrorDecay / numDecay;
            float totalError = (crossSectionDecay) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionDecay->SetBinError(iBin, totalError);
        }

        if (numOther != 0) {
            float term1      = numErrorOther / numOther;
            float totalError = (crossSectionOther) * (std::pow(((term1 * term1) + (term2 * term2)), 0.5)) * (1 / number_density) * (1 / slab_width) * (1e26);
            hCrossSectionOther->SetBinError(iBin, totalError);
        }
    } // End bin loop
}

double lariat::TrueXSPionAbs::distance(double x1, double y1, double z1, double x2, double y2, double z2) {
    double d = TMath::Sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
    return d;
}

void lariat::TrueXSPionAbs::beginJob() {
    art::ServiceHandle<art::TFileService> tfs;
    h_DE   = tfs->make<TH1D>("h_DE","h_DE; Energy Deposited [MeV]",200, 0,100);   
    h_DX   = tfs->make<TH1D>("h_DX","h_DX; Distance between points  [cm]",400, 0,20);   
    h_DEDX = tfs->make<TH1D>("h_DEDEX","h_DEDX; dE/dX [MeV/cm]",500, 0,50);   

    h_DEUniform   = tfs->make<TH1D>("h_DEUniform","h_DE; Energy Deposited [MeV]",200, 0,100);   
    h_DXUniform   = tfs->make<TH1D>("h_DXUniform","h_DX; Distance between points  [cm]",400, 0,20);   
    h_DEDXUniform = tfs->make<TH1D>("h_DEDEXUniform","h_DEDX; dE/dX [MeV/cm]",500, 0,50);   

    h_DeltaE     = tfs->make<TH1D>("h_DeltaE","h_DeltaE; dEDep - TrjDE [MeV/cm]",500, -1000,1000);   
    h_SimIDEDist = tfs->make<TH1D>("h_SimIDEDist","h_SimIDEDist; h_SimIDEDist [cm]",1000, 0,10);   
  
    h_UniformDistances = tfs->make<TH1D>("h_UniformDistances","h_UniformDistances; Distance between uniform points  [cm]",500, 0,5);   

    hInitialPz = tfs->make<TH1D>("hInitialPz"    , "Initial Pz [MeV/c]"    , 42, -100, 2000);
    hInitialKE = tfs->make<TH1D>("hInitialKE"    , "Initial Kinetic Energy [MeV]"    , 42, -100, 2000);
    hKEAtTPCFF = tfs->make<TH1D>("hKEAtTPCFF"    , "Kinetic Energy @ TPC FF [MeV]"   , 42, -100, 2000);

    hIncidentKE         = tfs->make<TH1D>("hIncidentKE"   , "Incident Kinetic Energy [MeV]"   , 42, -100, 2000); 
    hInteractingKE      = tfs->make<TH1D>("hInteractingKE", "Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEEl    = tfs->make<TH1D>("hInteractingKEEl", "Elastic Interacting Kinetic Energy [MeV]", 42, -100, 2000); 
    hInteractingKEElDep = tfs->make<TH1D>("hInteractingKEElDep", "Dep Elastic Interacting Kinetic Energy [MeV]", 42, -100, 2000); 
    hInteractingKEInel  = tfs->make<TH1D>("hInteractingKEInel", "Inelastic Interacting Kinetic Energy [MeV]", 42, -100, 2000);

    hInteractingKEPionAbs   = tfs->make<TH1D>("hInteractingKEPionAbs", "Pion Absorption Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEPionAbs0p = tfs->make<TH1D>("hInteractingKEPionAbs0p", "Pion Absorption 0p Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEPionAbsNp = tfs->make<TH1D>("hInteractingKEPionAbsNp", "Pion Absorption Np Interacting Kinetic Energy [MeV]", 42, -100, 2000);

    hInteractingKEOutsideRedVolume = tfs->make<TH1D>("hInteractingKEOutsideRedVolume", "Outside Reduced Volume Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEPionInelastic     = tfs->make<TH1D>("hInteractingKEPionInelastic", "Pion Inelastic Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEChargeExchange      = tfs->make<TH1D>("hInteractingKEChargeExchange", "Charge Exchange Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEDoubleChargeExchange = tfs->make<TH1D>("hInteractingKEDoubleChargeExchange", "Double Charge Exchange Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKECaptureAtRest       = tfs->make<TH1D>("hInteractingKECaptureAtRest", "Capture At Rest Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEDecay               = tfs->make<TH1D>("hInteractingKEDecay", "Decay Interacting Kinetic Energy [MeV]", 42, -100, 2000);
    hInteractingKEOther               = tfs->make<TH1D>("hInteractingKEOther", "Other Interacting Kinetic Energy [MeV]", 42, -100, 2000);

    hCrossSection     = tfs->make<TH1D>("hCrossSection"     , "Cross-Section [barn]"             , 42, -100, 2000);
    hCrossSectionEl   = tfs->make<TH1D>("hCrossSectionEl"   , "Elastic Cross-Section [barn]"     , 42, -100, 2000);
    hCrossSectionInel = tfs->make<TH1D>("hCrossSectionInel" , "Inelastic Cross-Section [barn]"   , 42, -100, 2000);

    hCrossSectionPionAbs   = tfs->make<TH1D>("hCrossSectionPionAbs", "Pion Absorption Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionPionAbs0p = tfs->make<TH1D>("hCrossSectionPionAbs0p", "Pion Absorption 0p Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionPionAbsNp = tfs->make<TH1D>("hCrossSectionPionAbsNp", "Pion Absorption Np Cross-Section [barn]", 42, -100, 2000);

    hCrossSectionOutsideRedVolume    = tfs->make<TH1D>("hCrossSectionOutsideRedVolume", "Outside Reduced Volume Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionPionInelastic       = tfs->make<TH1D>("hCrossSectionPionInelastic", "Pion Inelastic Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionChargeExchange      = tfs->make<TH1D>("hCrossSectionChargeExchange", "Charge Exchange Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionDoubleChargeExchange = tfs->make<TH1D>("hCrossSectionDoubleChargeExchange", "Double Charge Exchange Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionCaptureAtRest       = tfs->make<TH1D>("hCrossSectionCaptureAtRest", "Capture At Rest Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionDecay               = tfs->make<TH1D>("hCrossSectionDecay", "Decay Cross-Section [barn]", 42, -100, 2000);
    hCrossSectionOther               = tfs->make<TH1D>("hCrossSectionOther", "Other Cross-Section [barn]", 42, -100, 2000);

    hXZ    = tfs->make<TH2D>("hXZ"     , "hXZ"    , 110, -100, 10, 200, -100, 100);  
    hYZ    = tfs->make<TH2D>("hYZ"     , "hYZ"    , 110, -100, 10, 200, -100, 100); 
    hXZPre = tfs->make<TH2D>("hXZPre"  , "hXZPre" , 110, -100, 10, 200, -100, 100); 
    hYZPre = tfs->make<TH2D>("hYZPre"  , "hYZPre" , 110, -100, 10, 200, -100, 100); 

    hdEVsdX = tfs->make<TH2D>("hdEVsdX"  , "hdEVsdX" , 504, -1, 50, 1100, -10, 100); 
    hdEVsKE = tfs->make<TH2D>("hdEVsKE"  , "hdEVsKE" , 504, -1, 50, 220,  -10, 1000); 

    fTree = tfs->make<TTree>("effTree","analysis tree");
    fTree->Branch("run"      ,&run      ,"run/I");
    fTree->Branch("subrun"   ,&subrun   ,"subrun/I");
    fTree->Branch("eventN"   ,&eventN   ,"eventN/I");

    fTree->Branch("trueVtxX" ,&trueVtxX ,"trueVtxX/D");
    fTree->Branch("trueVtxY" ,&trueVtxY ,"trueVtxY/D");
    fTree->Branch("trueVtxZ" ,&trueVtxZ ,"trueVtxZ/D");
    fTree->Branch("trueEndX" ,&trueEndX ,"trueEndX/D");
    fTree->Branch("trueEndY" ,&trueEndY ,"trueEndY/D");
    fTree->Branch("trueEndZ" ,&trueEndZ ,"trueEndZ/D");
    fTree->Branch("finalKE"  ,&finalKE  ,"finalKE/D" );
    fTree->Branch("G4Process",&G4Process);
}

void lariat::TrueXSPionAbs::resetTree() {
    keepInteraction = false;

    run      = -999; 
    subrun   = -999; 
    eventN   = -999;
  
    trueVtxX = -999.;
    trueVtxY = -999.;
    trueVtxZ = -999.;
    trueEndX = -999.;
    trueEndY = -999.;
    trueEndZ = -999.;
    finalKE  = -999.;
    G4Process.clear();

    isPionAbsorptionSignal = false;
    numVisibleProtons      = -1;
    backgroundType         = -1;

    truthPrimaryPDG        = -99999;
    truthPrimaryVertexX    = -99999;
    truthPrimaryVertexY    = -99999;
    truthPrimaryVertexZ    = -99999;
    truthPrimaryDaughtersPDG.clear();
    truthPrimaryDaughtersProcess.clear();
    truthPrimaryDaughtersKE.clear();
}

bool lariat::TrueXSPionAbs::isWithinReducedVolume(double x, double y, double z) {
    return (
        (x > RminX) && (x < RmaxX) && 
        (y > RminY) && (y < RmaxY) && 
        (z > RminZ) && (z < RmaxZ)
    );
}

void lariat::TrueXSPionAbs::fillSignalInformation(
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

void lariat::TrueXSPionAbs::fillBackgroundInformation(
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

DEFINE_ART_MODULE(lariat::TrueXSPionAbs)
