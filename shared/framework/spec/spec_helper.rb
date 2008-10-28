begin
  require 'spec'
rescue LoadError
  require 'rubygems'
  gem 'rspec'
  require 'spec'
end

# Stubs for testing
class Fixnum
  def fdiv
  end
end

class Bignum
  def fdiv
  end
end

class SyncEngine
  def self.dosync
  end
  def self.lock_sync_mutex
  end
  def self.unlock_sync_mutex
  end
end

require 'rubygems'
require 'sqlite3'
require 'rhom/rhom'
