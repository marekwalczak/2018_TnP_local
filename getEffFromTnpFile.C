#include "TFile.h"
#include "helper.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TRatioPlot.h"
#include "TCanvas.h"
#include "TLegend.h"


void getEffFromTnpFile() {



//  TFile *ftnp_RD = new TFile("./Output/Data/tnp_DATA_ID_scenario_4.root");
//  TFile *ftnp_MC = new TFile("./Output/MC/tnp_MC_ID_scenario_4.root");


  TFile *ftnp_RD = new TFile("./Output/Data/tnp_DATA_TRG_scenario_4.root");
  TFile *ftnp_MC = new TFile("./Output/MC/tnp_MC_TRG_scenario_4.root");





  
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


  // Data vs MC: ID // "TrkMu_1bin", "Trk_ID_absetadep",
//  std::vector<string> plots = { "Trk_ID_pTdep_abseta00_12", "Trk_ID_pTdep_abseta12_16",  "Trk_ID_pTdep_abseta16_21", "Trk_ID_pTdep_abseta16_21", "Trk_ID_pTdep_abseta21_24", "Trk_ID_centdep"};
//  std::vector<string> vars = {"pt", "pt", "pt", "pt", "pt", "tag_hiBin"}; 
  // "pt", "abseta", 
 
  // Data vs MC: TRG // "TrkMu_1bin", "Trk_ID_absetadep",
  std::vector<string> plots = { "Trg_pTdep_abseta00_12", "Trg_pTdep_abseta12_16",  "Trg_pTdep_abseta16_21", "Trg_pTdep_abseta16_21", "Trg_pTdep_abseta21_24", "Trg_centdep"};
  std::vector<string> vars = {"pt", "pt", "pt", "pt", "pt", "tag_hiBin"}; 
  // "pt", "abseta", 
 

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
      //leg->AddEntry(gRD,"Data","lp");
      //leg->AddEntry(gRD,"MC - nominal","lp");
      //leg->AddEntry(gMC,"MC - bkg","lp");
      leg->AddEntry(gRD,"Data","lp");
      leg->AddEntry(gMC,"MC","lp");
    }
    leg->SetHeader(Form("2018 PbPb, %s",plots[i].c_str()));

    tr = new TRatioPlot(g2h(gRD,vars[i].c_str()),g2h(gMC,vars[i].c_str()));
    setTRatioPlotStyle(tr);
    tr->GetLowerRefYaxis()->SetRangeUser(0.5,1.6);
    tr->GetLowerRefYaxis()->SetTitle("Data / MC");
    
    
    cout << endl;
    for (int j = 0; j < 8; j++)
    {
    
    //cout << "DATA: " << g2h(gRD,vars[i].c_str())->GetBinContent(j+1) << " +/- " << g2h(gRD,vars[i].c_str())->GetBinError(j+1) << endl;
    //cout << "MC  : " << g2h(gMC,vars[i].c_str())->GetBinContent(j+1) << " +/- " << g2h(gMC,vars[i].c_str())->GetBinError(j+1) << endl;
    //cout << "SF = " << g2h(gRD,vars[i].c_str())->GetBinContent(j+1)/g2h(gMC,vars[i].c_str())->GetBinContent(j+1) << endl;
    
    double x, y, e; // this is where the coordinates of the point will be stored
    // now let's check the coordinates of e.g. 10th point
    tr->GetLowerRefGraph()->GetPoint(j, x, y);
    e = tr->GetLowerRefGraph()->GetErrorY(j);
    //printf("ratio of point %i @ x=%f is %f +/- %f\n", j+1, x, y, e);
    printf("%f, ", e);
    
    }
    cout << endl;
    
      //  cout << tr->GetLowerRefXaxis()->GetBinContent(3) << endl;
    
    c->Update();
    tr->GetUpperPad()->cd();
    leg->Draw();
    
    
    gStyle->SetOptTitle(0);
    //TPaveLabel *title = new TPaveLabel(0.4,0.4,0.6,0.6,"new title","brndc");
    //title->Draw();  
    
    
    c->SaveAs(Form("plots_Data_MC/TRG_test/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_Data_MC/1-3/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_Data_MC/3/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_Data_MC/syst_bkg_MC/hist_%s.pdf",plots[i].c_str()));
    //c->SaveAs(Form("plots_SL/jpsi_1-3/hist_%s.pdf",plots[i].c_str()));
  }

}

