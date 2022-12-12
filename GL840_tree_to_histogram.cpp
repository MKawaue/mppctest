#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>
#include <TCanvas.h>
#include <TH1D.h>

void GL840_temp20220902(TString filename,TString bg_filename,TString calib_filename, TString calibbg_filename){
  //constant
  const int nsample = 60;
  //const
  int nch = 64;
  //constants for histogram
  const int nbin = 50;//2022/9/14までは30
  const double bin_min = -1.0;
  const double bin_max = 250.0;//2022/9/14までは180

  //global variable
  TCanvas *c1 = new TCanvas("c1","c1",1500,1200);
  c1->Divide(6,6);
  TCanvas *c2 = new TCanvas("c2","c2",1500,1200);
  c2->Divide(6,6);
  double voltage[nch];
  double voltage_b[nch];
  double voltage_c[nch];
  double voltage_cb[nch];
  TH1D *h[nch];
  TH1D *h_b[nch];
  TH1D *h_c[nch];
  TH1D *h_cb[nch];

  double mean[nch];
  double mean_b[nch];
  double mean_c[nch];
  double mean_cb[nch];

  double prop[nch];

///////////////// DATA FILE /////////////////
  TFile *file = new TFile(filename);
  TTree * tree = (TTree*)file->Get("tree");
  tree->SetBranchAddress("voltage",voltage);

  for(int i=0;i<nch;i++)
  {
    h[i] = new TH1D(Form("h%d",i),Form("h%d",i),nbin,bin_min,bin_max);
  }

  for(int j=0; j<nsample; j++)
  {
    tree->GetEntry(j);
    for(int i=0; i<nch; i++)
    {
      h[i]->Fill(voltage[i]);
    }
  }

  for(int i=0; i<nch; i++)
  {
    if(i<nch/2)
    {
      c1->cd(i+1);
    }else
    {
      c2->cd(i-31);
    }
    h[i]->Draw();
    mean[i] = h[i]->GetMean();
  }

///////////////// BG DATA FILE /////////////////
  TFile *file_b = new TFile(bg_filename);
  TTree * tree_b = (TTree*)file_b->Get("tree");
  tree_b->SetBranchAddress("voltage",voltage_b);

  for(int i=0;i<nch;i++)
  {
    h_b[i] = new TH1D(Form("h_b%d",i),Form("h_b%d",i),nbin,bin_min,bin_max);
  }

  for(int j=0; j<nsample; j++)
  {
    tree_b->GetEntry(j);
    for(int i=0; i<nch; i++)
    {
      h_b[i]->Fill(voltage_b[i]);
    }
  }

  for(int i=0; i<nch; i++)
  {
    if(i<nch/2)
    {
      c1->cd(i+1);
    }else
    {
      c2->cd(i-31);
    }
    h_b[i]->Draw();
    mean_b[i] = h_b[i]->GetMean();
  }

  ///////////////// CALIBRATION DATA FILE /////////////////
    TFile *file_c = new TFile(calib_filename);
    TTree * tree_c = (TTree*)file_c->Get("tree");
    tree_c->SetBranchAddress("voltage",voltage_c);

    for(int i=0;i<nch;i++)
    {
      h_c[i] = new TH1D(Form("h_c%d",i),Form("h_c%d",i),nbin,bin_min,bin_max);
    }

    for(int j=0; j<nsample; j++)
    {
      tree_c->GetEntry(j);
      for(int i=0; i<nch; i++)
      {
        h_c[i]->Fill(voltage_c[i]);
      }
    }

    for(int i=0; i<nch; i++)
    {
      if(i<nch/2)
      {
        c1->cd(i+1);
      }else
      {
        c2->cd(i-31);
      }
      h_c[i]->Draw();
      mean_c[i] = h_c[i]->GetMean();
    }

    ///////////////// CALIBRATION BG DATA FILE /////////////////
      TFile *file_cb = new TFile(calibbg_filename);
      TTree * tree_cb = (TTree*)file_cb->Get("tree");
      tree_cb->SetBranchAddress("voltage",voltage_cb);

      for(int i=0;i<nch;i++)
      {
        h_cb[i] = new TH1D(Form("h_cb%d",i),Form("h_cb%d",i),nbin,bin_min,bin_max);
      }

      for(int j=0; j<nsample; j++)
      {
        tree_cb->GetEntry(j);
        for(int i=0; i<nch; i++)
        {
          h_cb[i]->Fill(voltage_cb[i]);
        }
      }

      for(int i=0; i<nch; i++)
      {
        if(i<nch/2)
        {
          c1->cd(i+1);
        }else
        {
          c2->cd(i-31);
        }
        h_cb[i]->Draw();
        mean_cb[i] = h_cb[i]->GetMean();
      }

  //OUTPUT FILE
  //filename = filename.ReplaceAll(".root","_64.root");
  filename = filename.ReplaceAll(".root","_64.root");
  TFile *rootfile = new TFile(filename,"recreate");
  TTree * tree_output = new TTree("tree","tree");
  double prop_for_tree;
  tree_output->Branch("prop",&prop_for_tree,"prop/D");

  //PROP
  TCanvas *c_p = new TCanvas("cp","cp",500,500);
  TH1D *h_p = new TH1D("hp","hp",30,0.,5.);
  c_p->cd();
  for(int i=0;i<nch;i++)
  {
    std::cout << "mean" << mean[i] << std::endl;
    if(i==39){std::cout << "//////ch39 " << mean[i] << ", b : " << mean_b[i] << ", c : " << mean_c[i] << ", cb : " << mean_cb[i] << std::endl;}
    mean[i] = mean[i] - mean_b[i];
    mean_c[i] = mean_c[i] - mean_cb[i];
    prop[i] = mean[i]/mean_c[i];
    prop_for_tree = prop[i];
    //if(i<32||(i>43&&i<48)||i>59)
    //if(i<32)
    //if(i==19||i==23||i==27||i==31||i==35||i==39||i==40||i==44)//fibercut用
    {
      h_p->Fill(prop[i]);
      //std::cout << "mean" << mean[i] << ", mean_b" << mean_b[i] <<std::endl;
      std::cout << "ch" << i << " : " << prop[i]<<std::endl;
      tree_output->Fill();
    }
  }
  h_p->Draw();

  tree_output->Write();

}

int main(int argc, char *argv[]){
  GL840_temp20220902(argv[1],argv[2],argv[3],argv[4]);
  return 0;
}
