beforeEach(function() {
  this.addMatchers({
    toBeSet: function() {
    	return (undefined != this.actual && null != this.actual);
    }
  })
});
