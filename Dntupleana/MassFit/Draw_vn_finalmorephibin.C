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
#include <TGraphAsymmErrors.h>
#include <TMultiGraph.h>

#include <./Systematics.h>

void Draw_vn_finalmorephibin(TString input_morephibin = "rootfiles/vn_morephibin_MBtrig_cent0to10_poly3bkg_effcorrected0.root", TString trigname = "MBtrig", int cent_low = 0, int cent_high = 10, double ptlow = 0.0, double pthigh = 40.0, TString fitoption = "poly3bkg", TString input_promptD0fraction = "promptD0_totaluncertainties/Fractionchange_ratioband_cent0to10.root", bool DrawsysBfeeddown_Alice = false, bool DrawsysBfeeddown_data = true)
{
	TH1::SetDefaultSumw2();
	gStyle->SetOptTitle(0);
	gStyle->SetOptStat(0);

	void CalculateBfeeddownError( TH1D * h_promptf_totaluncertainties, TGraphAsymmErrors * gr_vn_pt_sys_Bfeeddown);
	void Drawmorephibinvn(TH1D * vn_morephibin, TGraphErrors * gr_vn_morephibin_sys, TGraphAsymmErrors * gr_vn_morephibin_sys_Bfeeddown, TString trigname = "MBtrig", TString vnname = "v2", TString Ytitle = "v_{2}", int cent_low = 0, int cent_high = 100, double ptlow = 1.0, double pthigh = 40.0, TString fitoption = "poly3bkg", bool DrawsysBfeeddown_Alice = false, bool DrawsysBfeeddown_data = false);

    TFile * inputdata_promptD0fraction = new TFile(input_promptD0fraction);
    TH1D * h_promptf_totaluncertainties = (TH1D *) inputdata_promptD0fraction->Get("h_promptf_totaluncertainties_DCAcut");

	TFile * inputdata_morephibin = new TFile(input_morephibin);

	TH1D * v2_morephibin = (TH1D *) inputdata_morephibin->Get("v2_morephibin");
	TH1D * v3_morephibin = (TH1D *) inputdata_morephibin->Get("v3_morephibin");
	v2_morephibin->SetName("v2_morephibin"); v2_morephibin->SetTitle("v2_morephibin");
	v3_morephibin->SetName("v3_morephibin"); v3_morephibin->SetTitle("v3_morephibin");

    TH1D * v2_morephibin_sys = (TH1D *) v2_morephibin->Clone("v2_morephibin_sys");
    TH1D * v3_morephibin_sys = (TH1D *) v3_morephibin->Clone("v3_morephibin_sys");

    TH1D * v2_morephibin_relativesys = new TH1D("v2_morephibin_relativesys", "v2_morephibin_relativesys", Nptbin, ptbins);
    TH1D * v3_morephibin_relativesys = new TH1D("v3_morephibin_relativesys", "v3_morephibin_relativesys", Nptbin, ptbins);

    if( fitoption == "poly3bkg" || fitoption == "poly3bkg_floatwidth" )
    {
        Calculatetotalsys( cent_low, cent_high, 1.0, 40.0, "deltaphibins", "v2", v2_morephibin, v2_morephibin_sys, v2_morephibin_relativesys);
        Calculatetotalsys( cent_low, cent_high, 1.0, 40.0, "deltaphibins", "v3", v3_morephibin, v3_morephibin_sys, v3_morephibin_relativesys);
    }

    TGraphErrors * gr_v2_pt_sys = new TGraphErrors(v2_morephibin_sys); gr_v2_pt_sys->SetName("gr_v2_pt_sys");
    TGraphErrors * gr_v3_pt_sys = new TGraphErrors(v3_morephibin_sys); gr_v3_pt_sys->SetName("gr_v3_pt_sys");

    TGraphAsymmErrors* gr_v2_pt_sys_Bfeeddown = new TGraphAsymmErrors(v2_morephibin_sys); gr_v2_pt_sys_Bfeeddown->SetName("gr_v2_pt_sys_Bfeeddown");
    TGraphAsymmErrors* gr_v3_pt_sys_Bfeeddown = new TGraphAsymmErrors(v3_morephibin_sys); gr_v3_pt_sys_Bfeeddown->SetName("gr_v3_pt_sys_Bfeeddown");

	if( DrawsysBfeeddown_Alice )
	{
		CalculateBfeeddownError( h_promptf_totaluncertainties, gr_v2_pt_sys_Bfeeddown);
		CalculateBfeeddownError( h_promptf_totaluncertainties, gr_v3_pt_sys_Bfeeddown);
	}

	if( DrawsysBfeeddown_data )
	{
		Calculate_sys_Bfeeddown( cent_low, cent_high, 1.0, 40.0, "SP", "v2", v2_morephibin, gr_v2_pt_sys_Bfeeddown);
		Calculate_sys_Bfeeddown( cent_low, cent_high, 1.0, 40.0, "SP", "v3", v3_morephibin, gr_v3_pt_sys_Bfeeddown);
	}

	Drawmorephibinvn( v2_morephibin, gr_v2_pt_sys, gr_v2_pt_sys_Bfeeddown, trigname, "v2", "v_{2}", cent_low, cent_high, ptlow, pthigh, fitoption, DrawsysBfeeddown_Alice, DrawsysBfeeddown_data);
	Drawmorephibinvn( v3_morephibin, gr_v3_pt_sys, gr_v3_pt_sys_Bfeeddown, trigname, "v3", "v_{3}", cent_low, cent_high, ptlow, pthigh, fitoption, DrawsysBfeeddown_Alice, DrawsysBfeeddown_data);

	TFile * output = new TFile(Form("rootfiles/vn_finalmorephibin_%s_cent%dto%d_%s_Bfeeddownsys_Alice%d_data%d_effcorrected0.root", trigname.Data(), cent_low, cent_high, fitoption.Data(), DrawsysBfeeddown_Alice, DrawsysBfeeddown_data),"RECREATE");

	v2_morephibin->Write();
	v3_morephibin->Write();
	gr_v2_pt_sys->Write();
	gr_v3_pt_sys->Write();
	gr_v2_pt_sys_Bfeeddown->Write();
	gr_v3_pt_sys_Bfeeddown->Write();
	v2_morephibin_sys->Write();
	v3_morephibin_sys->Write();

	output->Close();
}

