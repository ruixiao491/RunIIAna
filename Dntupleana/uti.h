#ifndef uti_H
#define uti_H

#include <iostream>
#include <iomanip>
#include <utility>
#include <TFile.h>
#include <TDirectoryFile.h>
#include <TDirectory.h>
#include <TTree.h>
#include <TNtuple.h>
#include <TH1D.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH2D.h>
#include <TProfile.h>
#include <TGraph.h>
#include <TGraphAsymmErrors.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TPad.h>
#include <TLatex.h>
#include <TLegend.h>
#include <TLine.h>
#include <TBox.h>
#include <TCut.h>
#include <TMath.h>
#include <TRandom.h>
#include <TRandom3.h>
#include <TROOT.h>
#include <TStyle.h>

#include "parameters.h"

// Remove error
void removeError(TH1* h)
{
	for(int i=1;i<=h->GetNbinsX();i++)
	{
		h->SetBinError(i,0);
	}	
}

// divide by bin width
void divideBinWidth(TH1* h)
{
	h->Sumw2();
	for(int i=1;i<=h->GetNbinsX();i++)
	{
		Float_t val = h->GetBinContent(i);
		Float_t valErr = h->GetBinError(i);
		val/=h->GetBinWidth(i);
		valErr/=h->GetBinWidth(i);
		h->SetBinContent(i,val);
		h->SetBinError(i,valErr);
	}
	h->GetXaxis()->CenterTitle();
	h->GetYaxis()->CenterTitle();
}

int decideptbin( float dpt, float inputptbins[], int inputNptbin)
{
	int ipt = -1;
	for ( int i = 0 ; i < inputNptbin; i++)
	{
		if (dpt >= inputptbins[i] && dpt < inputptbins[i+1])  { ipt = i; break; }
	}
	if ( dpt > inputptbins[inputNptbin] ) ipt = inputNptbin-1;
	return ipt;
}

void book_masshist(TH1D * h[], float inputptbins[], int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
		h[i] = new TH1D(Form("%s_%d", hname.c_str(), i), h_title, Nmassbin, hmin, hmax);
		h[i]->Sumw2();
	}
}

void get_masshist(TFile * inputfile, TH1D * h[], int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int i = 0; i<inputNptbin; i++)
		h[i] = (TH1D *) inputfile->Get(Form("%s_%d", hname.c_str(), i));
}

void book_TProfile_mass_vn(TProfile * h[], float inputptbins[], int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
		h[i] = new TProfile(Form("%s_%d", hname.c_str(), i), h_title, Nmassbin, hmin, hmax);
		h[i]->Sumw2();
	}
}

void get_TProfile_mass_vn(TFile * inputfile, TProfile * h[], int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int i = 0; i<inputNptbin; i++)
		h[i] = (TProfile *) inputfile->Get(Form("%s_%d", hname.c_str(), i));
}

void book_masshist_random(TH1D * h[][Nsubsets], float inputptbins[], int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
		for(int j = 0; j<Nsubsets; j++)
		{
			h[i][j] = new TH1D(Form("%s_%d_%d", hname.c_str(), i, j), h_title, Nmassbin, hmin, hmax);
			h[i][j]->Sumw2();
		}
	}
}

void get_masshist_random(TFile * inputfile, TH1D * h[][Nsubsets], int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int i = 0; i<inputNptbin; i++)
		for(int j = 0; j<Nsubsets; j++)
			h[i][j] = (TH1D *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), i, j));
}

void book_TProfile_mass_vn_random(TProfile * h[][Nsubsets], float inputptbins[], int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
		for(int j = 0; j<Nsubsets; j++)
		{
			h[i][j] = new TProfile(Form("%s_%d_%d", hname.c_str(), i, j), h_title, Nmassbin, hmin, hmax);
			h[i][j]->Sumw2();
		}
	}
}

void get_TProfile_mass_vn_random(TFile * inputfile, TProfile * h[][Nsubsets], int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int i = 0; i<inputNptbin; i++)
		for(int j = 0; j<Nsubsets; j++)
			h[i][j] = (TProfile *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), i, j));
}

