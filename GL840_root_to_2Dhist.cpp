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
#include <TH2D.h>

void GL840_root_to_2Dhist(TString filename){
  //constant
  const int nsample = 60;
  const int nch = 64;
  //constants for histogram
  const int nbin = 50;//50;
  const double bin_min = -0.1;
  const double bin_max = 4.5;

  //global variable
  TCanvas *c = new TCanvas("c","c",500,500);
  double prop;
  double charge[nch];
  TH1D *h = new TH1D("h","h",nbin,bin_min,bin_max);
  double mean;

///////////////// 64 /////////////////
  TFile *file = new TFile(filename);
  TTree *tree = (TTree*)file->Get("tree");
  tree->SetBranchAddress("prop",&prop);
  for(int j=0; j<tree->GetEntries(); j++)
  {
    tree->GetEntry(j);
    h->Fill(prop);
  }
  h->Draw();

  ///map
  TCanvas *c_map1 = new TCanvas("c_map1","c_map1",500,500);
  TH2D *h_map1 = new TH2D("h_map1","h_map1",8,0,8,8,0,8);
  h_map1->GetZaxis()->SetRangeUser(0,1.5);

  int nentry;

  c_map1->cd();
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<8;j++)
    {
      //if(i*8+j<40)//64chでは消す
      {
        switch(i*8+j)
        {
          case 0:
            nentry = 3;
            break;
          case 1:
            nentry = 2;
            break;
          case 2:
            nentry = 1;
            break;
          case 3:
            nentry = 0;
            break;
          case 4:
            nentry = 16;
            break;
          case 5:
            nentry = 17;
            break;
          case 6:
            nentry = 18;
            break;
          case 7:
            nentry = 19;
            break;

          case 8:
            nentry = 7;
            break;
          case 9:
            nentry = 6;
            break;
          case 10:
            nentry = 5;
            break;
          case 11:
            nentry = 4;
            break;
          case 12:
            nentry = 20;
            break;
          case 13:
            nentry = 21;
            break;
          case 14:
            nentry = 22;
            break;
          case 15:
            nentry = 23;
            break;

          case 16:
            nentry = 11;
            break;
          case 17:
            nentry = 10;
            break;
          case 18:
            nentry = 9;
            break;
          case 19:
            nentry = 8;
            break;
          case 20:
            nentry = 24;
            break;
          case 21:
            nentry = 25;
            break;
          case 22:
            nentry = 26;
            break;
          case 23:
            nentry = 27;
            break;

          case 24:
            nentry = 15;
            break;
          case 25:
            nentry = 14;
            break;
          case 26:
            nentry = 13;
            break;
          case 27:
            nentry = 12;
            break;
          case 28:
            nentry = 28;
            break;
          case 29:
            nentry = 29;
            break;
          case 30:
            nentry = 30;
            break;
          case 31:
            nentry = 31;
            break;

          case 32:
            nentry = 63;
            break;
          case 33:
            nentry = 62;
            break;
          case 34:
            nentry = 61;
            break;
          case 35:
            nentry = 60;
            break;
          case 36:
            nentry = 47;
            break;
          case 37:
            nentry = 46;
            break;
          case 38:
            nentry = 45;
            break;
          case 39:
            nentry = 44;
            break;

          case 40:
            nentry = 59;
            break;
          case 41:
            nentry = 58;
            break;
          case 42:
            nentry = 57;
            break;
          case 43:
            nentry = 56;
            break;
          case 44:
            nentry = 43;
            break;
          case 45:
            nentry = 42;
            break;
          case 46:
            nentry = 41;
            break;
          case 47:
            nentry = 40;
            break;

          case 48:
            nentry = 55;
            break;
          case 49:
            nentry = 54;
            break;
          case 50:
            nentry = 53;
            break;
          case 51:
            nentry = 52;
            break;
          case 52:
            nentry = 36;
            break;
          case 53:
            nentry = 37;
            break;
          case 54:
            nentry = 38;
            break;
          case 55:
            nentry = 39;
            break;

          case 56:
            nentry = 51;
            break;
          case 57:
            nentry = 50;
            break;
          case 58:
            nentry = 49;
            break;
          case 59:
            nentry = 48;
            break;
          case 60:
            nentry = 32;
            break;
          case 61:
            nentry = 33;
            break;
          case 62:
            nentry = 34;
            break;
          case 63:
            nentry = 35;
            break;
          default:
            std::cout << "something wrong" << std::endl;
            break;
          }
        tree->GetEntry(nentry);
        h_map1->Fill(i,j,prop);
        charge[nentry] = prop;
      }
    }
  }
  h_map1->Draw("colz");
  h_map1->SetStats(0);
  filename = filename.ReplaceAll(".root","_2Dhist.pdf");
  c_map1->SaveAs(filename);
}

int main(int argc,char *argv[])
{
  GL840_root_to_2Dhist(argv[1]);
  return 0;
}
