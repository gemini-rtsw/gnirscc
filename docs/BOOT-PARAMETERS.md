# gnirscc crate boot parameters

## Production today (pisces-control)

From the crate console, 2026-02-01:

```
boot device          : ei
processor number     : 0
host name            : pisces-control
file name            : /home/gemvx/cristian/epics/GEM5/base/bin/mv167/vxWorks
inet on ethernet (e) : 10.2.2.87:ffffff00          (mkognirscc-ap1.hi.gemini.edu)
host inet (h)        : 10.2.2.57
gateway inet (g)     : 10.2.2.1
user (u)             : gemvx
flags (f)            : 0x8                         (blank ftp password: kernel over rsh)
target name (tn)     : gnirscc
startup script (s)   : /gemini/GEM5/gnirs/CC/CC/bin/mv167/startup.IS
```

Banner: `VxWorks version 5.2`, `WIND version 2.4`, `Motorola MVME167`, BSP 1.0,
16 MB.

## After the migration (mkotcsbootv2-lv1) -- PLANNED

Same pattern as hrwfs (`hrwfs/docs/BOOT-PARAMETERS.md`) and gmoscc. Two
fields change, and the kernel path moves off a personal directory:

```
host name            : mkotcsbootv2-lv1                          <-- was pisces-control
file name            : /gemini/external/GEM5/base/bin/mv167/vxWorks   <-- was ~cristian/...
inet on ethernet (e) : 10.2.2.87:ffffff00
host inet (h)        : 10.2.2.145                                <-- was 10.2.2.57
gateway inet (g)     : .                                         (same subnet; see hrwfs note)
user (u)             : gemvx
flags (f)            : 0x8
target name (tn)     : gnirscc
startup script (s)   : /gemini/GEM5/gnirs/CC/CC/bin/mv167/startup.IS   (unchanged)
```

**Bootsmith profile:** `MKO-PROD-GNIRSCC-new-bootserver`, in bootsmith-config
branch `gnirscc-new-bootserver`, commit `f816031` (local, not yet pushed). It is
`MKO-PROD-GNIRSCC` with `host_name`, `host_inet` and `file_name` changed as
above, the same way `MKO-PROD-HRWFS-new-bootserver` was made. bootsmith's own
loader reads it, and all 17 profiles still load.

## Verified: every path the new startup touches (2026-10-08)

The executed lines of the generated `local` and `startup.IS` name two hosts:
`mkotcsbootv2-lv1` (10.2.2.145), for the hostAdd, the only nfsMount
(`/gemini`) and nfsAuthUnixSet, and `mk-gnirs-perle` (10.2.2.86), the Perle
terminal server for the serial lines. There is no pisces reference, no
`/export/...`, and no `/gemdata`. `resource.def` names only 10.2.2.145.

All **35** file paths those lines reference resolve in a `/gemini`
assembled the way the new server would export it: the build at
`GEM5/gnirs/CC/CC`, plus the staged `external/GEM5`, `{astlib,slalib,timelib}`
and `GEM5/gnirs/IS/IS`. So do the four files the C code opens through
`configDirectory` (`gnirsConfig`, `gnirsMechanisms`, `gnirsFilters`,
`SafetyFilters`). The only `/gemini` paths embedded in the loaded data are
`mechanisms.pv`'s `dirLut = /gemini/GEM5/gnirs/CC/CC/data`, which is already
the fixed path, and two in IS files that are not live: `gmSeqSim.pv` is not
loaded, and the `gmSeq.pv` line is commented out. The IS reads its LUT
directory from `nirs:cc:dirLut`, so it follows the CC.

## What these confirm

**1. The kernel is loaded from a personal home directory.** The kernel and
`vxWorks.sym` come over rsh from `/home/gemvx/cristian/epics/GEM5/...`. That is
a developer's checkout on pisces, and it vanishes with pisces. The boot log's
`Loading... 457932 + 50744 + 26218` (text + data + bss) matches exactly one
staged kernel, read from the a.out headers:

| kernel | text + data + bss | match |
|---|---|---|
| **`/gemini/external/GEM5/base/bin/mv167/vxWorks`** (Nov 1998) | **457932 + 50744 + 26218** | **yes** |
| `epics3.12.2GEM5/base/bin/mv167/vxWorks` (build tree, 2002) | 467544 + 50908 + 27770 | no |
| `tornado2.0/mv167/{BC,STND,NetBuf}vxWorks` | 665996-667164 + ... | no |
| `tornado2.2/mv167/vxWorks` | 815008 + ... | no |

So the runtime tree's own kernel is the one booted, and `gem5-epics-runtime`
can ship it at its existing path.

The symbol table follows the kernel. The boot log shows
`Loading symbol table from pisces-control:/home/gemvx/cristian/epics/GEM5/base/bin/mv167/vxWorks.sym`,
and vxWorks always loads `<file name>.sym`, so changing `file name` moves both
off the personal directory.

**Confirmed identical (2026-10-08).** cristian's pair, pisces's
`/gemini/external/GEM5` pair and our staged copy all match:

```
fdec7df754a2098171718defa2f86869ec1c81339068b268a5ff239166bc9e9f  vxWorks
8e2045c5de4d9971ba0926e3fe5afecf3ae013162055f9e682ff8de1e67cc051  vxWorks.sym
```

So the new boot parameters point at `/gemini/external/GEM5/base/bin/mv167/vxWorks`
and boot **the same kernel, bit for bit**. Package the pair at that path and
record these hashes in the changelog, as gem-vxworks-tornado20 did.

The `tornado2.0/mv167` kernels are *not* used by gnirscc, so the
`gem-vxworks-tornado20` ownership question from hrwfs does not arise here.

