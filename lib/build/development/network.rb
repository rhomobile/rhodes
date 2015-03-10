module RhoDevelopment

  # Returns available network interfaces

  class Network
    # Returns array with available network interfaces on the machine.
    # IPV6, loopback and multicast interfaces are excepted.
    # @return [Array] array with available IP4V network interfaces.

    def self.available_addresses
      addresses = (Socket.ip_address_list.select { |each| each.ipv4? and !each.ipv4_loopback? and !each.ipv4_multicast? }.map { |each| each.ip_address }.uniq)
      return addresses
    end


  end

end