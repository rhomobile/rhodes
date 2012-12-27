ROOT_PATH = File.expand_path(File.dirname(__FILE__)) unless defined? ROOT_PATH

class Application < Rhoconnect::Base
  class << self
    def authenticate(username,password,session)
      # to run spec ok
      if (username == "testUserToFailAuth" && password != "userpass") then
        return false
      end
      true # do some interesting authentication here...
    end
    
    def initializer(path)
      super
    end
  end
end

Application.initializer(ROOT_PATH)

# Support passenger smart spawning/fork mode:
if defined?(PhusionPassenger)
  PhusionPassenger.on_event(:starting_worker_process) do |forked|
    if forked
      # We're in smart spawning mode.
      Store.db.client.reconnect
    else
      # We're in conservative spawning mode. We don't need to do anything.
    end
  end
end