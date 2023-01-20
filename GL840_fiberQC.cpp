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

void GL840_fiberQC(TString filename,TString calib_filename){
//void GL840_fiberQC(TString filename,TString bg_filename,TString calib_filename, TString calibbg_filename){
  //constant
  const int nsample = 60;
  //const
  int nch = 64;
  //constants for histogram
  const int nbin = 50;//2022/9/14までは30
  const double bin_min = -1.0;
  //const double bin_max = 250.0;//2022/9/14までは180
  const double bin_max = 350.0;
  const int calib_start = 0;
  const int calib_end = 10;
  const double criteria = 0.8;

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

  for(int i=0;i<nch;i++)
  {
    mean[i]=0;
    mean_b[i]=0;
    mean_c[i]=0;
    mean_cb[i]=0;
  }

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
      if(j>=calib_start&&j<=calib_end)
      {
        mean_b[i] = mean_b[i] + voltage[i];
      }else{
        h[i]->Fill(voltage[i]);
      }
    }
  }
  for(int i=0; i<nch; i++)
  {
    mean_b[i] = mean_b[i]/(calib_end - calib_start + 1);
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

  file->Close();

///////////////// BG DATA FILE /////////////////
/*
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
*/
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
      std::cout << "counter j : " << j <<std::endl;
      tree_c->GetEntry(j);
      for(int i=0; i<nch; i++)
      {
        if(j>=calib_start&&j<=calib_end)
        {
          std::cout << "calib_end : " << calib_end <<std::endl;
          if(i==32)
          {
            std::cout << "#############################" << std::endl;
            std::cout << "CH32 : BG pre : " << j << " index : " << mean_cb[i] << std::endl;
            std::cout << "#############################" << std::endl;
          }
          mean_cb[i] = mean_cb[i] + voltage_c[i];
          if(i==32)
          {
            std::cout << "#############################" << std::endl;
            std::cout << "CH32 : BG Calcurating : " << j << " index : " << mean_cb[i] << std::endl;
            std::cout << "#############################" << std::endl;
          }
        }else{
          h_c[i]->Fill(voltage_c[i]);
        }
      }
    }
    for(int i=0; i<nch; i++)
    {
      if(i==32)
      {
        std::cout << "#############################" << std::endl;
        std::cout << "CH32 : BG Before Calcuration : "<< mean_cb[i] << std::endl;
        std::cout << "#############################" << std::endl;
      }
      mean_cb[i] = mean_cb[i]/(calib_end - calib_start + 1);
      if(i==32)
      {
        std::cout << "#############################" << std::endl;
        std::cout << "CH32 : BG : "<< mean_cb[i] << std::endl;
        std::cout << "#############################" << std::endl;
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

    file_c->Close();

    ///////////////// CALIBRATION BG DATA FILE /////////////////
    /*
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
      */
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
    if(i==32)
    {
      std::cout << "#############################" << std::endl;
      std::cout << "CH32 : mean calib  : " << mean_c[i] << " , BG : " << mean_cb[i] << std::endl;
      std::cout << "#############################" << std::endl;
    }
    mean_c[i] = mean_c[i] - mean_cb[i];
    if(i==32)
    {
      std::cout << "#############################" << std::endl;
      std::cout << "CH32 : mean calib - BG : " << mean_c[i] << std::endl;
      std::cout << "#############################" << std::endl;
    }
    prop[i] = mean[i]/mean_c[i];
    if(i==32)
    {
        std::cout << "#############################" << std::endl;
        std::cout << "CH32 : " << prop[i] << " = " << mean[i] << " / " << mean_c[i]  << std::endl;
        std::cout << "#############################" << std::endl;
    }
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

  double prop_mean;
  prop_mean = h_p->GetMean();
  int damaged_fiber;
  damaged_fiber = h_p->Integral(1,h_p->FindBin(prop_mean*criteria));

  std::cout << "#############################" << std::endl;
  //if(damaged_fiber == 0)
  {
    //std::cout << "# " << damaged_fiber << " damaged fiber is found #" << std::endl;
  }//else
  {
    std::cout << "# " << damaged_fiber << " damaged fiber is found #" << std::endl;
    for(int i=0;i<nch;i++)
    {
        //if(prop[i]<prop_mean*criteria)
        {
          std::cout << "# channel " << i << " is damaged #" << std::endl;
          std::cout << prop[i] << " < " << prop_mean*criteria << " = " << prop_mean << " * " << criteria << std::endl;
        }
    }
  }
  std::cout << "#############################" << std::endl;
  std::cout << filename << " was made" << std::endl;

  tree_output->Write();
  rootfile->Close();
}

int main(int argc,char *argv[]){
  GL840_fiberQC(argv[1],argv[2]);
  //std::cout << "Return to main" << std::std::endl;
  return 0;
  //std::cout << "The last line" << std::std::endl;
}
