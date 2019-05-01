// Defines the entry point for the DLL application.
//

//#define _CRT_SECURE_NO_WARNINGS
//#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <windows.h>
#include <stdlib.h>
#include <shellapi.h>
#include <malloc.h>
#include <richedit.h>
#include <commdlg.h>
#include <math.h>
#include <algorithm>

#include <sstream>
#include <string>

#include "viewer.h"
#include "project_cfg.h"    // customize viewer

HINSTANCE   hinst;

char  inifilename[MAX_PATH]=PROJECTNAME_LC".ini";  // may be used to save data

int lastloadtime=0;   // Workaround to RichEdit bug

HMODULE FLibHandle=0;

//---------------------------------------------------------------------------
char* strlcpy( char* p, char* p2, int maxlen )
{
  strncpy( p, p2, maxlen );
  p[maxlen] = 0;
  return p;
}


//---------------------------------------------------------------------------
BOOL APIENTRY
DllMain( HANDLE hModule,
         DWORD  ul_reason_for_call,
         LPVOID lpReserved )
{
  switch ( ul_reason_for_call )
  {
    case DLL_PROCESS_ATTACH:
      hinst = (HINSTANCE)hModule;
      break;
    case DLL_PROCESS_DETACH:
      if (FLibHandle)
        FreeLibrary(FLibHandle);
      FLibHandle=NULL;

      break;
    case DLL_THREAD_ATTACH:
      break;
    case DLL_THREAD_DETACH:
      break;
  }

  return TRUE;
}


//---------------------------------------------------------------------------
void __stdcall
ListGetDetectString( char* detectString, int maxlen )
{
  strlcpy( detectString, PARSEFUNCTION, maxlen );
}


//---------------------------------------------------------------------------
void __stdcall
ListSetDefaultParams( ListDefaultParamStruct* dps )
{
  dps->PluginInterfaceVersionHi  = MAJOR_VERSION;
  dps->PluginInterfaceVersionLow = MINOR_VERSION;
  strlcpy( inifilename, dps->DefaultIniName, MAX_PATH-1 );
}


//---------------------------------------------------------------------------
HWND __stdcall
ListLoad( HWND parentWin, char* fileToLoad, int showFlags )
{
  HWND listWin = 0;
  RECT r;

  GetClientRect(parentWin,&r);
  lastloadtime=GetCurrentTime();

  if (!FLibHandle) {
    int OldError = SetErrorMode(SEM_NOOPENFILEERRORBOX);
      FLibHandle = LoadLibrary("Riched20.dll");
    if (FLibHandle < (HINSTANCE)HINSTANCE_ERROR)
      FLibHandle = LoadLibrary("RICHED32.DLL");
    if (FLibHandle < (HINSTANCE)HINSTANCE_ERROR)
      FLibHandle = NULL;
    SetErrorMode(OldError);
  }

  // Create window invisbile, only show when data fully loaded!
  listWin=CreateWindow("RICHEDIT50W","", WS_CHILD | ES_MULTILINE | ES_READONLY | WS_HSCROLL | WS_VSCROLL | ES_NOHIDESEL,
                    r.left,r.top,r.right-r.left,
                    r.bottom-r.top,parentWin,NULL,hinst,NULL);

  if (!listWin)
  {
     listWin=CreateWindow("RichEdit20A","", WS_CHILD | ES_MULTILINE | ES_READONLY | WS_HSCROLL | WS_VSCROLL | ES_NOHIDESEL,
                      r.left,r.top,r.right-r.left,
                      r.bottom-r.top,parentWin,NULL,hinst,NULL);
  }
  if (!listWin)
  {
    listWin=CreateWindow("RICHEDIT","", WS_CHILD | ES_MULTILINE | ES_READONLY | WS_HSCROLL | WS_VSCROLL | ES_NOHIDESEL,
                      r.left,r.top,r.right-r.left,
                      r.bottom-r.top,parentWin,NULL,hinst,NULL);
  }

  if (listWin) {
    CHARFORMAT cf;
    SendMessage(listWin, EM_SETMARGINS, EC_LEFTMARGIN, 8);
    SendMessage(listWin, EM_SETEVENTMASK, 0, ENM_UPDATE);

    memset(&cf,0,sizeof(CHARFORMAT));
    cf.cbSize = sizeof(CHARFORMAT);
    cf.dwMask = CFM_FACE  | CFM_BOLD ;
    cf.dwEffects = CFE_BOLD ;
    strcpy(cf.szFaceName, "Courier New");
    PostMessage(parentWin,WM_COMMAND,MAKELONG(lcp_ansi,itm_fontstyle),(LPARAM)listWin);

    SendMessage(listWin, EM_SETTYPOGRAPHYOPTIONS, (WPARAM)TO_SIMPLELINEBREAK, (LPARAM)TO_SIMPLELINEBREAK);
    SendMessage(listWin, EM_SETSEL,0,0);
    SendMessage(listWin, EM_SETCHARFORMAT, SCF_ALL, (LPARAM)&cf);

    ShowWindow( listWin, SW_SHOW );

    int ret = ListLoadNext( parentWin, listWin, fileToLoad, showFlags );
    if ( ret == LISTPLUGIN_ERROR ) {
        DestroyWindow( listWin );
        listWin = 0;
    }
  }

  return listWin;
}


//---------------------------------------------------------------------------
int __stdcall
ListLoadNext( HWND parentWin, HWND listWin, char* fileToLoad, int showFlags)
{
  std::string g_text;

  // Clear window
  SendMessage( listWin, WM_SETTEXT, 0, (LPARAM)NULL );

  // Call project main viewer
  PROJECT_VIEW_FUNC( listWin, fileToLoad );

  // Scroll to first line
  SendMessage(listWin, EM_SETSEL,0,0);
  PostMessage( parentWin, WM_COMMAND, MAKELONG( 0, itm_percent ), (LPARAM)listWin );
  return LISTPLUGIN_OK;
}


