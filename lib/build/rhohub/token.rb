class Rhohub::Token < Rhohub::Base

  def self.login(username, password)
    super({:username => username, :password => password},"token")
  end

end
