require 'yaml'
require 'set'

module RhoPackages

  class NilConfig
    def command_deps(command_name)
      []
    end

    def package_deps(package_name)
      []
    end
  end

  class YmlConfig
    def initialize(config_yml_file)
      @config = YAML::load_file(config_yml_file)
    end

    def repository
      @config['repository']
    end

    def command_deps(command_name)
      gather([], Set.new, fetch('commands', command_name)).reverse
    end

    def package_deps(package_name)
      gather([], Set.new, [package_name]).reverse
    end

    private

    def fetch(section, key)
      (@config['dependencies'][section] || {}).fetch(key, [])
    end

    def gather(list, set, package_names)
      package_names.each do |package_name|
        next if set.include?(package_name)

        list << package_name
        set << package_name

        gather(list, set, fetch('packages', package_name))
      end
      list
    end
  end

  class NilRepo
    def require(package_name)
    end
  end

  class Repo
    def initialize(root_dir)
      @root_dir = root_dir
    end

    def require(package_name)
      install(package_name) unless installed?(package_name)
    end

    private

    def install(package_name)
      require_relative 'tar_gzip.rb'
      require_relative 'jake.rb'
      fetch(package_name) do |tar_gz_file|
        TarGzip.unpack(tar_gz_file, @root_dir)
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

  class HttpRepo
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
    @@repo = NilRepo.new if repo_path.nil?
    @@repo = HttpRepo.new(root_dir, repo_path) if repo_path.start_with?('http://') or repo_path.start_with?('https://')
    @@repo = FileRepo.new(root_dir, repo_path)
  end

  def self.require(*package_names)
    package_names.each do |package_name|
      self.require_(@@config.package_deps(package_name))
    end
  end

  def self.require_by_command(command_name)
    self.require_(@@config.command_deps(package_name))
  end

  private

  def self.require_(package_names)
    package_names.each do |package_name|
      @@repo.require(package_name)
    end
  end

end
