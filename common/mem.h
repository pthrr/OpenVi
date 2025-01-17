/*      $OpenBSD: mem.h,v 1.10 2017/06/24 16:30:47 martijn Exp $        */

/* SPDX-License-Identifier: BSD-3-Clause */

/*
 * Copyright (c) 1993, 1994
 *      The Regents of the University of California.  All rights reserved.
 * Copyright (c) 1993, 1994, 1995, 1996
 *      Keith Bostic.  All rights reserved.
 * Copyright (c) 2022-2023 Jeffrey H. Johnson <trnsz@pobox.com>
 *
 * See the LICENSE.md file for redistribution information.
 *
 *      @(#)mem.h       10.7 (Berkeley) 3/30/96
 */

/*
 * Increase the size of a malloc'd buffer.  Two versions, one that
 * returns, one that jumps to an error label.
 */

#define BINC_GOTO(sp, lp, llen, nlen) {                                 \
        void *L__bincp;                                                 \
        if ((nlen) > (llen)) {                                          \
                if ((L__bincp = binc((sp), (lp), &(llen), (nlen)))      \
                    == NULL)                                            \
                        goto alloc_err;                                 \
                /*                                                      \
                 * !!!                                                  \
                 * Possible pointer conversion.                         \
                 */                                                     \
                (lp) = L__bincp;                                        \
        }                                                               \
}

#define BINC_RET(sp, lp, llen, nlen) {                                  \
        void *L__bincp;                                                 \
        if ((nlen) > (llen)) {                                          \
                if ((L__bincp = binc((sp), (lp), &(llen), (nlen)))      \
                    == NULL)                                            \
                        return (1);                                     \
                /*                                                      \
                 * !!!                                                  \
                 * Possible pointer conversion.                         \
                 */                                                     \
                (lp) = L__bincp;                                        \
        }                                                               \
}

/*
 * Get some temporary space, preferably from the global temporary buffer,
 * from a malloc'd buffer otherwise.  Two versions, one that returns, one
 * that jumps to an error label.
 */

#define GET_SPACE_GOTO(sp, bp, blen, nlen) {                            \
        GS *L__gp = (sp) == NULL ? NULL : (sp)->gp;                     \
        if (L__gp == NULL || F_ISSET(L__gp, G_TMP_INUSE)) {             \
                (bp) = NULL;                                            \
                (blen) = 0;                                             \
                BINC_GOTO((sp), (bp), (blen), (nlen));                  \
        } else {                                                        \
                BINC_GOTO((sp), L__gp->tmp_bp, L__gp->tmp_blen, (nlen));\
                (bp) = L__gp->tmp_bp;                                   \
                (blen) = L__gp->tmp_blen;                               \
                F_SET(L__gp, G_TMP_INUSE);                              \
        }                                                               \
}

#define GET_SPACE_RET(sp, bp, blen, nlen) {                             \
        GS *L__gp = (sp) == NULL ? NULL : (sp)->gp;                     \
        if (L__gp == NULL || F_ISSET(L__gp, G_TMP_INUSE)) {             \
                (bp) = NULL;                                            \
                (blen) = 0;                                             \
                BINC_RET((sp), (bp), (blen), (nlen));                   \
        } else {                                                        \
                BINC_RET((sp), L__gp->tmp_bp, L__gp->tmp_blen, (nlen)); \
                (bp) = L__gp->tmp_bp;                                   \
                (blen) = L__gp->tmp_blen;                               \
                F_SET(L__gp, G_TMP_INUSE);                              \
        }                                                               \
}

/*
 * Add space to a GET_SPACE returned buffer.  Two versions, one that
 * returns, one that jumps to an error label.
 */

