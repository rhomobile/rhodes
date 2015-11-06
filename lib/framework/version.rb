module RhodesFramework
  unless defined? RhodesFramework::VERSION
    VERSION = File.read(File.expand_path('../../../version', __FILE__)).strip rescue '5.3.0'
  end
  unless defined? RhodesFramework::DBVERSION
    DBVERSION = '2.2.2'
  end
end