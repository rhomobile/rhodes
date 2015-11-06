module Rhodes
  unless defined? Rhodes::VERSION
    VERSION = File.read(File.expand_path('../../version', __FILE__)).strip rescue '5.3.0'
  end
  unless defined? Rhodes::DBVERSION
    DBVERSION = '2.2.0'
  end
  
end
