require 'thread'
require 'mspec'
#require '../../lib/build/jake'
require 'restclient'

require 'yaml'

puts 'RUBY VERSION: ' + RUBY_VERSION.to_s

if RUBY_VERSION.to_s > "1.9.0"
	require 'psych'
end


cfgfilename = File.join(File.dirname(__FILE__),'config.yml')

$rho_root = nil

#chdir File.dirname(__FILE__)

if File.file?(cfgfilename)
	config = YAML::load_file(cfgfilename)
	$rho_root = config["rhodes"]
	$rhoconnect_root = config["rhoconnect"]
	$rho_root = File.expand_path($rho_root) if $rho_root
	$rhoconnect_root = File.expand_path($rhoconnect_root) if $rhoconnect_root
	$rhoelements_root = config["rhoelements"]
	$rhoelements_root = File.expand_path($rhoelements_root) if $rhoelements_root
end

unless $rho_root
	$rho_root = `get-tau-info --tau-path`.chomp
end

puts "$rho_root is #{$rho_root}"


$spec_path = FileUtils.pwd
$platform = 'android'
$device_id = nil
$server = nil
$requests = []
$signal = ConditionVariable.new
$mutex = Mutex.new
$rhoconnect_use_redis = true

require File.join($rho_root,'lib','build','rhoconnect_helper.rb')
require './run_rhoconnect_spec'

def rhoconnect_request(request,params)
	RestClient.post("#{SYNC_SERVER_HOST}:#{SYNC_SERVER_PORT}/rc/v1/#{request}",params.to_json, :content_type => :json)
end

describe 'Rhoconnect push spec' do
  before(:all) do
	  
	  #TODO: check that Rhoelements gem is installed
	  
	puts "Starting local server"
	$server, addr, port = Jake.run_local_server
	File.open(File.join($spec_path, 'rhoconnect_push_client', 'app', 'local_server.rb'), 'w') do |f|
	  f.puts "SPEC_LOCAL_SERVER_HOST = '#{addr}'"
	  f.puts "SPEC_LOCAL_SERVER_PORT = #{port}"
	end
	  
	$server.mount_proc('/', nil) do |req, res|
	  puts "Request headers: #{req.header.inspect}"
		  
	  query = req.query
	  puts "Request query:   #{query.inspect}"
		  
	  res.status = 200
		  
	  $mutex.synchronize do
		$requests << req
		$signal.signal
	  end
	end

	run_apps($platform)
	  
	@api_token = RhoconnectHelper.api_post('system/login', { :login => 'rhoadmin', :password => '' })
	puts "API token: #{@api_token}"
  end
	
  after(:all) do
	stop_apps
	cleanup_apps
  end
	
  def expect_request(name)
	val = nil
	$mutex.synchronize do
		$signal.wait($mutex)
		
		$requests.count.should == 1
		
		val = $requests.first.query[name]
		
		$requests.clear
    end
	val
  end

  it 'should login' do
	  puts 'Waiting message with login errCode'
	  expect_request('error').should == "0"
  end

  it 'should register' do
    puts 'Waiting message with Rhoconnect registaration...'
	$device_id = expect_request('device_id')
	$device_id.should_not be_nil
	$device_id.should_not == ''
  end

  it 'should proceed push message at foreground' do
    sleep 5
  
    puts 'Sending push message...'
  
	message = 'magic1'
	params = { :user_id=>['pushclient'], :message=>message }
	RhoconnectHelper.api_post('users/ping',params,@api_token)

	puts 'Waiting message with push content...'
	expect_request('alert').should == message
		
	sleep 3
  end

  it 'should proceed push message with exit comand' do
    puts 'Sending push message with exit command...'
	  
	message = 'exit'
	params = { :user_id=>['pushclient'], :message=>message }
	RhoconnectHelper.api_post('users/ping',params,@api_token)

	puts 'Waiting message with push content...'
	expect_request('alert').should == message
    
	sleep 5
    
    output = Jake.run2('adb', ['-e', 'shell', 'ps'], {:hide_output=>true})

    (output =~ /rhoconnect_push_client/).should be_nil
  end

  it 'should start to proceed push message' do
    puts 'Sending push message to start app...'
  
    message = 'magic3'
    params = { :user_id=>['pushclient'], :message=>message}
	RhoconnectHelper.api_post('users/ping',params,@api_token)

    puts 'Waiting message with push content...'
	expect_request('alert').should == message
    
    sleep 5
    
    output = Jake.run2('adb', ['-e', 'shell', 'ps'], {:hide_output=>true})
    
    (output =~ /rhoconnect_push_client/).should_not be_nil
  end

  it 'should proceed sequence of push messages' do
    puts 'Sending 5 push messages...'
    
    alerts = {}
	  
    5.times do |i|
      message = "magic#{i}"
      alerts[message] = true
      params = { :user_id=>['pushclient'], :message=>message}
	  RhoconnectHelper.api_post('users/ping',params,@api_token)
	  sleep 0.5
    end
    
    puts 'Waiting 5 messages with push content...'
    
	5.times do |i|
		$mutex.synchronize do
			break if $requests.count == 5
			$signal.wait($mutex)
		end
        puts "Message count: #{$requests.count}"
	end
      
	$requests.count.should == 5
	  
	$mutex.synchronize do
	  puts alerts.inspect
	  5.times do |i|
        message = $requests[i].query['alert']
        message.should_not be_nil
        puts "message: #{message}"
        alerts[message].should be_true
        alerts[message] = false
        #$requests.delete_at 0
      end
      $requests.clear
    end    
  end

  it 'should start to proceed sequence of push messages' do
    puts 'Sending push message with exit command...'

    message = 'exit'
	params = { :user_id=>['pushclient'], :message=>message }
	RhoconnectHelper.api_post('users/ping',params,@api_token)

    puts 'Waiting message with push content...'
    $mutex.synchronize do
      $signal.wait($mutex)
      
      $requests.count.should == 1

      alert = $requests.first.query['alert']
      
      alert.should_not be_nil
      alert.should == message
      $requests.clear
    end
    
    sleep 5
    
    output = Jake.run2('adb', ['-e', 'shell', 'ps'], {:hide_output=>true})
    
    (output =~ /rhoconnect_push_client/).should be_nil
  
#    puts 'Sending 5 push messages...'
    
#    alerts = {}
  
#    5.times do |i|
#      message = "magic#{i}"
#      alerts[message] = true
#      params = {'device_pin'=>$device_id, 'alert'=>message}
#      RhoPush::Gcm.send_ping_to_device($gcm_api_key, params)
#    end
    
#    puts 'Waiting 5 messages with push content...'
#    $mutex.synchronize do
    
#    5.times do |i|
#        break if $requests.count == 5
#        $signal.wait($mutex)
#    end
      
#    $requests.count.should == 5

#    5.times do |i|
#        message = $requests.first.query['alert']
#        message.should_not be_nil
#        alerts[message].should be_true
#        alerts[message] = false
#        $requests.delete_at 0
#      end
#    end
  end

end