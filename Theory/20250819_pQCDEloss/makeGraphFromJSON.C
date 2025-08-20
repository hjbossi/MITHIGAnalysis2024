#include <TGraphAsymmErrors.h>
#include <TCanvas.h>
#include <TAxis.h>
#include <fstream>
#include <iostream>
#include <vector>


#include "nlohmann/json.hpp"
using json = nlohmann::json;

void makeGraphFromJSON() {
    std::ifstream in("neneOutput.json");
    json j;
    in >> j;

    // assuming we only care about min bias for the moment. 
    auto data = j["NeNe_000_100_Pi0"];

    std::vector<double> x_vals, y_central, y_err;
    std::vector<double> x_err_low, x_err_high; 

    for (size_t i = 0; i < data["max"].size(); i++) {
        double x = data["max"][i][0].get<double>();
        double y_max = data["max"][i][1].get<double>();
        double y_min = data["min"][i][1].get<double>();

        double y_mid = 0.5 * (y_max + y_min);
        double err   = 0.5 * (y_max - y_min);

        x_vals.push_back(x);
        y_central.push_back(y_mid);
        y_err.push_back(err);

        x_err_low.push_back(0.0);
        x_err_high.push_back(0.0);
    }

    int n = x_vals.size();

    TGraphAsymmErrors *g = new TGraphAsymmErrors(n, x_vals.data(), y_central.data(), x_err_low.data(), x_err_high.data(), y_err.data(), y_err.data());
    g->SetFillColorAlpha(kRed, 0.35);
    g->SetLineColor(kRed);
    g->SetLineWidth(2);

    // plot just to check if it's correct
    TCanvas *c = new TCanvas("c","c",800,600);
    g->Draw("A3");   // "3" = draw filled band
    g->GetHistogram()->GetXaxis()->SetTitle("#it{p}_{T}");
    g->GetHistogram()->GetYaxis()->SetTitle("#it{R}_{AA}");
    g->GetHistogram()->GetYaxis()->SetRangeUser(0.0, 1.2); 
    c->SaveAs("NeNeMinBias.pdf");

    // write to a root file
    TFile* outFile = new TFile("NeNeMinBias.root", "RECREATE");
    outFile->cd();
    g->SetName("NeNeMinBias");
    g->Write();
    outFile->Close(); 
}
