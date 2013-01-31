describe("Rho common API", function () {

    var defaultId = 'DEFAULT_OBJECT_ID';

    beforeEach(function(){
        jasmine.Matchers.prototype.toBeNonEmpty = jasmine.Matchers.prototype.toBeTruthy;
    });

    it("is available under Rho namespace", function () {
        expect(Rho).toBeNonEmpty();
    });

    describe("utility module", function () {

        it("is available", function () {
            expect(Rho.util).toBeNonEmpty();
        });

        describe("nextId() function", function () {

            it("is available", function () {
                expect(Rho.util.nextId).toBeNonEmpty();
            });

            it("generates next ID value each time", function () {
                var ids = [];
                for (var i = 0; i < 3; i++) {
                    ids[i] = Rho.util.nextId();
                    expect(ids[i]).toBeNonEmpty();
                    for (var j = 0; j < ids.length; j++) {
                        if (i == j) continue;
                        expect(ids[i]).not.toEqual(ids[j]);
                    }
                }
            });

        });

        describe("apiReqFor() function", function () {

            it("is available", function () {
                expect(Rho.util.apiReqFor).toBeNonEmpty();
            });

            it("provides module request function", function () {
                expect(typeof Rho.util.apiReqFor('SOME_MODULE')).toEqual('function');
            });

            it("can perform synchronous request", function () {
                var apiReq = Rho.util.apiReqFor('Barcode1');
                var result = apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3],
                    method: 'testMethod'
                });
                expect(result).toBeNonEmpty();
                expect(typeof result).toEqual('string');
                expect(result).toEqual('abc123');
            });

            it("can perform asynchronous request with callback", function () {
                var apiReq = Rho.util.apiReqFor('Barcode1');

                var spy = jasmine.createSpy('callback');

                apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3, spy],
                    method: 'testMethod',
                    valueCallbackIndex: 4
                });

                waitsFor(function () {
                    return spy.wasCalled;
                }, 'Callback should be called', 1000);

                runs(function () {
                    expect(spy).toHaveBeenCalledWith('abc123');
                });
            });

            it("can perform asynchronous request with deferred", function () {
                var apiReq = Rho.util.apiReqFor('Barcode1');

                var dfr = apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3, function(){}],
                    method: 'testMethod',
                    valueCallbackIndex: 4
                });

                expect(dfr).toBeNonEmpty();
                expect(typeof dfr.done).toEqual('function');

                var spy = jasmine.createSpy('callback');

                dfr.done(spy);

                waitsFor(function () {
                    return spy.wasCalled;
                }, 'Callback should be called', 1000);

                runs(function () {
                    expect(spy).toHaveBeenCalledWith('abc123');
                });
            });

        });

    });

    describe("Barcode1 module", function () {

        it("is available", function () {
            expect(Rho.Barcode1).toBeNonEmpty();
        });

        it("is an instance of Barcode1 class", function () {
            expect(Rho.Barcode1.constructor.name).toEqual('Barcode1');
        });

        it("is a default instance", function () {
            expect(Rho.Barcode1.getId()).toEqual(Rho.util.defaultId());
        });


        describe("as default instance", function () {

            it("can enumerate instances", function () {
                var objs = Rho.Barcode1.enumerate();

                expect(objs).toBeNonEmpty();
                expect(objs instanceof Array).toBeNonEmpty();
                expect(objs.length).toBeGreaterThan(0);

                for(var i=0; i<objs.length; i++) {
                    expect(objs[i].constructor.name).toEqual('Barcode1');
                    expect(typeof objs[i].getId()).toEqual('string');
                    for (var j = 0; j < objs.length; j++) {
                        // they all should have different ids
                        if (i == j) continue;
                        expect(objs[i].getId()).not.toEqual(objs[j].getId());
                    }
                }
            });

            it("can get properties", function () {
                var props = Rho.Barcode1.getProps();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                var props = Rho.Barcode1.setProps({ resolution: '800x600' });

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('800x600');
            });

        });

        describe("as non-default instance", function () {

            var instance = null;
            beforeEach(function(){
                instance = Rho.Barcode1.enumerate()[0];
            });

            it("can enumerate instances", function () {
                var objs = instance.enumerate();

                expect(objs).toBeNonEmpty();
                expect(objs instanceof Array).toBeNonEmpty();
                expect(objs.length).toBeGreaterThan(0);

                for(var i=0; i<objs.length; i++) {
                    expect(objs[i].constructor.name).toEqual('Barcode1');
                    expect(typeof objs[i].getId()).toEqual('string');
                    for (var j = 0; j < objs.length; j++) {
                        // they all should have different ids
                        if (i == j) continue;
                        expect(objs[i].getId()).not.toEqual(objs[j].getId());
                    }
                }
            });

            it("can get properties", function () {
                var props = instance.getProps();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                var props = instance.setProps({ resolution: '800x600' });

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('800x600');
            });

        });

    });

});