void book_TH2D_mass_vn(TH2D * h[], float inputptbins[], int inputNptbin, string hname, int Nmassbin, float hmin, float hmax, int Nvnbin, float vnmin, float vnmax)
{
	TH2::SetDefaultSumw2();

	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
		h[i] = new TH2D(Form("%s_%d", hname.c_str(), i), h_title, Nmassbin, hmin, hmax, Nvnbin, vnmin, vnmax);
		h[i]->Sumw2();
	}
}

void get_TH2D_mass_vn(TFile * inputfile, TH2D * h[], int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int i = 0; i<inputNptbin; i++)
		h[i] = (TH2D *) inputfile->Get(Form("%s_%d", hname.c_str(), i));
}

void book_hist_vn_morephibin(TH1D * h[][Nphibinmore], float inputptbins[], int inputNptbin, int inputNphibin, TH1D * input_phibins, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != Nphibinmore )
	{
		cout << "inputNphibinmore out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(1);
	}

	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		for(int j = 0; j<inputNphibin; j++)
		{
			float phi_low = input_phibins->GetBinLowEdge(j+1);
			float phi_high = input_phibins->GetBinLowEdge(j+2);
			sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c, %1.2f < #Delta#Phi < %1.2f", pt_low, pt_high, phi_low, phi_high);
			h[i][j] = new TH1D(Form("%s_%d_%d", hname.c_str(), i, j), h_title, Nmassbin, hmin, hmax);
			h[i][j]->Sumw2();
		}
	}
}

void get_hist_vn_morephibin(TFile * inputfile, TH1D * h[][Nphibinmore], int inputNptbin, int inputNphibin, string hname)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != Nphibinmore )
	{
		cout << "inputNphibinmore out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(2);
	}

	for(int i = 0; i<inputNptbin; i++)
	{
		for(int j = 0; j<inputNphibin; j++)
		{
			h[i][j] = (TH1D *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), i, j));
		}
	}
}

void book_hist_vn_inoutplane(TH1D * h[][2], float inputptbins[], int inputNptbin, int inputNphibin, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != 2 )
	{
		cout << "inputNphibin out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(1);
	}

	char h_title[1000];
	for(int i = 0; i<inputNptbin; i++)
	{
		float pt_low = inputptbins[i];
		float pt_high = inputptbins[i+1];
		for(int j = 0; j<inputNphibin; j++)
		{
			if( j == 0 )  sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c, in plane", pt_low, pt_high);
			else sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c, out plane", pt_low, pt_high);
			h[i][j] = new TH1D(Form("%s_%d_%d", hname.c_str(), i, j), h_title, Nmassbin, hmin, hmax);
			h[i][j]->Sumw2();
		}
	}
}

void get_hist_vn_inoutplane(TFile * inputfile, TH1D * h[][2], int inputNptbin, int inputNphibin, string hname)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != 2 )
	{
		cout << "inputNphibin out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(2);
	}

	for(int i = 0; i<inputNptbin; i++)
	{
		for(int j = 0; j<inputNphibin; j++)
		{
			h[i][j] = (TH1D *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), i, j));
		}
	}
}

//for DCA cut sequence
void book_masshist_DCAsequence(TH1D * h[][Nptbin], float inputptbins[], int inputNDCAcut, int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	char h_title[1000];
	for(int iDCA = 0; iDCA < inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			float pt_low = inputptbins[i];
			float pt_high = inputptbins[i+1];
			sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
			h[iDCA][i] = new TH1D(Form("%s_%d_%d", hname.c_str(), iDCA, i), h_title, Nmassbin, hmin, hmax);
			h[iDCA][i]->Sumw2();
		}
	}
}

void get_masshist_DCAsequence(TFile * inputfile, TH1D * h[][Nptbin], int inputNDCAcut, int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for( int iDCA = 0; iDCA<inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			h[iDCA][i] = (TH1D *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), iDCA, i));
		}
	}
}

