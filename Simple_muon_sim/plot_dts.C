#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TStyle.h"


void plot_dts(){
    
    const int n_files = 6;
    TString file_roots [3] = 
            {"1mm_st-mat_Cu",
             "300mm_st-mat_AIR",
             "300mm_st-mat_Galactic"};                         // 
              // "6mm_st-mat_AIR",
              // "6mm_st-mat_Galactic"};
    
    TString file_suffix_pos = "_n-particles_1000000_mu+_out.root";
    TString file_suffix_neg = "_n-particles_1000000_mu-_out.root";
    TString file_prefix = "root/sep_0mm_st-x_";
    TString file_names [n_files];
    for(int i = 0; i < n_files/2; ++i) {
        file_names[2*i]   = file_prefix + file_roots[i] + file_suffix_pos;
        file_names[2*i+1] = file_prefix + file_roots[i] + file_suffix_neg;
    }                      
    TH1D* hists [n_files];
    const int i_charges [n_files] = {-1,1,-1,1,-1,1};
    
    for (int file = 0; file<n_files; ++file){
        TFile* in_file = new TFile(file_names[file], "READ");
        TString hist_name = file_roots[static_cast<int>(file/2)];
        hist_name += (file%2==0) ? "+" : "-";
        cout << file_names[file] <<" "<< hist_name << endl;  
        hists[file] = new TH1D(hist_name, hist_name, 100, 50, 20050);
        int nhit;
        int pdgid[500];
        int counter[500];
        int parentid[500];
        int trkid[500];
        double tof[500];
        
        TTree* tree = (TTree*) in_file->Get("t");
        tree->SetBranchAddress("tof",&tof);
        tree->SetBranchAddress("nhit",&nhit);
        tree->SetBranchAddress("counter",&counter);
        tree->SetBranchAddress("parentid",&parentid);
        tree->SetBranchAddress("trkid",&trkid);
        tree->SetBranchAddress("pdgid",&pdgid);
        
        const int n_entries = tree->GetEntries();
        
        for(int entry = 0; entry < n_entries; ++entry) {
            tree->GetEntry(entry);
            if(nhit<2) continue;
            int mu_id = -1;
            double t0=-1.0;
            for(int hit = 0; hit < nhit; ++hit) {
                bool mu_at_1 = (counter[hit]==2) && (pdgid[hit]==i_charges[file]*13);
                bool e_at_2  = (pdgid[hit]==i_charges[file]*11) && parentid[hit]==mu_id; // t0 to confirm we've seen a mu
                if (mu_at_1) {
                    t0 = tof[hit];
                    mu_id = trkid[hit];
                } else if (e_at_2) {
                    const double dt = tof[hit]-t0;
                    hists[file]->Fill(dt);
                }
            }
        }
        // in_file->Close();
    }
    gStyle->SetOptFit(true);
    TCanvas* can = new TCanvas("Rebin", "Rebin");
    can->Divide(3,2);   
    TF1* fits[n_files];
    // hists[0]->Draw();
    for(int file = 0; file < n_files; ++file) {
        can->cd(file+1);
        TString name = "fit";
        name += file;
        fits[file] = new TF1(name,"[0]*exp(-x/[1])",50,20050);
        cout << "here I am" << endl;
        fits[file]->SetParameter(0, hists[file]->GetMaximum());
        fits[file]->SetParameter(1,1500.0);
        hists[file]->Fit(fits[file]);
        hists[file]->Draw();

    }
}