# gnirscc: Linux cross-build + gemini-rtsw-ci (REL-5050)

Plan for doing to `gnirscc` what REL-4693 did to `gmoscc` and the hrwfs work
did to `hrwfs`. The procedure is `gmoscc/docs/README-LINUX-REHOST.md`; the
worked examples are `gmoscc/tools/linux-build/` and `hrwfs/tools/linux-build/`
(with `hrwfs/MIGRATION-PLAN.md` for everything learned the hard way).

---

## 0. The headline: a third EPICS generation and a different CPU

| | gmoscc | hrwfs | **gnirscc** |
|---|---|---|---|
| GEM tree | GEM8.6 | GEM7 | **GEM5** |
| EPICS | 3.13.9 | 3.13.4 | **3.12.2** (`epics3.12.2Gem5_26May99`, per `gnirsdc/setup.gnaac`) |
| target | `ppc604_long` | `ppc604` | **`mv167` -- MC68040** |
| Tornado / gcc | 2.2 / 2.96 | 2.0.2 / cygnus-2.7.2 | **unknown** |
| Linux cross-gcc | ANL tor2_2 ppc | ANL tor2_2 ppc | ANL publishes a **68k** build, but for **Tornado 2.2** only |
| setup | `applSetup.pl` | `applSetup.pl` | **shell `applSetup`**, `CONFIG_APPLIC` style, `config/` committed |
| record defs | `gemini.dbd` | `gemini.dbd` | **`default.dctsdr`** (3.12 binary SDR) |
| deplibs | astlib slalib timelib | + cfitsio, DHS | astlib slalib timelib |
| dependency RPMs | done | done | **none** -- no gem5 / 68k packages exist |

Nothing built for gmoscc or hrwfs is reusable except the procedure, the
pipeline and the lessons. gnirsdc is also GEM5/mv167, so every GEM5 dependency
RPM built here serves it too.

## 1. The two questions that size the project

### 1a. Can the EPICS 3.12.2 host tools build for Linux?

The 3.13 ports worked because 3.13 ships `CONFIG.Host.Linux` /
`CONFIG_HOST_ARCH.Linux`. I believe 3.12 has no Linux host arch. If so, the
host side (`applSetup`, the SDR/dct tools that produce `default.dctsdr`,
`snc`, the config rules) must be ported -- the biggest unknown, and possibly
weeks rather than days. The inventory answers whether the tree even carries
`base/src`.

Fallback if it is too expensive: hrwfs's **option D** -- everything except the
compile moves to Linux/CI (history, specs, dep RPMs, payload, deploy story);
the mv167 objects keep coming from polaris.

### 1b. Which compiler, against which vxWorks?

ANL's 68k gcc 2.96 is built for Tornado 2.2 / vxWorks 5.5. If GEM5's crate
runs 5.3.x (likely for a 1999 tree) the gap is wider than hrwfs's 2.7.2 ->
2.96, which hrwfs validated as benign *on PowerPC*. Options mirror hrwfs §0b:

- **A.** Rebuild the exact compiler from GPL sources (a 2002 tech-talk thread
  built gcc 2.7.2.3 `m68k-wrs-vxworks` and EPICS 3.13 for mv167/vxWorks 5.3).
- **B.** ANL 68k gcc 2.96 against the GEM5 Tornado headers, validated by object
  comparison and a functional crate test. Redo the hrwfs ABI audit for **68k**:
  struct return, bitfields, `long long`, alignment (68k aligns to 2, not 4).
- **D.** As above.

## 2. Known gnirscc-specific problems (from the repo alone)

1. **The startup scripts in `startup/` are not what production boots.** They
   `cd "/home/gnirs"`, load from `/home/gnirs/...` and
   `epics/base/bin/mv167` (a lab layout); `epicsBoot` is a Tucson lab crate
   (`hamster`). `startup.CC.epics.vws` loads `mytimeLib.o` (**no source in the
   repo**) and `ccTop.db`/`nirsCcSadTop.db` (repo has `nirsCCTop.db`/
   `nirsCCSadTop.db`). The deployed tree and the crate's boot parameters are the
   only truth.
2. **`applSetup` overwrites startup files.** `nirsSetup` already stashes
   `local.vws`; check whether 3.12's `applSetup` also clobbers `resource.def`
   and `UAE.dist` (it did in GEM7/GEM8.4 -- hrwfs Status 2026-09-15).
3. **`local.vws` still mounts `pisces-control:/export/gemini`.** Needs the same
   move to `mkotcsbootv2-lv1` gmoscc and hrwfs made -- coordinate with the
   `nfsv2-bootserver` exports/rhosts.
4. **Live-tuned config files.** `pv/` and `data/` (`gnirsMechanisms`,
   `gnirsConfig`, `mechanisms.pv`, `*.lut`, dated backups) are edited on the
   deployed tree and back-ported (PFTWGN-820, REL-4895). An RPM that owns them
   overwrites tuning on install and on `dnf downgrade`. **Decision needed:**
   `%config(noreplace)`, a separate `gnirscc-data` package, or keep them out of
   the RPM.
