
describe("<generator API specs>", function () {

    it("Test bool property", function () {
        Rho.GenPropBag.boolProp = false;
        expect(Rho.GenPropBag.boolProp).toEqual(false);
        Rho.GenPropBag.boolProp = true;
        expect(Rho.GenPropBag.boolProp).toEqual(true);
    });

    it("Test float default property", function () {
        Rho.GenPropBag.getDefault().floatProp = 0.1;
        expect(Rho.GenPropBag.getDefault().floatProp).toEqual(0.1);
        Rho.GenPropBag.getDefault().floatProp = 1.5;
        expect(Rho.GenPropBag.getDefault().floatProp).toEqual(1.5);
    });

    it("Test float property with ints", function () {
        Rho.GenPropBag.floatProp = 3.0;
        expect(Rho.GenPropBag.floatProp).toEqual(3.0);
    });

    it("Test int default property", function () {
        Rho.GenPropBag.intProp = 25;
        expect(Rho.GenPropBag.getDefault().intProp).toEqual(25);
        Rho.GenPropBag.getDefault().intProp = 15;
        expect(Rho.GenPropBag.getDefault().intProp).toEqual(15);
    });

    it("Test enumerate", function () {
        var objs = Rho.GenPropBag.enumerate();
        
        Rho.Log.info(objs.toString(), "test" );
        
        expect(objs[1].boolProp).toEqual(false);
        objs[1].boolProp = true;
        expect(objs[1].boolProp).toEqual(true);
    });

    xit("Test testMethod1", function () {
        var objs = Rho.GenPropBag.testMethod1();
        
        Rho.Log.info(objs.toString(), "test" );
       
    });

    it("Test getAllProperties", function () {
    
        var callbackFunction = function (data) 
        {
            Rho.Log.info(data.toString(), "test" );
        };

        Rho.GenPropBag.getDefault().getAllProperties(callbackFunction);
       
    });

    xit("Test getAllProperties witout callback", function () {
    
        var objs = Rho.GenPropBag.getDefault().getAllProperties();
        Rho.Log.info(data.toString(), "test" );
    });

});