/////////////  Check 1 file, otherwise same

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>

#include "TSystem.h"
#include "TTree.h"
#include "TKey.h"
#include "TH1.h"
#include "TH2.h"
#include "TPave.h"
#include "TText.h"

#include "TROOT.h"
#include "TFile.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TF1.h"

#include "RooFit.h"
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooArgSet.h"
#include "RooFitResult.h"

#include "TStyle.h"
#include "TLatex.h"
#include "TDirectory.h"
#include "TCollection.h"
#include "TPostScript.h"
#include "TMath.h"

#include "toyStudy_syst.h"

using namespace RooFit;
using namespace std;

/////////////////////////////////////////////////
//              P A R A M E T E R S            //
/////////////////////////////////////////////////

// Choose the efficiency type.
// Possible values: MUIDTRG, TRK, STA, MUID, TRG
#define Marek_Tot

// pp or PbPb?
bool isPbPb = false; // if true, will compute the centrality dependence
TString collTag = "pp"; // isPbPb ? "PbPb" : "pp";

// do the toy study for the correction factors? (only applies if MUIDTRG)
bool doToys = false;

//Whether to use asymetric errors in plotting the efficiencies and scale factors
bool asymErrs = false;

// how to fit efficiencies?
// 0 = [0]*Erf((x-[1])/[2])
// 1 = [0]*Erf((x-[1])/[2]) + [3]
// 2 = ([0]*Erf((x-[1])/[2]))*Exp(x/[4])+ [3]
// 3 = [0]
int fitfcn = 2;

// Location of the files
const int nSyst = 4;//4;
// the first file is for the nominal case, the following ones are for the systematics
/*const char* fDataName[nSyst] = {
	//"tnp_Ana_RD_PbPb_MuonIDTrg_AllMB.root",
	"tnp_Ana_RD_PbPb_MuonTrk_AllMB_isGlbPol2.root", 
};
const char* fMCName[nSyst] = {
   //"tnp_Ana_MC_PbPb_MuonIDTrg_AllMB.root",
	"tnp_Ana_MC_PbPb_MuonTrk_AllMB_isGlbPol2.root",
};// */

// names for systematics
const char* systName[nSyst] = {
  "nominal (3Gauss+pol2)","Background_pol3","Signal_Voigt+Gauss","MassRange_narrow"//,"LooseAcceptance"
};
//GLB:    "nominal (2Gauss+pol2)","Background_pol3","Signal_CB+Gauss","MassRange_narrow","LooseAcceptance"
//MUID:  "nominal (CB+Gauss+pol1)","Background_pol2","Signal_CB","MassRange_narrow","TightAcceptance"
//MUIDTRG:   "nominal (CB+Gauss+pol1)","Background_pol2","Signal_CB","MassRange_narrow"
//TRK:  "nominal (3Gauss+pol2)","Background_pol3","Signal_Voigt+Gauss","MassRange_narrow"//,"LooseAcceptance"

const double rsystrange = 0.02;
//GLB:0.02
//MUID:0.0035
//MUIDTRG:0.018
//TRK: 0.02

//////////////////////////////////////////////////////////////////////////

// Other parameters
#ifdef Marek_Tot
TString etaTag("MuIdTrg_etadep");
TString absetaTag("MuIdTrg_absetadep_HighPt");
TString centTag("MuIdTrg_centdep");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = { "Tot_pTdep_abseta00_08", "Tot_pTdep_abseta08_16",  "Tot_pTdep_abseta16_24" };
TString allTag("MuIdTrg_1bin");
TString absetaVar("abseta");
TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_MuIdTrg.txt");
ofstream file_Cent("CentValues_MuIdTrg.txt");
TString cutTag("tpTreeTrk");
TString cutLegend("HybridSoft ID + DoubleMu0 trigger");
const double effmin = 0.2;
const double sfrange = 0.19;
const char* fDataName[nSyst] = { "./Output/Data_syst/tnp_DATA_1-3_scenario_10_cbGausPlusPol1.root",  "./Output/Data_syst/tnp_DATA_1-3_scenario_10_cbcbPlusPol2.root", "./Output/Data_syst/tnp_DATA_1-3_scenario_10_cbcbPlusPol1_loose.root"};
const char* fMCName[nSyst] = {};
#endif


// Other parameters
#ifdef MUIDTRG
TString etaTag("MuIdTrg_etadep");
TString absetaTag("MuIdTrg_absetadep_HighPt");
TString centTag("MuIdTrg_centdep");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = { "Tot_pTdep_abseta00_08", "Tot_pTdep_abseta08_16",  "Tot_pTdep_abseta16_24" };
TString allTag("MuIdTrg_1bin");
TString absetaVar("abseta");
TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_MuIdTrg.txt");
ofstream file_Cent("CentValues_MuIdTrg.txt");
TString cutTag("tpTreeTrk");
TString cutLegend("HybridSoft ID + DoubleMu0 trigger");
const double effmin = 0.2;
const double sfrange = 0.19;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_HybridSoftIDTrigger_data_pp_CBGPlusPol1.root", "../../tnp_fitOutput_HybridSoftIDTrigger_data_pp_CBGPlusPol2.root", "../../tnp_fitOutput_HybridSoftIDTrigger_data_pp_CBPlusPol1.root", "../../tnp_fitOutput_HybridSoftIDTrigger_data_pp_CBGPlusPol1_narrowMrange.root" };
const char* fMCName[nSyst] = { "../../tnp_fitOutput_HybridSoftIDTrigger_MC_pp_CBGPlusPol1.root", "../../tnp_fitOutput_HybridSoftIDTrigger_MC_pp_CBGPlusPol2.root", "../../tnp_fitOutput_HybridSoftIDTrigger_MC_pp_CBPlusPol1.root", "../../tnp_fitOutput_HybridSoftIDTrigger_MC_pp_CBGPlusPol1_narrowMrange.root" };
#endif

#ifdef MUIDTRGfromTM
TString etaTag("MuIdTrg_etadep");
TString absetaTag("MuIdTrg_absetadep");
TString centTag("MuIdTrg_centdep");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = { "MuIdTrg_abseta00_12", "MuIdTrg_abseta12_18", "MuIdTrg_abseta18_21", "MuIdTrg_abseta21_24" };
TString allTag("MuIdTrg_1bin");
TString absetaVar("abseta");
TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_MuIdTrg.txt");
ofstream file_Cent("CentValues_MuIdTrg.txt");
TString cutTag("tpTree");
TString cutLegend("Soft ID + DoubleMu0 trigger, for tracker muons");
const double effmin = 0.;
const double sfrange = 0.3;
const char* fDataName[nSyst] = {  "../../tnp_fitOutput_MuonIDTrigger_fromTrkMuon_data_pp_pol1.root" };
const char* fMCName[nSyst] = { "../../tnp_fitOutput_MuonIDTrigger_fromTrkMuon_MC_pp_pol1.root" };
#endif

#ifdef MUID
TString etaTag("MuId_etadep");
TString absetaTag("MuId_absetadep_HighPt");
TString centTag("MuId_centdep");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = {"MuId_abseta00_12", "MuId_abseta12_18", "MuId_abseta18_21", "MuId_abseta21_24" };
TString allTag("MuId_1bin");
TString absetaVar("abseta");
TString centVar("tag_hiNtracks");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_MuId.txt");
ofstream file_Cent("CentValues_MuId.txt");
ofstream file_TestErr("MuId_ExpErr.txt");
TString cutTag("tpTree");
TString cutLegend("HybridSoft ID");
const double effmin = 0.85;
const double sfrange = 0.013;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_HybridSoftID_data_pp_CBGPlusPol1.root", "../../tnp_fitOutput_HybridSoftID_data_pp_CBGPlusPol2.root", "../../tnp_fitOutput_HybridSoftID_data_pp_CBPlusPol1.root", "../../tnp_fitOutput_HybridSoftID_data_pp_CBGPlusPol1_narrowMrange.root" };// , "../../tnp_fitOutput_HybridSoftID_data_pp_TightAcceptance_CBGPlusPol1.root"
const char* fMCName[nSyst] = { "../../tnp_fitOutput_HybridSoftID_MC_pp_CBGPlusPol1.root", "../../tnp_fitOutput_HybridSoftID_MC_pp_CBGPlusPol2.root", "../../tnp_fitOutput_HybridSoftID_MC_pp_CBPlusPol1.root", "../../tnp_fitOutput_HybridSoftID_MC_pp_CBGPlusPol1_narrowMrange.root" } ;//"../../tnp_fitOutput_HybridSoftID_MC_pp_TightAcceptance_CBGPlusPol1.root"
#endif

#ifdef TRG
TString etaTag("Trg_etadep");
TString absetaTag("Trg_absetadep");
TString centTag("Trg_centdep");
const int nAbsEtaBins = 5;
TString ptTag[nAbsEtaBins] = { "Trg_abseta00_09", "Trg_abseta09_12", "Trg_abseta12_18", "Trg_abseta18_21", "Trg_abseta21_24" };
TString allTag("Trg_1bin");
TString absetaVar("abseta");
TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_Trg.txt");
ofstream file_Cent("CentValues_Trg.txt");
ofstream file_TestErr("Trg_ExpErr.txt");
TString cutTag("tpTree");
TString cutLegend("DoubleMu0 trigger");
const double effmin = 0.;
const double sfrange = 0.15;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_Trigger_data_pp_CBGPlusPol1_separateBarrel.root" };
const char* fMCName[nSyst] = { "../../tnp_fitOutput_Trigger_MC_pp_CBGPlusPol1_separateBarrel.root" };
#endif

