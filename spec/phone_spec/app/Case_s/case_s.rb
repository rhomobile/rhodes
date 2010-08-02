class Case_s
    include Rhom::FixedSchema

    set :schema_version, '1.0'

    property :status, :string
    property :assigned_user_id, :string
    property :created_by_name, :string
    property :work_log, :string
    property :priority, :string
    property :case_number, :string
    property :account_id, :string
    property :type, :string
    property :modified_user_id, :string
    property :name, :string
    property :assigned_user_name, :string
    property :modified_by_name, :string
    property :description, :string
    property :date_modified, :string
    property :date_entered, :string
    property :resolution, :string
    property :created_by, :string
    property :account_name, :string
    
end