jasmine.Matchers.prototype.isNotEmptyString = function(){
    return (((typeof this.actual)  == 'string') && (this.actual.length != 0));
}

jasmine.Matchers.prototype.isNumberGreaterThenZero = function(){
    return (((typeof this.actual)  == 'number') && (this.actual > 0));
}

jasmine.Matchers.prototype.isBoolean  = function(){
    return (typeof this.actual)  == 'boolean';
}
