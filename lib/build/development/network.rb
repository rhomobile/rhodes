module RhoDevelopment

  class Network

    def self.available_addresses
      addresses = (Socket.ip_address_list.select { |each| each.ipv4? and !each.ipv4_loopback? and !each.ipv4_multicast? }.map { |each| each.ip_address }.uniq)
      return addresses
    end


  end

end