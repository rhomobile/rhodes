# encoding: UTF-8
#
# = uuid.rb - UUID generator
#
# Author:: Assaf Arkin  assaf@labnotes.org
#          Eric Hodel drbrain@segment7.net
# Copyright:: Copyright (c) 2005-2010 Assaf Arkin, Eric Hodel
# License:: MIT and/or Creative Commons Attribution-ShareAlike

require 'fileutils'
require 'thread'
require 'tmpdir'
require 'socket'
require 'macaddr'
require 'digest/sha1'
require 'tmpdir'


##
# = Generating UUIDs
#
# Call #generate to generate a new UUID. The method returns a string in one of
# three formats. The default format is 36 characters long, and contains the 32
# hexadecimal octets and hyphens separating the various value parts. The
# <tt>:compact</tt> format omits the hyphens, while the <tt>:urn</tt> format
# adds the <tt>:urn:uuid</tt> prefix.
#
# For example:
#
#   uuid = UUID.new
#
#   10.times do
#     p uuid.generate
#   end
#
# = UUIDs in Brief
#
# UUID (universally unique identifier) are guaranteed to be unique across time
# and space.
#
# A UUID is 128 bit long, and consists of a 60-bit time value, a 16-bit
# sequence number and a 48-bit node identifier.
#
# The time value is taken from the system clock, and is monotonically
# incrementing.  However, since it is possible to set the system clock
# backward, a sequence number is added.  The sequence number is incremented
# each time the UUID generator is started.  The combination guarantees that
# identifiers created on the same machine are unique with a high degree of
# probability.
#
# Note that due to the structure of the UUID and the use of sequence number,
# there is no guarantee that UUID values themselves are monotonically
# incrementing.  The UUID value cannot itself be used to sort based on order
# of creation.
#
# To guarantee that UUIDs are unique across all machines in the network,
# the IEEE 802 MAC address of the machine's network interface card is used as
# the node identifier.
#
# For more information see {RFC 4122}[http://www.ietf.org/rfc/rfc4122.txt].

