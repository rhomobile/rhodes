require 'erb'
require 'sqlite3'

class RHO

def serve(req)
	begin
		fname = File.join(File.dirname(File.expand_path(__FILE__)), '../Test/hello.erb')
		puts fname
		puts "Passed parameters: " + req.to_s + "\n"
		
		dbname =  File.join(File.dirname(File.expand_path(__FILE__)), 'syncdb.sqlite')
		puts "DB name = " + dbname
		
		db = SQLite3::Database.new( dbname )
		puts "Db opened\n"
		
		db.execute "create table foo (a,b)"
		
		10.times do |i|
			db.execute "insert into foo values (#{i}, #{i+1})"
		end

		db.execute( "select * from foo" ) do |row|
			puts "Row = " + row.to_s + "\n"
		end
		db.close
		
		erb = ERB.new(IO.read(fname))
		return erb.result
		
	rescue Exception => e
		puts e.message
		return ""
	end	
end

end
