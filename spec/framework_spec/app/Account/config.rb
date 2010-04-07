require 'rho'

product_schema = {
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
}

Rho::RhoConfig::add_source("Account", {"source_id"=>23, "sync_type"=>"none", "priority"=>1
#,"schema" => product_schema 
})