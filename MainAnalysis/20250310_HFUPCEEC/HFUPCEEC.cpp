#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TLorentzVector.h>
// include statements to pick up the temporary JECs from Nick
// #include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetUncertainty.h"
// #include "/afs/cern.ch/user/n/nbarnett/public/header_files/JetCorrector.h"

#include <iostream>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


//======= jetSelection =====================================//
// Check if the jet pass selection criteria
// for the moment the selections are handled in the actual functions, but could consider to move them here.
//============================================================//
bool jetSelection(DzeroJetUPCTreeMessenger *b, Parameters par, int j) {
  return true;
}

//======= triggerSelection =====================================//
// Here the event selection was done at the level of the skimmer
// do the trigger selection here
// could consider to do additional event selection for gammaN or Ngamma here too, but for now we don't (consider Ngamma and GammaN as combined)
//============================================================//
bool triggerSelection(DzeroJetUPCTreeMessenger *b, const Parameters &par) {
  if (par.IsData == false)
    return true;
  if (par.TriggerChoice == 1 && b->isL1ZDCOr == false)
    return false;
  if (par.TriggerChoice == 2 && b->isL1ZDCXORJet8 == false) return false;
  return true;
}

class DataAnalyzer {
public:
  TFile *inf, *outf;
  // histogram to keep track of the number of events
  TH1D *hNev;

  // EEC histograms
  TH1D* hEECInclusive; // inclusive EEC for gamma N UPC events
  TH1D* hTrackPt; // track pT
  TH1D* hTrackEta; // track eta
  TH1D* hPtSum; // total scalar pT sum
  TH1D* hMtSum; // total transverse mass
  TH1D* hMult; // multi


  DzeroJetUPCTreeMessenger *MDzeroJetUPC;


  DataAnalyzer(const char *filename, const char *outFilename, const char *mytitle = "")
      : inf(new TFile(filename)), MDzeroJetUPC(new DzeroJetUPCTreeMessenger(*inf, string("Tree"))),
        outf(new TFile(outFilename, "recreate")) {
    outf->cd();
  }

  ~DataAnalyzer() {
    deleteHistograms();
    inf->Close();
    outf->Close();
    delete MDzeroJetUPC;
  }