#ifdef STA
TString etaTag("STA_etadep");
TString absetaTag("STA_absetadep");
//TString centTag("STA_cent");
TString HFTag("STA_HFdep");
TString NtracksTag("STA_nTracksdep");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = { "STA_abseta00_12", "STA_abseta12_18", "STA_abseta18_21", "STA_abseta21_24" };
TString allTag("STA_1bin");
TString absetaVar("abseta");
TString HFVar("tag_hiHF");
TString NtracksVar("tag_hiNtracks");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_Sta.txt");
//ofstream file_Cent("CentValues_Sta.txt");T
String cutTag("tpTreeTrk");
TString cutLegend("Standalone");
const double effmin = 0.;
const double sfrange = 0.55;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_STA_data_pp_pol1.root" };
const char* fMCName[nSyst] = { "../../tnp_fitOutput_STA_MC_pp_pol1.root" };
#endif

#ifdef TRK
TString etaTag("Trk_etadep");
TString absetaTag("Trk_absetadep");
//TString centTag("Trk_centSeg");
const int nAbsEtaBins = 6;
TString ptTag[nAbsEtaBins] = { "Trk_pt", "Trk_pt", "Trk_pt", "Trk_pt", "Trk_pt", "Trk_pt",};//,"Trk_ptSeg"
TString allTag("Trk_1bin");
TString absetaVar("abseta");
//TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_Trk.txt");
ofstream file_Cent("CentValues_Trk.txt");
TString cutTag("tpTreeSta");
TString cutLegend("Inner tracking - Global");
const double effmin = 0.8;
const double sfrange = 0.04;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_Tracking_data_pp_triGaussPlusPol2.root", "../../tnp_fitOutput_Tracking_data_pp_triGaussPlusPol3.root","../../tnp_fitOutput_Tracking_data_pp_VoigtGaussPlusPol2.root", "../../tnp_fitOutput_Tracking_data_pp_triGaussPlusPol2_narrowMrange.root"};
const char* fMCName[nSyst] = { "../../tnp_fitOutput_Tracking_MC_pp_triGaussPlusPol2.root", "../../tnp_fitOutput_Tracking_MC_pp_triGaussPlusPol3.root","../../tnp_fitOutput_Tracking_MC_pp_VoigtGaussPlusPol2.root", "../../tnp_fitOutput_Tracking_MC_pp_triGaussPlusPol2_narrowMrange.root"};
#endif

#ifdef TM
TString etaTag("TM_etadep");
TString absetaTag("TM_absetadep");
//TString centTag("TM_centSeg");
const int nAbsEtaBins = 3;
TString ptTag[nAbsEtaBins] = { "TM_abseta00_12", "TM_abseta12_18", "TM_abseta18_24" };
TString allTag("TM_1bin");
TString absetaVar("abseta");
//TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_TM.txt");
ofstream file_Cent("CentValues_TM.txt");
TString cutTag("tpTreeTrk");
TString cutLegend("Tracker muon reconstruction");
const double effmin = 0.5;
const double sfrange = 0.04;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_TM_data_pp_pol2.root" };
const char* fMCName[nSyst] = { "../../tnp_fitOutput_TM_MC_pp_pol2.root" };
#endif

#ifdef GLB
TString etaTag("Glb_etadep");
TString absetaTag("Glb_absetadep");//Glb_absetadep_HighPt
//TString centTag("Glb_centSeg");
const int nAbsEtaBins = 4;
TString ptTag[nAbsEtaBins] = { "Glb_abseta00_12", "Glb_abseta12_18", "Glb_abseta18_24" , "Glb_abseta18_24"};
TString allTag("Glb_1bin");
TString absetaVar("abseta");
//TString centVar("tag_hiBin");
ofstream file_sfs("correction_functions.txt");
ofstream file_Eta("EtaValues_Glb.txt");
ofstream file_Cent("CentValues_Glb.txt");
TString cutTag("tpTreeTrk");
TString cutLegend("Global muon reconstruction");
const double effmin = 0.;
const double sfrange = 0.075;
const char* fDataName[nSyst] = { "../../tnp_fitOutput_Glb_wTrackID_data_pp_twoGaussPlusPol2.root","../../tnp_fitOutput_Glb_wTrackID_data_pp_twoGaussPlusPol3.root","../../tnp_fitOutput_Glb_wTrackID_data_pp_CBGPlusPol2.root","../../tnp_fitOutput_Glb_wTrackID_data_pp_twoGaussPlusPol2_narrowMrange.root"};//,"../../tnp_fitOutput_Glb_LooseAcceptance_data_pp_twoGaussPlusPol2.root" 
const char* fMCName[nSyst] = { "../../tnp_fitOutput_Glb_wTrackID_MC_pp_twoGaussPlusPol2.root","../../tnp_fitOutput_Glb_wTrackID_MC_pp_twoGaussPlusPol3.root","../../tnp_fitOutput_Glb_wTrackID_MC_pp_CBGPlusPol2.root","../../tnp_fitOutput_Glb_wTrackID_MC_pp_twoGaussPlusPol2_narrowMrange.root" };//,"../../tnp_fitOutput_Glb_LooseAcceptance_MC_pp_twoGaussPlusPol2.root"
#endif

// Function Define
TH2F *plotEff2D(RooDataSet *a, TString b);
vector<TGraphAsymmErrors*> plotEff_Nbins(RooDataSet *a, int aa, const char* varx, const char* var2);
TGraphAsymmErrors *plotEff_1bin(RooDataSet *a, int aa, const char* varx, int rebin = 1, double* tntot = NULL, bool asymErrors = true, TString dirName = "", TFile *fIn = NULL);
TGraphAsymmErrors *plotEffCent(RooDataSet **a1, int aa);
void formatTH1F(TH1* a, int b, int c, int d);
void formatTGraph(TGraph* a, int b, int c, int d);
void formatTLeg(TLegend* a);
void CalEffErr(TGraph *a, double *b);
void CalEffErr(vector<TGraphAsymmErrors*> a, double **b);
void plotSysts(TGraphAsymmErrors *graphs[nSyst], TCanvas *c1, TPad *p1, TH1F *h1, TPad *pr, TH1F *hr, TString header, TString tag);
TF1 *initfcn(const char* fname, int ifcn, double ptmin, double ptmax, double effguess);
TF1 *ratiofunc(const char* fname, TF1 *fnum, TF1 *fden);

ofstream file_binnedsfs("correction_binned.txt");

