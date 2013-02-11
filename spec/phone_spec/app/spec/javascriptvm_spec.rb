describe "JavaScriptVM" do

  it "should evaluate easy javascript" do
	js_function = "(function(x){ return x*x; })(10)"

    hundred = RhoScriptVM.call_js_function(js_function)

    puts js_function + ' = ' + hundred

    hundred.should == "100"
  end

  it "should evaluate moderate javascript" do
    names = ['Abigail', 'Addison', 'Aiden', 'Alexander', 'Alexis', 'Allison', 'Alyssa', 'Amelia', 'Andrew', 'Anna',
            'Anthony', 'Aubrey', 'Audrey', 'Ava', 'Avery', 'Benjamin', 'Brayden', 'Brooklyn', 'Caleb', 'Carter',
            'Charlotte', 'Chloe', 'Christian', 'Christopher', 'Claire', 'Connor', 'Daniel', 'David', 'Dylan', 'Eli',
            'Elijah', 'Elizabeth', 'Ella', 'Emily', 'Emma', 'Ethan', 'Evan', 'Evelyn', 'Gabriel', 'Gavin',
            'Grace', 'Hailey', 'Hannah', 'Harper', 'Henry', 'Isaac', 'Isabella', 'Jack', 'Jackson', 'Jacob',
            'James', 'Jayden', 'John', 'Jonathan', 'Joseph', 'Joshua', 'Kaylee', 'Landon', 'Layla', 'Leah',
            'Liam', 'Lillian', 'Lily', 'Logan', 'Lucas', 'Lucy', 'Luke', 'Madison', 'Mason', 'Matthew',
            'Max', 'Mia', 'Michael', 'Natalie', 'Nathan', 'Nevaeh', 'Noah', 'Olivia', 'Owen', 'Riley',
            'Ryan', 'Samantha', 'Samuel', 'Sarah', 'Sofia', 'Sophie', 'Tyler', 'Victoria', 'William', 'Wyatt',
            'Zoe', 'Zoey']

    test_string = '[' + names.reverse.map { |elm| "\"#{elm}\"" }.join(",") + ']';

	sorting_test = %{
		(function(test_array) {
			function merge(left, right, compare) {
				var result = [];

				while (left.length > 0 || right.length > 0) {
					if (left.length > 0 && right.length > 0) {
						if (compare(left[0], right[0]) <= 0) {
							result.push(left[0]);
							left = left.slice(1);
						}
						else {
							result.push(right[0]);
							right = right.slice(1);
						}
					}
					else if (left.length > 0) {
						result.push(left[0]);
						left = left.slice(1);
					}
					else if (right.length > 0) {
						result.push(right[0]);
						right = right.slice(1);
					}
				}
				return result;
			}

			function msort(compare) {
				var length = this.length, middle = Math.floor(length / 2);

				if (!compare) {
					compare = function(left, right) {
						if (left < right)
							return -1;
						if (left == right)
							return 0;
						else
							return 1;
					};
				}

				if (length < 2)
					return this;

				return merge(
					this.slice(0, middle).msort(compare),
					this.slice(middle, length).msort(compare),
					compare
				);
			}

			Array.prototype.msort = msort;

			var sorted = test_array.msort(function(left, right) {
				lval = left.toLowerCase();
				rval = right.toLowerCase();

				if (lval < rval)
					return -1;
				else if (lval == rval)
					return 0;
				else
					return 1;
			});

			return sorted.join();
		})(} + test_string + ')'

    sorted = RhoScriptVM.call_js_function(sorting_test)

    puts " Sorted result " + sorted

    sorted.should == names.sort().join(',')
  end

end
