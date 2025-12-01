////////////////////////////////////////////////////////////////////////
// Class:       Geant4Reweight
// Module Type: producer
// File:        Geant4Reweight_module.cc
//
// Written by Emilio Pelaez following sbnd's example:
// https://github.com/SBNSoftware/sbncode/blob/develop/sbncode/SBNEventWeight/Calculators/Geant4/Geant4WeightCalc.cxx
////////////////////////////////////////////////////////////////////////


// ##########################
// ### Framework includes ###
// ##########################
#include "art/Framework/Core/EDAnalyzer.h"
#include "art/Framework/Core/ModuleMacros.h"
#include "canvas/Persistency/Common/FindManyP.h"
#include "canvas/Persistency/Common/FindOneP.h" 
#include "art/Framework/Principal/Event.h"
#include "art/Framework/Principal/Handle.h"
#include "art/Framework/Principal/Run.h"
#include "art/Framework/Principal/SubRun.h"
#include "art/Framework/Core/EDFilter.h"
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
// ### TMVA includes ###
// #####################
// #include "TMVA/Reader.h"

// #####################
// ### ROOT includes ###
// #####################
#include <TH1F.h>
#include <TF1.h>
#include <TH2F.h>
#include <TGraph.h>
#include <TTree.h>
#include "TRandom3.h"
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

// Geant4 includes
#include "Geant4/G4LossTableManager.hh"
#include "Geant4/G4ParticleTable.hh"
#include "Geant4/G4ParticleDefinition.hh"
#include "Geant4/G4Material.hh"
#include "Geant4/G4MaterialCutsCouple.hh"

// Geant4Reweight includes
#include "geant4reweight/src/ReweightBase/G4ReweightManager.hh"
#include "geant4reweight/src/ReweightBase/G4ReweighterFactory.hh"
#include "geant4reweight/src/ReweightBase/G4Reweighter.hh"
#include "geant4reweight/src/ReweightBase/G4ReweightTraj.hh"
#include "geant4reweight/src/ReweightBase/G4ReweightStep.hh"
#include "geant4reweight/src/PropBase/G4ReweightParameterMaker.hh"

enum class ProcType : uint8_t {
    kDefault = 0,
    kElastic = 1,
    kInelastic = 2
};
double BetheBloch(double energy, double mass);
std::vector<std::pair<double, int>> ThinSliceBetheBloch(G4ReweightTraj* theTraj, double res, double mass, bool isElastic, const std::vector<ProcType>& proc_types);

class CalculateWeights : public art::EDAnalyzer {
    public:
        explicit CalculateWeights(fhicl::ParameterSet const& pset);
        CalculateWeights(CalculateWeights const &) = delete;
        CalculateWeights(CalculateWeights &&) = delete;
        CalculateWeights & operator = (CalculateWeights const &) = delete;
        CalculateWeights & operator = (CalculateWeights &&) = delete;

        // Required functions
        void analyze(art::Event const &evt) override;

        // Optional functions
        void beginJob() override;
        void endJob() override;
        void reconfigure(fhicl::ParameterSet const& pset);

        // Helper functions
        void resetTree();

    private:
        // Output tree
        TTree *WeightsTree;

        // Event metadata
        int run; 
        int subrun;
        int event;

        // fcl parameters
        bool bVerbose;
        int particlePDG;
        std::string simulation_producer_label_;
        int numSims;
        int numSigmas;
        std::string FracsFileName;
        std::vector<fhicl::ParameterSet> FitParSets;
        fhicl::ParameterSet fMaterial;
        std::string mode;

        // Random generator
        int       fRandSeed = 1989;
        TRandom3* fRand     = new TRandom3(fRandSeed);

        // Reweighter
        G4ReweighterFactory rwFactory;
        G4Reweighter* reweighter;
        G4ReweightParameterMaker* parMaker;
        std::vector<std::map<std::string, double>> UniverseVals;

        // Weights
        std::vector<double> weights;

        // Variables when reweighing
        double p_track_length; //!< Variables for by-particle output tree
        double p_init_momentum; //!< Variables for by-particle output tree
        double p_final_momentum; //!< Variables for by-particle output tree
        std::vector< double > p_energies_el; //!< Variables for by-particle output tree
        std::vector< int > p_sliceInts_el; //!< Variables for by-particle output tree
        std::vector< double > p_energies_inel; //!< Variables for by-particle output tree
        std::vector< int > p_sliceInts_inel; //!< Variables for by-particle output tree
        int p_nElasticScatters; //!< Variables for by-particle output tree
        std::vector<double> p_weight; //!< Variables for by-particle output tree
};

