/*
 *  Author: Allan Honey (Keck observatory) with considerable collaboration 
 *         by W.Lupton.
 *
 *         Derived from the Compumotor SX device support (devCmSx.c)
 *         written by Jeff Hill.
 *         
 */

/*
 *  devAscii provides the interface between records, whose DTYP='Ascii SIO',
 *  and drvAscii. The records supported are ai, ao, bi, bo, mbbi, mbbi 
 *  direct, mbbo, mbbo direct, longin, longout, stringin, stringout, and 
 *  waveform. Where the waveform record is used as a long stringin record 
 *  such that strings greater than MAX_STRING_SIZE (40) can be handled. 
 *
 *  The basic design is to pass record specific data to drvAscii, which 
 *  formats a request for drvSerial, then eventually asynchronously calls 
 *  back devAscii with the response.
 *
 *  PARM STRING USAGE
 *  During record init the PARM string (record's INP or OUT field) is parsed
 *  and the information is cached and passed to drvAscii. drvAscii uses the 
 *  information to determine how to handle i/o for the record.
 *
 *  The format of the PARM string is: 
 *    serial port name {[signal number | array size] @}
 *                     {<special record> || 
 *                      [<command prompt><response format>]
 *                     {<readback prompt><readback response format>}}
 *
 *    where:
 *    signal number = Sn @     - not implemented
 *    array number  = An @     - not implemented
 *
 *    special_record 
 *    = [ slope || 
 *        timeout || 
 *        writeCmt %s || 
 *        readCmt %s || 
 *        connect || 
 *        connectSts || 
 *        debug ]
 *
 *    note that the %s for the write and read command terminators are 
 *    necessary!
 *
 *    Note that all records must have a serial port name. As of Jan 97 the
 *    driver was not tested with file streams but in principle only minor
 *    changes should need to be made to make it work. All records except
 *    a stringout record must have a command prompt.
 *
 *  PLEASE refer to the modification history from August 2002 as the
 *         keyword REAL now exists for analog records only.
 * 
 *
 *  SPECIAL RECORDS
 *  The 'special records' control various aspects of how drvAscii functions.
 *  Note that a special record affects the i/o of all records associated
 *  with a serial link. A description of each 'special' record follows.
 *
 *  'slope' is used to manipulate analog i/o values. Slope is needed 
 *  because: the RVAL for an analog input record is an integer and there 
 *  is no way to circumvent conversion from RVAL to VAL (probably would 
 *  not want to anyways) so ESLO, ASLO and ROFF come into play; the RVAL 
 *  from an analog output record is an integer and if one used VAL then 
 *  the ESLO, ASLO and ROFF would be circumvented. Thus all analog output 
 *  values (rval) are divided by slope prior to formatting the output 
 *  string, and all analog input values are multiplied by slope (eg. a 
 *  remote device which returns values with a precision to the second 
 *  decimal place, such as 2.34, may have a slope of 100 such that the 
 *  associated AI rval would be 234 -> 0xea,the record's EGUL and EGUF 
 *  could be set so that the original value is restored or converted 
 *  directly into some other unit). 
 *
 *  The 'slope' record should be associated with an AO record.
 *
 *  PLEASE refer to the modification history from August 2002 as to how
 *         the modifications affect the above discussion of analog 
 *         conversions.
 *
 *  'timeout' allows one to control the maximum time that drvAscii will 
 *  wait for a response to a command/prompt. 'timeout' should be associated 
 *  with a longout record. (future mod is to allow this to be a float so 
 *  fractions of seconds can be specified).
 *
 *  'writeCmt %s' and 'readCmt %s' allows one to change the write and read 
 *  command terminators. The writeCmt is tagged on to the end of the output 
 *  string (prompts), the default is 'crlf'. The readCmt defines the end of 
 *  a response string, default is 'crlf'. The associated records must be 
 *  stringout records. The command terminator strings will be parsed using 
 *  C character and numeric escape codes - be aware that dbpf and caput do
 *  not handle escape codes identically! Also converting from a capfast 
 *  drawing to a database strips the '\' in '\n' so one cannot currently 
 *  specify the usual command terminators as a constant to a capfast record.
 *
 *  'connect' allows one to change the connection state of a serial link.
 *  Upon successful initialization the connection state will be on (1).
 *  When the connection state is off (0) then drvAscii will not perform any
 *  i/o. 
 *
 *  'connectSts' returns the current connection state of a serial link
 *  where 1 means on or ok and 0 means off or disabled.
 *
 *  (The following was added August 2002)
 *  'debug' allows one to control the amount of information generated
 *  from within drvAscii. A debug record must be an integer out record.
 *  There may be as many debug records as there are logical links being
 *  handled by drvAscii (i.e. drvAscii may have connections to multiple
 *  serial ports.
 * 
 *  There are essentially two levels of debug and they only affect the
 *  drvAscii functions. 

 *  If debug is non-zero then all data output from the default tx framing 
 *  routine (putFrame) is displayed on the console port, as is all input 
 *  received by the rx framing routine (getFrame). Where, the non-printing 
 *  characters are displayed as the ascii representation of 2 digit 
 *  hexidecimal characters, enclosed in brackets '[xx]'. 
 *
 *  If debug >= 8 then all the record-specific information is displayed 
 *  each time a drvAscii processes a record (writeInteger, writeReal, 
 *  writeString, readInteger, readReal, and readString functions). The
 *  displayed information is essentially that which is relevant to prompts 
 *  and responses. Thereby, allowing one to determine if the prompt and 
 *  response specifications were parsed as expected, as well as, allowing 
 *  one to work out what drvAscii is attempting to do)  
 *
 *  In addition, to the debug records, a drvAscii global variable 
 *  (drvAsciiDebugLevel) exists for controlling the displaying of 
 *  additional information. The use of this variable allows one to display 
 *  information during iocInit, which can not be done with the 
 *  aforementioned debug record, as well as processing information. 
 *  Said variable affects both the devAscii and drvAscii functions. The 
 *  variable is intended to be used as a bit mask.
 *
 *  Only two bits are used within the devAscii functions.
 *  
 *  If (drvAsciiDebugLevel & 1) then a one line message is displayed
 *  for each record that is handled at iocInit. This simply identifies
 *  the records, being processed, by name, and displayes the parm fields
 *  being parsed. 
 * 
 *  If (drvAsciiDebugLevel & 32) then the name of each read and
 *  write function herein is displayed when a function is invoked.
 *  Thus giving a poorman's trace.
 *
 *  The affects on the drvAscii functions are as follows.
 * 
 *  If (drvAsciiDebugLevel & 2) then the response data and formatting
 *  string used to parse that data is displayed from within 
 *  processOutputResponse().
 *
 *  If (drvAsciiDebugLevel & 8) various parsing info is displayed
 *  from within drvAsciiCreateSioLink() during iocInit. The intent
 *  is to allow one to figure what is wrong with a record.
 *
 *  If (drvAsciiDebugLevel & 16) then all input and output data is
 *  displayed in hex form rather than char form (this affects the
 *  displays from the debug records discussed above).
 *  
 *  If (drvAsciiDebugLevel & 64) the the name of each invoked function,
 *  within drvAscii, is displayed. Again this provides a poorman's 
 *  trace. 
 *
 *
 *      PROMPT AND RESPONSE FORMAT STRINGS (extensions to scanf syntax)
 *
 * KILL SYNTAX
 *  In order to make drvAscii as flexible as possible, with respect to 
 *  remotely connected devices, (at least as far as my imagination could 
 *  fathom) the format strings were extended beyond the usual '%' C spec-
 *  ifications.
 *
 *  drvAscii makes extensive use of sscanf, sprintf, strlen, ...
 *  thus certain values embedded in return strings could cause problems. 
 *  One of the first uses of drvAscii, at Keck, was interfacing to the 
 *  shutter clinometers.  Unfortunately, the underlying device preceeds 
 *  its response with a NULL byte ('\0'). Which of course is going to make 
 *  string processing very cumbersome (the string appears to be a null 
 *  string).
 *
 *  To provide a mechanism for 'ignoring' a leading null byte or, for that
 *  matter, any series of leading bytes, a kill syntax was implemented 
 *  ('%nk' or '%*k'). This essentially means skip the first 'n' characters 
 *  in a response, '%*k' means ignore the entire response. If the kill 
 *  syntax is used it must be the first characters in the response format 
 *  string and only one such specification is permitted. The kill syntax is 
 *  not permitted in a prompt string as it would be meaningless.
 *
 *
 * MULTILINE INPUT SYNTAX
 *  At Keck the infra-red choppers have status request commands which 
 *  generate multi-line responses. To handle this type of response it is 
 *  necessary to circumvent the normal read command terminator mechanism
 *  (ie. the low level framing routine normally waits for a single command 
 *  terminator sequence). This was accomplished by the command terminator 
 *  count syntax '%nt'. This causes the framing to wait for 'n' command 
 *  terminator sequences. 
 *
 *  The command terminator count format string must be either at the 
 *  beginning  of the response format string or immediately following the 
 *  kill string.
 *
 * 
 * BINARY CONVERSIONS
 *  At Keck the secondary actuators are driven by Compumotor motor con-
 *  trollers, to which the interface is via serial i/o. Several of the 
 *  commands to these controllers return statuses as a string of '0' and 
 *  '1' such as '1011001101' or '1110_0110_1111'. It is necessary to con-
 *  vert these strings to integer values before returning them to the Epics 
 *  records, which cannot be done with standard C scanf syntax.
 *
 *  drvAscii augments the standard C scanf syntax with '%nb %[abc...]'. 
 *  This syntax indicates that the incoming string is a sequence of '1' 
 *  and '0' which must be converted to an integer value 
 *  (eg. '101101' -> 0x2d -> 45). The optional '%[abc...]' syntax allows 
 *  one to specify a list of delimiter characters. If the delimiter string 
 *  exists then it must immediately follow the '%nb' string. Also note that 
 *  '%*b' is not valid, as the same can be accomplished with '%*d' or '%*f'.
 *
 * 
 * STRING TO NUMERIC
 *  At Keck we have some devices, namely the Compumotor motor controllers, 
 *  which return statuses as ascii characters (why they did this with some
 *  statuses and binary strings '0' and '1' for other statuses and did not
 *  allow one to select a mode is a mystery). In any event, drvAscii allows
 *  '%nc' or '%ns' to be specified for numeric data. Note that 'n' may not
 *  be larger than 4.  
 *
 *  This syntax will cause the incoming string to be converted to an 
 *  integer, and hence to a float for analog inputs. For instance the 
 *  string "abcd" would be converted to the integer value 1633837924 
 *  which is 0x61626364. Note that the first character of the incoming 
 *  string will be the high byte of the resultant integer value.
 *
 *  Note that this implementation is not complete and actually unacceptable
 *  with converting string<->numeric. The problem is that string functions
 *  are extensively used and, therefore, a 0 is converted to a null byte, 
 *  causing unexpected results. 
 *
 * LONG STRING INPUTS
 *  At Keck we had a need to allow inputting of strings longer than the 
 *  maximum string allowed for a stringin record (40 bytes). To accommodate
 *  this, device support for the waveform record was implemented. This
 *  allows strings upto DRV_SERIAL_BUF_SIZE (currently defined in 
 *  drvSerial.h as 0x400 -> 1024). 
 *
 *  Note that the user is assumed to have connected the waveform record to 
 *  a subroutine record (or equivalent) so as to parse the incoming string 
 *  as desired.
 *
 *  
 * PARM STRING EXAMPLES 
 *    @/tyCo/1 <timeout>
 *    this would be a record, associated with the link on '/tyCo/1' (which 
 *    is the vxworks stream associated with an on board serial port), that
 *    allows control of the i/o timeout for asynchronous i/o.
 *
 *    @/tyCo/1
 *    this would have to be a stringout record as they are the only records 
 *    for which a command/prompt is not necessary.
 *
 *    @/tyCo/1 <CHN1><001,%f>
 *    this would be an AI record for which a remote device would respond
 *    to the prompt 'CHN1' with '001,' followed by a floating point value.
 *    Note that the angle brackets are necessary delimiters however they 
 *    are stripped off and never seen by drvAscii. At this time, Jan 97,
 *    there is no provision for an escape code for angle brackets.
 *
 *    @/tyCo/1 <CHN1><%4k%f>
 *    same as the previous example except the '001,' will be ignored (ie.
 *    pattern matching will not occur). Note that all characters in an 
 *    input stream must be consumed or the stream is rejected. So if a
 *    remote device returns a string of characters before and/or after 
 *    a numeric string then those strings must be specified in the response
 *    format string. If the before or after strings can vary then it is
 *    necessary to input to a stringin or waveform record and process the
 *    stream with a subroutine.
 *
 *    @/tyCo/1 <CHN1><%*k>
 *    this is invalid as an input record cannot ignore all characters.
 * 
 *  A word of warning! Be very careful how one uses '%nc' as '%c' does
 *  not function the same as '%1c' and '%*c', with Vxworks implementation
 *  of sscanf. In actual fact '%1c' and '%*c' function as though they were
 *  '%1s'. I also believe that '%[^...]' is incorrect. 

 *  If one has the following string as input "1RA\r*@\r" then only the 
 *  following response  format string will succeed "%2t1RA%*[*]%s". These 
 *  will not "%2t1RA%*c%s", "%2t1RA%*[^*]%s". The problem is that leading 
 *  whitespace is incorrectly handled by sscanf.
 *
 *   
 * PROCESSING
 *  When an Ascii SIO record processes, the devAscii i/o function passes 
 *  drvAscii a pointer to an asynchronous IO structure and a pointer to an 
 *  output value. drvAscii formats an ascii string, as per the format 
 *  registered on record init as specified in the record's parm string 
 *  (INP or OUT field), setups up a callback routine, queues the string 
 *  plus port specific info, to the appropriate transmit task in drvSerial,
 *  and returns a status value to devAscii.

 *  If a response is expected then the status is set to
 *  S_drvAscii_AsyncCompletion (only occurs for input records) causing 
 *  devAscii to mark the record's PACT=true. If and when the remote device 
 *  responds the response string is parsed, as per the specified response 
 *  string format and, in the case of input records, the devAscii 
 *  asynchronous callback routine is called. 
 *
 *  If the write/read cycle fails then the asynchronous callback routine is
 *  passed an error status such that devAscii sets the records alarm state.
 *
 *  The async callback routine sets the records rval (val for stringin) 
 *  then causes the record to be processed again, which ultimately will 
 *  clear the PACT.
 * 
 *
 *
 *  Modification history:
 *
 *  Aug 2002 by A. Honey
 *    1. Significant modifications, within drvAscii.c, in regards to 
 *     the handling of synchronization semaphores so as to alleviate 
 *     potential loss of read/write synchronization. Although the 
 *     synchronization problem was infrequent it sometimes required a 
 *     processor reboot in order to correct the problem. Hopefully, 
 *     synchronization will now be auto-magically re-established.
 *
 *    2. Analog records may now have 'REAL' specified in their parm fields,
 *     after the link specification and before the first prompt format
 *     field. 
 *
 *     If an analog input record has the REAL attribute then the value 
 *     returned form the remote device is written into the record's VAL
 *     field and RVAL/ESLO conversions are bypassed. Note that also 
 *     bypasses the 'slope' record behavior. Similarly, analog output
 *     records will have their VAL values output to the remote device
 *     rather than their RVAL values. In general this eliminates the
 *     conversions to/fro real and integer which caused loss of
 *     precision, as well as, reducing the considerable complexity in using
 *     drvAscii for analog records.
 *
 *    3. The format-string delimiters no longer have to be '<' and '>'
 *     for all records. Now the first character follwing the link
 *     specification is assumed to be the field delimiter, with the pairs
 *     '<>', '()', '{}', and '[]' assumed, when the left hand delimiter
 *     is encountered. These are now valid (on a record-by-record basis):
 *                  @/tyco/0 <status?> <%s>
 *                  @/tyco/0 (status?) (%s)
 *                  @/tyco/0 !status?! !%s!
 *                  @/tyco/0 *status?* *%s*
 *                  @/tyco/0 Xstatus?X X%sX
 *
 *    4. User-specified framing routines can be specified.
 *     With this release one can create their own input and output
 *     framing routines and have them override the default getFrame()
 *     and putFrame() routines. This allows one to do more sophisticated
 *     packet framing (e.g. a simple checksum could be added/checked).
 *
 *     To specify special framing routines one must download the library,
 *     they created, containing those routine then register the 
 *     functions with drvAsciiSetTxFunc() and drvAsciiSetRxFunc(), all
 *     prior to iocInit. Note that a different set of framing routines 
 *     can be registered for each serial link. Also note that there are
 *     special requirements imposed on the framing routines. An example
 *     exists within drvAscii.c
 *     
 *    5. More extensive control of debugging information via link-specific
 *     debug records and via a drvAscii global variable drvAsciiDebugLevel.
 *
 *    6. Format specifications may now have embedded hex or octal bytes
 *     Said bytes must be of the form '\xnn' or '\ooo'. Those bytes are
 *     translated when the format specs are parsed during record init.
 *     For instance a prompt format such as <\x58\x59\x5a?> will result 
 *     in the string 'XYZ?' being output to the remote device.
 *     
 *    7. All numeric escape codes, that exist in output or input data
 *     streams, will be automatically translated. For instance a
 *     stringIn record with this prompt and response format '<%s?><%s>' 
 *     can be manipulated with "caput stringIn '\x58'" to result
 *     in a prompt of 'X?' being transmitted. This should simplify record
 *     manipulation for those apps which talk to multi-dropped devices
 *     whose addresses are leading non-printing ASCII bytes. The numeric
 *     escape codes need not result in a printable ASCII characters, 
 *     that is, "caput stringIn '\x81'" is valid. However, Beware that 
 *     drvAscii uses sprintf and sscanf so embedding a null byte will 
 *     cause unexpected behavior.
 *
 */
 
