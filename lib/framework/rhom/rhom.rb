#
#  rhom.rb
#  rhodes
#  This module represents the rhodes mini OM
#
#  Copyright (C) 2008 Rhomobile, Inc. All rights reserved.
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

      def database_client_reset
        SyncEngine.stop_sync
        
        params = ["", "", 0]
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET client_id=?, token=?, token_sent=?", params)
        
        if ( Rho::RhoConfig.exists?('bulksync_state') )
            Rho::RhoConfig.bulksync_state='0'
        end    
        ::Rho::RHO.get_user_db().execute_sql("UPDATE sources SET token=0")
        
        ::Rho::RHO.get_db_partitions().each_value do |db|
            db.destroy_tables(
             :exclude => (['sources','client_info']) )
        end
      
        hash_migrate = {}
        ::Rho::RHO.init_schema_sources(hash_migrate) 
      end
      
      def database_full_reset(reset_client_info=false)
        SyncEngine.stop_sync
        
        ::Rho::RHO.get_user_db().execute_sql("UPDATE client_info SET reset=1")
        if ( Rho::RhoConfig.exists?('bulksync_state') )
            Rho::RhoConfig.bulksync_state='0'
        end    
        ::Rho::RHO.get_user_db().execute_sql("UPDATE sources SET token=0")
        
        ::Rho::RHO.get_db_partitions().each_value do |db|
            db.destroy_tables(
             :exclude => (reset_client_info ? ['sources'] : ['sources','client_info']) )
        end
      
        hash_migrate = {}
        ::Rho::RHO.init_schema_sources(hash_migrate) 
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