// From here you need to set up your environments.
void TnPEffDraw_singleFile() {

	// gROOT->Macro("~/logon.C");
	gROOT->SetStyle("Plain");
	gStyle->SetOptStat(0);
	gStyle->SetTitle(0);
	gStyle->SetPadLeftMargin(0.11);
	gStyle->SetPadBottomMargin(0.13);
	gStyle->SetTitleYOffset(1.0);


	//data and MC root files as well as single bin for integrated efficiency
	TFile* fMC[nSyst];
	TFile* fData[nSyst];

cout << "point1" << endl;

	for (int i = 0; i < nSyst; i++) {
		fMC[i] = new TFile(fMCName[i]);
		fData[i] = new TFile(fDataName[i]);
	}

	TCanvas *c1 = new TCanvas("c1", "", 700, 600);

	vector<RooDataSet*> daPtData0[nSyst], daPtData1[nSyst];

	for (int k = 0; k < nSyst; k++) {
		for (int i = 0; i < nAbsEtaBins; i++)
		{
			daPtData0[k].push_back((RooDataSet*)fMC[k]->Get(cutTag + "/" + ptTag[i] + "/fit_eff"));
			daPtData1[k].push_back((RooDataSet*)fData[k]->Get(cutTag + "/" + ptTag[i] + "/fit_eff"));

cout <<"Looking for"<<cutTag + "/" + ptTag[i] + "/fit_eff" << endl;
cout << (RooDataSet*)fMC[k]->Get(cutTag + "/" + ptTag[i] + "/fit_eff") << endl;
		}
	}

	vector<TGraphAsymmErrors*> ComPt0[nSyst], ComPt1[nSyst], ComSFPt[nSyst];

	for (int k = 0; k < nSyst; k++) {
cout << "k is " << k << endl;
		for (unsigned int i = 0; i < daPtData0[k].size(); i++)
		{
			cout << k << " " << i << " " << daPtData1[k][i] << endl;
			ComPt0[k].push_back(plotEff_1bin(daPtData0[k][i], 1, "pt", 1,NULL, asymErrs, cutTag + "/" + ptTag[i], fMC[k]));
			cout << k << " " << i << " " << daPtData1[k][i] << endl;
			ComPt1[k].push_back(plotEff_1bin(daPtData1[k][i], 1, "pt", 1,NULL, asymErrs, cutTag + "/" + ptTag[i], fData[k]));
		}
	}
	
	RooDataSet* daEtaData0[nSyst];
	RooDataSet* daEtaData1[nSyst];

cout << "point2" << endl;

	for (int i = 0; i < nSyst; i++) {
		daEtaData0[i] = (RooDataSet*)fMC[i]->Get(cutTag + "/" + etaTag + "/fit_eff");
		daEtaData1[i] = (RooDataSet*)fData[i]->Get(cutTag + "/" + etaTag + "/fit_eff");
	}

	TGraphAsymmErrors* ComEta0[nSyst];
	TGraphAsymmErrors* ComEta1[nSyst];

	for (int i = 0; i < nSyst; i++) {
	  ComEta0[i] = plotEff_1bin(daEtaData0[i], 1, "eta", 1,NULL, asymErrs, cutTag + "/" + etaTag, fMC[i]);
	  ComEta1[i] = plotEff_1bin(daEtaData1[i], 1, "eta", 1,NULL, asymErrs, cutTag + "/" + etaTag, fData[i]);
	}

	RooDataSet* daPtMC1Bin0[nSyst];
	RooDataSet* daPtData1Bin0[nSyst];
	RooDataSet* daAbsEtaMC1[nSyst];
	RooDataSet* daAbsEtaData1[nSyst];
//	RooDataSet* daCentMC1[nSyst];
//	RooDataSet* daCentData1[nSyst];

	for (int i = 0; i < nSyst; i++) {
		daPtMC1Bin0[i] = (RooDataSet*)fMC[i]->Get(cutTag + "/" + allTag + "/fit_eff");
		daPtData1Bin0[i] = (RooDataSet*)fData[i]->Get(cutTag + "/" + allTag + "/fit_eff");
		daAbsEtaMC1[i] = (RooDataSet*)fMC[i]->Get(cutTag + "/" + absetaTag + "/fit_eff");
		cout<<"Looking for "<<cutTag + "/" + absetaTag + "/fit_eff"<<endl; 
		daAbsEtaData1[i] = (RooDataSet*)fData[i]->Get(cutTag + "/" + absetaTag + "/fit_eff");
/*		if (isPbPb) {
			daCentMC1[i] = (RooDataSet*)fMC[i]->Get(cutTag + "/" + centTag + "/fit_eff");
			daCentData1[i] = (RooDataSet*)fData[i]->Get(cutTag + "/" + centTag + "/fit_eff");
		}
// */
	}

cout << "point3" << endl;

	TGraphAsymmErrors* effPtMC[nSyst];
	TGraphAsymmErrors* effPtData[nSyst];
	vector<TGraphAsymmErrors*> effAbsEtaMC[nSyst];
	vector<TGraphAsymmErrors*> effAbsEtaData[nSyst];
//	TGraphAsymmErrors* effCentMC = NULL;
//	TGraphAsymmErrors* effCentData = NULL;
cout << "made tgraphs" << endl;
	for (int k = 0; k < nSyst; k++) {
cout << "entered the for loop" << endl;
                effPtMC[k] = plotEff_1bin(daPtMC1Bin0[k], 0, "eta", 1,NULL, asymErrs, cutTag + "/" + etaTag, fMC[k]);
	 	effPtData[k] = plotEff_1bin(daPtData1Bin0[k], 0, "eta", 1,NULL, asymErrs, cutTag + "/" + etaTag, fData[k]);
cout << "pt was successful" << endl;
		effAbsEtaMC[k] = plotEff_Nbins(daAbsEtaMC1[k], 0, "pt", absetaVar);
		effAbsEtaData[k] = plotEff_Nbins(daAbsEtaData1[k], 0, "pt", absetaVar);
cout << "eta was successful" << endl;
/*		if (isPbPb && k == 0) {
			effCentMC = plotEff_1bin(daCentMC1[k], 0, centVar);
			effCentData = plotEff_1bin(daCentData1[k], 0, centVar);
		}
// */
cout << "k is " << k << endl;
	}

cout << "exited the for loop" << endl;

/*
	if (isPbPb) {
		effCentMC->SetMarkerStyle(20);
		effCentMC->SetMarkerSize(1.4);
		effCentMC->SetMarkerColor(kRed + 1);
		effCentMC->SetLineColor(kRed + 1);
		effCentData->SetMarkerStyle(25);
		effCentData->SetMarkerSize(1.4);
		effCentData->SetMarkerColor(kBlue + 1);
		effCentData->SetLineColor(kBlue + 1);
	}
// */
	int nbins_abseta = ComPt0[0].size();
	for (int k = 0; k < nSyst; k++)
	{
		for (int i = 0; i < nbins_abseta; i++)
		{
			ComPt0[k][i]->SetMarkerStyle(20);
			ComPt0[k][i]->SetMarkerSize(1.4);
			ComPt0[k][i]->SetMarkerColor(kRed + 1);
			ComPt0[k][i]->SetLineColor(kRed + 1);
			ComPt1[k][i]->SetMarkerStyle(25);
			ComPt1[k][i]->SetMarkerSize(1.4);
			ComPt1[k][i]->SetMarkerColor(kBlue + 1);
			ComPt1[k][i]->SetLineColor(kBlue + 1);
		}

		ComEta0[k]->SetMarkerStyle(20);
		ComEta0[k]->SetMarkerSize(1.4);
		ComEta0[k]->SetMarkerColor(kRed + 1);
		ComEta0[k]->SetLineColor(kRed + 1);
		ComEta1[k]->SetMarkerStyle(25);
		ComEta1[k]->SetMarkerSize(1.4);
		ComEta1[k]->SetMarkerColor(kBlue + 1);
		ComEta1[k]->SetLineColor(kBlue + 1);
	}


	// 1 Bin
	double Trk0[nSyst][4];
	double Trk1[nSyst][4];
	double*** TrkAbsEta0 = new double**[nSyst];
	double*** TrkAbsEta1 = new double**[nSyst];

cout << "point4" << endl;

	for (int k = 0; k < nSyst; k++) {
		TrkAbsEta0[k] = new double*[nbins_abseta];
		TrkAbsEta1[k] = new double*[nbins_abseta];

		for (int i = 0; i < nbins_abseta; i++)
		{
			TrkAbsEta0[k][i] = new double[4];
			TrkAbsEta1[k][i] = new double[4];
		}

		CalEffErr(effPtMC[k], Trk0[k]);
		CalEffErr(effPtData[k], Trk1[k]);
		CalEffErr(effAbsEtaMC[k], TrkAbsEta0[k]);

		CalEffErr(effAbsEtaData[k], TrkAbsEta1[k]);
	}

	c1->cd();
	TPad *pad1 = new TPad("pad1", "pad1", 0, 0.26, 1, 1);
	c1->cd();
	TPad *pad2 = new TPad("pad2", "pad2", 0, 0, 1, 0.3*0.96);
	pad1->SetBottomMargin(0.04);
	pad2->SetTopMargin(0);
	pad2->SetFillColor(0);
	pad2->SetFillStyle(0);
	pad2->SetBottomMargin(gStyle->GetPadBottomMargin() / 0.3);
	pad1->SetTopMargin(gStyle->GetPadTopMargin() / 0.7);
	// pad2->SetGridy();
	pad1->Draw();
	pad1->cd();

	// // adapt text size
	// lTextSize *= 1./0.7;


	TH1F *hPad = new TH1F("hPad", ";p^{#mu}_{T} [GeV/c];Single #mu Efficiency", 5, 0, 30);
	TH1F *hPad1 = new TH1F("hPad1", ";#eta^{#mu};Single #mu Efficiency", 5, -2.4, 2.4);
//	TH1F *hPad2 = new TH1F("hPad2", ";Centrality - nTracks ;Single #mu Efficiency", 5, 0, 300);
	hPad->GetXaxis()->CenterTitle();
	hPad1->GetXaxis()->CenterTitle();
//	hPad2->GetXaxis()->CenterTitle();
	hPad->GetXaxis()->SetLabelSize(0.05);
	hPad->GetXaxis()->SetTitleSize(0.05);
	hPad->GetXaxis()->SetTitleOffset(1.2);
	hPad->GetYaxis()->SetLabelSize(0.05);
	hPad->GetYaxis()->SetTitleSize(0.05);
	hPad->GetYaxis()->SetTitleOffset(1.);
	hPad1->GetXaxis()->SetLabelSize(0.05);
	hPad1->GetXaxis()->SetTitleSize(0.05);
	hPad1->GetXaxis()->SetTitleOffset(1.2);
	hPad1->GetYaxis()->SetLabelSize(0.05);
	hPad1->GetYaxis()->SetTitleSize(0.05);
	hPad1->GetYaxis()->SetTitleOffset(1.);
/*	hPad2->GetXaxis()->SetLabelSize(0.);
	hPad2->GetXaxis()->SetTitleSize(0.);
	hPad2->GetXaxis()->SetTitleOffset(1.2);
	hPad2->GetYaxis()->SetLabelSize(0.05);
	hPad2->GetYaxis()->SetTitleSize(0.05);
	hPad2->GetYaxis()->SetTitleOffset(1.);
// */

	hPad->GetYaxis()->SetRangeUser(effmin, 1.05);
	hPad1->GetYaxis()->SetRangeUser(effmin, 1.05);
//	hPad2->GetYaxis()->SetRangeUser(effmin, 1.05);

	pad2->cd();
	pad2->SetGridy();
	double tsize = (1. / 0.36)*hPad->GetYaxis()->GetTitleSize(); // 1./0.36
	TH1F *hPadr = (TH1F*)hPad->Clone("hPadr"); hPadr->GetYaxis()->SetRangeUser(1. - sfrange, 1. + sfrange);
	hPadr->GetYaxis()->SetTitle("Scale Factor");
	hPadr->GetXaxis()->SetTitleSize(tsize);
	hPadr->GetXaxis()->SetLabelSize(tsize);
	hPadr->GetYaxis()->SetTitleSize(tsize);
	hPadr->GetYaxis()->SetLabelSize(tsize);
	hPadr->GetYaxis()->SetNdivisions(504, kTRUE);
	TH1F *hPadr_syst = (TH1F*)hPadr->Clone("hPadr_syst");
	hPadr_syst->GetYaxis()->SetRangeUser(1. - rsystrange, 1. + rsystrange);
	TH1F *hPad_syst = (TH1F*)hPad->Clone("hPad_syst");
	TH1F *hPad_SFsyst = (TH1F*)hPad->Clone("hPad_SFsyst");
	hPad_SFsyst->GetYaxis()->SetRangeUser(1. - sfrange, 1. + sfrange);
	hPad_SFsyst->GetYaxis()->SetTitle("Scale Factor on single #mu efficiency");
	TH1F *hPad1r = (TH1F*)hPad1->Clone("hPad1r"); hPad1r->GetYaxis()->SetRangeUser(1. - sfrange, 1. + sfrange);
	hPad1r->GetYaxis()->SetTitle("Scale Factor");
	hPad1r->GetXaxis()->SetTitleSize(tsize);
	hPad1r->GetXaxis()->SetLabelSize(tsize);
	hPad1r->GetYaxis()->SetTitleSize(tsize);
	hPad1r->GetYaxis()->SetLabelSize(tsize);
	hPad1r->GetYaxis()->SetNdivisions(504, kTRUE);
	TH1F *hPad1r_syst = (TH1F*)hPad1r->Clone("hPad1r_syst");
	hPad1r_syst->GetYaxis()->SetRangeUser(1. - rsystrange, 1. + rsystrange);
	TH1F *hPad1_syst = (TH1F*)hPad1->Clone("hPad1_syst");
	
/*	TH1F *hPad2r = (TH1F*)hPad2->Clone("hPad2r"); hPad2r->GetYaxis()->SetRangeUser(1. - sfrange, 1. + sfrange);
	hPad2r->GetYaxis()->SetTitle("Scale Factor");
	hPad2r->GetXaxis()->SetTitleSize(tsize);
	hPad2r->GetXaxis()->SetLabelSize(tsize);
	hPad2r->GetYaxis()->SetTitleSize(tsize);
	hPad2r->GetYaxis()->SetLabelSize(tsize);
	hPad2r->GetYaxis()->SetNdivisions(504, kTRUE);
// */
	pad1->cd();

cout << "point5" << endl;
 
	//For output 2D graphs of final scale factors with stat+syst errors
	vector<double> eta,etaErr,pt,ptErr,SF,SFstatErr,SFsysErr,SFtotErr;
	int nPtsFinal = 0;

	TString header;
	for (int i = 0; i < nbins_abseta; i++)
	{
		TF1 *fdata = NULL;
		TF1 *fmc = NULL;
		double ptmin, ptmax;
		double etamin, etamax;

		for (int k = 0; k < nSyst; k++)
		{
			pad1->cd();
			hPad->Draw();
			TLatex *lt1 = new TLatex();
			lt1->SetNDC();

			char legs[512];
			TLegend *leg1 = new TLegend(0.25, 0.05, 0.66, 0.43); //(0.43, 0.05, 0.66, 0.43);
			leg1->SetFillStyle(0);
			leg1->SetFillColor(0);
			leg1->SetBorderSize(0);
			leg1->SetTextSize(0.030);
			ptmin = ((RooRealVar*)daPtData0[k][i]->get()->find("pt"))->getBinning().binLow(0);

			if (daPtData0[k][i]->get()->find("abseta"))
			{
				etamin = ((RooRealVar*)daPtData0[k][i]->get()->find("abseta"))->getBinning().binLow(0);
				etamax = ((RooRealVar*)daPtData0[k][i]->get()->find("abseta"))->getBinning().binHigh(0);
				header = TString("#splitline{") + cutLegend + Form(" Efficiency}{(p^{#mu}_{T}>%.1f, |#eta| #in [%.1f, %.1f])}", ptmin, etamin, etamax);
			}
			else
			{
				etamin = ((RooRealVar*)daPtData0[k][i]->get()->find("eta"))->getBinning().binLow(0);
				etamax = ((RooRealVar*)daPtData0[k][i]->get()->find("eta"))->getBinning().binHigh(0);
				header = TString("#splitline{") + cutLegend + Form(" Efficiency}{(p^{#mu}_{T}>%.1f, #eta #in [%.1f, %.1f])}", ptmin, etamin, etamax);
			}
			leg1->SetHeader(header);
			sprintf(legs, "MC PYTHIA: %.4f^{ + %.3f}_{ - %.3f}", TrkAbsEta0[k][i][0], TrkAbsEta0[k][i][1], TrkAbsEta0[k][i][2]);
			leg1->AddEntry(ComPt0[k][i], legs, "pl");
			sprintf(legs, "Data: %.4f^{ + %.3f}_{ - %.3f}", TrkAbsEta1[k][i][0], TrkAbsEta1[k][i][1], TrkAbsEta1[k][i][2]);
			leg1->AddEntry(ComPt1[k][i], legs, "pl");
			leg1->Draw("same");

			ComPt0[k][i]->Draw("pz same");
			ComPt1[k][i]->Draw("pz same");

			lt1->SetTextSize(0.05);
			lt1->DrawLatex(0.43, 0.55, "CMS Preliminary");
			lt1->DrawLatex(0.43, 0.49, collTag + "  #sqrt{s_{NN}} = 5.02 TeV");

			// now take care of the data/mc ratio panel
			c1->cd();
			// pad2->SetFrameFillStyle(4000);
			pad2->Draw();
			pad2->cd();
			hPadr->Draw();

			int nbins_mc = ComPt0[k][i]->GetN();
			int nbins = ComPt1[k][i]->GetN();
			double *xr = new double[nbins];
			double *yr = new double[nbins];
			double *xrlo = new double[nbins];
			double *yrlo = new double[nbins];
			double *xrhi = new double[nbins];
			double *yrhi = new double[nbins];

			TGraphAsymmErrors* ComPt0_forRatio = NULL;
			if (nbins_mc == nbins) {
				ComPt0_forRatio = ComPt0[k][i];
			}
			else {
				double* tntot = new double[nbins_mc];
				TString tag = absetaVar + "_bin0__pt_bin";
				for (int j = 0; j < nbins_mc; j++) {
					RooFitResult *fitres = NULL;
					// Loop on the directories to find the one we are looking for
					TDirectory *tdir = (TDirectory*)fMC[0]->Get(cutTag + "/" + ptTag[i]);
					TIter next(tdir->GetListOfKeys()); TObject *obj;
					while ((obj = next())) {
						obj = ((TKey*)obj)->ReadObj();
						if (TString(obj->ClassName()) != "TDirectoryFile") continue;
						if (TString(obj->GetName()).Contains(Form("__pt_bin%i", j))) { tdir = (TDirectory*)obj; break; }
					}

					fitres = (RooFitResult*)tdir->Get("fitresults");
					if (!fitres) cerr << "ERROR I couldn't find the fit results! Expect a crash soon..." << endl;
					tntot[j] = ((RooRealVar*)fitres->floatParsFinal().find("numTot"))->getVal() * (((RooRealVar*)fitres->floatParsFinal().find("fSigAll"))->getVal());
					delete fitres;
				}
				ComPt0_forRatio = plotEff_1bin(daPtData0[k][i], 1, "pt", nbins_mc / nbins, tntot, asymErrs, cutTag + "/" + ptTag[i], fMC[0]);
				delete[] tntot;
			}

cout << "point6" << endl;

			// here we assume that the mc uncertainty is negligible compared to the data one: simply scale everything by the central value.
			for (int j = 0; j < nbins; j++)
			{
				xr[j] = ComPt1[k][i]->GetX()[j];
				xrlo[j] = ComPt1[k][i]->GetErrorXlow(j);
				xrhi[j] = ComPt1[k][i]->GetErrorXhigh(j);
				yr[j] = ComPt1[k][i]->GetY()[j] / ComPt0_forRatio->GetY()[j];
				yrlo[j] = ComPt1[k][i]->GetErrorYlow(j) / ComPt0_forRatio->GetY()[j];
				yrhi[j] = ComPt1[k][i]->GetErrorYhigh(j) / ComPt0_forRatio->GetY()[j];
			}
			TGraphAsymmErrors *gratio = new TGraphAsymmErrors(nbins, xr, yr, xrlo, xrhi, yrlo, yrhi);
			gratio->SetMarkerStyle(20);
			gratio->SetMarkerColor(kBlack);
			gratio->SetMarkerSize(1.0);
			gratio->SetLineColor(kBlack);
			gratio->SetLineWidth(1);
			gratio->Draw("pz same");

			ComSFPt[k].push_back(gratio);

			// save (nominal only)
			if (k == 0) {
				c1->SaveAs(cutTag + Form("Eff%i_", i) + collTag + "_RD_MC_PT.root");
				c1->SaveAs(cutTag + Form("Eff%i_", i) + collTag + "_RD_MC_PT.pdf");
				c1->SaveAs(cutTag + Form("Eff%i_", i) + collTag + "_RD_MC_PT.png");
			}

cout << "point7" << endl;

			// in case we are looking at muon Id + trigger: get the scale factor at the same time
#if defined MUIDTRG || defined STA || defined MUID || defined TRG || defined MUIDTRGfromTM || defined TM || defined GLB || defined TRK
			pad1->cd();
			ptmax = ((RooRealVar*)daPtData0[k][i]->get()->find("pt"))->getMax();
			TLatex tchi; tchi.SetNDC();
			tchi.SetTextSize(0.035);
			double chi2, pval; int dof;

			if (k == 0) {
				// fit data
				fdata = initfcn("fdata", fitfcn, ptmin, ptmax, 0.8);// ComPt1[k][i]->GetX()[ComPt1[k][i]->GetN() - 1]);
				fdata->SetLineWidth(2);
				fdata->SetLineColor(kBlue);
				/*ComPt1[k][i]->Fit(fdata, "RME");
				leg1->AddEntry(fdata, formula(fdata), "pl");

				chi2 = ComPt1[k][i]->Chisquare(fdata);
				dof = ComPt1[k][i]->GetN() - fdata->GetNpar();
				pval = TMath::Prob(chi2, dof);
				tchi.SetTextColor(kBlue);
				tchi.DrawLatex(0.6, 0.92, Form("#chi^{2}/dof = %.1f/%d (p-value: %.2f)", chi2, dof, pval));*/

				// in the case of the exponential fall at high pt, do the fit first without it
				if (fitfcn == 2) {
					fdata->FixParameter(4, 0);
					ComPt1[k][i]->Fit(fdata, "RME");
					fdata->SetParLimits(4, -1.5, 0);
				}
				ComPt1[k][i]->Fit(fdata, "WRME");

				// fit mc
				//fmc = (TF1*)fdata->Clone("fmc");
				fmc = initfcn("fmc", fitfcn, ptmin, ptmax, 0.8);
				// Initialize the normalization to the efficiency in the last point
				//if (isPbPb) fmc->SetParameters(ComPt0[k][i]->GetX()[ComPt0[k][i]->GetN() - 1], 0.5, 2.5);
				//else fmc->SetParameters(ComPt0[k][i]->GetX()[ComPt0[k][i]->GetN() - 1], 2.2, 1.5);
				fmc->SetLineColor(kRed);
				ComPt0[k][i]->Fit(fmc, "WRME");
				//ComPt0[k][i]->Fit(fmc, "RME");



				//Errors on exponential
				//file_TestErr << "RD value: " << fdata->GetParameter(4) << endl;
				//file_TestErr << "RD error: " << fdata->GetParError(4) << endl;
				//file_TestErr << "RD Sigma away from 0: " << 0-(fdata->GetParameter(4) / fdata->GetParError(4)) << endl<<endl;

				//file_TestErr << "MC value: " << fmc->GetParameter(4)<<endl;
				//file_TestErr << "MC error: " << fmc->GetParError(4) << endl;
				//file_TestErr << "MC Sigma away from 0: " << 0-(fmc->GetParameter(4)/fmc->GetParError(4)) << endl<<endl<<endl;

				//draw


				leg1->AddEntry(fmc, formula(fmc,5), "pl");
				leg1->AddEntry(fdata, formula(fdata,5), "pl");

				chi2 = ComPt1[k][i]->Chisquare(fdata);
				dof = ComPt1[k][i]->GetN() - fdata->GetNpar();
				pval = TMath::Prob(chi2, dof);
				tchi.SetTextColor(kBlue);
				tchi.DrawLatex(0.6, 0.92, Form("#chi^{2}/dof = %.1f/%d (p-value: %.2f)", chi2, dof, pval));


				chi2 = ComPt0[k][i]->Chisquare(fmc);
				dof = ComPt0[k][i]->GetN() - fmc->GetNpar();
				pval = TMath::Prob(chi2, dof);
				tchi.SetTextColor(kRed);
				tchi.DrawLatex(0.6, 0.88, Form("#chi^{2}/dof = %.1f/%d (p-value: %.2f)", chi2, dof, pval));

				leg1->Draw();

				// now the bottom panel
				pad2->cd();
				// hPadr->Draw();
				TF1 *fratio = ratiofunc("fratio", fdata, fmc);
				fratio->Draw("same");

				chi2 = gratio->Chisquare(fratio);
				dof = gratio->GetN() - fratio->GetNpar() / 2;
				pval = TMath::Prob(chi2, dof);
				tchi.SetTextColor(kBlack);
				tchi.SetTextSize(0.035*0.7 / 0.3);
				tchi.DrawLatex(0.6, 0.8, Form("#chi^{2}/dof = %.1f/%d (p-value: %.2f)", chi2, dof, pval));

				// save (nominal only)
				c1->SaveAs(cutTag + Form("SF%i_", i) + collTag + "_RD_MC_PT.root");
				c1->SaveAs(cutTag + Form("SF%i_", i) + collTag + "_RD_MC_PT.pdf");
				c1->SaveAs(cutTag + Form("SF%i_", i) + collTag + "_RD_MC_PT.png");

				// print the fit results to file
				file_sfs << "Data " << etamin << " " << etamax << endl;
				file_sfs << formula(fdata, 5) << endl;
				file_sfs << "MC " << etamin << " " << etamax << endl;
				file_sfs << formula(fmc, 5) << endl;
				file_sfs << endl;

				// // print the binned ratio to the other file
				// file_binnedsfs << "// " << etamin << " < |eta| < " << etamax << endl;
				// for (int i = 0; i<gratio->GetN(); i++) {
				// 	if (i>0) file_binnedsfs << "else ";
				// 	file_binnedsfs << "if (pt<" << gratio->GetX()[i] + gratio->GetEXhigh()[i] << ") return " << gratio->GetY()[i] << ";" << endl;
				// }
				// file_binnedsfs << endl;
			}
		}

		//Calculate the systematic error from the maximal variation in each Pt bin
		int nPtBins = ComSFPt[0][i]->GetN();
		double MaxVar[nPtBins];
		for (int l = 0; l<nPtBins; l++) {
		  MaxVar[l] = 0;
		  for (int k = 1; k < nSyst; k++)
		    {
		      if(l<ComSFPt[k][i]->GetN())
			{
			  double var = fabs(ComSFPt[k][i]->GetY()[l] - ComSFPt[0][i]->GetY()[l]);
			  if(var>MaxVar[l]) MaxVar[l] = var;
			}
		    }
		  cout<<"Systematic error on SF for Pt bin #"<<l<<" = "<< MaxVar[l] <<endl;
		}

		//Print the binned ratio, and its stat,syst, and tot errors, and record everything for the final output TGraph2DErrors
		file_binnedsfs << "//Central scale-factor value, statistical error, systematic error, total error" << endl;
		file_binnedsfs << "if( (fabs(eta) > " << etamin << ") && (fabs(eta) < " << etamax << ") ){" << endl;
		double etaval = (etamax+etamin)/2;
		double etaerr = (etamax-etamin)/2;
		for (int l = 0; l<nPtBins; l++) {
		  eta.push_back(etaval); etaErr.push_back(etaerr);
		  pt.push_back(ComSFPt[0][i]->GetX()[l] + (ComSFPt[0][i]->GetEXhigh()[l] - ComSFPt[0][i]->GetEXlow()[l])/2);//force value to be at the center of the bin (symmetric errors)
		  ptErr.push_back((ComSFPt[0][i]->GetEXhigh()[l] + ComSFPt[0][i]->GetEXlow()[l])/2);

		  SF.push_back(ComSFPt[0][i]->GetY()[l]);
		  SFstatErr.push_back(ComSFPt[0][i]->GetEYhigh()[l]);
		  SFsysErr.push_back(MaxVar[l]);
		  SFtotErr.push_back(sqrt(SFsysErr[nPtsFinal]*SFsysErr[nPtsFinal] + SFstatErr[nPtsFinal]*SFstatErr[nPtsFinal]));

		  if (l>0) file_binnedsfs << "else ";
		  file_binnedsfs << "if (pt<" << ComSFPt[0][i]->GetX()[l] + ComSFPt[0][i]->GetEXhigh()[l] << ") return std::make_tuple(" << SF[nPtsFinal] <<", "<< SFstatErr[nPtsFinal] <<", "<< SFsysErr[nPtsFinal]<<", "<<SFtotErr[nPtsFinal] << ");" << endl;
		  nPtsFinal += 1;
		}
		file_binnedsfs << "}" << endl;;

cout << "point8-endofmaincode" << endl;

		// plot systematics
		TGraphAsymmErrors *graphssyst_data[nSyst];
		//data
		for (int k = 0; k < nSyst; k++) graphssyst_data[k] = ComPt1[k][i];
		plotSysts(graphssyst_data, c1, pad1, hPad_syst, pad2, hPadr_syst, header, Form("syst_data_pt_%i", i));
		//mc
		TGraphAsymmErrors *graphssyst_mc[nSyst];
		for (int k = 0; k < nSyst; k++) graphssyst_mc[k] = ComPt0[k][i];
		plotSysts(graphssyst_mc, c1, pad1, hPad_syst, pad2, hPadr_syst, header, Form("syst_mc_pt_%i", i));
		//Scale factors
		TGraphAsymmErrors *graphssyst_sf[nSyst];
		for (int k = 0; k < nSyst; k++) graphssyst_sf[k] = ComSFPt[k][i];
		plotSysts(graphssyst_sf, c1, pad1, hPad_SFsyst, pad2, hPadr_syst, header, Form("syst_SF_pt_%i", i));

		// toys study 
		if (doToys) toyStudy(nSyst, graphssyst_data, graphssyst_mc, fdata, fmc, cutTag + Form("toys%i_", i) + collTag + "_RD_MC_PT", 0);
#else
	}
#endif // ifdef MUIDTRG or STA
	}