/*
 * ANSI C
 */
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<ctype.h>
#include	<limits.h>


/*
 * EPICS
 */
#include        <epicsAssert.h>
#include	<alarm.h>
#include	<cvtTable.h>
#include	<dbDefs.h>
#include	<dbAccess.h>
#include	<dbFldTypes.h>
#include        <recSup.h>
/*
#include        <recGbl.h>
*/
#include	<devSup.h>
#include	<dbScan.h>
#include	<link.h>
#include	<aiRecord.h>
#include	<aoRecord.h>
#include	<biRecord.h>
#include	<boRecord.h>
#include	<mbbiRecord.h>
#include        <mbbiDirectRecord.h>
#include        <mbboRecord.h>
#include        <mbboDirectRecord.h>
#include        <stringinRecord.h>
#include        <stringoutRecord.h>
#include        <longinRecord.h>
#include        <longoutRecord.h>
#include	<waveformRecord.h>
#include        <devLib.h>

/*
 * Ascii driver
 */
#include "drvSerial.h"
#include "drvAscii.h" 

#define S_devAscii_Ok            0
#define S_devAscii_dontConvert   2
#define S_devAscii_badRec        3
 
#define devInitPassBeforeDevInitRec 0
#define devInitPassAfterDevInitRec 1

/* 
 *  The following structure is created for each record.
 *  The resulting structure is pointed to by the record's dpvt field. This 
 *  is essentially the handle used by the driver (drvAscii.c) for processing 
 *  the request.
 */ 
typedef struct ascii_dev_priv { 
  void        *pRec;  /* record pointer */
  long        sigNum_arraySize; /* NOT YET IMPLEMENTED 970106 */
  IOSCANPVT   spvt;   /* support interrupt from this parameter    */
  drvAsyncIO  aio;    /* async io structure. included in this structure is:*/
                      /* id - (drvSioLinkId) drvSerial structure for tx/rx */
		      /* pCB - the device callback routine for async       */
                      /* completion pIoDoneArg - (arg for pIoDoneCB) which */
                      /* is a pointerto the record (*pAppDrvPrivate) -     */
                      /* drvAscii callback function used for proccessing   */
                      /* responses to output commands                      */
}devAsciiPriv; 
 
LOCAL drvAsyncUpdateCallBack devAsciiUpdate;

/*
 *      DEVICE ENTRY TABLES
 */
LOCAL long devAsciiInit();

typedef struct {
  long		number;
  DEVSUPFUN	report;
  DEVSUPFUN	init;
  DEVSUPFUN	init_record;
  DEVSUPFUN	get_ioint_info;
  DEVSUPFUN	read_write;
} INTEGERDSET;

typedef struct {
  long		number;
  DEVSUPFUN	report;
  DEVSUPFUN	init;
  DEVSUPFUN	init_record;
  DEVSUPFUN	get_ioint_info;
  DEVSUPFUN	read_write;
  DEVSUPFUN	special_linconv;
} FLOATDSET;

/*
 *  Device entry table for AI records
 */
LOCAL long aiInitRec();
LOCAL long aiGetIoIntInfo();
LOCAL long aiRead();
LOCAL long aiLinearConvert();
LOCAL devIoDoneCallBack aiReadAsyncCompletion;
LOCAL devRealIoDoneCallBack aiReadRealAsyncCompletion;

FLOATDSET devAiAscii = {
  6,
  NULL,
  devAsciiInit,
  aiInitRec,
  aiGetIoIntInfo,
  aiRead,
  aiLinearConvert
};

/*
 *  Device entry table for AO records
 */
LOCAL long aoInitRec();
LOCAL long aoWrite();
LOCAL long aoLinearConvert();
FLOATDSET devAoAscii = {
  6,
  NULL,
  devAsciiInit,
  aoInitRec,
  NULL,
  aoWrite,
  aoLinearConvert
};

/*
 *  Device entry table for BI records
 */

LOCAL long biInitRec();
LOCAL long biGetIoIntInfo();
LOCAL long biRead();
LOCAL devIoDoneCallBack biReadAsyncCompletion;
INTEGERDSET devBiAscii = { 
  5,
  NULL,
  devAsciiInit,
  biInitRec,
  biGetIoIntInfo,
  biRead
};

/*
 *  Device entry table for BO records 
 */

LOCAL long boInitRec();
LOCAL long boWrite();
INTEGERDSET devBoAscii = { 
  5,
  NULL,
  devAsciiInit,
  boInitRec,
  NULL,
  boWrite
};

/*
 *  Device entry table for MBBI records
 */

LOCAL long mbbiInitRec();
LOCAL long mbbiGetIoIntInfo();
LOCAL long mbbiRead();
LOCAL devIoDoneCallBack mbbiReadAsyncCompletion;
INTEGERDSET devMbbiAscii = {
  5,
  NULL,
  devAsciiInit,
  mbbiInitRec,
  mbbiGetIoIntInfo,
  mbbiRead
};

/*
 *  Device entry table for MBBO records
 */

LOCAL long mbboInitRec();
LOCAL long mbboWrite();
INTEGERDSET  devMbboAscii = { 
  5,
  NULL,
  devAsciiInit,
  mbboInitRec,
  NULL,
  mbboWrite
};

/*
 *  Device entry table for MBBI direct records
 */
LOCAL long mbbiDirectInitRec();
LOCAL long mbbiDirectGetIoIntInfo();
LOCAL long mbbiDirectRead();
LOCAL devIoDoneCallBack mbbiDirectReadAsyncCompletion;
INTEGERDSET devMbbiDirectAscii = {
  5,
  NULL,
  devAsciiInit,
  mbbiDirectInitRec,
  mbbiDirectGetIoIntInfo,
  mbbiDirectRead
};

/*
 *  Device entry table for MBBO direct records
 */

