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
        console.log("MZV_DEBUG : _normalize_complex_conditions : " + JSON.stringify(conditions) + ", " + op);
        if(!conditions){
            return ['', []];
        };
        op = op || 'AND';
        if(Object.prototype.toString.call(conditions) === '[object Array]') {
            var quests = new Array();
            var sqlRes = '';
            for (var item in conditions) {
                if (sqlRes.length > 0) {
                    sqlRes += ' ' + op + ' ';
                };
                if(item.hasOwnProperty('conditions')) {
                    var subCond = this._normalize_complex_conditions(item.conditions, item.op);
                }
                else {
                    var subCond = this._normalize_hash_condition(item, 'AND');
                }
                sqlRes += '(' + subCond[0] + ')';
                quests = quests.concat(subCond[1]);
            }
            return [sqlRes, quests];
        }
        else {
            return this._normalize_hash_condition(conditions, op);
        };
    };

    Rho.NewORMModel.prototype._normalize_hash_condition = function(item, op){
        // Hash condition can be of two forms:
        // 1) {'key1':'value'1, 'key2':'value2' ... }, 'op':'OP1' => translates into : key1=value1 OP1 key2=value2
        // 2) or full form { 'op':'OP1', 'func':'FUNC1', 'attrib':'key1', 'value(s)':'value1' } => translates into FUNC1(key1) OP1 value1
        if(item.hasOwnProperty('attrib')) {
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
                case 'attrib':
                    itemName = item[propName];
                    break;
                case 'value':
                case 'values':
                    itemValues = item[propName];
                    break;
                };
            };
            if(itemValues instanceof String){
                itemValues = [itemValues];
            };

            subCond = this.buildComplexWhereCond(itemName, itemValues, itemOp, itemFunc);
            return [subCond[0], subCond.slice(1)];
        }
        else {
            console.log("MZV_DEBUG : _normalize_hash_conditions : " + JSON.stringify(item) + ", " + op);
            var sqlRes = "";
            var condQuests = new Array();
            for (var propName in item) {
                console.log("MZV_DEBUG : _breaking cond : " + propName + ", " + item[propName]);
                var subCond = this.buildComplexWhereCond(propName, [item[propName]], "=", '');
                console.log("MZV_DEBUG : _received back : " + JSON.stringify(subCond) + ", " + subCond[0] + ", " + JSON.stringify(subCond.slice(1)));
                if(sqlRes.length > 0) {
                    sqlRes += " " + op + " ";
                };
                sqlRes += subCond[0];
                condQuests = condQuests.concat(subCond.slice(1));
                console.log("MZV_DEBUG : _should be concat : " + sqlRes + ", " + JSON.stringify(condQuests));
            };
            console.log("MZV_DEBUG : _and in the end we have : " + sqlRes + ", " + JSON.stringify(condQuests));
            return [sqlRes, condQuests];
        };
    };


    Rho.NewORMModel.prototype._normalize_conditions = function(what, conditions, op){
        console.log("MZV_DEBUG : make_conditions : " + what + ", " + JSON.stringify(conditions) + ", " + op);
        if (!op) {
            var retV;
            if(!conditions) {
                retV = this.buildSimpleWhereCond(what, []);
                return [retV[0], retV.slice(1)];
            }
            else if(Object.prototype.toString.call(conditions) === '[object String]') {
                retV = this.buildSimpleWhereCond(what, [conditions]);
                return [retV[0], retV.slice(1)];
            }
            else if(Object.prototype.toString.call(conditions) === '[object Array]') {
                retV = this.buildSimpleWhereCond(what, conditions);
                return [retV[0], retV.slice(1)];
            } else {
                return this._normalize_complex_conditions(conditions, 'AND');
            };
        };
        return this._normalize_complex_conditions(conditions, op);
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
        if(Object.prototype.toString.call(order_dir) === '[object String]'){
            order_dir = [order_dir];
        };
        if(Object.prototype.toString.call(order_attr) === '[object String]'){
            order_attr = [order_attr];
        };
        normalized_vector_args.order = this.buildFindOrder(order_attr, order_dir);

        // 3) Normalize SELECT
        var select_arr = options.select || [];
        if(Object.prototype.toString.call(options.conditions) === '[object String]') {
            select_arr = [select_arr];
        };
        normalized_vector_args.select = select_arr;

        // 4) Build Where Conditions
        var condVal = this._normalize_conditions(what, options.conditions, options.op);
        console.log(" we have here : " + condVal[0] + ", " + JSON.stringify(condVal[1]));
        normalized_string_args.conditions = condVal[0] || "";
        normalized_vector_args.quests = condVal[1] || [];
    };

    Rho.NewORMModel.prototype.find = function(what, options){
        what = what || 'all';
        options = options || {};
        var _found;
        // first - check for Hash conditions PropertyBag
        if(this.fixed_schema) {
            var normalized_vector_args = {};
            var normalized_string_args = {};
            console.log("we have here fixed schema options as " + JSON.stringify(options));
            this._normalize_args_for_find(what, options, normalized_string_args, normalized_vector_args);
            console.log("we have here options before find as " + what + ", " + JSON.stringify(normalized_string_args) + ", " + JSON.stringify(normalized_vector_args));
            _found = this.findObjects(what, normalized_string_args,
                                    normalized_vector_args['quests'],
                                    normalized_vector_args['select'],
                                    normalized_vector_args['order']);
        } else { // property bag
            // only LIMIT is supported
            normalized_string_args = {};
            var limitArgs = this.buildFindLimits(what, options);
            for (var property in limitArgs) {
                normalized_string_args[property] = limitArgs[property];
            }
            // 3) Normalize SELECT
            var select_arr = options.select || [];
            if(Object.prototype.toString.call(options.conditions) === '[object String]') {
                select_arr = [select_arr];
            };
            normalized_string_args['op'] = options.op || 'AND';
            if(Object.prototype.toString.call(options.conditions) === '[object Object]'){
                _found = this.findObjectsPropertyBagByCondHash(what, options.conditions, normalized_string_args, select_arr);
            } else { // the only other supported case is simple string (WHERE sql) or array (WHERE sql + quests)
                options.conditions = options.conditions || [""];
                options.quests = [];
                if(Object.prototype.toString.call(options.conditions) === '[object Array]') {
                    options.quests = options.conditions.slice(1);
                    options.conditions = options.conditions[0];
                };
                console.log("we are here and " + what + ", " + JSON.stringify(options.conditions) + ", " + JSON.stringify(options.quests));
                _found = this.findObjectsPropertyBagByCondArray(what, options.conditions, options.quests, normalized_string_args, select_arr);
            };
        };

        console.log("we are here in find result and " + JSON.stringify(_found) + ", " + JSON.stringify(_found[0]));

        switch (what) {
            case 'all'  : return this._wrapORMInstances(_found);
            case 'count': return _found ;
            case 'first':
            default     : return this._wrapORMInstance(_found[0]);
        };
    };

    Rho.NewORMModel.prototype.findBySql = function(sqlQuery){
        var _found = this.find_by_sql(sqlQuery);
        return this._wrapORMInstances(_found);
    };

    Rho.NewORMModel.prototype.paginate = function(options){
        options['page'] = options['page'] || 0;
        options['per_page'] = options['per_page'] || 10;
        options['offset'] = options['page'] * options['per_page'];
        return this.find('all', options);
    };

    Rho.NewORMModel.prototype.delete_all = function(options){
        console.log("we are in before 2 delete_all");
        var conditions = options || {};
        options = {};
        options.conditions = conditions;

        if(this.fixed_schema) {
            console.log("we are in before 3 fixed_schema");
            var normalized_vector_args = {};
            var normalized_string_args = {};
            this._normalize_args_for_find("all", options, normalized_string_args, normalized_vector_args);
            console.log("we are in before 3.1 fixed_schema" + options + ", " + JSON.stringify(normalized_string_args) + JSON.stringify(normalized_vector_args));
            return this.deleteObjects(normalized_string_args,
                                      normalized_vector_args['quests']);
        } else { // property bag
            normalized_string_args = {};
            console.log("we are in before 2.1 propbag");
            var limitArgs = this.buildFindLimits("all", options);
            console.log("we are in before 2.2 propbag");
            for (var property in limitArgs) {
                normalized_string_args[property] = limitArgs[property];
            }
            normalized_string_args.op = options.op || 'AND';
            if(Object.prototype.toString.call(options.conditions) === '[object Object]'){
                console.log("we are in before 3 propbag " + options.conditions);
                return this.deleteObjectsPropertyBagByCondHash(options.conditions, normalized_string_args);
            } else { // the only other supported case is simple string (WHERE sql) or array (WHERE sql + quests)
                console.log("we are in before 4 propbag");
                conditions = options.conditions || [""];
                quests = [];
                if(Object.prototype.toString.call(options.conditions) === '[object Array]') {
                    quests = options.conditions.slice(1);
                    conditions = options.conditions[0];
                };
                console.log("we are here and " + JSON.stringify(conditions) + ", " + JSON.stringify(quests));
                return this.deleteObjectsPropertyBagByCondArray(conditions, quests, normalized_string_args);
            };
        };
    };
    Rho.NewORMModel.prototype.deleteAll = function(options){
        console.log("we are in before 1 deleteAll")
        return this.delete_all(options);
    };
    Rho.NewORMModel.prototype._wrapORMInstance = function(objInst){
        if(!objInst) {
            return undefined;
        }
        var _new_instance = new _ORMObjectInstance(this, objInst);
        return _new_instance;
    };
    Rho.NewORMModel.prototype._wrapORMInstances = function(_objects){
        var _model_instances = [];
        for (var i = 0; i < _objects.length; i++) {
            var _object_instance = new _ORMObjectInstance(this, _objects[i]);
            _model_instances.push(_object_instance);
        };
        return _model_instances;
    };
    Rho.NewORMModel.prototype.make = function(objHash){
        var _new_instance = new _ORMObjectInstance(this, this.createInstance(objHash));
        return _new_instance;
    };
    Rho.NewORMModel.prototype.count = function(){
        return this.getCount();
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
        Object.defineProperty(this, 'vars', {
            value: function () {
                var objAttrs = {};
                for (var attrname in this) { objAttrs[attrname] = this[attrname]; }
                return objAttrs;
            },
            enumerable: false
        });
        Object.defineProperty(this, 'get', {
            value: function (propname) {
                return this[propname];
            },
            enumerable: false
        });
        Object.defineProperty(this, 'has', {
            value: function (propname) {
                return this.hasOwnProperty(propname);
            },
            enumerable: false
        });
        Object.defineProperty(this, 'set', {
            value: function (propname, value) {
                this[propname] = value;
                return this;
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
                var updatedAttrs = this._klass_model().saveObject(this.object, this);
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

    Rho.NewORM.dbPartitions = {};
    Rho.NewORM.dbConnection = function (partition){
        //look for db connection, if not found create one
        var filepath = Rho.Application.databaseFilePath(partition);
        var partitions = Rho.NewORM.getDbPartitions();
        var db;
        if(partitions && partitions[partition]){
            db = partitions[partition];
        }
        else{
            db = new Rho.Database(filepath, partition);
            partitions[partition] = db;
        }
        return db;
    };
    Rho.NewORM.getDbPartitions = function() {
        return Rho.NewORM.dbPartitions;
    };
    Rho.NewORM.clear = function(){
        Rho.NewORMModel.clear();
    };

    // Global initialization
    console.log("Replacing Old Rho.ORM with new one!!!");
    Rho.ORM = Rho.NewORM;
    Rho.ORMHelper = Rho.NewORM;
    Rho.ORMModel = Rho.NewORMModel;
    Rho.ORM.dbConnection('local');
    Rho.ORM.dbConnection('user');
    Rho.ORM.dbConnection('app');


})(Rho.jQuery, Rho, Rho.util);

