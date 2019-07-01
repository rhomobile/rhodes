#ifndef RHODESLIB_H
#define RHODESLIB_H
#include <string>
#include "IRhoRuby.h"

std::string executeRubyMethodWithJSON(const char* full_class_name, const char* method_name, const char* parameters);
void rholib_init(const std::string& rootPath, const std::string& userPath, const std::string& runTimePath,
                 const std::string& logPath,  const std::string& httpProxy, const std::string& logPort, bool isJs);
void executeInRubyThread(rho::ruby::IRunnable* runnable);
std::string getRubyServerURL();

#endif // RHODESLIB_H
