require 'rho/rhocontroller'
require 'helpers/browser_helper'

class BenchmarkController < Rho::RhoController
  include BrowserHelper
  
  #GET /Benchmark
  def index
    render
  end
  
  def create_test_data
	db = Rho::RHO.get_src_db('Product')
	db.start_transaction
  
	(0..100).each do |i|
	    attrs = {'brand'=>"HTC#{i}", 'name'=>"Name#{i}"}
		
	    (0..100).each do |j|		
	        attrs['price'] = j.to_s
    		Product.create( attrs )
	    end
	end
	
	db.commit	
  end
  
  def search_test
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://localhost:9292/application')
    SyncEngine.login('', '', "")	
    Rho::RhoConfig.bulksync_state='1'
    $bench_results = "" unless $bench_results
    
    time = Time.now.to_f
	#create_test_data
    Customer.sync

	$bench_results << "   Sync data(10,000) (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
	time = Time.now.to_f
	
	@customers = Customer.find(:all, :conditions => {'JobTitle' => 'PerfManager'})
	
	$bench_results << "   Search (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
    render :action => :customers
    
	$bench_results << "   Render (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
	puts "BENCH results: \n#{$bench_results }"
	render :action => :index
	
  end

  def create_test
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://rhodes-store-server.heroku.com/application')
    SyncEngine.login('', '', "")	
    Rho::RhoConfig.bulksync_state='1'
    SyncEngine.dosync
    
    $bench_results = "" unless $bench_results
    time = Time.now.to_f
	
	Product.create( {'brand'=>"HTC", 'name'=>"NameTest", "price"=>'1'} )
	$bench_results << "   Create 1 item (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

    time = Time.now.to_f
	Product.sync
	$bench_results << "   Sync 1 item (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

    time = Time.now.to_f
	@products = Product.find(:all, :conditions => {'name' => 'NameTest'})
	
	$bench_results << "   Search (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
    render :action => :products
    
	$bench_results << "   Render (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	puts "BENCH results: \n#{$bench_results }"
	render :action => :index
	
  end

  def bulk_test
    $bench_results = "" unless $bench_results  
    time = Time.now.to_f
  
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    #SyncEngine.set_syncserver('http://store-bulk.rhohub.com/application')
    #SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    SyncEngine.set_syncserver('http://192.168.0.62:9292/application')
    SyncEngine.login('', '', "")	
    
    Rho::RhoConfig.bulksync_state='0'    
	$bench_results << "   Reset and login(ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

    time = Time.now.to_f
    
    SyncEngine.dosync
    
	$bench_results << "   Bulk sync(ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
	@customers = Customer.find(:all, :conditions => {'JobTitle' => 'PerfManager'})
	
	$bench_results << "   Search (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
    render :action => :customers
    
	$bench_results << "   Render (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	puts "BENCH results: \n#{$bench_results }"
	render :action => :index
	
  end
  
  def asynchttp_test
    $bench_results = "" unless $bench_results  
    
    ::Rhom::Rhom.database_fullclient_reset_and_logout
      
	time = Time.now.to_f
  
    res = Rho::AsyncHttp.get( :url => 'http://rhodes-perftest-json.heroku.com/test' )
	$bench_results << "   Download (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
	time = Time.now.to_f
    
    #parsed = res['body']
    parsed = Rho::JSON.parse(res['body'])

	$bench_results << "   Parse JSON(ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f

	db = Rho::RHO.get_src_db('Customer')
	db.start_transaction
    
    parsed.each do |id, values|
        #values = srcHash['customer']    
        values['object'] = id
        Customer.create(values)
    end
	db.commit
	    
	$bench_results << "   Insert to DB (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
	@customers = Customer.find(:all, :conditions => {'JobTitle' => 'PerfManager'})
	
	$bench_results << "   Search (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	time = Time.now.to_f
	
    render :action => :customers
    
	$bench_results << "   Render (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

	puts "BENCH results: \n#{$bench_results }"
	render :action => :index
    
  end
  
end