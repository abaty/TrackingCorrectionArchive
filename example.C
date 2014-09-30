#include "PbPb_track_correction.h"
#include <iostream>
#include "HiForest/HiForestAnalysis/hiForest.h"


void example()
{
  //intializeCorrections loads the histograms with the corrections in them
  //should be called before using the corrections
  initializeCorrections();

/*
  Pt should be above 0.5.
  Eta should have |eta|<2.4.
  Centrality given using bins of 0.5% (200 hiBins total) i.e. 6 correspondes to 2.5-3%.
  Rmin is defined as the distance to the nearest jet with pt>50 GeV/c and |eta|<2.
  It is calculated with sqrt(dphi^2+deta^2) between the track and the jet.
  The function calculates this for you, but you have to feed it arrays with the jetPt, jetEta, jetPhi, and number of jets (nref) for each event.

  getCorrection gives the total correction with Fake and Efficiency acounted for.
  required parameters are:

  track pt
  track eta
  track phi
  centrality bin
  an array of the pt of all jets in the event
  an array of the eta of all jets in the event
  an array of the phi of all jets in the event
  the number of jets in the event

*/
 HiForest *  h = new HiForest("/mnt/hadoop/cms/store/user/velicanu/PbPbForest_MatchEqR_Calo_HIHighPt_HIRun2011-14Mar2014-v4-reclean/1.root","forest");
 h->LoadNoTrees();
 h->hasTrackTree = true;
 h->hasEvtTree = true;
 h->hasAkVs3CaloJetTree = true;


 int nEntries = h->GetEntries();
//looping over events
for(int i=0; i<nEntries; i++)
  {
    h->GetEntry(i);

    //only for this data file, to avoid some empty events
    if(h->track.nTrk == 0) continue;

    double cent = h->evt.hiBin;
    
    //track loop
    for( int j=0; j<20; j++)
    { 
      double pt = h->track.trkPt[j];
      double eta = h->track.trkEta[j];
      double phi = h->track.trkPhi[j];

      //correction is grabbed here
      double corr = getCorrection(pt,eta,phi,cent,h->akVs3Calo.jtpt,h->akVs3Calo.jteta,h->akVs3Calo.jtphi,h->akVs3Calo.nref);
      std::cout << corr << "\n" << std::endl;
    }
  }
}
