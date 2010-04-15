require 'rho'

product_schema = {
    #:columns =>['brand','created_at','name','price','quantity','sku','updated_at']
    :sql => "CREATE TABLE Product ( 
        brand varchar default NULL,
        created_at varchar default NULL,
        name varchar default NULL,
        price varchar default NULL,
        quantity int default NULL,
        sku varchar default NULL,
        updated_at varchar default NULL,
        object varchar(255) PRIMARY KEY )"
}

Rho::RhoConfig::add_source("Product", {"priority"=>0, "partition"=>'application',
  "schema" => nil }) #product_schema })