  void analyze(Parameters &par) {
    outf->cd();
    int numBins = 25;  // Number of bins
    double start = 0.005, stop = 5.0;

    // Compute logarithmic bin edges
    std::vector<double> EECBins;
    for (int i = 0; i < numBins; ++i) {
        double binEdge = start * pow(stop / start, static_cast<double>(i) / (numBins - 1));
        EECBins.push_back(binEdge);
    }


    hNev = new TH1D("hNev", "", 1, 0, 1);
    hEECInclusive = new TH1D("hEECInclusive", "", EECBins.size()-1, EECBins.data()); 
    hTrackPt = new TH1D("hTrackPt", "", 50, 0, 50); 
    hMult = new TH1D("hMult", "", 50, 0, 50);
    hTrackEta = new TH1D("hTrackEta", "", 20, -5, 5); 
    hPtSum =new TH1D("hPtSum", "", 50, 0, 200); 
    hMtSum = new TH1D("hMtSum", "", 50, 0, 200); 
    

    par.printParameters();
    unsigned long nEntry = MDzeroJetUPC->GetEntries() * par.scaleFactor;
    std::cout << "Processing " << nEntry << " events." << std::endl;
    ProgressBar Bar(cout, nEntry);
    Bar.SetStyle(1);
    // loop over the number of events
    int numberAccEvents = 0;
    for (unsigned long i = 0; i < nEntry; i++) {
      MDzeroJetUPC->GetEntry(i);

      if(i % 1000 == 0) std::cout << "On entry " << i << std::endl;
      
      // only difference in the data part is the trigger
      if (par.IsData && triggerSelection(MDzeroJetUPC, par)) {
         numberAccEvents++; // increment the number of events
         hMult->Fill(MDzeroJetUPC->Nch);
         
         // first loop over tracks to fill the scalar pT sum and the transverse mass sum
         double PtSum = 0.0; 
         double MtSum = 0.0; 
         for (unsigned long a = 0; a < MDzeroJetUPC->particleFlow_pT->size(); a++) {
            PtSum += MDzeroJetUPC->particleFlow_pT->at(a);
            MtSum += MDzeroJetUPC->particleFlow_M->at(a);
         } // end first loop over the number of tracks

         
         
         // skip event if it is outside of the desired hard scale
         // case 1: using visible energy as the hard scale
         if(par.UsePtSumHardScale && (PtSum < par.MinPtSum || PtSum > par.MaxPtSum)) continue; 
         // case 2: using the visible mass as the hard scale.
         if(!par.UsePtSumHardScale && (MtSum < par.MinMtSum || MtSum > par.MaxMtSum)) continue; 

         // now fill the EEC if the event has the desired hard scale
          for (unsigned long a = 0; a < MDzeroJetUPC->Nch; a++) {
            // acceptance cuts on track a
            if( MDzeroJetUPC->trkPt->at(a) < par.MinTrackPT ) continue; 
            if(abs(MDzeroJetUPC->trkEta->at(a)) >  par.MaxTrackY) continue; 
            hTrackPt->Fill(MDzeroJetUPC->trkPt->at(a)); 
            hTrackEta->Fill(MDzeroJetUPC->trkEta->at(a)); 
            
            for (unsigned long b = a+1; b < MDzeroJetUPC->Nch; b++) {
              // acceptance cuts on track b
              if(MDzeroJetUPC->trkPt->at(b) < par.MinTrackPT ) continue; 
              if(abs(MDzeroJetUPC->trkEta->at(b) ) >  par.MaxTrackY ) continue; 

              double deltaEta = MDzeroJetUPC->trkEta->at(a) - MDzeroJetUPC->trkEta->at(b); 
              double deltaPhi = MDzeroJetUPC->trkPhi->at(a) - MDzeroJetUPC->trkPhi->at(b); 
              double deltaR = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi); 
              double EEC = MDzeroJetUPC->trkPt->at(a)* MDzeroJetUPC->trkPt->at(b);
              hEECInclusive->Fill(deltaR, EEC); 
            } // end of track loop   
          } // end of track loop
      }

      // -----------------------

      //-----------------------------------
      //----- MC analysis-----
      //-----------------------------------
      if (!par.IsData) {
        numberAccEvents++; // increment the number of events
        hMult->Fill(MDzeroJetUPC->Nch);

        // first loop over tracks to fill the visible energy and the visible mass
        double PtSum = 0.0; 
        double MtSum = 0.0; 
        for (unsigned long a = 0; a < MDzeroJetUPC->Nch; a++) {
          PtSum += MDzeroJetUPC->PFEnergy->at(a);
          TLorentzVector tVec; 
          tVec.SetPtEtaPhiE(MDzeroJetUPC->trkPt->at(a), MDzeroJetUPC->trkEta->at(a), MDzeroJetUPC->trkPhi->at(a), MDzeroJetUPC->PFEnergy->at(a));
          MtSum += tVec.M(); 
        } // end first loop over the number of tracks
        hPtSum->Fill(PtSum); 
        hMtSum->Fill(MtSum); 
        
        // skip event if it is outside of the desired hard scale
         // case 1: using visible energy as the hard scale
         if(par.UsePtSumHardScale && (PtSum < par.MinPtSum || PtSum > par.MaxPtSum)) continue; 
         // case 2: using the visible mass as the hard scale.
         if(!par.UsePtSumHardScale && (MtSum < par.MinMtSum || MtSum > par.MaxMtSum)) continue; 
         
        for (unsigned long a = 0; a < MDzeroJetUPC->Nch; a++) {
            // acceptance cuts on track a
            if( MDzeroJetUPC->trkPt->at(a) < par.MinTrackPT ) continue; 
            if(abs(MDzeroJetUPC->trkEta->at(a)) >  par.MaxTrackY) continue; 
            hTrackPt->Fill(MDzeroJetUPC->trkPt->at(a)); 
            hTrackEta->Fill(MDzeroJetUPC->trkEta->at(a)); 
            for (unsigned long b = a+1; b < MDzeroJetUPC->Nch; b++) {
              // acceptance cuts on track b
              if(MDzeroJetUPC->trkPt->at(b) < par.MinTrackPT ) continue; 
              if(abs(MDzeroJetUPC->trkEta->at(b) ) >  par.MaxTrackY ) continue; 
              double deltaEta = MDzeroJetUPC->trkEta->at(a) - MDzeroJetUPC->trkEta->at(b); 
              double deltaPhi = MDzeroJetUPC->trkPhi->at(a) - MDzeroJetUPC->trkPhi->at(b); 
              double deltaR = sqrt(deltaEta*deltaEta + deltaPhi*deltaPhi); 
              double EEC = MDzeroJetUPC->trkPt->at(a)* MDzeroJetUPC->trkPt->at(b);
              hEECInclusive->Fill(deltaR, EEC); 
            } // end of track loop   
        }
      
      

        
      }   // end of mc
      // -----------------------

    }     // end of event loop

