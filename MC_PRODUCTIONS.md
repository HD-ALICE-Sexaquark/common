# Dedicated MC Productions

Study of two six-quark (uuddss) hypothetical states and their (1) production in heavy-ion collisions (namely Pb-Pb at 5
TeV of center-of-mass energy) and (2) reconstruction in ALICE.
1. Compactly bound anti-sexaquarks, stable within certain mass range
2. Resonance-like (anti)h-dibaryons with fixed mass

## Sexaquark Simulations

- 20 anti-sexaquarks injected per event, per channel, per mass
- background:
  - HIJING
  - +100 injected anti-neutrons per event
- reaction channels: (each correspond to a different production cycle)
  - A: anti-sexaquark + neutron -> anti-lambda + kaon zero short
  - D: anti-sexaquark + proton  -> anti-lambda + positive kaon
  - E: anti-sexaquark + proton  -> positive kaon + positive kaon + X
- anti-sexaquark properties:
  - invariant mass: 1.73, 1.8, 1.87, 1.94, 2.01 GeV/c^2 (each correspond to a different production cycle)
  - |rapidity| < 0.8 (uniform)
  - full phi range (uniform)
  - pt < 5 GeV/c (uniform)
- anti-sexaquark-material interaction:
  - forced to always happen
  - 5 < radius < 180 cm (uniform)
  - struck nucleon with Nuclear Fermi Motion present (simple gaussian on P)
    - note: unfortunately wrong model (gaussian mean = 0.25 GeV, gaussian std dev = 0.005)
- anchored to:
  - real data from Run 2 of LHC
    - data-taking periods: LHC15o (2015) and LHC18q+LHC18r (2018)
  - Pb-Pb collisions with center of mass energy = 5.02 TeV
- transport code: GEANT3
  - note: the initial kinematics of the reaction products were generated with TGenPhaseSpace, but they are transported
  with GEANT3 (i.e. (anti)lambdas and K0S decay channels are not controlled)
- production names:
  - LHC23l1a3
  - LHC23l1b3
  - subdirectories: all 15 possible combinations of <reaction channel><mass>
- total statistics: 1k x run numbers (369) x reaction channels (3) x sexaquark masses (5) ≈ 5.54 million events

## H-dibaryon Simulations

- injected:
  - 50 h-dibaryons + 50 anti-h-dibaryons injected per event
    - decay channels:
      - anti-h-dibaryon -> anti-lambda + anti-lambda -> anti-proton + pi plus
      - h-dibaryon -> lambda + lambda -> proton + pi minus
      - always forced, with signal (anti)protons obeying certain spin correlations
  - plus enough amount of primary protons, charged kaons and charged pions to have well-defined primary vertices
  - i.e. essentially signal-only mc
- (anti)h-dibaryons properties:
  - invariant mass: 2.234 GeV/c^2
  - gamma: 0.002 GeV (attempt to have it resonance-like)
  - |rapidity| < 0.8 (uniform)
  - full phi range (uniform)
  - pt according to blast-wave model assuming random centralities
- anchored to:
  - real data from Run 2 of LHC
    - data-taking periods: LHC15o (2015) and LHC18q+LHC18r (2018)
  - Pb-Pb collisions with center of mass energy = 5.02 TeV
- transport code: GEANT3
  - effective only on final state particles (i.e. protons, charged kaons and charged pions)
- production name: LHC26h
