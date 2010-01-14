require 'find'

describe "Find" do

  before do
    Dir.mkdir("findspec")
    Dir.mkdir("findspec/1")
    Dir.mkdir("findspec/1/2")
    Dir.mkdir("findspec/1/2/4")
    Dir.mkdir("findspec/2")
    Dir.mkdir("findspec/3")
    Dir.mkdir("findspec/3/4")
  end

  after do
    Dir.rmdir("findspec/3/4")
    Dir.rmdir("findspec/3")
    Dir.rmdir("findspec/2")
    Dir.rmdir("findspec/1/2/4")
    Dir.rmdir("findspec/1/2")
    Dir.rmdir("findspec/1")
    Dir.rmdir("findspec")

  end
  it "should find all the files" do
    count = 0
    Find.find("findspec") do |path|
      count += 1
    end
    count.should == 7
  end

  it "should prune" do
    count = 0
    Find.find("findspec") do |path|
      if path =~ /3/
        Find.prune
      end

      count += 1
    end
    count.should == 5

    count = 0
    Find.find("findspec") do |path|
      if path =~ /1/
        Find.prune
      end

      count += 1
    end
    count.should == 4

  end

end