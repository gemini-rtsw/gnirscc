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

### Second inventory (2026-10-08, `gnirscc-inv2.txt`)

- **The GNU source for the cygnus 2.2.3 toolchain is on polaris**:
  `$VX_DIR/gnu/src/{gcc,gas,ld,bfd,binutils,libiberty,...}`. It is kept as
  a reference for the exact flags and predefines. **Decision (Hawi): we
  will NOT rebuild or emulate the old compiler.** We move to a newer
  compiler, then test and fix. A Solaris guest under QEMU has already
  been tried and was not workable.
- **EPICS 3.12.2 does ship Linux config**, from 1997: `CONFIG.Unix.Linux`,
  `CONFIG_ARCH.Linux`, `CONFIG_SITE.Unix.Linux`, `CONFIG.Vx.Linux`,
  `CONFIG_SITE.Vx.Linux`. So `HOST_ARCH=Linux` is a supported setting to
  modernise, not a port from nothing.
- `grep` for `bld|dct|sdr|snc|e2sr|e2db|antelope|e_flex` in
  `$EPICS/config/{RULES,CONFIG}*` matched **nothing** (lowercase only;
  check the rules for uppercase variables once the tree is staged).
  This is promising for "no compiled host tools needed".
- `version.h`: `VXWORKS_VERSION "5.2 Rev B"`.
- `~/.gem5` sources `$EPICS/config/epics.csh` and sets `CVSROOT
  :pserver:gemvx@polaris:/usr/software/dev/cvsroot/rtcvsroot`. That is
  where the gnirscc CVS history lives, if it is ever wanted.
- **The deploy already has a fixed path**: `/gemini/GEM5/gnirs/CC/CC -> V1-27/`,
  a selector symlink (as gmoscc's `setgmos` and hrwfs's `hrwfs/hrwfs` were).
  `mechanisms.pv` already names `/gemini/GEM5/gnirs/CC/CC/data`. The RPM can
  replace the link with a real directory without a boot-parameter change,
  if the boot line names `.../CC/CC/...` (still to confirm).
- **Space on polaris.** `/` and `/var/tmp` have ~217 MB free.
  `/export/home` (local) has **4 GB free**. `/home/gemvx` is
  `pisces:/export/home/gemvx`, **the same 99%-full filesystem as
  `/gemini`**, so never stage there: it would eat production's space.

### Trees staged (2026-10-08)

Collected without writing to polaris: the polaris trees went over an ssh tar
pipe to hstecher-ld1, and the pisces trees came via `stage-buildenv.sh` on
hbftelops-ld3. They are escrowed in `~/work/gnirscc-buildenv/{polaris,pisces}`
and unpacked at their original paths under `~/work/gnirscc-buildenv/root/`.
The file counts match polaris exactly (EPICS 7804, vxWorks 2662).

**The Vx build needs no EPICS host tools.** `CONFIG.Vx.68k` and `RULES.Vx`
invoke only `cc68k`/`cpp68k`/`ld68k -r`/`ar68k`, `sed` and `install`. The
exact compile line is:

```
cc68k -B$(VX_GNU_LIB)/gcc-lib/ -nostdinc -O -Wall -I. -I.. -I$(EPICS_BASE_INCLUDE)
      -I$(EPICS_BASE_RECS) -I$(VX_INCLUDE) -DvxWorks -DV5_vxWorks
      -DCPU=MC68040 -m68040 -DCPU_FAMILY=MC680X0 -c
ld68k -r -o <prod> <objs>
```

The Unix side calls host tools in three places, and all three are already
satisfied by committed output:
- `capfast/`: `sch2edif`/`e2sr` (dead). The three `.db` files are
  committed in `data/`.
- `ascii/`: builds `default.dctsdr`/`.sdrSum` from `cat_ascii/{devSup,drvSup}.ascii`
  via `base/tools/makesdr` and the SPARC `bld*` tools. The committed copies
  are **byte-identical to production**, so seed them instead of building.
- `.st`/`snc`: unused.

**Deployed objects are `a.out SunOS mc68020`, not stripped.** Whatever
compiler we use must emit a.out that the crate's kernel can load.

**`startup.IS` is the live startup** (it matches `epicsBoot`'s `startup.IS`;
boot parameters still to confirm). Its generated output differs from
`startup/startup.IS.vws` only by macro substitution. The *other* variants
(`startup.CC.epics` etc.) are stale, but `startup.IS` is not. It:
- `cd`s to `/gemini/GEM5/gnirs/CC/V1-27/..`, then loads everything through
  `./CC/...` -- the `CC -> V1-27` selector. An RPM can make `CC/CC` a real
  directory, but the `cd` line embeds the versioned path via `$(iocpath)`,
  so `APPLIC_IOCPATH` has to become `.../CC/CC`.
