if __FILE__==$0
    require 'webrick'
    $local_server = WEBrick::HTTPServer.new :Port => 81
end


$local_server.mount_proc '/chunked' do |req,res|
  res.status = 200
  res.chunked = true
  res.body = "1234567890"
end

modtime = "#{Time.now}"

$local_server.mount_proc '/resume_download' do |req,res|
    puts "#{Time.now} ====== resume_download, req: #{req.inspect}"
    
    body = '1234567890'
    
    res.keep_alive = false
    
    res['Last-Modified'] = modtime
    
    if req.request_method == 'HEAD' then
        res.content_length = body.length
        res.status = 200
        res.body = body
    elsif req.request_method == 'GET' then
        range = req['Range']
        
        if range then
            r = range.split('=')
            
            units = r[0]
            range = r[1]
            
            r = range.split('-')
            
            lo = 0
            hi = body.length - 1
            
            if r[0] and (r[0].length>0) then
                lo = r[0].to_i
            end
            
            if r[1] and (r[1].length>0) then
                hi = r[1].to_i
            end
            
            bad = ( hi < lo ) or ( lo < 0 ) or ( hi > ( body.length-1 ) ) or ( units != 'bytes' )
            
            puts "requsting content-range: #{range}, hi=#{hi},lo=#{lo},bad=#{bad}"
            
            if bad then
                res.status = 503
            else
                res.body = body[lo..hi]
                res.status = 206
                res.content_length = hi - lo + 1
                res['Content-Range'] = "bytes #{lo}-#{hi}/#{body.length}"
            end
        else
            res.content_length = body.length
            res.body = body[0, body.length/2]
            res.status = 503
        end
    end
    
    puts "#{Time.now} ====== resume_download, res: #{res.inspect}"
end

if __FILE__==$0
    $local_server.start
end
