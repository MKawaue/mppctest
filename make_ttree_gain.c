using namespace TMath;
using namespace std;

////////////FUNCTIONS////////////
void fill_hist(TTree *tree,int *Adc,TH1D *h,int i)//TH1D *hにint Adc[64]をFillする関数. iはforのカウンター(=ch number)
{
  //TH1D *h = new TH1D("h","ADC",80,760,920);
  int nEntry = tree->GetEntries();
  for(int iEntry = 0;iEntry<nEntry;iEntry++)
  {
    tree->GetEntry(iEntry);
    h->Fill(Adc[i]);
  }
  h->Draw();
}

void peak_search(TH1D *h,double *mean0,double *mean0_error,double *mean1,double *mean1_error)//TH1D *hを受け取って0p.e.と1p.e.のpeakをmean0,mean1に代入する
{
  //double mean0,mean1;
  double factor0,factor1;
  double sigma0,sigma1;
  double charge0,charge1;

  int max_peak = 10;
  double ini_mean[2];

  TF1 *f1 = new TF1("f1","[0]*Gaus(x,[1],[2])+[3]*Gaus(x,[4],[5])");
  f1->SetParLimits(0,100,2000);
  f1->SetParLimits(1,780,1000);
  f1->SetParLimits(2,0,100);
  f1->SetParLimits(3,100,2000);
  f1->SetParLimits(4,780,1000);
  f1->SetParLimits(5,0,100);

  TF1 *f2 = new TF1("f2","[0]*Gaus(x,[1],[2])+[3]*Gaus(x,[4],[5])");
  f2->SetParLimits(0,100,2000);
  f2->SetParLimits(1,780,1000);
  f2->SetParLimits(2,0,100);
  f2->SetParLimits(3,100,2000);
  f2->SetParLimits(4,780,1000);
  f2->SetParLimits(5,0,100);


  TSpectrum *s = new TSpectrum(max_peak);
  s->Search(h,1,"");
  //double *ini_mean;

  double *mean = s->GetPositionX();
  int N = s->GetNPeaks();
  //peakの最小値二つをini_mean[2]に入れたいだけだが・・・[0]が最小[1]が二番目
  for(int j=0;j<N;j++)
  {
    if(j==0)//１つ目は問題なし
    {
      ini_mean[0]=mean[0];
    }else if(j==1)//二つ目
    {
      if(ini_mean[0]<mean[1])//２つ目の方が大きければそのまま
      {
        ini_mean[1]=mean[1];
      }else//さもなくば、ini_meanの順番を逆にして入れる
      {
        ini_mean[1]=ini_mean[0];
        ini_mean[0]=mean[1];
      }
    }else//残り
    {
      if(mean[j]<ini_mean[1])//二番目の最小値を更新するか？
      {
        if(mean[j]<ini_mean[0])//さらに最小値を更新するか？
        {
          ini_mean[1]=ini_mean[0];
          ini_mean[0]=mean[j];
        }else
        {
          ini_mean[1]=mean[j];
        }
      }
    }
  }

  f1->SetParameter(0,1000);
  f1->SetParameter(1,ini_mean[0]);//mean
  f1->SetParameter(2,10);//sigma
  f1->SetParameter(3,1000);
  f1->SetParameter(4,ini_mean[1]);//mean
  f1->SetParameter(5,10);//sigma

  gStyle->SetOptFit(1111);
  h->Fit("f1","Q","",ini_mean[0]-20,ini_mean[1]+10);
  //cout << ini_mean[0]-20 << "kara" << ini_mean[1]+10 << "made" << endl;
  //h->Fit("f1","","",760,920);
  //f1->SetRange(760,920);
  //f1->Draw("same");
  //h->Reset();

  factor0 = f1->GetParameter(0);
  *mean0 = f1->GetParameter(1);
  sigma0 = f1->GetParameter(2);
  factor1 = f1->GetParameter(3);
  *mean1 = f1->GetParameter(4);
  sigma1 = f1->GetParameter(5);


  f2->SetParameter(0,factor0);
  f2->SetParameter(1,*mean0);
  f2->SetParameter(2,sigma0);
  f2->SetParameter(3,factor1);
  f2->SetParameter(4,*mean1);
  f2->SetParameter(5,sigma1);

  h->Fit(f2,"Q","",*mean0-3.*sigma0,*mean1+1.*sigma1);
/*
  cout << "mean0=" << *mean0 << ", sigma0=" << sigma0 << endl;
  cout << "mean1=" << *mean1 << ", sigma1=" << sigma1 << endl;
  cout << *mean0-1.*sigma0 << "kara" << *mean1+1.*sigma1 << "made" << endl;
*/
  *mean0 = f2->GetParameter(1);
  *mean0_error = f2->GetParError(1);
  //sigma0 = f2->GetParameter(2);
  *mean1 = f2->GetParameter(4);
  *mean1_error = f2->GetParError(4);
  //sigma1 = f2->GetParameter(5);

  //h->Fit(f1,"Q","",*mean0-1.*sigma0,*mean1+1.*sigma1);
  //cout << *mean0 << "to" << *mean1 << endl;
  //f1->Draw("same");
}


