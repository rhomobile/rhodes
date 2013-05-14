describe("<ORM module specs>", function() {

    var cleanVars = function(object) {
        var vars = object.vars();
        var cleanVars = {};
        for (var key in vars) {
            if (vars.hasOwnProperty(key)) {
                if (key !== 'object' && key !== 'source_id') {
                    cleanVars[key] = vars[key];
                }
            }
        }
        return cleanVars;
    };

    beforeEach(function(){
        Rho.ORM.clear();
    });


    // Rho.ORM.clear: function() {
    // Rho.ORM.addModel: function(modelClass) {
    // Rho.ORM.getModel: function(modelClass) {
    it('add model, get model, clear all models', function() {
        // relays on beforeEach
        expect(Rho.ORM.getModel('Model')).toBeUndefined();

        var Model = Rho.ORM.addModel('Model');
        expect(Model).toBeDefined();
        expect(Rho.ORM.getModel('Model')).toBe(Model);

        Rho.ORM.clear();
        expect(Rho.ORM.getModel('Model')).toBeUndefined();
    });

    it('makes empty object', function() {
        var empty = Rho.ORM.addModel('Model').make();
        expect(cleanVars(empty)).toEqual({});
        var vars = empty.vars();
        var keySet = {};
        for (var key in vars) {
            if (vars.hasOwnProperty(key)) {
                keySet[key] = 0;
            }
        }
        expect(keySet).toEqual({'object': 0, 'source_id': 0});
    });

    it('makes object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').make({'key': 'value'}))).toEqual({'key': 'value'});
    });

    it('creates empty object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').create())).toEqual({});
    });

    it('creates object', function() {
        expect(cleanVars(Rho.ORM.addModel('Model').create({'key': 'value'}))).toEqual({'key': 'value'});
    });

    describe("<model's object>", function() {
        var Model = undefined;
        var object = undefined;

        beforeEach(function(){
            Model = Rho.ORM.addModel('Model');
            object = Model.make({'key': 'value'});
        });

        it('returns vars', function() {
            expect(cleanVars(object)).toEqual({'key': 'value'});
        });

        it('retrieves object id', function() {
            expect(object.object()).toBe(object.get('object'));
        });

        it('gets existing property', function() {
            expect(object.get('key')).toBe('value');
        });

        it('gets absent property', function() {
            expect(object.get('absent key')).toBeUndefined();
        });

        it('sets property', function() {
            object.set('key', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'another value'});
        });

        it('sets new property', function() {
            object.set('new key', 'new value');
            expect(cleanVars(object)).toEqual({'key': 'value', 'new key': 'new value'});
        });

        it('supports set chaining', function() {
            object.set('key', 'another value').set('new key', 'new value');
            expect(cleanVars(object)).toEqual({'key': 'another value', 'new key': 'new value'});
        });

        it('sets property', function() {
            object.set('key', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'another value'});
        });

        it('sets property with empty name', function() {
            object.set('', 'another value');
            expect(cleanVars(object)).toEqual({'key': 'value', '': 'another value'});
        });

        it('has properties', function() {
            expect(object.has('key')).toBe(true);
            expect(object.has('absent key')).toBe(false);
        });
    });

    it('creates object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create({'key': 'value'});
        var after = Model.count();
        expect(after).toBe(before + 1);
    });

    it('saves object to database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();

        var object = Model.create({'key': 'value'});

        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        object.set('key', 'another value').set('new key', 'new value');
        expect(Model.find(object.object()).vars()).not.toEqual(object.vars());
        object.save();
        expect(Model.find(object.object()).vars()).toEqual(object.vars());
    });

    it('updates object attributes in database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();

        var object = Model.create({'key': 'value', 'original key': 'original value'});

        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        object.updateAttributes({'key': 'another value', 'new key': 'new value'});
        expect(Model.find(object.object()).vars()).toEqual(object.vars());
        expect(cleanVars(object)).toEqual({'key': 'another value', 'new key': 'new value', 'original key': 'original value'});
    });

    it('destroys object in database', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        var object1 = Model.create({'key1': 'value1'});
        var object2 = Model.create({'key2': 'value2'});

        expect(Model.count()).toBe(2);

        object1.destroy();

        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(found[0].vars()).toEqual(object2.vars());
    });

    it('does not create empty object in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create();
        expect(Model.count()).toBe(before);
    });

    it('does not create object with the only empty property in database', function() {
        var Model = Rho.ORM.addModel('Model');
        var before = Model.count();
        Model.create({'': 'value'});
        expect(Model.count()).toBe(before);
    });

    it('deletes all objects in database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.create({'key': 'value'});
        expect(Model.count()).toBeGreaterThan(0);
        Model.deleteAll();
        expect(Model.count()).toBe(0);
    });

    it('deletes all objects of specific model in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        expect(Model1.count()).toBeGreaterThan(0);
        expect(Model2.count()).toBeGreaterThan(0);

        var before1 = Model1.count();
        Model2.deleteAll();
        var after1 = Model1.count();

        expect(after1).toBe(before1);
        expect(Model2.count()).toBe(0);
    });

    it('reads object from database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();
        Model.create({'key': 'value'});
        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(cleanVars(found[0])).toEqual({'key': 'value'});
    });

    it('does not write empty property to database', function() {
        var Model = Rho.ORM.addModel('Model');
        Model.deleteAll();
        Model.create({'key': 'value', '': 'empty'});
        var found = Model.find('all');
        expect(found.length).toBe(1);
        expect(cleanVars(found[0])).toEqual({'key': 'value'});
    });

    it('counts objects in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        var before1 = Model1.count();

        Model1.create({'key': 'value'});
        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        var after1 = Model1.count();

        expect(after1).toBe(before1 + 2);
    });

    it('counts objects in database using find', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        var before1 = Model1.find('count');

        Model1.create({'key': 'value'});
        Model1.create({'key': 'value'});
        Model2.create({'key': 'value'});

        var after1 = Model1.find('count');

        expect(after1).toBe(before1 + 2);
    });

    it('counts objects in database using find with condition', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        var before1 = Model1.find('count', {conditions: {'key': 'value to find'}});

        Model1.create({'key': 'value'});
        Model1.create({'key': 'value to find'});
        Model1.create({'key': 'value to find'});
        Model1.create({'another key': 'value to find'});
        Model2.create({'key': 'value to find'});

        var after1 = Model1.find('count', {conditions: {'key': 'value to find'}});

        expect(after1).toBe(before1 + 2);
    });
