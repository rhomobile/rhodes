require 'erb'
require 'sqlite3'

class RHO

def serve(req)
	begin
		fname = File.join(File.dirname(File.expand_path(__FILE__)), '../Test/hello.erb')
		puts fname
		puts "Passed parameters: " + req.to_s + "\n"
		
		db = SQLite3::Database.new( "lib/syncdb.sqlite" )
		#db.execute( "select * from table" ) do |row|
		#	puts row
		#end
		db.close
		
		erb = ERB.new(IO.read(fname))
		return erb.result
		
	rescue Exception => e
		puts e.message
		return ""
	end	
end

end
