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
		
	    (0..10).each do |j|		
	        attrs['price'] = j.to_s
    		Product.create( attrs )
	    end
	end
	
	db.commit	
  end
  
  def search_test
	puts "search_test"  

    ::Rhom::Rhom.database_fullclient_reset_and_logout
	create_test_data

    puts "search_test1"
    time = Time.now.to_f
	
	$products = Product.find(:all, :conditions => {'brand' => 'HTC3'})
	
	@time = ((Time.now.to_f - time) * 10**3 ).to_i
	
	render :action => :search_results
  end

  def create_test
	puts "create_test"

    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://rhodes-store-server.heroku.com/application')
    SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    SyncEngine.set_threaded_mode(true)    
    Rho::RhoConfig.bulksync_state='1'
    
    puts "create_test1"
    time = Time.now.to_f
	
	Product.create( {'brand'=>"HTC", 'name'=>"NameTest", "price"=>'1'} )
	Product.sync
	
	@time = ((Time.now.to_f - time) * 10**3 ).to_i
	
	render :action => :create_results
	
  end

  def bulk_test
    puts "bulk_test"
    
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_fullclient_reset_and_logout
    SyncEngine.set_syncserver('http://store-bulk.rhohub.com/application')
    SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    SyncEngine.set_threaded_mode(true)    
    Rho::RhoConfig.bulksync_state='0'    

    puts "bulk_test1"
    time = Time.now.to_f
    
    SyncEngine.dosync
    
	@time = ((Time.now.to_f - time) * 10**3 ).to_i
	
	render :action => :bulk_results
    
  end
  
  def start__
    @count = 1000
	@bench_results = []
	#test = { 'search' => :search_test, 'create' => :create_test}
	test = { 'create' => :create_test}
	
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_full_reset_and_logout
    SyncEngine.set_syncserver('http://rhodes-store-server.heroku.com/application')
    SyncEngine.login('lars', 'larspass', "/app/Settings/login_callback")	
    SyncEngine.set_threaded_mode(true)    
    
	#create_test_data
	
	test.each do |name, func|
	    time = Time.now.to_f
		self.send func
		time = ((Time.now.to_f - time) * 10**3 ).to_i
		@bench_results << {'name'=> name, 'time'=>"#{time}"}
	end
	
	render :action => :index
  end
  
end