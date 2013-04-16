describe("<ORM module specs>", function() {

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
        Rho.ORM.addModel('Model').make();
    });

    it('creates empty object', function() {
        Rho.ORM.addModel('Model').create();
    });

    describe("<model's object>", function() {
        var Model = undefined;
        var object = undefined;

        beforeEach(function(){
            Model = Rho.ORM.addModel('Model');
            object = Model.make({'key': 'value'});
        });

        it('gets property', function() {
            expect(object.get('key')).toBe('value');
        });

        it('sets property', function() {
            object.set('key', 'another value');
            expect(object.get('key')).toBe('another value');
        });

        it('sets new property', function() {
            object.set('new key', 'new value');
            expect(object.get('new key')).toBe('new value');
            expect(object.get('key')).toBe('value');
        });

        it('support set chaining', function() {
            object.set('key', 'another value').set('new key', 'new value');
            expect(object.get('key')).toBe('another value');
            expect(object.get('new key')).toBe('new value');
        });
    });

    it('creates and destroys objects in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var object = Model.create({'key': 'value'});
        expect(Model.find('all')).toEqual([]);
    }) ;
});
