#include "TFile.h"
#include "helper.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TRatioPlot.h"
#include "TCanvas.h"
#include "TLegend.h"


void getEffFromTnpFile() {



  TFile *ftnp_RD = new TFile("~/Downloads/Data_cent_pTa1/tnp_DATA_3_scenario_0.root");
  TFile *ftnp_MC = new TFile("~/Downloads/MC_cent_pTa1/tnp_MC_3_scenario_0.root");

  //TFile *ftnp_RD = new TFile("./files/Data/tnp_Ana_DATA_2_ID_tpTreeTrk_scenario_0.root");
  //TFile *ftnp_MC = new TFile("./files/MC/tnp_Ana_MC_2_ID_tpTreeTrk_scenario_0.root");

  //TFile *ftnp_RD = new TFile("./files/Data/tnp_Ana_DATA_3_Trg_tpTreeTrk_scenario_0.root");
  //TFile *ftnp_MC = new TFile("./files/MC/tnp_Ana_MC_3_Trg_tpTreeTrk_scenario_0.root");


  //TFile *ftnp_RD = new TFile("./files/Data/tnp_Ana_DATA_1-3_SM_scenario_0.root");
  //TFile *ftnp_MC = new TFile("./files/MC/tnp_Ana_MC_PbPb_1-3_SM_scenario_0.root");
  
  RooDataSet *ds_RD=NULL;  RooDataSet *ds_MC=NULL;
  TGraphAsymmErrors *gRD=NULL;  TGraphAsymmErrors *gMC=NULL; TGraphAsymmErrors* pull=NULL;
  TCanvas* c = new TCanvas(); 
  TRatioPlot *tr = NULL;

  TLegend* leg = new TLegend(0.6,0.2,0.9,0.4);
  leg->SetBorderSize(0); leg->SetFillColor(0);


  //std::vector<string> plots = {"Trg_pair_eta00_12_cent0_10", "Trg_pair_eta00_12_cent10_20", "Trg_pair_eta00_12_cent20_40", "Trg_pair_eta00_12_cent40_60", "Trg_pair_eta00_12_cent60_100", "Trg_pair_eta00_12_cent0_100"};
  std::vector<string> plots = {"TrkMu_absetadep", "TrkMu_pt","TrkMu_centdep"};
  //std::vector<string> plots = {"TrkMu_abseta00_09", "TrkMu_abseta09_12", "TrkMu_abseta12_21", "TrkMu_abseta21_24", "TrkMu_absetadep", "TrkMu_pt"};
//{"Trg_pair_eta00_12_cent0_10", "Trg_pair_eta00_12_cent10_20", "Trg_pair_eta00_12_cent20_40", "Trg_pair_eta00_12_cent40_60", "Trg_pair_eta00_12_cent60_100", "Trg_pair_eta00_12_cent0_100"};
  std::vector<string> vars = {"abseta", "pt", "tag_hiBin"}; //{"pt"};
//{"pt", "pt", "pt", "pt", "pt", "pt"};

  for (uint i=0; i<plots.size(); i++) {
    c->cd(); c->Clear();
    ds_RD = (RooDataSet*) ftnp_RD->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    ds_MC = (RooDataSet*) ftnp_MC->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    if (!ds_RD) { std::cout << "WTF RD IS NULL"; }
    if (!ds_MC) { std::cout << "WTF MC IS NULL"; }
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
      leg->AddEntry(gRD,"Data","lp");
      leg->AddEntry(gMC,"MC","lp");
    }
    leg->SetHeader(Form("pbpb, trigger, %s",plots[i].c_str()));

    tr = new TRatioPlot(g2h(gRD,1,vars[i]),g2h(gMC,1,vars[i]));
    setTRatioPlotStyle(tr);
    tr->GetLowerRefYaxis()->SetRangeUser(0.69,1.41);
    tr->GetLowerRefYaxis()->SetTitle("Data / MC");
    c->Update();
    tr->GetUpperPad()->cd();
    leg->Draw();
    c->SaveAs(Form("plots/3_%s.pdf",plots[i].c_str()));
  }

}