- **Also loads the GNIRS Instrument Sequencer into the same crate**:
  `ld < ../IS/IS/bin/mv167/gmSeqAppl` and `../IS/IS/data/nirsSeq{,Sad}Top.db`.
  The IS is a separate GEM5/mv167 build that is not in this repo and not yet
  located in git. It has to move with the CC, or the crate keeps
  loading a Solaris-built IS beside a Linux-built CC.

**`data/` is written at runtime.** The live `data/` holds 204 files: 28
release files plus timestamped `gnirsMechanisms.*`/`gnirsConfig.*` backups,
the newest from 2026-10-02. `gnirsConfig`, `gnirsMechanisms` and
`mechanisms.pv` differ from the repo. **An RPM must not own `data/`'s tuned
files** (§2.4 is now settled in that direction).

**Drift fixed:** `data/nirsCCSadTop.db` lacked the GNFR-75349 thresholds
(committed only to the `.sch`). It is now the deployed copy.

**`local` still mounts `pisces-control`.** The deployed `local` names
`pisces-control:/export/gemini`, the same as the repo's.

### The Instrument Sequencer tree (staged 2026-10-08)

`/gemini/GEM5/gnirs/IS/IS` is in `~/work/gnirscc-buildenv/pisces/gnirs-is-deployed.tar.gz`
(115 entries). It is unpacked beside the CC at `root/gemini/GEM5/gnirs/IS/IS`.

- **IS V1-7, built 3 Dec 2012 and never rebuilt.** `gmSeqAppl` is
  a.out mc68020, 44912 + 644 + 640 bytes, sha256 `056c5dfb248c...`. The source
  is still in CVS.
- **Every file `startup.IS` loads from it is present:** `gmSeqAppl`,
  `nirsSeq{,Sad}Top.db`, `gmSeq.pv`, `startup.pv`,
  `loadFOC{imaging,spectral,spatial}.pv`.
- **Its `data/` is tuned live, like the CC's.** `focus{SB,SR,LB,LR}-{imaging,spectral}.dat`
  are edited in operations and backed up by date, from 2021 to 2026-03-13. The same
  rule applies: no RPM may own them.
- `data/README.2024.05.20` records an **open operational workaround**. The IS
  loaded the focusSR tables for ShortBlue, so the SR tables were overwritten
  with SB copies (the originals are backed up as `*.2024.05.20`). That is not
  this migration's to fix, but whoever owns the IS should know it is
  documented only in a README on the file server.

**Plan for the IS:** package the deployed V1-7 binaries and startup data
as-is, as a frozen, prebuilt dependency (`gnirs-is`, like `gmos-deplibs`). The
tuned focus tables stay out of the package. Port the IS source from CVS
later as its own job. The CC's new-compiler objects will then run beside a
cygnus-2.2.3 `gmSeqAppl`, which is the same mixed-compiler situation hrwfs
analysed, and is covered by the same crate test.

## 0c. RESULT: the toolchain experiment works (2026-10-08)

**ANL's Linux 68k gcc 2.96 builds gnirscc against the vxWorks 5.2b headers,
and the result loads.** Reproduce it with `tools/linux-build/compile-test.sh`
followed by `tools/linux-build/check-objects.sh`.

Toolchain: `https://epics.anl.gov/base/gnu-tools.tor2_2-m68k-rhel5.tgz`,
sha256 `3ea24b7322d815ec1e8f438ee5faf3ce00cbd4219574ed23621858728376fc7e`.
The GPL source is `cum.tor2_2-m68k.tgz`, sha256
`1e0fa2c16cc61478dd32a3a2170bf4aeac8a445e4810a88ab27bb299d3a24a24`.
Both are kept in `~/work/gnirscc-buildenv/toolchain`. They install as
`$VX_DIR/gnu/Linux.68k`, which is the slot `CONFIG_COMMON`'s
`VX_GNU_BIN = $(VX_GNU)/$(HOST_ARCH).$(ARCH_CLASS)/bin` already expects for
`HOST_ARCH=Linux`.

- **63 of 63 sources compile; all 5 products link.** There are no compiler
  or header incompatibilities. The 254 warnings are 1990s-C cosmetics
  (braces around scalar initializers, implicit int, parentheses).
