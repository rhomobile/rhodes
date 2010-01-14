require 'bsearch'

describe "Bsearch" do

  it "should find first occurance" do
    index = %w(a b c c c d e f).bsearch_first {|x| x <=> "c"}
    index.should == 2

    index = %w(a b c e f).bsearch_first {|x| x <=> "c"}
    index.should == 2

  end

  it "should not find matches" do
    index = %w(a b e f).bsearch_first {|x| x <=> "c"}
    index.should == nil

    index = %w(a b e f).bsearch_last {|x| x <=> "c"}
    index.should == nil

  end

  it "should find upper and lower boundaries" do
    index = %w(a b e f).bsearch_lower_boundary {|x| x <=> "c"}
    index.should == 2

    index = %w(a b e f).bsearch_upper_boundary {|x| x <=> "c"}
    index.should == 2
  end

  it "should find ranges" do
    range = %w(a b c c c d e f).bsearch_range {|x| x <=> "c"}
    range.should == (2...5)

    range = %w(a b c d e f).bsearch_range {|x| x <=> "c"}
    range.should == (2...3)

    range = %w(a b d e f).bsearch_range {|x| x <=> "c"}
    range.should == (2...2)
    
  end

end