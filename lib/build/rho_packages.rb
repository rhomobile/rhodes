require 'yaml'

module DependencyResolver

  @@config
  
  def self.loadConfig( path )
    @@config = YAML::load_file(path)
  end

  def self.resolveDependencies( command, platform )
    deps = getCommandDependencies( command )

    installed = getInstalledPackages

    puts "Command #{command} dependencies: #{deps}, installed packages #{installed}"

    deps.each { |dep|
      if !installed.include?(dep)
        installPackage(dep)
      end
    }
  end

  def self.getCommandDependencies( command )
    packages = []

    @@config['packages'].each { |name,package|
      commands = package['commands']
    
      packages << name if commands and commands.include?( command )
    }

    deps = packages

    packages.each { |package|
      deps.concat( getPackageDependencies( package ) )
    }

    deps.uniq!

    return deps
  end

  def self.getPackageDependencies( packageName )
    package = @@config['packages'][packageName]
    deps = package['dependencies']

    return [] unless deps

    deps.each { |dep|
      deps.concat(getPackageDependencies( dep ) )
    }

    deps.uniq!

    return deps
  end

  def self.getInstalledPackages
    #TODO: get list of installed packages
    return [ 'core' ]
  end

  def self.getAvailablePackages
    packages = []

    @@config['packages'].each { |name,package|
      packages << name
    }

    return packages
  end

  def self.installPackage(package)
    available = getAvailablePackages

    if !available.include?(package)
      raise "Package #{packages} is not available"
    end

    puts "Installing package #{package}"

    #TODO: download and install packages
    raise 'Not Implemented'
  end

  def self.uninstallModule
    raise 'Not Implemented'
  end

end

DependencyResolver.loadConfig( File.join(File.dirname(__FILE__),'rho_packages.yml'))