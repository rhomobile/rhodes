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
  def self.login
    1
  end
  def self.logout
  end
  def self.logged_in
    1
  end
end
require 'rho'
class << Rho::RhoFSConnector
  def __rhoGetCurrentDir
    "../../../apps/"
  end
end
