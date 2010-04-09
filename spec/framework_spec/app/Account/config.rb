require 'rho'

product_schema = {
=begin
    :columns =>
[    
'billing_address_country',
'assigned_user_id',
'website',
'billing_address_street',
'assigned_user_name',
'created_by',
'campaign_id',
'billing_address_city',
'shipping_address_street',
'email1',
'date_modified',
'sic_code',
'industry',
'parent_name',
'name',
'annual_revenue',
'ticker_symbol',
'phone_fax',
'shipping_address_city',
'ownership',
'phone_office',
'phone_alternate',
'billing_address_state',
'modified_user_id',
'description',
'modified_by_name',
'employees',
'shipping_address_postalcode',
'billing_address_postalcode',
'account_type',
'date_entered',
'rating',
'parent_id',
'created_by_name',
'shipping_address_state',
'type_acct',
'object_acct',
'acct_object',
'new_name',
'image_uri',
'address'
]
=end
:sql =>
"CREATE TABLE Account ( 
'billing_address_country' varchar default NULL,'assigned_user_id' varchar default NULL,'website' varchar default NULL,'billing_address_street' varchar default NULL,'assigned_user_name' varchar default NULL,
'created_by' int default NULL,'campaign_id' varchar default NULL,'billing_address_city' varchar default NULL,'shipping_address_street' varchar default NULL,'email1' varchar default NULL,'date_modified' varchar default NULL,'sic_code' varchar default NULL,'industry' varchar default NULL,'parent_name' varchar default NULL,'name' varchar default NULL,'annual_revenue' varchar default NULL,'ticker_symbol' varchar default NULL,'phone_fax' varchar default NULL,'shipping_address_city' varchar default NULL,'ownership' varchar default NULL,'phone_office' varchar default NULL,'phone_alternate' varchar default NULL,'billing_address_state' varchar default NULL,'modified_user_id' varchar default NULL,'description' varchar default NULL,'modified_by_name' varchar default NULL,'employees' varchar default NULL,'shipping_address_postalcode' varchar default NULL,'billing_address_postalcode' varchar default NULL,'account_type' varchar default NULL,'date_entered' varchar default NULL,'rating' varchar default NULL,'parent_id' varchar default NULL,'created_by_name' varchar default NULL,'shipping_address_state' varchar default NULL,'type_acct' varchar default NULL,'object_acct' varchar default NULL,'acct_object' varchar default NULL,'new_name' varchar default NULL,'image_uri' varchar default NULL,'address' varchar default NULL,
object varchar(255) PRIMARY KEY )"

}

Rho::RhoConfig::add_source("Account", {"source_id"=>23, "sync_type"=>"none", "priority"=>1,
 "schema" => nil })