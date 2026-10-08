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

## 0b. Inventory results (polaris, 2026-10-08)

From `stage-buildenv.sh inventory`, run under `GEM5` (= `source ~/.gem5`).
The run produced 892 lines; it is kept untracked at
`tools/linux-build/gnirscc-inventory.txt`.

**It is older than assumed: pre-Tornado vxWorks 5.2b and a 1993 compiler.**

| | measured |
|---|---|
| host | SunOS 5.8, sun4u (Ultra-250) |
| EPICS | `/usr/software/dev/packages/epics/epics3.12.2GEM5`, `epicsVersion.h` = **R3.12.2GEM5** (update level 5) |
| vxWorks | **`VX_DIR=/usr/software/dev/packages/vxworks/v5.2b`**. No `WIND_BASE`, so there is **no Tornado at all** |
| compiler | `cc68k` = **`gcc version cygnus-2.2.3.1`** (`.../v5.2b/gnu/solaris.68k`, a SPARC ELF binary) |
| object format | **a.out**: `file` says `impure mc68020 executable`, not ELF. So the script's `strings \| grep GCC:` finds nothing, because a.out has no `.comment` section |
| host arches in base/bin | `solaris`, `hkbaja47`, `mv147`, `mv167`. **No Linux** |
| EPICS source | present: `base/src` 27 MB, `extensions/src` 89 MB, plus the original `R3.12.2GEM5{base,ext}.Tar.gz` (1999) |
| applSetup | a **csh script** (`extensions/bin/solaris/applSetup`, v1.10 1995 + RGO edits). It should run on Linux under tcsh with no port |
| support libs | `/gemini/astlib -> astlib-1.3`, `/gemini/slalib -> slalib-1.1`, `/gemini/timelib -> timelib-1.3`, with source in `extensions/src/gemini/*` |
| GEM5 runtime | `/gemini/external/GEM5/base/bin/mv167/{iocCore,drvSup,recSup,devSup,seq,...}` plus `vxWorks` (Nov 1998). The build tree's copy differs: `recSup` is a different date and `vxWorks` is 2002 |
| mv167 kernels | `tornado2.0/mv167/{BC,STND,NetBuf}vxWorks` (2001-04), `tornado2.2/mv167/vxWorks` (2004), plus the GEM5 one above. **Which one GNIRS boots is unknown** |
| polaris `/` | 90% used, **217 MB free**, and `/var/tmp` is on `/`. The EPICS tree alone is 183 MB, so staging is tight |

The existing V1-24..V1-27 builds in `/home/gemvx/hstecher` show the
generated `CONFIG.Defs`: `APPLIC_IOCPATH = pisces:/gemini/GEM5/gnirs/CC/V1-27`,
`APPLIC_DCTSDR = default`, `APPLIC_SITE` empty, and `APPLIC_DEPENDS` =
astlib + timelib only. The built `bin/mv167` has **no `mytimeLib.o`**, which
confirms that `startup.CC.epics.vws` is stale.

### What this changes

- **The compiler gap is far wider than hrwfs's.** It is cygnus gcc 2.2.3
  against vxWorks 5.2b, with no Tornado. ANL's 68k gcc 2.96 targets Tornado
  2.2 / vxWorks 5.5 headers, so we would be compiling against **5.2b**
  headers with a compiler seven years newer, and the result would be loaded
  by a 5.2b kernel (if that is what boots). Treat option B as an
  experiment, not an assumption.
- **The host-tool problem may be smaller than feared.** gnirscc has no `.st`
  (so no `snc`), Capfast is dead (so no `e2sr`/`e2db`), `default.dctsdr`
  is committed, and applSetup is csh. If the 3.12 config rules call no
  compiled host tool for an mv167 build, then only `gmake` and `cc68k` matter.
  **To verify:** `$EPICS/config/*` (see §5).
- **New option E: run the original compiler unmodified.** `cc68k` is a SPARC
  Solaris binary. A Solaris 8 guest under `qemu-system-sparc` would
  reproduce production's compiler exactly, so the validation would be
  byte-level. It is heavy and has licensing questions, but it is the only
  route to exact output if no compiler source exists.

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

Done: `stage-buildenv.sh inventory` (§0b).

Next, all read-only, csh on polaris after `GEM5`:

1. `cat ~/.gem5`
2. The vxWorks 5.2b tree: `ls -la $VX_DIR`, `du -sk $VX_DIR/*`,
   `grep -i version $VX_DIR/h/version.h`, `ls $VX_DIR/gnu`,
   `ls $VX_DIR/gnu/solaris.68k/lib/gcc-lib/m68k-wrs-vxworks/cygnus-2.2.3.1`
   (is there any GNU **source** anywhere under `$VX_DIR`?)
3. The config rules: `ls -la $EPICS/config`, then
   `grep -n 'bld\|dct\|sdr\|snc\|e2sr\|e2db\|antelope\|e_flex\|BIN)' $EPICS/config/RULES* $EPICS/config/CONFIG*`
   (which host binaries an mv167 build actually invokes)
4. `file ~hstecher/gnirscc-git-V1-27/bin/mv167/*`
5. `df -k` (find a filesystem with room for staging)
6. The live deploy: `ls -la /gemini/GEM5/gnirs/CC` and `ls -la` of the release
   in use
7. The CC crate's **boot parameters** (`p` at the boot prompt, or
   `bootParamsShow`): which kernel, which startup script