void CalculateBfeeddownError( TH1D * h_promptf_totaluncertainties, TGraphAsymmErrors * gr_vn_pt_sys_Bfeeddown)
{
    for( int ibin = 0; ibin < gr_vn_pt_sys_Bfeeddown->GetN(); ibin++ )
    {
        gr_vn_pt_sys_Bfeeddown->GetEXlow()[ibin] = 0.5;
        gr_vn_pt_sys_Bfeeddown->GetEXhigh()[ibin] = 0.5;
        gr_vn_pt_sys_Bfeeddown->GetEYlow()[ibin] = 0.0;

        double vnvalue = gr_vn_pt_sys_Bfeeddown->GetY()[ibin];
        double promptfraction_lower = h_promptf_totaluncertainties->GetBinContent(ibin+1) - h_promptf_totaluncertainties->GetBinError(ibin+1);

        cout << " ibin: " << ibin << " vnvalue: " << vnvalue << " promptfraction: " << h_promptf_totaluncertainties->GetBinContent(ibin+1) << " error: " << h_promptf_totaluncertainties->GetBinError(ibin+1) << endl;

        if( promptfraction_lower > 0 && vnvalue < 1.0 )
            gr_vn_pt_sys_Bfeeddown->GetEYhigh()[ibin] = vnvalue/promptfraction_lower - vnvalue;
        else
            gr_vn_pt_sys_Bfeeddown->GetEYhigh()[ibin] = 0;
    }
}

