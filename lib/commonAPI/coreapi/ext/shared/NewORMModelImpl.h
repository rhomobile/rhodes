//
//  NewORMModelImpl.cpp
#include "generated/cpp/NewORMBase.h"
#include "generated/cpp/NewORMModelBase.h"
#include "common/RhoStd.h"
#include "common/AutoPointer.h"
#include "common/RhodesApp.h"
#include "common/RhoConf.h"
#include "common/RhoTime.h"
#include "logging/RhoLog.h"
#include "db/DBAdapter.h"
#include "sync/RhoconnectClientManager.h"

namespace rho {
    
    using namespace apiGenerator;
    using namespace common;
    
    class CNewORMModelImpl : public CNewORMModelBase
    {
    public:
        struct SchemaIndexDef
        {
            rho::String name_;
            Vector<rho::String> columns_;
            bool unique_;

            SchemaIndexDef()
                : name_(),
                  columns_(),
                  unique_(false)
                {}

            SchemaIndexDef(const rho::String& name,
                     const rho::Vector<rho::String>& cols,
                     const bool bUnique)
                : name_(name),
                  columns_(cols),
                  unique_(bUnique)
                {}
        };

        struct ModelPropertyDef
        {
            rho::String name_;
            rho::String type_;
            rho::String option_;

            ModelPropertyDef()
                : name_(),
                  type_(),
                  option_()
                {}

            ModelPropertyDef(const rho::String& name,
                     const rho::String& propType,
                     const rho::String& option)
                : name_(name),
                  type_(propType),
                  option_(option)
                {}
        };

        static HashtablePtr<rho::String, CNewORMModelImpl*>& models() { return models_; }
        static void init_once();
        static int get_start_id(const rho::String& partition);

        CNewORMModelImpl(const rho::String& strID);
        virtual ~CNewORMModelImpl() {}

        const rho::String& name() const { return name_; }
        const rho::String& id() const { return id_; }
        bool fixed_schema() {
            return fixed_schema_;
        }
        void setFixed_schema(const bool newVal, rho::apiGenerator::CMethodResult& oResult);
        void getFixed_schema(rho::apiGenerator::CMethodResult& oResult);
        
        bool freezed_model()
        {
            return freezed_;
        }
        void setFreezed(const bool newVal, rho::apiGenerator::CMethodResult& oResult);
        void getFreezed(rho::apiGenerator::CMethodResult& oResult);

        // API Model lifecycle methods
        void init(const rho::String& modelName, rho::apiGenerator::CMethodResult& oResult);
        void destroy(rho::apiGenerator::CMethodResult& oResult);

        // Model definition methods
        void set(const rho::String& propName, const rho::String& propValue, rho::apiGenerator::CMethodResult& oResult);
        void enable(const rho::String& propName, rho::apiGenerator::CMethodResult& oResult);
        void setBelongsTo(const rho::String& propName, const rho::String& sourceName, rho::apiGenerator::CMethodResult&);
        void getBelongsTo(const rho::String& propName, rho::apiGenerator::CMethodResult&);
        
        void setModelProperty(const rho::String& propName, 
                              const rho::String& propType, 
                              const rho::String& option,
                              rho::apiGenerator::CMethodResult&);
        void getModelProperty(const rho::String& propName, 
                              rho::apiGenerator::CMethodResult&);
        void setSchemaIndex(const rho::String& indexName, const rho::Vector<rho::String>& indexColumns, bool bUniqueIndex, rho::apiGenerator::CMethodResult& oResult);

        // Model initialization methods
        void initModel(rho::apiGenerator::CMethodResult& oResult);
        void initAssociations(rho::apiGenerator::CMethodResult& oResult);
        void initBlobAttributes(rho::apiGenerator::CMethodResult& oResult);
        void initSyncSourceProperties(rho::apiGenerator::CMethodResult& oResult);
        void initDbSource(rho::apiGenerator::CMethodResult& oResult);
        void initDbSchema(rho::apiGenerator::CMethodResult& oResult);

        // Object instance methods
        void validateFreezedAttributes(const rho::Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult);
        void validateFreezedAttribute(const rho::String& attrName, rho::apiGenerator::CMethodResult& oResult);
        void createInstance(const Hashtable<rho::String, rho::String>& attrs, rho::apiGenerator::CMethodResult& oResult);
        void createObject(const Hashtable<rho::String, rho::String>& objHash, rho::apiGenerator::CMethodResult& oResult);
        void updateObject(const rho::String& objId, 
                          const Hashtable<rho::String, rho::String>& oldAttrs,
                          const Hashtable<rho::String, rho::String>& newAttrs, 
                          rho::apiGenerator::CMethodResult& oResult);
        void saveObject(const rho::String& objId,
                        const Hashtable<rho::String, rho::String>& attrs,
                        rho::apiGenerator::CMethodResult& oResult);
        void deleteObject(const rho::String& objId, 
                          rho::apiGenerator::CMethodResult& oResult);
        void hasChanges(const rho::String& objId, rho::apiGenerator::CMethodResult& oResult);
        void canModify(const rho::String& objId, rho::apiGenerator::CMethodResult& oResult);

