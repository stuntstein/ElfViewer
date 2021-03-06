/*
ELFDump.cpp - Dump ELF file using ELFIO library.

Copyright (C) 2001-2011 by Serge Lamikhov-Center

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#ifdef _MSC_VER
  #define _SCL_SECURE_NO_WARNINGS
  #define ELFIO_NO_INTTYPES
#endif

#include <iostream>
#include <elfio/elfio.hpp>
#include <elfio/elfio_dump.hpp>

#include <windows.h>
#include <richedit.h>

#include "project_cfg.h"

using namespace ELFIO;

//---------------------------------------------------------------------------
void print_str( HWND hwnd, std::string str)
{
  int iTotalTextLength = GetWindowTextLength(hwnd);
  int linecount=SendMessage(hwnd,EM_GETLINECOUNT,0,0);
  int iStartPos;

  iStartPos = iTotalTextLength - (linecount-1);
//  SendMessage(hwnd, EM_SETSEL, (WPARAM)(int)iTotalTextLength, (LPARAM)(int)iTotalTextLength);   // set cursor at the last byte
  SendMessage(hwnd, EM_REPLACESEL, (WPARAM)(BOOL)FALSE, (LPARAM)str.c_str());                           // Insert text
}

//---------------------------------------------------------------------------
void elfdump( HWND hwnd, std::string fileToLoad )
{
  ELFIO::elfio reader;
  unsigned int fileSize;
  std::ostringstream out;

  // Print plugin name and version
  out << PROJECTNAME" v" << std::to_string(MAJOR_VERSION) << "." << std::to_string(MINOR_VERSION);
#if defined(__X64__)
  out << " x64" << std::endl << std::endl;
#else
  out << " x32" << std::endl << std::endl;
#endif

  if ( !reader.load( fileToLoad ) ) {
      out << "Not a valid ELF file!" << std::endl;
      print_str( hwnd,out.str());
      return;
  }

  // get file size
  {
    FILE* pFile;
    pFile = fopen ( fileToLoad.c_str() , "rb" );
    fseek (pFile, 0, SEEK_END);   // non-portable
    fileSize=ftell (pFile);
    fclose ( pFile );
  }

  dump::header         ( out, reader );
  dump::segment_headers( out, reader );

  if (fileSize > 50000000)
  {
    out << std::endl;
    out << "File size greater than 50M. Will not show section table, symbol, dynamic and reloc sections!" << std::endl;
    out << std::endl;
  }
  else
  {
    dump::section_headers( out, reader );
    dump::symbol_tables  ( out, reader );
    dump::notes          ( out, reader );
    dump::dynamic_tags   ( out, reader );
    dump::relocations    ( out, reader );
    dump::section_datas  ( out, reader );
    dump::segment_datas  ( out, reader );
  }

  out << "--------------------------------------------------------" << std::endl
      << "Generated by ELFIO (C++ library for ELF file processing)" << std::endl
      << "http://elfio.sourceforge.net/" << std::endl
      << "Converted to TC plugin by Peter Stein" << std::endl;

  #if defined(_DEBUG_)
  {
    std::time_t result = std::time(nullptr);
    out << std::asctime(std::localtime(&result)) << std::endl;
  }
  #endif

  print_str( hwnd,out.str());
  return;
}