//Plot the 2D graph final output of scale factors
if(nPtsFinal>0)
  {
    TFile *fout =new TFile("Scale_factors_graphs.root", "RECREATE");

    map<int,vector<double> > whichErr = { {0,SFstatErr} , {1,SFsysErr} , {2,SFtotErr}};
    map<int,TString > whichErr_str = { {0,"statistical"} , {1,"systematical"} , {2,"total"}};
    for(int e=0;e<3;e++){
      TGraph2DErrors *g_SF = new TGraph2DErrors(nPtsFinal, eta.data(), pt.data(), SF.data(), etaErr.data(), ptErr.data(), whichErr[e].data());
      g_SF->SetName("Scale_factors_with_"+whichErr_str[e]+"_errors");
      g_SF->SetTitle("Scale factors with "+whichErr_str[e]+" errors;|#eta|;p_{T};SF");
      g_SF->SetMarkerSize(1);
      g_SF->Write();
    }
    fout->Close();
  }

cout << "point9" << endl;

	//---------- This is for eta dependence
	TLegend *leg1 = new TLegend(0.43, 0.11, 0.66, 0.33);
	TLatex *lt1 = new TLatex();
	for (int k = 0; k < nSyst; k++)
	{
		pad1->cd();
		hPad1->Draw();

		ComEta0[k]->Draw("pz same");
		ComEta1[k]->Draw("pz same");

		lt1->SetNDC();
		char legs[512];
		leg1->SetFillStyle(0);
		leg1->SetFillColor(0);
		leg1->SetBorderSize(0);
		leg1->SetTextSize(0.035);
		double ptmin = ((RooRealVar*)daEtaData0[k]->get()->find("pt"))->getBinning().binLow(0);
		leg1->SetHeader(TString("#splitline{") + cutLegend + Form(" Efficiency}{(p^{#mu}_{T}>%.1f)}", ptmin));
		sprintf(legs, "MC PYTHIA: %.4f^{ + %.3f}_{ - %.3f}", Trk0[k][0], Trk0[k][1], Trk0[k][2]);
		leg1->AddEntry(ComPt0[k][0], legs, "pl");
		sprintf(legs, "Data: %.4f^{ + %.3f}_{ - %.3f}", Trk1[k][0], Trk1[k][1], Trk1[k][2]);
		leg1->AddEntry(ComPt1[k][0], legs, "pl");
		leg1->Draw("same");
		leg1->Draw("same");

		lt1->SetTextSize(0.05);
		lt1->DrawLatex(0.43, 0.50, "CMS Preliminary");
		//lt1->DrawLatex(0.43,0.54,"pp  #sqrt{s} = 5.02 TeV");
		lt1->DrawLatex(0.43, 0.44, collTag + "  #sqrt{s_{NN}} = 5.02 TeV");

		// now take care of the data/mc ratio panel
		c1->cd();
		// pad2->SetFrameFillStyle(4000);
		pad2->Draw();
		pad2->cd();
		hPad1r->Draw();

		int nbins = ComEta0[k]->GetN();
		double *xr = new double[nbins];
		double *yr = new double[nbins];
		double *xrlo = new double[nbins];
		double *yrlo = new double[nbins];
		double *xrhi = new double[nbins];
		double *yrhi = new double[nbins];

		// here we assume that the mc uncertainty is negligible compared to the data one: simply scale everything by the central value.
		for (int j = 0; j < nbins; j++)
		{
			xr[j] = ComEta1[k]->GetX()[j];
			xrlo[j] = ComEta1[k]->GetErrorXlow(j);
			xrhi[j] = ComEta1[k]->GetErrorXhigh(j);
			yr[j] = ComEta1[k]->GetY()[j] / ComEta0[k]->GetY()[j];
			yrlo[j] = ComEta1[k]->GetErrorYlow(j) / ComEta0[k]->GetY()[j];
			yrhi[j] = ComEta1[k]->GetErrorYhigh(j) / ComEta0[k]->GetY()[j];
		}
		TGraphAsymmErrors *gratio1 = new TGraphAsymmErrors(nbins, xr, yr, xrlo, xrhi, yrlo, yrhi);
		gratio1->SetMarkerStyle(20);
		gratio1->SetMarkerColor(kBlack);
		gratio1->SetMarkerSize(1.0);
		gratio1->SetLineColor(kBlack);
		gratio1->SetLineWidth(1);
		gratio1->Draw("pz same");

		if (k == 0) {
			c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Eta.root");
			c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Eta.pdf");
			c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Eta.png");


			// print the eta dependence to file
			double xVal, yVal, ErrDown, ErrUp;
			file_Eta << "Data " << endl << endl;
			file_Eta << "Eta Value ErrDown ErrUp" << endl;
			for (int bin = 0; bin < ComEta1[k]->GetN(); bin++)
			{
				ComEta1[k]->GetPoint(bin, xVal, yVal);
				ErrDown = ComEta1[k]->GetErrorYlow(bin);
				ErrUp = ComEta1[k]->GetErrorYhigh(bin);
				file_Eta << xVal << " " << yVal << " " << ErrDown << " " << ErrUp << endl;
			}

			file_Eta << endl << endl << "MC " << endl << endl;
			file_Eta << "Eta Value ErrDown ErrUp" << endl;
			for (int bin = 0; bin < ComEta0[k]->GetN(); bin++)
			{
				ComEta0[k]->GetPoint(bin, xVal, yVal);
				ErrDown = ComEta0[k]->GetErrorYlow(bin);
				ErrUp = ComEta0[k]->GetErrorYhigh(bin);
				file_Eta << xVal << " " << yVal << " " << ErrDown << " " << ErrUp << endl;
			}
			file_Eta.close();
		}
	}

