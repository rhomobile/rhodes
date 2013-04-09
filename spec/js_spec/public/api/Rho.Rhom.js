
(function ($, rho, rhoUtil) {
    'use strict';

    var moduleNS = 'Rho.Rhom';


    // === Rhom class definition ===

    var makeModel = function(modelClass) {
        Opal.Rhom._scope.RhomObjectFactory.$init_object(modelClass);
        return Opal.Object._scope[modelClass];
    };

    var models = {};

    var Rhom = {
        addModel: function(modelClass) {
            models[modelClass] = makeModel();
            return models[modelClass];
        },
        getModel: function(modelClass) {
            return models[modelClass];
        }
    };

    // === Rhom instance members ===



    // === Rhom static members ===



    // === Rhom default instance support ===



    rhoUtil.namespace(moduleNS, Rhom);

})(jQuery, Rho, Rho.util);
