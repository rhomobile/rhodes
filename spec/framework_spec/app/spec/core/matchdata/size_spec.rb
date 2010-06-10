require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/length'

describe "MatchData#size" do
  it_behaves_like(:matchdata_length, :size)
end