- **Same object format:** `a.out SunOS mc68020`, not stripped, the same as production.
- **`.data` is identical in size for every product; `.bss` is too, except
  drvAscii +8 and sioSup +4. `.text` is 0-3% larger.**

  | product | production text/data/bss | built |
  |---|---|---|
  | ccGlobal.o | 680/376/0 | 680/376/0 |
  | cicsLib.o | 1848/100/7680 | 1876/100/7680 |
  | epicsCAInt.o | 2512/8/0 | 2520/8/0 |
  | tnetDev | 2160/0/0 | 2220/0/0 |
  | sioSup | 8848/32/532 | 9116/32/536 |
  | drvAscii | 44668/508/24 | 45972/508/32 |
  | hdwrControl.o | 60592/2420/48 | 62284/2420/48 |
  | epicsControl.o | 134772/1040/20596 | 136864/1040/20596 |

- **Exported symbols are identical for all 7 loaded products.**
- **Every module loads.** Following startup.IS's `ld` order, each undefined
  symbol resolves against the production vxWorks 5.2 `vxWorks.sym` (2787
  symbols) plus the modules loaded before it. That gives 0 unresolved, the
  same as the production objects. gcc 2.96 introduced **no new
  compiler-support references**: the only `___` symbols are vxWorks libc's
  (`___errno`, `___ctype`, `___srget`, ...), the same set as production. A
  negative control (a module moved ahead of its dependencies) correctly
  fails with 29 unresolved.
- **Checked the two warnings that could change behaviour:**
  - `motors.c:292` calls `sqrt` with no `<math.h>` (the motor-timeout
    calculation for short moves). Both compilers take the result as a
    `double` from `d0:d1`, and the instruction sequence around the call is
    the same apart from register choice. **No behaviour change.**
  - `ccGlobal.o` `.data` has the same size but different bytes. It is a table of 94
    pointers to string literals, which gcc 2.96 lays out in reverse order.
    Following each pointer gives the **identical string sequence**.
- `tandp.c`'s implicit `fabs` produces no call and no `fabs` instruction in
  either build, so it is handled identically.

**What is proven:** the toolchain, the headers, the object format, and that
the objects load. **Not yet proven:** the full UAE `gmake` build
(applSetup under tcsh, HOST_ARCH=Linux), and runtime behaviour, which needs
the crate test (Phase 4). As with hrwfs, our modules will run beside a
cygnus-built kernel, EPICS runtime, support libs and IS.

## 0d. RESULT: the full UAE build works on Linux (2026-10-08)

`tools/linux-build/build.sh` runs `setup.sh && gmake` in the
`gnirscc-build:el9` container against the staged trees, and **exits 0 on the
first attempt**. It descends into every directory (`ascii capfast pv src startup sys/*`, host
and mv167) and produces **the complete production payload**:

- `bin/mv167`: the same 16 files as the deployed V1-27, nothing missing or extra;
- `data/` and `include/`: the same file lists as production;
- `check-objects.sh build-out`: **PASS**. The format is a.out mc68020, the
  exports match production for all 7 loaded products, and there are 0
  unresolved symbols in boot order;
- the generated scripts `startup`, `startup.CC.epics`, `startup.python` and
  `startup.seed` are **byte-identical to production**. `local*` and
  `startup.IS` differ only in the `cd`, which now reaches the fixed
  `/gemini/GEM5/gnirs/CC/CC` instead of `V1-27`, and in one commented line
  that embeds the build directory through `$(install)` (§0b item f);
- `data/` content differs only in the three live-tuned files
  (`gnirsConfig`, `gnirsMechanisms`, `mechanisms.pv`).

The real build's flags come from `CONFIG_APPLIC`: `-ansi -Wall -pedantic`,
`-nostdinc`, `-m68040`, no `-O` (`VX_OPT=NO`). EPICS headers are reached
through the checkout's `epics/` links. That makes it marginally closer to
production than `compile-test.sh` (sioSup 9060 vs 9116, drvAscii 45300 vs
45972).

What it took:
- `gem5-env.sh`: `HOST_ARCH=Linux`, `VX_DIR`, and the 68k tools on `PATH`,
  with `GCC_EXEC_PREFIX` *unset* (polaris points it at the SPARC tools).
- `setup.sh` runs the 3.12 `applSetup` through `csh`, using nirsSetup's
  arguments plus explicit `-b/-c/-e`. Without them applSetup looks for
  base via `which caRepeater` and extensions via `GetVar`, which are
  Solaris binaries. It also stashes and restores all four versioned files
  applSetup overwrites, and seeds the Capfast `.db` files and
  `default.{dctsdr,sdrSum}` so the dead generators never run.
- No EPICS host tool was built or needed.

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
