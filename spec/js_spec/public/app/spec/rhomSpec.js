describe("<rhom module specs>", function () {

    beforeEach(function(){
        Rho.Rhom.clear();
    });


    // Rho.Rhom.clear: function() {
    // Rho.Rhom.addModel: function(modelClass) {
    // Rho.Rhom.getModel: function(modelClass) {
    it('add model, get model, clear all models', function() {
        // relays on beforeEach
        expect(Rho.Rhom.getModel('Model')).toBeUndefined();

        var Model = Rho.Rhom.addModel('Model');
        expect(Model).toBeDefined();
        expect(Rho.Rhom.getModel('Model')).toBe(Model);

        Rho.Rhom.clear();
        expect(Rho.Rhom.getModel('Model')).toBeUndefined();
    });

    it('creates object, gets properties', function() {
        var Model = Rho.Rhom.addModel('Model');
        var object = Model.create({'key': 'value'});
        expect(object.get('key')).toBe('value');
    });
});
