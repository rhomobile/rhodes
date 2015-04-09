//
//  gunzip.h
//  RhoLib
//
//  Created by Alex Epifanoff on 23.09.14.
//
//

#pragma once

namespace gunzip
{
  int UnzipGzip(const char* inputFilename, const char* outputFilename);
}