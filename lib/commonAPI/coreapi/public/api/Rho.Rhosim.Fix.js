(function () {
    // Fix for bug in str.toLowerCase() method of String class in QTWebView component executing on Windows.
    String.prototype.toLowerCase = function (str) { return __rhoNativeApi.toLowerCase(this); }
})();
