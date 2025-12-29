'\" t
.so wrs.an
.\" mv162/target.nr - Motorola MVME162,MVME162LX target specific documentation
.\"
.\" Copyright 1984-1993 Wind River Systems, Inc.
.\"
.\" modification history
.\" --------------------
.\" 01c,17mar94,vin  added comments about snooping problem.
.\" 01b,05nov93,dzb  added notes about MVME162LX.
.\" 01a,11feb93,ccc  created from mv167.
.\"
.\" NOTE
.\" EXOS
.\"
.TH "mv162" T "Motorola MVME162,MVME162LX" "Rev: 5 Nov 93" "VXWORKS REFERENCE MANUAL"

.SH "NAME"
.aX "Motorola MVME162,MVME162LX"

.SH "INTRODUCTION"
This manual entry provides board-specific information necessary to run
VxWorks.  Before running VxWorks, verify that the board runs in the
factory configuration using vendor-supplied ROMs and jumper settings,
and then checking operation through the RS-232 connection.

.SH "BOOT ROMS"
The MVME162 uses one 27C201 (or compatible) VxWorks boot ROM.
The MVME162LX uses one 27C020 (or compatible) VxWorks boot ROM.
Install the ROM as follows:
.TS
center;
cf3 cf3
a a .
.ne 3
Socket	Target
_
U38	MVME162
U24	MVME162LX
.TE

The board has non-volatile RAM; thus boot parameters
are preserved whenever the system is powered off.
.PP
To load VxWorks, and for more information, follow the instructions in
"Getting Started"
chapter of the
.I "VxWorks Programmer's Guide."

.SH "JUMPERS"
The following jumpers must be changed from the factory defaults.
Consult the hardware manual for details of jumper configurations.

.TS E
expand;
cf3 s s
lf3 lf3 lf3
l l lw(3i) .
.ne 5
MVME162 Target Board
.sp .25
Jumper	Function	Description
_
J1	system controller	T{
Install if the board is to be system controller.
There must be only one system controller on the backplane,
and it must be in the first slot.
T}
J22 9-10	boot source	T{
Remove to boot from installed ROM, install to boot from FLASH memory.
T}
.sp .5
.T&
cf3 s s
lf3 lf3 lf3
l l l .
.ne 5
MVME162LX Target Board
.sp .25
Jumper	Function	Description
_
J1	system controller	T{
Install if the board is to be system controller.
There must be only one system controller on the backplane,
and it must be in the first slot.
T}
J11 7-8	boot source	T{
Remove to boot from installed ROM, install to boot from FLASH memory.
T}
.TE

.SH "DEVICES"
The
z8530Serial.c - Z8530 SCC (Serial Communications Controller) tty driver
is provided for the two on-board serial ports;
see the manual entry for tyCoDrv.
.PP
The chip drivers included are:
    mccTimer.c - Memory Controller Chip Timer library
    vmeChip2Vme.c - VMEchip2 VMEbus interface library
    nvRam.c - non-volatile RAM library
.PP
A device driver for the on-board
NCR 53C710
SCSI controller is provided.
The INCLUDE_SCSI directive must be enabled in config.h.
.PP
A network interface for the on-board Intel 82696 chip allows VxWorks
to run without an additional network board.
The interface is called "ei" and should be specified as
the boot device to the boot ROM.
Using a separate network board, e.g. CMC ENP 10/L,
is possible but not all configurations are tested.

.SH "SPECIAL CONSIDERATIONS"
To properly configure the MVME162LX's four on-board serial I/O channels,
the MVME162LX pre-processor directive must be enabled in config.h.  Simply
change the #if statement above the MVME162LX macro to TRUE, then re-build
the VxWorks image.
.PP
On the MVME162, the SCSI and Ethernet connections appear on the P2 connector.
The MVME712/MVME712M Transition Module provides convenient access.
On the MVME162LX, the SCSI and Ethernet connections are accessible from
connectors on the front panel of the board.
.PP
Both Parity and ECC DRAM mezzanine modules are supported by this BSP.
Although the target hardware may support stacking of DRAM modules, more than
one module will not be automatically configured.  Thus, use of only one
Parity DRAM module *or* one ECC DRAM module is supported.  In cases
where a Parity DRAM and ECC DRAM are both present, the Parity DRAM
will be used.
.PP
Due to the snooping problems with the Test And Set (TAS) command over the 
VME, the shared memory pool for the backplane driver is defined in the low
memory and marked as non-cacheable. Bus snooping the VME accesses has been 
turned off due to this problem. This will not effect the cache and snooping
for the on-board devices such as ethernet and SCSI as snooping will be turned
on for these devices. The size of the memory required for the shared memory 
should be a multiple of 8k which is the default page size for the mmu. Any
interprocessor communication done through a shared memory, should make sure
that the cache is disabled for that shared memory location. For this reason,
if cacheDmaMalloc() is used to obtain a cache coherent buffer to be used
as a shared memory pool across the VME bus, then the memory region should be
made non-cacheable using the following as an example changing 
MEMORY_ADDRESS and MEMORY_SIZE as needed:  

