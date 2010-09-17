require 'rho/rhocontroller'

class BenchmarkController < Rho::RhoController

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
    ::Rhom::Rhom.database_fullclient_reset_and_logout
	
    time = Time.now.to_f
	create_test_data
	
	$bench_results << "   Prepare data(100000) (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
	time = Time.now.to_f
	
	$products = Product.find(:all, :conditions => {'brand' => 'HTC3'})
	
	$bench_results << "   Search (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
  end

  def create_test
    time = Time.now.to_f
	
	Product.create( {'brand'=>"HTC", 'name'=>"NameTest", "price"=>'1'} )
	$bench_results << "   Create 1 item (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

    time = Time.now.to_f
	Product.sync
	$bench_results << "   Sync 1 item (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
  end

  def bulk_test
    time = Time.now.to_f
  
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://store-bulk.rhohub.com/application')
    SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    Rho::RhoConfig.bulksync_state='0'    
	$bench_results << "   Reset and login(ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"

    time = Time.now.to_f
    
    SyncEngine.dosync
    
	$bench_results << "   Bulk sync(ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"
	
  end
  
  def start
	test = { 'Create bench' => :create_test, 'Search bench' => :search_test, 'Bulk bench' => :bulk_test}
	$bench_results = ""
	
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://rhodes-store-server.heroku.com/application')
    SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    Rho::RhoConfig.bulksync_state='1'
    
	test.each do |name, func|
	    time = Time.now.to_f
		self.send func
		$bench_results << "#{name} (ms): #{((Time.now.to_f - time) * 10**3 ).to_i}\n"		
	end
	
	puts "BENCH results: \n#{$bench_results }"
	render :action => :index
  end
  
end