LOCAL long mbboDirectInitRec();
LOCAL long mbboDirectWrite();
INTEGERDSET  devMbboDirectAscii = {
  5,
  NULL,
  devAsciiInit,
  mbboDirectInitRec,
  NULL,
  mbboDirectWrite
};

/*
 *  Device entry table for stringin records
 */
LOCAL long siInitRec();
LOCAL long siGetIoIntInfo();
LOCAL long siRead();
LOCAL devIoDoneCallBack siReadAsyncCompletion;
INTEGERDSET  devSiAscii = {
  5,
  NULL,
  devAsciiInit,
  siInitRec,
  siGetIoIntInfo,
  siRead
};

/*
 *  Device entry table for stringout records
 */
LOCAL long soInitRec();
LOCAL long soWrite();
INTEGERDSET  devSoAscii = {
  5,
  NULL,
  devAsciiInit,
  soInitRec,
  NULL,
  soWrite
};

/*
 *  Device entry table for longin records
 */
LOCAL long liInitRec();
LOCAL long liGetIoIntInfo();
LOCAL long liRead();
LOCAL devIoDoneCallBack liReadAsyncCompletion;
INTEGERDSET devLiAscii = {
  5,
  NULL,
  devAsciiInit,
  liInitRec,
  liGetIoIntInfo,
  liRead
};

/*
 *  Device entry table for longout records
 */
LOCAL long loInitRec();
LOCAL long loWrite();
INTEGERDSET devLoAscii = {
  5,
  NULL,
  devAsciiInit,
  loInitRec,
  NULL,
  loWrite
};


/*
 *  Device entry table for waveform (long stringin) records
 */
LOCAL long wfInitRec();
LOCAL long wfGetIoIntInfo();
LOCAL long wfRead();
LOCAL devIoDoneCallBack wfReadAsyncCompletion;
INTEGERDSET  devWfAscii = {
  5,
  NULL,
  devAsciiInit,
  wfInitRec,
  wfGetIoIntInfo,
  wfRead
};

extern int drvAsciiDebugLevel;


/*
 * ---------------------------------------------------------------------------
 *  copyString - copys one string to another upto a maximum length
 *     or a specified character. This is used by parseAsciiAddress.
 */
LOCAL void copyString ( char *pStr,
			char eos,
			char *pDest,
			long pDestLength )
{
  long index = 0;

  while ( index < pDestLength 
	  && 
	  pStr[index] != eos ) {

    pDest[index] = pStr[index];
    index++;
  }
  
  pDest[index] = '\0';
}


/*
 * ---------------------------------------------------------------------------
 *  getChannel - returns the 'channel' which is the value you for 'S' in
 *   the usual '#Cn Sm @', albeit '#Cn' is not relevant. Note that 'Am' 
 *   can be in place of 'Sm' but is not yet supported (as of 970117) and
 *   is intended for arrays of values.
 */
LOCAL long getChannel( char *bfr ) 
{
  char *pStart;
  long number = 0;

  pStart = bfr;

  while ( pStart 
	  && 
	  (*pStart != 'S' && *pStart != 'A') ) 
    pStart--; 

  if ( !pStart ) { 

	    number = -1;
	
  } else if ( *(pStart-1) != ' ' ) {

    number = -1;

  } else {

    pStart++;
    sscanf( bfr, "%ld", &number );
  }

  return number;
}


/*
 * ---------------------------------------------------------------------------
 * parseAsciiAddress() - parses a record's parm field into:
 *     a 'filename' (port designation passed to drvAscii
 *     a command/prompt string or special command name
 *     a command response format string
 *     a readback prompt string (output value's initial value request ) 
 *     a readback response format string
 *
 *     Note that only the 'filename' must exist for all record types.
 *     The type of record determines whether or not the command/prompt 
 *     and command response format strings are required. The readback
 *     prompt and response format strings are always optional.
 * 
 *     The resultant strings are copied into the appropriate char array
 *     within 'drvCmndArg *pCmndArgs'
 *
 */
