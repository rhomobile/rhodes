describe("RhoSync", function() {
    it("able to be configured", function() {
        expect(rhosync.rho.config.syncServer).toEqual(syncUrl);
    });

    it("is able to be initialized with models", function() {

        var okHdlr = jasmine.createSpy('for ok');
        var errHdlr = jasmine.createSpy('for errors');

        expect(rhosync.init).toBeDefined();

        var models = [
            {name: 'Product', fields: [
                {name: 'name',      type: 'string'},
                {name: 'price',     type: 'int'},
                {name: 'available', type: 'boolean', defaultValue: true}
                ]},
            {name: 'Order', fields: [
                {name: 'unmber',       type: 'int'},
                {name: 'productName',  type: 'string'},
                {name: 'customerName', type: 'string'},
                {name: 'address',      type: 'string'},
                {name: 'phone',        type: 'string'}
                ]}
        ];

        rhosync.init(models /*, 'native'*/).done(okHdlr).fail(errHdlr);

        waitsForSpies([okHdlr, errHdlr], 'RhoSync init timeout', 3000);
        runs(function(){
            expect(errHdlr).not.toHaveBeenCalled();
            if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

            expect(rhosync.rho.models).toBeDefined('models map');
            expect(rhosync.rho.engine.sources).toBeDefined('sources map');

            expect(rhosync.rho.models.Product).toBeSet('Product model');
            expect(rhosync.rho.models.Product.name).toBeSet('Product model');
            expect(rhosync.rho.engine.sources.Product).toBeSet('Product model');
            expect(rhosync.rho.engine.sources.Product.name).toBeSet('Product model');
            expect(rhosync.rho.engine.sources.Product.id).toBeGreaterThan(0);
            expect(rhosync.rho.models.Product.name).toEqual(rhosync.rho.engine.sources.Product.name);
            jasmine.log(rhosync.rho.models.Product.name +' source id = ' +rhosync.rho.engine.sources.Product.id);

            expect(rhosync.rho.models.Order).toBeSet('Order model');
            expect(rhosync.rho.models.Order.name).toBeSet('Order model');
            expect(rhosync.rho.engine.sources.Order).toBeSet('Order model');
            expect(rhosync.rho.engine.sources.Order.name).toBeSet('Order model');
            expect(rhosync.rho.engine.sources.Order.id).toBeGreaterThan(0);
            expect(rhosync.rho.models.Order.name).toEqual(rhosync.rho.engine.sources.Order.name);
            jasmine.log(rhosync.rho.models.Order.name +' source id = ' +rhosync.rho.engine.sources.Order.id);

            expect(rhosync.rho.engine.sources.Product.name).not.toEqual(rhosync.rho.engine.sources.Order.name);
            expect(rhosync.rho.engine.sources.Product.id).not.toEqual(rhosync.rho.engine.sources.Order.id);
        });
    });

    it("should login ok with proper credentials", function() {
        //fakeAjax({urls: {'/login': {successData: 'login'}}});

        var okHdlr = jasmine.createSpy('ajax handler spy');

        $(window).bind(rhosync.rho.events.GENERIC_NOTIFICATION, null, notify);

        rhosync.rho.protocol.login(userlogin, userpass).done(function(data, response){
            okHdlr("success", response);
        }).fail(function(data, response){
            okHdlr("error", response);
        });

        waitsForSpies(okHdlr, 'login timeout');
        runs(function(){
            expect(okHdlr).toHaveBeenCalledWith("success", null);
            expect(notified).toBeTruthy();
            $(window).unbind(rhosync.rho.events.GENERIC_NOTIFICATION, notify);
        });
    });

    it("should fail to login with wrong credentials", function() {
        //fakeAjax({urls: {'/login': {successData: 'error'}}});

        var okHdlr = jasmine.createSpy('ajax handler spy');

        $(window).bind(rhosync.rho.events.GENERIC_NOTIFICATION, null, notify);

        rhosync.rho.protocol.login(userlogin, wrongpass).done(function(data, response){
            okHdlr("success", response);
        }).fail(function(data, response){
            okHdlr("error", response);
        });

        waitsForSpies(okHdlr, 'login timeout');
        runs(function(){
            expect(okHdlr).toHaveBeenCalledWith("error", "Unauthorized");
            expect(notified).toBeTruthy();
            $(window).unbind(rhosync.rho.events.GENERIC_NOTIFICATION, notify);
        });
    });

    describe("Rhomobile.rho.storage", function() {

        it("is able to open database and transaction", function() {
            var okHdlr = jasmine.createSpy('for ok');
            var errHdlr = jasmine.createSpy('for errors');

            runs(function(){
                expect(rhosync.rho.storage.open).toBeDefined();
                rhosync.rho.storage.open().done(okHdlr).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'open database timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });
            runs(function(){
                try {
                    expect(rhosync.rho.storage.tx).toBeDefined();
                    rhosync.rho.storage.tx().ready(okHdlr).fail(errHdlr);
                } catch (ex) {
                    jasmine.log(ex);
                }
            });
            waitsForSpies([okHdlr, errHdlr], 'open database timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });
        });

        it("is able to perform a query", function() {
            var okHdlr = jasmine.createSpy('for ok');
            var errHdlr = jasmine.createSpy('for errors');

            expect(rhosync.rho.storage.executeSql).toBeDefined();
            rhosync.rho.storage.executeSql("SELECT name FROM sqlite_master WHERE type='table'", null).done(function(){
                okHdlr(arguments);
            }).fail(function(){
                errHdlr(arguments);
            });

            waitsForSpies([okHdlr, errHdlr], 'db query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });
        });

        it("is able to be initialized", function() {
            var okHdlr = jasmine.createSpy('for ok');
            var errHdlr = jasmine.createSpy('for errors');

            expect(rhosync.rho.storage.initSchema).toBeDefined();
            rhosync.rho.storage.initSchema().done(okHdlr).fail(errHdlr);

            waitsForSpies([okHdlr, errHdlr], 'db initialization timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            runs(function(){
                expect(rhosync.rho.storage.getAllTableNames).toBeDefined();
                rhosync.rho.storage.getAllTableNames().done($.proxy(function(tx, tbNames){
                    okHdlr(arguments);
                    this.names = tbNames;
                }, this)).done(okHdlr).fail(errHdlr);
            });

            waitsForSpies([okHdlr, errHdlr], 'table names read timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.names).toBeDefined();
                jasmine.log('Table names are: "' +this.names.toString().replace(/,/g, '", "') +'"');
                expect(this.names.length).toEqual(4+1);
                expect(this.names).toContain('sources');
            });
        });

        it("is able to store clients", function() {
            var okHdlr = jasmine.createSpy('for ok');
            var errHdlr = jasmine.createSpy('for errors');

            expect(rhosync.rho.engine.Client).toBeDefined();
            expect(rhosync.rho.storage.listClientsId).toBeDefined();
            expect(rhosync.rho.storage.insertClient).toBeDefined();
            expect(rhosync.rho.storage.storeClient).toBeDefined();
            expect(rhosync.rho.storage.loadClient).toBeDefined();
            expect(rhosync.rho.storage.deleteClient).toBeDefined();

            var id1 = 'testId1_#' +Date.now().toString();
            var id2 = 'testId2_#' +Date.now().toString();

            // create clients
            var client1 = new rhosync.rho.engine.Client(id1);
            client1.session = "session1";
            var client2 = new rhosync.rho.engine.Client(id2);
            client2.session = "session2";

            // store them
            runs(function(){
                jasmine.log('insertClient()');
                rhosync.rho.storage.insertClient(client1).done(function(){
                        rhosync.rho.storage.insertClient(client2).done(okHdlr).fail(errHdlr);
                }).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients insert query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            // check there are two clients at least
            runs(function(){
                jasmine.log('listClientsId()');
                rhosync.rho.storage.listClientsId().done($.proxy(function(tx, ids){
                    this.ids = ids;
                    this.idsLengthWithTestClients = ids.length;
                }, this)).done(okHdlr).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients list select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.ids).toBeDefined();
                expect(this.ids.length).toBeDefined();
                expect(this.ids.length).toBeGreaterThan(1);
            });

            // read and verify clients
            runs(function(){
                jasmine.log('loadClient()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadClient(id1, tx).done($.proxy(function(tx, client){
                                this.client1 = client;
                            }, this)),
                            rhosync.rho.storage.loadClient(id2, tx).done($.proxy(function(tx, client){
                                this.client2 = client;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

                expect(this.client1).toBeDefined();
                expect(this.client1.id).toEqual(id1);
                expect(this.client1.session).toBeDefined();
                expect(this.client1.session).toEqual("session1");

                expect(this.client2).toBeDefined();
                expect(this.client2.id).toEqual(id2);
                expect(this.client2.session).toBeDefined();
                expect(this.client2.session).toEqual("session2");
            });

            // update them
            runs(function(){
                client1.session = "updatedSession1";
                client2.session = "updatedSession2";
                jasmine.log('storeClient()');
//                rhosync.rho.storage.storeClient(client1).done(function(){
//                        rhosync.rho.storage.storeClient(client2).done(okHdlr).fail(errHdlr);
//                }).fail(errHdlr);
                rhosync.rho.storage.tx("read-write").ready(function(db, tx){
                    $.when(
                            rhosync.rho.storage.storeClient(client1, tx).done(function(tx, client){
                            }),
                            rhosync.rho.storage.storeClient(client2, tx).done(function(tx, client){
                            })
                    ).done(function(obj, status){
                        okHdlr(obj, status);
                    }).fail(function(obj, error){
                        errHdlr(obj, error);
                    });
                }).done(function(obj, status){
                    okHdlr(obj, status);
                }).fail(function(obj, error){
                    errHdlr(obj, error);
                });
            });
            waitsForSpies([okHdlr, errHdlr], 'clients update query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });


            // read and verify updates
            runs(function(){
                jasmine.log('loadClient()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadClient(id1, tx).done($.proxy(function(tx, client){
                                this.client1 = client;
                                this.c1 = client;
                            }, this)),
                            rhosync.rho.storage.loadClient(id2, tx).done($.proxy(function(tx, client){
                                this.client2 = client;
                                this.c2 = client;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

                expect(this.client1).toBeDefined();
                expect(this.client1.id).toEqual(id1);
                expect(this.client1.session).toBeDefined();
                expect(this.client1.session).toEqual("updatedSession1");
                if (this.client1.session != "updatedSession1") {
//                    debugger;
//                    alert("sessions aren't equal! " +this.c1);
                }

                expect(this.client2).toBeDefined();
                expect(this.client2.id).toEqual(id2);
                expect(this.client2.session).toBeDefined();
                expect(this.client2.session).toEqual("updatedSession2");
                if (this.client2.session != "updatedSession2") {
//                    debugger;
//                    alert("sessions aren't equal! " +this.c2);
                }
            });

            // delete them
            runs(function(){
                jasmine.log('deleteClient()');
                rhosync.rho.storage.deleteClient(this.client1).done($.proxy(function(tx, client){
                    this.client1 = client;
                    rhosync.rho.storage.deleteClient(this.client2).done($.proxy(function(tx, client){
                        this.client2 = client;
                    }, this)).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients delete query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            // check there are two clients has been deleted
            runs(function(){
                jasmine.log('listClientsId()');
                rhosync.rho.storage.listClientsId().done($.proxy(function(tx, ids){
                    this.ids = ids;
                }, this)).done(okHdlr).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients list select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

                expect(this.ids).toBeDefined();
                expect(this.ids.length).toBeDefined();
                expect(this.ids.length).toEqual(this.idsLengthWithTestClients - 2);
            });

            // check load failure for absent clients
            runs(function(){
                jasmine.log('loadClient()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadClient(id1, tx).done($.proxy(function(tx, client){
                                this.client1 = client;
                            }, this)),
                            rhosync.rho.storage.loadClient(id2, tx).done($.proxy(function(tx, client){
                                this.client2 = client;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'clients select query timeout');
            runs(function(){
                expect(errHdlr).toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.client1).toBeNull();
                expect(this.client2).toBeNull();
            });
        });

        it("is able to store sources", function() {
            var okHdlr = jasmine.createSpy('for ok');
            var errHdlr = jasmine.createSpy('for errors');

            expect(rhosync.rho.engine.Source).toBeDefined();
            expect(rhosync.rho.storage.listSourcesId).toBeDefined();
            expect(rhosync.rho.storage.insertSource).toBeDefined();
            expect(rhosync.rho.storage.storeSource).toBeDefined();
            expect(rhosync.rho.storage.loadSource).toBeDefined();
            expect(rhosync.rho.storage.deleteSource).toBeDefined();

            var id1 = 'testId1_#' +Date.now().toString();  // It shouldn't work at all!!! client_id is BIGINT !
            var id2 = 'testId2_#' +Date.now().toString();

            // create sources
            var source1 = new rhosync.rho.engine.Source(id1);
            source1.name = "name1";
            var source2 = new rhosync.rho.engine.Source(id2);
            source2.name = "name2";

            // store them
            runs(function(){
                jasmine.log('insertSource()');
                rhosync.rho.storage.insertSource(source1).done(function(){
                        rhosync.rho.storage.insertSource(source2).done(okHdlr).fail(errHdlr);
                }).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources insert query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            // check there are two sources at least
            runs(function(){
                jasmine.log('listSourcesId()');
                rhosync.rho.storage.listSourcesId().done($.proxy(function(tx, ids){
                    this.ids = ids;
                    this.idsLengthWithTestSources = ids.length;
                }, this)).done(okHdlr).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources list select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.ids).toBeDefined();
                expect(this.ids.length).toBeDefined();
                expect(this.ids.length).toBeGreaterThan(1);
            });

            // read and verify sources
            runs(function(){
                jasmine.log('loadSource()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadSource(id1, tx).done($.proxy(function(tx, source){
                                this.source1 = source;
                            }, this)),
                            rhosync.rho.storage.loadSource(id2, tx).done($.proxy(function(tx, source){
                                this.source2 = source;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

                expect(this.source1).toBeDefined();
                expect(this.source1.id).toEqual(id1);
                expect(this.source1.name).toBeDefined();
                expect(this.source1.name).toEqual("name1");

                expect(this.source2).toBeDefined();
                expect(this.source2.id).toEqual(id2);
                expect(this.source2.name).toBeDefined();
                expect(this.source2.name).toEqual("name2");
            });

            // update them
            runs(function(){
                source1.name = "updatedName1";
                source2.name = "updatedName2";
                jasmine.log('storeSource()');
//                rhosync.rho.storage.storeSource(source1).done(function(){
//                        rhosync.rho.storage.storeSource(source2).done(okHdlr).fail(errHdlr);
//                }).fail(errHdlr);
                rhosync.rho.storage.tx("read-write").ready(function(db, tx){
                    $.when(
                            rhosync.rho.storage.storeSource(source1, tx).done(function(tx, source){
                            }),
                            rhosync.rho.storage.storeSource(source2, tx).done(function(tx, source){
                            })
                    ).done(function(obj, status){
                        okHdlr(obj, status);
                    }).fail(function(obj, error){
                        errHdlr(obj, error);
                    });
                }).done(function(obj, status){
                    okHdlr(obj, status);
                }).fail(function(obj, error){
                    errHdlr(obj, error);
                });
            });
            waitsForSpies([okHdlr, errHdlr], 'sources update query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            // read and verify updates
            runs(function(){
                jasmine.log('loadSource()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadSource(id1, tx).done($.proxy(function(tx, source){
                                this.source1 = source;
                                this.s1 = source;
                            }, this)),
                            rhosync.rho.storage.loadSource(id2, tx).done($.proxy(function(tx, source){
                                this.source2 = source;
                                this.s2 = source;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);

                expect(this.source1).toBeDefined();
                expect(this.source1.id).toEqual(id1);
                expect(this.source1.name).toBeDefined();
                expect(this.source1.name).toEqual("updatedName1");
                if (this.source1.name != "updatedName1") {
//                    debugger;
//                    alert("names aren't equal! "+ this.s1);
                }

                expect(this.source2).toBeDefined();
                expect(this.source2.id).toEqual(id2);
                expect(this.source2.name).toBeDefined();
                expect(this.source2.name).toEqual("updatedName2");
                expect(this.source2.name).toEqual("updatedName2");
                if (this.source1.name != "updatedName1") {
//                    debugger;
//                    alert("names aren't equal! "+ this.s2);
                }
            });

            // delete them
            runs(function(){
                jasmine.log('deleteSource()');
                rhosync.rho.storage.deleteSource(this.source1).done($.proxy(function(tx, source){
                    this.source1 = source;
                    rhosync.rho.storage.deleteSource(this.source2).done($.proxy(function(tx, source){
                        this.source2 = source;
                    }, this)).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources delete query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
            });

            // check there are two sources has been deleted
            runs(function(){
                jasmine.log('listSourcesId()');
                rhosync.rho.storage.listSourcesId().done($.proxy(function(tx, ids){
                    this.ids = ids;
                }, this)).done(okHdlr).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources list select query timeout');
            runs(function(){
                expect(errHdlr).not.toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.ids).toBeDefined();
                expect(this.ids.length).toBeDefined();
                expect(this.ids.length).toEqual(this.idsLengthWithTestSources - 2);
            });

            // check load failure for absent sources
            runs(function(){
                jasmine.log('loadSource()');
                rhosync.rho.storage.tx().ready($.proxy(function(db, tx){
                    $.when(
                            rhosync.rho.storage.loadSource(id1, tx).done($.proxy(function(tx, source){
                                this.source1 = source;
                            }, this)),
                            rhosync.rho.storage.loadSource(id2, tx).done($.proxy(function(tx, source){
                                this.source2 = source;
                            }, this))
                    ).done(okHdlr).fail(errHdlr);
                }, this)).fail(errHdlr);
            });
            waitsForSpies([okHdlr, errHdlr], 'sources select query timeout');
            runs(function(){
                expect(errHdlr).toHaveBeenCalled();
                if(0 < errHdlr.callCount) jasmine.log(errHdlr.mostRecentCall.args);
                expect(this.source1).toBeNull();
                expect(this.source2).toBeNull();
            });
        });
    });
});