void Drawmorephibinvn(TH1D * vn_morephibin, TGraphErrors * gr_vn_morephibin_sys, TGraphAsymmErrors * gr_vn_morephibin_sys_Bfeeddown, TString trigname = "MBtrig", TString vnname = "v2", TString Ytitle = "v_{2}", int cent_low = 0, int cent_high = 100, double ptlow = 1.0, double pthigh = 40.0, TString fitoption = "poly3bkg", bool DrawsysBfeeddown_Alice = false, bool DrawsysBfeeddown_data = false)
{
	TCanvas * cfg_vn = new TCanvas(Form("cfg_comparison_%s_%s", trigname.Data(), vnname.Data()));

	gr_vn_morephibin_sys->GetY()[0] = 3.;
	gr_vn_morephibin_sys->GetY()[1] = 3.;
	gr_vn_morephibin_sys_Bfeeddown->GetY()[0] = 3.;
	gr_vn_morephibin_sys_Bfeeddown->GetY()[1] = 3.;
    if( cent_low == 0 && cent_high == 10 ) 
    {   
        vn_morephibin->SetBinContent( vn_morephibin->FindBin(1.5), 3 );
        vn_morephibin->SetBinError( vn_morephibin->FindBin(1.5), 0 );

        gr_vn_morephibin_sys->GetY()[2] = 3.;
        gr_vn_morephibin_sys->GetEY()[2] = 0.;
		gr_vn_morephibin_sys_Bfeeddown->GetY()[2] = 3.;
		gr_vn_morephibin_sys_Bfeeddown->GetEYlow()[2] = 0.;
		gr_vn_morephibin_sys_Bfeeddown->GetEYhigh()[2] = 0.;
    }   

	gr_vn_morephibin_sys->SetFillStyle(0);
	gr_vn_morephibin_sys->SetLineColor(2.0);
	gr_vn_morephibin_sys->SetMarkerSize(0);

	vn_morephibin->SetLineWidth(0);
	vn_morephibin->SetMarkerSize(0.8);
	vn_morephibin->Draw();

	for( int ibin = 0; ibin < gr_vn_morephibin_sys->GetN(); ibin++ )
		gr_vn_morephibin_sys->GetEX()[ibin] = 0.5;

    gr_vn_morephibin_sys->SetMarkerColor(4.0);
    gr_vn_morephibin_sys->SetMarkerStyle(21);
    //gr_vn_morephibin_sys->SetLineWidth(1);
    gr_vn_morephibin_sys->SetLineWidth(0);//hard probes
    gr_vn_morephibin_sys->SetFillStyle(0);
    gr_vn_morephibin_sys->SetLineColor(4.0);

    gr_vn_morephibin_sys_Bfeeddown->SetMarkerColor(4.0);
    gr_vn_morephibin_sys_Bfeeddown->SetMarkerStyle(21);
    gr_vn_morephibin_sys_Bfeeddown->SetLineWidth(0);
    gr_vn_morephibin_sys_Bfeeddown->SetFillStyle(1001);
    gr_vn_morephibin_sys_Bfeeddown->SetFillColor(kBlue-9);
    gr_vn_morephibin_sys_Bfeeddown->SetLineColor(kBlue-9);

    if( DrawsysBfeeddown_Alice || DrawsysBfeeddown_data ) gr_vn_morephibin_sys_Bfeeddown->Draw("2same");

	if( fitoption == "poly3bkg" ) gr_vn_morephibin_sys->Draw("E2same");
	vn_morephibin->Draw("same");

	TLatex Tl;
	Tl.SetNDC();
	Tl.SetTextAlign(12);
	Tl.SetTextSize(0.05);
	Tl.SetTextFont(42);
	Tl.DrawLatex(0.125,0.965, "#font[61]{CMS} #scale[0.8]{Preliminary}");
	Tl.DrawLatex(0.57,0.965, "#scale[0.8]{PbPb #sqrt{s_{NN}} = 5.02 TeV}");

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
    
    if( DrawsysBfeeddown_Alice || DrawsysBfeeddown_data )
    {   
        tex = new TLatex(0.3,0.25,"Filled box: syst. from non-prompt D^{0}");
        tex->SetNDC();
        tex->SetTextFont(42);
        tex->SetTextSize(0.04);
        tex->SetLineWidth(2);
        tex->Draw();

        tex = new TLatex(0.3,0.20,"Open box: other syst.");
        tex->SetNDC();
        tex->SetTextFont(42);
        tex->SetTextSize(0.04);
        tex->SetLineWidth(2);
        tex->Draw();
    }   

	cfg_vn->SaveAs(Form("Plots_vn/cfg_finalmorephibin_%s_%s_cent%dto%d_Bfeeddownsys_Alice%d_data%d_%s_effcorrected0.pdf", trigname.Data(), vnname.Data(), cent_low, cent_high, DrawsysBfeeddown_Alice, DrawsysBfeeddown_data, fitoption.Data()));
}
