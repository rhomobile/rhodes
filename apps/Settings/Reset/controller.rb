require 'rho/rhocontroller'

class ResetController < Rho::RhoController

  #GET /Reset
  def index
    render
  end
  
  #GET /Reset/do_reset
  def do_reset
    SyncEngine::trigger_sync_db_reset
    @msg = "Database will be deleted on next sync."
    redirect Rho::RhoConfig.start_path
  end
end