class UUID

  # Version number.
  module Version
    version = Gem::Specification.load(File.expand_path("../uuid.gemspec", File.dirname(__FILE__))).version.to_s.split(".").map { |i| i.to_i }
    MAJOR = version[0]
    MINOR = version[1]
    PATCH = version[2]
    STRING = "#{MAJOR}.#{MINOR}.#{PATCH}"
  end

  VERSION = Version::STRING

  ##
  # Clock multiplier. Converts Time (resolution: seconds) to UUID clock
  # (resolution: 10ns)
  CLOCK_MULTIPLIER = 10000000

  ##
  # Clock gap is the number of ticks (resolution: 10ns) between two Ruby Time
  # ticks.
  CLOCK_GAPS = 100000

  ##
  # Version number stamped into the UUID to identify it as time-based.
  VERSION_CLOCK = 0x0100

  ##
  # Formats supported by the UUID generator.
  #
  # <tt>:default</tt>:: Produces 36 characters, including hyphens separating
  #                     the UUID value parts
  # <tt>:compact</tt>:: Produces a 32 digits (hexadecimal) value with no
  #                     hyphens
  # <tt>:urn</tt>:: Adds the prefix <tt>urn:uuid:</tt> to the default format
  FORMATS = {
    :compact => '%08x%04x%04x%04x%012x',
    :default => '%08x-%04x-%04x-%04x-%012x',
    :urn     => 'urn:uuid:%08x-%04x-%04x-%04x-%012x',
  }

  ##
  # MAC address (48 bits), sequence number and last clock
  STATE_FILE_FORMAT = 'SLLQ'

  @state_file = nil
  @mode = nil
  @uuid = nil

  ##
  # The access mode of the state file.  Set it with state_file.

  def self.mode
    @mode
  end

  def self.mode=(mode)
    @mode = mode
  end

  ##
  # Generates a new UUID string using +format+.  See FORMATS for a list of
  # supported formats.

  def self.generate(format = :default)
    @uuid ||= new
    @uuid.generate format
  end

  ##
  # Returns the UUID generator used by generate.  Useful if you need to mess
  # with it, e.g. force next sequence when forking (e.g. Unicorn, Resque):
  #
  # after_fork do
  #   UUID.generator.next_sequence
  # end
  def self.generator
    @uuid ||= new
  end

  ##
  # Call this to use a UUID Server.  Expects address to bind to (SOCKET_NAME is
  # a good default)
  def self.server=(address)
    @uuid = Client.new(address) unless Client === @uuid
  end

  ##
  # Creates an empty state file in #Dir.tmpdir/ruby-uuid or the windows common
  # application data directory using mode 0644.  Call with a different mode
  # before creating a UUID generator if you want to open access beyond your
  # user by default.
  #
  # If the default state dir is not writable, UUID falls back to ~/.ruby-uuid.
  #
  # State files are not portable across machines.
  def self.state_file(mode = 0644)
    return @state_file unless @state_file.nil?

    @mode = mode

    begin
      require 'Win32API'

      csidl_common_appdata = 0x0023
      path = 0.chr * 260
      get_folder_path = Win32API.new('shell32', 'SHGetFolderPath', 'LLLLP', 'L')
      get_folder_path.call 0, csidl_common_appdata, 0, 1, path

      state_dir = File.join(path.strip)
    rescue LoadError
      state_dir = Dir.tmpdir
    end

    @state_file = File.join(state_dir, 'ruby-uuid')

    if !File.writable?(state_dir) || (File.exists?(@state_file) && !File.writable?(@state_file)) then
      @state_file = File.expand_path('.ruby-uuid', '~')
    end

    @state_file
  end

  ##
  # Specify the path of the state file.  Use this if you need a different
  # location for your state file.
  #
  # Set to false if your system cannot use a state file (e.g. many shared
  # hosts).
  def self.state_file=(path)
    @state_file = path
    @mode ||= 0644
  end

  ##
  # Returns true if +uuid+ is in compact, default or urn formats.  Does not
  # validate the layout (RFC 4122 section 4) of the UUID.
  def self.validate(uuid)
    return true if uuid =~ /\A[\da-f]{32}\z/i
    return true if
      uuid =~ /\A(urn:uuid:)?[\da-f]{8}-([\da-f]{4}-){3}[\da-f]{12}\z/i
  end

  ##
  # Generate a pseudo MAC address because we have no pure-ruby way
  # to know  the MAC  address of the  NIC this system  uses.  Note
  # that cheating  with pseudo arresses here  is completely legal:
  # see Section 4.5 of RFC4122 for details.
  #
  # This implementation is shamelessly stolen from
  #  https://github.com/spectra/ruby-uuid/blob/master/uuid.rb
  # Thanks spectra.
  #
  def pseudo_mac_address
    sha1 = ::Digest::SHA1.new
    256.times do
      r = [rand(0x100000000)].pack "N"
      sha1.update r
    end
    str = sha1.digest
    r = rand 14 # 20-6
    node = str[r, 6] || str
    if RUBY_VERSION >= "1.9.0"
      nnode = node.bytes.to_a
      nnode[0] |= 0x01
      node = ''
      nnode.each { |s| node << s.chr }
    else
      node[0] |= 0x01 # multicast bit
    end
    node.bytes.collect{|b|b.to_s(16)}.join.hex & 0x7FFFFFFFFFFF
  end

  ##
  # Uses system calls to get a mac address
  #
  def iee_mac_address
    begin
      Mac.addr.gsub(/:|-/, '').hex & 0x7FFFFFFFFFFF
    rescue
      0
    end
  end

  ##
  # return iee_mac_address if available, pseudo_mac_address otherwise
  #
  def mac_address
    return iee_mac_address unless iee_mac_address == 0
    return pseudo_mac_address
  end

  ##
  # Create a new UUID generator.  You really only need to do this once.
  def initialize
    @drift = 0
    @last_clock = (Time.now.to_f * CLOCK_MULTIPLIER).to_i
    @mutex = Mutex.new

    state_file = self.class.state_file
    if state_file && File.size?(state_file) then
      next_sequence
    else
      @mac = mac_address
      fail "Cannot determine MAC address from any available interface, tried with #{mac_address}" if @mac == 0
      @sequence = rand 0x10000

      # Ensure the mode is respected, even with a restrictive umask
      File.open(state_file, 'w') { |f| f.chmod(self.class.mode) } if state_file && !File.exists?(state_file)

      if state_file
        open_lock 'wb' do |io|
          write_state io
        end
      end
    end
  end

  ##
  # Generates a new UUID string using +format+.  See FORMATS for a list of
  # supported formats.
  def generate(format = :default)
    template = FORMATS[format]

    raise ArgumentError, "invalid UUID format #{format.inspect}" unless template

    # The clock must be monotonically increasing. The clock resolution is at
    # best 100 ns (UUID spec), but practically may be lower (on my setup,
    # around 1ms). If this method is called too fast, we don't have a
    # monotonically increasing clock, so the solution is to just wait.
    #
    # It is possible for the clock to be adjusted backwards, in which case we
    # would end up blocking for a long time. When backward clock is detected,
    # we prevent duplicates by asking for a new sequence number and continue
    # with the new clock.

    clock = @mutex.synchronize do
      clock = (Time.new.to_f * CLOCK_MULTIPLIER).to_i & 0xFFFFFFFFFFFFFFF0

      if clock > @last_clock then
        @drift = 0
        @last_clock = clock
      elsif clock == @last_clock then
        drift = @drift += 1

        if drift < 10000 then
          @last_clock += 1
        else
          Thread.pass
          nil
        end
      else
        next_sequence
        @last_clock = clock
      end
    end until clock

    template % [
        clock        & 0xFFFFFFFF,
       (clock >> 32) & 0xFFFF,
      ((clock >> 48) & 0xFFFF | VERSION_CLOCK),
      @sequence      & 0xFFFF,
      @mac           & 0xFFFFFFFFFFFF
    ]
  end

  ##
  # Updates the state file with a new sequence number.
  def next_sequence
    if self.class.state_file
      open_lock 'rb+' do |io|
        @mac, @sequence, @last_clock = read_state(io)

        io.rewind
        io.truncate 0

        @sequence += 1

        write_state io
      end
    else
      @sequence += 1
    end
  rescue Errno::ENOENT
    open_lock 'w' do |io|
      write_state io
    end
  ensure
    @last_clock = (Time.now.to_f * CLOCK_MULTIPLIER).to_i
    @drift = 0
  end

  def inspect
    mac = ("%012x" % @mac).scan(/[0-9a-f]{2}/).join(':')
    "MAC: #{mac}  Sequence: #{@sequence}"
  end

