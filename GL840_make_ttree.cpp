#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <TString.h>
#include <TFile.h>
#include <TTree.h>

  void GL840_make_ttree(TString filename){
  //constant
  const int nsample = 60;//101;
  //const
  int nch = 64;//20;
  //constants for histogram
  const int nbin = 30;
  const double bin_min = -1.0;
  const double bin_max = 1.0;

  //global variable
  double voltage[nch];
  double value;
  std::cout << "FILENAME : " << filename << std::endl;
  std::ifstream data(filename);

  TString rootfile_name = filename.ReplaceAll(".txt",".root");
  TFile *rootfile = new TFile(rootfile_name,"recreate");
  TTree * tree = new TTree("tree","tree");

  tree->Branch("nch",&nch,"nch/I");
  tree->Branch("voltage",voltage,"voltage[nch]/D");

  std::string line;
  int j = 0;
  while(std::getline(data,line))
  {
    std::cout << "preLINE : " << line << std::endl;
    std::string tmp_str;
    std::istringstream line_stream(line);
    std::cout << "LINE : " << line << std::endl;
    int i = 0;
    while(std::getline(line_stream,tmp_str,' '))
    {
      voltage[i] = std::stod(tmp_str);
      std::cout << "ch" << i << " : " << voltage[i] << std::endl;
      i++;
    }
    tree->Fill();
    j++;
    std::cout << "entry : " << j << std::endl;
  }
  std::cout << "Before Tree Write" << std::endl;
  tree->Write();
  std::cout << "After Tree Write" << std::endl;
  //return 0;
  rootfile->Close();
}

  int main(int argc,char *argv[]){
  GL840_make_ttree(argv[1]);
  std::cout << "Return to main" << std::endl;
  return 0;
  //std::cout << "The last line" << std::endl;
}
