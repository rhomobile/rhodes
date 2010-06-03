#require 'time'
require 'rhom/rhom_object'

module Rhom
  class RhomSource
    include ::Rhom::RhomObject
    attr_reader   :source_id, :name, :last_updated, :last_inserted_size, 
                  :last_deleted_size, :last_sync_duration,
                  :last_sync_success, :distinct_objects, :backend_refresh_time,
                  :partition, :schema, :schema_version, :model_type
                  
    def initialize(args,count=0)
      # setup the name
      # TODO: should really store this in the database
      #Rho::RhoConfig::sources.each do |key,value|
      #  if value['source_id'].to_i == args['source_id'].to_i
      #    @name = key
      #  end
      #end
      @name = args['name']
      @source_id = args['source_id'].to_i
      @last_updated = Time.at(args['last_updated'].to_i)
      @last_inserted_size = args['last_inserted_size'].to_i
      @last_deleted_size = args['last_deleted_size'].to_i
      @last_sync_duration = args['last_sync_duration'].to_i
      @last_sync_success = args['last_sync_success'].to_i == 1 ? true : false
      @backend_refresh_time = Time.at(args['backend_refresh_time'].to_i)
      @partition =  args['partition']
      @schema =  args['schema']
      @schema_version =  args['schema_version']
      @model_type = args['model_type']
      
      #VERY SLOW OPERATION!
      #@distinct_objects = ::Rhom::RhomDbAdapter::select_from_table(
      #                                                      'object_values',
      #                                                       'object',
      #                                                       {"source_id"=>@source_id},
      #                                                       {"distinct"=>true}).length
    end
    
    def distinct_objects
        ::Rho::RHO.get_src_db(@name).select_from_table(
            'object_values',
            'object',
            {"source_id"=>@source_id},
            {"distinct"=>true}).length
    end    

    def get_lastsync_objectcount
        SyncEngine.get_lastsync_objectcount(@source_id.to_i)
    end    
    
    class << self
      include ::Rhom::RhomObject
      def find(*args)
        list = []
        if args.first == :all
          results = ::Rho::RHO.get_src_db().select_from_table('sources', '*')
          
          results.each do |result|
            list << RhomSource.new(result)
          end
        else
          result = ::Rho::RHO.get_src_db().select_from_table('sources', '*', 
                                                            {"source_id" => strip_braces(args.first)}).first
          puts 'result: ' + result.inspect
          list << RhomSource.new(result)
        end
        
        if args.first == :first
            return list.length > 0 ? list[0] : nil
        end 
        
        list
      end
      
      def find_all_ashash
        ar = find(:all)  
        res = {}
        ar.each do |src|
            res[src.name] = src
        end
        
        res
      end
      
      #def update_attributes(params=nil)
      #  if params
      #    ::Rhom::RhomDbAdapter::update_into_table('sources', {"source_url"=>params['source_url']},
      #                                             {"source_id"=>strip_braces(params['source_id'])})
      #  end
      #end
    end
  end
end