    hNev->SetBinContent(1, numberAccEvents);
    std::cout << "The number of accepted events is " << numberAccEvents << std::endl;

  }       // end of analyze

  void writeHistograms(TFile *outf) {
    outf->cd();
    smartWrite(hNev);
    smartWrite(hEECInclusive);
    smartWrite(hTrackPt); 
    smartWrite(hTrackEta); 
    smartWrite(hPtSum); 
    smartWrite(hMtSum);
    smartWrite(hMult);


  }

private:
  void deleteHistograms() {
    delete hNev;
    delete hEECInclusive;
    delete hPtSum; 
    delete hMult; 
    delete hTrackPt; 
    delete hTrackEta;



  }
};

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  bool IsGammaN = CL.GetBool("IsGammaN", true);      // GammaN analysis (or NGamma)
  float MinTrackPt = CL.GetDouble("MinTrackPT", 0.0); // Minimum track PT
  float MaxTrackPt = CL.GetDouble("TrackJetPT", 10000); // Max track PT
  float MinTrackY = CL.GetDouble("MinTrackY", -2.4); // min track y
  float MaxTrackY = CL.GetDouble("MaxTrackY", 2.4); // max track y
  float MinPtSum = CL.GetDouble("MinPtSum", 0.0); // Minimum visible energy
  float MaxPtSum = CL.GetDouble("MaxPtSum", 10000); // Max visible energy
  float MinMtSum = CL.GetDouble("MinMtSum", 0.0); // Minimum visible mass
  float MaxMtSum = CL.GetDouble("MaxMtSum", 10000); // Max visible Mass
  bool UsePtSumHardScale = CL.GetBool("UsePtSumHardScale", 1); 
  int TriggerChoice = CL.GetInt("TriggerChoice", 2); // 0 = no trigger sel, 1 = isL1ZDCOr, 2 = isL1ZDCXORJet8
  float scaleFactor = CL.GetDouble("scaleFactor", 1); // Scale factor for the number of events to be processed.
  bool IsData = CL.GetBool("IsData", 0);              // Data or MC
  Parameters par(MinTrackPt, MaxTrackPt, MinTrackY, MaxTrackY, MinPtSum, MaxPtSum, MinMtSum, MaxMtSum,  IsGammaN,UsePtSumHardScale, TriggerChoice, IsData, scaleFactor);
  par.input = CL.Get("Input", "mergedSample.root"); // Input file
  par.output = CL.Get("Output", "output.root");     // Output file
  par.nThread = CL.GetInt("nThread", 1);            // The number of threads to be used for parallel processing.
  par.nChunk =  CL.GetInt("nChunk", 1); // Specifies which chunk (segment) of the data to process, used in parallel processing.
  std::cout << "Parameters are set" << std::endl;
  // Analyze Data
  DataAnalyzer analyzer(par.input.c_str(), par.output.c_str(), "");
  analyzer.analyze(par);
  analyzer.writeHistograms(analyzer.outf);
  par.printParameters();
  saveParametersToHistograms(par, analyzer.outf);
  cout << "done!" << analyzer.outf->GetName() << endl;
}