LOCAL long parseAsciiAddress ( const char *dtype,
			       char       *pAddr,
			       char       *pFileName,
			       unsigned    maxFileName,
			       drvCmndArg *pCmndArgs,
			       long       *sigNum_arraySize)
{
  int  status;
  int  done = 0;
  int  offset = 0;

  char format[32];
  char tempStr[256];
  char *pStart = NULL,
       *pEnd = NULL,
       *ptr,
       cEnd, cStart;
    
  if ( drvAsciiDebugLevel & 1 )
    printf("parseAsciiAddress(%s)\n", pAddr );

  /* 
   *  Ensures that all strings are null terminated. This is protection
   *  against an undefined string.
   */
  pCmndArgs->cmndPrompt[0] =
  pCmndArgs->cmndFormat[0] =
  pCmndArgs->rbvPrompt[0] =
  pCmndArgs->rbvFormat[0] = '\0';
  
  /*
   *  Parse the 'file' name (ie. the serial link identification string).
   *  If it doesn't exist then abort.
   */
  assert( maxFileName>=1 );
  assert( pFileName );
 
  sprintf( format, "%%%ds",maxFileName-1 );
  status = sscanf( pAddr, format, pFileName );

  if ( status < 1 ) {


    return S_drvAscii_badParam;

  } else {
    /*
     *  The link designation must begin with '@' and end at
     *  the first whitespace character.
     */
    pStart = pAddr;
    
    while ( !isspace( *pStart ) && (*pStart != '\0')) pStart++;

    if ( *pStart == '\0' )
    /* There is nothing else to do. */
      return S_devAscii_Ok;

    /* 
     *  Determine if there are any special parsing specifications.
     */    
    do {

      sscanf( pStart, "%s", tempStr );

      ptr = tempStr;

      while ( *ptr != '\0' ) { *ptr = (char) toupper( *ptr ); ptr++; };
      
      if ( strncmp( tempStr, "REAL", 3 ) == 0 ) {

	/*  Output values are to be taken from val not rval or input 
	 *  values are to be written to val and not rval. This is to
	 *  bypass the default analog record processing rval->val. This 
	 *  also eliminates the use of 'slope' processing.
	 */
	offset = 5;
	pCmndArgs->passThru = 1;

      } else done = 1;

      if ( !done )
	pStart += offset;

    } while ( !done );

    /*
     *  The first character after the special parsing designations
     *  is the field delimiter. The special case is the backward
     *  compatiblity issue that '<' and '>' are a matched set, and
     *  these pairs are assumed: '{}', '[]', '()'.
     */
    while ( isspace( *pStart ) && (*pStart != '\0') ) pStart++;
    cStart = *pStart;
    
    if ( cStart == '<' )
      cEnd = '>';

    else if ( cStart == '{' )
      cEnd = '}';

    else if ( cStart == '[' )
      cEnd = ']';

    else if ( cStart == '(' )
      cEnd = ')';

    else
      cEnd = cStart;
 
    if ( cStart == '"' ) {

      errPrintf( S_drvAscii_badParam, __FILE__, __LINE__,
		 "'\"' double quote delimiters are not allowed!\n");
      return S_drvAscii_badParam;
    }


    /* Locate the first command/prompt string delimiter. */
    pStart = strchr( pAddr, cStart );

    /* Attempt to locate the signal number/array size delimiter. */
    pEnd = strchr( pAddr, '@' );     
 
    if ( pStart == NULL ) {

	return S_drvAscii_badParam;
    
    } else if ( pEnd && !(pEnd > pStart) ) { 
      /* 
       *  The @ is not embedded in a command/prompt so assume it delimits 
       *  a signal number or array size.
       *
       *  NOTE THIS ALL FAILS IF @ IS PART OF THE SERIAL PORT NAME!!!
       */
      *sigNum_arraySize = getChannel( pEnd );

      if ( *sigNum_arraySize < 0 ) {

	*sigNum_arraySize = 0;

	return S_drvAscii_badParam;
      }
    }
 
    /*
     *  Locate the command/prompt string. If it doesn't exist and the 
     *  record is not a string record then abort (ie. all records but 
     *  string records require a command/prompt string).
     */
    if ( pStart ) {
      ptr = pStart;
      ptr++;
      pEnd = strchr( ptr, cEnd );
      
      if ( !pEnd ) {

	return S_drvAscii_badParam;
      }

      copyString( ++pStart, cEnd,
		  pCmndArgs->cmndPrompt,
		  min( (int)((int)pEnd - (int)pStart),
		       sizeof( pCmndArgs->cmndPrompt )-1 ) ); 
    }

    /*  Find any command response format string. */
    if ( ++pEnd == '\0' )
      return S_drvAscii_Ok;

    pStart = strchr( pEnd, cStart );


    if ( pStart ) {

      ptr = pStart;
      ptr++;
      pEnd = strchr (ptr, cEnd );

      if ( !pEnd ) {

	return S_drvAscii_badParam;
      }

      copyString( ++pStart, cEnd,
		  pCmndArgs->cmndFormat,
		  min( (int)((int)pEnd - (int)pStart),
		       sizeof( pCmndArgs->cmndFormat )-1 ) );
    }
    
    /* 
     *  Find any readback prompt string. This would typically exist for
     *  an output for which an initial value can be obtained.
     */
    if ( ++pEnd == '\0' ) 
      return S_drvAscii_Ok;

    pStart = strchr( pEnd, cStart );
      
    if (pStart) {

      ptr = pEnd;
      ptr++;
      pEnd = strchr( ptr, cEnd );

      if ( !pEnd ) {

	return S_drvAscii_badParam;
      }

      copyString( ++pStart, cEnd,
		  pCmndArgs->rbvPrompt,
		  min( (int)((int)pEnd - (int)pStart),
		       sizeof( pCmndArgs->rbvPrompt )-1 ) );
    }
   
    /* 
     *  Find any readback response format string. This would typically
     *  exist for formatting a response for a request for the initial
     *  value for an output record.
     */
    if ( ++pEnd == '\0' ) 
      return S_drvAscii_Ok;

    pStart = strchr( pEnd, cStart );
   
    if (pStart) {

      ptr = pEnd;
      ptr++;
      pEnd = strchr( ptr, cEnd );

      if (!pEnd) {

	return S_drvAscii_badParam;
      }

      copyString( ++pStart, cEnd,
		  pCmndArgs->rbvFormat,
		  min( (int)((int)pEnd - (int)pStart),
		       sizeof( pCmndArgs->rbvFormat )-1 ) );
    }
  }
  
  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * devAsciiInitPrivate() - this function validates a record's parm field
 *     and establishes the drvAscii info required for processing the record.
 *     This includes the callback info required for asynchronous completion. 
 */
LOCAL long devAsciiInitPrivate( const char   *dtype, 
				struct link  *pLink, 
				void         *pRec, 
				devAsciiPriv **ppPriv,
				long         *sigNum_arraySize )
{	
  int	       status;
  char	       fileName[sizeof(pLink->value)];
  drvCmndArg   *pCmndArgs;
  devAsciiPriv *pPriv = NULL;

  /*  If the record does not have INST_IO defined then abort. */
  if ( pLink->type != INST_IO ) {

    status = S_db_badField;

    recGblRecordError ( status, pRec, 
		        ": Address type must be type \"instrument\"" );
    return status;
  }
 
  if ( drvAsciiDebugLevel & 1 )
    printf("devAsciiInitPrivate(%s,%s),",dtype, ((aiRecord *)pRec)->name );

  /* 
   *  Allocate the structure which will ultimately be pointed to by the 
   *  record's dpvt field. 
   */
  pPriv = calloc( 1, sizeof( *pPriv ) );

  if ( pPriv == NULL ) {
    /* 
     *  This error may be catastrophic as pPriv being NULL is not 
     *  appropriately captured.
     */
    *ppPriv = NULL;
    status = S_dev_noMemory;

    recGblRecordError ( status, pRec, 
			": no room for device private" );
    return status;
  }

  /*  Init the scan private info. */
  scanIoInit( &pPriv->spvt );

  /*  Ensure that the record's dpvt pointer is set. */
  *ppPriv = pPriv;

  /*  Allocate space for the strings embedded in the parm field. */
  pCmndArgs = calloc( 1,sizeof( drvCmndArg ) );

  if ( !pCmndArgs ) {

    status = S_dev_noMemory;

    recGblRecordError ( status, pRec, 
			": no room for command args" );
    return status;
  }

  /*  Parse the parm field. */
  status = parseAsciiAddress( dtype,
			      pLink->value.instio.string,
			      fileName,
			      sizeof(fileName),
			      pCmndArgs,
			      sigNum_arraySize );
  if ( status ) {

    /*  The record's parm field is invalid! */
    free( pCmndArgs );

    recGblRecordError (status, pRec, 
		       ": Syntax error in PARM string");

    return status;
  }

  /*
   *  Setup the drvAscii info and ensure a comms link is established with
   *  drvSerial.
   */
  status = drvAsciiCreateSioLink( dtype,  
				  fileName, 
				  pCmndArgs, 
				  &pPriv->aio.id, 
				  devAsciiUpdate, 
				  pPriv   );
  if ( status ) {

    /* The link was not successfully established! */  
    recGblRecordError ( status, pRec, 
			": failed to create serial link" );
    return status;
  } 

  return S_devAscii_Ok;
} 


/*
 * ---------------------------------------------------------------------------
 * devAsciiUpdate() 
 */
LOCAL void devAsciiUpdate(void *pArg, 
			  long status, 
			  long value)
{
  devAsciiPriv 	*pPriv = (devAsciiPriv *)pArg;
  
  scanIoRequest( pPriv->spvt );
}

/*
 * ---------------------------------------------------------------------------
 * devAsciiInit() 
 */
LOCAL long devAsciiInit(unsigned pass)
{
  long	status;
  
  switch ( pass ) {

  case devInitPassBeforeDevInitRec: 
    status = S_devAscii_Ok;
    break;
    
  case devInitPassAfterDevInitRec: 
    /*
     *  Initiate scanning on all links. 
     */
    status = drvAsciiInitiateAll();
    break;
    
  default: /*  Not expected. */
    status = S_devAscii_Ok;
    break;
  }
  
  return status;
}


/*
 * ---------------------------------------------------------------------------
 * reportRecCheck() - determines if the record's private info is valid 
 *     or not. If validity cannot be ascertained and the record's undefined 
 *     fields is not set then a record error message is generated. In 
 *     addiion the record's alarm severity is set to UDF, it's udf field 
 *     is set and it's pact is set. This effectively inhibits the record 
 *     from processing in the future.       
 */     
LOCAL long reportBadRec(
		       struct dbCommon *pRec,
		       devAsciiPriv	*pPriv
		       )
{
  /* 
   *  If the device private structure was never created then set
   *  the record's fault states.
   */
  if ( pPriv == NULL ) {

    /*
     *  Generate and error message and set the record's alarm state.
     *  This is only done if udf is not true to ensure a flood of 
     *  messages does not occur.
     */
    if ( pRec->udf == FALSE ) {

      recGblRecordError( S_db_badField, (void *)pRec,
			 "devAscii: the record failed init so it has been disabled" );
    }


    recGblSetSevr( pRec, UDF_ALARM, INVALID_ALARM );
 
    /*
     *  Ensure that the record is disabled.
     */
    pRec->pact = TRUE;
    pRec->udf  = TRUE;

    return S_devAscii_badRec; 
  }

  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * reportWriteFail() - determines if the read failed because the record
 *     is faulty. 
 *
 *     If the record is faulty and the record's undefined field is not
 *     set then a record error message is generated. In addition the 
 *     record's alarm severity is set to UDF, it's udf field is set and 
 *     it's pact is set. This effectively inhibits the record from pro-
 *     cessing in the future. 
 *
 *     If the write failed for any other reason then a write error message 
 *     is generated and the record's alarm severity is set to a WRITE alarm.
 */     
LOCAL long reportWriteFail(
			   struct dbCommon *pRec,
			   long status
			   )
{
  if ( (status == S_drvAscii_badParam) ||  
       (status == S_devAscii_badRec)      ) {
    /*
     *  There is something wrong with the record so mark
     *  it so it will never attempt a write in the future.
     */
    if ( pRec->udf == FALSE ) {
      /*
       *  Generate an error message and set the record's alarm state.
       */
      recGblRecordError( S_db_badField, (void *)pRec,
			 "devAscii: Invalid record; the record has been disabled" );

    }

    recGblSetSevr( pRec, UDF_ALARM, INVALID_ALARM );

    pRec->pact = TRUE;
    pRec->udf  = TRUE;

    return S_db_badField;

  } else {
    /* 
     *  The write failed so set the record into alarm.
     */
    recGblRecordError( S_drvAscii_dataErr , (void *)pRec,
		       "devAscii: write failed" );

    recGblSetSevr( pRec, WRITE_ALARM, MAJOR_ALARM );
  }

  return status;
}


/*
 * ---------------------------------------------------------------------------
 * reportReadFail() - determines if the read failed because the record
 *     is faulty. 
 *
 *     If the record is faulty and the record's undefined field is not
 *     set then a record error message is generated. In addition the 
 *     record's alarm severity is set to UDF, it's udf field is set and 
 *     it's pact is set. This effectively inhibits the record from pro-
 *     cessing in the future. 
 *
 *     If the write failed for any other reason then a write error message 
 *     is generated and the record's alarm severity is set to a READ alarm.
 */     
LOCAL long reportReadFail(
			   struct dbCommon *pRec,
			   long status
			   )
{
  if ( (status == S_drvAscii_badParam) ||  
       (status == S_devAscii_badRec)      ) {
    /*
     *  There is something wrong with the record so mark
     *  it so it will never attempt a write in the future.
     */
    if ( pRec->udf == FALSE ) {
      /*
       *  Generate an error message and set the record's alarm state.
       */
      recGblRecordError( S_db_badField, (void *)pRec,
			 "devAscii: Invalid record; the record has been disabled" );

      recGblSetSevr( pRec, UDF_ALARM, INVALID_ALARM );
    }

    pRec->pact = TRUE;
    pRec->udf  = TRUE;

    return S_db_badField;

  } else {
    /* 
     *  The read failed so set the record into alarm.
     */
    recGblRecordError( S_drvAscii_dataErr , (void *)pRec,
		       "devAscii: read failed" );

    recGblSetSevr( pRec, READ_ALARM, MAJOR_ALARM );
  }

  return status;
}


/* 
 * ---------------------------------------------------------------------------
 * devAsciiWriteAsyncCompletion() -  Asynchronous write completion routine.
 */
LOCAL void devAsciiWriteAsyncCompletion( void *pArg, long status, long value )
{
  struct dbCommon       *pRec = (struct dbCommon *)pArg;
  struct rset           *prset = (struct rset *)(pRec->rset);
 
  if ( drvAsciiDebugLevel & 32 ) printf("devAsciiWriteAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The write completed succesfully.
     */
    pRec->udf = FALSE;

  } else {
    /*
     *  The read failed so report the failure.
     */
    reportWriteFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}


/*
 * ---------------------------------------------------------------------------
 * aiInitRec() - initializes an AI record. This setups up all the drvAscii
 *     info, drvSerial info, and asynchronous completion callback info,
 *     all of which is attached to the record's device private field.
 */     
LOCAL long aiInitRec (struct aiRecord *pAi) 
{
  long           status;
  devAsciiPriv   *pPriv = NULL;

  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "REAL_IN", 
				&pAi->inp, pAi, &pPriv, 
				&(pPriv->sigNum_arraySize) );

  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   *  The response format can be missing, in which case '%f' is assumed.
   *
   *  The response format may be string ('%nc' or '%ns') but 'n' must not
   *  be greater than 4. With a string specification the input ascii stream
   *  will be converted to an integer value (eg. "abcd" -> 0x61626364 ->
   *  1633837924). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_STRING) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pAi,
		       "devAscii: (Ai init_record) Illegal format spec");

    pAi->dpvt = (void *) NULL;

    pAi->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndInfo.dataType = RBF_STRING;

    pAi->udf  = FALSE;

    pAi->dpvt = (void *) pPriv;

    /*  Set linear conversion slope. */
    pAi->eslo = pAi->eguf - pAi->egul;

    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    if (pPriv->aio.id.pCmndArg->passThru)
      pPriv->aio.pIoDoneCB = aiReadRealAsyncCompletion;
    else
      pPriv->aio.pIoDoneCB = aiReadAsyncCompletion;

    pPriv->aio.pIoDoneArg  = pAi;
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * aiRead() - analog input routine
 */
LOCAL long aiRead (struct aiRecord * pAi)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pAi->dpvt;

  long		status;
  long       	rval;
  
  double        fval;

  if ( drvAsciiDebugLevel & 32 ) printf("aiRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pAi->pact == TRUE ) {

    if ( pAi->udf )
      return S_devAscii_badRec; 

    else {

      if ( pPriv->aio.id.pCmndArg->passThru )
	return S_devAscii_dontConvert;
      else
	return S_devAscii_Ok; 
    }
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pAi, 
			 (devAsciiPriv	*)  pAi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous set the pact.
   */
  if ( pPriv->aio.id.pCmndArg->passThru )
    status = drvAsciiRealIo( &pPriv->aio, &fval );
  else
    status = drvAsciiIntIo( &pPriv->aio, &rval );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pAi->pact = TRUE; 

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok) {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    if ( pPriv->aio.id.pCmndArg->passThru ) {

      pAi->val = fval;

      status = S_devAscii_dontConvert;

    } else {

      pAi->rval = rval;

      status = S_devAscii_Ok;
    }

    pAi->udf  = FALSE;

    return status;

  } else {

    return reportReadFail( (struct dbCommon *)pAi, status );

  }
}

/* 
 * ---------------------------------------------------------------------------
 *  aiReadAsyncCompletion() - Asynchronous read completion routine for AI
 */
LOCAL void aiReadAsyncCompletion(void *pArg, long status, long value)
{
  struct aiRecord      *pAi = (struct aiRecord *)pArg;
  struct dbCommon      *pRec = (struct dbCommon *)pArg;
  struct rset          *prset = (struct rset *)(pRec->rset);

  if ( drvAsciiDebugLevel & 32 ) printf("aiReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input data.
     */
    pAi->rval = value;

    pAi->udf  = FALSE;

  } else {

    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}

/* 
 * ---------------------------------------------------------------------------
 *  aiReadRelaAsyncCompletion() - Asynchronous read completion routine for AI
 */
LOCAL void aiReadRealAsyncCompletion(void *pArg, long status, double value)
{
  struct aiRecord      *pAi = (struct aiRecord *)pArg;
  struct dbCommon      *pRec = (struct dbCommon *)pArg;
  struct rset          *prset = (struct rset *)(pRec->rset);

  if ( drvAsciiDebugLevel & 32 ) printf("aiReadRealAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input data.
     */
    pAi->val = value;

    pAi->udf  = FALSE;

  } else {

    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}

/*
 * ---------------------------------------------------------------------------
 * aiLinearConvert() - provides for eslo calc when eguf or egul
 *     are modified.
 */
LOCAL long aiLinearConvert (struct aiRecord *pAi, int after)
{
  if( !after ) {

    return( S_devAscii_Ok );
  }
  
  /*  Set linear conversion slope. */
  pAi->eslo = pAi->eguf - pAi->egul;
  
  return( S_devAscii_Ok );
}

/*
 * ---------------------------------------------------------------------------
 * aiGetIoIntInfo() - Used for obtaining the scan private info
 */
LOCAL long aiGetIoIntInfo(int             cmd,
			  struct aiRecord *pAi,
			  IOSCANPVT       *ppvt)
{
  devAsciiPriv *pPriv = (devAsciiPriv *) pAi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;
  
  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * aoInitRec() - initializes AO records. This setups up all the drvAscii
 *     and drvSerial info, all of which is attached to the record's device 
 *     private field.
 */
LOCAL long aoInitRec (struct aoRecord *pAo) 
{
  devAsciiPriv	*pPriv = NULL;
  long           status;
  long		 rval;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "REAL_OUT", 
				&pAo->out, pAo, &pPriv,
				&(pPriv->sigNum_arraySize) );

  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt must exist. However a data type specification need not 
   *  exist, in which case '%f' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format strings 
   *  must be of the form '%*' ). 
   *
   *  Output formats of '%nc' and '%ns' are valid but 'n' must not be 
   *  greater than 4. That is, a 4 byte integer will be converted to an 
   *  output string of 4 ascii chars (eg. 1633837924 -> 0x61626364 -> 
   *  "abcd"). Note that the resultant ascii chars may not be printable 
   *  ascii chars (eg 23200612 -> 0x01620364 -> ^Ab^Cd, where ^A is 
   *  control A -> 0x01).
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0)  
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       ||
      ((pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR) 
       &&
        (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING)
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pAo,
		       "devAscii: (Ao init_record) Illegal format spec");

    pAo->dpvt = (void *) NULL;

    pAo->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType = RBF_STRING;

    pAo->udf  = FALSE;

    pAo->dpvt = pPriv;

    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pAo;
    
    /* 
     *  Set linear conversion slope.
     */
    pAo->eslo = pAo->eguf - pAo->egul;
    
    if ( status == OK
	 && 
	 strlen( pPriv->aio.id.pCmndArg->rbvPrompt ) > 0 )

      /*  If possible get an initial value for the record. */
      if ( drvAsciiReadOutput( &pPriv->aio.id, &rval ) != OK 
	   || 
	   pAo->pini ) {

	status = S_devAscii_dontConvert;

      } else {

	/*  Set the initial value. */
	pAo->rval = (long) rval;

      } else status = S_devAscii_dontConvert;
  }

  return status; 
}