cout << "point10" << endl;

	// plot systematics
	//data
	plotSysts(ComEta1, c1, pad1, hPad1_syst, pad2, hPad1r_syst, header, "syst_data_eta");
	//mc
	plotSysts(ComEta0, c1, pad1, hPad1_syst, pad2, hPad1r_syst, header, "syst_mc_eta");

	//-------- This is for centrality dependence
/*	if (isPbPb) {
		pad1->cd();
		hPad2->Draw();

		effCentMC->Draw("pz same");
		effCentData->Draw("pz same");

		leg1->Draw("same");

		lt1->SetTextSize(0.05);
		lt1->DrawLatex(0.43, 0.50, "CMS Preliminary");
		//lt1->DrawLatex(0.43,0.54,"pp  #sqrt{s} = 5.02 TeV");
		lt1->DrawLatex(0.43, 0.44, collTag + "  #sqrt{s_{NN}} = 5.02 TeV");

		// now take care of the data/mc ratio panel
		c1->cd();
		// pad2->SetFrameFillStyle(4000);
		pad2->Draw();
		pad2->cd();
		hPad2r->Draw();

		int nbins2 = effCentMC->GetN();
		double* xr2 = new double[nbins2];
		double* yr2 = new double[nbins2];
		double* xr2lo = new double[nbins2];
		double* yr2lo = new double[nbins2];
		double* xr2hi = new double[nbins2];
		double* yr2hi = new double[nbins2];

		// here we assume that the mc uncertainty is negligible compared to the data one: simply scale everything by the central value.
		for (int j = 0; j < nbins2; j++)
		{
			xr2[j] = effCentData->GetX()[j];
			xr2lo[j] = effCentData->GetErrorXlow(j);
			xr2hi[j] = effCentData->GetErrorXhigh(j);
			yr2[j] = effCentData->GetY()[j] / effCentMC->GetY()[j];
			yr2lo[j] = effCentData->GetErrorYlow(j) / effCentMC->GetY()[j];
			yr2hi[j] = effCentData->GetErrorYhigh(j) / effCentMC->GetY()[j];
		}
		TGraphAsymmErrors *gratio2 = new TGraphAsymmErrors(nbins2, xr2, yr2, xr2lo, xr2hi, yr2lo, yr2hi);
		gratio2->SetMarkerStyle(20);
		gratio2->SetMarkerColor(kBlack);
		gratio2->SetMarkerSize(1.0);
		gratio2->SetLineColor(kBlack);
		gratio2->SetLineWidth(1);
		gratio2->Draw("pz same");

		c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Cent.root");
		c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Cent.pdf");
		c1->SaveAs(cutTag + "Eff_" + collTag + "_RD_MC_Cent.png");

cout << "point11" << endl;

		// print the centrality dependence to file
		double xVal, yVal, ErrDown, ErrUp;
		file_Cent << "Data " << endl << endl;
		file_Cent << "Centrality Value ErrDown ErrUp" << endl;
		for (int bin = 0; bin < effCentData->GetN(); bin++)
		{
			effCentData->GetPoint(bin, xVal, yVal);
			ErrDown = effCentData->GetErrorYlow(bin);
			ErrUp = effCentData->GetErrorYhigh(bin);
			file_Cent << xVal << " " << yVal << " " << ErrDown << " " << ErrUp << endl;
		}

		file_Cent << endl << endl << "MC " << endl << endl;
		file_Cent << "Centrality Value ErrDown ErrUp" << endl;
		for (int bin = 0; bin < effCentMC->GetN(); bin++)
		{
			effCentMC->GetPoint(bin, xVal, yVal);
			ErrDown = effCentMC->GetErrorYlow(bin);
			ErrUp = effCentMC->GetErrorYhigh(bin);
			file_Cent << xVal << " " << yVal << " " << ErrDown << " " << ErrUp << endl;
		}
		file_Cent.close();
	}
// */

	file_sfs.close();
	file_binnedsfs.close();

