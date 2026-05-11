# GNIRS CC

Crate-controller source tree for GNIRS. Builds VxWorks `mv167` binaries that
get deployed to `/gemini/GEM5/gnirs/CC/<release>` for use by the instrument.

## Layout

- `nirs.env` — self-contained build environment. Sets `NIRSCC_CVSRELEASE`,
  `NIRS_CCDIR`, `NIRS_GEM5_INSTPATH`, then sources `nirsSetup CC`.
- `Makefile`, `Makefile.subdirs` — top-level build rules.
- `src/`, `pv/`, `capfast/`, `dl/`, `db/`, `startup/`, `config/` — source.
- `tools/deploy.sh` — safe rdist wrapper (use this, **not** raw `rdist`).
- `Distfile` — generated; rdist input consumed by `deploy.sh`.

## Build hosts

Build on **polaris**, deploy from **pisces**. `/gemini` is NFS-mounted the
same way on both, so the destination is reachable from either, but the
established workflow keeps the source tree on polaris and runs the deploy
from pisces.

## One-time setup

Pick a release name (e.g. `V1-27`) and check the tree out under your
home directory:

```sh
# on your workstation
rsync -avz gnirscc polaris-tunnel:/home/gemvx/<user>/gnirscc-git-V1-27
```

If you're cutting a new release, bump `NIRSCC_CVSRELEASE` at the top of
`nirs.env` to match the directory name.

## Build

On polaris, from the tree root:

```sh
cd ~/gnirscc-git-V1-27
. ./nirs.env        # sources env, then runs `sh -x ./nirsSetup CC`
make                # full build (mv167 target)
make Distfile       # generate the rdist Distfile
```

`nirs.env` auto-detects `NIRS_CCDIR` from its own location, so the tree
can live anywhere under your home directory.

## Deploy

On pisces, from the same tree:

```sh
cd ~/gnirscc-git-V1-27
cat Distfile        # sanity-check src and dst before deploying
./tools/deploy.sh
```

`deploy.sh` parses the Distfile, prints the source and destination, and
requires you to type the version directory name (e.g. `V1-27`) verbatim
to confirm. **Never run `rdist -f Distfile` by hand** — a stale Distfile
once overwrote V1-21 with a V1-24 build and deleted ~220 dated backups
before it was caught. The wrapper exists to prevent a repeat.

## Releases

Release notes live in `RELEASE.NOTES`. After deploy, the install path is:

```
/gemini/GEM5/gnirs/CC/<NIRSCC_CVSRELEASE>
```