////////////MAIN FUNCTION////////////
int make_ttree_gain(TString input_file_name)
{
  //output
  double gain[64];
  double gain_error[64];
  double HV;
  TString output_file_name = "../rootfile/" + input_file_name + "_gain.root";
  TFile *outfile = new TFile(output_file_name,"recreate");
  TTree *tree = new TTree("tree","tree");
  tree->Branch("gain",gain,"gain[64]/D");
  tree->Branch("gain_error",gain_error,"gain_error[64]/D");
  tree->Branch("HV",&HV,"HV/D");

  TCanvas *c[5];

  //58,58.5,59,59.5,60の5点で測定
  for(int iHV=0;iHV<5;iHV++)
  {
  //input
  //int hv = 580+5*iHV;
  //TString file_name;
  //sprintf(file_name,"data/setHV%d.root",hv);
  TString file_name = "../rootfile/" + input_file_name +Form("%d_tree.root",580+5*iHV);
  /*
    if(iHV==0)
      {
        file_name = "../rootfile/setHV580_tree.root";
      }else if(iHV==1)
      {
	       file_name = "../rootfile/setHV585_tree.root";
      }else if(iHV==2)
      {
	       file_name = "../rootfile/setHV590_tree.root";
      }else if(iHV==3)
      {
	       file_name = "../rootfile/setHV595_tree.root";
      }else if(iHV==4)
      {
	       file_name = "../rootfile/setHV600_tree.root";
      }
      */

  TFile *file = new TFile(file_name,"read");
  TTree *t = (TTree*)file->Get("tree");

  //output
/*
  double gain[64];
  double HV;
  file_name.ReplaceAll(".root","_gain.root");
  file_name.ReplaceAll("data","rootfile");
  TFile *outfile = new TFile(file_name,"recreate");
  TTree *tree = new TTree("tree","tree");
  tree->Branch("gain",gain,"gain[64]/D");
  tree->Branch("HV",&HV,"HV/D");
*/

  HV=58+0.5*iHV;

  //canvas
  c[iHV] = new TCanvas(Form("c%d",iHV),Form("c%d",iHV),1920,1080);
  c[iHV]->Divide(5,5);

  //variable
  double mean0,mean1;
  double mean0_error,mean1_error;
  //double ini_mean[2];

  int Adc[64] = {0};
  t->SetBranchAddress("adc",Adc);
  //int nEntry = t->GetEntries();
  //TH1F *h = new TH1F("h","ADC",100,700,900);
  TH1D *h[64];

  for(int i = 33;i < 64;i++)
  {
    if(i<48)//前半
    {
      c[iHV]->cd(i-32);

      //TH1D *h = new TH1D("h","ADC",80,760,920);
      h[i] = new TH1D(Form("h%d",i),Form("h%d",i),80,760,920);

      fill_hist(t,Adc,h[i],i);

      peak_search(h[i],&mean0,&mean0_error,&mean1,&mean1_error);

      gain[i] = mean1-mean0;
      gain_error[i] = sqrt(mean0_error*mean0_error+mean1_error*mean1_error);

    }else if(i > 53)//後半
    {
      c[iHV]->cd(i-38);
      h[i] = new TH1D(Form("h%d",i),Form("h%d",i),80,760,920);

      fill_hist(t,Adc,h[i],i);
      peak_search(h[i],&mean0,&mean0_error,&mean1,&mean1_error);

      gain[i] = mean1-mean0;
      gain_error[i] = sqrt(mean0_error*mean0_error+mean1_error*mean1_error);
    }
  }
  tree->Fill();
}
  //TFile *outfile = new TFile(file_name,"recreate");
  outfile->cd();
  tree->Write();

  return 0;
}
