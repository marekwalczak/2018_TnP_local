#ifndef helper_h
#define helper_h

#include "TGraphAsymmErrors.h"
#include "RooDataSet.h"
#include "RooRealVar.h"
#include "TAxis.h"
#include "TRandom3.h"
#include "TH1.h"
#include "TStyle.h"
#include "TRatioPlot.h"

TGraphAsymmErrors *plotEff(RooDataSet *a, int aa, const char* varx){
  const RooArgSet *set = (a ? a->get() : NULL);
  RooRealVar *xAx = (set ? (RooRealVar*)set->find(varx) : NULL);
  RooRealVar *eff = (set ? (RooRealVar*)set->find("efficiency") : NULL);

  const int nbins = (xAx ? xAx->getBinning().numBins() : 0);

  double tx[nbins], txhi[nbins], txlo[nbins];
  double ty[nbins], tyhi[nbins], tylo[nbins];

  for (int i=0, j=0; i<nbins; i++) {
    a->get(j);
    const auto binCenter = xAx->getBinning().binCenter(i);
    const auto binWidth = fabs(xAx->getBinning().binWidth(i)/2.);
    const auto binValue = xAx->getVal();
    const bool hasEff = (fabs(binValue - binCenter) <= binWidth);
    tx[i] = binCenter;
    txhi[i] = binWidth;
    txlo[i] = binWidth;
    ty[i] = (hasEff ? eff->getVal() : 0.0);
    tyhi[i] = (hasEff ? fabs(eff->getErrorHi()) : 0.0);
    tylo[i] = (hasEff ? fabs(eff->getErrorLo()) : 0.0);
    if (hasEff) { j++; }
    std::cout << xAx->getVal() << "  " << fabs(xAx->getBinning().binWidth(i)/2.) << "  " << xAx->getError() << "  " << tx[i] << "  " << ty[i] << std::endl;
  }

  cout<<"NBins : "<<nbins<<endl;

  const double *x = tx;
  const double *xhi = txhi;
  const double *xlo = txlo;
  const double *y = ty;
  const double *yhi = tyhi;
  const double *ylo = tylo;


  TGraphAsymmErrors *b = NULL;
  if(aa == 1) {b = new TGraphAsymmErrors(nbins,x,y,xlo,xhi,ylo,yhi);}
  if(aa == 0) {b = new TGraphAsymmErrors(nbins,x,y,0,0,ylo,yhi);}

  b->SetMaximum(1.1);
  b->SetMinimum(0.0);
  b->SetMarkerStyle(20);
  b->SetMarkerColor(kRed+2);
  b->SetMarkerSize(1.0);
  b->SetTitle("");
  b->GetXaxis()->SetTitleSize(0.1);
  b->GetYaxis()->SetTitleSize(0.1);
  b->GetXaxis()->SetTitle("#eta");
  b->GetYaxis()->SetTitle("Efficiency");
  b->GetXaxis()->CenterTitle();

  for (int i=0; i<nbins; i++) {
    cout << "BIN: " <<  x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
  }

  return b;

};

bool IsAccept(Double_t pt, Double_t eta)
{

     //return(1);
     //return (TMath::Abs(eta) < 2.4); 

   return (TMath::Abs(eta) < 2.4 &&
         (    ( TMath::Abs(eta) < 1.0 && pt >= 3.4 ) ||
              (  1.0 <= TMath::Abs(eta) && TMath::Abs(eta) < 1.5 && pt >= 5.8-2.4*TMath::Abs(eta) ) ||
              (  1.5 <= TMath::Abs(eta) && pt >= 3.3667 - 7.0/9.0*TMath::Abs(eta)) ));
};

double findCenWeight(const int Bin) {
  double NCollArray[40]={
    1747.8600, 1567.5300, 1388.3900, 1231.7700, 1098.2000, 980.4390, 861.6090, 766.0420, 676.5150, 593.4730,
    521.9120, 456.5420, 398.5460, 346.6470, 299.3050, 258.3440, 221.2160, 188.6770, 158.9860, 134.7000,
    112.5470, 93.4537, 77.9314, 63.5031, 52.0469, 42.3542, 33.9204, 27.3163, 21.8028, 17.2037,
    13.5881, 10.6538, 8.3555, 6.4089, 5.1334, 3.7322, 3.0663, 2.4193, 2.1190, 1.7695
  };
  return(NCollArray[Bin]);
};

