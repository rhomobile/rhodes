# XXX ENV.inspect crashes
#require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
#
#describe "ENV.inspect" do
#
#  it "returns a String that looks like a Hash with real data" do
#    ENV["foo"] = "bar"
#    ENV.inspect.should =~ /\{.*"foo"=>"bar".*\}/
#    ENV.delete "foo"
#  end
#
#end
