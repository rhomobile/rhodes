# encoding: utf-8

require 'tmpdir'
require 'socket'
require 'fileutils'
require 'rbconfig'
require 'thread'

class Object
  def systemu(*a, &b) SystemUniversal.new(*a, &b).systemu end
end

class SystemUniversal
#
# constants
#
  SystemUniversal::VERSION = '2.6.4' unless SystemUniversal.send(:const_defined?, :VERSION)
  def SystemUniversal.version() SystemUniversal::VERSION end
  def version() SystemUniversal::VERSION end
  def SystemUniversal.description
    "universal capture of stdout and stderr and handling of child process pid for windows, *nix, etc."
  end
#
# class methods
#

  @host = Socket.gethostname
  @ppid = Process.ppid
  @pid = Process.pid
  @turd = ENV['SYSTEMU_TURD']

  c = begin; ::RbConfig::CONFIG; rescue NameError; ::Config::CONFIG; end
  ruby = File.join(c['bindir'], c['ruby_install_name']) << c['EXEEXT']
  @ruby = if system('%s -e 42' % ruby)
    ruby
  else
    system('%s -e 42' % 'ruby') ? 'ruby' : warn('no ruby in PATH/CONFIG')
  end

  class << SystemUniversal
    %w( host ppid pid ruby turd ).each{|a| attr_accessor a}

    def quote(*words)
      words.map{|word| word.inspect}.join(' ')
    end
  end

#
# instance methods
#

  def initialize argv, opts = {}, &block
    getopt = getopts opts

    @argv = argv
    @block = block

    @stdin = getopt[ ['stdin', 'in', '0', 0] ]
    @stdout = getopt[ ['stdout', 'out', '1', 1] ]
    @stderr = getopt[ ['stderr', 'err', '2', 2] ]
    @env = getopt[ 'env' ]
    @cwd = getopt[ 'cwd' ]

    @host = getopt[ 'host', self.class.host ]
    @ppid = getopt[ 'ppid', self.class.ppid ]
    @pid = getopt[ 'pid', self.class.pid ]
    @ruby = getopt[ 'ruby', self.class.ruby ]
  end

  def systemu
    tmpdir do |tmp|
      c = child_setup tmp
      status = nil

      begin
        thread = nil

        quietly{
          IO.popen "#{ quote(@ruby) } #{ quote(c['program']) }", 'rb+' do |pipe|
            line = pipe.gets
            case line
              when %r/^pid: \d+$/
                cid = Integer line[%r/\d+/]
              else
                begin
                  buf = pipe.read
                  buf = "#{ line }#{ buf }"
                  e = Marshal.load buf
                  raise unless Exception === e
                  raise e
                rescue
                  raise "systemu: Error - process interrupted!\n#{ buf }\n"
                end
            end
            thread = new_thread cid, @block if @block
            pipe.read rescue nil
          end
        }
        status = $?
      ensure
        if thread
          begin
            class << status
              attr 'thread'
            end
            status.instance_eval{ @thread = thread }
          rescue
            42
          end
        end
      end

      if @stdout or @stderr
        open(c['stdout'], 'rb'){|f| relay f => @stdout} if @stdout
        open(c['stderr'], 'rb'){|f| relay f => @stderr} if @stderr
        status
      else
        [status, open(c['stdout'], 'rb'){|f| f.read}, open(c['stderr'], 'rb'){|f| f.read}]
      end
    end
  end

  def quote *args, &block
    SystemUniversal.quote(*args, &block)
  end

  def new_thread child_pid, block
    q = Queue.new
    Thread.new(child_pid) do |cid|
      current = Thread.current
      current.abort_on_exception = true
      q.push current
      block.call cid
    end
    q.pop
  end

  def child_setup tmp
    stdin = File.expand_path(File.join(tmp, 'stdin'))
    stdout = File.expand_path(File.join(tmp, 'stdout'))
    stderr = File.expand_path(File.join(tmp, 'stderr'))
    program = File.expand_path(File.join(tmp, 'program'))
    config = File.expand_path(File.join(tmp, 'config'))

    if @stdin
      open(stdin, 'wb'){|f| relay @stdin => f}
    else
      FileUtils.touch stdin
    end
    FileUtils.touch stdout
    FileUtils.touch stderr

    c = {}
    c['argv'] = @argv
    c['env'] = @env
    c['cwd'] = @cwd
    c['stdin'] = stdin
    c['stdout'] = stdout
    c['stderr'] = stderr
    c['program'] = program
    open(config, 'wb'){|f| Marshal.dump(c, f)}

    open(program, 'wb'){|f| f.write child_program(config)}

    c
  end

  def quietly
    v = $VERBOSE
    $VERBOSE = nil
    yield
  ensure
    $VERBOSE = v
  end

  def child_program config
    <<-program
      # encoding: utf-8

      PIPE = STDOUT.dup
      begin
        config = Marshal.load(IO.read('#{ config }'))

        argv = config['argv']
        env = config['env']
        cwd = config['cwd']
        stdin = config['stdin']
        stdout = config['stdout']
        stderr = config['stderr']

        Dir.chdir cwd if cwd
        env.each{|k,v| ENV[k.to_s] = v.to_s} if env

        STDIN.reopen stdin
        STDOUT.reopen stdout
        STDERR.reopen stderr

        PIPE.puts "pid: \#{ Process.pid }"
        PIPE.flush                        ### the process is ready yo!
        PIPE.close

        exec *argv
      rescue Exception => e
        PIPE.write Marshal.dump(e) rescue nil
        exit 42
      end
    program
  end

  def relay srcdst
    src, dst, _ = srcdst.to_a.first
    if src.respond_to? 'read'
      while((buffer = src.read(8192))); dst << buffer; end
    else
      if src.respond_to?(:each_line)
        src.each_line{|buf| dst << buf}
      else
        src.each{|buf| dst << buf}
      end
    end
  end

  def slug_for(*args)
    options = args.last.is_a?(Hash) ? args.pop : {}
    join = (options[:join] || options['join'] || '_').to_s
    string = args.flatten.compact.join(join)
    words = string.to_s.scan(%r|[/\w]+|)
    words.map!{|word| word.gsub %r|[^/0-9a-zA-Z_-]|, ''}
    words.delete_if{|word| word.nil? or word.strip.empty?}
    words.join(join).downcase.gsub('/', (join * 2))
  end

  def tmpdir d = Dir.tmpdir, max = 42, &b
    i = -1 and loop{
      i += 1

      tmp = File.join(d, slug_for("systemu_#{ @host }_#{ @ppid }_#{ @pid }_#{ rand }_#{ i += 1 }"))

      begin
        Dir.mkdir tmp
      rescue Errno::EEXIST
        raise if i >= max
        next
      end

      break(
        if b
          begin
            b.call tmp
          ensure
            FileUtils.rm_rf tmp unless SystemU.turd
          end
        else
          tmp
        end
      )
    }
  end

  def getopts opts = {}
    lambda do |*args|
      keys, default, _ = args
      catch(:opt) do
        [keys].flatten.each do |key|
          [key, key.to_s, key.to_s.intern].each do |k|
            throw :opt, opts[k] if opts.has_key?(k)
          end
        end
        default
      end
    end
  end