.CS
    /* configure the shared memory area as non-cacheable */

    #ifdef INCLUDE_MMU_BASIC
    (void) vmBaseStateSet (NULL, (void *)MEMORY_ADDRESS,               
                          MEMORY_SIZE,
                          VM_STATE_MASK_CACHEABLE,
                          VM_STATE_CACHEABLE_NOT);
    #endif /* INCLUDE_MMU_BASIC */

    #ifdef INCLUDE_MMU_FULL   /* unbundled mmu product */
    (void) vmStateSet (NULL, (void *)MEMORY_ADDRESS,
                      MEMORY_SIZE,
                      VM_STATE_MASK_CACHEABLE,
                      VM_STATE_CACHEABLE_NOT);
    #endif /* INCLUDE_MMU_FULL */
.CE

These changes would be in effect until the snooping problems with
TAS instruction are fixed on the hardware. Memory from 0x2000 to 0x20000 has 
been allocated for shared memory. The RAM_LOW_ADRS for this reason has been 
raised from 0x1000 to 0x20000.
.PP
The VMEbus slave base address is set in software, in the routine
sysProcNumSet().
If the board has 4Mbytes of memory then the memory will be
mapped to the standard (24-bit) address range starting at LOCAL_MEM_BUS_A24.
Regardless of local memory size, the local DRAM will also be mapped to
the extended (32-bit) address range starting at LOCAL_MEM_BUS_A32.
.PP
The size of local memory is determined from the LOCAL_MEM_SIZE macro.
Be sure to set LOCAL_MEM_SIZE to the size of local RAM so that VMEbus
mapping is properly performed.
It should be noted that the default configuration has LOCAL_MEM_SIZE set
to the minimum size of 4Mbytes, which will invoke a mapping of local memory
to the VMEbus address given by LOCAL_MEM_BUS_A24.  This is in standard
(A24) address space.  Local DRAM will also be mapped to the VMEbus address
given by LOCAL_MEM_BUS_A32. This is in extended (A32) address space.
Also note that only processor 0 (the backplane master) has its RAM dual-
ported onto the VMEbus.
.PP
On the MVME162 and MVME162LX target boards, the FLASH memory may be
used as memory space for a VxWorks boot ROM image or a VxWorks standalone
ROM image.  This is particularly noteworthy for the MVME612 due to the
limited ROM space on the MVME162 (4Mbit maximum).
.PP
The image to be burned into FLASH must first be converted from a.out
object file format to a binary file.  This is done with the aoutToBin
program supplied with VxWorks:

    /usr/vw/bin/<host>/aoutToBin < vxWorks.st_rom > vxWorks.st_rom_bin

Note that the a.out file "vxWorks.st_rom" is used instead of the S-record
file "vxWorks.st_rom.hex".  The a.out file is produced during the build
of the VxWorks standalone ROM image vxWorks.st_rom.hex.  Similarly, "bootrom"
is the a.out file produced when building bootrom.hex.

To burn a VxWorks boot ROM image or VxWorks standalone
ROM image into FLASH memory, insert the Motorola-supplied boot ROM
162Bug into the ROM socket, and remove jumper that selects the boot
option.  At the "162-Bug>" prompt, use the "niot", then "niop" 162Bug
commands to download the binary file version of the relevant VxWorks image.
Consult Motorola's "Debugging Package for Motorola 68K CISC CPUs User's Manual"
for more information on using 162Bug commands; they are not
supported by Wind River Systems.

    162-Bug>niop
    Controller LUN =00? 
    Device LUN     =00? 
    Get/Put        =G? 
    File Name      =? vxWorks.st_rom_bin
    Memory Address =0000E000? 0x10008
    Length         =00000000? 
    Byte Offset    =00000000? 

    Bytes Received =&321548, Bytes Loaded =&321548
    Bytes/Second   =&64309, Elapsed Time =5 Second(s)

Next, edit the first 8 bytes of the downloaded image to add the program
counter and stack addresses needed at start-up.

    162-Bug>mm 0x10000
    00010000 0000? 0000
    00010002 0000? 1000 
    00010004 0000? ff80
    00010006 0000? 0008
    00010008 46FC? .
    162-Bug>

Next, program the downloaded VxWorks binary image into FLASH memory with
the 162Bug command "pflash":

    162-Bug>pflash 0x10000:0x80000 0xffa00000
    Source Starting/Ending Addresses      =00010000/0008ffff
    Destination Starting/Ending Addresses =FFA00000/FFA7FFFF
    Number of Effective Bytes             =00080000 (&524288)
 
    Program FLASH Memory (Y/N)? y

Finally, power-down, replace boot option jumper and power-up.  Upon power-up,
execution should immediately begin from the image just programmed into flash.

.SH "BOARD LAYOUT"
The diagrams below show the location of jumpers relevant to VxWorks:
.bS
______________________________              _______________________________
|             P1             |    MVME162   |             P2              |
|                            ----------------                      ____   |
|                                                                 |    |  |
|                                                          pin 1  \e____|  |
|                                                                  U38    |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|  X - SYS CONTROLLER J1                                                  |
|                                                                         |
|_________________________________________________________________________|
.bE
.bS
______________________________              _______________________________
|             P1             |  MVME162LX   |             P2              |
|                            ----------------                             |
|                              ____________                               |
|                             |            |                              |
|                             >    U24     |                              |
|                             |____________|                              |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|                                                                         |
|      - SYS CONTROLLER J1                                                |
|                                                                         |
|_________________________________________________________________________|
.bE

.SH "SEE ALSO"
if_ex, if_enp,
.pG "Getting Started, Configuration"
