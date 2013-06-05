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
    
    //events
    
    var listeners = {};
    
    this.dispatchEvent = function(event) {
        var result = undefined;
        var functions = listeners[event.type];
        for (var i = 0; i < functions.length; i++) {
            result = functions[i](event);
            if (typeof result != 'undefined') {
                if (!result) return result;
            }
        }
        return result;
    }

    this.addEventListener = function(type, listener) {
        var existing = listeners[type];
        if (!existing) {
            existing = [];
            listeners[type] = existing;
        }
        existing.push(listener);
    }
    
    this.addEventListeners = function(listeners) {
        for (type in listeners) {
            this.addEventListener(type, listeners[type]);
        }
    }
    
    this.removeEventListener = function(type, listener)
    {
        var existing = listeners[type];
        if (existing) {
            while (var index = existing.indexOf(listener)) {
                existing.splice(index,1);
            }
        }
    }
    
    //document methods
    
    addMethods({
        present: ['path', 'callback', 'options'],
        dismiss: ['callback'],
        reload: [],
        search: ['query', 'animated'],
        saveChangedAnnotations: ['callback'],
    });
    
    //configuration
    
    addMethods({
        setOptions: ['options', 'animated'],
        setOption: ['name', 'value', 'animated'],
        //getOption: ['name', 'callback'],
    });
    
    //page scrolling
    
    addMethods({
        setPage: ['page', 'animated'],
        getPage: ['callback'],
        getScreenPage: ['callback'],
        scrollToNextPage: ['animated'],
        scrollToPreviousPage: ['animated'],
    });
    
};