Double_t findNcoll(int hiBin) {
   const int nbins = 200;
   const Double_t Ncoll[nbins] = {1976.95, 1944.02, 1927.29, 1891.9, 1845.3, 1807.2, 1760.45, 1729.18, 1674.8, 1630.3, 1590.52, 1561.72, 1516.1, 1486.5, 1444.68, 1410.88, 1376.4, 1347.32, 1309.71, 1279.98, 1255.31, 1219.89, 1195.13, 1165.96, 1138.92, 1113.37, 1082.26, 1062.42, 1030.6, 1009.96, 980.229, 955.443, 936.501, 915.97, 892.063, 871.289, 847.364, 825.127, 806.584, 789.163, 765.42, 751.187, 733.001, 708.31, 690.972, 677.711, 660.682, 640.431, 623.839, 607.456, 593.307, 576.364, 560.967, 548.909, 530.475, 519.575, 505.105, 490.027, 478.133, 462.372, 451.115, 442.642, 425.76, 416.364, 405.154, 392.688, 380.565, 371.167, 360.28, 348.239, 340.587, 328.746, 320.268, 311.752, 300.742, 292.172, 281.361, 274.249, 267.025, 258.625, 249.931, 240.497, 235.423, 228.63, 219.854, 214.004, 205.425, 199.114, 193.618, 185.644, 180.923, 174.289, 169.641, 161.016, 157.398, 152.151, 147.425, 140.933, 135.924, 132.365, 127.017, 122.127, 117.817, 113.076, 109.055, 105.16, 101.323, 98.098, 95.0548, 90.729, 87.6495, 84.0899, 80.2237, 77.2201, 74.8848, 71.3554, 68.7745, 65.9911, 63.4136, 61.3859, 58.1903, 56.4155, 53.8486, 52.0196, 49.2921, 47.0735, 45.4345, 43.8434, 41.7181, 39.8988, 38.2262, 36.4435, 34.8984, 33.4664, 31.8056, 30.351, 29.2074, 27.6924, 26.7754, 25.4965, 24.2802, 22.9651, 22.0059, 21.0915, 19.9129, 19.1041, 18.1487, 17.3218, 16.5957, 15.5323, 14.8035, 14.2514, 13.3782, 12.8667, 12.2891, 11.61, 11.0026, 10.3747, 9.90294, 9.42648, 8.85324, 8.50121, 7.89834, 7.65197, 7.22768, 6.7755, 6.34855, 5.98336, 5.76555, 5.38056, 5.11024, 4.7748, 4.59117, 4.23247, 4.00814, 3.79607, 3.68702, 3.3767, 3.16309, 2.98282, 2.8095, 2.65875, 2.50561, 2.32516, 2.16357, 2.03235, 1.84061, 1.72628, 1.62305, 1.48916, 1.38784, 1.28366, 1.24693, 1.18552, 1.16085, 1.12596, 1.09298, 1.07402, 1.06105, 1.02954};
   return Ncoll[hiBin];
};

TH1F *g2h(TGraphAsymmErrors *g, const std::string& varName="") {
   int n = g->GetN();
   double *x = g->GetX();
   double *y = g->GetY();
   double *exl = g->GetEXlow();
   double *exh = g->GetEXhigh();
   double *eyl = g->GetEYlow();
   double *eyh = g->GetEYhigh();
   double bins[n+1];
   //bins[0] = x[0]>=0. ? 0. : x[0]*1.2;
   for (int i=0; i<n; i++) bins[i] = x[i]-exl[i];
   bins[n] = x[n-1]+exh[n-1];
   TH1F *ans = new TH1F(Form("tmp%i",gRandom->Integer(1e9)),"tmp",n,bins);
   for (int i=0; i<n; i++) {
      ans->SetBinContent(i+1,y[i]);
      ans->SetBinError(i+1,(eyl[i]+eyh[i])/2);
   }
   ans->SetLineColor(g->GetLineColor());
   ans->SetMarkerColor(g->GetMarkerColor());
   ans->SetMarkerStyle(g->GetMarkerStyle());
   ans->GetYaxis()->SetTitle("Efficiency");
   ans->GetXaxis()->SetTitle(varName.c_str());
   return ans;
};

void setTRatioPlotStyle(TRatioPlot *tr) {
   gStyle->SetOptStat(0);
   tr->SetH1DrawOpt("");
   tr->SetH2DrawOpt("");
   tr->Draw();
   tr->GetLowerRefXaxis()->SetTitleOffset(0.9);
   tr->SetLowBottomMargin(0.4);
   tr->SetRightMargin(0.05);
   tr->GetUpperRefYaxis()->SetRangeUser(0,1.1);
   //tr->GetUpperRefXaxis()->SetRangeUser(-2.5,2.5);
   //tr->GetLowerRefYaxis()->SetRangeUser(0.,1.11);
   //tr->GetLowerRefXaxis()->SetRangeUser(-2.5,2.5);
   //tr->GetLowerRefYaxis()->SetNdivisions(503,kFALSE);
   tr->GetUpperRefYaxis()->SetTitle("Efficiency");
   tr->GetLowerRefYaxis()->SetTitle("Data/MC");
   tr->GetUpperRefYaxis()->SetTitleOffset(0.9);
   tr->GetLowerRefYaxis()->SetTitleOffset(0.9);
   tr->RangeAxisChanged();
};
#endif // #ifndef helper_h
