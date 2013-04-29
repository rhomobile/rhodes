describe("<system module specs>", function () {

    if (isApplePlatform()) {
        var alptabet = [ 
            'alpha', 'beta', 'gamma', 'delta',
            'epsilon', 'zeta', 'eta', 'theta',
            'iota', 'kappa', 'lambda', 'mu', 
            'nu', 'xi', 'omicron', 'pi', 
            'rho', 'sigma', 'tau', 'upsilon', 
            'phi', 'chi', 'psi', 'omega'
        ]; 
        var nicks = [
            'Horlitted','Opinkh1992','Maidest1930','Hicamen',
            'Waidelve','Tremen','Ressay','Beturped',
            'Haveretwound91','Theaver','Thamou1930','Schistermin1928',
            'Mong1936','Kinters','Firmervis','Affeard',
            'Tookins','Thrunt','Thaps1958','Offs1967',
            'Yount1939','Careason','Undidesix34','Uness1981'
        ];

        it("testString callback", function () {
            var teststring = 'amdin';
            var reversestring = teststring.split('').reverse().join('');

            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testString(reversestring,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(teststring)
            })
        });

        it("testBool callback", function () {
            var testValue = true;

            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testBool(testValue,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(!testValue)
            })
        });

        it("testInt callback", function () {
            var testValue = 42;
            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testInt(testValue,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(testValue + 1)
            })
        });

        it("testFloat callback", function () {
            var testValue = -42.0;
            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testFloat(testValue,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(testValue-0.5)
            })
        });

        it("testArray callback", function () {
            var testarray = alptabet.slice(0);

            var sorted = testarray.sort(function(a, b) { 
                return a.toLowerCase() < b.toLowerCase()?1:-1;
            });

            var reversed = sorted.reverse();

            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testArray(reversed,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(sorted)
            })
        });

        it("testHash callback", function () {
            var testhash = {};
            var ref = {};

            for (var i = 0; i < alptabet.length; i++) {
                var k = alptabet[i];
                var v = nicks[i];

                testhash[k]=v;

                if ("aeiouy".indexOf(k[0])<0) {
                    ref[k] = v.split('').reverse().join('');
                } 
            };

            var flag, value;

            var callbackFunction = function (val) {
                value = val;
                flag = true
            };

            runs(function () {
                Rho.GenCallbackTests.testHash(testhash,callbackFunction);
            });

            waitsFor(
                function () {
                    return flag;
                },
                "Timeout",
                500
            );

            runs(function () {
                expect(value).toEqual(ref)
            })
        });
    }

})
;