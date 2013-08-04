
#ifndef PMEXE_RC_H_INCLUDED
#define PMEXE_RC_H_INCLUDED

#define PMEXE_RCDATA_ID  65432

/*
 * This structure mimics the RCDATA structure that you should
 * define in your resource file.
 */

typedef struct pmexe_data {
    USHORT structSize;         /* size of struct in bytes */
    USHORT minVersionLevel;    /* minimum VX-REXX version needed */
    USHORT timeoutInterval;    /* timeout interval in seconds */
    USHORT hideAfterLoad;      /* if non-zero, hide right away */
    USHORT bitmapID;           /* resource ID of bitmap */
    USHORT displayMessage;     /* only if TRUE & bitmap not found */
} pmexe_data;

#endif
