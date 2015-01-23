#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2014 Rhomobile, Inc.
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

class IPhoneBuild
  class << self
      def process_output(input,options = {})
        lines = input.encode('UTF-8', :invalid => :replace).split(/\r?\n/) 
        result = []
        lines.each do |data|
          # highligh "error:", "warning:", and "note" messages by adding colored messages
          if data =~ /error:/
            result << "\033[31m \033[1m #{'='*20} ERROR! #{'='*20} \033[22m \033[0m"
          elsif data =~ /warning:/
            result << "\033[34m \033[1m #{'='*20} WARNING! #{'='*20} \033[22m \033[0m"
          elsif data =~ /note:/
            result << "\033[32m  #{'='*20} NOTE! #{'='*20} \033[22m \033[0m"
          # reduce clang trace, leave only file compilation flags
          elsif data =~ /clang/
            m = /(.*?)clang(.*?) -c (.*?) -o (.*?)\.o/.match(data)
            if m 
              result << "#{m[1]}clang ... -c #{m[3]} -o #{m[4]}.o"
              data = ""
            end
          elsif data =~ /error[s]? generated/
            fail "had errors during compilation"
          end
          if data
            result << data
          end
        end

        result.join($/)
      end

      def run_and_trace(cmd,args,options = {})
        if options[:rootdir]
          $rootdir = options[:rootdir]
        else
          $rootdir = ENV['RHO_ROOT']
        end

        if $rootdir.nil?
          $rootdir = File.expand_path(File.join(File.dirname(__FILE__), '..','..','..'))
          puts "== iphonecommon.rb: setting $rootdir to #{$rootdir} =="
        end 

        require File.join($rootdir, 'lib','build','jake')

        Jake.run2(cmd,args,options) do |line|
          puts process_output(line)
          $stdout.flush
        end

        if $?.exitstatus != 0
          fail "Command '#{cmd}' with args '#{args.join(' ')}' failed with code #{$?.exitstatus}"
        end

        $?
      end
  end
end