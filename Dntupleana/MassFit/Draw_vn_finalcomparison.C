#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TF1.h>
#include <TNamed.h>
#include <TNtuple.h>
#include <TTree.h>
#include <TMath.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TGraphErrors.h>
#include <TMultiGraph.h>

void Draw_vn_finalcomparison(TString input_morephibin = "rootfiles/vn_finalmorephibins_MBtrig_cent30to50_poly3bkg_effcorrected0.root", TString input_vnvmass_SP = "rootfiles/vn_combinedfit_vnvsmass_MBtrig_SP_cent30to50_poly3bkg_floatwidth_effcorrected0.root", TString trigname = "MBtrig", int cent_low = 30, int cent_high = 50, double ptlow = 0.0, double pthigh = 40.0, bool Drawchargedparticle = false, TString fitoption = "poly3bkg_floatwidth_poly3bkg_floatwidth")
{
	TH1::SetDefaultSumw2();
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	void Drawcomparison(TH1D * vn_morephibin, TH1D * vn_morephibin_sys, TH1D * vn_vnvsmass_SP, TH1D * vn_vnvsmass_SP_sys, TString trigname = "MBtrig", TString vnname = "v2", TString Ytitle = "v_{2}", int cent_low = 0, int cent_high = 100, double ptlow = 1.0, double pthigh = 35.0, bool Drawchargedparticle = false, TString fitoption = "poly3bkg");

	TFile * inputdata_morephibin = new TFile(input_morephibin);
	TFile * inputdata_vnvsmass_SP = new TFile(input_vnvmass_SP);

	TH1D * v2_morephibin = (TH1D *) inputdata_morephibin->Get("v2_morephibin");
	TH1D * v3_morephibin = (TH1D *) inputdata_morephibin->Get("v3_morephibin");
	v2_morephibin->SetName("v2_morephibin"); v2_morephibin->SetTitle("v2_morephibin");
	v3_morephibin->SetName("v3_morephibin"); v3_morephibin->SetTitle("v3_morephibin");

	TH1D * v2_morephibin_sys = (TH1D *) inputdata_morephibin->Get("v2_morephibin_sys");
	TH1D * v3_morephibin_sys = (TH1D *) inputdata_morephibin->Get("v3_morephibin_sys");
	v2_morephibin_sys->SetName("v2_morephibin_sys"); v2_morephibin_sys->SetTitle("v2_morephibin_sys");
	v3_morephibin_sys->SetName("v3_morephibin_sys"); v3_morephibin_sys->SetTitle("v3_morephibin_sys");

	TH1D * h_v2_pt_SP = (TH1D *) inputdata_vnvsmass_SP->Get("h_v2_pt");
	TH1D * h_v3_pt_SP = (TH1D *) inputdata_vnvsmass_SP->Get("h_v3_pt");
	h_v2_pt_SP->SetName("h_v2_pt_SP"); h_v2_pt_SP->SetTitle("h_v2_pt_SP");
	h_v3_pt_SP->SetName("h_v3_pt_SP"); h_v3_pt_SP->SetTitle("h_v3_pt_SP");

	TH1D * h_v2_pt_sys_SP = (TH1D *) inputdata_vnvsmass_SP->Get("h_v2_pt_sys");
	TH1D * h_v3_pt_sys_SP = (TH1D *) inputdata_vnvsmass_SP->Get("h_v3_pt_sys");
	h_v2_pt_sys_SP->SetName("h_v2_pt_sys_SP"); h_v2_pt_sys_SP->SetTitle("h_v2_pt_sys_SP");
	h_v3_pt_sys_SP->SetName("h_v3_pt_sys_SP"); h_v3_pt_sys_SP->SetTitle("h_v3_pt_sys_SP");

	Drawcomparison( v2_morephibin, v2_morephibin_sys, h_v2_pt_SP, h_v2_pt_sys_SP, trigname, "v2", "v_{2}", cent_low, cent_high, ptlow, pthigh, Drawchargedparticle, fitoption);

	if( Drawchargedparticle ) return;
	Drawcomparison( v3_morephibin, v3_morephibin_sys, h_v3_pt_SP, h_v3_pt_sys_SP, trigname, "v3", "v_{3}", cent_low, cent_high, ptlow, pthigh, false, fitoption);

	TFile * output = new TFile(Form("rootfiles/vn_phibinandSP_%s_cent%dto%d_%s.root", trigname.Data(), cent_low, cent_high, fitoption.Data()),"RECREATE");
	v2_morephibin->Write();
	v3_morephibin->Write();
	h_v2_pt_SP->Write();
	h_v3_pt_SP->Write();
	output->Close();
}

void Drawcomparison(TH1D * vn_morephibin, TH1D * vn_morephibin_sys, TH1D * vn_vnvsmass_SP, TH1D * vn_vnvsmass_SP_sys, TString trigname = "MBtrig", TString vnname = "v2", TString Ytitle = "v_{2}", int cent_low = 0, int cent_high = 100, double ptlow = 1.0, double pthigh = 35.0, bool Drawchargedparticle = false, TString fitoption = "poly3bkg")
{
	TCanvas * cfg_vn = new TCanvas(Form("cfg_comparison_%s_%s", trigname.Data(), vnname.Data()));

	vn_morephibin->Draw();
	vn_morephibin_sys->Draw("E2same");
	vn_morephibin->Draw("same");
	vn_vnvsmass_SP_sys->Draw("E2same");
	vn_vnvsmass_SP->Draw("same");

	TLatex Tl;
	Tl.SetNDC();
	Tl.SetTextAlign(12);
	Tl.SetTextSize(0.05);
	Tl.SetTextFont(42);
	Tl.DrawLatex(0.125,0.965, "#font[61]{CMS} #scale[0.8]{Preliminary}");
	Tl.DrawLatex(0.57,0.965, "#scale[0.8]{PbPb #sqrt{s_{NN}} = 5.02 TeV}");

	TLegend * leg = new TLegend(0.50, 0.74, 0.70, 0.87);
	leg->SetTextSize(0.05);
	leg->SetTextFont(42);
//	leg->AddEntry(vn_vnvsmass_SP, Form("%s vs mass method {SP}", Ytitle.Data()));
	leg->AddEntry(vn_vnvsmass_SP, "SP method");
	leg->AddEntry(vn_morephibin, "#Delta#Phi bins method");

	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	leg->Draw();

	TLatex* tex;
	tex = new TLatex(0.20,0.83,"|y| < 1.0");
	tex->SetNDC();
	tex->SetTextFont(42);
	tex->SetTextSize(0.05);
	tex->SetLineWidth(2);
	tex->Draw();

	tex = new TLatex(0.20,0.76,Form("Cent. %d-%d%%", cent_low, cent_high));
	tex->SetNDC();
	tex->SetTextFont(42);
	tex->SetTextSize(0.05);
	tex->SetLineWidth(2);
	tex->Draw();

	TF1 * fun = new TF1("fun", "0.0", 0, 100);
	fun->SetLineColor(1.0);
	fun->SetLineStyle(3);
	fun->SetLineWidth(1);
	fun->Draw("same");

	cfg_vn->SaveAs(Form("Plots_vn/cfg_finalcomparison_%s_%s_cent%dto%d_charged%d_phibinandSP_%s.pdf", trigname.Data(), vnname.Data(), cent_low, cent_high, Drawchargedparticle, fitoption.Data()));
}