$local_server.mount_proc '/chunked' do |req,res|
  res.status = 200
  res.chunked = true
  res.body = "1234567890"
end