/*
if !defined?(RHO_WP7)
  it "should raise RecordNotFound error if nil given as find argument" do

    bExc = false
    begin
      getAccount.find(nil)
    rescue Exception => e
	    puts "Exception : #{e}"
        bExc = e.is_a?(::Rhom::RecordNotFound)
    end
    bExc.should == true

  end
end
*/
    it('finds all objects in database', function() {
        var Model1 = Rho.ORM.addModel('Model1');
        var Model2 = Rho.ORM.addModel('Model2');

        Model1.deleteAll();

        Model1.create({'key1': 'value1'});
        Model2.create({'key2': 'value2'});
        Model1.create({'key3': 'value3'});

        var found = Model1.find('all');

        expect(found.length).toBe(2);
        var i = (found[0].has('key1')) ? 0 : 1;
        expect(cleanVars(found[i    ])).toEqual({'key1': 'value1'});
        expect(cleanVars(found[1 - i])).toEqual({'key3': 'value3'});
    });

    it('finds all objects with one condition', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        Model.create({'key': 'value1'});
        var objects = [Model.create({'key': 'value2'}), Model.create({'key': 'value2'})];

        var found = Model.find('all', {conditions: {'key': 'value2'}});

        expect(found.length).toBe(2);
        var i = (found[0].object() === objects[0].object()) ? 0 : 1;
        expect(found[i    ].vars()).toEqual(objects[0].vars());
        expect(found[1 - i].vars()).toEqual(objects[1].vars());
    });

    it('finds all objects with conditions', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        Model.create({'key1': 'value2'});
        Model.create({'key2': 'value3'});
        var objects = [
            Model.create({'key1': 'value2', 'key2': 'value3'}),
            Model.create({'key1': 'value2', 'key2': 'value3'})
        ];
        Model.create({'key1': 'value2', 'key2': 'value2'});
        Model.create({'key1': 'value3', 'key2': 'value3'});

        var found = Model.find('all', {conditions: {'key1': 'value2', 'key2': 'value3'}});

        expect(found.length).toBe(2);
        var i = (found[0].object() === objects[0].object()) ? 0 : 1;
        expect(found[i    ].vars()).toEqual(objects[0].vars());
        expect(found[1 - i].vars()).toEqual(objects[1].vars());
    });

    it('finds specific object', function() {
        var Model = Rho.ORM.addModel('Model');

        var original = Model.create({'key1': 'value1'});
        Model.create({'key2': 'value2'});

        expect(Model.find(original.object()).vars()).toEqual(original.vars());
    });

    it('finds first object in database', function() {
        var Model = Rho.ORM.addModel('Model');

        Model.deleteAll();

        var originals = [Model.create({'key1': 'value1'}), Model.create({'key3': 'value3'})];

        var found = Model.find('first');

        expect(found.vars()).toEqual(originals[(found.has('key1')) ? 0 : 1].vars());
    });

