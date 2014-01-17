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
    Rho.NewORMModel.prototype.find = function(params){
        var _found = this.findObjects(params);
        switch (params[0]) {
            case 'all'  : return _wrapORMInstances(_found);
            case 'count': return _found ;
            case 'first':
            default     : return _wrapORMInstance(_found[0]);
        }
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
