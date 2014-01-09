(function ($, rho, rhoUtil) {
    'use strict';

    // === Rho.NewORM wrapper members ===
    Rho.NewORM.addModel = function(modelname, modeldef){
        var model = new Rho.NewORMModel(modelname);
        modeldef(model);
        model.initModel();
        return model;   
    };

    // === Rho.NewORMModel wrapper members ===

    Rho.NewORMModel.prototype.find = function(params){
        var _objects = this.findObjects(params);
        var _model_instances = [];
        for (var i = 0; i < _objects.length; i++) {
            var _object_instance = new _ORMObjectInstance(this, _objects[i]);
            _model_instances.pushObject(_object_instance);
        };
        return _model_instances;
    };

    Rho.NewORMModel.prototype.create = function(objHash){
        var _new_instance = new _ORMObjectInstance(this, this.createObject(objHash));
        return _new_instance;
    };

    // === Rho.NewORMModel Object's wrapper methods ===
    var _ORMObjectInstance = function(model, obj){
        for (var attrname in obj) { this[attrname] = obj[attrname]; }

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

})(Rho.jQuery, Rho, Rho.util);