#define ADD_SPACE_GOTO(sp, bp, blen, nlen) {                            \
        GS *L__gp = (sp) == NULL ? NULL : (sp)->gp;                     \
        if (L__gp != NULL && (bp) == L__gp->tmp_bp) {                   \
                F_CLR(L__gp, G_TMP_INUSE);                              \
                BINC_GOTO((sp), L__gp->tmp_bp, L__gp->tmp_blen, (nlen));\
                (bp) = L__gp->tmp_bp;                                   \
                (blen) = L__gp->tmp_blen;                               \
                F_SET(L__gp, G_TMP_INUSE);                              \
        } else                                                          \
                BINC_GOTO((sp), (bp), (blen), (nlen));                  \
}

#define ADD_SPACE_RET(sp, bp, blen, nlen) {                             \
        GS *L__gp = (sp) == NULL ? NULL : (sp)->gp;                     \
        if (L__gp != NULL && (bp) == L__gp->tmp_bp) {                   \
                F_CLR(L__gp, G_TMP_INUSE);                              \
                BINC_RET((sp), L__gp->tmp_bp, L__gp->tmp_blen, (nlen)); \
                (bp) = L__gp->tmp_bp;                                   \
                (blen) = L__gp->tmp_blen;                               \
                F_SET(L__gp, G_TMP_INUSE);                              \
        } else                                                          \
                BINC_RET((sp), (bp), (blen), (nlen));                   \
}

/* Free a GET_SPACE returned buffer. */

#define FREE_SPACE(sp, bp, blen) {                                      \
        GS *L__gp = (sp) == NULL ? NULL : (sp)->gp;                     \
        if (L__gp != NULL && (bp) == L__gp->tmp_bp)                     \
                F_CLR(L__gp, G_TMP_INUSE);                              \
        else                                                            \
                free(bp);                                               \
}

/*
 * Malloc a buffer, casting the return pointer.  Various versions.
 */

#define CALLOC(sp, p, nmemb, size) {                                    \
        if (((p) = calloc((nmemb), (size))) == NULL)                    \
                msgq((sp), M_SYSERR, NULL);                             \
}

#define CALLOC_GOTO(sp, p, nmemb, size) {                               \
        if (((p) = calloc((nmemb), (size))) == NULL)                    \
                goto alloc_err;                                         \
}

#define CALLOC_RET(sp, p, nmemb, size) {                                \
        if (((p) = calloc((nmemb), (size))) == NULL) {                  \
                msgq((sp), M_SYSERR, NULL);                             \
                return (1);                                             \
        }                                                               \
}

#define MALLOC(sp, p, size) {                                           \
        if (((p) = malloc(size)) == NULL)                               \
                msgq((sp), M_SYSERR, NULL);                             \
}

#define MALLOC_GOTO(sp, p, size) {                                      \
        if (((p) = malloc(size)) == NULL)                               \
                goto alloc_err;                                         \
}

#define MALLOC_RET(sp, p, size) {                                       \
        if (((p) = malloc(size)) == NULL) {                             \
                msgq((sp), M_SYSERR, NULL);                             \
                return (1);                                             \
        }                                                               \
}

#define REALLOC(sp, p, size) {                                          \
        void *tmpp;                                                     \
        if (((tmpp) = (realloc((p), (size)))) == NULL) {                \
                msgq((sp), M_SYSERR, NULL);                             \
                free(p);                                                \
        }                                                               \
        p = tmpp;                                                       \
}

#define REALLOCARRAY(sp, p, nelem, size) {                              \
        void *tmpp;                                                     \
        if (((tmpp) =                                                   \
            (openbsd_reallocarray((p), (nelem), (size)))) == NULL) {    \
                msgq((sp), M_SYSERR, NULL);                             \
                free(p);                                                \
        }                                                               \
        p = tmpp;                                                       \
}

/*
 * Versions of memmove(3) and memset(3) that use the size of the
 * initial pointer to figure out how much memory to manipulate.
 */

#define MEMMOVE(p, t, len)      memmove((p),     (t), (len) * sizeof(*(p)))
#define MEMSET(p, value, len)   memset( (p), (value), (len) * sizeof(*(p)))
