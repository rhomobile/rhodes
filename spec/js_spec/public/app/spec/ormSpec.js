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
        expect(cleanVars(Rho.ORM.addModel('Model').make())).toEqual({});
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
    });

    it('creates object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.find('all').length;
        Model.create({'key': 'value'});
        var after = Model.find('all').length;
        expect(after).toBe(before + 1);
    });

    it('does not create empty object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.find('all').length;
        Model.create();
        expect(Model.find('all').length).toBe(before);
    });

    it('does not create object with the only empty property in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.find('all').length;
        Model.create({'': 'value'});
        expect(Model.find('all').length).toBe(before);
    });

    it('deletes all objects in database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.create({'key': 'value'});
        expect(Model.find('all').length).toBeGreaterThan(0);
        Model.deleteAll();
        expect(Model.find('all').length).toBe(0);
    });

    it('deletes all objects of specific model in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        expect(Model1.find('all').length).toBeGreaterThan(0);
        expect(Model2.find('all').length).toBeGreaterThan(0);

        var before1 = Model1.find('all').length;
        Model2.deleteAll();
        var after1 = Model1.find('all').length;

        expect(after1).toBe(before1);
        expect(Model2.find('all').length).toBe(0);
    });
});
