/*
 * myresmgr.h
 *
 *  Created on: Sep 8, 2016
 *      Author: alex
 */

#ifndef MYRESMGR_H_
#define MYRESMGR_H_

int io_read (resmgr_context_t *ctp, io_read_t *msg,
             iofunc_ocb_t *ocb);
int io_write (resmgr_context_t *ctp, io_write_t *msg,
              iofunc_ocb_t *ocb);
int io_devctl (resmgr_context_t *ctp, io_devctl_t *msg, iofunc_ocb_t *ocb);

#endif /* MYRESMGR_H_ */
