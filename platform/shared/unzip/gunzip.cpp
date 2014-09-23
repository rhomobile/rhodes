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


/*
  int UnzipGzip(const char* inputFilename, const char* outputFilename)
  {
      FILE* input;
      FILE* output;
    
      static const int CHUNK_SIZE = 0x4000;
    
      z_stream strm = {0};
      unsigned char in[CHUNK_SIZE];
      unsigned char out[CHUNK_SIZE];
    
      strm.zalloc = Z_NULL;
      strm.zfree = Z_NULL;
      strm.opaque = Z_NULL;
      strm.next_in = in;
      strm.avail_in = 0;
      strm.next_out = out;
    
      int status = 0;
    
      input = fopen(inputFilename, "rb" );
      if ( 0 == input ) goto unzip_gzip_error;
    
      output = fopen(outputFilename, "wb" );
      if ( 0 == output ) goto unzip_gzip_error;
    
      if ( inflateInit2(&strm) < 0 ) goto unzip_gzip_error;

      while (true) {
          int bytes_read;

          bytes_read = fread (in, sizeof (char), sizeof (in), input);
          if (ferror(input)) goto unzip_gzip_error;

          strm.avail_in = bytes_read;
          do {
              strm.avail_out = CHUNK_SIZE;
              if (inflate(&strm, Z_NO_FLUSH) < 0) goto unzip_gzip_error;
            
              fwrite(out, sizeof(char), strm.avail_out, output);
            
              if (ferror(output)) goto unzip_gzip_error;
          }
        
          while (strm.avail_out == 0);
          if (feof (input)) {
              inflateEnd (& strm);
              break;
          }
      }
    
      status |= fclose(input);
      status |= fclose(output);

      return (0==status?0:-1);
    
  unzip_gzip_error:

      inflateEnd (& strm);
    
      fclose(input);
      fclose(output);

      return -1;
  }
  */
  
}