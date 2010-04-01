require 'json'
require 'mechanize'
require 'zip/zip'
require 'uri'

namespace "rhosync" do
  task :config => "config:common" do
    process_rhoconfig(File.join($app_basedir,'rhoconfig.txt'))
    uri = URI.parse($rhoconfig['syncserver'])
    $url = "#{uri.scheme}://#{uri.host}"
    $url = "#{$url}:#{uri.port}" if uri.port && uri.port != 80
    $host = uri.host
    $port = uri.port
    $agent = Mechanize.new
    $appname = $rhoconfig['syncserver'].split('/').last
    puts "appname: #{$appname}"
    $token_file = File.join(ENV['HOME'],'.rhosync_token')
    $token = File.read($token_file) if File.exist?($token_file)
  end
  
  desc "Flush the rhosync database (you will need to run rhosync:get_api_token afterwards)"
  task :reset => :config do
    $agent.post("#{$url}/api/reset",:api_token => $token)
  end
  
  desc "Fetches current api token from rhosync"
  task :get_api_token => :config do
    login = ask "admin login: "
    password = ask "admin password: "
    $agent.post("#{$url}/login", :login => login, :password => password)
    $token = $agent.post("#{$url}/api/get_api_token").body
    File.open($token_file,'w') {|f| f.write $token}
    puts "Token is saved in: #{$token_file}"
  end
  
  desc "Clean rhosync, import application and create new user"
  task :clean_start => [:flushdb, :get_api_token, :import_app, :create_user] do
  end
  
  desc "Creates and subscribes user for application in rhosync"
  task :create_user => :config do
    login = ask "new user login: "
    password = ask "new user password: "
    post("/api/create_user", {:app_name => $appname, :api_token => $token,
      :attributes => {:login => login, :password => password}})
  end
  
  desc "Updates an existing user in rhosync"
  task :update_user => :config do
    login = ask "login: "
    password = ask "password: "
    new_password = ask "new password: "
    post("/api/update_user", {:app_name => $appname, :api_token => $token,
      :login => login, :password => password, :attributes => {:new_password => new_password}})
  end
  
  desc "Reset source refresh time"
  task :reset_refresh_time => :config do
    user = ask "user: "
    source_name = ask "source name: "
    post("/api/set_refresh_time", {:api_token => $token, :app_name => $appname,
      :user_name => user, :source_name => source_name})
  end
  
  desc "Run rhosync source adapter specs"
  task :spec do
    files = File.join($app_basedir,'rhosync/spec/sources/*_spec.rb')
    puts "files: #{files.inspect}"
    Spec::Rake::SpecTask.new('rhosync:spec') do |t|
      t.spec_files = FileList[files]
      t.spec_opts = %w(-fs --color)
      t.rcov = true
      t.rcov_opts = ['--exclude', 'spec/*,gems/*']
    end
  end
end

def post(path,params)
  req = Net::HTTP.new($host,$port)
  resp = req.post(path, params.to_json, 'Content-Type' => 'application/json')
  print_resp(resp, resp.is_a?(Net::HTTPSuccess) ? true : false)
end

def print_resp(resp,success=true)
  if success
    puts "=> OK" 
  else
    puts "=> FAILED"
  end
  puts "=> " + resp.body if resp and resp.body and resp.body.length > 0
end

def archive(path)
  File.join(path,File.basename(path))+'.zip'
end

def ask(msg)
  print msg
  STDIN.gets.chomp
end

def compress(path)
  path.sub!(%r[/$],'')
  FileUtils.rm archive(path), :force=>true
  Zip::ZipFile.open(archive(path), 'w') do |zipfile|
    Dir["#{path}/**/**"].reject{|f|f==archive(path)}.each do |file|
      zipfile.add(file.sub(path+'/',''),file)
    end
    zipfile.close
  end
end

# TODO: Share this code with the framework Rho class
def process_rhoconfig(file)
  begin
    $rhoconfig = {}
    File.open(file).each do |line|
      # Skip empty or commented out lines
      next if line =~ /^\s*(#|$)/
      parts = line.chomp.split('=')
      key = parts[0]
      value = parts[1] if parts[1]
      if key and value
        val = value.strip.gsub(/\'|\"/,'')
        val = val == 'nil' ? nil : val
        $rhoconfig[key.strip] = val
      end  
    end
  rescue Exception => e
    puts "Error opening rhoconfig.txt: #{e}, using defaults."
    puts e.backtrace.join("\n")
  end
end