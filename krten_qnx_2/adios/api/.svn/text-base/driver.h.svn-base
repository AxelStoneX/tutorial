
/*
 *  driver.h
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
 *  ADIOS drivers.  It defines the common messages and device control
 *  operations between ADIOS and its various low-level hardware drivers.
 *
 *  2003 06 20  R. Krten        created
 *  2003 10 23  R. Krten        released for publication
*/

#include <devctl.h>

/*
 *  Driver Commands
 *
 *  DCMD_GET_CONFIG
 *      reads the configuration of the device, and returns the number of
 *      available channels and the maximum resolution of the analog I/O.
 *
 *  DCMD_GET_ADIS
 *      gets Analog/Digital InputS (ADIS).  The number of analog inputs
 *      and digital inputs is specified, and the same number is returned.
 *
 *  DCMD_SET_CPAO
 *      set Channel/Port/AnalogOutput (CPAO).  This is used to set the
 *      value of an analog output port.
 *
 *  DCMD_SET_CPBDO
 *      set Channel/Port/Bit/DigitalOutput (CPBDO).  This is used to set
 *      the value of a digital output port.
*/

/***********************************************************************************/
#define DCMD_GET_CONFIG                 (set_device_direction (0x1000, DEVDIR_FROM)) 
/***********************************************************************************/

typedef struct
{
    int         reserved;               // reserved and must be zero
}   dcmd_get_config_i_t;

typedef struct
{
    int         version;                // version of driver
    int         ndi, ndo, nai, nao;     // number of available channels of DI, DO, AI and AO
    int         nbpc;                   // number of bytes per channel (e.g., 2 for PCL-711, 3 for DIO-144)
    int         maxresai, maxresao;     // maximum bit resolution of analog input and output (# bits)
}   dcmd_get_config_o_t;

typedef union
{
    dcmd_get_config_i_t     i;
    dcmd_get_config_o_t     o;
}   dcmd_get_config_t;

/***********************************************************************************/
#define DCMD_GET_ADIS                   (set_device_direction (0x1001, DEVDIR_FROM)) 
/***********************************************************************************/

typedef struct
{
    int         nais;                   // number of analog inputs to fetch
    int         ndis;                   // number of digital inputs to fetch
}   dcmd_get_adis_i_t;

typedef struct
{
    char        buf [1];                // data buffer
}   dcmd_get_adis_o_t;

typedef union
{
    dcmd_get_adis_i_t       i;
    dcmd_get_adis_o_t       o;
}   dcmd_get_adis_t;

/*********************************************************************************/
#define DCMD_SET_CPAO                   (set_device_direction (0x1002, DEVDIR_TO)) 
/*********************************************************************************/

typedef struct
{
    char        channel [4];
    char        port [4];
    int         value;
}   cpv_t;

typedef struct
{
    int         naos;                   // number of analog outputs to write
    cpv_t       aval [1];               // channel, port, and analog value array
}   dcmd_set_cpao_i_t;

typedef struct
{
    int         sts;
}   dcmd_set_cpao_o_t;

typedef union
{
    dcmd_set_cpao_i_t       i;
    dcmd_set_cpao_o_t       o;
}   dcmd_set_cpao_t;

/*********************************************************************************/
#define DCMD_SET_CPBDO                  (set_device_direction (0x1003, DEVDIR_TO)) 
/*********************************************************************************/

typedef struct
{
    char        channel [4];
    char        port [4];
    int         bitnum;
    int         value;
}   cpbv_t;

typedef struct
{
    int         ndos;                   // number of digital outputs to write
    cpbv_t      dval [1];               // channel, port, bitnum, and bit value array
}   dcmd_set_cpbdo_i_t;

typedef struct
{
    int         sts;
}   dcmd_set_cpbdo_o_t;

typedef union
{
    dcmd_set_cpbdo_i_t      i;
    dcmd_set_cpbdo_o_t      o;
}   dcmd_set_cpbdo_t;