CalculateWeights::CalculateWeights(fhicl::ParameterSet const& pset) : art::EDAnalyzer{pset} {
    // Configure parameter set
    this->reconfigure(pset);
}

void CalculateWeights::beginJob() {
    // Get input file
    TFile FracsFile(FracsFileName.c_str(), "OPEN");

    // Configure reweighter
    parMaker = new G4ReweightParameterMaker(FitParSets, true, particlePDG);
    static G4ReweightManager* RWManager = new G4ReweightManager({fMaterial});
    reweighter = rwFactory.BuildReweighter(particlePDG, &FracsFile, parMaker->GetFSHists(), fMaterial, RWManager, parMaker->GetElasticHist());

    // Read input parameter sets and set up universes
    size_t n_parsets = FitParSets.size();
    std::vector<std::string> FitParNames;
    std::vector<double> FitParNominals;
    std::vector<double> FitParSigmas;
    std::map<std::string, double> theNominals;

    for (size_t i_parset = 0; i_parset < n_parsets; ++i_parset) {
        fhicl::ParameterSet theSet = FitParSets.at(i_parset);
        std::string theName        = theSet.get<std::string>("Name");
        double theNominal          = theSet.get<double>("Nominal",1.);
        double theSigma            = theSet.get<double>("Sigma",0.);

        FitParNames.push_back(theName);
        FitParNominals.push_back(theNominal);
        FitParSigmas.push_back(theSigma);

        theNominals[theName] = theNominal;
    }

    if (mode == "multisim") {
        for (int j = 0; j < numSims; j++) {
            std::map<std::string, double> tmp_vals;
            for (size_t i_parset = 0; i_parset < n_parsets; ++i_parset) {
                double r = fRand->Gaus(0.0, 1.0);
                tmp_vals[FitParNames.at(i_parset)] = FitParNominals.at(i_parset) + (FitParSigmas.at(i_parset) * r);
            }
            UniverseVals.push_back(tmp_vals);
        }
    } else if (mode == "multisigma") {
        for (size_t i_parset = 0; i_parset < n_parsets; ++i_parset) {
            for (int i_sigma = -1 * numSigmas; i_sigma < int(numSigmas + 1); i_sigma++) {
                std::map<std::string, double> tmp_vals(theNominals);
                tmp_vals[FitParNames.at(i_parset)] = FitParNominals.at(i_parset) + i_sigma * FitParSigmas.at(i_parset);
                UniverseVals.push_back(tmp_vals);
            }
        }
    } else {
        UniverseVals.push_back(theNominals);
    }

    numSims = UniverseVals.size();

    // Make vectors the right size
    weights.resize(numSims, 1.0);
    p_weight.resize(numSims, 1.0);

    // Make histograms and tree branches
    art::ServiceHandle<art::TFileService> tfs;
    WeightsTree = tfs->make<TTree>("WeightsTree", "WeightsTree");

    WeightsTree->Branch("run", &run, "run/I");
    WeightsTree->Branch("subrun", &subrun, "subrun/I");
    WeightsTree->Branch("event", &event, "event/I");
    WeightsTree->Branch("weights", "std::vector<double>", &weights);
    WeightsTree->Branch("p_weight", "std::vector<double>", &p_weight);
}

void CalculateWeights::endJob() {
    
}

