/*
 * pmexe.h -- Definitions for using the PMExe functions.
 */

#ifndef PMEXE_H_INCLUDED
#define PMEXE_H_INCLUDED


    extern BOOL    PMExeRun( ULONG argc, PSZ argv[] );
    extern ULONG   PMExeVersion( void );
    extern HMODULE PMExeLoadDLL( void );

#endif
