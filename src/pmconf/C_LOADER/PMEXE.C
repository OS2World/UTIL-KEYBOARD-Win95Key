/*
 * PMEXE.C -- Sample stub file for building your own version of PMEXE.EXE
 *            for use in building VX-REXX programs.
 *
 *  Possible reasons for building your own stub:
 *
 *     1) You want to display a custom splash page.
 *     2) You want to disable version checking, or replace it
 *        with your own dialog.
 *     3) You want to add some external functions without
 *        placing them in a DLL.
 *
 *  To build the stub, simply call PMExeLoadDLL to load the runtime
 *  DLL, PMExeVersion to check the version, and then PMExeRun to run
 *  the VX-REXX program.
 */

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INCL_DOS

#include <os2.h>

#include "pmexe.h"

#define MAX_PATH  300
#define MAX_DRIVE 10

static BOOL IsLANDrive( PSZ path );

/*
 * Declare the types for the entry points...
 */

typedef int ( EXPENTRY _PMAIN )( ULONG argc, PSZ argv[] );
typedef _PMAIN *PMAIN;

typedef ULONG ( EXPENTRY _PVERSION )( PULONG vers_num, PSZ buf, ULONG len );
typedef _PVERSION *PVERSION;

/*
 * PMExeRun -- Call this routine to run your VX-REXX program.  It will
 *             only return when the program has exited.  Simply pass
 *             in the argc, argv pair that gets passed to the main
 *             function of your program.
 */

BOOL PMExeRun( ULONG argc, PSZ argv[] )
  {
    HMODULE  module;
    BOOL     ok = FALSE;
    PMAIN    procaddr = NULLHANDLE;

    module = PMExeLoadDLL();
    if( !module ) goto leave;

    DosQueryProcAddr( module, 0, "pmexe_main", (PFN *) &procaddr );
    if( !procaddr ) goto leave;

    (*procaddr)( argc, argv );

    ok = TRUE;

  leave:
    return( ok );
  }

/*
 * PMExeVersion -- Return the version of the VROBJ.DLL runtime library.
 *                 Returns a ULONG >= 100, where version 1.0 = 100,
 *                 1.01 = 101, 2.0 = 200, 2.0a = 201, etc.  You can
 *                 modify this routine to get back a version string
 *                 (the one that VRVersion returns in REXX) but there's
 *                 really no point...
 */

ULONG PMExeVersion( void )
  {
    HMODULE  module;
    PVERSION procaddr = NULLHANDLE;
    ULONG    which = 0;

    module = PMExeLoadDLL();
    if( !module ) goto leave;

    DosQueryProcAddr( module, 0, "_VROBJ_VERSION_INFO_", (PFN *) &procaddr );
    if( !procaddr ) goto leave;

    (*procaddr)( &which, NULL, 0 );

  leave:
    return( which );
  }

/*
 * PMExeLoadDLL -- Call this routine to search for and load the VROBJ.DLL
 *                 runtime library.  Returns the module handle for the DLL,
 *                 or NULLHANDLE if it couldn't be found.
 *
 *   This routine looks complicated, but it could be simplified if you
 *   wanted.  It first checks to see if VROBJ is in memory.  If not,
 *   it looks in the directory where the executable came from, providing
 *   that directory is not on a LAN Server drive.  Then it just uses
 *   the normal LIBPATH.
 */

HMODULE PMExeLoadDLL( void )
  {
    static HMODULE module = NULLHANDLE;
    HMODULE        dummy;
    char           path[ MAX_PATH + 1 ];
    char           save[ MAX_PATH + 1 ];
    PPIB           pib;
    PTIB           tib;
    PSZ            p;
    ULONG          drive, map;
    FILESTATUS3    infobuf;
    ULONG          len;

    if( module ) goto leave;

    /* See if someone has it loaded already... if so, we still
       have to call DosLoadModule to make sure it gets mapped
       into our space... */

    if( DosQueryModuleHandle( "VROBJ", &dummy ) == 0 ) goto load_it;

    /* Otherwise, figure out where the executable is located, change
       directory to there, then try doing a DosLoadModule.  This assumes
       that '.' is in the LIBPATH */

    DosGetInfoBlocks( &tib, &pib );
    DosQueryModuleName( pib->pib_hmte, MAX_PATH, path );

    if( !IsLANDrive( path ) && strlen( path ) ) {
        p = path + strlen( path );

        while( *p != '\\' ){
            --p;
        }

        p[1] = '\0';

        strcat( path, "VROBJ.DLL" );

        if( DosQueryPathInfo( path, 1, &infobuf,
                              sizeof( infobuf ) ) == 0 ){
            DosQueryCurrentDisk( &drive, &map );

            /* Stupidity to be aware of: DosQueryCurrentDir does not
               return the leading '\', but DosSetCurrentDir requires
               it... */

            save[0] = '\\';
            save[1] = '\0';

            len = MAX_PATH - 1;
            DosQueryCurrentDir( drive, &save[1], &len );

            p[1] = '\0';

            if( *p == '\\' && *(p-1) != ':' ){
                p[0] = '\0';
            }

            DosSetDefaultDisk( tolower( path[0] ) - 'a' + 1 );
            DosSetCurrentDir( path );

            DosLoadModule( NULL, 0, "VROBJ", &module );

            DosSetDefaultDisk( drive );
            DosSetCurrentDir( save );
        }
    }

  load_it:
    if( !module ){
        /* Try just loading it from the LIBPATH... */

        DosLoadModule( NULL, 0, "VROBJ", &module );
    }

  leave:
    return( module );
  }


#define IFS_BUFF_SIZE       100
#define IFS_LAN             "LAN"

static BOOL IsLANDrive( PSZ path )
  {
    PSZ     ifs;
    ULONG   len;
    char    drv[ MAX_DRIVE ];
    char    buff[ IFS_BUFF_SIZE ];

    if( path[ 0 ] == '\\' && path[ 1 ] == '\\' ){
        return( TRUE );
    }

    drv[ 0 ] = path[ 0 ];
    drv[ 1 ] = ':';
    drv[ 2 ] = '\0';

    len = IFS_BUFF_SIZE;

    if( DosQFSAttach( drv, 0, FSAIL_QUERYNAME, (PVOID)buff, &len ) ) {
        return( FALSE );
    }

    ifs = buff + offsetof( FSQBUFFER2, szName ) +
          ( (PFSQBUFFER2)buff )->cbName + 1;

    return( !stricmp( ifs, IFS_LAN ) );
}