protected

  ##
  # Open the state file with an exclusive lock and access mode +mode+.
  def open_lock(mode)
    File.open self.class.state_file, mode, self.class.mode do |io|
      begin
        io.flock File::LOCK_EX
        yield io
      ensure
        io.flock File::LOCK_UN
      end
    end
  end

  ##
  # Read the state from +io+
  def read_state(io)
    mac1, mac2, seq, last_clock = io.read(32).unpack(STATE_FILE_FORMAT)
    mac = (mac1 << 32) + mac2

    return mac, seq, last_clock
  end


  ##
  # Write that state to +io+
  def write_state(io)
    mac2 =  @mac        & 0xffffffff
    mac1 = (@mac >> 32) & 0xffff

    io.write [mac1, mac2, @sequence, @last_clock].pack(STATE_FILE_FORMAT)
  end


  # You don't have to use this, it's just a good default.
  SOCKET_NAME ="/var/lib/uuid.sock"

  # With UUID server you don't have to worry about multiple processes
  # synchronizing over the state file, calling next_sequence when forking a
  # process and other things you're probably not worried about (because
  # statistically they're very unlikely to break your code).
  #
  # But if you are worried about and thought to yourself, "what would a simple
  # UUID server look like?", here's the answer.  The protocol is dead simple:
  # client sends a byte, server responds with a UUID.  Can use TCP or domain
  # sockets.
  class Server

    # Create new server.  Nothing interesting happens until you call listen.
    def initialize()
      @generator = UUID.new
    end

    # Start the server listening on the specific address.  Blocks and never
    # returns.  Address can be:
    # - A Socket object
    # - UNIX domain socket name (e.g. /var/run/uuid.sock, must start with /)
    # - IP address, colon, port (e.g. localhost:1337)
    def listen(address)
      sock = bind(address)
      while client = sock.accept
        Thread.start(client) do |socket|
          while socket.read 1
            socket.write @generator.generate
          end
        end
      end
    end

    # Returns UNIXServer or TCPServer from address.  Returns argument if not a
    # string, so can pass through (see #listen).
    def bind(address)
      return address unless String === address
      if address[0] == ?/
        if File.exist?(address)
          raise ArgumentError, "#{address} is not a socket" unless File.socket?(address)
          File.unlink(address)
        end
        sock = UNIXServer.new(address)
        File.chmod 0666, address
      elsif address =~ /^(\d+\.\d+\.\d+\.\d+):(\d+)$/
        sock = TCPServer.new($1, $2.to_i)
      else
        raise ArgumentError, "Don't know how to bind #{address}"
      end
      sock.setsockopt(IPPROTO_TCP, TCP_NODELAY, 1) if defined?(TCP_NODELAY)
      sock
    end

  end


  # Every server needs a client.  Client provides you with the single ultimate
  # method: #generate.  Typically you'll use this instead of the local UUID
  # generator:
  #   UUID.server = UUID::SOCKET_NAME
  class Client

    def initialize(address)
      @socket = connect(address)
      at_exit { close }
    end

    # Talks to server and returns new UUID in specified format.
    def generate(format = :default)
      @socket.write "\0"
      uuid = @socket.read(36)
      return uuid if format == :default
      template = FORMATS[format]
      raise ArgumentError, "invalid UUID format #{format.inspect}" unless template
      template % uuid.split("-").map { |p| p.to_i(16) }
    end

    # Returns UNIXSocket or TCPSocket from address.  Returns argument if not a
    # string, so can pass through.
    def connect(address)
      return address unless String === address
      if address[0] == ?/
        sock = UNIXSocket.new(address)
      elsif address =~ /^(\d+\.\d+\.\d+\.\d+):(\d+)$/
        sock = TCPSocket.new($1, $2.to_i)
      else
        raise ArgumentError, "Don't know how to connect to #{address}"
      end
      sock.setsockopt(IPPROTO_TCP, TCP_NODELAY, 1) if defined?(TCP_NODELAY)
      sock
    end

    def next_sequence #:nodoc: Stubbed to do nothing.
    end

    def inspect
      @socket ? "Server on #{Socket.unpack_sockaddr_in(@socket.getsockname).reverse!.join(':')}" : "Connection closed"
    end

    # Close the socket.
    def close
      @socket.shutdown if @socket
      @socket = nil
    end

  end

end
