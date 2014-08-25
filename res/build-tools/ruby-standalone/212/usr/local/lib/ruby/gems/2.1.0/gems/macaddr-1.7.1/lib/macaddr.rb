##
# Cross platform MAC address determination.  Works for:
# * /sbin/ifconfig
# * /bin/ifconfig
# * ifconfig
# * ipconfig /all
#
# To return the first MAC address on the system:
#
#   Mac.address
#
# To return an array of all MAC addresses:
#
#   Mac.address.list
#

begin
  require 'rubygems'
rescue LoadError
  nil
end

require 'systemu'
require 'socket'

module Mac
  VERSION = '1.7.1'

  def Mac.version
    ::Mac::VERSION
  end

  def Mac.dependencies
    {
      'systemu' => [ 'systemu' , '~> 2.6.2' ]
    }
  end

  def Mac.description
    'cross platform mac address determination for ruby'
  end


  class << self

    ##
    # Accessor for the system's first MAC address, requires a call to #address
    # first

    attr_accessor "mac_address"

    ##
    # Discovers and returns the system's MAC addresses.  Returns the first
    # MAC address, and includes an accessor #list for the remaining addresses:
    #
    #   Mac.addr # => first address
    #   Mac.addr.list # => all addresses

    def address
      return @mac_address if defined? @mac_address and @mac_address

      @mac_address = from_getifaddrs
      return @mac_address if @mac_address

      cmds = '/sbin/ifconfig', '/bin/ifconfig', 'ifconfig', 'ipconfig /all', 'cat /sys/class/net/*/address'

      output = nil
      cmds.each do |cmd|
        _, stdout, _ = systemu(cmd) rescue next
        next unless stdout and stdout.size > 0
        output = stdout and break
      end
      raise "all of #{ cmds.join ' ' } failed" unless output

      @mac_address = parse(output)
    end

    link   = Socket::PF_LINK   if Socket.const_defined? :PF_LINK
    packet = Socket::PF_PACKET if Socket.const_defined? :PF_PACKET
    INTERFACE_PACKET_FAMILY = link || packet # :nodoc:

    def from_getifaddrs
      return unless Socket.respond_to? :getifaddrs

      interfaces = Socket.getifaddrs.select do |addr|
        addr.addr.pfamily == INTERFACE_PACKET_FAMILY
      end

      mac, =
        if Socket.const_defined? :PF_LINK then
          interfaces.map do |addr|
            addr.addr.getnameinfo
          end.find do |m,|
            !m.empty?
          end
        elsif Socket.const_defined? :PF_PACKET then
          interfaces.map do |addr|
            addr.addr.inspect_sockaddr[/hwaddr=([\h:]+)/, 1]
          end.find do |mac_addr|
            mac_addr != '00:00:00:00:00:00'
          end
        end

      @mac_address = mac if mac
    end

    def parse(output)
      lines = output.split(/\n/)

      candidates = lines.select{|line| line =~ RE}
      raise 'no mac address candidates' unless candidates.first
      candidates.map!{|c| c[RE].strip}

      maddr = candidates.first
      raise 'no mac address found' unless maddr

      maddr.strip!
      maddr.instance_eval{ @list = candidates; def list() @list end }
      maddr
    end

    ##
    # Shorter alias for #address

    alias_method "addr", "address"
  end

  RE = %r/(?:[^:\-]|\A)(?:[0-9A-F][0-9A-F][:\-]){5}[0-9A-F][0-9A-F](?:[^:\-]|\Z)/io
end

MacAddr = Macaddr = Mac
