window.PSPDFKit = new function() {
    
    //utilities
    
    var self = this;
    function addMethods(methods) {
        for (var name in methods) {
            (function() {
                var methodName = name;
                var methodArgs = methods[methodName];
                self[methodName] = function() {
                    var callback = null;
                    var argArray = [];
                    for (var index in arguments) {
                        argArray.push(arguments[index]);
                        if (methodArgs[index] == 'callback') {
                            callback = arguments[index];
                        }
                    }
                    cordova.exec(function (result) {
                        if (callback) callback(result);
                    }, function (error) {
                        alert(error);
                    }, "PSPDFKit", methodName, argArray);
                }
            })();
        }
    }
    
    //document methods
    
    addMethods({
        present: ['path', 'options'],
        dismiss: [],
        reload: [],
        search: ['query', 'animated'],
        saveChangedAnnotations: ['callback']
    });
    
    //configuration
    
    addMethods({
        setOptions: ['options', 'animated'],
        setOption: ['name', 'value', 'animated'],
        //getOption: ['name', 'callback']
    });
    
    //page scrolling
    
    addMethods({
        setPage: ['page', 'animated'],
        getPage: ['callback'],
        getScreenPage: ['callback']
    });
    
};