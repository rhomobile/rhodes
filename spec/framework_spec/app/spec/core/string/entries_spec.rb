require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/to_a'

ruby_version_is ""..."1.9" do
  describe "String#entries" do
    it_behaves_like :string_to_a, :entries
  end
end