        // Model instance methods
        void anyChangedObjects(rho::apiGenerator::CMethodResult& oResult);
        void getCount(rho::apiGenerator::CMethodResult& oResult);
        void getBackendRefreshTime(rho::apiGenerator::CMethodResult& oResult);
        void findObjects(const rho::String& what, 
                         const Hashtable<rho::String, rho::String>& strOptions,
                         const rho::Vector<rho::String>& quests,
                         const rho::Vector<rho::String>& select_attrs,
                         const rho::Vector<rho::String>& order_attrs,
                         rho::apiGenerator::CMethodResult& oResult);
        void findObjectsPropertyBagByCondHash(const rho::String& what, 
                    const Hashtable<rho::String, rho::String>& conditions, 
                    const Hashtable<rho::String, rho::String>& strOptions,
                    const Vector<rho::String>& selectAttrs, 
                    rho::apiGenerator::CMethodResult& oResult);
        void findObjectsPropertyBagByCondArray(const rho::String& what, 
                    const rho::String& conditions,
                    const Vector<rho::String>& quests, 
                    const Hashtable<rho::String, rho::String>& strOptions,
                    const Vector<rho::String>& selectAttrs, 
                    rho::apiGenerator::CMethodResult& oResult);
        void find_by_sql(const rho::String& sqlQuery, rho::apiGenerator::CMethodResult& oResult);
        void deleteObjects(const Hashtable<rho::String, rho::String>& strOptions, 
                           const Vector<rho::String>& quests, 
                           rho::apiGenerator::CMethodResult& oResult);
        void deleteObjectsPropertyBagByCondHash(const Hashtable<rho::String, rho::String>& conditions,
                    const Hashtable<rho::String, rho::String>& strOptions, 
                    rho::apiGenerator::CMethodResult& oResult);
        void deleteObjectsPropertyBagByCondArray(const rho::String& conditions,
                    const Vector<rho::String>& quests, 
                    const Hashtable<rho::String, rho::String>& strOptions, 
                    rho::apiGenerator::CMethodResult& oResult);

        // Aux methods
        rho::String _make_select_attrs_str(const rho::Vector<rho::String>& select_attrs,
                                           rho::Hashtable<rho::String, rho::String>& attrsSet);
        rho::String _make_order_str(const Vector<rho::String>& order_attrs);
        rho::String _make_limit_str(const Hashtable<rho::String, rho::String>& options);

        void buildFindLimits(const rho::String& whatArg, 
                             const Hashtable<rho::String, rho::String>& options,
                             rho::apiGenerator::CMethodResult& oResult);
        void buildFindOrder(const Vector<rho::String>& orderAttrs,
                            const Vector<rho::String>& orderOps,
                            rho::apiGenerator::CMethodResult& oResult);
        void buildFindOrderString(const Vector<rho::String>& orderAttrs,
                    const Vector<rho::String>& orderOps,
                    rho::apiGenerator::CMethodResult& oResult);
        void buildSimpleWhereCond(const rho::String& what,
                                  const rho::Vector<rho::String>& conditions,
                                  rho::apiGenerator::CMethodResult& oResult);
        void buildComplexWhereCond(const rho::String& key, 
                                   const Vector<rho::String>& values, 
                                   const rho::String& val_op, 
                                   const rho::String& val_func,
                                   rho::apiGenerator::CMethodResult& oResult);
        rho::String _make_cond_where_ex(const rho::String& key, 
                                        const Vector<rho::String>& values, 
                                        const rho::String& val_op, 
                                        const rho::String& val_func,
                                        Vector<rho::String>& quests);
        void findObjectsFixedSchema(const rho::String& what, 
                         const Hashtable<rho::String, rho::String>& strOptions,
                         const rho::Vector<rho::String>& quests,
                         const rho::Vector<rho::String>& select_attrs,
                         const rho::Vector<rho::String>& order_attrs,
                         rho::apiGenerator::CMethodResult& oResult);
        void _deleteObject(db::CDBAdapter& db,
                           const bool is_sync_source, 
                           const rho::String& source_id,
                           const rho::String& objId,
                           const Hashtable<rho::String, rho::String>& attrs,
                           rho::apiGenerator::CMethodResult& oResult);
        db::CDBAdapter& _get_db(rho::apiGenerator::CMethodResult& oResult);

