#!/usr/bin/env ruby

require 'net/http'
require 'net/https'
require 'uri'

module Google
  def self.client_login(login, password, source)
    url = URI.parse('https://www.google.com/accounts/ClientLogin')

    data = {}
    data['accountType'] = 'HOSTED_OR_GOOGLE'
    data['Email'] = login
    data['Passwd'] = password
    data['service'] = 'ac2dm'
    data['source'] = source

    req = Net::HTTP::Post.new(url.path)
    req.set_form_data data
    http = Net::HTTP.new(url.host, url.port)
    http.use_ssl = true
    http.verify_mode = OpenSSL::SSL::VERIFY_NONE
    res = http.request(req)
    res.error! unless res.is_a? Net::HTTPSuccess

    res.body.split("\n").each do |line|
      line.chomp!
      next unless line =~ /^Auth=/
      return line.gsub(/^Auth=/, '')
    end

    return nil
  end

  def self.send_c2d_message(regid, authtoken, params)
    url = URI.parse('https://android.apis.google.com/c2dm/send')

    data = {}
    data['registration_id'] = regid
    data['collapse_key'] = `uuidgen`.strip.gsub('-', '')

    params.each do |k,v|
      v = v.join(',') if v.is_a? Array
      data["data.#{k.to_s}"] = v
    end

    req = Net::HTTP::Post.new url.path, 'Authorization' => "GoogleLogin auth=#{authtoken}"
    req.set_form_data data

    http = Net::HTTP.new(url.host, url.port)
    http.use_ssl = true
    http.verify_mode = OpenSSL::SSL::VERIFY_NONE;
    http.request(req)
  end
end

# Example:
#authtoken = Google.client_login('push-app@gmail.com', 'PASSWORD', 'push-app-1')
#puts authtoken
