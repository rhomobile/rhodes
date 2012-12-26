(function($) {

    function publicInterface() {
        return {
            SyncObject: SyncObject,
            Model: Model
        };
    }

    var rho = RhoSync.rho;

    function SyncObject(defn) {
        this.id = null;
        this.fields = defn.fields;
        this.values = {};

        var isNew = true;
        this.__defineGetter__('isNew', function() {
            return isNew;
        });

        var isChanged = false;
        this.__defineGetter__('isChanged', function() {
            return isChanged;
        });

        var _setValue =  function(name, v) {
            this.values[name] = v;
            isChanged = true;
            // db write goes here
        };

        this.addField = function(name, type) {
            this.__defineGetter__(name, function() {
                return this.values[name];
            });
            this.__defineSetter__(name, function(v) {
                _setValue(name, v);
            });
        };

        this.deleteField = function(name) {
            this.__defineGetter__(name, undefined);
            this.__defineSetter__(name, undefined);
            delete this.fields[name];
            delete this.values[name];
        };

        this.clearNotifications = function () {};
        this.destroy = function() {};

        this.updateAttributes = function(attribs) {

            this.save();
        };

        this.save = function() {
            // do save in db
            isNew = false;
            isChanged = false;
        };

    }

    function Model(defn) {

        this.source = new rho.engine.Source(defn.sourceId, defn.name, 'incremental', rho.storage, rho.engine, this);

        this.__defineGetter__('name', function() {
            return this.source.name;
        });
        this.__defineSetter__('name', function(v) {
            this.source.name = v;
        });

        this.belongsTo = defn.belongsTo;

        // Rhom API methods
        this.deleteAll = function(conditions) {};
        this.find = function(args) {};
        this.findAll = function(args) {};
        this.findBySql = function(query) {};

        this.newObject = function(attribs) {
            return new SyncObject(attribs);
        };

        this.createObject = function(attribs) {
            var obj = this.newObject(attribs);
            obj.save();
            return obj;
        };

        this.paginate = function(args) {};
        this.sync = function(callback, cbData, showStatusPopup) {};
        this.setNotification = function(url, params) {};
        this.save = function() {};
        this.canModify = function() {};
    }

    $.extend(rho, {domain: publicInterface()});

})(jQuery);