        void onSyncDeleteError(const rho::String& objId,
                               const Hashtable<rho::String, rho::String>& attrHash, 
                               const rho::String& action,
                               rho::apiGenerator::CMethodResult& oResult);
        void onSyncUpdateError(const rho::String& objId,
                               const Hashtable<rho::String, rho::String>& attrHash, 
                               const Hashtable<rho::String, rho::String>& rollbackHash,
                               const rho::String& action,
                               rho::apiGenerator::CMethodResult& oResult);
        void onSyncCreateError(const Vector<rho::String>& objects,
                               const rho::String& action,
                               rho::apiGenerator::CMethodResult& oResult);
        void pushChanges(rho::apiGenerator::CMethodResult& oResult);

    private:
        void init_defaults();

        rho::String _make_insert_attrs_sql_script(const rho::String& objectId,
                                                        const Hashtable<rho::String, rho::String>& attrs, 
                                                        Vector<rho::String>& quests) const;
        rho::String _make_create_sql_script() const;
        rho::Vector<rho::String> _create_sql_schema_indices() const;
        rho::String _make_insert_or_update_attr_sql_script(const rho::String srcId, 
                                                                const rho::String& objId, 
                                                                const rho::String& attrKey,
                                                                const rho::String& attrValue,
                                                                Vector<rho::String>& quests);
        bool _get_object_attrs(const rho::String& objId, 
                                Hashtable<rho::String, rho::String>& attrs,
                                const Hashtable<rho::String, rho::String>& selectAttrs, 
                                rho::apiGenerator::CMethodResult& oResult);

        void _processDbResult(IDBResult& res, 
                              const rho::String& what,
                              const Hashtable<rho::String, rho::String>& attrSet,
                              const bool tableResults, 
                              rho::apiGenerator::CMethodResult& oResult);

        static int _get_partition_start_id(const rho::String& partition);
        static rho::String _get_db_type(const rho::String& propType);
        static bool _is_reserved_name(const rho::String& attrName);
        static rho::String _strip_braces(const rho::String& str);
        static bool _is_non_str_func_where(const rho::String& val_func);
        rho::Vector<rho::String> _get_attribs_of_PropertyBagModel(rho::apiGenerator::CMethodResult& oResult);
        
        static HashtablePtr<rho::String, CNewORMModelImpl*> models_;
        static Hashtable<rho::String, int> reserved_names_;

        rho::String id_;
        rho::String name_;
        bool fixed_schema_;
        bool freezed_;
        Hashtable<rho::String, ModelPropertyDef> modelProperties_;
        Hashtable<rho::String, SchemaIndexDef > schemaIndices_;
        Hashtable<rho::String, rho::Vector<rho::String> > belongsTo_;
    };

    class CNewORMModelSingletonImpl: public CNewORMModelSingletonBase
    {
    public:
        CNewORMModelSingletonImpl(): CNewORMModelSingletonBase()
        {
            CNewORMModelImpl::init_once();
        }
        
        //methods
        // enumerate  
        virtual void enumerate(rho::apiGenerator::CMethodResult& oResult) {
            Vector<rho::String> ret_models;
            HashtablePtr<rho::String, CNewORMModelImpl*>& models = CNewORMModelImpl::models();
            for(HashtablePtr<rho::String, CNewORMModelImpl*>::iterator it = models.begin(); it != models.end(); ++it) {
                ret_models.push_back((*it).second -> id());
            }

            oResult.set(ret_models);
        }

        virtual void getModel(const rho::String& source_name, rho::apiGenerator::CMethodResult& oResult) {
            //rho::String ret_model("");
            HashtablePtr<rho::String, CNewORMModelImpl*>& models = CNewORMModelImpl::models();
            HashtablePtr<rho::String, CNewORMModelImpl*>::iterator it = models.find(source_name);
            if(it != models.end())
                oResult.set((*it).second -> id());
            else {
                oResult = rho::apiGenerator::CMethodResult();
            }
        }

        virtual void clear(rho::apiGenerator::CMethodResult&) 
        {  
            HashtablePtr<rho::String, CNewORMModelImpl*>& models = CNewORMModelImpl::models();
            for(HashtablePtr<rho::String, CNewORMModelImpl*>::iterator it = models.begin(); it != models.end(); ) {
                models.erase(it++);
            }
        }
    };
    
    ////////////////////////////////////////////////////////////////////////
    
    class CNewORMModelFactory: public CNewORMModelFactoryBase    {
    public:
        CNewORMModelFactory()
        {}
        
        INewORMModelSingleton* createModuleSingleton()
        { 
            return new CNewORMModelSingletonImpl();
        }
        
        virtual INewORMModel* createModuleByID(const rho::String& strID){ return new CNewORMModelImpl(strID); };    
    };
    
}