
/*
 *  api.h
 *
 *  Copyright 2003 QNX Software Systems Limited. All Rights Reserved.
 * 
 *  You must obtain a written license from and pay applicable license fees to QNX 
 *  Software Systems Limited before you may reproduce, modify or distribute this software, 
 *  or any work that includes all or part of this software.   Free development 
 *  licenses are available for evaluation and non-commercial purposes.  For more 
 *  information visit http://licensing.qnx.com or email licensing@qnx.com.
 *  
 *  This file may contain contributions from others.  Please review this entire 
 *  file for other proprietary rights or license notices, as well as the QNX 
 *  Development Suite License Guide at http://licensing.qnx.com/license-guide/ 
 *  for other information.
 *
 *  This module contains the manifest constants and declarations for
 *  clients of ADIOS.
 *
 *  2003 06 20  R. Krten        created
 *  2003 08 05  R. Krten        annotated memory dump; added "nbpc" value for "tag" bug
 *  2003 10 23  R. Krten        released for publication
*/

#include <inttypes.h>

/*
 *  SHARED MEMORY LAYOUT
 *
 *  The following data structures are used in the shared memory
 *  layout and are therefore common to all users of the shared
 *  memory (such as ADIOS which writes into shared memory, and
 *  showsamp which reads from shared memory).
 *
 *  Here is a typical shared memory layout, showing two cards,
 *  a PCL-711 card at 0x2d0, and a DIO-144 card at 0x220, a
 *  ring buffer with 2000 elements, and head set to 0 and tail
 *  set to 99:
 *  
 *  adios_signature_t:
 *      0000 signature(4)       // signature ("ADIO")
 *      0004 datablock(4)       // datablock, which 4k page the data section starts at (1)
 *      0008 num_cis(4)         // number of entries in the CIS (2)
 *      000C num_elems(4)       // size of the ring buffer (2000)
 *  adios_daq_status_t:
 *      0010 head(4)            // index to newest valid & stable data element (0)
 *      0014 tail(4)            // index to oldest valid & stable data element (99)
 *      0018 element_size(4)    // size of each element, including any padding (52)
 *  adios_cis_t:
 *      001C name(128)          // name of the device, "/dev/pcl711-02d0"
 *      009C nai(4)             // number of analog inputs (8)
 *      00A0 nao(4)             // number of analog outputs (1)
 *      00A4 ndi(4)             // number of digital inputs (16)
 *      00A8 ndo(4)             // number of digital outputs (16)
 *      00AC nbpc(4)            // number of bytes per channel (2)
 *      00B0 maxresai(4)        // maximum bit resolution of analog input (12)
 *  adios_cis_t:
 *      00B4 name(128)          // name of the device, "/dev/dio144-0220"
 *      0134 nai(4)             // number of analog inputs (0)
 *      0138 nao(4)             // number of analog outputs (0)
 *      013C ndi(4)             // number of digital inputs (144)
 *      0140 ndo(4)             // number of digital outputs (144)
 *      0144 nbpc(4)            // number of bytes per channel (3)
 *      0148 maxresai(4)        // maximum bit resolution of analog input (0)
 *  (filler)
 *      014C -> 0FFF            // not used; filler to align to next 4k block
 *  adios_data_header_t:
 *      1000 t0ns(8)            // beginning of snapshot 0 time
 *      1008 t1ns(8)            // end of snapshot 0 time
 *  (data)
 *      1010 ai0(2)             // PCL-711 analog input channel 0 sample
 *      1012 ai1(2)             // PCL-711 analog input channel 1 sample
 *      1014 ai2(2)             // PCL-711 analog input channel 2 sample
 *      1016 ai3(2)             // PCL-711 analog input channel 3 sample
 *      1018 ai4(2)             // PCL-711 analog input channel 4 sample
 *      101A ai5(2)             // PCL-711 analog input channel 5 sample
 *      101C ai6(2)             // PCL-711 analog input channel 6 sample
 *      101E ai7(2)             // PCL-711 analog input channel 7 sample
 *      1020 di(2)              // PCL-711 digital input channel 8 sample (16 bits)
 *      1022 di(18)             // DIO-144 digital input channel 0 through 5 samples (144 bits)
 *  adios_data_header_t:
 *      1034 t0ns(8)            // beginning of snapshot 1 time
 *      103C t1ns(8)            // end of snapshot 1 time
 *  (data)
 *      1044 ai0(2)             // PCL-711 analog input channel 0 sample
 *      ...
 *
 *  In the above example, if another DIO-144 was added to the configuration, the
 *  values that would change would be the "0008 num_cis" (which would go to 3),
 *  "0018 element_size" would increase to 70 (there are now 18 more bytes of
 *  data), another adios_cis_t structure would be present (starting at offset
 *  014C, shrinking the size of the filler section).  In the data section, 18
 *  additional bytes would be present in each sample.
*/

#define ADIOS_SIGNATURE     "ADIO"
typedef struct adios_signature_s
{
    char            signature [4];
    int             datablock;              // which 4k page the data section starts at
    int             num_cis;                // number of entries in the CIS
    int             num_elems;              // size of ring buffer
}   adios_signature_t;

typedef struct adios_daq_status_s
{
    int             head;                   // index to newest valid & stable data element
    int             tail;                   // index to oldest valid & stable data element
    int             element_size;           // size of each element, including any padding
}   adios_daq_status_t;

#define MAXNAME     128
typedef struct adios_cis_s
{
    char            name [MAXNAME];         // registered name, e.g. /dev/pcl711-02d0
    int             nai;                    // number of analog inputs
    int             nao;                    // number of analog outputs (these don't show in shared memory)
    int             ndi;                    // number of digital inputs
    int             ndo;                    // number of digital outputs (these don't show in shared memory)
    int             nbpc;                   // number of bytes per channel (e.g., 2 for PCL711 (it has one 16-bit digital value), 3 for DIO144 (three 8-bit ports))
    int             maxresai;               // maximum bit resolution of analog input (# bits)
}   adios_cis_t;

typedef struct adios_data_header_s
{
    uint64_t        t0ns, t1ns;             // time of snapshot in ns since Jan 1 1970 00:00:00 (t0ns == 0 if sample invalid; t0ns is the start and t1ns is the end)
}   adios_data_header_t;

