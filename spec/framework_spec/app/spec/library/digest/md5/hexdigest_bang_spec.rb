require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../../spec_helper'
require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/shared/constants'

describe "Digest::MD5#hexdigest!" do

  it 'returns a hexdigest and resets the state' do
    cur_digest = Digest::MD5.new

    cur_digest << MD5Constants::Contents
    cur_digest.hexdigest!.should == MD5Constants::Hexdigest
    cur_digest.hexdigest.should == MD5Constants::BlankHexdigest
  end

end