cout << "point12-endofeverything" << endl;

}

void formatTH1F(TH1* a, int b, int c, int d) {
	a->SetLineWidth(2);
	a->SetLineStyle(c);
	a->SetMarkerSize(2);
	a->SetLineColor(b);
	a->SetMarkerColor(b);
	a->GetYaxis()->SetTitle("Efficiency");
	if (d == 1) {
		a->GetXaxis()->SetTitle("p_{T} [GeV/c]");
	}
	else if (d == 2) {
		a->GetXaxis()->SetTitle("#eta");
	}
	else if (d == 3) {
		a->GetXaxis()->SetTitle("rapidity");
	}
	else if (d == 4) {
		a->GetXaxis()->SetTitle("Centrality");
	}
	a->GetXaxis()->CenterTitle(true);
	a->GetXaxis()->SetLabelSize(0.05);
	a->GetXaxis()->SetTitleSize(0.05);
	a->GetXaxis()->SetTitleOffset(0.9);
	a->GetYaxis()->SetLabelSize(0.05);
	a->GetYaxis()->SetTitleSize(0.05);
	a->GetYaxis()->SetTitleOffset(0.8);

}

void formatTLeg(TLegend* a) {

	a->SetFillStyle(0);
	a->SetFillColor(0);
	a->SetBorderSize(0);
	a->SetTextSize(0.03);

}


void formatTGraph(TGraph* a, int b, int c, int d)
{

	a->SetMarkerStyle(c);
	a->SetMarkerColor(b);
	a->SetMarkerSize(1.0);
	a->SetLineColor(b);
	a->SetLineWidth(1);
	a->GetXaxis()->SetLabelSize(0.05);
	a->GetXaxis()->SetTitleSize(0.06);
	a->GetXaxis()->SetTitleOffset(0.9);
	a->GetYaxis()->SetTitle("Efficiency");
	a->GetXaxis()->CenterTitle();
	if (d == 1) {
		a->GetXaxis()->SetTitle("p_{T} (GeV/c)");
	}
	else if (d == 2) {
		a->GetXaxis()->SetTitle("eta");
	}
	else if (d == 3) {
		a->GetXaxis()->SetTitle("rapidity");
	}
	else if (d == 4) {
		a->GetXaxis()->SetTitle("Centrality");
	}
	a->GetYaxis()->SetRangeUser(0, 1);
	a->GetXaxis()->SetRangeUser(0, 10);
	a->GetYaxis()->SetLabelSize(0.05);
	a->GetYaxis()->SetTitleSize(0.05);
	a->GetYaxis()->SetTitleOffset(0.9);


}