void CalculateWeights::analyze(art::Event const &evt) {
    resetTree();

    run = evt.run(); subrun = evt.subRun(); event = evt.event();
    if (bVerbose) std::cout << "Run: " << run << ", subrun: " << subrun << ", event: " << event << std::endl;
    if (bVerbose) std::cout << std::endl;

    /////////////////
    // Get MC data //
    /////////////////

    // Load geometry
    art::ServiceHandle<geo::Geometry> geom;
    geo::View_t view = geom->View(0);
    geo::TPCID tpcid;

    // Get simulated particles
    auto particle_handle = evt.getValidHandle<std::vector<simb::MCParticle>>(simulation_producer_label_);
    std::vector<art::Ptr<simb::MCParticle>> particle_vector;
    art::fill_ptr_vector(particle_vector, particle_handle);

    // Get particle list
    art::ServiceHandle<cheat::ParticleInventoryService> pi_serv;
    const sim::ParticleList& plist = pi_serv->ParticleList();
    
    // Loop over MCParticles in the event
    for (size_t iParticle = 0; iParticle < plist.size(); ++iParticle) {
        const simb::MCParticle* p = plist.Particle(iParticle);

        // Get information about particle
        int p_PDG = p->PdgCode();
        int mcpID = p->TrackId();
        std::string EndProcess = p->EndProcess();
        double mass = p->Mass() * 1000; // GeV/c^2 to MeV/c^2

        // Only want to record weights for one type of particle (from .fcl file)
        if (p_PDG == particlePDG) {
            std::vector<double> trajpoint_X;
            std::vector<double> trajpoint_Y;
            std::vector<double> trajpoint_Z;
            std::vector<double> trajpoint_PX;
            std::vector<double> trajpoint_PY;
            std::vector<double> trajpoint_PZ;
            std::vector<int> elastic_indices;

            // Get list of processses from true trajector
            const std::vector<std::pair<size_t, unsigned char>>& processes = p->Trajectory().TrajectoryProcesses();
            std::map<size_t, std::string> process_map;

            for (auto it = processes.begin(); it != processes.end(); ++it) {
                process_map[it->first] = p->Trajectory().KeyToProcess(it->second);
            }

            // Loop over trajectory points
            for (size_t i = 0; i < p->NumberTrajectoryPoints(); ++i) {
                double X = p->Position(i).X();
                double Y = p->Position(i).Y();
                double Z = p->Position(i).Z();
                geo::Point_t testpoint1 {X, Y, Z};
                // const TGeoMaterial* testmaterial1 = geom->Material(testpoint1);

                try { tpcid = geom->PositionToTPCID(testpoint1); }
                catch (...) { continue; } // point not in a TPC (i.e., not in LAr active volume) 

                trajpoint_X.push_back(X);
                trajpoint_Y.push_back(Y);
                trajpoint_Z.push_back(Z);

                trajpoint_PX.push_back(p->Px(i));
                trajpoint_PY.push_back(p->Py(i));
                trajpoint_PZ.push_back(p->Pz(i));

                auto itProc = process_map.find(i);
                if (itProc != process_map.end() && itProc->second == "hadElastic") {
                    // Push back the index relative to the start of the reweightable steps
                    elastic_indices.push_back(trajpoint_X.size() - 1);
                    // if (fDebug) std::cout << "Elastic index: " << trajpoint_X.size() - 1 << std::endl;
                }
            }

            // Now find daughters of the MCP
            std::vector<int> daughter_PDGs;
            std::vector<int> daughter_IDs;
            for (int i_mcp = 0; i_mcp < p->NumberDaughters(); ++i_mcp) {
                int daughterID = p->Daughter(i_mcp);
                for (size_t i_p = 0; i_p < plist.size(); ++i_p) {
                    const simb::MCParticle* test_mcp = plist.Particle(i_p);
                    if (test_mcp->TrackId() == daughterID){
                        int pid = test_mcp->PdgCode();
                        daughter_PDGs.push_back(pid);
                        daughter_IDs.push_back(test_mcp->TrackId());
                        break;
                    }
                }
            }

            // We have all the information we have, so we can reweigh now
            
            // Make a G4ReweightTraj
            G4ReweightTraj theTraj(mcpID, p_PDG, 0, event, std::make_pair(0,0));

            // Create its set of G4ReweightSteps and add them to the Traj
            std::vector<G4ReweightStep*> allSteps;

            size_t nSteps = trajpoint_PX.size();

            if (nSteps < 2) continue;

            p_nElasticScatters = elastic_indices.size();
            std::unordered_set<size_t> elasticSet(elastic_indices.begin(), elastic_indices.end()); // for lookup

            // Store process types
            std::vector<ProcType> proc_types;
            proc_types.reserve(nSteps); proc_types.push_back(ProcType::kDefault);

            for (size_t iStep = 1; iStep < nSteps; ++iStep) {
                // Save process to fast enum
                ProcType procType = ProcType::kDefault;
                if (elasticSet.count(iStep)) procType = ProcType::kElastic;
                else if (iStep == trajpoint_PX.size() - 1 && EndProcess.find("Inelastic") != std::string::npos) procType = ProcType::kInelastic;
                proc_types.push_back(procType);

                // Still have to save string
                std::string proc = "default";
                if (iStep == trajpoint_PX.size() - 1) proc = EndProcess;
                else if (std::find(elastic_indices.begin(), elastic_indices.end(), iStep) != elastic_indices.end()) proc = "hadElastic";

                double deltaX = (trajpoint_X.at(iStep) - trajpoint_X.at(iStep-1));
                double deltaY = (trajpoint_Y.at(iStep) - trajpoint_Y.at(iStep-1));
                double deltaZ = (trajpoint_Z.at(iStep) - trajpoint_Z.at(iStep-1));

                double len = std::sqrt(deltaX * deltaX + deltaY * deltaY + deltaZ * deltaZ);

                double preStepP[3] = {
                    trajpoint_PX.at(iStep-1)*1.e3,
                    trajpoint_PY.at(iStep-1)*1.e3,
                    trajpoint_PZ.at(iStep-1)*1.e3
                };

                double postStepP[3] = {
                    trajpoint_PX.at(iStep)*1.e3,
                    trajpoint_PY.at(iStep)*1.e3,
                    trajpoint_PZ.at(iStep)*1.e3
                };

                if (iStep == 1) {
                    theTraj.SetEnergy(
                        std::sqrt(preStepP[0]*preStepP[0] + preStepP[1]*preStepP[1] + preStepP[2]*preStepP[2] + mass*mass)
                    );
                }

                // Create step
                G4ReweightStep* theStep = new G4ReweightStep(mcpID, p_PDG, 0, event, preStepP, postStepP, len, proc);
                theStep->SetDeltaX(deltaX); theStep->SetDeltaY(deltaY); theStep->SetDeltaZ(deltaZ);
                theTraj.AddStep(theStep);
            } // end loop over iStep

            for (size_t k = 0; k < daughter_PDGs.size(); ++k) {
                theTraj.AddChild(
                    new G4ReweightTraj(daughter_IDs[k], daughter_PDGs[k], mcpID, event, std::make_pair(0,0))
                );
            }

            p_track_length = theTraj.GetTotalLength();

            p_init_momentum = std::sqrt(theTraj.GetEnergy() * theTraj.GetEnergy() - mass*mass);

            double pfx = theTraj.GetStep(theTraj.GetNSteps() - 1 )->GetPreStepPx();
            double pfy = theTraj.GetStep(theTraj.GetNSteps() - 1 )->GetPreStepPy();
            double pfz = theTraj.GetStep(theTraj.GetNSteps() - 1 )->GetPreStepPz();
            p_final_momentum = std::sqrt(pfx * pfx + pfy * pfy + pfz * pfz);

            std::vector<std::pair<double, int>> thin_slice_inelastic = ThinSliceBetheBloch(&theTraj, .5, mass, false, proc_types);
            std::vector<std::pair<double, int>> thin_slice_elastic   = ThinSliceBetheBloch(&theTraj, .5, mass, true, proc_types);

            p_energies_inel.clear();
            p_sliceInts_inel.clear();
            for(size_t islice = 0; islice < thin_slice_inelastic.size(); ++islice ){
                p_energies_inel.push_back(thin_slice_inelastic[islice].first);
                p_sliceInts_inel.push_back(thin_slice_inelastic[islice].second);
            }

            p_energies_el.clear();
            p_sliceInts_el.clear();
            for( size_t islice = 0; islice < thin_slice_elastic.size(); ++islice ){
                p_energies_el.push_back( thin_slice_elastic[islice].first );
                p_sliceInts_el.push_back( thin_slice_elastic[islice].second );
            }

            // Loop through universes 
            for (size_t j = 0; j < weights.size(); ++j) {
                float w;

                parMaker->SetNewVals(UniverseVals.at(j));
                reweighter->SetNewHists(parMaker->GetFSHists());
                reweighter->SetNewElasticHists(parMaker->GetElasticHist());

                w = reweighter->GetWeight(&theTraj);
                weights[j] *= std::max((float) 0.0, w);

                p_weight[j] = w;
            }

            if (bVerbose) {
                std::cout << "PDG = " << p_PDG << std::endl;
                std::cout << "  Weights by particle: ";
                for (unsigned int j=0; j<weights.size(); j++){
                    std::cout << p_weight[j] << ", ";
                }
                std::cout << std::endl;
                std::cout << "  Overall weight saved by event: ";
                for (unsigned int j=0; j<weights.size(); j++){
                    std::cout << weights[j] << ", ";
                }
                std::cout << std::endl;
            }
        }
    }
    WeightsTree->Fill();
}

