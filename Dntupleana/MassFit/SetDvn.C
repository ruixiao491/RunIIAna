#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <iomanip>
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

//centrality 0-10%, 10-30, 30-50%
//SP method
TH1D * h_v2_pt[3];
TH1D * h_v3_pt[3];
TGraphErrors * gr_v2_pt_sys[3];
TGraphErrors * gr_v3_pt_sys[3];
TGraphAsymmErrors * gr_v2_pt_sys_Bfeeddown[3];
TGraphAsymmErrors * gr_v3_pt_sys_Bfeeddown[3];

//charged particle SP
TGraphErrors * grSteveSPv2_chargeparticle[3];
TGraphErrors * grSteveSPv2_sys_chargeparticle[3];
TGraphErrors * grSteveSPv3_chargeparticle[3];
TGraphErrors * grSteveSPv3_sys_chargeparticle[3];

//phi bin method
TH1D * h_v2_pt_phibin[3];
TH1D * h_v3_pt_phibin[3];
TGraphErrors * gr_v2_pt_sys_phibin[3];
TGraphErrors * gr_v3_pt_sys_phibin[3];
TGraphAsymmErrors * gr_v2_pt_sys_Bfeeddown_phibin[3];
TGraphAsymmErrors * gr_v3_pt_sys_Bfeeddown_phibin[3];

void Readvn_SP()
{
	TFile * input[3];
	input[0]= new TFile("rootfiles/vn_finalcombinedfit_vnvsmass_MBtrig_SP_cent0to10_poly3bkg_floatwidth_Bfeeddownsys_Alice0_data1_effcorrected0.root");
	input[1]= new TFile("rootfiles/vn_finalcombinedfit_vnvsmass_MBtrig_SP_cent10to30_poly3bkg_floatwidth_Bfeeddownsys_Alice0_data1_effcorrected0.root");
	input[2]= new TFile("rootfiles/vn_finalcombinedfit_vnvsmass_MBtrig_SP_cent30to50_poly3bkg_floatwidth_Bfeeddownsys_Alice0_data1_effcorrected0.root");

	for( int icen = 0; icen < 3; icen++ )
	{
		h_v2_pt[icen] = ( TH1D * ) input[icen]->Get("h_v2_pt");
		h_v3_pt[icen] = ( TH1D * ) input[icen]->Get("h_v3_pt");
		gr_v2_pt_sys[icen] = ( TGraphErrors * ) input[icen]->Get("gr_v2_pt_sys");
		gr_v3_pt_sys[icen] = ( TGraphErrors * ) input[icen]->Get("gr_v3_pt_sys");
		gr_v2_pt_sys_Bfeeddown[icen] = ( TGraphAsymmErrors * ) input[icen]->Get("gr_v2_pt_sys_Bfeeddown");
		gr_v3_pt_sys_Bfeeddown[icen] = ( TGraphAsymmErrors * ) input[icen]->Get("gr_v3_pt_sys_Bfeeddown");

		grSteveSPv2_chargeparticle[icen] = ( TGraphErrors * ) input[icen]->Get("grSteveSPv2_chargeparticle");
		grSteveSPv2_sys_chargeparticle[icen] = ( TGraphErrors * ) input[icen]->Get("grSteveSPv2_sys_chargeparticle");
		grSteveSPv3_chargeparticle[icen] = ( TGraphErrors * ) input[icen]->Get("grSteveSPv3_chargeparticle");
		grSteveSPv3_sys_chargeparticle[icen] = ( TGraphErrors * ) input[icen]->Get("grSteveSPv3_sys_chargeparticle");
	}
}

void Readvn_phibin()
{
	TFile * input_phibin[3];
	input_phibin[0] = new TFile("rootfiles/vn_finalmorephibin_MBtrig_cent0to10_poly3bkg_Bfeeddownsys_Alice0_data1_effcorrected0.root");
	input_phibin[1] = new TFile("rootfiles/vn_finalmorephibin_MBtrig_cent10to30_poly3bkg_Bfeeddownsys_Alice0_data1_effcorrected0.root");
	input_phibin[2] = new TFile("rootfiles/vn_finalmorephibin_MBtrig_cent30to50_poly3bkg_Bfeeddownsys_Alice0_data1_effcorrected0.root");

	for( int icen = 0; icen < 3; icen++ )
	{
		h_v2_pt_phibin[icen] = (TH1D *) input_phibin[icen]->Get("v2_morephibin");
		h_v3_pt_phibin[icen] = (TH1D *) input_phibin[icen]->Get("v3_morephibin");
		gr_v2_pt_sys_phibin[icen] = (TGraphErrors *) input_phibin[icen]->Get("gr_v2_pt_sys");
		gr_v3_pt_sys_phibin[icen] = (TGraphErrors *) input_phibin[icen]->Get("gr_v3_pt_sys");
		gr_v2_pt_sys_Bfeeddown_phibin[icen] = (TGraphAsymmErrors *) input_phibin[icen]->Get("gr_v2_pt_sys_Bfeeddown");
		gr_v3_pt_sys_Bfeeddown_phibin[icen] = (TGraphAsymmErrors *) input_phibin[icen]->Get("gr_v3_pt_sys_Bfeeddown");
	}
}

void SetDvn()
{
	Readvn_SP();
	Readvn_phibin();

	for( int icen = 0; icen < 3; icen++ )
	{
		for( int ipt = 0; ipt < gr_v2_pt_sys[icen]->GetN(); ipt++ )
		{
			//cout << std::setprecision(3) << " pt center: " << gr_v2_pt_sys[icen]->GetX()[ipt];
			//cout << std::setprecision(4) << " v2: " << h_v2_pt[icen]->GetBinContent(ipt+1) << " stat: " << h_v2_pt[icen]->GetBinError(ipt+1);
			//cout << std::setprecision(4) << " B feed-down sys: " << gr_v2_pt_sys_Bfeeddown[icen]->GetEYlow()[ipt] << " other sys: " << gr_v2_pt_sys[icen]->GetEY()[ipt] << endl;
			//cout << std::setprecision(3) << " pt center: " << gr_v2_pt_sys[icen]->GetX()[ipt];
			cout << std::setprecision(4) << std::fixed << " " << h_v2_pt[icen]->GetBinContent(ipt+1) << " " << h_v2_pt[icen]->GetBinError(ipt+1);
			cout << std::setprecision(4) << std::fixed << " " << gr_v2_pt_sys_Bfeeddown[icen]->GetEYlow()[ipt] << " " << gr_v2_pt_sys[icen]->GetEY()[ipt] << endl;
		}
	}
}
