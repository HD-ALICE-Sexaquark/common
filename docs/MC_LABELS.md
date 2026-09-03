# MC Labeling

Consumers of T2DS' RNTuples should be able to split candidates into **TrueSignal** / **RealBkg** / **Hybrid**. Those
three sets must be mutually exclusive, exhaustive, and mean the same thing for both antisexaquark and (anti)h-dibaryon
analyses.

These analyses are searches, so **TrueSignal** and **RealBkg** are the two sets that have to be exactly right.
**Hybrid** has too many shapes to be worth enumerating, and only becomes interesting once something has been found.
Hence: define **TrueSignal** and **RealBkg** positively, and let **Hybrid** be whatever is left.

Orthogonal to the split, `GeneratorMask` represents the generator of origin. It answers if a candidate is formed from
constituents coming from the different generators.

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

`GeneratorMask` is the bitwise OR of `1 << Generator` over the candidate's charged leaves (and over its own record, when
one describes it). `0` means nothing described the candidate: it only happens on a default-constructed POD, never on a
linked composite, whose constituents always contribute at least one bit each.

Unlike `IsTrue` and `IsTrueSignal`, `GeneratorMask` does **not** depend on the PID hypothesis the candidate was built
with. It is a property of the particles, not of the assumption made about them.

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
  carries no signal, no common mother at all, both daughters being the same background mc particle, and every wrong-sign
  or mixed combination of background, etc.
- **Hybrid** -- anything else: `!IsTrueSignal && !IsRealBkg`

## Generator of Origin

Each charged leaf contributes one bit, `1 << Generator`; a candidate's `GeneratorMask` is the OR of them. Storing the
five raw generator bits rather than a collapsed triple costs the same byte and keeps antineutron progeny distinguishable
from auxiliary primaries, so the three classes below stay derived.

| class                   | letter | generators                                             |
|-------------------------|--------|--------------------------------------------------------|
| realistic event         |   H    | HIJING (`0`)                                           |
| injected background     |   N    | injected antineutrons (`1`), auxiliary primaries (`3`) |
| injected signal         |   S    | antisexaquark reactions (`2`), (anti)h-dibaryons (`4`) |

Note that the two productions do not populate the same classes: antisexaquark mc has H and N, (anti)h-dibaryon mc has
N but no HIJING at all.

`MC::Origin` (in `MC_Helpers.hpp`) is the predicate set consumers filter with: `CarriesHIJING` / `CarriesInjectedBkg` /
`CarriesSignal` ask whether the mask intersects a group, their `Only*` counterparts ask whether it is a non-empty subset
of one, and `Classes` collapses the mask to the compact H|N|S composition code -- its seven non-empty values are the
seven possible mixtures, ready to histogram or to switch on. `Cached::` exposes the same set as scalar getters on
`Cached::McParticle` and `Cached::InjectedSexa`.

That composition code is its own type, `MC::Origin::EClass`, and not interchangeable with a `GeneratorMask`: their low
values overlap numerically (a mask's `4` is the antisexaquark reaction, a class code's `4` is "carries signal"), so
feeding one where the other is expected would be silently wrong rather than a compile error.

Two relations tie the mask back to the three-way split. Both are worth knowing, and neither is obvious:

- `IsRealBkg` is exactly `!MC::Origin::CarriesSignal(GeneratorMask)`, at every level. At a leaf, `MC::Classify` returns
  `generation == kNone` if and only if `Generator` is neither `2` nor `4`, i.e. if and only if the mask carries no S
  bit; composites combine `IsRealBkg` with `&&` and masks with `|`, which is the same statement under De Morgan.
  The two fields are two views of one fact and must not be allowed to drift.
- `IsTrueSignal` implies `OnlySignal(GeneratorMask)`, but **not** the converse. All-S yet not TrueSignal happens for:
  two constituents from two *different* injections, a constituent reconstructed under the wrong PDG hypothesis, GEANT3
  re-interaction progeny of a reaction product, and split images of one mc particle.

Together they say that **Hybrid** is exactly "carries an S bit but is not TrueSignal".

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

For every row of that table, `GeneratorMask` is `1 << Generator`.

Composite particles are: V0s ((anti)lambdas and K0S, through their weak decays into their charged modes); antisexaquark
candidates (their 3 different channels under study); and (anti)h-dibaryon candidates.

The composites are formed with two daughters.

- A `TrueSignal` composite should have both daughters match their PDG codes, have both distincts `McEntry`, both share a
  single non-dummy `Mother_McEntry`, the mother's PDG must match the V0 hypothesis and have an `StatusCode` within
  signal range; and all particles have the same signal `Generator` (equal to `2` or `4` depending on the channel in
  study).

- A `RealBkg` composite should have all possibly-determined truth particles coming from a background `Generator`.

- Every composite ORs its constituents' `GeneratorMask` into its own.

## Extra Details

- Two reconstructed tracks (different `EsdEntry`) can be split images of one mc particle (same `McEntry`). They fail the
  distinct mc particles clause, so they're never **TrueSignal**. Instead, they're **RealBkg** if that particle descends
  from no injection, and **Hybrid** if it does.

- The wrong-sign and mixed-channel candidates will never satisfy **TrueSignal**. However, they can be **Hybrid** or
  **RealBkg**, depending if any daughter downstream carries signal or not.

- PDG-matching is orthogonal to the split. A correctly reconstructed HIJING lambda is `IsTrue` and **RealBkg**.

- **Hybrid** single charged particles that come from GEANT3 progeny (`SignalGeneration > 2`), aren't worth extra
  computation effort or walks to extract their `SignalID`. For now.