void CalculateWeights::resetTree() {
    std::fill(weights.begin(), weights.end(), 1.0);
    std::fill(p_weight.begin(), p_weight.end(), 1.0);
}

void CalculateWeights::reconfigure(fhicl::ParameterSet const& pset) {
    bVerbose = pset.get<bool>("Verbose", false);
    particlePDG = pset.get<int>("ParticlePDG", -211);
    simulation_producer_label_ = pset.get<std::string>("SimulationLabel", "largeant");
    numSims = pset.get<int>("number_of_multisim", 0);
    numSigmas = pset.get<int>("number_of_sigmas", 0);
    FracsFileName = pset.get<std::string>("fracsfile");
    FitParSets = pset.get< std::vector<fhicl::ParameterSet>>("parameters");
    fMaterial = pset.get<fhicl::ParameterSet>("material");
    mode = pset.get<std::string>("mode");
}

inline double BetheBloch(double E, double m) {
    static constexpr double K = .307075;
    static constexpr double rho = 1.390;
    static constexpr double Z = 18;
    static constexpr double A = 40;
    static constexpr double I = 188E-6;
    static constexpr double me = .511;

    double gamma = E / m;
    double beta2 = 1. - 1. / (gamma * gamma);
    double Tmax  = 2 * me * beta2 * gamma * gamma;
    return (K * (Z/A) * rho / beta2) * (0.5 * log(Tmax * Tmax / (I * I)) - beta2);
}