/*
 * ---------------------------------------------------------------------------
 * aoWrite() - analog output routine
 */
LOCAL long aoWrite (struct aoRecord * pAo)
{
  devAsciiPriv 	*pPriv = (devAsciiPriv *)pAo->dpvt;

  long		status;

  double        fval;

  if ( drvAsciiDebugLevel & 32 ) printf("aoWrite\n");

  /* 
   *  If a write is already in progress, or the record is locked-out,
   *  then exit.
   */
  if ( pAo->pact == TRUE ) {

    if ( pAo->udf )
      return S_devAscii_badRec; 

    else {

      if ( pPriv->aio.id.pCmndArg->passThru )
	return S_devAscii_dontConvert;
      else
	return S_devAscii_Ok; 
    }
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pAo, 
			 (devAsciiPriv	*)  pAo->dpvt );

  if ( status != S_devAscii_Ok ) return status;
  
  /* 
   *  Perform the write. 
   */
  if ( pPriv->aio.id.pCmndArg->passThru ) {

    fval = pAo->oval;

    status = drvAsciiRealIo( &pPriv->aio, &fval );

  } else {

    fval = pAo->rval;

    status = drvAsciiRealIo( &pPriv->aio, &fval );
  }

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pAo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pAo, status );
  }

  pAo->udf = FALSE;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * aoLinearConvert() - provides for eslo calc when eguf or egul are modified.
 */
LOCAL long aoLinearConvert (struct aoRecord *pAo, int after)
{
  if( !after ) return( S_devAscii_Ok );

  /*  Set linear conversion slope. */
  pAo->eslo = pAo->eguf - pAo->egul;

  return( S_devAscii_Ok );
}


/*
 * ---------------------------------------------------------------------------
 * biInitRec() - initializes BI records. This setups up all the drvAscii
 *     info, drvSerial info, and asynchronous completion callback info, 
 *     all of which is attached to the record's device private field.
 */
LOCAL long biInitRec(struct biRecord *pBi) 
{
  long         status;
  devAsciiPriv *pPriv= NULL;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_IN", 
				&pBi->inp, pBi, &pPriv, 
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   *  The response format can be missing, in which case '%d' is assumed.
   *
   *  The response format may be char ('%nc') but 'n' must not
   *  be greater than 4. With a string specification the input ascii stream
   *  will be converted to an integer value (eg. "abcd" -> 0x61626364 ->
   *  1633837924). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_STRING) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pBi,
		       "devAscii: (Bi init_record) Illegal format spec");

    pBi->dpvt = (void *) NULL;

    pBi->udf = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndInfo.dataType = RBF_STRING;

    pBi->udf = FALSE;

    pBi->dpvt = (void *) pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = biReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pBi;
    
    pBi->mask = 1;
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * Used for obtaining the scan private info
 */
LOCAL long biGetIoIntInfo( int             cmd,
			   struct biRecord *pBi,
			   IOSCANPVT	   *ppvt)
{
  devAsciiPriv *pPriv = (devAsciiPriv *) pBi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;
  
  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * biRead() - BI input routine
 */
LOCAL long biRead (struct biRecord * pBi)
{
  devAsciiPriv 	*pPriv = (devAsciiPriv *)pBi->dpvt;
  long		val;
  long		status;
  
  if ( drvAsciiDebugLevel & 32 ) printf("biRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pBi->pact ) {

    if ( pBi->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pBi, 
			 (devAsciiPriv	*)  pBi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous set the pact.
   */
  status = drvAsciiIntIo( &pPriv->aio, &val );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pBi->pact = TRUE;

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok) {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    pBi->rval = val;

    pBi->udf  = FALSE;

    return S_devAscii_Ok;

  } else {

    return reportReadFail( (struct dbCommon *)pBi, status );
  }
}

/* 
 * ---------------------------------------------------------------------------
 *  biReadAsyncCompletion() - Asynchronous read completion routine for BI
 */
LOCAL void biReadAsyncCompletion(void *pArg, long status, long value)
{
  struct biRecord      *pBi = (struct biRecord *)pArg;
  struct dbCommon      *pRec = (struct dbCommon *)pArg;
  struct rset          *prset = (struct rset *)(pRec->rset);
  
  if ( drvAsciiDebugLevel & 32 ) printf("biReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input data.
     */
    pBi->rval = value;

    pBi->udf  = FALSE;

  } else {

    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}


/*
 * ---------------------------------------------------------------------------
 * boInitRec() - initializes BO records. This setups up all the drvAscii 
 *     and drvSerial info, all of which is attached to the record's  
 *     device private field.
 */
LOCAL long boInitRec(struct boRecord *pBo) 
{
  long          status;
  devAsciiPriv	*pPriv = NULL;
  long		rval;
   
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_OUT", 
				&pBo->out, pBo, &pPriv,
				&(pPriv->sigNum_arraySize) );
   
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt must exist. However a data type specification need 
   *  not exist,in which case '%f' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format 
   *  strings must be of the form '%*' ). 
   *
   *  Output formats of '%nc' and '%ns' are valid but 'n' must not
   *  be greater than 4. That is, a 4 byte integer will be converted 
   *  to an output string of 4 ascii chars (eg. 1633837924 -> 0x61626364
   *  -> "abcd"). Note that the resultant ascii chars may not be 
   *  printable ascii chars (eg 23200612 -> 0x01620364 -> ^Ab^Cd, 
   *  where ^A is control A -> 0x01).
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0)  
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
      ((pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR) 
       &&
        (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING)
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pBo,
		       "devAscii: (Bo init_record) Illegal format spec");

    pBo->dpvt = (void *) NULL;

    pBo->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType = RBF_STRING;

    pBo->udf  = FALSE;

    pBo->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pBo;
    
    pBo->mask = 1;
    
    if ( status == OK 
	 && 
	 strlen( pPriv->aio.id.pCmndArg->rbvPrompt ) > 0 ) {

      /*  If possible get an initial value for the record. */
      status = drvAsciiReadOutput( &pPriv->aio.id, &rval );
    
      if ( status == OK ) {

	/*  Set the initial value. */
	pBo->rval = (long) rval;
	
	status = S_devAscii_Ok;
      }
    }
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * boWrite() - BO output routine
 */
LOCAL long boWrite (struct boRecord * pBo)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *)pBo->dpvt;
  int		status;
  long		rval;
  
  if ( drvAsciiDebugLevel & 32 ) printf("boWrite\n");

  /* 
   *  If a write is already in progress, or the record is locked-out,
   *  then exit. 
   */
  if ( pBo->pact ) {
  
    if ( pBo->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pBo, 
			 (devAsciiPriv	*)  pBo->dpvt );

  if ( status != S_devAscii_Ok ) return status;
  
  rval = pBo->rval & pBo->mask;

  /* 
   *  Perform the write. 
   */
  status = drvAsciiIntIo( &pPriv->aio, &rval );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pBo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pBo, status );
  }

  pBo->udf = FALSE;

  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * mbbiInitRec() - initializes MBBI records. This setups up all the drvAscii 
 *     info, drvSerial info, and asynchronous completion callback info, 
 *     all of which is attached to the record's device private field.
 */
LOCAL long mbbiInitRec(struct mbbiRecord *pMbbi)
{
  devAsciiPriv *pPriv = NULL;
  long         status;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_IN", 
				&pMbbi->inp, pMbbi, &pPriv,
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   *  The response format can be missing, in which case '%f' is assumed.
   *
   *  The response format may be string ('%nc' or '%ns') but 'n' must not
   *  be greater than 4. With a string specification the input ascii stream
   *  will be converted to an integer value (eg. "abcd" -> 0x61626364 ->
   *  1633837924). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_STRING) 
	||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pMbbi,
		       "devAscii: (Mbbi init_record) Illegal format spec");

    pMbbi->dpvt = (void *) NULL;

    pMbbi->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndInfo.dataType = RBF_STRING;

    pMbbi->udf  = FALSE;

    pMbbi->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer back 
     *  to the record.
     */
    pPriv->aio.pIoDoneCB  = mbbiReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pMbbi;
    
    pMbbi->shft = 0;
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * mbbiGetIoIntInfo() - Used for obtaining the scan private info
 */
LOCAL long mbbiGetIoIntInfo( int                 cmd,
			     struct mbbiRecord   *pMbbi,
			     IOSCANPVT           *ppvt)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;
  
  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * mbbiRead() - MBBI input routine
 */
LOCAL long mbbiRead (struct mbbiRecord * pMbbi)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbi->dpvt;
  long   	 val;
  long		 status;
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbbiRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pMbbi->pact ) { 

    if ( pMbbi->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pMbbi, 
			 (devAsciiPriv	*)  pMbbi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous set the pact.
   */
  status = drvAsciiIntIo(&pPriv->aio, &val);

  if (status == S_drvAscii_AsyncCompletion) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pMbbi->pact = TRUE;

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok) {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    pMbbi->rval = val;

    pMbbi->udf  = FALSE;

    return S_devAscii_Ok;

  } else {

    return reportReadFail( (struct dbCommon *)pMbbi, status );

  }
}

/* 
 * ---------------------------------------------------------------------------
 * mbbiReadAsyncCompletion() -  Asynchronous read completion routine for BI
 */
LOCAL void mbbiReadAsyncCompletion(void *pArg, long status, long value)
{
  struct mbbiRecord *pMbbi= (struct mbbiRecord *)pArg;
  struct dbCommon   *pRec = (struct dbCommon *)pArg;
  struct rset       *prset = (struct rset *)(pRec->rset);
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbbiReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input data.
     */
    pMbbi->rval = value;

    pMbbi->udf  = FALSE;

  } else {
    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}


/*
 * ---------------------------------------------------------------------------
 * mbboInitRec() - initializes MBBO records. This setups up all the drvAscii 
 *     and drvSerial info, all of which is attached to the record's device 
 *     private field.
 */
