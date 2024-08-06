require 'yaml'
require 'set'

module RhoPackages

  class NilConfig
    def package_deps(package_name)
      []
    end
  end

  class YmlConfig
    def initialize(config_yml_file)
      @config = YAML::load_file(config_yml_file)

      if OS.windows?
        @platfrom_suffix = 'win'
      elsif OS.mac?
        @platfrom_suffix = 'mac'
      else
        fail
      end
    end

    def repository
      @config['repository']
    end

    def package_deps(package_name)
      gather([], Set.new, [package_name]).reverse
    end

    def package_file(package_name)
      file = package_name
      file += ".#{@platfrom_suffix}" if platform_specific?(package_name)
      file
    end

    def package_load_path(package_name)
      package(package_name).fetch('ruby_load_path', nil)
    end

    private

    def package(package_name)
      @config['packages'].fetch(package_name, {})
    end

    def platform_specific?(package_name)
      package(package_name).fetch('platform_specific', false)
    end

    def deps(package_name)
      package(package_name).fetch('depends_on', [])
    end

    def gather(list, set, package_names)
      package_names.each do |package_name|
        next if set.include?(package_name)

        list << package_name
        set << package_name

        gather(list, set, deps(package_name))
      end
      list
    end
  end

  class NilRepo
    def install(package_name, ruby_load_path)
    end
  end

  class Repo
    def initialize(root_dir)
      @root_dir = root_dir
    end

    def install(package_name, ruby_load_path)
      unless ruby_load_path.nil?
        load_path = File.join(@root_dir, ruby_load_path)
        $LOAD_PATH.unshift(load_path) unless $LOAD_PATH.include?(load_path)
      end

      install_(package_name, ruby_load_path) unless installed?(package_name)
    end

    private

    def install_(package_name, ruby_load_path)
      require_relative 'zip_tar_gz.rb'
      require_relative 'jake.rb'

      fetch(package_name) do |tar_gz_file|
        ZipTarGz.unpack_tar_gz(tar_gz_file, @root_dir)
      end

      Jake.edit_yml(File.join(@root_dir, 'installed.yml')) do |yml|
        yml << package_name
        yml.sort!
      end
    end

    def installed?(package_name)
      YAML::load_file(File.join(@root_dir, 'installed.yml')).include?(package_name)
    end

    def fetch(package_name)
      fail 'overload it'
    end
  end

  class FileRepo < Repo
    def initialize(root_dir, repo_path)
      super(root_dir)
      @repo_path = repo_path
    end

    private

    def fetch(package_name)
      yield File.join(@repo_path, "#{package_name}.tar.gz")
    end
  end

  class HttpRepo < Repo
    def initialize(root_dir, repo_url)
      super(root_dir)
      @repo_url = repo_url
    end

    private

    def fetch(package_name)
      Dir.mktmpdir do |dir|
        filename = "#{package_name}.tar.gz"
        file = File.join(dir, filename)
        http_get("#{@repo_url}/#{filename}", file)
        yield file
      end
    end

    def http_get(url, file, opts={})
      require 'net/https'
      require 'uri'

      uri = URI.parse(url)

      if opts[:proxy].nil?
        http = Net::HTTP.new(uri.host, uri.port)
      else
        proxy_uri = URI.parse(proxy)
        http = Net::HTTP.new(uri.host, uri.port, proxy_uri.host, proxy_uri.port, proxy_uri.user, proxy_uri.password)
      end

      if uri.scheme == 'https'
        http.use_ssl = true
        http.verify_mode = OpenSSL::SSL::VERIFY_NONE
      end

      header_resp = nil
      http.start { header_resp = http.head(uri.path) }

      fail unless header_resp.kind_of?(Net::HTTPSuccess)

      File.open(file, 'wb') do |f|
        http.request_get(uri.path) { |r| f.write(r.body) }
      end
    end
  end

  @@config = NilConfig.new
  @@repo = NilRepo.new

  def self.set_root(root_dir)
    @@config = YmlConfig.new(File.join(root_dir, 'config.yml'))

    repo_path = @@config.repository
    if repo_path.nil?
      @@repo = NilRepo.new
    elsif repo_path.start_with?('http://') or repo_path.start_with?('https://')
      @@repo = HttpRepo.new(root_dir, repo_path)
    else
      @@repo = FileRepo.new(root_dir, repo_path)
    end
  end

  def self.request(*package_names)
    package_names.each do |package_name|
      self.request_(@@config.package_deps(package_name))
    end
  end

  private

  def self.request_(package_names)
    package_names.each do |package_name|
      @@repo.install(@@config.package_file(package_name), @@config.package_load_path(package_name))
    end
  end

end
