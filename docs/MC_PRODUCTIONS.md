# Dedicated MC Productions

Study of two six-quark (uuddss) hypothetical states and their (1) production in heavy-ion collisions (namely Pb-Pb at 5
TeV of center-of-mass energy) and (2) reconstruction in ALICE.

1. Compactly bound antisexaquarks, stable within certain mass range
2. Resonance-like (anti)h-dibaryons with fixed mass

## AntiSexaquark Reaction Simulations

- 20 antisexaquarks injected per event, per channel, per mass (`AliMCParticle::GeneratorIndex == 2`)
- background:
  - HIJING (`AliMCParticle::GeneratorIndex == 0`)
  - +100 injected anti-neutrons per event (`AliMCParticle::GeneratorIndex == 1`)
- reaction channels: (each correspond to a different production cycle)
  - A: antisexaquark + neutron -> anti-lambda + kaon zero short
  - D: antisexaquark + proton  -> anti-lambda + positive kaon
  - H: antisexaquark + proton  -> antiproton + positive kaon + positive kaon + pi0 (of interest: only kaons)
- antisexaquark properties:
  - invariant mass: 1.73, 1.8, 1.87, 1.94, 2.01 GeV/c^2 (each correspond to a different production cycle)
  - |rapidity| < 0.8 (uniform)
  - full phi range (uniform)
  - pt < 5 GeV/c (uniform)
- antisexaquark-material interaction:
  - forced to always happen
  - 5 < radius < 180 cm (uniform)
  - struck nucleon with Nuclear Fermi Motion present (simple gaussian on P)
    - note: unfortunately wrong model (gaussian mean = 0.25 GeV, gaussian std dev = 0.005)
- anchored to:
  - real data from Run 2 of LHC
    - data-taking periods: LHC15o (2015) and LHC18q+LHC18r (2018)
  - Pb-Pb collisions with center of mass energy = 5.02 TeV
- transport code: GEANT3
- production names:
  - LHC23l1a3
  - LHC23l1b3
  - subdirectories: all 15 possible combinations of <reaction channel><mass>
- total statistics: 1k x run numbers (369) x reaction channels (3) x sexaquark masses (5) ≈ 5.54 million events
- extra details:
  - both antisexaquark and the struck nucleons are not recorded in the MC stack; only their reaction ids and 3-momenta
    is stored in sim.log files (which are what the first program E2T collects and stores in POD::InjectedSexa)
  - note: the initial kinematics of the first gen. reaction products were generated with TGenPhaseSpace, but they are
    transported (including their decays) with GEANT3

## (Anti)H-dibaryon Simulations

- injected:
  - 50 h-dibaryons + 50 anti-h-dibaryons injected per event (`AliMCParticle::GeneratorIndex == 6`)
    - decay channels:
      - anti-h-dibaryon -> anti-lambda + anti-lambda -> anti-proton + pi plus
      - h-dibaryon -> lambda + lambda -> proton + pi minus
  - plus enough amount of primary (anti)protons, charged kaons and charged pions to have well-defined primary vertices
  (`AliMCParticle::GeneratorIndex == 0,1,2,3,4,5`)
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
- production name: LHC26h
- extra details:
  - unlike the antisexaquark simulations, every particle (including the h-dibaryons) were recorded in the mc stack
  - however, the lambdas were forced to decay and the protons and pions were adjusted to certain spin correlations,
    making them the only particles in the signal decay tree to be transported with GEANT3

## Prior By-Construction Knowledge

- these signal-dedicated simulations have injections in all the events; for any program, only checking the first event
  is enough to find production cycle information
- in any mc stack, the generator index is passed downstream through the particles' decays
- in E2T, `POD::McParticle::Generator` is a transformation from `AliMcParticle::GeneratorIndex`:
  - no transformation at all for antisexaquark mc: `0` for HIJING, `1` for antineutrons, `2` for antisexaquark
    reactions
  - for (anti)h-dibaryon mc: `3` if `GeneratorIndex < 6`; `4` if injected (anti)h-dibaryon (`GeneratorIndex == 6`)
- `AliMcParticle::MCStatusCode` doesn't get passed downstream. in E2T, it gets directly stored as
  `POD::McParticle::StatusCode`. for injected particles, this field is equivalent to the reaction/injection id that T2DS
  reads to form `SignalID`. however, as shown in `docs/*.txt`, no all injected particles carry it:
  - in the antisexaquark simulations, only the first generation signal products have `SignalID == StatusCode`
  - in the (anti)h-dibaryon simulations, only the (anti)H and first generation (anti)lambdas have
  `SignalID == StatusCode`
  this means: signal particles of posterior generations need to fill `SignalID` from the mothers' `StatusCode`
