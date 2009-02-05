require 'rho/rhocontroller'

class WikipediaPageController < Rho::RhoController

  def index
    puts "WikipediaPage index"
    @page = WikipediaPage.find(:all)[0]
    
    # data assemble all the attributes
    puts @page.inspect.to_s
    
    puts "with #{@page.instance_variables.length} instance_variables"
    
    puts "reconstructing #{@page.packet_count} elements"
    puts "expecting #{@page.data_length} bytes"
    
    page_pieces = []
    # elements start at 0
    0.upto(@page.packet_count.to_i - 1) do |page|
      page_pieces << @page.send("p_#{page}".to_s)
    end
    encoded_page = page_pieces.join
    
    puts "encoded_page length = #{encoded_page.length}"
    puts "difference = #{encoded_page.length - @page.data_length.to_i}"

    @data = encoded_page.unpack("m")[0]
    
    render
  end
  
  def create
    puts "WikipediaPage create with #{@params['search'].inspect.to_s}\n"
    
    @page = WikipediaPage.new(:search => @params['search'])
    @page.save
    
    SyncEngine::dosync

    redirect :action => :index
  end
end