LOCAL long mbboInitRec(struct mbboRecord *pMbbo)
{
  long		 status;
  devAsciiPriv	*pPriv = NULL;
  long 		 rval;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_OUT", 
				&pMbbo->out, pMbbo, &pPriv,
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt must exist. However a data type specification need 
   *  not exist, in which case '%f' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format   
   *  stringsmust be of the form '%*' ). 
   *
   *  Output formats of '%nc' and '%ns' are valid but 'n' must not 
   *  be greater than 4. That is, a 4 byte integer will be converted 
   *  to an output string of 4 ascii chars (eg. 1633837924 -> 
   *  0x61626364 -> "abcd"). Note that the resultant ascii chars may
   *  not be printable ascii chars (eg 23200612 -> 0x01620364 -> 
   *  ^Ab^Cd, where ^A is control A -> 0x01).
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0)  
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING) 
	||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pMbbo,
		       "devAscii: (Mbbo init_record) Illegal format spec");

    pMbbo->dpvt = (void *) NULL;

    pMbbo->udf = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType = RBF_STRING;

    pMbbo->udf = FALSE;

    pMbbo->dpvt = (void *) pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pMbbo;
    
    pMbbo->shft = 0;
    
    if ( status == OK 
	 && 
	 strlen( pPriv->aio.id.pCmndArg->rbvPrompt ) > 0 ) {

      /*  If possible get an initial value for the record. */
      status = drvAsciiReadOutput( &pPriv->aio.id, &rval );
      
      if ( status == OK ) {

	/*  Set the initial value. */
	pMbbo->rval = (long) rval;
	
	status = S_devAscii_Ok;
      }
    }
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * mbboWrite() - MBBO output routine
 */
LOCAL long mbboWrite (struct mbboRecord *pMbbo)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbo->dpvt;
  int		status;
  long		rval;
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbboWrite\n");

  /* 
   * if a write is already in progress, or the record is locked-out,
   * then exit 
   */
  if ( pMbbo->pact ) {
 
    if ( pMbbo->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pMbbo, 
			 (devAsciiPriv	*)  pMbbo->dpvt );

  if ( status != S_devAscii_Ok ) return status;
  
  /* 
   *  The use of NOBT and MASK not currently implemented.
   */
  rval = pMbbo->rval;

  /* 
   *  Perform the write. 
   */
  status = drvAsciiIntIo( &pPriv->aio, &rval );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pMbbo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pMbbo, status );
    
  }

  pMbbo->udf = FALSE;

  return S_devAscii_Ok;
}
 

/*
 * ---------------------------------------------------------------------------
 * siInitRec() - initializes stringin records. This setups up all the 
 *     drvAscii info, drvSerial info, and asynchronous completion callback 
 *     info, all of which is attached to the record's device private field.
 */
LOCAL long siInitRec(struct stringinRecord *pSi)
{
  devAsciiPriv  *pPriv = NULL;
  long          status = S_devAscii_Ok;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "STRING_IN",
				&pSi->inp, pSi, &pPriv,	
				&(pPriv->sigNum_arraySize) );

  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist. if %s exists in the prompt then the contents
   *  of VAL will be used as part or all of the prompt.
   */
  if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED 
	&&
	pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_STRING ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_STRING 
	&&
	pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pSi,
		       "devAscii: (stringIn init_record) Illegal format spec");

    pSi->udf  = TRUE;

    pSi->dpvt = (void *) NULL;

  } else {

    pSi->udf  = FALSE;

    pSi->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer back 
     *  to the record.
     */
    pPriv->aio.pIoDoneCB  = siReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pSi;

    pPriv->aio.id.respStr[0] = '\0';
    
    pSi->val[0] = '\0';
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * siGetIoIntInfo() - Used for obtaining the scan private info
 */
LOCAL long siGetIoIntInfo(   int                     cmd,
			     struct stringinRecord   *pSi,
			     IOSCANPVT               *ppvt)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pSi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;

  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * siRead() - stringin input routine
 */
LOCAL long siRead (struct stringinRecord * pSi)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pSi->dpvt;
  drvAsyncIO    *pDrvAsyncIO = &pPriv->aio;

  long		 status;
  long           nelm;

  if ( drvAsciiDebugLevel & 32 ) printf("siRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pSi->pact ) { 

    if ( pSi->udf )
      return S_devAscii_badRec;
 
    else
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pSi, 
			 (devAsciiPriv	*)  pSi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous set the pact.
   */
  if ( pDrvAsyncIO->id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING ) 
    /* 
     *  We do not check whether or not the val field is null. If it is null
     *  then only the current write command terminator will be output. Note
     *  that this could be anything by setting the readCMT record.
     */
    status = drvAsciiStringIo(&pPriv->aio, pSi->val );

  else
    status = drvAsciiStringIo( &pPriv->aio, "" );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pSi->pact = TRUE;

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok) {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    nelm = min( pDrvAsyncIO->id.respStrCnt, sizeof( pSi->val ) );

    strncpy( pSi->val, pDrvAsyncIO->id.respStr, nelm );      
    pSi->val[nelm] = '\0';

    pSi->udf = FALSE;

    return S_devAscii_Ok;

  } else {

    return reportReadFail( (struct dbCommon *)pSi, status );

  }
}

/* 
 * ---------------------------------------------------------------------------
 * siReadAsyncCompletion() -  Asynchronous read completion routine for 
 *   stringin records
 */
LOCAL void siReadAsyncCompletion( void *pArg, long status, long value )
{
  struct stringinRecord *pSi = (struct stringinRecord *)pArg;
  struct dbCommon       *pRec = (struct dbCommon *)pArg;
  struct rset           *prset = (struct rset *)(pRec->rset);
  devAsciiPriv          *pDevAsciiPriv = (devAsciiPriv *)(pSi->dpvt);
  drvAsyncIO            *pDrvAsyncIO = &pDevAsciiPriv->aio;

  long                   nelm;

  if ( drvAsciiDebugLevel & 32 ) printf("siReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input data.
     */
    nelm = min( pDrvAsyncIO->id.respStrCnt, sizeof( pSi->val ) );

    strncpy( pSi->val, pDrvAsyncIO->id.respStr, nelm ); 
    pSi->val[nelm] = '\0';

    pSi->udf = FALSE;
    
  } else {
    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}



/*
 * ---------------------------------------------------------------------------
 * soInitRec() - initializes stringout records. This setups up
 *     all the drvAscii and drvSerial info, all of which is attached 
 *     to the record's device private field.
 */
LOCAL long soInitRec(struct stringoutRecord *pSo)
{
  long		status;
  devAsciiPriv	*pPriv = NULL;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "STRING_OUT", 
				&pSo->out, pSo, &pPriv,
				&(pPriv->sigNum_arraySize) );
    
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt need not exist. However if a prompt does exist and a data type 
   *  is specified then the data type must be string (ie. '%ns' is the only
   *  valid option). If no data type is specified then '%s' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format strings  
   *  must be of the form '%*' ). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_STRING 
	&&
        pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
        (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError(status, (void *)pSo,
		      "devAscii: (stringOut init_record) Illegal format spec");

    pSo->dpvt = (void *) NULL;

    pSo->udf = TRUE;

  } else {

    pSo->udf = FALSE;

    pSo->dpvt = (void *) pPriv;

    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pSo;
    
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * soWrite() - string output routine
 */
LOCAL long soWrite (struct stringoutRecord *pSo)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pSo->dpvt;
  int		 status;

  if ( drvAsciiDebugLevel & 32 ) printf("soWrite\n");

  /* 
   *  If a write is already in progress, or the record is locked-out,
   *  then exit.
   */
  if ( pSo->pact ) { 

    if ( pSo->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pSo, 
			 (devAsciiPriv	*)  pSo->dpvt );

  if ( status != S_devAscii_Ok ) return status;
  
  /* 
   *  Perform the write.
   */
  /*
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) > 0) 
       &&
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_UNDEFINED ) ) 

    status = drvAsciiStringIo( &pPriv->aio, "" );

  else 
  */
  if (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING)
    status = drvAsciiStringIo( &pPriv->aio, pSo->val );
  else
    status = drvAsciiStringIo( &pPriv->aio, "" );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pSo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pSo, status );
  }

  pSo->udf = FALSE;

  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * liInitRec()- initializes longin records. This setups up
 *     all the drvAscii info, drvSerial info, and asynchronous completion
 *     callback info, all of which is attached to the record's device
 *     private field.
 */
LOCAL long liInitRec(struct longinRecord *pLi) 
{
  long         status;
  devAsciiPriv *pPriv = NULL;

  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_IN", 
				&pLi->inp, pLi, &pPriv,
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   *  The response format can be missing, in which case '%f' is assumed.
   *
   *  The response format may be string ('%nc' or '%ns') but 'n' must not
   *  be greater than 4. With a string specification the input ascii stream
   *  will be converted to an integer value (eg. "abcd" -> 0x61626364 ->
   *  1633837924). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_STRING) 
	||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pLi,
		       "devAscii: (longIn init_record) Illegal format spec");

    pLi->dpvt = (void *) NULL;

    pLi->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndInfo.dataType = RBF_STRING;

    pLi->udf  = FALSE;

    pLi->dpvt = pPriv;
  
    /* 
     *  Setup the async callback routine, the arg to which is a pointer back 
     *  to the record
     */
    pPriv->aio.pIoDoneCB  = liReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pLi;
  }

  return status;
}

/* 
 * ---------------------------------------------------------------------------
 * liGetIoIntInfo() -  Asynchronous read completion routine for AI
 */
LOCAL long liGetIoIntInfo( int                 cmd,
			   struct longinRecord *pLi,
			   IOSCANPVT	       *ppvt)
{
  devAsciiPriv *pPriv = (devAsciiPriv *) pLi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;
  
  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * liRead() - longin input routine
 */
LOCAL long liRead (struct longinRecord * pLi)
{
  devAsciiPriv 	*pPriv = (devAsciiPriv *)pLi->dpvt;
  long		val;
  long		status;
  
  if ( drvAsciiDebugLevel & 32 ) printf("liRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pLi->pact == TRUE ) {

    if ( pLi->udf )
      return S_devAscii_badRec;
 
    else
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pLi, 
			 (devAsciiPriv	*)  pLi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous then
   *  set the pact 
   */
  status = drvAsciiIntIo( &pPriv->aio, &val );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pLi->pact = TRUE;

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok) {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    pLi->val = val;

    pLi->udf = FALSE;

    return S_devAscii_Ok;

  } else {

    return reportReadFail( (struct dbCommon *)pLi, status );

  }
}

/* 
 * ---------------------------------------------------------------------------
 * liReadAsyncCompletion() -  Asynchronous read completion routine for longin
 */
LOCAL void liReadAsyncCompletion(void *pArg, long status, long value)
{
  struct longinRecord   *pLi = (struct longinRecord *)pArg;
  struct dbCommon       *pRec = (struct dbCommon *)pArg;
  struct rset           *prset = (struct rset *)(pRec->rset);
  
  if ( drvAsciiDebugLevel & 32 ) printf("liReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so update the record's value field.
     */
    pLi->val = value;

    pLi->udf = FALSE;

  } else {
    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}


/*
 * ---------------------------------------------------------------------------
 * loInitRec()- initializes longout records. This setups up all the drvAscii 
 *     and drvSerial info, all of which is attached to the record's device 
 *     private field.
 */
LOCAL long loInitRec(struct longoutRecord *pLo) 
{
  long          status;
  devAsciiPriv	*pPriv = NULL;
  long		rval;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_OUT", 
				&pLo->out, pLo, &pPriv,
				&(pPriv->sigNum_arraySize) );
   
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt must exist. However a data type specification need 
   *  not exist, in which case '%f' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format 
   *  strings must be of the form '%*' ). 
   *
   *  Output formats of '%nc' and '%ns' are valid but 'n' must not
   *  be greater than 4. That is, a 4 byte integer will be converted 
   *  to an output string of 4 ascii chars (eg. 1633837924 -> 0x61626364 
   *  -> "abcd"). Note that the resultant ascii chars may not be 
   *  printable ascii chars (eg 23200612 -> 0x01620364 -> ^Ab^Cd, 
   *  where ^A is control A -> 0x01).
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0)  
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
      ((pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR) 
       &&
        (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    recGblRecordError( status, (void *)pLo,
		       "devAscii: (longOut init_record) Illegal format spec");

    pLo->dpvt = (void *) NULL;

    pLo->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType = RBF_STRING;

    pLo->udf  = FALSE;

    pLo->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pLo;
    
    /*  Even on error ensure the record's fields are initialized. */
    if ( status == OK 
	 && 
	 strlen( pPriv->aio.id.pCmndArg->rbvPrompt ) > 0 ) {
      /*  If possible get an initial value for the record. */
      status = drvAsciiReadOutput( &pPriv->aio.id, &rval );

      if ( status == OK ) {
	/*  Set the initial value. */
	pLo->val = rval;

	status = S_devAscii_Ok;
      }
    }
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * loWrite() - longout output routine
 */
LOCAL long loWrite (struct longoutRecord * pLo)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *)pLo->dpvt;
  int		 status;
  long		 rval;
  
  if ( drvAsciiDebugLevel & 32 ) printf("loWrite\n");

  /* 
   *  If a write is already in progress, or the record is locked-out,
   *  then exit. 
   */
  if ( pLo->pact ) {

    if ( pLo->udf )
      return S_devAscii_badRec;
 
    else
      return S_devAscii_Ok; 
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pLo, 
			 (devAsciiPriv	*)  pLo->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  rval = pLo->val;

  /* 
   *  Perform the write. 
   */
  status = drvAsciiIntIo( &pPriv->aio, &rval );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pLo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pLo, status );
  }

  pLo->udf = FALSE;

  return S_devAscii_Ok;
}


