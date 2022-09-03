// A run of PYTHIA8 for soft events with MPI.
// Output: dN/dEta.

// Stdlib header file for input and output.
#include <iostream>

#include "Pythia8/Pythia.h"

// ROOT, for histogramming.
#include "TH1.h"

// ROOT, for interactive graphics.
#include "TVirtualPad.h"
#include "TApplication.h"

// ROOT, for saving file.
#include "TFile.h"

using namespace Pythia8;

int main(int argc, char* argv[]) {

  // Create the ROOT application environment.
  TApplication theApp("hist", &argc, argv);

  // Generator.
  Pythia pythia_w_mpi;
  Pythia pythia_wo_mpi;

  // Generation parameters.
  int nEvents = 1000;

  // Process selection.
  // with mpi.
  pythia_w_mpi.readString("Beams:idA = 2212");
  pythia_w_mpi.readString("Beams:idB = 2212");
  pythia_w_mpi.readString("Beams:eCM = 7000.");
  pythia_w_mpi.readString("SoftQCD:all = on");
  pythia_w_mpi.readString("PartonLevel:all = on"); // default = on.
  pythia_w_mpi.readString("PartonLevel:MPI = on"); // Switch on MPI. default = on.

  // without mpi.
  pythia_wo_mpi.readString("Beams:idA = 2212");
  pythia_wo_mpi.readString("Beams:idB = 2212");
  pythia_wo_mpi.readString("Beams:eCM = 7000.");
  pythia_wo_mpi.readString("SoftQCD:all = on");
  pythia_wo_mpi.readString("PartonLevel:all = on"); // default = on.
  pythia_wo_mpi.readString("PartonLevel:MPI = off"); // Switch off MPI. default = on.

  // Initialization.
  pythia_w_mpi.init();
  pythia_wo_mpi.init();

  // Create file on which histogram(s) can be saved.
  TFile* outFile = new TFile("soft_w_o_mpi.root", "recreate");

  // Book histogram(s).
  TH1F* eta_w_mpi = new TH1F("eta_w_mpi", "dN/d#eta (w_mpi)", 100, -5., 5.);
  TH1F* eta_wo_mpi = new TH1F("eta_wo_mpi", "dN/d#eta (wo_mpi)", 100, -5., 5.);

  // Begin event loop. Generate event; skip if generation aborted.
  // with mpi
  for(int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if(!pythia_w_mpi.next()) continue;

    // Particle loop.
    for(int i = 0; i < pythia_w_mpi.event.size(); ++i) {
      if(pythia_w_mpi.event[i].isFinal() && pythia_w_mpi.event[i].isCharged()) {
        eta_w_mpi->Fill(pythia_w_mpi.event[i].eta());
      }
    }
    // End of event loop.
  }

  // without mpi
  for(int iEvent = 0; iEvent < nEvents; ++iEvent) {
    if(!pythia_wo_mpi.next()) continue;

    // Particle loop.
    for(int i = 0; i < pythia_wo_mpi.event.size(); ++i) {
      if(pythia_wo_mpi.event[i].isFinal() && pythia_wo_mpi.event[i].isCharged()) {
        eta_wo_mpi->Fill(pythia_wo_mpi.event[i].eta());
      }
    }
    // End of event loop.
  }

  // Statistics on event generation.
  pythia_w_mpi.stat();
  pythia_wo_mpi.stat();

  // Save histogram(s) on file.
  eta_w_mpi->Write();
  eta_wo_mpi->Write();

  // Close file.
  delete outFile;

  // Done.
  return 0;
}