5. **Capfast is dead but its outputs are committed.** `capfast/Makefile.Unix`
   builds `nirsCCTop.db nirsCCSadTop.db temperatureControl.db` via `e2sr`; all
   three are in `data/`. Seed them so make never invokes the schematic tools.
6. **Versioned deploy dir** `/gemini/GEM5/gnirs/CC/<release>`, plus a manual
   `deploy.sh`/rdist step that already caused one overwrite incident. Move to
   a fixed path (as gmoscc/hrwfs did) only once the boot parameters are known.
7. **History.** GitHub starts at a 2025-12-29 "Initial commit"; the CVS history
   (V1-0..V1-26) is not in git. Optional, via the gnirsdc cvs2git route.
8. **Repo cruft** that should not ship: `data.NOW`, `*.tar.gz`/`*.tar.Z` in
   `sys/`, `hdwrControl/{Former,Roddier,keep,*.py}`, `include/gnirsCC.peter.h`.

## 3. Pipeline cautions (gemini-rtsw-ci / gemini-rtsw-repo)

- `el_version` defaults to 8: matrix `el: ['9']`, local scripts `--el 9`.
- `build_rpm.sh:199` substitutes the literal `gis_mk` for `%{name}` when there
  is no `%define name`. Use `%define name gnirscc` (as hrwfs does).
- Macro-derived package names (`gem5-slalib-%{libdir}`) need hrwfs's
  `docs/gemini-rtsw-ci-macro-name.patch`, not yet on ci `main`.
- Generation-specific names and paths for every new dep RPM
  (`gem-epics3122gem5`, `gem5-epics-runtime`, `gem5-<lib>-V..`) so they
  co-install with the gem7/gem84/gem86 sets.
- `gem-vxworks-tornado20` already owns `tornado2.0/mv167/{vxWorks,vxWorks.sym,
  debug}` (as symlinks). If GNIRS boots from there, extend that package --
  a second owner is a dnf conflict on the boot server.
- Publishing: `upload-rpm.sh --tag-only`, then `rebuild-latest` in
  gemini-rtsw-repo on a runner; grant this repo Write on `rpm-repo`.
- The 32-bit cross-tools need `glibc.i686` matching x86_64 glibc
  (`custom-repo-setup.sh`), and a build tree on a filesystem with 32-bit inode
  numbers (hrwfs §0b: `EOVERFLOW` reported against a header).

## 4. Work plan

### Phase 1 -- inventory (blocks everything)
- [ ] `tools/linux-build/stage-buildenv.sh inventory` on polaris under `GEM5`
- [ ] `alias GEM5` and the files it sources
- [ ] deployed tree + crate boot parameters (see §5)
- [ ] decide 1a and 1b

### Phase 2 -- environment
- [ ] `buildenv` / `runtime` / `deployed` tarballs, escrowed outside the repo
      (`~/work/gnirscc-buildenv`)
- [ ] toolchain experiment: compile `sys/*` with the chosen 68k gcc against the
      GEM5 headers (hrwfs §0b method)
- [ ] host tools for `HOST_ARCH=Linux` -- build or port
- [ ] `tools/linux-build/setup.sh` + zero-setup `Makefile`, as in hrwfs

### Phase 3 -- packaging and pipeline
- [ ] dep RPMs: `gem-tornado<ver>-linux` (68k), `gem-epics3122gem5`,
      `gem5-epics-runtime`, `gem5-{astlib,slalib,timelib}`, kernel
- [ ] `gnirscc.spec` (with `%package devel`), submodule, `.github/workflows/ci.yml`
- [ ] data-file ownership decision (§2.4)

### Phase 4 -- validation
- [ ] object comparison against a polaris build of the same commit
      (per function; strip debug/.comment)
- [ ] `rpm -ql` vs the deployed tree, file for file
- [ ] functional crate test: init, datum and move each mechanism, temperature
      control, CAD/CAR paths

## 5. Needed from polaris / production

1. `sh stage-buildenv.sh inventory` output, run **after** `GEM5` in the same
   csh session (save to a file and send it back).
2. `alias GEM5`, and every file it sources (e.g. `~/.gem5`, `epics.csh`).
3. The deployed tree: `ls -la /gemini/GEM5/gnirs/CC` (which release is live,
   any selector symlink), then `stage-buildenv.sh deployed <that path>`.
4. The CC crate's **boot parameters** (`p` at the vxWorks boot prompt, or
   `bootParamsShow` from the shell) -- which kernel and which startup script.
5. Then, once 1-4 are reviewed: `buildenv` and `runtime` tarballs (sizes are
   in the inventory -- check `/var/tmp` space first).
