#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

module Rho
  class RhoError < Exception
    ERR_NONE = 0
    ERR_NETWORK = 1
    ERR_REMOTESERVER = 2
    ERR_RUNTIME = 3
    ERR_UNEXPECTEDSERVERRESPONSE = 4
    ERR_DIFFDOMAINSINSYNCSRC = 5
    ERR_NOSERVERRESPONSE = 6
    ERR_CLIENTISNOTLOGGEDIN = 7
    ERR_CUSTOMSYNCSERVER = 8
    ERR_UNATHORIZED = 9
    ERR_CANCELBYUSER = 10
    ERR_SYNCVERSION = 11
    ERR_GEOLOCATION = 12
    
    attr_reader :code
    
    def initialize(err_code)
        @code = err_code
    end
    
    def message
        ::Rho::RhoError.err_message(code)        
    end
    
    def unknown_client?(msg)
       code ==  ERR_REMOTESERVER && msg && msg.downcase == 'unknown client'
    end
    
  end # RhoError
end # Rho

#require 'rholang/localization_simplified'
#LocalizationSimplified.requre_loc('rholang/rhoerror_',false)