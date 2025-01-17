/*      $OpenBSD: ex_quit.c,v 1.5 2014/11/12 04:28:41 bentley Exp $     */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1992, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1992, 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022-2023 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 */

#include <sys/types.h>
#include <sys/queue.h>

#include <bitstring.h>
#include <limits.h>
#include <stdio.h>

#include "../common/common.h"

/*
 * ex_quit -- :quit[!]
 *      Quit.
 *
 * PUBLIC: int ex_quit(SCR *, EXCMD *);
 */
int
ex_quit(SCR *sp, EXCMD *cmdp)
{
        int force;

        force = FL_ISSET(cmdp->iflags, E_C_FORCE);

        /* Check for file modifications, or more files to edit. */
        if (file_m2(sp, force) || ex_ncheck(sp, force))
                return (1);

        F_SET(sp, force ? SC_EXIT_FORCE : SC_EXIT);
        return (0);
}
