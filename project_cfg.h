#ifndef __PROJECT_CFG_H__
#define __PROJECT_CFG_H__

// KEEP
#define _STR(x) #x
#define STR(x) _STR(x)

#define MAJOR_VERSION 1
#define MINOR_VERSION 0

#define PARSEFUNCTION "SIZE<100000000 & ([0]=127 & [1]=\"E\" & [2]=\"L\" & [3]=\"F\") | FORCE"

#define PROJECTNAME         STR(PROJECT)
#define PROJECTNAME_LC      STR(PROJECT_LC)
#define PROJECT_VIEW_FUNC   elfdump

void elfdump( HWND , std::string );

#endif // __PROJECT_CFG_H__
