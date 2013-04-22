
describe("<generator API specs>", function () {

/*
    it("Test bool property", function () {
        expect(Rho.GenPropBag.boolProp).toEqual(false);
        Rho.GenPropBag.boolProp = true;
        expect(Rho.GenPropBag.boolProp).toEqual(true);
    });
*/
    it("Test bool default property", function () {
        expect(Rho.GenPropBag.getDefault().boolProp).toEqual(false);
        Rho.GenPropBag.getDefault().boolProp = true;
        expect(Rho.GenPropBag.getDefault().boolProp).toEqual(true);
    });

    it("Test int default property", function () {
        expect(Rho.GenPropBag.getDefault().intProp).toEqual(0);
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

});