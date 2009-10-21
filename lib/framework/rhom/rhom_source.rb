require 'time'
require 'rhom/rhom_object'

module Rhom
  class RhomSource
    include ::Rhom::RhomObject
    attr_accessor :source_url
    attr_reader   :source_id, :name, :last_updated, :last_inserted_size, 
                  :last_deleted_size, :last_sync_duration,
                  :last_sync_success, :distinct_objects
                  
    def initialize(args,count=0)
      # setup the name
      # TODO: should really store this in the database
      Rho::RhoConfig::sources.each do |key,value|
        if value['source_id'].to_i == args['source_id'].to_i
          @name = key
        end
      end
      @source_id = args['source_id'].to_i
      @source_url = args['source_url']
      @last_updated = Time.at(args['last_updated'].to_i).to_s
      @last_inserted_size = args['last_inserted_size'].to_i
      @last_deleted_size = args['last_deleted_size'].to_i
      @last_sync_duration = args['last_sync_duration'].to_i
      @last_sync_success = args['last_sync_success'].to_i == 1 ? true : false
      #VERY SLOW OPERATION!
      #@distinct_objects = ::Rhom::RhomDbAdapter::select_from_table(
      #                                                      'object_values',
      #                                                       'object',
      #                                                       {"source_id"=>@source_id},
      #                                                       {"distinct"=>true}).length
    end
    
    def distinct_objects
        ::Rhom::RhomDbAdapter::select_from_table(
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
          results = ::Rhom::RhomDbAdapter::select_from_table('sources', '*')
          
          results.each do |result|
            list << RhomSource.new(result)
          end
        else
          result = ::Rhom::RhomDbAdapter::select_from_table('sources', '*', 
                                                            {"source_id" => strip_braces(args.first)}).first
          list << RhomSource.new(result)
        end
        list.size > 1 ? list : list[0]
      end
      
      def update_attributes(params=nil)
        if params
          ::Rhom::RhomDbAdapter::update_into_table('sources', {"source_url"=>params['source_url']},
                                                   {"source_id"=>strip_braces(params['source_id'])})
        end
      end
    end
  end
end
