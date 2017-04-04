//
//  gunzip.cpp
//  RhoLib
//
//  Created by Alex Epifanoff on 23.09.14.
//
//

#include "gunzip.h"
#include "zlib.h"
#include <stdio.h>

namespace gunzip
{

  using namespace zlib;

  int UnzipGzip(const char* inputFilename, const char* outputFilename)
  {
    static const int LENGTH = 0x1000;
    
    gzFile input;
    FILE* output;
    
    input = gzopen( inputFilename, "rb" );
    if ( 0 == input )
    {
      return -1;
    }
    
    output = fopen( outputFilename, "wb" );
    if ( 0 == output )
    {
      gzclose(input);
      return -1;
    }
    
    int status = 0;
    unsigned char buffer[LENGTH];
    
    while (true)
    {
      int bytes_read = gzread(input, buffer, LENGTH);

      if (fwrite(buffer,sizeof(char),bytes_read,output) != bytes_read )
      {
        status = -1;
        break;
      }
    
      if (bytes_read < LENGTH )
      {
        if (!gzeof (input))
        {
            status = -1;
        }
        break;
      }
    }
    
    gzclose(input);
    fclose(output);
    
    return status;
   
  }
}
