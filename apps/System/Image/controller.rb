require 'rho/rhocontroller'

class ImageController < Rho::RhoController

  def index
    puts "Camera index controller"
    @images = Image.find(:all)
    render
  end

  def new
    Camera::take_picture('/System/Image/camera_callback')
    redirect :action => :index
  end

  def edit
    Camera::choose_picture('/System/Image/camera_callback')
    redirect :action => :index
  end

  def show
    puts "Create new image record in the DB"
    puts "Params: " + @params.inspect
    render :action => :show
  end
      
  def camera_callback
    puts "Route camera callback to appropriate view"    
    if @params['status'] == 'ok'
      image = Image.new({'image_uri'=>@params['image_uri']})
      image.save
      puts "new Image object: " + image.inspect
      WebView::refresh
      #WebView::navigate '/System/Camera/show?'+@request['request-body']
    end  
    #reply on the callback
    render :action => :camera_callback, :layout => false
  end
      
end
