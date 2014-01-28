(function ($, rho, rhoUtil) {
    'use strict';

    // === Rho.NewORM wrapper members ===
    Rho.NewORM.addModel = function(modelname, modeldef){
        var model = new Rho.NewORMModel(modelname);
        modeldef(model);
        model.initModel();
        return model;   
    };
    Rho.NewORM.getModel = function(modelname){
        return Rho.NewORMModel.getModel(modelname);
    };

    // === Rho.NewORMModel wrapper members ===
    Rho.NewORMModel.prototype._normalize_complex_conditions = function(conditions, op){
        if(!conditions){
            return ['', []];
        };
        op = op || 'AND';
        if(conditions instanceof Array){
            var quests = [];
            var sqlRes = '';
            for (var item in conditions) {
                if (sqlRes.length > 0) {
                    sqlRes += ' ' + op + ' ';
                };
                if(item.hasOwnProperty('conditions')) {
                    var subCond = _normalize_complex_conditions(item.conditions, item.op);
                }
                else {
                    var subCond = _normalize_complex_condition(item);
                }
                sqlRes += '(' + subCond[0] + ')';
                quests = quests.concat(subCond[1]);
            }
            return [sqlRes, quests];
        }
        else {
            return _normalize_complex_condition(conditions);
        };
    };
    
    Rho.NewORMModel.prototype._normalize_complex_condition = function(item){
        var sqlRes = "";
        var condQuests = [];
        var itemOp = "=";
        var itemFunc = "";
        var itemName = "";
        var itemValues = "";
        for (var propName in item) {
            switch (propName) {
            case 'op'  : 
                itemOp = item[propName];
                break;
            case 'func': 
                itemFunc = item[propName];
                break;
            case 'name': 
                itemName = item[propName];
                break;
            case 'value':
            case 'values':
                itemValues = item[propName];
            default: 
                itemName = propName;
                itemValues = item[Values];
            };
        };
        if(itemValues instanceof String){
            itemValues = [itemValues];
        };

        subCond = buildComplexWhereCond(itemName, itemValues, itemOp, itemFunc);
        return [subCond[0], subCond.slice(1)];    
    }; 
        

    Rho.NewORMModel.prototype._normalize_conditions = function(what, conditions, op){
        console.log("MZV_DEBUG : make_conditions : " + what + ", " + JSON.stringify(conditions) + ", " + op);
        if (!op) {
            var retV = [];
            if(!conditions) {
                retV = this.buildSimpleWhereCond(what, []);
            }
            else if(conditions instanceof String){
                retV = this.buildSimpleWhereCond(what, [conditions]);
            }
            else if(conditions instanceof Array){
                retV = this.buildSimpleWhereCond(what, conditions);
            };
            return [retV[0], retV.slice(1)];
        };
        return _normalize_complex_conditions(conditions, op);
    };
          

    Rho.NewORMModel.prototype._normalize_args_for_find = function(what, options, normalized_string_args, normalized_vector_args){
        // 1) Normalize LIMITS
        var limitArgs = this.buildFindLimits(what, options);
        for (var property in limitArgs) {
            normalized_string_args[property] = limitArgs[property];
        }
        
        // 2) Normalize ORDER BY
        var order_dir = options.orderdir || [];
        var order_attr = options.order || [];
        if (order_dir instanceof String){
            order_dir = [order_dir];
        };
        if (order_attr instanceof String){
            order_attr = [order_attr];
        }; 
        normalized_vector_args.order = this.buildFindOrder(order_attr, order_dir);
        
        // 3) Normalize SELECT
        var select_arr = options.select || [];
        normalized_vector_args.select = select_arr;

        // 4) Build Where Conditions
        var condVal = _normalize_conditions(what, options.conditions, options.op);
        console.log(" we have here : " + condVal[0] + ", " + JSON.stringify(condVal[1]));
        normalized_string_args.conditions = condVal[0] || "";
        normalized_vector_args.quests = condVal[1] || [];
    };

    Rho.NewORMModel.prototype.find = function(what, options){
        var normalized_vector_args = {};
        var normalized_string_args = {};
        _normalized_args_for_find(what, options, normalized_string_args, normalized_vector_args);
        var _found = this.findObjects(what, normalized_string_args, 
                                        normalized_vector_args['quests'],
                                        normalized_vector_args['select'],
                                        normalized_vector_args['order']);
        switch (what) {
            case 'all'  : return _wrapORMInstances(_found);
            case 'count': return _found ;
            case 'first':
            default     : return _wrapORMInstance(_found[0]);
        };
    };

    Rho.NewORMModel.prototype.findBySql = function(sqlQuery){
        var _found = this.find_by_sql(sqlQuery);
        return _wrapORMInstances(_found);
    };

    Rho.NewORMModel.prototype.paginate = function(options){
        options['page'] = options['page'] || 0;
        options['per_page'] = options['per_page'] || 10;
        options['offset'] = options['page'] * options['per_page'];
        return find('all', options);
    };

    Rho.NewORMModel.prototype.delete_all = function(what, options){
        var normalized_vector_args = {};
        var normalized_string_args = {};
        _normalized_args_for_find(what, options, normalized_string_args, normalized_vector_args);
        return this.deleteObjects(what, normalized_string_args, 
                                        normalized_vector_args['quests']);
    };

    Rho.NewORMModel.prototype._wrapORMInstance = function(objInst){
        var _new_instance = new _ORMObjectInstance(this, objInst);
        return _new_instance;    
    };
    Rho.NewORMModel.prototype._wrapORMInstances = function(_objects){
        var _model_instances = [];
        for (var i = 0; i < _objects.length; i++) {
            var _object_instance = new _ORMObjectInstance(this, _objects[i]);
            _model_instances.pushObject(_object_instance);
        };
        return model_instances;   
    };
    Rho.NewORMModel.prototype.create = function(objHash){
        var _new_instance = new _ORMObjectInstance(this, this.createObject(objHash));
        return _new_instance;
    };

    // === Rho.NewORMModel Object's wrapper methods ===
    var _ORMObjectInstance = function(model, obj){
        for (var attrname in obj) { this[attrname] = obj[attrname]; }
        this.source_id = model.source_id

        Object.defineProperty(this, '_klass_model', {
            value: function () {
                return model;
            },
            enumerable: false
        });
        Object.defineProperty(this, 'updateAttributes', {
            value: function (newAttrs) {
                var updatedAttrs = this._klass_model().updateObject(this.object, this, newAttrs);
                for (var attrname in updatedAttrs) { this[attrname] = updatedAttrs[attrname]; }
                console.log("and after update we have : " + JSON.stringify(this));
                return this;
            },
            enumerable: false
        });
        Object.defineProperty(this, 'save', {
            value: function () {
                var updatedAttrs = this._klass_model().saveObject(this.object, objAttrs);
                for (var attrname in updatedAttrs) { this[attrname] = updatedAttrs[attrname]; }
                return this;
            },
            enumerable: false
        });
        Object.defineProperty(this, 'destroy', {
            value: function () {
                this._klass_model().deleteObject(this.object);
                return this;
            },
            enumerable: false
        });  
    };

    if(Rho.NewORM.useNewOrm()){
        console.log("Replacing Old Rho.ORM with new one!!!");
        Rho.ORM = Rho.NewORM;
        Rho.ORMModel = Rho.NewORMModel;
    };

})(Rho.jQuery, Rho, Rho.util);