/*
 * ---------------------------------------------------------------------------
 * mbbiDirectInitRec() - initializes MBBI direct records. This setups up
 *     all the drvAscii info, drvSerial info, and asynchronous completion
 *     callback info, all of which is attached to the record's device
 *     private field.
 */
LOCAL long mbbiDirectInitRec(struct mbbiDirectRecord *pMbbi)
{
  devAsciiPriv *pPriv = NULL;
  long         status;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_IN", 
				&pMbbi->inp, pMbbi, &pPriv,
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   *  The response format can be missing, in which case '%f' is assumed.
   *
   *  The response format may be string ('%nc' or '%ns') but 'n' must not
   *  be greater than 4. With a string specification the input ascii stream
   *  will be converted to an integer value (eg. "abcd" -> 0x61626364 ->
   *  1633837924). 
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_STRING) 
	||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms and
     *  disable its processing.
     */
    status = S_db_badField;

    if ( pMbbi->udf == FALSE ) {

      recGblRecordError( status, (void *)pMbbi,
			 "devAscii: (Mbbi direct init_record) Illegal format spec");

      recGblSetSevr( (struct dbCommon *)pMbbi, UDF_ALARM, INVALID_ALARM );

    }

    pMbbi->dpvt = (void *) NULL;

    pMbbi->udf = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndInfo.dataType = RBF_STRING;

    pMbbi->udf = FALSE;

    pMbbi->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer back 
     *  to the record.
     */
    pPriv->aio.pIoDoneCB  = mbbiDirectReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pMbbi;
    
    pMbbi->shft = 0;
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * mbbiDirectGetIoIntInfo() - Used for obtaining the scan private info
 */
LOCAL long mbbiDirectGetIoIntInfo( int                     cmd,
				   struct mbbiDirectRecord *pMbbi,
				   IOSCANPVT               *ppvt)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbi->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;

  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * mbbiDirectRead() - MBBI direct input routine
 */
LOCAL long mbbiDirectRead (struct mbbiDirectRecord * pMbbi)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbi->dpvt;
  long   	val;
  long		status;
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbbiDirectRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit.
   */
  if ( pMbbi->pact ) {

    if ( pMbbi->udf ) 
      return S_devAscii_badRec; 

    else 
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pMbbi, 
			 (devAsciiPriv	*)  pMbbi->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous then
   *  set the pact 
   */
  status = drvAsciiIntIo( &pPriv->aio, &val );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pMbbi->pact = TRUE;

    return S_devAscii_Ok;

  } else if ( status == S_drvAscii_Ok )  {
    /* 
     *  The read completed non-asynchronously so update the record's
     *  value field.
     */
    pMbbi->rval = val;

    pMbbi->udf  = FALSE;

    return S_devAscii_Ok;

  } else if (status) {

    return reportReadFail( (struct dbCommon *)pMbbi, status );

  }
  
    return S_devAscii_Ok;
}

/* 
 * ---------------------------------------------------------------------------
 * mbbiDirectReadAsyncCompletion() -  Asynchronous read completion routine 
 *   for MBBI direct
 */
LOCAL void mbbiDirectReadAsyncCompletion(void *pArg, long status, long value)
{
  struct mbbiDirectRecord *pMbbi= (struct mbbiDirectRecord *)pArg;
  struct dbCommon         *pRec = (struct dbCommon *)pArg;
  struct rset             *prset = (struct rset *)(pRec->rset);
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbbiDirectReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so update the record's value field.
     */
    pMbbi->rval = value;

    pMbbi->udf  = FALSE;

  } else {
    /*
     *  The read failed so report the failure.
     */
    reportReadFail( pRec, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}


/*
 * ---------------------------------------------------------------------------
 * mbboDirectInitRec() - initializes MBBO direct records. This setups up
 *     all the drvAscii and drvSerial info, all of which is attached 
 *     to the record's device private field.
 */
LOCAL long mbboDirectInitRec(struct mbboDirectRecord *pMbbo)
{
  long		status;
  devAsciiPriv	*pPriv = NULL;
  long 		rval;
  
  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "INTEGER_OUT",
				&pMbbo->out, pMbbo, &pPriv,
				&(pPriv->sigNum_arraySize) );
  
  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  A prompt must exist. However a data type specification need 
   *  not exist, in which case '%f' is assumed.
   *
   *  Responses must not cause assignment (ie. all reponse format 
   *  strings must be of the form '%*' ). 
   *
   *  Output formats of '%nc' and '%ns' are valid but 'n' must not 
   *  be greater than 4. That is, a 4 byte integer will be converted 
   *  to an output string of 4 ascii chars (eg. 1633837924 -> 0x61626364 
   *  -> "abcd"). Note that the resultant ascii chars may not be 
   *  printable ascii chars (eg 23200612 -> 0x01620364 -> ^Ab^Cd, 
   *  where ^A is control A -> 0x01).
   *
   *  Also arrays of values are not currently supported (Jan. 1997).
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0)  
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       ((pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR) 
	&&
        (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataCnt > 4) ) 
       ||
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING) 
	||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) {
    /*
     *  Something is wrong with the record so mark it for alarms.
     */
    status = S_db_badField;

    if ( pMbbo->udf == FALSE ) {

      recGblRecordError( status, (void *)pMbbo,
			 "devAscii: (Mbbo direct init_record) Illegal format spec");

      recGblSetSevr( (struct dbCommon *)pMbbo, UDF_ALARM, INVALID_ALARM );
    }

    pMbbo->dpvt = (void *) NULL;

    pMbbo->udf  = TRUE;

  } else {

    if ( pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType == RBF_CHAR )
      pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType = RBF_STRING;

    pMbbo->udf  = FALSE;

    pMbbo->dpvt = (void *) pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer  
     *  back to the record.
     */
    pPriv->aio.pIoDoneCB  = devAsciiWriteAsyncCompletion;
    pPriv->aio.pIoDoneArg = pMbbo;
    
    pMbbo->shft = 0;
    
    if ( status == OK 
	 && 
	 strlen( pPriv->aio.id.pCmndArg->rbvPrompt ) > 0 ) {
      /*  If possible get an initial value for the record. */
      status = drvAsciiReadOutput( &pPriv->aio.id, &rval );
    
      if ( status == OK ) {
	/*  Set the initial value. */
	pMbbo->rval = rval;

	status = S_devAscii_Ok;
      }
    }
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * mbboDirectWrite() - MBBO direct output routine
 */
LOCAL long mbboDirectWrite (struct mbboDirectRecord *pMbbo)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pMbbo->dpvt;
  int		status;
  long		rval;
  
  if ( drvAsciiDebugLevel & 32 ) printf("mbboDirectWrite\n");

  /* 
   *  If a write is already in progress, or the record is locked-out,
   *  then exit.
   */
  if ( pMbbo->pact ) {

    if ( pMbbo->udf ) 
      return S_devAscii_badRec; 
    
    else 
      return S_devAscii_Ok; 
  }
  
  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pMbbo, 
			 (devAsciiPriv	*)pMbbo->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Use of NOBT and MASK not currently implemented.
   */
  rval = pMbbo->rval;

  /* 
   *  Perform the write.
   */
  status = drvAsciiIntIo( &pPriv->aio, &rval );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  write completes so mark it as such.
     */
    pMbbo->pact = TRUE;
    
    status = S_devAscii_Ok;

  } else if ( status != S_drvAscii_Ok ) {
    /*
     *  The write failed so set the record's alarm states.
     */
    return reportWriteFail( (struct dbCommon *)pMbbo, status );

  }

  pMbbo->udf = FALSE;

  return S_devAscii_Ok;
}
 

/*
 * ---------------------------------------------------------------------------
 * wfInitRec() - initializes waveform records. The waveform record is used
 *     as a long stringin record, that is form inputting strings that are
 *     greater than default stringin record size (40 bytes).
 *
 *     This setups up all the drvAscii info, drvSerial info, and asynchronous
 *     completion callback info, all of which is attached to the record's 
 *     device private field.
 */
LOCAL long wfInitRec(struct waveformRecord *pWf)
{
  devAsciiPriv  *pPriv = NULL;
  long          status = S_devAscii_Ok;
  
  if ( (pWf->ftvl != DBF_CHAR) && (pWf->ftvl != DBF_STRING) ) {

    status = S_db_badField;

    /*  
     *  The record is invalid so generate an error and an alarm then 
     *  ensure the record is disabled so it does not process in the
     *  future.
     */
    recGblRecordError( status, (void *)pWf,
		       "devAscii: (waveform init_record) Illegal format spec");

    recGblSetSevr( (struct dbCommon *)pWf, UDF_ALARM, INVALID_ALARM );

    pWf->pact = TRUE;
    pWf->udf  = TRUE;

    return status;
  }

  /*  Initialize the record and driver. */
  status = devAsciiInitPrivate( "STRING_IN",
				&pWf->inp, pWf, &pPriv,	
				&(pPriv->sigNum_arraySize) );

  if ( status != S_drvAscii_Ok && status != S_devAscii_Ok ) {

    return status;

  }

  /*
   *  The prompt must exist but it cannot have a data type specification.
   *
   */
  if ( (strlen(pPriv->aio.id.pCmndArg->cmndPrompt) == 0) 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killCnt 
       ||
       pPriv->aio.id.pCmndArg->cmndPromptInfo.killAll 
       || 
       (pPriv->aio.id.pCmndArg->cmndPromptInfo.dataType != RBF_UNDEFINED) 
       ||
       pPriv->aio.id.pCmndArg->cmndInfo.dataType == RBF_CHAR
       ||
       (pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_STRING
	&&
        pPriv->aio.id.pCmndArg->cmndInfo.dataType != RBF_UNDEFINED) 
       ||
       (status != S_drvAscii_Ok) ) { 
    /*
     *  Something is wrong with the record so mark it for alarms
     *  and disable it's processing.
     */
    status = S_db_badField;

    if ( pWf->udf == FALSE ) {

      recGblRecordError( status, (void *)pWf,
			 "devAscii: (waveform init_record) Illegal format spec");

      recGblSetSevr( (struct dbCommon *)pWf, UDF_ALARM, INVALID_ALARM );
    }

    pWf->dpvt = (void *) NULL;
    pWf->udf  = TRUE;

  } else {

    pWf->udf  = FALSE;

    pWf->dpvt = pPriv;
    
    /* 
     *  Setup the async callback routine, the arg to which is a pointer back 
     *  to the record
     */
    pPriv->aio.pIoDoneCB  = wfReadAsyncCompletion;
    pPriv->aio.pIoDoneArg = pWf;

    pPriv->aio.id.respStr[0] = '\0';
    
    ((char *)pWf->bptr)[0] = '\0';
  }

  return status;
}

