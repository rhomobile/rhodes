describe("<rhoapi module specs>", function () {

    beforeEach(function(){
        jasmine.Matchers.prototype.toBeNonEmpty = jasmine.Matchers.prototype.toBeTruthy;
    });

    it("is available", function () {
        expect(Rho.util).toBeNonEmpty();
    });

    describe("namespace() function", function () {

        it("is available", function () {
            expect(typeof Rho.util.namespace).toEqual('function');
        });

        it("able to extend namespaces", function () {
            Rho.util.namespace("Rho.Test", {test: 123});
            Rho.util.namespace("Rho.Test.A", {a: 1});
            Rho.util.namespace("Rho.Test.A", {aa: 11});
            Rho.util.namespace("Rho.Test.A.AA", {aaa: 111});
            Rho.util.namespace("Rho.Test.B", {b: 2});
            Rho.util.namespace("Rho.Test.C", {c: 3});

            Rho.util.namespace("Rho.Test.A", {a: 'should not override if already defined'});

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
            var apiReq = Rho.util.apiReqFor('Rho:System');
            var result = apiReq({
                instanceId: '0',
                args: [],
                method: 'deviceName'
            });
            expect(result).toBeNonEmpty();
            expect(typeof result).toEqual('string');
            expect(result.length).toBeGreaterThan(0);
            jasmine.log('platform is: ' + result);
        });

        // TODO: fix asynchronous requests
        if (false) {

            it("can perform asynchronous request with callback", function () {
                var apiReq = Rho.util.apiReqFor('Rho:System');

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
                var apiReq = Rho.util.apiReqFor('Rho:System');

                var dfr = apiReq({
                    instanceId: '12345',
                    args: ['abc', 1, 2, 3, function () {
                    }],
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

        }

    });

});
