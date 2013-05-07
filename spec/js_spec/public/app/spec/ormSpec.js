describe("<ORM module specs>", function() {

    var cleanVars = function(object) {
        var vars = object.vars();
        var cleanVars = {};
        for (var key in vars) {
            if (key !== 'object' && key !== 'source_id') {
                cleanVars[key] = vars[key];
            }
        }
        return cleanVars;
    };

    beforeEach(function(){
        Rho.ORM.clear();
    });


    // Rho.ORM.clear: function() {
    // Rho.ORM.addModel: function(modelClass) {
    // Rho.ORM.getModel: function(modelClass) {
    it('add model, get model, clear all models', function() {
        // relays on beforeEach
        expect(Rho.ORM.getModel('Model')).toBeUndefined();

        var Model = Rho.ORM.addModel('Model');
        expect(Model).toBeDefined();
        expect(Rho.ORM.getModel('Model')).toBe(Model);

        Rho.ORM.clear();
        expect(Rho.ORM.getModel('Model')).toBeUndefined();
    });

    it('makes empty object', function() {
        var empty = Rho.ORM.addModel('Model').make();
        expect(cleanVars(empty)).toEqual({});
        var keySet = {};
        for (var key in empty.vars()) {
            keySet[key] = 0;
        }
        expect(keySet).toEqual({'object': 0, 'source_id': 0});
    });

    it('makes object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').make({'key': 'value'}))).toEqual({'key': 'value'});
    });

    it('creates empty object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').create())).toEqual({});
    });

    it('creates object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').create({'key': 'value'}))).toEqual({'key': 'value'});
    });

    describe("<model's object>", function() {
        var Model = undefined;
        var object = undefined;

        beforeEach(function(){
            Model = Rho.ORM.addModel('Model');
            object = Model.make({'key': 'value'});
        });

        it('returns vars', function() {
            expect(cleanVars(object)).toEqual({'key': 'value'});
        });

        it('retrieves object id', function() {
            expect(object.object()).toBe(object.get('object'));
        });

        it('gets existing property', function() {
            expect(object.get('key')).toBe('value');
        });

        it('gets absent property', function() {
            expect(object.get('absent key')).toBeUndefined();
        });

        it('sets property', function() {
            object.set('key', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'another value'});
        });

        it('sets new property', function() {
            object.set('new key', 'new value');
            expect(cleanVars(object)).toEqual({'key': 'value', 'new key': 'new value'});
        });

        it('supports set chaining', function() {
            object.set('key', 'another value').set('new key', 'new value');
            expect(cleanVars(object)).toEqual({'key': 'another value', 'new key': 'new value'});
        });

        it('sets property', function() {
            object.set('key', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'another value'});
        });

        it('sets property with empty name', function() {
            object.set('', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'value', '': 'another value'});
        });

        it('has properties', function() {
            expect(object.has('key')).toBe(true);
            expect(object.has('absent key')).toBe(false);
        });
    });

    it('creates object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create({'key': 'value'});
        var after = Model.count();
        expect(after).toBe(before + 1);
    });

    it('saves object to database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();

        var object = Model.create({'key': 'value'});

        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        object.set('key', 'another value').set('new key', 'new value');
        expect(Model.find(object.object()).vars()).not.toEqual(object.vars());
        object.save();
        expect(Model.find(object.object()).vars()).toEqual(object.vars());
    });

    it('updates object attributes in database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();

        var object = Model.create({'key': 'value', 'original key': 'original value'});

        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        object.updateAttributes({'key': 'another value', 'new key': 'new value'});
        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        expect(cleanVars(object)).toEqual({'key': 'another value', 'new key': 'new value', 'original key': 'original value'});
    });

    it('destroys object in database', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        var object1 = Model.create({'key1': 'value1'});
        var object2 = Model.create({'key2': 'value2'});

        expect(Model.count()).toBe(2);

        object1.destroy();

        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(found[0].vars()).toEqual(object2.vars());
    });

    it('does not create empty object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create();
        expect(Model.count()).toBe(before);
    });

    it('does not create object with the only empty property in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create({'': 'value'});
        expect(Model.count()).toBe(before);
    });

    it('deletes all objects in database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.create({'key': 'value'});
        expect(Model.count()).toBeGreaterThan(0);
        Model.deleteAll();
        expect(Model.count()).toBe(0);
    });

    it('deletes all objects of specific model in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        expect(Model1.count()).toBeGreaterThan(0);
        expect(Model2.count()).toBeGreaterThan(0);

        var before1 = Model1.count();
        Model2.deleteAll();
        var after1 = Model1.count();

        expect(after1).toBe(before1);
        expect(Model2.count()).toBe(0);
    });

    it('reads object from database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();
        Model.create({'key': 'value'});
        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(cleanVars(found[0])).toEqual({'key': 'value'});
    });

    it('does not write empty property to database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();
        Model.create({'key': 'value', '': 'empty'});
        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(cleanVars(found[0])).toEqual({'key': 'value'});
    });

    it('counts objects in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        var before1 = Model1.count();

        Model1.create({'key': 'value'});
        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        var after1 = Model1.count();

        expect(after1).toBe(before1 + 2);
    });

    it('finds all objects in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        Model1.deleteAll();

        Model1.create({'key1': 'value1'});
        Model2.create({'key2': 'value2'});
        Model1.create({'key3': 'value3'});

        var found = Model1.find('all');

        expect(found.length).toBe(2);
        var i = (found[0].has('key1')) ? 0 : 1;
        expect(cleanVars(found[i    ])).toEqual({'key1': 'value1'});
        expect(cleanVars(found[1 - i])).toEqual({'key3': 'value3'});
    });

    it('finds specific object', function() {
        var Model = Rho.ORM.addModel('Model');

        var original = Model.create({'key1': 'value1'});
        Model.create({'key2': 'value2'});

        expect(Model.find(original.object()).vars()).toEqual(original.vars());
    });

    it('finds first object in database', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        var originals = [Model.create({'key1': 'value1'}), Model.create({'key3': 'value3'})];

        var found = Model.find('first');

        expect(found.vars()).toEqual(originals[(found.has('key1')) ? 0 : 1].vars());
    });

});
