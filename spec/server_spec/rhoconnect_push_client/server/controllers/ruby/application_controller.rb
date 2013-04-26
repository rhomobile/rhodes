class ApplicationController < Rhoconnect::Controller::AppBase
  register Rhoconnect::EndPoint

  post "/login", :rc_handler => :authenticate,
                 :deprecated_route => {:verb => :post, :url => ['/application/clientlogin', '/api/application/clientlogin']} do
    login = params[:login]
    password = params[:password]
    true # do some interesting authentication here...
  end

  get "/rps_login", :rc_handler => :rps_authenticate do
    login = params[:login]
    password = params[:password]
    true # optionally handle rhoconnect push authentication...
  end
end
