/*      $OpenBSD: ex_version.c,v 1.10 2014/11/12 04:28:41 bentley Exp $ */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1991, 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1991, 1993, 1994, 1995, 1996
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
#include "version.h"

/*
 * ex_version -- :version
 *      Display the program version.
 *
 * PUBLIC: int ex_version(SCR *, EXCMD *);
 */

int
ex_version(SCR *sp, EXCMD *cmdp)
{
        msgq(sp, M_INFO, VI_VERSION);
        return (0);
}