//---------------------------------------------------------------------------
void __stdcall
ListCloseWindow( HWND listWin )
{
  DestroyWindow( listWin );
  return;
}


//---------------------------------------------------------------------------
int __stdcall
ListNotificationReceived( HWND listWin, int message, WPARAM wParam, LPARAM lParam )
{
  int firstvisible;
  int linecount;

  switch ( message ) {
  case WM_COMMAND:
    switch ( HIWORD( wParam ) ) {
    case EN_UPDATE:
    case EN_VSCROLL:
      firstvisible = (int)SendMessage( listWin, EM_GETFIRSTVISIBLELINE, 0, 0 );
      linecount    = (int)SendMessage( listWin, EM_GETLINECOUNT, 0, 0 );
      if ( linecount > 0 ) {
        int percent = MulDiv( firstvisible, 100, linecount );
        PostMessage( GetParent( listWin ), WM_COMMAND,
                     MAKELONG( percent, itm_percent ), (LPARAM)listWin );
      }
      return 0;
    }
    break;
  case WM_NOTIFY:
    break;
  case WM_MEASUREITEM:
    break;
  case WM_DRAWITEM:
    break;
  }
  return 0;
}


//---------------------------------------------------------------------------
int __stdcall
ListSendCommand( HWND listWin, int command, int parameter )
{
  switch ( command ) {
  case lc_copy:
    SendMessage( listWin, WM_COPY, 0, 0 );
    return LISTPLUGIN_OK;
  case lc_newparams:
    HFONT font;
    if ( parameter & lcp_ansi ) {
      font = (HFONT)GetStockObject( ANSI_FIXED_FONT );
    }
    else {
      font = (HFONT)GetStockObject( SYSTEM_FIXED_FONT );
    }
    SendMessage( listWin, WM_SETFONT, (WPARAM)font, MAKELPARAM( true, 0 ) );
    PostMessage( GetParent( listWin ), WM_COMMAND, MAKELONG( 0, itm_next ), (LPARAM)listWin );
    return LISTPLUGIN_ERROR;
  case lc_selectall:
    SendMessage( listWin, EM_SETSEL, 0, -1 );
    return LISTPLUGIN_OK;
  case lc_setpercent:
    int firstvisible = (int)SendMessage( listWin, EM_GETFIRSTVISIBLELINE, 0, 0 );
    int linecount    = (int)SendMessage( listWin, EM_GETLINECOUNT, 0, 0 );
    if ( linecount > 0 ) {
      int pos = MulDiv( parameter, linecount, 100 );
      SendMessage( listWin, EM_LINESCROLL, 0, pos - firstvisible );
      firstvisible = (int)SendMessage( listWin, EM_GETFIRSTVISIBLELINE, 0, 0 );
      // Place caret on first visible line!
      int firstchar = (int)SendMessage( listWin, EM_LINEINDEX, firstvisible, 0);
      SendMessage( listWin, EM_SETSEL, firstchar, firstchar );
      pos = MulDiv( firstvisible, 100, linecount );
      // Update percentage display
      PostMessage( GetParent( listWin ), WM_COMMAND, MAKELONG( pos, itm_percent ), (LPARAM)listWin );
      return LISTPLUGIN_OK;
    }
    break;
  }

  return LISTPLUGIN_ERROR;
}


//---------------------------------------------------------------------------
int __stdcall
ListSearchText( HWND listWin, char* searchString, int searchParameter )
{
  FINDTEXT find;
  int startPos, Flags;

  if ( ( searchParameter & lcs_findfirst ) && !( searchParameter & lcs_backwards ) ) {
    //Find first: Start at top visible line
    int firstline = (int)SendMessage( listWin, EM_GETFIRSTVISIBLELINE, 0, 0 );
    startPos      = (int)SendMessage( listWin, EM_LINEINDEX, firstline, 0 );
    SendMessage( listWin, EM_SETSEL, startPos, startPos );
  } else {
    //Find next: Start at current selection+1
    SendMessage( listWin, EM_GETSEL, (WPARAM)&startPos, 0 );
    ++startPos;
  }

  find.chrg.cpMin=startPos;
  find.chrg.cpMax=SendMessage(listWin,WM_GETTEXTLENGTH,0,0);
  Flags=0;
  if (searchParameter & lcs_wholewords)
    Flags |= FR_WHOLEWORD;
  if (searchParameter & lcs_matchcase)
    Flags |= FR_MATCHCASE;
  if (!(searchParameter & lcs_backwards))
    Flags |= FR_DOWN;
  find.lpstrText=searchString;

  int index=SendMessage(listWin, EM_FINDTEXT, Flags, (LPARAM)&find);

  if ( index != -1 ) {
    int indexend = index + (int)strlen( searchString );
    SendMessage( listWin, EM_SETSEL, index, indexend );
    int line = (int)SendMessage( listWin, EM_LINEFROMCHAR, index, 0 ) - 3;
    if ( line < 0 )
        line = 0;
    line -= (int)SendMessage( listWin, EM_GETFIRSTVISIBLELINE, 0, 0 );
    SendMessage( listWin, EM_LINESCROLL, 0, line );

    return LISTPLUGIN_OK;
  }

  SendMessage( listWin, EM_SETSEL, -1, -1);  // Restart search at the beginning
  return LISTPLUGIN_ERROR;
}