void book_TProfile_mass_vn_DCAsequence(TProfile * h[][Nptbin], float inputptbins[], int inputNDCAcut, int inputNptbin, string hname, int Nmassbin, float hmin, float hmax)
{
	char h_title[1000];
	for(int iDCA = 0; iDCA < inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			float pt_low = inputptbins[i];
			float pt_high = inputptbins[i+1];
			sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c", pt_low, pt_high);
			h[iDCA][i] = new TProfile(Form("%s_%d_%d", hname.c_str(), iDCA, i), h_title, Nmassbin, hmin, hmax);
			h[iDCA][i]->Sumw2();
		}
	}
}

void get_TProfile_mass_vn_DCAsequence(TFile * inputfile, TProfile * h[][Nptbin], int inputNDCAcut, int inputNptbin, string hname)
{
	TH1::SetDefaultSumw2();

	for(int iDCA = 0; iDCA < inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			h[iDCA][i] = (TProfile *) inputfile->Get(Form("%s_%d_%d", hname.c_str(), iDCA, i));
		}
	}
}

void book_hist_vn_morephibin_DCAsequence(TH1D * h[][Nptbin][Nphibinmore], float inputptbins[], int inputNDCAcut, int inputNptbin, int inputNphibin, TH1D * input_phibins, string hname, int Nmassbin, float hmin, float hmax)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != Nphibinmore )
	{
		cout << "inputNphibinmore out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(1);
	}

	char h_title[1000];
	for(int iDCA = 0; iDCA < inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			float pt_low = inputptbins[i];
			float pt_high = inputptbins[i+1];
			for(int j = 0; j<inputNphibin; j++)
			{
				float phi_low = input_phibins->GetBinLowEdge(j+1);
				float phi_high = input_phibins->GetBinLowEdge(j+2);
				sprintf(h_title, " %2.1f < p_{T} < %2.1f GeV/c, %1.2f < #Delta#Phi < %1.2f", pt_low, pt_high, phi_low, phi_high);
				h[iDCA][i][j] = new TH1D(Form("%s_%d_%d_%d", hname.c_str(), iDCA, i, j), h_title, Nmassbin, hmin, hmax);
				h[iDCA][i][j]->Sumw2();
			}
		}
	}
}

void get_hist_vn_morephibin_DCAsequence(TFile * inputfile, TH1D * h[][Nptbin][Nphibinmore], int inputNDCAcut, int inputNptbin, int inputNphibin, string hname)
{
	TH1::SetDefaultSumw2();

	if( inputNphibin != Nphibinmore )
	{
		cout << "inputNphibinmore out of range!!!  inputNphibin = " << inputNphibin << endl;
		exit(2);
	}

	for(int iDCA = 0; iDCA<inputNDCAcut; iDCA++)
	{
		for(int i = 0; i<inputNptbin; i++)
		{
			for(int j = 0; j<inputNphibin; j++)
			{
				h[iDCA][i][j] = (TH1D *) inputfile->Get(Form("%s_%d_%d_%d", hname.c_str(), iDCA, i, j));
			}
		}
	}
}

// make a histogram from TF1 function
TH1F* functionHist(TF1* f, TH1F* h, TString fHistname)
{
	TH1F* hF = (TH1F*)h->Clone(fHistname);
	for (int i=1;i<=h->GetNbinsX();i++)
	{
		Double_t var = f->Integral(h->GetBinLowEdge(i),h->GetBinLowEdge(i+1))/h->GetBinWidth(i);
		hF->SetBinContent(i,var);
		hF->SetBinError(i,0);
	}
	return hF;
}

TLegend* myLegend(Double_t x1, Double_t y1, Double_t x2, Double_t y2)
{
	TLegend* leg = new TLegend(x1,y1,x2,y2);
	leg->SetBorderSize(0);
	leg->SetFillStyle(0);
	return leg; 
}

#endif //  uti_H
