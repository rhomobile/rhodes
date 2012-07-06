#require File.expand_path('../../../../spec_helper', __FILE__)
#require File.expand_path('../../shared/constants', __FILE__)
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../shared/constants'

require 'openssl'

describe "OpenSSL::Config#freeze" do
  it "needs to be reviewed for completeness"

  ruby_bug "redmine:484", "1.8.7" do
    platform_is_not :windows do
      it "freezes" do
        c = OpenSSL::Config.new
        lambda{c['foo'] = [ ['key', 'value'] ]}.should_not raise_error
        c.freeze
        c.frozen?.should be_true
        lambda{c['foo'] = [ ['key', 'value'] ]}.should raise_error
      end
    end
  end
end
