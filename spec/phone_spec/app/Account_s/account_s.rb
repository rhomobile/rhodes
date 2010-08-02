class Account_s
    include Rhom::FixedSchema

    set :schema_version, '1.0'
    
    property :billing_address_country, :string
    property :assigned_user_id, :string
    property :website, :string
    property :billing_address_street, :string
    property :assigned_user_name, :string
    property :created_by, :string
    property :campaign_id, :string
    property :billing_address_city, :string
    property :shipping_address_street, :string
    property :shipping_address_country, :string
    property :email, :string
    property :date_modified, :string
    property :sic_code, :string
    property :industry, :string
    property :parent_name, :string
    property :name, :string
    property :annual_revenue, :string
    property :ticker_symbol, :string
    property :phone_fax, :string
    property :shipping_address_city, :string
    property :ownership, :string
    property :phone_office, :string
    property :phone_alternate, :string
    property :billing_address_state, :string
    property :modified_user_id, :string
    property :description, :string
    property :modified_by_name, :string
    property :employees, :string
    property :shipping_address_postalcode, :string
    property :billing_address_postalcode, :string
    property :account_type, :string
    property :date_entered, :string
    property :rating, :string
    property :parent_id, :string
    property :created_by_name, :string
    property :shipping_address_state, :string
    
    property :type_acct, :string
    property :object_acct, :string
    property :new_name, :string
    property :last_checked, :string
    property :acct_object,  :string
    
     property :address,  :string
end