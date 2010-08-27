require 'rho/rhocontroller'

class PerftestController < Rho::RhoController

  #GET /Perftest
  def index
    render
  end
  
  def create_test
	puts "create_test"
	
	db = Rho::RHO.get_src_db('Perftest')
	db.start_transaction
	
	(0..@count).each do |i|
		Perftest.create( {'name'=>"Test#{i}"} )
	end
	
	db.commit
  end

  def read_test
	puts "read_test"  
	
	items = Perftest.find_all#(:select => ['name'])
  end

  def readbyone_test
	puts "readbyone_test"  

	db = Rho::RHO.get_src_db('Perftest')	
	(0..@count).each do |i|
		item  = Perftest.find(:first, :conditions => {'name'=>"Test#{i}"})
		#db.execute_sql( "SELECT object FROM object_values WHERE attrib=? AND source_id=? AND value = ? LIMIT 1 OFFSET 0", ['name', '1', 'Test0'] )
		#db.execute_sql( "SELECT attrib,value FROM object_values WHERE object=? AND source_id=?", ["52092439016780", "1"] );

	end
  end

  def update_test
	puts "update_test"    
	
	db = Rho::RHO.get_src_db('Perftest')
	db.start_transaction
	
	(0..@count).each do |i|
		item  = Perftest.find(:first, :conditions => {'name'=>"Test#{i}"})
		new_name = item.name + "_TEST"
		item.name = new_name
		item.save
	end
	
	db.commit
	
  end

  def delete_test
	puts "delete_test"
	
    items = Perftest.find_all
	
	db = Rho::RHO.get_src_db('Perftest')
	db.start_transaction
	
	items.each do |item|
		item.destroy
	end
	
	db.commit
  end
  
  def start
    @count = 1000
	@test_results = []
	test = { 'create' => :create_test, 'read all' => :read_test, 'read by one' => :readbyone_test, 'update' => :update_test, 'delete' => :delete_test}
	#test = { 'create' => :create_test, 'read by one' => :readbyone_test}	
	
    SyncEngine.set_threaded_mode(false)
    ::Rhom::Rhom.database_full_reset_and_logout
	
	test.each do |name, func|
	    time = Time.now.to_f
		self.send func
		time = ((Time.now.to_f - time) * 10**3 ).to_i
		@test_results << {'name'=> name, 'time'=>"#{time}"}
	end
	
	render :action => :index
  end
  
end