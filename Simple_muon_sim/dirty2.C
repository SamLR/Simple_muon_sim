void dirty2(){
    TFile* in_file = new TFile("root/sep_6mm_st-x_3mm_st-mat_Galactic_n-particles_1000000_out.root", "READ");
    // TFile* in_file = new TFile("root/sep_6mm_st-x_1mm_st-mat_Cu_n-particles_1000000_out.root", "READ");
    // TFile* in_file = new TFile("root/sep_6mm_st-x_3mm_st-mat_AIR_n-particles_1000000_out.root", "READ");
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
    
    TH1D* hist_raw = new TH1D("dt","dt",20000,0,20000);
    TH1D* hist_rebin = new TH1D("dt_rebin","dt_rebin",100,50,20050);
    for(int entry = 0; entry < n_entries; ++entry) {
        tree->GetEntry(entry);
        if(nhit<2) continue;
        int mu_id = -1;
        double t0=-1.0;
        for(int hit = 0; hit < nhit; ++hit) {
            bool mu_at_1 = (counter[hit]==2) && (pdgid[hit]==13);
            bool e_at_2  = (pdgid[hit]==11) && parentid[hit]==mu_id; // t0 to confirm we've seen a mu
            if (mu_at_1) {
                t0 = tof[hit];
                mu_id = trkid[hit];
            } else if (e_at_2) {
                const double dt = tof[hit]-t0;
                hist_raw->Fill(dt);
                hist_rebin->Fill(dt);   
            }
        }
    }
    TCanvas* can_raw = new TCanvas("Raw", "Raw");
    hist_raw->Draw();

    TCanvas* can_rebin = new TCanvas("Rebin", "Rebin");
    TF1* exp_fit = new TF1("fit","[0]*exp(-x/[1])",50,20000);
    exp_fit->SetParameter(0, hist_rebin->GetMaximum());
    exp_fit->SetParameter(1,1500.0);
    
    hist_rebin->Fit(exp_fit);
    gStyle->SetOptFit(true);
    hist_rebin->Draw();
}