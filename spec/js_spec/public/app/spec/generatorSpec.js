
describe("<generator API specs>", function () {

<!--
    it("Test bool property", function () {
        expect(Rho.GenPropBag.boolProp).toEqual(false);
        Rho.GenPropBag.boolProp = true;
        expect(Rho.GenPropBag.boolProp).toEqual(true);
    });

    it("Test bool default property", function () {
        expect(Rho.GenPropBag.default().boolProp).toEqual(false);
        Rho.GenPropBag.default().boolProp = true;
        expect(Rho.GenPropBag.default().boolProp).toEqual(true);
    });
-->

    it("Test enumerate", function () {
        Rho.GenPropBag.enumerate();
    });
    
<!-- TODO: enumerate -->
});