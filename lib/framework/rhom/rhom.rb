#------------------------------------------------------------------------
# (The MIT License)
# 
# Copyright (c) 2008-2011 Rhomobile, Inc.
# 
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
# 
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
# 
# http://rhomobile.com
#------------------------------------------------------------------------

#  This module represents the rhodes mini OM

require 'rhom/rhom_object_factory'
require 'rhom/rhom_object'
require 'rhom/rhom_db_adapter'
require 'rhom/rhom_model'

module Rhom
  class RecordNotFound < StandardError
  end

  class Rhom
    attr_accessor :factory
  
    def initialize
      @factory = RhomObjectFactory.new
    end
    
    class << Rhom
      def client_id
        c_id = ::Rho::RHO.get_user_db().select_from_table('client_info','client_id')[0]
        c_id.nil? ? nil : c_id['client_id']
      end

      def database_client_reset(reset_local_models=true)
        puts "database_client_reset : reset_local_models=#{reset_local_models}"      
        
		#load all partitions
		Rho::RHO.load_all_sources

        old_interval = SyncEngine.set_pollinterval(0)
        SyncEngine.stop_sync
        
        params = ["", "", 0]
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET client_id=?, token=?, token_sent=?", params)
        
        if ( Rho::RhoConfig.exists?('bulksync_state') )
            Rho::RhoConfig.bulksync_state='0'
        end    
        
        ::Rho::RHO.get_db_partitions().each do |partition, db|
            next if !reset_local_models && partition == 'local'
            
            db.execute_sql("UPDATE sources SET token=0")        
            db.destroy_tables( :exclude => (['sources','client_info']) )
        end
      
        hash_migrate = {}
        ::Rho::RHO.init_schema_sources(hash_migrate) 
        
        SyncEngine.set_pollinterval(old_interval)
      end

      def database_local_reset()
        puts "database_local_reset"
        
		#load all partitions
		Rho::RHO.load_all_sources

        ::Rho::RHO.get_db_partitions().each do |partition, db|
        
            next if partition != 'local'        
            
            db.destroy_tables( :exclude => ['sources','client_info'] )
        end
      
        hash_migrate = {}
        ::Rho::RHO.init_schema_sources(hash_migrate) 
        
      end

      def database_full_reset_ex(*args)
        puts "database_full_reset_ex #{args}"
        
        if (args.count == 0 || !args[0][:models])
        
            database_full_reset( 
              args.count > 0 && !args[0][:reset_client_info].nil?() ? args[0][:reset_client_info] : false, 
              args.count > 0 && !args[0][:reset_local_models].nil?() ? args[0][:reset_local_models] : true )
            
            return    
        end

        raise ArgumentError, "reset_client_info should not be true if reset selected models" if args.count > 0 && args[0][:reset_client_info]
        
		#load all partitions
		Rho::RHO.load_all_sources

        old_interval = SyncEngine.set_pollinterval(0)
        SyncEngine.stop_sync
        
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET reset=1")
        
        Rho::RhoConfig.reset_models = ""
        
        args[0][:models].each do |src_name|
            db = ::Rho::RHO.get_src_db(src_name)
            src_partition = Rho::RhoConfig.sources[src_name]['partition']
            is_schema_source = !Rho::RhoConfig.sources[src_name]['schema'].nil?
            
            next if !args[0][:reset_local_models] && src_partition == 'local'        

            if (src_partition != 'local')
                Rho::RhoConfig.reset_models += "&sources[][name]=#{src_name}"
            end
                
            begin
              db.start_transaction
              db.execute_sql("UPDATE sources SET token=0 WHERE name = ?", src_name )
                      
              if is_schema_source
                db.execute_sql("DELETE FROM #{src_name}")
              else
                db.execute_sql("DELETE FROM object_values WHERE source_id=?", Rho::RhoConfig.sources[src_name]['source_id'].to_i)
              end
              
              db.commit
 
            rescue Exception => e
              puts 'database_full_reset_ex Exception: ' + e.inspect
              db.rollback
              
              raise    
            end    
        
        end

        #hash_migrate = {}
        #::Rho::RHO.init_schema_sources(hash_migrate) 
        
        SyncEngine.set_pollinterval(old_interval)
                        
      end
      
      def database_full_reset(reset_client_info=false, reset_local_models=true)
        puts "database_full_reset : reset_client_info=#{reset_client_info}, reset_local_models=#{reset_local_models}"
        
		#load all partitions
		Rho::RHO.load_all_sources

        old_interval = SyncEngine.set_pollinterval(0)
        SyncEngine.stop_sync
        
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET reset=1")
        if ( Rho::RhoConfig.exists?('bulksync_state') )
            Rho::RhoConfig.bulksync_state='0'
        end    
        
        ::Rho::RHO.get_db_partitions().each do |partition, db|
        
            next if !reset_local_models && partition == 'local'        
            
            db.execute_sql("UPDATE sources SET token=0")        
            db.destroy_tables( :exclude => (reset_client_info ? ['sources'] : ['sources','client_info']) )
        end

        if ( reset_client_info && Rho::RhoConfig.exists?('push_pin') )
            Rho::RhoConfig.push_pin=''
        end    
      
        hash_migrate = {}
        ::Rho::RHO.init_schema_sources(hash_migrate) 
        
        SyncEngine.set_pollinterval(old_interval)
      end
      
      def database_full_reset_and_logout
        database_full_reset
        SyncEngine.logout
      end

      def database_fullclient_reset_and_logout
        database_full_reset(true)
        SyncEngine.logout
      end

        def search(args)
          searchParams = ""
          
          searchParams += '&offset=' + Rho::RhoSupport.url_encode(args[:offset]) if args[:offset]
          searchParams += '&max_results=' + Rho::RhoSupport.url_encode(args[:max_results]) if args[:max_results]

          callbackParams = args[:callback_param] ? args[:callback_param] : ""
          
          if args[:search_params]
            args[:search_params].each do |key,value|
              searchParams += '&' + "conditions[#{Rho::RhoSupport.url_encode(key)}]" + '=' + Rho::RhoSupport.url_encode(value)
              callbackParams += '&' + "search_params[#{Rho::RhoSupport.url_encode(key)}]" + '=' + Rho::RhoSupport.url_encode(value)
            end  
          end
            
          #set_notification(args[:callback], args[:callback_param]) if args[:callback]
          src_ar = args[:sources]
          
          #check sources
          raise ArgumentError, "no sources on search" if src_ar.nil? or src_ar.length == 0
          src_ar.each do |src_name|
             raise ArgumentError, "no sources on search" if Rho::RhoConfig::sources[src_name].nil?
          end
          
          SyncEngine.dosearch_source(src_ar, args[:from] ? args[:from] : 'search',
            searchParams, args[:sync_changes] ? args[:sync_changes] : false, args[:progress_step] ? args[:progress_step] : -1,
            args[:callback], callbackParams )
        end
      
    end #class methods
  end # Rhom
end # Rhom
