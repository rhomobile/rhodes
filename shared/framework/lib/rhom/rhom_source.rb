require 'time'

module Rhom
  class RhoSource
    attr_reader :source_id, :source_url, 
                :last_updated, :last_inserted_size, 
                :last_deleted_size, :last_sync_duration,
                :last_sync_success
                  
    def initialize(source_id=nil)
      if source_id
        result = ::Rhom::RhomDbAdapter::select_from_table('sources', '*', 
                                                          {"source_id"=>source_id.to_s}).first
        @source_id = source_id
        @source_url = result['source_url']
        @last_updated = Time.new(result['last_updated'])
        @last_inserted_size = result['last_inserted_size']
        @last_deleted_size = result['last_deleted_size']
        @last_sync_duration = result['last_sync_duration']
        @last_sync_success = result['last_sync_success'] == 1 ? true : false
    end
  end
end
