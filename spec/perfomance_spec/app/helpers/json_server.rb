require 'socket'
require 'webrick'

server = WEBrick::HTTPServer.new :BindAddress => 'localhost', :Port => 5000

puts "LISTEN ON #{server.config[:Port]}"

trap('INT') { server.shutdown }

server.mount_proc('/test') do |req,res|
  res.status = 200
  
  res.body = File.read('perftest.json')
end

server.start