# MC Labeling

Consumers of T2DS' RNTuples should be able to split candidates into **TrueSignal** / **Hybrid** / **RealBkg**. Those
three sets must be mutually exclusive, exhaustive, and mean the same thing for both antisexaquark and (anti)h-dibaryon
analyses.

These analyses are searches, so **TrueSignal** and **RealBkg** are the two sets that have to be exactly right.
**Hybrid** has too many shapes to be worth enumerating, and only becomes interesting once something has been found.
Hence: define **TrueSignal** and **RealBkg** positively, and let **Hybrid** be whatever is left.

## Fields

`SignalID` (= reaction id = injection id = `StatusCode`) is the unique index of the reaction / injection a particle
descends from, or `-1` when there is none or it isn't reachable in a couple of hops.

`SignalGeneration` is the depth in the (reaction) decay tree. Possible values:
- `-1` = no injected ancestor
- `0` = the injection itself: the injected (anti)h-dibaryon, or the antisexaquark reaction (which leaves no MC record)
- `1` = first generation: (anti)lambdas, K0S, charged kaons, etc.
- `2` = second generation: daughters of the above
- `3` = GEANT3 progeny, however many generations further down

`SignalGeneration >= 0` means has an injected ancestor; `SignalID >= 0` means that ancestor's id is also
**reachable**. The two differ exactly for GEANT3 progeny, whose id would take a walk up the mother chain that isn't
worth it (see Extra Details), so "descends from an injection" must always be asked of `SignalGeneration`, never of
`SignalID`. `SignalGeneration <= 2` then means is a genuinely relevant member of the reaction's own decay tree, as
opposed to GEANT3 re-interaction progeny. Both fields are sign-agnostic and independent of the PID hypothesis, and both
are derived from `Generator`, which is passed downstream through decays.

`IsTrue` is `true` when the PDG code matches the PID hypothesis.

`IsTrueSignal` means every daughter is a true daughter of one same injected id, under the PID hypothesis.

`IsSecondary` is `IsSecFromMat || IsSecFromWeak || SignalGeneration >= 1`.

`IsRealBkg` means carries no signal at all: neither the particle nor -- for a composite -- any of its constituents
descends from an injection. It is the stored form of the **RealBkg** rules below, and the only field a composite may ask
of its constituents, because a **Hybrid** composite with no common mother has a dummy record whose `SignalGeneration` is
`-1` even though it does carry signal through a constituent.

## Rules

For a single charged particle:

- **TrueSignal** -- `SignalID >= 0`, `SignalGeneration <= 2`, and `IsTrue`
- **RealBkg** -- no injected ancestor: `SignalGeneration == -1`
- **Hybrid** -- anything else: `!IsTrueSignal && !IsRealBkg`

For a composite built from constituents `c1..cn` (charged particles or composites):

- **TrueSignal** -- every `ci` is Signal, they are distinct mc particles, they share one `SignalID`, and together they
  are the true daughters of one same mother whose PDG also matches the composite's hypothesis. The antisexaquark
  reaction leaves no mc record, so for its candidates the shared `SignalID` stands in for that common mother.
- **RealBkg** -- no `ci` carries signal: `c1.IsRealBkg && ... && cn.IsRealBkg`. examples: a true common mother that
  carries no signal, no common mother at all, both daughters being the same background mc particle, and every
  wrong-sign or mixed combination of background, etc.
- **Hybrid** -- anything else: `!IsTrueSignal && !IsRealBkg`

## Field Bookkeeping

Independent of the rules above: fill every truth field that is reachable and that actually describes the candidate;
dummy only what is unreachable. A link that would misdescribe the candidate counts as unreachable, not as extra
information.

For single charged particles, when their `POD::McParticle` gets extended into `POD::Extended::McParticle`. Antisexaquark
MC only contains generators `0-2` and (anti)h-dibaryon MC only contains generators `3-4`. More info:
`docs/MC_PRODUCTIONS.md`.

| particle                                            | `Generator` | `Mother_McEntry`      | `StatusCode` | `SignalGeneration` | `SignalID`   | `IsSecondary`       | set under a hypothesis          |
|-----------------------------------------------------|-------------|-----------------------|--------------|--------------------|--------------|---------------------|---------------------------------|
| HIJING particle, at any depth                       |         `0` | any                   | any          |               `-1` | `-1`         | `IsSecFromMat/Weak` | RealBkg                         |
| injected antineutron, and its annihilation progeny  |         `1` | any                   | any          |               `-1` | `-1`         | `IsSecFromMat/Weak` | RealBkg                         |
| first gen. reaction product                         |         `2` | `-1`, by construction | `[600,620)`  |                `1` | `StatusCode` | true                | Signal if `IsTrue`, else Hybrid |
| its weak-decay daughter: (anti)proton, charged pion |         `2` | gen-1 product         | any          |                `2` | mother's     | true                | Signal if `IsTrue`, else Hybrid |
| anything deeper (GEANT3 progeny)                    |         `2` | any                   | any          |                `3` | `-1`         | true                | Hybrid                          |
| aux. primary protons/kaons/pions, and their progeny |         `3` | any                   | any          |               `-1` | `-1`         | `IsSecFromMat/Weak` | RealBkg                         |
| the injected (anti)H itself                         |         `4` | none (`-1`)           | `[400,500)`  |                `0` | `StatusCode` | false               | --                              |
| its (anti)lambda                                    |         `4` | injected (anti)H      | any          |                `1` | H's          | true                | Signal if `IsTrue`, else Hybrid |
| the (anti)lambda's (anti)proton/pions               |         `4` | (anti)lambda          | any          |                `2` | H's          | true                | Signal if `IsTrue`, else Hybrid |
| anything deeper (GEANT3 progeny)                    |         `4` | any                   | any          |                `3` | `-1`         | true                | Hybrid                          |

Composite particles are: V0s ((anti)lambdas and K0S, through their weak decays into their charged modes); antisexaquark
candidates (their 3 different channels under study); and (anti)h-dibaryon candidates.

The composites are formed with two daughters.

- A `TrueSignal` composite should have both daughters match their PDG codes, have both distincts `McEntry`, both share a
  single non-dummy `Mother_McEntry`, the mother's PDG must match the V0 hypothesis and have an `StatusCode` within
  signal range; and all particles have the same signal `Generator` (equal to `2` or `4` depending on the channel in
  study).

- A `RealBkg` composite should have all possibly-determined truth particles coming from a background `Generator`.

## Extra Details

- Two reconstructed tracks (different `EsdEntry`) can be split images of one mc particle (same `McEntry`). They fail the
  distinct mc particles clause, so they're never **TrueSignal**. Instead, they're **RealBkg** if that particle descends
  from no injection, and **Hybrid** if it does.

- The wrong-sign and mixed-channel candidates will never satisfy **TrueSignal**. However, they can be **Hybrid** or
  **RealBkg**, depending if any daughter downstream carries signal or not.

- PDG-matching is orthogonal to the split. A correctly reconstructed HIJING lambda is `IsTrue` and **RealBkg**.

- **Hybrid** single charged particles that come from GEANT3 progeny (`SignalGeneration > 2`), aren't worth extra
  computation effort or walks to extract their `SignalID`. For now.
