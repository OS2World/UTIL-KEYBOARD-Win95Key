/*
 * stub.c -- Sample stub file that calls the routines to load and
 *           start VX-REXX, with optional support for displaying
 *           a splash screen.
 */

#include <stddef.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <process.h>

#define INCL_DOS
#define INCL_WIN

#include <os2.h>
#include "pmexe.h"
#include "pmexe_rc.h"

#define MAX_PATH 300


static BOOL ShowMessage( char *msg, BOOL askyesno );
static void SplashThread( void *data );

static pmexe_data PMExeData;
static PPIB       ExePIB;
static BOOL       Running = FALSE;
static BOOL       BringToFront = TRUE;

/*
 * main -- The C runtime entry point, as usual...
 */

int main( int argc, char *argv[] )
  {
    HMODULE     module;
    ULONG       version;
    pmexe_data *tmp = NULL;
    PTIB        ptib;

    /* Use some commonsense defaults */

    PMExeData.structSize      = sizeof( PMExeData );
    PMExeData.minVersionLevel = 210;
    PMExeData.timeoutInterval = 10;
    PMExeData.hideAfterLoad   = TRUE;
    PMExeData.bitmapID        = PMEXE_RCDATA_ID;
    PMExeData.displayMessage  = TRUE;

    /* Load the pmexe_data structure if it exists */

    DosGetInfoBlocks( &ptib, &ExePIB );

    if( DosGetResource( ExePIB->pib_hmte, RT_RCDATA, PMEXE_RCDATA_ID,
                        (PPVOID) &tmp ) == 0 ){
        if( tmp->structSize >= 12 ){
            PMExeData = *tmp;
        }

        DosFreeResource( tmp );
    }

    /* Start the thread that displays the splash screen... */

    _beginthread( SplashThread, NULL, 32768, NULL );

    /* Load the DLL... */

    module = PMExeLoadDLL();
    if( !module ){
        BringToFront = FALSE;
        ShowMessage( "Could not find VROBJ.DLL.  Please ensure it is in the LIBPATH.",
                     FALSE );
        goto done;
    }

    /* Check the version... */

    version = PMExeVersion();
    if( version < PMExeData.minVersionLevel ){
        BringToFront = FALSE;
        if( !ShowMessage( "This program requires a higher version of VX-REXX"
                          " to run.  Do you want to try running it "
                          "anyhow?", TRUE ) ){
            goto done;
        }
    }

    /* Run it... */

    Running      = TRUE;
    BringToFront = TRUE;

    PMExeRun( argc, argv );

  done:
    return( 0 );
  }

/*
 * ShowMessage -- Create a message queue and put up a message
 *                box....
 */

static BOOL ShowMessage( char *str, BOOL askyesno )
  {
    HAB     hab;
    HMQ     hmq;
    PSZ     title;
    USHORT  retcode;
    char    path[ MAX_PATH + 1 ];

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0 );

    WinCancelShutdown( hmq, TRUE );

    DosQueryModuleName( ExePIB->pib_hmte, MAX_PATH, path );

    if( strlen( path ) > 0 ){
        title = path + strlen( path );
        while( *(title-1) != '\\' ){
            --title;
        }
    } else {
        title = "VX-REXX Application";
    }

    retcode =  WinMessageBox( HWND_DESKTOP, HWND_DESKTOP, str, title, 0,
                    askyesno ? (MB_QUERY|MB_YESNO) : (MB_ERROR|MB_OK) );

    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return( retcode );
  }

/*
 * SplashThread -- Throw up a simple splash screen.  It's a bit cheesy,
 *                 because all it does is put up a static control as
 *                 a top-level window.  But it works!
 */

static void SplashThread( void *data )
  {
    HAB     hab = NULLHANDLE;
    HMQ     hmq = NULLHANDLE;
    HWND    hWnd = NULLHANDLE;
    char    caption[50];
    ULONG   style;
    QMSG    qmsg;
    ULONG   timerID;

    data = data;

    DosSetPriority( PRTYS_THREAD, PRTYC_REGULAR, 11, 0 );

    hab = WinInitialize( 0 );
    hmq = WinCreateMsgQueue( hab, 0 );

    /* See if the bitmap exists */

    if( PMExeData.bitmapID != 0 ){
        PVOID addr;

        if( DosGetResource( ExePIB->pib_hmte, RT_BITMAP,
                            PMExeData.bitmapID, &addr ) == 0 ){
            DosFreeResource( addr );
        } else {
            PMExeData.bitmapID = 0;
        }
    }

    /* If it does, set up caption & style bits */

    if( PMExeData.bitmapID ){
        #ifdef __WATCOMC__
            caption[0] = '#';
            utoa( PMExeData.bitmapID, &caption[1], 10 );
        #else
            sprintf( caption, "#%d", PMExeData.bitmapID );
        #endif

        style = WS_CLIPSIBLINGS | SS_BITMAP | SS_AUTOSIZE;
    } else {
        if( PMExeData.displayMessage == FALSE ) goto leave;

        strcpy( caption, "Loading..." );
        style = WS_CLIPSIBLINGS | SS_TEXT | DT_CENTER |
                DT_VCENTER | DT_WORDBREAK;
    }

    /* Create the window... */

    hWnd = WinCreateWindow( HWND_DESKTOP, WC_STATIC,
                            caption, style, 0, 0, 200, 50,
                            NULLHANDLE, HWND_TOP, 0,
                            NULL, NULL );

    if( hWnd ){
        LONG  sh, sw;
        RECTL rect;
        LONG  cx, cy;
        LONG  cw, ch;

        sh = WinQuerySysValue( HWND_DESKTOP, SV_CYSCREEN );
        sw = WinQuerySysValue( HWND_DESKTOP, SV_CXSCREEN );

        WinQueryWindowRect( hWnd, &rect );

        cw = rect.xRight - rect.xLeft;
        ch = rect.yTop - rect.yBottom;

        cx = ( sw - cw ) / 2;
        cy = ( sh - ch ) / 2;

        WinSetWindowPos( hWnd, HWND_TOP, cx, cy, cw, ch,
                         SWP_SIZE | SWP_MOVE | SWP_ZORDER | SWP_SHOW );
        WinUpdateWindow( hWnd );
    }

    /*
     * Start the timer and pump the loop.  Since we only have one
     * window, all messages must come from it...
     */

    timerID = WinStartTimer( hab, NULLHANDLE, NULLHANDLE, 1000 );

    while( WinGetMsg( hab, &qmsg, NULLHANDLE, 0, 0 ) ){
        if( qmsg.msg == WM_BUTTON1DOWN || qmsg.msg == WM_BUTTON2DOWN ||
            qmsg.msg == WM_BUTTON3DOWN ){
            break;
        }

        if( qmsg.msg == WM_TIMER ){
            if( Running && PMExeData.hideAfterLoad ) break;
            if( PMExeData.timeoutInterval-- == 0 ) break;

            if( BringToFront ){
                WinSetWindowPos( hWnd, HWND_TOP, 0, 0, 0, 0, SWP_ZORDER );
            }
        }

        WinDispatchMsg( hab, &qmsg );
    }

    if( timerID ){
        WinStopTimer( hab, NULLHANDLE, timerID );
    }

    if( hWnd ){
        WinDestroyWindow( hWnd );
    }

    /* Clean things up... */

  leave:
    WinDestroyMsgQueue( hmq );
    WinTerminate( hab );

    return;
  }