TGraphAsymmErrors *plotEff_1bin(RooDataSet *a, int aa, const char* varx, int rebin, double *tntot, bool asymErrors, TString dirName, TFile *fIn) {
	const RooArgSet *set = a->get();
	RooRealVar *xAx = (RooRealVar*)set->find(varx);
	RooRealVar *eff = (RooRealVar*)set->find("efficiency");

	int nbins = xAx->getBinning().numBins();

	double tx[nbins], txhi[nbins], txlo[nbins];
	double ty[nbins], tyhi[nbins], tylo[nbins];

	for (int i = 0; i < nbins; i++) {
		a->get(i);
		ty[i] = eff->getVal();
		tx[i] = xAx->getVal();
		txhi[i] = fabs(xAx->getErrorHi());
		txlo[i] = fabs(xAx->getErrorLo());
		tyhi[i] = fabs(eff->getErrorHi());
		tylo[i] = fabs(eff->getErrorLo());
	}

	//Looking for the symmetric errors in the fit result
	if(!asymErrors && dirName!="" && fIn!=NULL){
	  for (int i = 0; i < nbins; i++) {

	    RooFitResult *fitres = NULL;
	    // Loop on the directories to find the one we are looking for
	    TDirectory *tdir = (TDirectory*)fIn->Get(dirName);
	    TIter next(tdir->GetListOfKeys()); TObject *obj;
	    while ((obj = next())) {
	      obj = ((TKey*)obj)->ReadObj();
	      if (TString(obj->ClassName()) != "TDirectoryFile") continue;
	      if (TString(obj->GetName()).Contains(Form(((TString)varx=="pt")?"__pt_bin%i":"eta_bin%i__", i))) { tdir = (TDirectory*)obj; break; }
	    }

	    fitres = (RooFitResult*)tdir->Get("fitresults");
	    if (!fitres) cerr << "ERROR I couldn't find the fit results! Expect a crash soon..." << endl;
	    RooRealVar* eff_fitresult = (RooRealVar*) ((fitres==NULL)?NULL:(fitres->floatParsFinal().find("efficiency")));
	    delete fitres;

	    // TString str1 = (TString)((varx=="pt")?( "abseta_bin0__pt_bin" + (TString)to_string(i) ):( "eta_bin"+(TString)to_string(i)+"__pt_bin0"  ));
	    // RooFitResult* fitresult = (RooFitResult*) file->Get( cutTag + "/" + ptTag[i] +"/"+ str1 + fitresStr + "/fitresults");
	    if(eff_fitresult!=NULL){
	      tyhi[i] = eff_fitresult->getError();
	      tylo[i] = eff_fitresult->getError();
	    }
	  }
	}

	// do the rebinning here if necessary
	if (rebin > 1)
	{
		if (nbins % rebin != 0) cout << "ERROR YOU ARE TRYING TO REBIN " << nbins << " BY " << rebin << " HOW IS THIS SUPPOSED TO WORK ???" << endl;

		nbins = nbins / rebin;
		for (int i = 0; i < nbins; i++) {
			double ntot1 = tntot[2 * i];
			double ntot2 = tntot[2 * i + 1];
			ty[i] = (ty[2 * i] * ntot1 + ty[2 * i + 1] * ntot2) / (ntot1 + ntot2);
			tx[i] = (tx[2 * i] * ntot1 + tx[2 * i + 1] * ntot2) / (ntot1 + ntot2);
			cout << "..." << ntot1 << " " << tx[2 * i] << " " << ntot2 << " " << tx[2 * i + 1] << " " << tx[i] << endl;
			txhi[i] = (txhi[2 * i] * ntot1 + txhi[2 * i + 1] * ntot2) / (ntot1 + ntot2);
			txlo[i] = (txlo[2 * i] * ntot1 + txlo[2 * i + 1] * ntot2) / (ntot1 + ntot2);
			tyhi[i] = (tyhi[2 * i] * ntot1 + tyhi[2 * i + 1] * ntot2) / (ntot1 + ntot2);
			tylo[i] = (tylo[2 * i] * ntot1 + tylo[2 * i + 1] * ntot2) / (ntot1 + ntot2);
		}
	}


	cout << "NBins : " << nbins << endl;

	const double *x = tx;
	const double *xhi = txhi;
	const double *xlo = txlo;
	const double *y = ty;
	const double *yhi = tyhi;
	const double *ylo = tylo;


	TGraphAsymmErrors *b = new TGraphAsymmErrors();
	if (aa == 1) { *b = TGraphAsymmErrors(nbins, x, y, xlo, xhi, ylo, yhi); }
	if (aa == 0) { *b = TGraphAsymmErrors(nbins, x, y, 0, 0, ylo, yhi); }
	b->SetMaximum(1.1);
	b->SetMinimum(0.0);
	b->SetMarkerStyle(20);
	b->SetMarkerColor(kRed + 2);
	b->SetMarkerSize(1.0);
	b->SetTitle("");
	b->GetXaxis()->SetTitleSize(0.1);
	b->GetYaxis()->SetTitleSize(0.1);
	b->GetXaxis()->SetTitle("#eta");
	b->GetYaxis()->SetTitle("Efficiency");
	b->GetXaxis()->CenterTitle();
	//b->Draw("apz");

	for (int i = 0; i < nbins; i++) {
		cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
	}

	return b;

}

vector<TGraphAsymmErrors*> plotEff_Nbins(RooDataSet *a, int aa, const char* varx, const char* var2) {
	const RooArgSet *set = a->get();
	RooRealVar *xAx = (RooRealVar*)set->find(varx);
	RooRealVar *abseta = (RooRealVar*)set->find(var2);
	RooRealVar *eff = (RooRealVar*)set->find("efficiency");

	int nbins = xAx->getBinning().numBins();
	const int nbins2 = abseta->getBinning().numBins();
	// cout << nbins << " " << nbins2 << endl;

	double **tx = new double*[nbins2], **txhi = new double*[nbins2], **txlo = new double*[nbins2];
	double **ty = new double*[nbins2], **tyhi = new double*[nbins2], **tylo = new double*[nbins2];
	for (int i = 0; i < nbins2; i++)
	{
		tx[i] = new double[nbins];
		txlo[i] = new double[nbins];
		txhi[i] = new double[nbins];
		ty[i] = new double[nbins];
		tylo[i] = new double[nbins];
		tyhi[i] = new double[nbins];
	}

	for (int i = 0; i < nbins*nbins2; i++) {
		a->get(i);
		int bin1 = xAx->getBinning().binNumber(xAx->getVal());
		int bin2 = abseta->getBinning().binNumber(abseta->getVal());
		ty[bin2][bin1] = eff->getVal();
		tx[bin2][bin1] = xAx->getVal();
		txhi[bin2][bin1] = fabs(xAx->getErrorHi());
		txlo[bin2][bin1] = fabs(xAx->getErrorLo());
		tyhi[bin2][bin1] = fabs(eff->getErrorHi());
		tylo[bin2][bin1] = fabs(eff->getErrorLo());
	}

	// cout<<"NBins pt: "<<nbins<<endl;

	vector<TGraphAsymmErrors*> result;
	for (int i = 0; i < nbins2; i++)
	{
		const double *x = tx[i];
		const double *xhi = txhi[i];
		const double *xlo = txlo[i];
		const double *y = ty[i];
		const double *yhi = tyhi[i];
		const double *ylo = tylo[i];
		TGraphAsymmErrors *b = new TGraphAsymmErrors();
		if (aa == 1) { *b = TGraphAsymmErrors(nbins, x, y, xlo, xhi, ylo, yhi); }
		if (aa == 0) { *b = TGraphAsymmErrors(nbins, x, y, 0, 0, ylo, yhi); }
		b->SetMaximum(1.1);
		b->SetMinimum(0.0);
		b->SetMarkerStyle(20);
		b->SetMarkerColor(kRed + 2);
		b->SetMarkerSize(1.0);
		b->SetTitle("");
		b->GetXaxis()->SetTitleSize(0.05);
		b->GetYaxis()->SetTitleSize(0.05);
		b->GetXaxis()->SetTitle("p_{T} [GeV/c]");
		b->GetYaxis()->SetTitle("Efficiency");
		b->GetXaxis()->CenterTitle();
		//b->Draw("apz");
		result.push_back(b);
	}

	// for (int i=0; i<nbins; i++) {
	//   cout << x[i] << " " << y[i] << " " << yhi[i] << " " << ylo[i] << endl;
	// }

	return result;

}



TH2F *plotEff2D(RooDataSet *a, TString b) {
	const RooArgSet *set = a->get();
	RooRealVar *yAx = (RooRealVar*)set->find("pt");
	RooRealVar *xAx = (RooRealVar*)set->find("eta");
	RooRealVar *eff = (RooRealVar*)set->find("efficiency");

	//const int xnbins = xAx->getBinning().numBins();
	//const int ynbins = yAx->getBinning().numBins();

	const double *xvbins = xAx->getBinning().array();
	const double *yvbins = yAx->getBinning().array();

	TH2F* h = new TH2F(b, "", xAx->getBinning().numBins(), xvbins, yAx->getBinning().numBins(), yvbins);

	gStyle->SetPaintTextFormat("5.2f");
	gStyle->SetPadRightMargin(0.12);
	gStyle->SetPalette(1);
	h->SetOption("colztexte");
	h->GetZaxis()->SetRangeUser(-0.001, 1.001);
	h->SetStats(kFALSE);
	h->GetYaxis()->SetTitle("p_{T} [GeV/c]");
	h->GetXaxis()->SetTitle("#eta");
	h->GetXaxis()->CenterTitle();
	h->GetYaxis()->CenterTitle();
	h->GetXaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleSize(0.05);
	h->GetYaxis()->SetTitleOffset(0.8);
	h->GetXaxis()->SetTitleOffset(0.9);
	for (int i = 0; i < a->numEntries(); i++) {
		a->get(i);
		h->SetBinContent(h->FindBin(xAx->getVal(), yAx->getVal()), eff->getVal());
		h->SetBinError(h->FindBin(xAx->getVal(), yAx->getVal()), (eff->getErrorHi() - eff->getErrorLo()) / 2.);
	}

	return h;

}