**2. The deploy path is already fixed:** the startup script is read through
`/gemini/GEM5/gnirs/CC/CC -> V1-27`. An RPM that installs a real `CC/CC`
directory needs no change to `startup script (s)`. This is the same
transition gmoscc (`setgmos`) and hrwfs (`hrwfs/hrwfs`) made.

**3. The crate also runs the Instrument Sequencer.** `startup.IS` loads
`../IS/IS/bin/mv167/gmSeqAppl` and four IS `.pv`/`.db` files from
`/gemini/GEM5/gnirs/IS/IS`. There is no separate IS IOC: the IS is part of
*this* IOC's runtime. So the new boot server must serve the IS tree too,
whether or not the IS source is ported.

**4. `/gemdata` is mounted but never used.** Nothing in `sys/`, `include/`,
`pv/` or `data/` references it. The mount in `local.vws` can be dropped, and
`/gemdata` does not need exporting to 10.2.2.87.

## Hand edits and baked-in paths to fix (what gmoscc/hrwfs fixed)

In the production `startup.IS`, `local`, `resource.def` and the build:

| # | issue | today | fix (precedent) |
|---|---|---|---|
| a | NFS server | `local`: `nfsMount "pisces-control", "/export/gemini"`, `"/export/gemdata"`, `nfsAuthUnixSet "pisces-control"` | `hostAdd` + mount `mkotcsbootv2-lv1:/gemini`; drop `/gemdata` (gmoscc 03c3552, hrwfs 3c33e4f) |
| b | IOC log + NTP | `resource.def`: `EPICS_IOC_LOG_INET` / `EPICS_TS_NTP_INET` = `10.2.2.57` | `10.2.2.145` (gmoscc 03c3552) |
| c | **applSetup clobbers startup files** | 3.12 `applSetup` (lines 787-789, 837-838) copies `resource.def`, `local.vws`, `UAE.dist` from the templates every run. `nirsSetup` stashes only `local.vws`; the committed `resource.def` is byte-identical to the template, so any edit to it is silently lost | stash **all three** before applSetup and restore after, failing on mismatch (hrwfs c41df27) |
| d | versioned `cd` | `cd "/gemini/GEM5/gnirs/CC/V1-27/.."` comes from `APPLIC_IOCPATH=pisces:/gemini/GEM5/gnirs/CC/V1-27` | `APPLIC_IOCPATH=<host>:/gemini/GEM5/gnirs/CC/CC`. It **must keep the `HOST:` prefix**: 3.12 `CONFIG_APPLIC:149` takes `DIST_PATH` as word 2 after splitting on `:`, so a bare path yields `cd "/.."` (hrwfs fde3d94) |
| e | relative loads | everything loads via `./CC/...` and `../IS/IS/...` after that `cd`, so a wrong `cd` silently loads another build | make loads absolute `/gemini/GEM5/gnirs/CC/CC/...` and `/gemini/GEM5/gnirs/IS/IS/...` so a boot cannot depend on the `cd` (gmoscc 03c3552) |
| f | build path in comments | commented lines expand `$(install)` to `/home/gemvx/hstecher/gnirscc-git-V1-27`; in CI that becomes the rpmbuild dir and trips hrwfs's "no rpmbuild path in payload" guard | remove `$(install)` from the comments, or the dead lines entirely |
| g | kernel off a personal dir | `file name` = `~cristian/epics/GEM5/...` | `/gemini/external/GEM5/base/bin/mv167/vxWorks`, packaged |
| h | stale comment | `####CC Simulation Mode CURRENTLY ENABLED####` above `pvload startupCC.pv` (the non-sim file) | correct or remove |

**Status (2026-10-08): a-f and h are fixed in the source**, and
`tools/linux-build/check-build.sh` now fails any build that regresses them.
It checks that both scripts `cd` to `$DEPLOY/..`, that no relative load
remains, that the IS loads from `$IS_DEPLOY`, that no executed line names
pisces, that `local` mounts `/gemini` from the new server, that no build
directory leaks into the payload, and that every object is a.out mc68020.
Run against the pre-fix tree, it fails on all of them. Item g is a
boot-parameter change, made at the crate.

**Deploy these together with the boot-server switch.** The new `local`
mounts `/gemini` from mkotcsbootv2-lv1. Booted with today's parameters, the
crate would read its startup over rsh from pisces and then NFS-mount the new
server. That only works if both serve identical trees, so treat the
boot-parameter change and the first boot of this build as one step.

No hand-maintained version string was found (`grep '"V1-'` is clean), unlike
gmoscc's `gm:sad:name`.

## Boot server additions (nfsv2-bootserver, TCS variant)

**Prepared:** nfsv2-bootserver branch `tcs-add-gnirscc`, commit `a12dbd9`
(local, not yet pushed). It makes the two config edits below.

- `config/tcs/exports`: add `10.2.2.87(rw,no_root_squash)` to the `/gemini`
  line. Do not add it to `/gemdata` (item 4).
- `config/tcs/rhosts`: `10.2.2.87`, `mkognirscc-ap1.hi.gemini.edu`,
  `mkognirscc-ap1` as `gemvx`. 10.2.2.87 has a PTR record, so the IP alone
  does not satisfy `ruserok()` (same reasoning as the GMOS/HRWFS entries).
- `/gemini` on the host must carry `external/GEM5` (runtime + kernel),
  `astlib`/`slalib`/`timelib` (`-1.3`/`-1.1`/`-1.3` and their links),
  `GEM5/gnirs/CC/CC`, **and `GEM5/gnirs/IS/IS`**.
- **Untested:** vxWorks **5.2** as an NFSv2/rsh client of `nfs-user-server`.
  gmoscc (5.5) and hrwfs (5.4) are the precedents, and 5.2 is older than both.
