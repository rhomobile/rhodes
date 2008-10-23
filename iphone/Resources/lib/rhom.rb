#
#  rhom.rb
#  rhodes
#  This module represents the rhodes mini OM
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
require 'sqlite3'
require 'rhom_object_factory'
require 'rhom_object'
  
class Rhom
  include RhomObject
  attr_accessor :database, :factory
  
  def initialize
    @factory = RhomObjectFactory.new
    SyncEngine::dosync
  end
  
  class << self
    
    def init_db_connection
      dbname =  File.join(File.dirname(File.expand_path(__FILE__)), '../db/syncdb.sqlite')
      puts "DB name = " + dbname
      @database = SQLite3::Database.new(dbname)
    end
  
    def close_db_connection
      if @database
        @database.close
      else
        return false
      end
      return true
    end
  
    # execute a sql statement
    # optionally, disable the factory processing 
    # which returns the result array directly
    def execute_sql(sql=nil)
      result = []
      if sql
        puts 'query is ' + sql
        # Make sure we lock the sync engine's mutex
        # before we perform a database transaction.
        # This prevents concurrency issues.
        begin
          SyncEngine::lock_sync_mutex
          # execute sql statement inside of transaction
          # result is returned as an array of hashes
          init_db_connection
          @database.transaction
          @database.results_as_hash = true
          result = @database.execute sql
          @database.commit
          close_db_connection
          SyncEngine::unlock_sync_mutex
        rescue
          # make sure we unlock even if there's an error!
          SyncEngine::unlock_sync_mutex
        end
      end
      result
    end
  end # class methods
end # Rhom