void CalEffErr(TGraph *a, double *b) {
	const int nbins = 100;
	double x[nbins], y[nbins];
	double sum = 0, errHighSum = 0, errLowSum = 0, sqSumHigh = 0, sqSumLow = 0;
	//double b[3] = 0;

	int nBins = a->GetN();
	for (int i = 0;i < a->GetN();i++) {
		a->GetPoint(i, x[i], y[i]);
		//cout<<"Eff x = "<<x[i]<<" y = "<<y[i]<<endl;
		double eHigh = a->GetErrorYhigh(i);
		double eLow = a->GetErrorYlow(i);
		//cout<<"Err high = "<<eHigh<<" low = "<<eLow<<endl;
		sum += y[i];
		errHighSum += eHigh;
		sqSumHigh += eHigh*eHigh;
		errLowSum += eLow;
		sqSumLow += eLow*eLow;
	}
	b[0] = sum / nBins;
	b[1] = sqrt(sqSumHigh) / nBins;
	b[2] = sqrt(sqSumLow) / nBins;
	//cout<<"b1 : "<<b[0]<<", b2 : "<<b[1]<<", b3 : "<<b[2]<<endl;

	cout << b[0] << "^{" << b[1] << "}_{" << b[2] << "}" << endl;
	//return b[3];
}

void CalEffErr(vector<TGraphAsymmErrors*> a, double **b) {
	const int nbins = 100;
	const int vsize = a.size();

	for (int vbin = 0; vbin < vsize; vbin++)
	{
	  	double x[nbins], y[nbins];
		double sum = 0, errHighSum = 0, errLowSum = 0, sqSumHigh = 0, sqSumLow = 0;
		//double b[3] = 0;

		int nBins = a[vbin]->GetN();
		for (int i = 0;i < a[vbin]->GetN();i++) {
		        a[vbin]->GetPoint(i, x[i], y[i]);
			//cout<<"Eff x = "<<x[i]<<" y = "<<y[i]<<endl;
			double eHigh = a[vbin]->GetErrorYhigh(i);
			double eLow = a[vbin]->GetErrorYlow(i);
			//cout<<"Err high = "<<eHigh<<" low = "<<eLow<<endl;
			sum += y[i];
			errHighSum += eHigh;
			sqSumHigh += eHigh*eHigh;
			errLowSum += eLow;
			sqSumLow += eLow*eLow;
		}

		b[vbin][0] = sum / nBins;
		b[vbin][1] = sqrt(sqSumHigh) / nBins;
		b[vbin][2] = sqrt(sqSumLow) / nBins;
		//cout<<"b1 : "<<b[0]<<", b2 : "<<b[1]<<", b3 : "<<b[2]<<endl;

		cout << b[vbin][0] << "^{" << b[vbin][1] << "}_{" << b[vbin][2] << "}" << endl;
	}
	//return b[3];
}

void plotSysts(TGraphAsymmErrors *graphs[nSyst], TCanvas *c1, TPad *p1, TH1F *h1, TPad *pr, TH1F *hr, TString header, TString tag) {
	p1->cd();
	h1->Draw();
	TLatex *lt1 = new TLatex();
	lt1->SetNDC();

	char legs[512];
	TLegend *leg1 = new TLegend(0.43, 0.05, 0.66, 0.43);
	leg1->SetFillStyle(0);
	leg1->SetFillColor(0);
	leg1->SetBorderSize(0);
	leg1->SetTextSize(0.035);
	leg1->SetHeader(header);
	for (int k = 0; k < nSyst; k++) {
		graphs[k]->SetMarkerColor(k < 4 ? k + 1 : k + 2);
		graphs[k]->SetMarkerStyle(k + 20);
		graphs[k]->SetLineColor(k < 4 ? k + 1 : k + 2);
		leg1->AddEntry(graphs[k], systName[k], "pl");
		graphs[k]->Draw("pz same");
	}
	leg1->Draw("same");

	// lt1->SetTextSize(0.05);
	// lt1->DrawLatex(0.43,0.60,"CMS Preliminary");
	// //lt1->DrawLatex(0.43,0.54,"pp  #sqrt{s} = 5.02 TeV");
	// lt1->DrawLatex(0.43,0.54,collTag + "  #sqrt{s_{NN}} = 5.02 TeV");

	// now take care of the data/mc ratio panel
	c1->cd();
	// pad2->SetFrameFillStyle(4000);
	pr->Draw();
	pr->cd();
	hr->Draw();

	int nbins_mc = graphs[0]->GetN();
	int nbins = graphs[0]->GetN();
	double **xr = new double*[nSyst];
	double **yr = new double*[nSyst];
	double **xrlo = new double*[nSyst];
	double **yrlo = new double*[nSyst];
	double **xrhi = new double*[nSyst];
	double **yrhi = new double*[nSyst];
	for (int j = 0; j < nSyst; j++) {
		xr[j] = new double[nbins];
		yr[j] = new double[nbins];
		xrlo[j] = new double[nbins];
		yrlo[j] = new double[nbins];
		xrhi[j] = new double[nbins];
		yrhi[j] = new double[nbins];
	}

	TGraphAsymmErrors *gratio[nSyst];

	TGraphAsymmErrors* ComPt0_forRatio = NULL;
	ComPt0_forRatio = graphs[0];

	for (int k = 1; k < nSyst; k++) {
		double maxvar = -1;
		for (int j = 0; j < nbins; j++)
		{
			xr[k][j] = graphs[k]->GetX()[j];
			xrlo[k][j] = graphs[k]->GetErrorXlow(j);
			xrhi[k][j] = graphs[k]->GetErrorXhigh(j);
			yr[k][j] = graphs[k]->GetY()[j] / ComPt0_forRatio->GetY()[j];
			maxvar = max(maxvar, yr[k][j]); maxvar = max(maxvar, 1. / yr[k][j]);
			yrlo[k][j] = graphs[k]->GetErrorYlow(j) / ComPt0_forRatio->GetY()[j];
			yrhi[k][j] = graphs[k]->GetErrorYhigh(j) / ComPt0_forRatio->GetY()[j];
		}
		cout << "MAX (" << tag << ", " << systName[k] << "): " << maxvar << endl;
		gratio[k] = new TGraphAsymmErrors(nbins, xr[k], yr[k], xrlo[k], xrhi[k], yrlo[k], yrhi[k]);
		gratio[k]->SetMarkerStyle(20 + k);
		gratio[k]->SetMarkerColor(k < 4 ? k + 1 : k + 2);
		gratio[k]->SetMarkerSize(1.0);
		gratio[k]->SetLineColor(k < 4 ? k + 1 : k + 2);
		gratio[k]->SetLineWidth(1);
		gratio[k]->Draw("pz same");
	}

	// save
	c1->SaveAs(tag + ".root");
	c1->SaveAs(tag + ".pdf");
	c1->SaveAs(tag + ".png");
}

TF1 *initfcn(const char* fname, int ifcn, double ptmin, double ptmax, double effguess) {
	TString formula;
	if (ifcn == 0) formula = "[0]*TMath::Erf((x-[1])/[2])";
	else if (ifcn == 1) formula = "[0]*TMath::Erf((x-[1])/[2])+[3]";
	else if (ifcn == 2) formula = "[0]*(TMath::Erf((x-[1])/[2])*TMath::Exp([4]*x)) + [3]";
	else if (ifcn == 3) formula = "[0]";
	else formula = "[0]*(TMath::Erf((x-[1])/[2])*TMath::Exp([4]*x)) + [3]";
	TF1 *ans = new TF1(fname, formula, ptmin, ptmax);
	if (ifcn == 0) {
		ans->SetParNames("eff0", "x0", "m");
		// Initialize the normalization to the efficiency in the last point
		ans->SetParLimits(0, 0, 1);
		ans->SetParLimits(1, -5.5, 10.);
		ans->SetParLimits(2, 0, 20.);
		ans->SetParameters(effguess, 0.1, 0.5);
	}
	else if (ifcn == 1) {
		ans->SetParNames("eff0", "x0", "m", "cst");
		// Initialize the normalization to the efficiency in the last point
		ans->SetParLimits(0, 0, 9.5);
		ans->SetParLimits(1, -10., 10.);
		ans->SetParLimits(2, 0, 50.);
		ans->SetParLimits(3, -2., 2.);
		ans->SetParameters(effguess, 2., 1.0, 0.);
	}
	else if (ifcn == 2) {
		ans->SetParNames("eff0", "x0", "m", "cst", "fall");
		// Initialize the normalization to the efficiency in the last point
		ans->SetParLimits(0, 0, 95);
		ans->SetParLimits(1, -10., 10.);
		ans->SetParLimits(2, 0, 50.);
		ans->SetParLimits(3, -10., 10.);
		ans->SetParLimits(4, -0.5, 0.0);
		ans->SetParameters(effguess, 2., 1., 0., 0.);
	}
	else if (ifcn == 3) {
		ans->SetParNames("const");
		ans->SetParLimits(0, 0, 1.5);
		ans->SetParameter(0, effguess);
	}
	return ans;
}

TF1 *ratiofunc(const char* fname, TF1 *fnum, TF1 *fden) {
	TString formnum = fnum->GetExpFormula();
	TString formden = fden->GetExpFormula();
	int nparnum = fnum->GetNpar();
	int nparden = fden->GetNpar();
	int npartot = nparnum + nparden;
	// replace the names of the parameters of the numerator
	for (int i = 0; i<nparnum; i++) {
		formnum.ReplaceAll(Form("[p%i]", i), Form("[%i]", i));
		formnum.ReplaceAll(Form("[%s]", fnum->GetParName(i)), Form("[%i]", i));
	}
	// replace the names of the parameters of the denominator
	for (int i = 0; i<nparden; i++) {
		formden.ReplaceAll(Form("[p%i]", i), Form("[%i]", i + nparnum));
		formden.ReplaceAll(Form("[%i]", i), Form("[%i]", i + nparnum));
		formden.ReplaceAll(Form("[%s]", fden->GetParName(i)), Form("[%i]", i + nparnum));
	}
	double xmin, xmax; fnum->GetRange(xmin, xmax);
	TF1 *ans = new TF1(fname, TString("(") + formnum + ")/(" + formden + ")", xmin, xmax);
	for (int i = 0; i<nparnum; i++) ans->SetParameter(i, fnum->GetParameter(i));
	for (int i = 0; i<nparden; i++) ans->SetParameter(i + nparnum, fden->GetParameter(i));
	return ans;
}