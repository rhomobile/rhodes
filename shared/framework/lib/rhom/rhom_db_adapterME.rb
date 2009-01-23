#
#  rhom_db_adapter.rb
#  rhodes
#
#  Copyright (C) 2008 Lars Burgess. All rights reserved.
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
require 'rhodes'

module Rhom
  class RhomDbAdapter
    
    @@database = nil
    
    class << self
      def open(dbfile=nil)
        puts "DB name = " + dbfile.inspect
        unless @@database or dbfile.nil?
			db = DbAdapter.new(Rhodes::DBVERSION)
			@@database = db
        end
		
      end
    
      def close
        if @@database and not @@database.closed?
          @@database.close
          @@database = nil
        else
          return false
        end
        return true
      end   
    
      # execute a sql statement
      # optionally, disable the factory processing 
      # which returns the result array directly
      def execute_sql
        result = []
		  # Make sure we lock the sync engine's mutex
		  # before we perform a database transaction.
		  # This prevents concurrency issues.
		  begin
			SyncEngine::lock_sync_mutex
			result = yield
			SyncEngine::unlock_sync_mutex
		  rescue Exception => e
			puts "exception when running query: #{e}"
			# make sure we unlock even if there's an error!
			SyncEngine::unlock_sync_mutex
		  end
        puts "returned #{result.length.to_s} records..."
        result
      end
    
      def select_from_table(table=nil,columns=nil,condition=nil,params=nil)
	    execute_sql { @@database.selectFromTable(table, columns, condition, params) }
      end
    
      def insert_into_table(table=nil,values=nil)
	    execute_sql { @@database.insertIntoTable(table, values) }
      end
    
      def delete_from_table(table=nil,condition=nil)
	    execute_sql { @@database.deleteFromTable(table, condition) }
	  end
    
      def delete_all_from_table(table=nil)
	    execute_sql { @@database.deleteAllFromTable(table) }
	  end
    
      def update_into_table(table=nil,values=nil,condition=nil)
		execute_sql { @@database.updateIntoTable(table, values, condition) }
	  end
    end # class methods
  end # RhomDbAdapter
end # Rhom

at_exit do
	Rhom::RhomDbAdapter::close
end
