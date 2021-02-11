#include "TFile.h"
#include "helper.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TRatioPlot.h"
#include "TCanvas.h"
#include "TLegend.h"


void getEffFromTnpFile_syst() {

// systematics
//TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-3_scenario_10_cent_190.root");
//tnp_DATA_1-3_scenario_10_cbcbPlusPol2.root
//tnp_DATA_1-3_scenario_10_cbGausPlusPol1.root
//tnp_DATA_1-3_scenario_10_cbcbPlusPol1_loose.root
//tnp_DATA_1-3_scenario_10_mass.root
//tnp_DATA_1-3_scenario_10_cent_190.root


  TFile *ftnp_RD = new TFile("./Output/Data/tnp_DATA_1-2_scenario_10.root");
  
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_cbcbPlusPol1_loose.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_cbcbPlusPol2.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_cbGausPlusPol1.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_cent_190.root");
  TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_cent_170.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_1-2_scenario_10_mass.root");


  //TFile *ftnp_RD = new TFile("./Output/Data/tnp_DATA_3_scenario_10.root");
  
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_cbcbPlusPol1_loose.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_cbcbPlusPol2.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_cbGausPlusPol1.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_cent_190.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_cent_170.root");
  //TFile *ftnp_MC = new TFile("./Output/Data_syst/tnp_DATA_3_scenario_10_mass.root");



  
  RooDataSet *ds_RD=NULL;  RooDataSet *ds_MC=NULL;
  TGraphAsymmErrors *gRD=NULL;  TGraphAsymmErrors *gMC=NULL; TGraphAsymmErrors* pull=NULL;
  TCanvas* c = new TCanvas(); 
  TRatioPlot *tr = NULL;
  
  
  //TLegend* leg = new TLegend(0.66,0.7,0.94,0.87); // top right
  //TLegend* leg = new TLegend(0.12,0.7,0.45,0.87); // top left
  TLegend* leg = new TLegend(0.66,0.1,0.94,0.27); // bottom right 
  //TLegend* leg = new TLegend(0.12,0.1,0.45,0.27); // bottom left 
  //TLegend* leg = new TLegend(0.36,0.1,0.64,0.27); // center bottom
  leg->SetBorderSize(1); leg->SetFillColor(0);


  // Data vs MC: 1-3
  //std::vector<string> plots = { "Tot_absetadep_pT1d4_1d9", "Tot_absetadep_pT1d9_2d4",  "Tot_absetadep_pT2d4_2d9", "Tot_absetadep_pT2d9_3d4", "Tot_absetadep_pT3d4_6", "Tot_absetadep_pT6_10", "Tot_absetadep_pT1d4_10", "Tot_pTdep_abseta00_08", "Tot_pTdep_abseta08_16",  "Tot_pTdep_abseta16_24"};
  //std::vector<string> vars = { "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "pt", "pt", "pt"}; 

  // Data vs MC: 1-2
  std::vector<string> plots = { "Trk_ID_absetadep_pT1d4_1d9", "Trk_ID_absetadep_pT1d9_2d4",  "Trk_ID_absetadep_pT2d4_2d9", "Trk_ID_absetadep_pT2d9_3d4", "Trk_ID_absetadep_pT3d4_6", "Trk_ID_absetadep_pT6_10", "Trk_ID_absetadep_pT1d4_10", "Trk_ID_pTdep_abseta00_08", "Trk_ID_pTdep_abseta08_16",  "Trk_ID_pTdep_abseta16_24"};
  std::vector<string> vars = { "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "pt", "pt", "pt"}; 


  // Data vs MC: 3
  //std::vector<string> plots = { "Trg_absetadep_pT1d4_1d9", "Trg_absetadep_pT1d9_2d4",  "Trg_absetadep_pT2d4_2d9", "Trg_absetadep_pT2d9_3d4", "Trg_absetadep_pT3d4_6", "Trg_absetadep_pT6_10", "Trg_absetadep_pT1d4_10", "Trg_pTdep_abseta00_08", "Trg_pTdep_abseta08_16",  "Trg_pTdep_abseta16_24"};
  //std::vector<string> vars = { "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "pt", "pt", "pt"}; 


  
  
  // SL Upsilon
  //std::vector<string> plots = {"Tot_absetadep_pT1_1d5", "Tot_absetadep_pT1d5_2", "Tot_absetadep_pT2_2d5",  "Tot_absetadep_pT2d5_3", "Tot_absetadep_pT3_4", "Tot_absetadep_pT4_5", "Tot_absetadep_pT5_6", "Tot_pTdep_abseta00_08", "Tot_pTdep_abseta08_16", "Tot_pTdep_abseta16_24"};
  //std::vector<string> vars = {"abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "abseta", "pt", "pt", "pt"}; 

  // SL jpsi
  //std::vector<string> plots = {"Tot_pTdep_abseta20_24", "Tot_pTdep_abseta20_22", "Tot_pTdep_abseta22_24"};
  //std::vector<string> vars = {"pt", "pt", "pt"}; 



  //std::vector<string> plots = {"TrkMu_centdep"};
  //std::vector<string> vars = {"tag_hiBin"}; 

  
  //std::vector<string> plots = {"ID_1bin_pT1_10"};
  //std::vector<string> vars = {"pt"}; 

 

  for (uint i=0; i<plots.size(); i++) {
    c->cd(); c->Clear();
    
   
    
    ds_RD = (RooDataSet*) ftnp_RD->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    ds_MC = (RooDataSet*) ftnp_MC->Get(Form("tpTreeTrk/%s/fit_eff",plots[i].c_str()));
    if (!ds_RD) { std::cout << "-> RD IS NULL " << endl; }
    if (!ds_MC) { std::cout << "-> MC IS NULL " << endl; }
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
      leg->AddEntry(gRD,"SoftID eff - nominal","lp");
      leg->AddEntry(gMC,"SoftID eff - centrality 170 - 200","lp");
 
    }
    leg->SetHeader(Form("2018 PbPb, %s",plots[i].c_str()));

    tr = new TRatioPlot(g2h(gRD,vars[i].c_str()),g2h(gMC,vars[i].c_str()));
    setTRatioPlotStyle(tr);
    tr->GetLowerRefYaxis()->SetRangeUser(0.5,1.6);
    tr->GetLowerRefYaxis()->SetTitle("nominal / syst");
    c->Update();
    tr->GetUpperPad()->cd();
    leg->Draw();
    
    
    gStyle->SetOptTitle(0);
    //TPaveLabel *title = new TPaveLabel(0.4,0.4,0.6,0.6,"new title","brndc");
    //title->Draw();  
    
    
    //c->SaveAs(Form("plots_syst/1-2/loose/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/1-2/bkg/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/1-2/sig/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/1-2/cent/hist_%s.pdf",plots[i].c_str()));
    c->SaveAs(Form("plots_syst/1-2/cent_170/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/1-2/mass/hist_%s.pdf",plots[i].c_str()));

    
    //c->SaveAs(Form("plots_syst/3/loose/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/3/bkg/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/3/sig/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/3/cent/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/3/cent_170/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_syst/3/mass/hist_%s.pdf",plots[i].c_str()));
    
  }

}

