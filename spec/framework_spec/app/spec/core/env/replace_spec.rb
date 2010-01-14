# XXX Crashes
#require File.dirname(File.join(__rhoGetCurrentDir(), __FILE__)) + '/../../spec_helper'
#
#describe "ENV.replace" do
#
#  it "replaces ENV with a Hash" do
#    ENV["foo"] = "bar"
#    e = ENV.reject { |k, v| k == "foo" }
#    e["baz"] = "bam"
#    ENV.replace e
#    ENV["foo"].should == nil
#    ENV["baz"].should == "bam"
#    ENV.delete "baz"
#  end
#
#end
