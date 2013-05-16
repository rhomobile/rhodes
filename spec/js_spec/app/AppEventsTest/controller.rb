require 'rho/rhocontroller'
require 'json'

class AppEventsTestController < Rho::RhoController
	
  def set_callback
    Rho::Application.setApplicationNotify( method( :app_callback ) )
  end

  def app_callback(data)
    puts "Handling #{data.to_s}"
    WebView.execute_js("appEventsTest.eventCallback('#{data['applicationEvent']}',#{data['eventData'].to_json});")
  end

end