/*
  it "should find with conditions" do
    @accts = getAccount.find(:all, :conditions => {'industry' => 'Technology'}, :order => 'name', :orderdir => "desc")
    @accts.length.should == 2
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"
  end

  it "should find with advanced OR conditions" do
    query = '%IND%'
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query},
        :op => 'OR', :select => ['name','industry'])

    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should find with advanced OR conditions with order" do
    query = '%IND%'
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query},
        :op => 'OR', :select => ['name','industry'],
        :order=>'name', :orderdir=>'DESC' )

    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should NOT find with advanced OR conditions" do
    query = '%IND33%'
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query},
        :op => 'OR', :select => ['name','industry'])

    @accts.length.should == 0
  end

  it "should find with advanced AND conditions" do
    query = '%IND%'
    query2 = '%chnolo%' #LIKE is case insensitive by default
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query2
       },
       :op => 'AND',
       :select => ['name','industry'])

    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should find with advanced AND conditions with order" do
    query = '%IND%'
    query2 = '%chnolo%' #LIKE is case insensitive by default
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query2
       },
       :op => 'AND',
       :select => ['name','industry'],
       :order=>'name', :orderdir=>'DESC')

    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should NOT find with advanced AND conditions" do
    query = '%IND123%'
    query2 = '%chnolo%'     #LIKE is case insensitive by default
    @accts = getAccount.find( :all,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query2},
        :op => 'AND', :select => ['name','industry'])

    @accts.length.should == 0
  end

  it "should count with advanced AND conditions" do
    query = '%IND%'
    query2 = '%chnolo%'     #LIKE is case insensitive by default
    nCount = getAccount.find( :count,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query2},
        :op => 'AND' )

    nCount.should == 1
  end

  it "should count 0 with advanced AND conditions" do
    query = '%IND123%'
    query2 = '%chnolo%'     #LIKE is case insensitive by default
    nCount = getAccount.find( :count,
       :conditions => {
        {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
        {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query2},
        :op => 'AND')

    nCount.should == 0
  end

  it "should find with advanced AND conditions and non-string value" do
    res = getAccount.find( :all,
       :conditions => {
        {:func=>'length', :name=>'name', :op=>'>'} => 0
       },
       :op => 'AND')

    res.should_not be_nil
    res.length.should  == 2
  end

  it "should search with LIKE" do
    query2 = '%CHNolo%'     #LIKE is case insensitive by default
    nCount = getAccount.find( :count,
       :conditions => {
        {:name=>'industry', :op=>'LIKE'} => query2}
    )

    nCount.should_not == 0
  end

  it "should search with 3 LIKE" do
    getAccount.create({:SurveyID=>"Survey1", :CallID => 'Call1', :SurveyResultID => 'SurveyResult1'})
    getAccount.create({:SurveyID=>"Survey2", :CallID => 'Call2', :SurveyResultID => 'SurveyResult2'})
    getAccount.create({:SurveyID=>"Survey3", :CallID => 'Call3', :SurveyResultID => 'SurveyResult3'})

    shift_callreport = true
    prevresult = getAccount.find(:first, :conditions =>
            {{:func => 'LOWER', :name => 'SurveyID', :op => 'LIKE'} => 'survey%',
            {:func => 'LOWER', :name => 'CallID', :op => 'LIKE'} => 'call%',
            {:func => 'LOWER', :name => 'SurveyResultID', :op => 'LIKE'} => 'surveyresult%'},
            :op => 'AND') if shift_callreport

    prevresult.should_not be_nil
  end

  it "should search with IN array" do
    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'IN'} => ["Technology", "Technology2"] }
    )

    items.length.should == 2

    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'IN'} => ["Technology2"] }
    )

    items.length.should == 0

  end

  it "should search with IN string" do
    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'IN'} => "\"Technology\", \"Technology2\"" }
    )

    items.length.should == 2

    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'IN'} => "\"Technology2\"" }
    )

    items.length.should == 0

  end

  it "should search with NOT IN array" do
    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'NOT IN'} => ["Technology1", "Technology2"] }
    )

    items.length.should == 2

    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'NOT IN'} => ["Technology"] }
    )

    items.length.should == 0

  end

  it "should search with NOT IN string" do
    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'NOT IN'} => "\"Technology1\", \"Technology2\"" }
    )

    items.length.should == 2

    items = getAccount.find( :all,
       :conditions => {
        {:name=>'industry', :op=>'NOT IN'} => "\"Technology\"" }
    )

    items.length.should == 0

  end

  it "should find with group of advanced conditions" do
    query = '%IND%'
    cond1 = {
       :conditions => {
            {:name=>'name', :op=>'LIKE'} => query,
            {:name=>'industry', :op=>'LIKE'} => query},
       :op => 'OR'
    }
    cond2 = {
        :conditions => {
            {:name=>'description', :op=>'LIKE'} => 'Hello%'}
    }

    @accts = getAccount.find( :all,
       :conditions => [cond1, cond2],
       :op => 'AND',
       :select => ['name','industry','description'])

    @accts.length.should == 1
    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
  end

  it "should not find with group of advanced conditions" do
    query = '%IND%'
    cond1 = {
       :conditions => {
            {:func=>'UPPER', :name=>'name', :op=>'LIKE'} => query,
            {:func=>'UPPER', :name=>'industry', :op=>'LIKE'} => query},
       :op => 'OR'
    }
    cond2 = {
        :conditions => {
            {:name=>'description', :op=>'LIKE'} => 'Hellogg%'}
    }

    @accts = getAccount.find( :all,
       :conditions => [cond1, cond2],
       :op => 'AND',
       :select => ['name','industry'])

    @accts.length.should == 0
  end

  it "should find first with conditions" do
    @mobio_ind_acct = getAccount.find(:first, :conditions => {'name' => 'Mobio India'})
    @mobio_ind_acct.name.should == "Mobio India"
    @mobio_ind_acct.industry.should == "Technology"
  end

  it "should order by column" do
    @accts = getAccount.find(:all, :order => 'name')

    @accts.first.name.should == "Aeroprise"
    @accts.first.industry.should == "Technology"
    @accts[1].name.should == "Mobio India"
    @accts[1].industry.should == "Technology"
  end

  it "should desc order by column" do
    @accts = getAccount.find(:all, :order => 'name', :orderdir => 'DESC')

    @accts.first.name.should == "Mobio India"
    @accts.first.industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"
  end

  it "should order by block" do
    @accts = getAccount.find(:all, :order => 'name') do |x,y|
        y <=> x
    end

    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"

    @accts = getAccount.find(:all, :order => 'name', :orderdir => 'DESC') do |x,y|
        y <=> x
    end

    @accts[0].name.should == "Aeroprise"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Mobio India"
    @accts[1].industry.should == "Technology"

    puts "block without order parameter"
    @accts = getAccount.find(:all) do |item1,item2|
        item2.name <=> item1.name
    end

    @accts[0].name.should == "Mobio India"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Technology"

  end

  it "should order by multiple columns" do
    getAccount.create(:name=>'ZMobile', :industry => 'IT', :modified_by_name => 'user')
    getAccount.create(:name=>'Aeroprise', :industry => 'Accounting', :modified_by_name => 'admin')

    @accts = getAccount.find(:all, :order => ['name', 'industry'], :orderdir => ['ASC', 'DESC'])

    @accts.length().should == 4
    @accts[0].name.should == "Aeroprise"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Accounting"
    @accts[2].name.should == "Mobio India"
    @accts[2].industry.should == "Technology"
    @accts[3].name.should == "ZMobile"
    @accts[3].industry.should == "IT"

    puts "multiple order with condition"
    @accts = getAccount.find(:all, :conditions => {:modified_by_name => 'admin'},
        :order => ['name', 'industry'], :orderdir => ['ASC', 'DESC'])

    @accts.length().should == 3
    @accts[0].name.should == "Aeroprise"
    @accts[0].industry.should == "Technology"
    @accts[1].name.should == "Aeroprise"
    @accts[1].industry.should == "Accounting"
    @accts[2].name.should == "Mobio India"
    @accts[2].industry.should == "Technology"

  end

*/

    it("syncs", function() {
        var Model = Rho.ORM.addModel('Model');
        if (typeof Rho !== 'undefined' && typeof Rho.RhoConnectClient !== 'undefined') {
            var count = 0;
            Model.sync(function() {++count;});
            waitsFor(function() {return count > 0;}, 'callback', 10000);
            runs(function() {expect(count).toBe(101);});
        } else {
            Model.sync();
        }
    });

});
