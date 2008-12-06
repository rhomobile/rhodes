require 'rho'
require 'rho/rhocontroller'

module Rho
  class SettingsController < Rho::RhoController
    include Rhom::RhomObject
    def get_source(id)
      ::Rhom::RhomDbAdapter::select_from_table('sources','*',{"source_id"=>strip_braces(id)}).first
    end

    def update_source(source, id)
      if source and id
        ::Rhom::RhomDbAdapter::update_into_table('sources',
                                                 {"source_url"=>source['source_url']},
                                                 {"source_id"=>strip_braces(id)})
      end                             
    end

    def get_all_sources
      Rho::RhoConfig::sources
    end
  end
end