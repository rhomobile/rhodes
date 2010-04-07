require 'rho'

product_schema = {
    :columns =>['brand','created_at','name','price','quantity','sku','updated_at']
}

Rho::RhoConfig::add_source("Product", {"source_id"=>2, "url"=>"Product", "priority"=>0, "partition"=>'application'
#    ,"schema" => product_schema 
})