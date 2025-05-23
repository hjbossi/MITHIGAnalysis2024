#include <TCanvas.h>
#include <TCut.h>
#include <TFile.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TH3D.h>
#include <TLegend.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TGraphAsymmErrors.h>
#include <TLine.h>
#include <TF1.h>
#include <TProfile.h>

#include <iostream>
#include <string>

using namespace std;
#include "CommandLine.h" // Yi's Commandline bundle
#include "Messenger.h"   // Yi's Messengers for reading data files
#include "SetStyle.h"
#include "ProgressBar.h" // Yi's fish progress bar
#include "helpMessage.h" // Print out help message
#include "parameter.h"   // The parameters used in the analysis
#include "utilities.h"   // Yen-Jie's random utility functions


void plotHFJetPerf(std::vector<string> input, string tag, int pthatmin){

    // input files
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);

    // open the files
    TFile* inputFile = TFile::Open(input.at(0).c_str());
    inputFile->ls();
    
    TDirectoryFile* paramDirectory = (TDirectoryFile*)inputFile->Get("par");
    TH1D* hTrigChoice = (TH1D*)paramDirectory->Get("parTriggerChoice");
    TH1D* hMinTrackPt = (TH1D*)paramDirectory->Get("parMinTrackPT");
    TH1D* hMaxTrackPt = (TH1D*)paramDirectory->Get("parMaxTrackPT");
    TH1D* hMaxTrackY = (TH1D*)paramDirectory->Get("parMaxTrackY");
    TH1D* hMinEvis = (TH1D*)paramDirectory->Get("parMinEvis");
    TH1D* hMaxEvis = (TH1D*)paramDirectory->Get("parMaxEvis");
    TH1D* hMinMvis = (TH1D*)paramDirectory->Get("parMinMvis");
    TH1D* hMaxMvis = (TH1D*)paramDirectory->Get("parMaxMvis");
    TH1D* hUseEvisHardScale = (TH1D*)paramDirectory->Get("parUseEvisHardScale");
    TH1D* hIsData = (TH1D*)paramDirectory->Get("parIsData");
    
    TFile* inputFileMC = TFile::Open(input.at(1).c_str());
    inputFileMC->ls();

    // draw the histograms
    TLatex* cms; 
    if(hIsData->GetBinContent(1) == 0) cms = new TLatex(0.12,0.92,"#bf{CMS} #it{Simulation} PYTHIA8 #gammaN (5.36 TeV)");
    else cms = new TLatex(0.12,0.92,"#bf{CMS} UPC PbPb (5.36 TeV)");
    cms->SetNDC();
    cms->SetTextSize(0.05);
    cms->SetTextFont(42);



    // =====================================================
    // Jet pT distributions
    // =====================================================
    TCanvas* c = new TCanvas("c", "c", 600, 600);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLogx(); 
    c->SetLogy(); 
    c->SetRightMargin(0.05);
    c->SetLeftMargin(0.13);

    TLegend* leg = new TLegend(0.16, 0.7, 0.5, 0.85);
    leg->SetBorderSize(0);
    leg->SetFillStyle(0);
    leg->SetTextFont(42);
    leg->SetTextSize(0.035);
    
    TLegend* legDataOnly = new TLegend(0.16, 0.7, 0.5, 0.85);
    legDataOnly->SetBorderSize(0);
    legDataOnly->SetFillStyle(0);
    legDataOnly->SetTextFont(42);
    legDataOnly->SetTextSize(0.035);

  
    TLegend* legDijet = new TLegend(0.16, 0.7, 0.5, 0.85);
    legDijet->SetBorderSize(0);
    legDijet->SetFillStyle(0);
    legDijet->SetTextFont(42);
    legDijet->SetTextSize(0.035);


    TLegend* leg2 = new TLegend(0.16, 0.7, 0.5, 0.85);
    leg2->SetBorderSize(0);
    leg2->SetFillStyle(0);
    leg2->SetTextFont(42);
    leg2->SetTextSize(0.035);
    
    TLegend* legDataMC = new TLegend(0.56, 0.65, 0.7, .75);
    legDataMC->SetBorderSize(0);
    legDataMC->SetFillStyle(0);
    legDataMC->SetTextFont(42);
    legDataMC->SetTextSize(0.035);

    static vector<int> Colors = GetCVDColors6();


    //--------------------------------
    // loop over all of the input files
    //-------------------------------

    // get the histograms
    TH1D* hNev = (TH1D*)inputFile->Get("hNev");
    hNev->SetName(Form("hNev_%d", 0));
    int nEvents = hNev->GetBinContent(1);

    // EEC inclusive
    TH1D* hEECInclusive = (TH1D*)inputFile->Get("hEECInclusive");
    hEECInclusive->SetName(Form("hEECInclusive_%d", 0));


    TH1D* hEECInclusiveMC = (TH1D*)inputFileMC->Get("hEECInclusive");
    hEECInclusiveMC->SetName(Form("hEECInclusiveMC_%d", 0));


    // track pT 
    TH1D* hTrackPtData = (TH1D*)inputFile->Get("hTrackPt"); 
    hTrackPtData->SetName("hTrackPtData"); 
    
    TH1D* hTrackPtMC = (TH1D*)inputFileMC->Get("hTrackPt"); 
    hTrackPtMC->SetName("hTrackPtMC"); 
    
    // track eta 
    TH1D* hTrackEtaData = (TH1D*)inputFile->Get("hTrackEta"); 
    hTrackEtaData->SetName("hTrackEtaData"); 
    
    TH1D* hTrackEtaMC = (TH1D*)inputFileMC->Get("hTrackEta"); 
    hTrackEtaMC->SetName("hTrackEtaMC"); 

        // track mult
    TH1D* hMultData = (TH1D*)inputFile->Get("hMult"); 
    hMultData->SetName("hMultData"); 
    
    TH1D* hMultMC = (TH1D*)inputFileMC->Get("hMult"); 
    hMultMC->SetName("hMultMC"); 

 // Evis
 
     TH1D* hEvisData = (TH1D*)inputFile->Get("hEvis"); 
    hEvisData->SetName("hEvisData"); 
    
    TH1D* hEvisMC = (TH1D*)inputFileMC->Get("hEvis"); 
    hEvisMC->SetName("hEvisMC"); 

    TLatex* TrackString = new TLatex(0.52,0.83,Form("#it{p}_{T, track} > %0.1f GeV, |y_{track}| <  %0.1f ",hMinTrackPt->GetBinContent(1), hMaxTrackY->GetBinContent(1)));
    TrackString->SetNDC();
    TrackString->SetTextSize(0.035);
    TrackString->SetTextFont(42);

    TLatex* VisString; 
    if(hUseEvisHardScale->GetBinContent(1) == 1)VisString = new TLatex(0.6,0.75,Form("%0.1f < #it{E}_{vis} < %0.1f", hMinEvis->GetBinContent(1), hMaxEvis->GetBinContent(1)));
    else VisString = new TLatex(0.6,0.75,Form("%0.1f < #it{E}_{vis} < %0.1f", hMinEvis->GetBinContent(1), hMaxEvis->GetBinContent(1)));
    VisString->SetNDC();
    VisString->SetTextSize(0.035);
    VisString->SetTextFont(42);
    
    TLatex* TrigString;
    TLatex* TrigString2D;
    if(hTrigChoice->GetBinContent(1) == 0){
    TrigString = new TLatex(0.62,0.80,"No Trigger Selection");
    TrigString2D = new TLatex(0.55,0.80,"No Trigger Selection");
    }
    else if(hTrigChoice->GetBinContent(1) == 1){
    TrigString = new TLatex(0.65,0.78,"Data, ZDC 1n OR");
    TrigString2D = new TLatex(0.58,0.78,"Data, ZDC 1n OR");
    }
    else if(hTrigChoice->GetBinContent(1) == 2){
    TrigString = new TLatex(0.65,0.80,"ZDC XOR + Jet 8");
    TrigString2D = new TLatex(0.58,0.80,"ZDC XOR + Jet 8");
    }
    else{
    std::cout << "[Error]: Unrecognized trigger option " << hTrigChoice->GetBinContent(1) << std::endl;
    return;
    }

    TrigString->SetNDC();
    TrigString->SetTextSize(0.035);
    TrigString->SetTextFont(42);

    TrigString2D->SetNDC();
    TrigString2D->SetTextSize(0.035);
    TrigString2D->SetTextFont(42);
    
    leg->AddEntry(hEECInclusive, "Data"); 

    leg->AddEntry(hEECInclusiveMC, Form("Monte Carlo, #hat{#it{p}}_{T, min} = %d", pthatmin)); 


    hEECInclusive->Scale(1.0/hEECInclusive->Integral(), "width"); 
    hEECInclusive->GetYaxis()->SetRangeUser(1e-2, 1);
    hEECInclusive->SetMarkerColor(Colors[0]); 
    hEECInclusive->SetLineColor(Colors[0]); 
    hEECInclusive->SetLineWidth(2); 
    hEECInclusive->SetMarkerStyle(20); 

    hEECInclusiveMC->Scale(1.0/hEECInclusiveMC->Integral(), "width"); 
    hEECInclusiveMC->SetMarkerColor(Colors[0]); 
    hEECInclusiveMC->SetLineColor(Colors[0]); 
    hEECInclusiveMC->SetLineWidth(2); 
    hEECInclusiveMC->SetMarkerStyle(24); 

    hEECInclusive->GetXaxis()->SetTitle("#Delta#it{R}"); 
    hEECInclusive->GetYaxis()->SetTitle("Normalized E2C"); 

    hEECInclusive->Draw(); 
    hEECInclusiveMC->Draw("same"); 
    cms->Draw(); 
    TrigString->Draw(); 
    leg->Draw();
    TrackString->Draw();  
    VisString->Draw(); 
    c->SaveAs(Form("EECDataMCComparison_%s.pdf", tag.c_str()));

    
    TCanvas* c3 = new TCanvas("c3", "c3", 600, 600);
    c3->SetTickx(1);
    c3->SetTicky(1);
    c3->SetRightMargin(0.05);
    c3->SetLeftMargin(0.13);
    
    hTrackEtaData->Scale(1.0/hTrackEtaData->Integral(), "width"); 
    hTrackEtaData->SetMarkerColor(Colors[0]); 
    hTrackEtaData->SetLineColor(Colors[0]); 
    hTrackEtaData->SetLineWidth(2); 
    hTrackEtaData->SetMarkerStyle(20); 
    
    hTrackEtaMC->Scale(1.0/hTrackEtaMC->Integral(), "width"); 
    hTrackEtaMC->SetMarkerColor(Colors[1]); 
    hTrackEtaMC->SetLineColor(Colors[1]); 
    hTrackEtaMC->SetLineWidth(2); 
    hTrackEtaMC->SetMarkerStyle(20); 
    
    legDataMC->AddEntry(hTrackEtaData, "Data");
    legDataMC->AddEntry(hTrackEtaMC,  Form("Monte Carlo, #hat{#it{p}}_{T, min} = %d", pthatmin));
    hTrackEtaData->GetXaxis()->SetTitle("#eta_{Track}"); 
    hTrackEtaData->GetXaxis()->SetTitleOffset(1.1); 
    hTrackEtaData->GetYaxis()->SetTitle("Area and Bin Width Norm.");
    hTrackEtaData->GetYaxis()->SetRangeUser(0, 0.5);
    hTrackEtaData->Draw("same");
    hTrackEtaMC->Draw("same"); 
    legDataMC->Draw(); 
    TrigString->Draw(); 
    TrackString->Draw(); 
    VisString->Draw();  
    cms->Draw(); 
    c3->SaveAs(Form("TrackEta_%s.pdf", tag.c_str() ));
    
    TCanvas* c4 = new TCanvas("c4", "c4", 600, 600);
    c4->SetTickx(1);
    c4->SetTicky(1);
    c4->SetLogy(); 
    c4->SetRightMargin(0.05);
    c4->SetLeftMargin(0.13);
    
    hTrackPtData->Scale(1.0/hTrackPtData->Integral(), "width"); 
    hTrackPtData->SetMarkerColor(Colors[0]); 
    hTrackPtData->SetLineColor(Colors[0]); 
    hTrackPtData->SetLineWidth(2); 
    hTrackPtData->SetMarkerStyle(20); 
    
    hTrackPtMC->Scale(1.0/hTrackPtMC->Integral(), "width"); 
    hTrackPtMC->SetMarkerColor(Colors[1]); 
    hTrackPtMC->SetLineColor(Colors[1]); 
    hTrackPtMC->SetLineWidth(2); 
    hTrackPtMC->SetMarkerStyle(20); 
    
    hTrackPtData->GetXaxis()->SetTitle("p_{T, Track} (GeV)"); 
    hTrackPtData->GetXaxis()->SetTitleOffset(1.1); 
    hTrackPtData->GetYaxis()->SetTitle("Area and Bin Width Norm.");
    hTrackPtData->Draw("same");
    hTrackPtMC->Draw("same"); 
    legDataMC->Draw(); 
    TrigString->Draw(); 
    TrackString->Draw(); 
    VisString->Draw();  
    cms->Draw(); 
    c4->SaveAs(Form("TrackPtSpectrum_%s.pdf", tag.c_str() ));
    
    TCanvas* c5 = new TCanvas("c5", "c5", 600, 600);
    c5->SetTickx(1);
    c5->SetTicky(1);
    c5->SetLogy(); 
    c5->SetRightMargin(0.05);
    c5->SetLeftMargin(0.13);
    
    hMultData->Scale(1.0/hMultData->Integral(), "width"); 
    hMultData->SetMarkerColor(Colors[0]); 
    hMultData->SetLineColor(Colors[0]); 
    hMultData->SetLineWidth(2); 
    hMultData->SetMarkerStyle(20); 
    
    hMultMC->Scale(1.0/hMultMC->Integral(), "width"); 
    hMultMC->SetMarkerColor(Colors[1]); 
    hMultMC->SetLineColor(Colors[1]); 
    hMultMC->SetLineWidth(2); 
    hMultMC->SetMarkerStyle(20); 
    
    hMultData->GetXaxis()->SetTitle("#it{N}_{ch}"); 
    hMultData->GetXaxis()->SetTitleOffset(1.1); 
    hMultData->GetYaxis()->SetTitle("Area and Bin Width Norm.");
    hMultData->Draw("same");
    hMultMC->Draw("same"); 
    legDataMC->Draw(); 
    TrigString->Draw(); 
    TrackString->Draw(); 
    VisString->Draw();  
    cms->Draw(); 
    c5->SaveAs(Form("TrackMult%s.pdf", tag.c_str() ));
    
    
    TCanvas* c6 = new TCanvas("c6", "c6", 600, 600);
    c6->SetTickx(1);
    c6->SetTicky(1);
    c6->SetLogy(); 
    c6->SetRightMargin(0.05);
    c6->SetLeftMargin(0.13);
    
    hEvisData->Scale(1.0/hEvisData->Integral(), "width"); 
    hEvisData->SetMarkerColor(Colors[0]); 
    hEvisData->SetLineColor(Colors[0]); 
    hEvisData->SetLineWidth(2); 
    hEvisData->SetMarkerStyle(20); 
    
    hEvisMC->Scale(1.0/hEvisMC->Integral(), "width"); 
    hEvisMC->SetMarkerColor(Colors[1]); 
    hEvisMC->SetLineColor(Colors[1]); 
    hEvisMC->SetLineWidth(2); 
    hEvisMC->SetMarkerStyle(20); 
    
    hEvisData->GetXaxis()->SetTitle("#it{E}_{vis} (GeV)"); 
    hEvisData->GetXaxis()->SetTitleOffset(1.1); 
    hEvisData->GetYaxis()->SetTitle("Area and Bin Width Norm.");
    hEvisData->Draw("same");
    hEvisMC->Draw("same"); 
    legDataMC->Draw(); 
    TrigString->Draw(); 
    TrackString->Draw();  
    VisString->Draw(); 
    cms->Draw(); 
    c6->SaveAs(Form("Evis%s.pdf", tag.c_str() ));
}

//============================================================//
// Main analysis
//============================================================//
int main(int argc, char *argv[]) {
  if (printHelpMessage(argc, argv))
    return 0;
  CommandLine CL(argc, argv);
  // input to this step is the result of running
  std::vector<string> input = CL.GetStringVector("Input", ""); // Input files
  string tag = CL.Get("tag", "test");
  int pthatmin = CL.GetInt("PtHat", 5); 
  plotHFJetPerf(input, tag, pthatmin);
}