end

# some monkeypatching for JRuby
if defined? JRUBY_VERSION
  require 'jruby'
  java_import org.jruby.RubyProcess

  class SystemUniversal
    def systemu
      split_argv = JRuby::PathHelper.smart_split_command @argv
      process = java.lang.Runtime.runtime.exec split_argv.to_java(:string)

      stdout, stderr = [process.input_stream, process.error_stream].map do |stream|
        StreamReader.new(stream)
      end

      exit_code = process.wait_for
      field = process.get_class.get_declared_field("pid")
      field.set_accessible(true)
      pid = field.get(process)
      [
        RubyProcess::RubyStatus.new_process_status(JRuby.runtime, exit_code, pid),
        stdout.join,
        stderr.join
      ]
    end

    class StreamReader
      def initialize(stream)
        @data = ""
        @thread = Thread.new do
          reader = java.io.BufferedReader.new java.io.InputStreamReader.new(stream)

          while line = reader.read_line
            @data << line << "\n"
          end
        end
      end

      def join
        @thread.join
        @data
      end
    end
  end
end



SystemU = SystemUniversal unless defined? SystemU
Systemu = SystemUniversal unless defined? Systemu













if $0 == __FILE__
#
# date
#
  date = %q( ruby -e"  t = Time.now; STDOUT.puts t; STDERR.puts t  " )

  status, stdout, stderr = systemu date
  p [status, stdout, stderr]

  status = systemu date, 1=>(stdout = '')
  p [status, stdout]

  status = systemu date, 2=>(stderr = '')
  p [status, stderr]
#
# sleep
#
  sleep = %q( ruby -e"  p(sleep(1))  " )
  status, stdout, stderr = systemu sleep
  p [status, stdout, stderr]

  sleep = %q( ruby -e"  p(sleep(42))  " )
  status, stdout, stderr = systemu(sleep){|cid| Process.kill 9, cid}
  p [status, stdout, stderr]
#
# env
#
  env = %q( ruby -e"  p ENV['A']  " )
  status, stdout, stderr = systemu env, :env => {'A' => 42}
  p [status, stdout, stderr]
#
# cwd
#
  env = %q( ruby -e"  p Dir.pwd  " )
  status, stdout, stderr = systemu env, :cwd => Dir.tmpdir
  p [status, stdout, stderr]
end
