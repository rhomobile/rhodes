require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/fixtures/classes'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/values_at_spec'

ruby_version_is ""..."1.9" do
  describe "Hash#indices" do
    it_behaves_like(:hash_values_at, :indices)
  end
end
