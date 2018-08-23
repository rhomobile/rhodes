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

require 'logger'


class IPhoneBuild
  class << self

      def log(level, msg)
          if $logger
              $logger.add(level, msg)
          else
              puts msg
          end
      end

      def init_logger
          if $logger.nil?
              $logger = Logger.new(STDOUT)
              if ENV["RHODES_BUILD_LOGGER_LEVEL"] and ENV["RHODES_BUILD_LOGGER_LEVEL"] != ""
                  level = ENV["RHODES_BUILD_LOGGER_LEVEL"]
                  if level == "DEBUG"
                      $logger.level = Logger::DEBUG
                  end
                  if level == "INFO"
                      $logger.level = Logger::INFO
                  end
              end
          end
      end

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
        init_logger
        if options[:rootdir]
          $rootdir = options[:rootdir]
        else
          $rootdir = ENV['RHO_ROOT']
        end

        if $rootdir.nil?
          $rootdir = File.expand_path(File.join(File.dirname(__FILE__), '..','..','..'))
          log  Logger::DEBUG, "== iphonecommon.rb: setting $rootdir to #{$rootdir} =="
        end

        log Logger::INFO,  "run CMD: ["+cmd+"] ARGS: ["+args.to_s+"]"

        require File.join($rootdir, 'lib','build','jake')

        printer = nil

        begin
          require 'xcpretty'

          printer_opts = {
              :unicode => true,
              :colorize => STDIN.tty? && STDOUT.tty?,
              :formatter => XCPretty::Simple
          }

          printer = XCPretty::Printer.new(printer_opts)
        rescue Exception => e
          require File.join(File.dirname(__FILE__),'/../../../lib/build/BuildOutput')
          BuildOutput.note('Please install xcpretty gem in order to have better formatted xcode output','Build hint')
        end

        output_traces = []

        Jake.run2(cmd,args,options) do |line|
          output_traces << line
        end

        level = Logger::DEBUG

        if ($?.exitstatus != 0)
            level = Logger::INFO
        end

        output_traces.each do |line|
            if printer.nil?
                log(level, process_output(line))
            else
                if level == Logger::DEBUG
                    printer.pretty_print(line)
                end
            end
        end
        $stdout.flush

        if $?.exitstatus != 0
          fail "Command '#{cmd}' with args '#{args.join(' ')}' failed with code #{$?.exitstatus}"
        end

        $?
      end
  end
end
