require 'rho/rhocontroller'

class WikipediaPageController < Rho::RhoController

  def index
    render
  end
  
  def show
    puts "WikipediaPage show"
    
    @page = WikipediaPage.find(:all)[0]

    page_pieces = []
    # elements start at 0
    0.upto(@page.packet_count.to_i - 1) do |page|
     page_pieces << @page.send("p_#{page}".to_s)
    end
    encoded_page = page_pieces.join

    @data = encoded_page.unpack("m")[0]

    render :action => :show, :layout => false
  end
  
  def create
    puts "WikipediaPage create with #{@params['search'].inspect.to_s}\n"
    
    @page = WikipediaPage.new(:search => @params['search'])
    @page.save
    
    SyncEngine::dosync

    redirect :action => :show
  end
end