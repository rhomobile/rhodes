beforeEach(function() {
  this.addMatchers({
    toBeSet: function(descr) {
    	return (undefined != this.actual && null != this.actual
                && this.actual == this.actual // (NaN == NaN) is always false
                );
    }
  })
});