std::vector<std::pair<double, int>> ThinSliceBetheBloch(
    G4ReweightTraj * theTraj, 
    double res, 
    double mass, 
    bool isElastic,
    const std::vector<ProcType>& proc_types
) {
    std::vector< std::pair<double, int> > result;

    //First slice position
    //  double sliceEdge = res;
    //  double lastPos = 0.;
    //  double nextPos = 0.;
    //  double px,py,pz;
    int interactInSlice = 0;

    //Get total distance traveled in z
    //  double totalDeltaZ = 0.;
    double disp = 0.;
    //  double oldDisp = 0.;
    //  int crossedSlices = 0;

    int currentSlice = 0;
    int oldSlice = 0;

    double sliceEnergy = theTraj->GetEnergy();
    size_t nSteps = theTraj->GetNSteps();

    double inv_res = 1.0 / res;
    for (size_t is = 0; is < nSteps; ++is) {
        auto theStep  = theTraj->GetStep(is);
        disp         += theStep->GetStepLength();
        currentSlice  = int(disp * inv_res);
        ProcType type = proc_types[is];

        // Check to see if in a new slice and it's not the end
        if (oldSlice != currentSlice && is < nSteps - 1){
            //Save interaction info of the prev slice and reset
            result.push_back(std::make_pair(sliceEnergy, interactInSlice));
            interactInSlice = 0;

            // Update the energy
            sliceEnergy = sliceEnergy - res * BetheBloch(sliceEnergy, mass);
            if (sliceEnergy - mass < 0.) sliceEnergy = 0.0001;

            // If it's more than 1 slice, add in non-interacting slices
            for (int ic = 1; ic < abs(oldSlice - currentSlice); ++ic){
                result.push_back(std::make_pair(sliceEnergy, 0));

                // Update the energy again
                sliceEnergy = sliceEnergy - res * BetheBloch(sliceEnergy, mass);
                if (sliceEnergy - mass < 0.) sliceEnergy = 0.0001;
            }

            if ((!isElastic  && type == ProcType::kInelastic) || (isElastic  && type == ProcType::kElastic)) {
                interactInSlice = 1;
            }
        } else if(oldSlice != currentSlice && is == nSteps - 1) {
            // It's crossed a slice and it's the last step. Save both info

            result.push_back(std::make_pair(sliceEnergy, interactInSlice));
            interactInSlice = 0;

            // Update the energy
            sliceEnergy = sliceEnergy - res*BetheBloch(sliceEnergy, mass);
            if (sliceEnergy - mass < 0.) sliceEnergy = 0.0001;

            //If it's more than 1 slice, add in non-interacting slices
            for (int ic = 1; ic < abs(oldSlice - currentSlice); ++ic) {
                result.push_back( std::make_pair(sliceEnergy, 0));

                // Update the energy again
                sliceEnergy = sliceEnergy - res * BetheBloch(sliceEnergy, mass);
                if (sliceEnergy - mass < 0.) sliceEnergy = 0.0001;
            }

            // Save the last slice
            if ((!isElastic  && type == ProcType::kInelastic) || (isElastic  && type == ProcType::kElastic)) {
                interactInSlice = 1;
            }
            result.push_back(std::make_pair(sliceEnergy, interactInSlice));
        } else if(oldSlice == currentSlice && is == nSteps - 1) {
            // It's the end, so just save this last info
            if ((!isElastic  && type == ProcType::kInelastic) || ( isElastic  && type == ProcType::kElastic)) {
                interactInSlice = 1;
            }
            result.push_back(std::make_pair(sliceEnergy, interactInSlice));
        } else {
            // Same slice, not the end. Check for interactions
            if ((!isElastic  && type == ProcType::kInelastic) || (isElastic  && type == ProcType::kElastic)) {
                interactInSlice = 1;
            }
        }

        // Update old slice
        oldSlice = currentSlice;
    }
    return result;
}

DEFINE_ART_MODULE(CalculateWeights)