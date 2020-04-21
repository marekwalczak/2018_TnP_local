#include "TFile.h"
#include "helper2.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TRatioPlot.h"
#include "TCanvas.h"
#include "TLegend.h"


void getEffFromTnpFile2() {



  TFile *ftnp_RD = new TFile("./SL/tnp_MC_1-3_scenario_10_Upsilon.root");
  TFile *ftnp_MC = new TFile("./SL/tnp_MC_1-3_scenario_10_Upsilon_Pythia.root");


  
  RooDataSet *ds_RD=NULL;  RooDataSet *ds_MC=NULL;
  TGraphAsymmErrors *gRD=NULL;  TGraphAsymmErrors *gMC=NULL; TGraphAsymmErrors* pull=NULL;
  TCanvas* c = new TCanvas(); 
  TRatioPlot *tr = NULL;


  TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
  leg->SetBorderSize(0); leg->SetFillColor(0);


  std::vector<string> plots = {"TrkMu_abseta00_08", "TrkMu_abseta08_16", "TrkMu_abseta16_24"};

  std::vector<string> vars = {"pt", "pt", "pt"}; 


  for (uint i=0; i<plots.size(); i++) {
    c->cd(); c->Clear();
    ds_RD = (RooDataSet*) ftnp_RD->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    ds_MC = (RooDataSet*) ftnp_MC->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    if (!ds_RD) { std::cout << "RD IS NULL"; }
    if (!ds_MC) { std::cout << "MC IS NULL"; }
    if (!ds_RD || !ds_MC) continue;
    gRD = plotEff(ds_RD,1,vars[i].c_str());
    gMC = plotEff(ds_MC,1,vars[i].c_str());
    gRD->SetLineColor(kBlue);
    gRD->SetMarkerColor(kBlue);
    gRD->SetMarkerStyle(kOpenSquare);
    gMC->SetLineColor(kRed);
    gMC->SetMarkerColor(kRed);
    gMC->SetMarkerStyle(kFullCircle);
    if (i==0){
      leg->AddEntry(gRD,"STARLIGHT","lp");
      leg->AddEntry(gMC,"Pythia MC","lp");
    }
    leg->SetHeader(Form("2018 PbPb, trigger, %s",plots[i].c_str()));

    tr = new TRatioPlot(g2h(gRD,vars[i].c_str()),g2h(gMC,vars[i].c_str()));
    setTRatioPlotStyle(tr);
    tr->GetLowerRefYaxis()->SetRangeUser(0.5,2);
    tr->GetLowerRefYaxis()->SetTitle("Data / MC");
    c->Update();
    tr->GetUpperPad()->cd();
    leg->Draw();
    c->SaveAs(Form("plots_UPsTest2/hist_%s.pdf",plots[i].c_str()));
  }

}

