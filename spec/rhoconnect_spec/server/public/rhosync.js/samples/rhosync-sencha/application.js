(function($, Ext) {

    var LOG = new RhoSync.Logger('application.js');

    Ext.setup({
        // Mobile app setup options goes here if needed.
        // Icons, splash-screens, etc.
    });

    new Ext.Application({
        launch: doAppLaunch // it performs application initialization and start
    });

    // let's start here
    function doAppLaunch() {
        // UI initialization
        initUI();
    }

    // Here is model definitions. RhoSync.js don't need field definitions,
    // but it is needed for Ext.data.Model instances initializing.
    // At the moment RhoSync.js stores all values as strings.
    var modelDefinitions = [
        {
            name: 'Product',
            fields: [
                {name: 'id',        type: 'int'},
                {name: 'brand',     type: 'string'},
                {name: 'name',      type: 'string'},
                {name: 'sku',       type: 'string'},
                {name: 'price',     type: 'string'},
                {name: 'quantity',  type: 'string'}
            ]
        },
        {
            name: 'Customer',
            fields: [
                {name: 'id',      type: 'int'},
                {name: 'first',   type: 'string'},
                {name: 'last',    type: 'string'},
                {name: 'phone',   type: 'string'},
                {name: 'email',   type: 'string'},
                {name: 'address', type: 'string'},
                {name: 'city',    type: 'string'},
                {name: 'state',   type: 'string'},
                {name: 'zip',     type: 'string'},
                {name: 'lat',     type: 'string'},
                {name: 'long',    type: 'string'}
            ]
        }
    ];

    // Here are templates used to present objects in the lists,
    // fields are in the curve braces
    var displayTemplates = {
        'Product': '{brand} {name}',
        'Customer': '{first} {last}'
    };

    // Edit form definitions. The same as above: fields are needed for Ext.formFormPanel instances.
    // At the moment RhoSync.js stores all values as strings.
    var editForms = {
        'Product': [
            {xtype: 'textfield', name: 'name', label: 'Name', required: true},
            {xtype: 'textfield', name: 'brand', label: 'Brand', required: true},
            {xtype: 'textfield', name: 'sku', label: 'SKU'},
            {xtype: 'textfield', name: 'price', label: 'Price'},
            {xtype: 'textfield', name: 'quantity', label: 'Quantity'}
        ],
        'Customer': [
            {xtype: 'textfield', name: 'first', label: 'First name', required: true},
            {xtype: 'textfield', name: 'last', label: 'Last name', required: true},
            {xtype: 'textfield', name: 'phone', label: 'Phone'},
            {xtype: 'textfield', name: 'email', label: 'Email'},
            {xtype: 'textfield', name: 'address', label: 'Address'},
            {xtype: 'textfield', name: 'city', label: 'City'},
            {xtype: 'textfield', name: 'state', label: 'State'},
            {xtype: 'textfield', name: 'zip', label: 'ZIP'},
            {xtype: 'textfield', name: 'lat', label: 'Latitude'},
            {xtype: 'textfield', name: 'long', label: 'Longitude'}
        ]
    };


    // RhoSync.js initialization
    function initRhosync(username, doReset) {
        // For the case we starting slowly it shows "wait please" message
        var msg = Ext.Msg.alert('Application initialization', 'Wait please..', Ext.emptyFn);

        // At last, initialize RhoSync.js itself and return the promise object
        return RhoSync.init(modelDefinitions, 'native', syncProgressUpdate, doReset).done(function(){
            // Initialized, now we can hide (do not destroy, it's kind of singletone) "wait please" message
            msg.hide();
            // Reload all lists
            reloadLists();
        }).fail(function(error){
            // RhoSync.js hasn't been initialized properly
            Ext.Msg.alert('Error', error, Ext.emptyFn);
        });
    }

    // Utility array with model names to track a sync progress
    var syncProgressArray = [];
    // Utility variable with total models number to track a sync progress
    var syncProgressLength = 0;

    // Init progress variables before progress tracking
    function initSyncProgress() {
        syncProgressArray = [];
        // Fill array with model names
        $.each(modelDefinitions, function(idx, model){
            syncProgressArray.push(model.name);
        });
        // Set the total number of models to sync
        syncProgressLength = syncProgressArray.length;
        // Reset progress indicator value
        setProgress(0);
    }

    // Set progress indicator value in percents
    function setProgress(percents) {
        $('div.progressDone').css('width', percents +'%');
    }

    // Sync progress update callback, receives model name just has been synchronized
    function syncProgressUpdate(modelName) {
        // Exclude this model name from tracking array
        for (var i=0; i<syncProgressArray.length; i++) {
            if (syncProgressArray[i] == modelName) {
                syncProgressArray.splice(i, 1);
                break;
            }
        }
        // New length of tracking array
        var len = syncProgressArray.length;
        // Calculate the progress
        var progress = (syncProgressLength - len) * (100/syncProgressLength);
        // Set progress indicator
        setProgress(progress);
    }

    // To show the object list for exact model
    function showObjects(record) {
        var modPanel = Ext.getCmp('modelsPanel');
        // Method "forth" stores current page before transition, to use later in method "back".
        // Besides history handling it acts similar to setActiveItem().
        // First parameter is id of page to transit on. It is based on the model name.
        // Second parameter is animation to use
        modPanel.getLayout().forth(record.data.name +'List', null /*use default animation*/, record.data.name);
    }

    // Show the edit form for exact object
    function showForm(record) {
        // Model name is accessible from store
        var modelName = record.store.model.modelName;
        // To load record to the form, we need obtain form firstly. Form id is based on the model name.
        var form = Ext.getCmp(modelName+'Form');
        form.loadRecord(record);

        // Calculate proper title for known model names
        var title = 'Product' == modelName
                ? record.data.brand +' ' +record.data.name
                : record.data.first +' ' +record.data.last;

        // If empty, then it is new object
        title = (title && title.replace(' ', '')) ?  title : 'New ' +modelName;

        var modPanel = Ext.getCmp('modelsPanel');
        // Method "forth" stores current page before transition, to use later in method "back".
        // Besides history handling it acts similar to setActiveItem().
        // First parameter is id of form to transit on.
        // Second parameter is animation to use
        modPanel.getLayout().forth(form.id, null /*use default animation*/, title);
    }

    var progressPanel = null;
    function showProgressBar(title) {
        // Inner HTML code to change style upon a progress
        var html = '<div class="progressBar"><div class="progressDone"></div><div class="progressTodo"></div></div>';
        // Make it singletone
        if (!progressPanel) {
            progressPanel = new Ext.Sheet({
                id: 'progressPanel',
                floating: true,
                modal: true,
                centered: true,
                width: 300,
                height: 140,
                styleHtmlContent: true,
                html: html,
                dockedItems: [{
                        dock: 'top',
                        xtype: 'toolbar'
                }]
            });
        }
        // Show it with 'pop' animation
        progressPanel.show('pop');
        // Set the title
        progressPanel.dockedItems.get(0).setTitle(title);
        return progressPanel;
    }

    // Show error message with OK button
    function showError(title, errCode, err) {
        // use error code as a message if error message is empty
        Ext.Msg.alert(title, err || errCode, Ext.emptyFn);
        // Send error record to console log
        LOG.error(title +': ' +errCode +': ' +err);
    }

    // Active user name
    var activeUserName = null;

    // Perform login with username and password
    function doLogin(username, password){
        RhoSync.login(username, password, new RhoSync.SyncNotification()).done(function(){
            // Store active user name
            activeUserName = username;
            // Init DB for the user on success
            initRhosync(username).done(function(){
                // Update UI state on success: hide login form and show navigation pages
                updateLoggedInState();
            });
        }).fail(function(errCode, err){
            // Show error on failure
            showError('Login error', errCode, err);
        });
    }

    // Perform logout
    function doLogout(){
        RhoSync.logout().done(function(){
            // Reset login form to initial state
            Ext.getCmp('loginForm').reset();
            // Update UI state on success: show login form and hide navigation pages
            updateLoggedInState();
        }).fail(function(errCode, err){
            // Show error on failure
            showError('Logout error', errCode, err);
        });
    }

    // Update UI state based on logged in status
    function updateLoggedInState() {
        if (RhoSync.isLoggedIn()) {
            // If user is logged in OK
            // Show navigational pages and forms panel
            Ext.getCmp('mainPanel').setActiveItem('modelsPanel');
            // Show logout and enable sync button
            Ext.getCmp('logoutButton').show();
            Ext.getCmp('syncButton').enable();
        } else {
            // Hide navigational pages and forms panel. Show login Form.
            Ext.getCmp('mainPanel').setActiveItem('loginForm');
            // Hide logout and disable sync button
            Ext.getCmp('logoutButton').hide();
            Ext.getCmp('syncButton').disable();
        }
        // Refresh UI
        Ext.getCmp('mainPanel').doLayout();

        reloadLists();

        // Navigate back to the root list of models
        var modPanel = Ext.getCmp("modelsPanel");
        // Going back while history isn't empty
        while(!modPanel.getLayout().isHistoryEmpty()) {
            // Method "back" transits to last page from the history, then removes it from the history.
            // Besides history handling it acts similar to setActiveItem().
            // It uses transition animation which is reverse to stored in the history. Also it restores the title.
            modPanel.getLayout().back();
        }
        modPanel.getLayout().setActiveItem('ModelList');
        // Refresh models panel. It is the panel with all lists and forms.
        modPanel.doLayout();
        modPanel.doComponentLayout();
    }

    // Just reloads all object lists.
    function reloadLists() {
        $.each(modelDefinitions, function(i, model) {
            Ext.getCmp(model.name +'List').store.read();
        });
    }

    // Perform data synchronization with the server
    function doSync(){
        initSyncProgress();
        // Show progress bar
        var progress = showProgressBar('Synchronizing..');
        RhoSync.syncAllSources().done(function(){
            // Make progress bar disappear not so fast
            setTimeout(function(){ progress.hide(); }, 500);
            // Reload all lists
            reloadLists();
        }).fail(function(errCode, err){
            progress.hide();
            // Show error message on failure
            showError('Synchronization error', errCode, err);
        });
    }

    // Update record from the form
    function doUpdate(form) {
        // Get edited record and store
        var record = form.getRecord();
        var store = record.store;
        // Update record
        form.updateRecord(record, true);
        // Force store to flush the data to persistence layer
        store.sync();
        // Switch page back to the object list
        Ext.getCmp('modelsPanel').getLayout().back();
    }

    // Delete record in the form
    function doDelete(form){
        // Ask for confirmation
        Ext.Msg.confirm('Delete object', 'Are you sure?', function(yn){
            // If yes, perform the object deletion
            if ('yes' == yn.toLowerCase()) {
                // Get record and store
                var record = form.getRecord();
                var store = record.store;
                // Remove record
                store.remove(record);
                // Force store to flush the data to persistence layer
                store.sync();
                // Switch page back to the object list
                Ext.getCmp('modelsPanel').getLayout().back();
            }
        });
    }

    // All panels, used as pages/cards in mainPanel with CardHistoryLayout
    var allPages = [];

    // Init data-related parts of UI
    function initDataPages() {
        // This function builds models list to use as root page/menu of navigation.
        // It doesn't participates in any data exchange.
        function buildModelsList(data) {
            // The only purpose of this model is just to hold real models name.
            Ext.regModel('ModelSelectionItem', {
                fields: [{name: 'name', type: 'string'}]
            });
            // Store with real model names
            var store = new Ext.data.Store({
                autoLoad: true,
                model: 'ModelSelectionItem',
                data : data,
                proxy: {
                    // it is read-only
                    type: 'memory',
                    reader: {
                        type: 'json',
                        root: 'items'
                    }
                }
            });
            // List with real model names
            var list = new Ext.List({
                id: 'ModelList',
                fullscreen: false,
                itemTpl: '{name}', // It shows just model names
                store: store
            });
            // Taping on model name in the list it transits UI on appropriate model instance list
            list.on('itemtap', function(list, index, item, evt){
                // get item
                var record = list.getRecord(item);
                // show the list of model instances
                showObjects(record);
            });
            return list;
        }

        // This function builds store for provided model
        function buildStoreFor(model) {
            return new Ext.data.Store({
                // It forms id as <ModelName>Store
                id: model.name+'Store',
                autoLoad: false,
                model: model.name,
                proxy: {
                    type: 'rhosync',
                    // Here is special type of Proxy used. It is
                    // Ext.data.RhosyncStorageProxy defined in the rhosync.ext.js file
                    dbName: 'rhoSyncDb',
                    root: 'items',
                    reader: {
                        type: 'json',
                        root: 'items'
                    }
                }
            });
        }

        // This function builds object list for provided model, store and item display template
        function buildListFor(model, store, itemTpl) {
            var list = new Ext.List({
                // It forms id as <ModelName>List
                id: model.name+'List',
                fullscreen: false,
                store: store,
                itemTpl: itemTpl
            });
            // Taping on item it transits UI on appropriate object edit form
            list.on('itemtap', function(list, index, item, evt){
                // Get item
                var record = list.getRecord(item);
                // Show the edit form
                showForm(record);
            });
            // On list show event we updating createButton
            list.on('show', function(){
                var createButton = Ext.getCmp('createButton');
                // to make it visible on the toolbar
                createButton.show();
                createButton.doComponentLayout();
                // and to call appropriate form on click
                createButton.setHandler(function(btn) {
                    // Add new record to the store
                    var record = store.add({})[0];
                    // And show edit form for it
                    showForm(record);
                });
            });
            // On list hide event we make createButton hidden
            list.on('hide', function(){
                var createButton = Ext.getCmp('createButton');
                createButton.hide();
                createButton.doComponentLayout();
            });
            return list;
        }

        // This function builds the object edit form for provided model
        function buildFormFor(model) {

            // It's submit button in the form
            var submitItem = {xtype: 'button', text: 'Save', handler: function(btn) {
                var form = Ext.getCmp(model.name+'Form');
                // It makes form/record update on the click
                doUpdate(form);
            }};

            // The form itself
            var form = new Ext.form.FormPanel({
                // It forms id as <ModelName>Form
                id: model.name+'Form',
                scroll: 'vertical',
                items: editForms[model.name].concat(submitItem)
            });

            // On form show event we updating deleteButton
            form.on('show', function(){
                var deleteButton = Ext.getCmp('deleteButton');
                // to make it visible on the toolbar
                deleteButton.show();
                deleteButton.doComponentLayout();
                // and to delete the object on click
                deleteButton.setHandler(function(btn) {
                    doDelete(form);
                });
            });
            // On list hide event we make deleteButton hidden
            form.on('hide', function(){
                var deleteButton = Ext.getCmp('deleteButton');
                deleteButton.hide();
                deleteButton.doComponentLayout();
            });
            return form;
        }

        // Temporary variable with pages (there are forms and lists for models)
        var pgs = [];
        // List of model names
        var modelsData = {items:[]};

        // For each of model definition
        $.each(modelDefinitions, function(idx, model){
            // we are register the model with Ext.ModelMgr
            Ext.regModel(model.name, model);
            // build store, list and form
            var store = buildStoreFor(model);
            var list = buildListFor(model, store, displayTemplates[model.name]);
            var form = buildFormFor(model);
            // push pages into array
            pgs.push(list);
            pgs.push(form);
            // push model name to list on the root page
            modelsData.items.push({name: model.name});
        });

        // Build the list of all model names
        var list = buildModelsList(modelsData);
        // Make list of all pages
        allPages = [list].concat(pgs);
    }

    // UI initialization
    function initUI() {

        // Init data-related parts of UI firstly
        initDataPages();
        // then init general parts of UI

        // Logout button instance
        var logoutButton = new Ext.Button({
            id: 'logoutButton',
            text: 'Logout',
            handler: doLogout
        });

        // Back button instance, it is hidden by default
        // and becomes visible on first event of pages navigation
        var  backButton = new Ext.Button({
            id: 'backButton',
            text: 'Back',
            ui: 'back',
            hidden: true,
            handler: function() {
                // Switch page back
                modelsPanel.getLayout().back();
            }
        });

        // init DB button instance
        var initDbButton = new Ext.Button({
            id: 'initDbButton',
            text: 'Init DB',
            handler: function(){initRhosync(activeUserName, true /*do reset data*/);}
        });

        // sync button instance
        var syncButton = new Ext.Button({
            id: 'syncButton',
            text: 'Sync',
            handler: doSync
        });

        // Create button instance, it is hidden by default
        // and becomes visible on object list activation
        var createButton = new Ext.Button({
            id: 'createButton',
            text: 'Create new',
            hidden: true
        });

        // Delete button instance, it is hidden by default
        // and becomes visible on object edit form activation
        var deleteButton = new Ext.Button({
            id: 'deleteButton',
            text: 'Delete',
            hidden: true,
            handler: doDelete
        });

        // Login form instance
        var loginForm = new Ext.form.FormPanel({
            id: 'loginForm',
            fullscreen: true,
            standardSubmit: false,
            dockedItems: [
                {
                    id: 'loginToolbar',
                    xtype: 'toolbar',
                    dock : 'top',
                    title: 'Sign in'
                }
            ],
            items: [
                {
                    xtype: 'textfield',
                    name : 'login',
                    label: 'Username',
                    value: 'testUserToFailAuth'
                },
                {
                    xtype: 'passwordfield',
                    name : 'password',
                    label: 'Password'
                },
                {
                    xtype: 'button',
                    text: 'Login',
                    ui: 'confirm',
                    handler: function() {
                        doLogin(loginForm.getValues().login, loginForm.getValues().password);
                    }
                }
            ]
        });

        // Models panel instance. It contains model and object lists and forms.
        var modelsPanel = new Ext.Panel({
            id: 'modelsPanel',
            fullscreen: true,
            layout: {
                xtype: 'layout',
                // Here is used a special type of layout: CardHistoryLayout.
                // It is extension of the CardLayout, which is able to handle
                // navigation forth and back with history.
                type:'cardhistory',
                // It uses 'slide' as default transition animation.
                defaultAnimation: 'slide',
                // It obtains current title to store in the history
                getTitle: function(){
                    return Ext.getCmp('modelsToolbar').titleEl.getHTML();
                },
                // And able to restore it transiting back
                setTitle: function(text){
                    Ext.getCmp('modelsToolbar').setTitle(text);
                },
                // Also, it able to update Back button visibility and text based on history state.
                setBack: function(isVisible, text) {
                    // Get back button
                    var back = Ext.getCmp("backButton");
                    // Make it visible if history isn't empty
                    back.setVisible(isVisible);
                    // Set button text if one has been stored in the history
                    if (text) {
                        back.setText(text);
                    }
                    // Refresh the button
                    back.doComponentLayout();
                }
            },
            dockedItems: [
                {
                    id: 'modelsToolbar',
                    xtype: 'toolbar',
                    dock : 'top',
                    title: 'Model',
                    items: [
                        backButton,         // on the left side
                        {xtype: 'spacer'},
                        {xtype: 'spacer'},
                        logoutButton,       // on the right side
                        createButton,       // on the right side
                        deleteButton,       // on the right side
                        initDbButton,       // on the right side
                        syncButton          // on the right side
                    ]
                }
            ],
            items: allPages
        });

        // Main panel is container for loginForm and modelsPanel, it uses CardLayout to switch them.
        var mainPanel = new Ext.Panel({
            id: 'mainPanel',
            fullscreen: true,
            layout: 'card',
            items: [loginForm, modelsPanel]
        });

        mainPanel.setActiveItem('loginForm');
        
        // Ok, we have UI created and ready to use it. So just update logged in state.
        //updateLoggedInState();
        // good luck!
    }

})(jQuery, Ext);
