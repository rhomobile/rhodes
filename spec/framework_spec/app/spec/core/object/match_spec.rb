require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'

describe Object, '=~' do
  it 'returns nil matching any object' do
    o = Object.new

    (o =~ /Object/).should == nil
    (o =~ 'Object').should == nil
    (o =~ Object).should == nil
    (o =~ Object.new).should == nil
    (o =~ nil).should == nil
    (o =~ true).should == nil
  end
end
