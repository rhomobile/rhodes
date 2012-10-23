require 'rho'
require 'rho/rhocontroller'
require 'rho/rhoerror'
require 'helpers/browser_helper'
require 'openssl'
require 'openssl/digest'
require 'json'
require 'digest'
require 'digest/md5'
require 'digest/sha1'
require 'rho/rhoevent.rb'
require 'time'
require 'rho/rhocontact'
require 'rho/rhobluetooth'
require 'rho/rhotoolbar'
require 'rho/rhotabbar'

require 'generator/generator'
      
class GeneratorController < Rho::RhoController
  include BrowserHelper
  
  def index
    render
  end

  def test
    render :action => :start

    moduleFile = File.new(File.join(Rho::RhoApplication::get_base_app_path(), "test.rb") , "w+")
    moduleFile.puts "def aaa\nend"
    moduleFile.close
  end

  def start
    render :action => :start
    generate    
  end

end
