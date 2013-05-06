require 'rho'
require 'rho/rhocontroller'
require 'rho/rhoerror'
require 'helpers/browser_helper'
require 'sync_server'
require 'local_server'
require 'push_server'


class SettingsController < Rho::RhoController
  include BrowserHelper
  
  def index
    @msg = @params['msg']
    
    puts "Logged in: #{SyncEngine.logged_in}"
=begin
      if SyncEngine.logged_in
          puts "logging out"
          SyncEngine.logout
          sleep 5
      end
=end
	SyncEngine.set_syncserver("http://#{SYNC_SERVER_HOST}:#{SYNC_SERVER_PORT}")
	RhoConf.set_property_by_name('rhoconnect_push_server',"http://#{PUSH_SERVER_HOST}:#{PUSH_SERVER_PORT}")
	puts "push server is #{RhoConf.get_property_by_name('rhoconnect_push_server')}"
    
      #    System.set_push_notification('/app/Settings/push_callback', '')
	SyncEngine.login('pushclient', 'pushclient', '/app/Settings/login_callback' )
  end

  def login_callback
    errCode = @params['error_code'].to_i
	  
	puts "login callback, error code: #{errCode}"
	  
    if errCode != 0
      if errCode == Rho::RhoError::ERR_CUSTOMSYNCSERVER
        @msg = @params['error_message']
      end
        
      if !@msg || @msg.length == 0   
        @msg = Rho::RhoError.new(errCode).message
      end
      
    end

    host = SPEC_LOCAL_SERVER_HOST
    port = SPEC_LOCAL_SERVER_PORT
	Rho::AsyncHttp.get :url => "http://#{host}:#{port}?error=#{errCode}"
	  
	puts "sent error code to the spec server"
	  
	check_registration
  end
  
  def check_registration
    puts "Check push registration"
  
    host = SPEC_LOCAL_SERVER_HOST
    port = SPEC_LOCAL_SERVER_PORT

    if(Rho::RhoConfig.exist?('push_pin') && Rho::RhoConfig.push_pin != '')
	  puts "RhoConfig: #{Rho::RhoConfig}"
		
      puts "Sending device_id: #{Rho::RhoConfig.push_pin}"
      Rho::AsyncHttp.get :url => "http://#{host}:#{port}?device_id=#{Rho::RhoConfig.push_pin}"
    else
      Rho::Timer.start(5000, url_for(:action=>'check_registration'), '')
    end
  end
	
  def push_callback
	puts "======> PUSH CALLBACK params #{@params.inspect}"
	  
    host = SPEC_LOCAL_SERVER_HOST
    port = SPEC_LOCAL_SERVER_PORT
    exit = nil
    
    if @params['alert']
      case @params['alert']
      when 'exit'
        exit = true
        puts 'Exit command received!!!!!'
      end
    end
	  
	url = "http://#{host}:#{port}?alert=#{@params['alert']}&error=#{@params['error']}"

	  
      if @params['error']
      		puts "skipping response: #{url} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
      else
		puts "sending response: #{url} <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"
		Rho::AsyncHttp.get :url => url
      end
    
    puts "before exit, exit = #{exit}"
    if exit then
        puts "before exit, exit = #{exit}"
        System.exit
        puts "after exit, exit = #{exit}"
    end
    puts "after exit, exit = #{exit}"

  end
 
end
