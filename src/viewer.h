#ifndef _VIEWER__H
#define _VIEWER__H

#define lc_copy            1
#define lc_newparams       2
#define lc_selectall       3
#define lc_setpercent      4

#define lcp_wraptext       1
#define lcp_fittowindow    2
#define lcp_ansi           4
#define lcp_ascii          8
#define lcp_variable      12
#define lcp_forceshow     16
#define lcp_fitlargeronly 32
#define lcp_center        64

#define lcs_findfirst      1
#define lcs_matchcase      2
#define lcs_wholewords     4
#define lcs_backwards      8

#define itm_percent   0xFFFE
#define itm_fontstyle 0xFFFD
#define itm_wrap      0xFFFC
#define itm_fit       0xFFFB
#define itm_next      0xFFFA
#define itm_center    0xFFF9

#define LISTPLUGIN_OK      0
#define LISTPLUGIN_ERROR   1

typedef struct {
    int size;
    DWORD PluginInterfaceVersionLow;
    DWORD PluginInterfaceVersionHi;
    char DefaultIniName[MAX_PATH];
} ListDefaultParamStruct;

#define DECLSPEC  __declspec(dllexport) __stdcall

#ifdef __cplusplus
extern "C" {
#endif

HWND DECLSPEC ListLoad( HWND parentWin, char* fileToLoad, int showFlags );
HWND DECLSPEC ListLoadW( HWND parentWin, WCHAR* fileToLoad, int showFlags );
int  DECLSPEC ListLoadNext( HWND parentWin, HWND pluginWin, char* fileToLoad, int showFlags );
int  DECLSPEC ListLoadNextW( HWND parentWin, HWND pluginWin, WCHAR* fileToLoad, int showFlags );
void DECLSPEC ListCloseWindow( HWND listWin );
void DECLSPEC ListGetDetectString( char* detectString, int maxlen );
int  DECLSPEC ListSearchText( HWND listWin, char* searchString, int searchParameter );
int  DECLSPEC ListSearchTextW( HWND listWin, WCHAR* searchString, int searchParameter );
int  DECLSPEC ListSearchDialog( HWND listWin, int findNext );
int  DECLSPEC ListSendCommand( HWND listWin, int command, int parameter );
int  DECLSPEC ListPrint( HWND listWin, char* fileToPrint, char* defPrinter,
                         int printFlags, RECT* margins );
int  DECLSPEC ListPrintW( HWND listWin, WCHAR* fileToPrint, WCHAR* defPrinter,
                          int printFlags, RECT* margins );
int  DECLSPEC ListNotificationReceived( HWND listWin, int message, WPARAM wParam, LPARAM lParam );
void DECLSPEC ListSetDefaultParams( ListDefaultParamStruct* dps );

HBITMAP DECLSPEC ListGetPreviewBitmap( char* fileToLoad, int width, int height,
                                       char* contentbuf, int contentbuflen );
HBITMAP DECLSPEC ListGetPreviewBitmapW( WCHAR* fileToLoad, int width, int height,
                                        char* contentbuf,int contentbuflen);

#ifdef __cplusplus
}
#endif

#endif // _VIEWER__H
