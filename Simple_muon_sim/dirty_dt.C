void dirty_dt(){
    TFile* in_file = new TFile("air_out_1M.root", "READ");
    int nhit;
    double tof[500];
    TTree* tree = (TTree*) in_file->Get("t");
    tree->SetBranchAddress("tof",&tof);
    tree->SetBranchAddress("nhit",&nhit);
    
    const int n_entries = tree->GetEntries();
    
    TH1D* hist_raw = new TH1D("dt","dt",20000,0,20000);
    TH1D* hist_rebin = new TH1D("dt_rebin","dt_rebin",200,50,20050);
    for(int entry = 0; entry < n_entries; ++entry) {
        tree->GetEntry(entry);
        if(nhit<2) continue;
        const double t0 = tof[0];
        for(int hit = 1; hit < nhit; ++hit) {
            const double dt = tof[hit]-t0;
            hist_raw->Fill(dt);
            hist_rebin->Fill(dt);
        }
    }
    TCanvas* can_raw = new TCanvas("Raw", "Raw");
    hist_raw->Draw();

    TCanvas* can_rebin = new TCanvas("Rebin", "Rebin");
    // TF1* exp_fit = TF1("fit","[0]*exp(-x/[1])",50,20000);
    // hist_rebin->Fit(exp_fit);
    gStyle->SetOptFit(true);
    hist_rebin->Draw();
}