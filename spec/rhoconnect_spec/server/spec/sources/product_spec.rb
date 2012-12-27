require File.join(File.dirname(__FILE__),'..','spec_helper')

describe "Product" do
  it_should_behave_like "SpecHelper" do
  
    before(:each) do
      setup_test_for Product,'testuser'
      @product = {
        'name' => 'iPhone',
        'brand' => 'Apple',
        'price' => '$299.99',
        'quantity' => '5',
        'sku' => '1234'
      }
    end
  
    it "should process Product query" do
      test_create(@product)
      test_query.size.should > 0
      query_errors.should == {}
    end
  
    it "should process Product create" do
      new_product_id = test_create(@product)
      new_product_id.should_not be_nil
      create_errors.should == {}
      md[new_product_id].should == @product
    end
  
    it "should process Product update" do
      product_id = test_create(@product)
      md.should == {product_id => @product}
      test_update({product_id => {'price' => '$199.99'}})
      update_errors.should == {}
      test_query
      md[product_id]['price'].should == '$199.99'
    end
  
    it "should process Product delete" do
      product_id = test_create(@product)
      md.should == {product_id => @product}
      test_delete(product_id => @product)
      delete_errors.should == {}
      md.should == {}
    end
  end
end