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

        describe("namespace() function", function () {

            it("is available", function () {
                expect(typeof Rho.util.namespace).toEqual('function');
            });

            it("able to extend namespaces", function () {
                Rho.util.namespace("Rho.Test", {test:123});
                Rho.util.namespace("Rho.Test.A", {a:1});
                Rho.util.namespace("Rho.Test.A", {aa:11});
                Rho.util.namespace("Rho.Test.A.AA", {aaa:111});
                Rho.util.namespace("Rho.Test.B", {b:2});
                Rho.util.namespace("Rho.Test.C", {c:3});

                Rho.util.namespace("Rho.Test.A", {a:'should not override if already defined'});

                expect(Rho.Test.test).toEqual(123);
                expect(Rho.Test.A.a).toEqual(1);
                expect(Rho.Test.A.aa).toEqual(11);
                expect(Rho.Test.A.AA.aaa).toEqual(111);
                expect(Rho.Test.B.b).toEqual(2);
                expect(Rho.Test.C.c).toEqual(3);
            });

        });

        describe("nextId() function", function () {

            it("is available", function () {
                expect(typeof Rho.util.nextId).toEqual('function');
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
                expect(typeof Rho.util.apiReqFor).toEqual('function');
            });

            it("provides module request function", function () {
                expect(typeof Rho.util.apiReqFor('SOME_MODULE')).toEqual('function');
            });

            it("can perform synchronous request", function () {
                var apiReq = Rho.util.apiReqFor('Rho:Barcode');
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
                var apiReq = Rho.util.apiReqFor('Rho:Barcode');

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
                var apiReq = Rho.util.apiReqFor('Rho:Barcode');

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

    describe("Barcode module", function () {

        it("is available", function () {
            expect(Rho.Barcode).toBeNonEmpty();
        });

        it("it have default() method", function () {
            expect(typeof Rho.Barcode['default']).toEqual('function');
        });

        it("can provide default instance via default() method", function () {
            var defInst = Rho.Barcode['default']();
            expect(defInst.constructor.name).toEqual('Barcode');
        });

        it("can enumerate instances", function () {
            var objs = Rho.Barcode.enumerate();

            expect(objs).toBeNonEmpty();
            expect(objs instanceof Array).toBeNonEmpty();
            expect(objs.length).toBeGreaterThan(0);

            for(var i=0; i<objs.length; i++) {
                expect(objs[i].constructor.name).toEqual('Barcode');
                expect(typeof objs[i].getId()).toEqual('string');
                for (var j = 0; j < objs.length; j++) {
                    // they all should have different ids
                    if (i == j) continue;
                    expect(objs[i].getId()).not.toEqual(objs[j].getId());
                }
            }
        });

        describe("as default instance", function () {

            var defInstance;

            beforeEach(function () {
                defInstance = Rho.Barcode['default']();
            });

            it("can get properties", function () {
                var props = defInstance.getProperties();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                defInstance.setProperties({ resolution: '800x600' }, function(props) {
                   expect(props).toBeNonEmpty();
                   expect(typeof props).toEqual('object');

                   expect(typeof props.resolution).toEqual('string');
                   expect(props.resolution).toEqual('800x600');
                });
            });

        });

        describe("as non-default instance", function () {

            var instance = null;

            beforeEach(function(){
                instance = Rho.Barcode.enumerate()[0];
            });

            it("can get properties", function () {
                var props = instance.getProperties();

                expect(props).toBeNonEmpty();
                expect(typeof props).toEqual('object');

                expect(typeof props.resolution).toEqual('string');
                expect(props.resolution).toEqual('1024x768');
            });

            it("can set properties", function () {
                instance.setProperties({ resolution: '800x600' }, function(props) {
                   expect(props).toBeNonEmpty();
                   expect(typeof props).toEqual('object');

                   expect(typeof props.resolution).toEqual('string');
                   expect(props.resolution).toEqual('800x600');
                });
            });

        });

    });

});
