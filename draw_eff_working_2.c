#include "TStyle.h"
#include <memory>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <utility>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TClonesArray.h>
#include "TFile.h"





void draw_eff_working_2(){

system("mkdir res");

cout << endl << "******* opening files *******" << endl;

TFile* f_MC = new TFile("./SL/tnp_MC_SL_coh_jpsi_1-3_scenario_0.root","read");
TFile* f_Data = new TFile("./SL/tnp_MC_SL_incoh_jpsi_1-3_scenario_0.root","read");


          
cout << endl << "******* geting histos *******" << endl << endl;



(TDirectory*)f_MC->cd("tpTreeTrk/TrkMu_absetadep/fit_eff_plots/");
gDirectory->ls();
TDirectory * dir_MC = gDirectory;
TH1F *eff_MC_abseta = (TH1F *)dir_MC->Get("abseta_PLOT");
eff_MC_abseta->Draw("");
eff_MC_abseta->SaveAs("res/eff_MC_abseta.pdf");

(TDirectory*)f_Data->cd("tpTreeTrk/TrkMu_absetadep/fit_eff_plots/");
gDirectory->ls();
TDirectory * dir_Data = gDirectory;
TH1F *eff_Data_abseta = (TH1F *)dir_Data->Get("abseta_PLOT");
eff_Data_abseta->Draw("");
eff_Data_abseta->SaveAs("res/eff_Data_abseta.pdf");





}

/*

  TCanvas *c1 = new TCanvas("c1","c1",1000,600);
  //d1->SetLogy();

  histo_QQ_rap->GetXaxis()->SetRangeUser(-5, 5);
  histo_QQ_rap->GetYaxis()->SetRangeUser(0, 8000); // zmianiłem
  histo_QQ_rap->GetXaxis()->SetTitleOffset(1.2);
  histo_QQ_rap->GetYaxis()->SetTitleOffset(1.22);
 
  histo_QQ_rap->GetXaxis()->SetTitle("QQ rap");

  //histo_QQ_rap->SetMarkerStyle(8);
  //histo_QQ_rap->SetMarkerColor(kRed);
  histo_QQ_rap->SetLineColor(kRed);
  histo_QQ_rap->SetLineWidth(4);
  histo_QQ_rap_Gen->SetLineColor(kBlue);
  histo_QQ_rap_Gen->SetLineWidth(4);

  histo_QQ_rap->Draw("");
  histo_QQ_rap_Gen->Draw("hist same");

  histo_QQ_rap->SetStats(0); // gets rid of anoying stat box

  TLegend *l1 = new TLegend(0.15,0.70,0.35,0.80,NULL,"brNDC");
  l1->AddEntry(histo_QQ_rap,"Reco, ex cuts","l");
  l1->AddEntry(histo_QQ_rap_Gen,"Gen","l");
  l1->SetFillColor(10);
  l1->Draw();

  c1->Update();
  c1->SaveAs("plots/Acc_QQ_rap.png");


*/