/*
 * ---------------------------------------------------------------------------
 * wfGetIoIntInfo() - Used for obtaining the scan private info
 */
LOCAL long wfGetIoIntInfo(   int                     cmd,
			     struct waveformRecord   *pWf,
			     IOSCANPVT               *ppvt)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pWf->dpvt;
  
  if ( pPriv ) 
    *ppvt = pPriv->spvt;
  
  else 
    *ppvt = NULL;

  return S_devAscii_Ok;
}

/*
 * ---------------------------------------------------------------------------
 * wfRead() - waveform input routine
 */
LOCAL long wfRead (struct waveformRecord * pWf)
{
  devAsciiPriv	*pPriv = (devAsciiPriv *) pWf->dpvt;
  drvAsyncIO    *pDrvAsyncIO = &pPriv->aio;
  long		 status;
  
  if ( drvAsciiDebugLevel & 32 ) printf("wfRead\n");

  /* 
   *  If a read is already in progress, or the record is locked out, then 
   *  exit. 
   */
  if ( pWf->pact ) {

    if ( pWf->udf )
      return S_devAscii_badRec; 

    else
      return S_devAscii_Ok; 
  }

  /*
   *  Determine if the record is valid so that processing can proceed.
   */
  status = reportBadRec( (struct dbCommon *)pWf, 
			 (devAsciiPriv	*)  pWf->dpvt );

  if ( status != S_devAscii_Ok ) return status;

  /* 
   *  Perform the read and if completion is asynchronous set the pact.
   */
  if ( pDrvAsyncIO->id.pCmndArg->cmndPromptInfo.dataType == RBF_STRING ) 
    /* 
     *  We do not check whether or not the val field is null. If it is null
     *  then only the current write command terminator will be output. Note
     *  that this could be anything by setting the readCMT record.
     */
    status = drvAsciiStringIo(&pPriv->aio, pWf->val );

  else
  status = drvAsciiStringIo( &pPriv->aio, "" );

  if ( status == S_drvAscii_AsyncCompletion ) {
    /*
     *  The record will be asynchronously called back when the 
     *  read completes so mark it as such.
     */
    pWf->pact = TRUE;

    return S_devAscii_Ok;
      
  } else if ( status == S_drvAscii_Ok )  {
    /* 
     *  The read completed non-asynchronously so copy the input into
     *  the record.
     */
    pWf->nord = pWf->nelm < pDrvAsyncIO->id.respStrCnt
      ? pWf->nelm : pDrvAsyncIO->id.respStrCnt;

    strncpy( pWf->bptr, pDrvAsyncIO->id.respStr, pWf->nord );

    pWf->udf = FALSE;
    
    return S_devAscii_Ok;
    
  } else {

    return reportReadFail( (struct dbCommon *)pWf, status );
    
  }
}

/* 
 * ---------------------------------------------------------------------------
 * wfReadAsyncCompletion() -  Asynchronous read completion routine for 
 *   wave form records
 */
LOCAL void wfReadAsyncCompletion( void *pArg, long status, long value )
{
  struct waveformRecord *pWf = (struct waveformRecord *)pArg;
  struct dbCommon       *pRec = (struct dbCommon *)pArg;
  struct rset           *prset = (struct rset *)(pRec->rset);
  devAsciiPriv          *pDevAsciiPriv = (devAsciiPriv *)(pWf->dpvt);
  drvAsyncIO            *pDrvAsyncIO = &pDevAsciiPriv->aio;
 
  if ( drvAsciiDebugLevel & 32 ) printf("wfReadAsyncCompletion\n");

  dbScanLock( pRec );

  if ( status == S_drvAscii_Ok ) {
    /* 
     *  The read completed succesfully so copy the input into the record.
     */
    pWf->nord = pWf->nelm < pDrvAsyncIO->id.respStrCnt 
      ? pWf->nelm : pDrvAsyncIO->id.respStrCnt;

    strncpy( pWf->bptr, pDrvAsyncIO->id.respStr, pWf->nord );
 
    pWf->udf = FALSE;

  } else {
    /*
     *  The read failed so report the failure.
     */
    reportReadFail( (struct dbCommon *)pWf, status );

  }

  if ( !pRec->udf )
    /* 
     *  Cause the record to process iff the record is not faulty as
     *  this will clear the pact, which is not desired for a faulty
     *  record.
     */
    (*prset->process)( pRec );

  dbScanUnlock( pRec );
}



/*+*********************************************************************
  $Log: devAscii.c,v $
  Revision 1.1  2009/08/01 03:39:00  mrippa
  New drvAscii for gnirs

  Revision 1.9  2003/03/19 01:56:34  aobld
  Modified soWrite() so that the null string "" is sent when the
  format info is RBF_UNDEFINED. Previously, the default value of the
  VAL field ('string') was output along with the prompt.
  (A Honey)

  Revision 1.8  2003/03/19 00:23:34  ahoney
  Modified the debug controlled printf statement within devAsciiInitPrivate().
  A Honey

  Revision 1.7  2002/12/02 19:44:52  ahoney
  Corrected a bug in stringIn and waveform read processing where the end-of-string terminator was not set in the val field

  Revision 1.6  2002/09/05 17:55:29  ahoney
  Analog output behavior was modified to use OVAL rather than VAL. This
  was done to preserve the filtering functionality provided by OROC.

  Revision 1.5  2002/09/03 20:53:11  ahoney
        1. Significant modifications, within drvAscii.c, in regards to
         the handling of synchronization semaphores so as to alleviate
         potential loss of read/write synchronization. Although the
         synchronization problem was infrequent it sometimes required a
         processor reboot in order to correct the problem. Hopefully,
         synchronization will now be auto-magically re-established.

        2. Analog records may now have 'REAL' specified in their parm fields,
         after the link specification and before the first prompt format
         field.

         If an analog input record has the REAL attribute then the value
         returned form the remote device is written into the record's VAL
         field and RVAL/ESLO conversions are bypassed. Note that also
         bypasses the 'slope' record behavior. Similarly, analog output
         records will have their VAL values output to the remote device
         rather than their RVAL values. In general this eliminates the
         conversions to/fro real and integer which caused loss of
         precision, as well as, reducing the considerable complexity in using
         drvAscii for analog records.

        3. The format-string delimiters no longer have to be '<' and '>'
         for all records. Now the first character follwing the link
         specification is assumed to be the field delimiter, with the pairs
         '<>', '()', '{}', and '[]' assumed, when the left hand delimiter
         is encountered. These are now valid (on a record-by-record basis):
                      @/tyco/0 <status?> <%s>
                      @/tyco/0 (status?) (%s)
                      @/tyco/0 !status?! !%s!
                      @/tyco/0  status?   %s
                      @/tyco/0 Xstatus?X X%sX

        4. User-specified framing routines can be specified.
         With this release one can create their own input and output
         framing routines and have them override the default getFrame()
         and putFrame() routines. This allows one to do more sophisticated
         packet framing (e.g. a simple checksum could be added/checked).

         To specify special framing routines one must download the library,
         they created, containing those routine then register the
         functions with drvAsciiSetTxFunc() and drvAsciiSetRxFunc(), all
         prior to iocInit. Note that a different set of framing routines
         can be registered for each serial link. Also note that there are
         special requirements imposed on the framing routines. An example
         exists within drvAscii.c

        5. More extensive control of debugging information via link-specific
         debug records and via a drvAscii global variable drvAsciiDebugLevel.

        6. Format specifications may now have embedded hex or octal bytes
         Said bytes must be of the form '\xnn' or '\ooo'. Those bytes are
         translated when the format specs are parsed during record init.
         For instance a prompt format such as <\x58\x59\x5a?> will result
         in the string 'XYZ?' being output to the remote device.

        7. All numeric escape codes, that exist in output or input data
         streams, will be automatically translated. For instance a
         stringIn record with this prompt and response format '<%s?><%s>'
         can be manipulated with "caput stringIn '\x58'" to result
         in a prompt of 'X?' being transmitted. This should simplify record
         manipulation for those apps which talk to multi-dropped devices
         whose addresses are leading non-printing ASCII bytes. The numeric
         escape codes need not result in a printable ASCII characters,
         that is, "caput stringIn '\x81'" is valid. However, Beware that
         drvAscii uses sprintf and sscanf so embedding a null byte will
         cause unexpected behavior.

  Revision 1.4  2000/05/06 02:05:51  ktsubota
  Incorporated changes by A.Honey

  Revision 1.2  1999/07/16 04:20:09  ahoney
  Corrected a bug in mbboInitRec.
  Updated a few statuses.

  Revision 1.1  1998/12/03 23:56:10  ktsubota
  Initial insertion

  Revision 1.13  1998/04/03 23:13:16  ahoney
  Removed the previous mod for string out records, as it changes previous
  behaviour.

  Revision 1.12  1998/03/18 01:39:20  ahoney
  Updated the processing for stringIn records to allows for dynamic
  prompts. This is accomplished by using the VAL field for output when
  prompting and input when a response arrives.

  Revision 1.11  1997/02/08 00:49:21  ahoney
  Removed setting of PACT=1 on failure within mbbireadasynccompletion

  As this would permanently disable the relevant record(s).

 * Revision 1.10  1997/01/23  02:14:03  ahoney
 * Within string input routines I changes strlen( pSi->val ) to
 * sizeof( pSi->val ). These were potential bugs.
 *
 * Revision 1.9  1997/01/21  02:37:55  ahoney
 * Mods to better handle conversion from integer to strings. This is
 * still unacceptable as drvAscii cannot handle null bytes as they
 * appear to be end-of-string terminators.
 *
 * Revision 1.8  1997/01/20  20:50:56  ahoney
 * Extensive mods to accomodate:
 *   -added mbbi and mbbo direct records;
 *   -added waveform records (long stringin);
 *   -added conversion of binary streams '0' and '1', with or without
 *    delimiters
 *   -added conversion from string to numeric 'abcd'->0x61626364->1633837924
 *   -added support for muliple line input strings
 *
 * Revision 1.7  1996/12/18  23:27:30  ahoney
 * Mods to support ao,bi,bo,mbbi,mbbo,longin,longout,stringin, and stringout
 * records. These were necessitated for use with IFSM and chopper.
 *
 * Revision 1.6  1996/09/13  21:04:59  ahoney
 * removed '#define LOCAL' which used during debugging.
 *
 * Revision 1.5  1996/09/13  20:49:50  ahoney
 * Mods to accomodate the flat lamp device.
 *
 * Note this driver was completed only so far as was necessary for the
 * data acquisition systems. The drive will still need a few mods for
 * the IFSM.
 *
 * Revision 1.4  1996/09/11  21:51:48  ahoney
 * Mods to incorporate longin and longout records as needed for the
 * dome flat lamps. Also modified the handling of '%nk' in data formats
 * so that 'n' characters can be 'killed' at the beginning of a data
 * stream - this allows stripping leading NULLs,...
 *
 * Revision 1.3  1996/08/14  18:37:55  ahoney
 * Modified all async completion routines so that PACT is set false on
 * errors. This was done to correct the problem when the serial link is
 * down on startup.
 *
 * Revision 1.2  1996/07/13  00:38:03  ahoney
 * Removed some debugging printf's.
 *
 * Revision 1.1  1996/07/12  23:26:22  ahoney
 * drvAscii is a new directory for support for serial comms to remote
 * devices via ascii strings
 *
 *
***********************************************************************/
 


