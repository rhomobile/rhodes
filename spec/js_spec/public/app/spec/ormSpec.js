describe("<ORM module specs>", function () {

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

    it('creates object, gets properties', function() {
        var Model = Rho.ORM.addModel('Model');
        var object = Model.create({'key': 'value'});
        expect(object.get('key')).toBe('value');
    });
});
