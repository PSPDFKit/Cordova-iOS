//
//  PSPDFKit.h
//  PSPDFPlugin for Apache Cordova
//
//  Copyright 2013 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY AUSTRIAN COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

var PSPDFKitPlugin = new function() {
    
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
                    for (var i = 0; i < arguments.length; i++) {
                        argArray.push(arguments[i]);
                        if (methodArgs[i] == 'callback') {
                            if (typeof (arguments[i]) == "function") {
                                callback = arguments[i];
                            }
                        }
                    }
                    cordova.exec(function (result) {
                        if (callback) callback(result);
                    }, function (error) {
                        alert(error);
                    }, 'PSPDFKitPlugin', methodName, argArray);
                }
            })();
        }
    }
    
    //events
    
    var listeners = {};
    
    this.dispatchEvent = function(event) {
        var result = undefined;
        var functions = listeners[event.type];
        if (functions) {
            for (var i = 0; i < functions.length; i++) {
                result = functions[i](event);
                if (typeof result != 'undefined') {
                    if (!result) return result;
                }
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
            var index;
            while (index = existing.indexOf(listener)) {
                existing.splice(index,1);
            }
        }
    }

    //license key
    
    addMethods({
               setLicenseKey: ['key'],
               });

    //document methods
    
    addMethods({
        present: ['path', 'callback', 'options'],
        dismiss: ['callback'],
        reload: [],
        search: ['query', 'animated', 'headless'],
        saveAnnotations: ['callback'],
    });
    
    //configuration
    
    addMethods({
        setOptions: ['options', 'animated'],
        getOptions: ['names', 'callback'],
        setOption: ['name', 'value', 'animated'],
        getOption: ['name', 'callback'],
    });
    
    //page scrolling
    
    addMethods({
        setPage: ['page', 'animated'],
        getPage: ['callback'],
        getScreenPage: ['callback'],
        getPageCount: ['callback'],
        scrollToNextPage: ['animated'],
        scrollToPreviousPage: ['animated'],
    });
    
    //toolbar
    
    var leftBarButtonItems = ['close'];
    var rightBarButtonItems = ['search', 'outline', 'viewMode'];
    
    this.dispatchLeftBarButtonAction = function(index)
    {
        leftBarButtonItems[index].action();
    }

    this.dispatchRightBarButtonAction = function(index)
    {
        rightBarButtonItems[index].action();
    }

    this.setLeftBarButtonItems = function(items)
    {
        leftBarButtonItems = items;
        cordova.exec(function (result) { }, function (error) { },
                     'PSPDFKitPlugin', 'setLeftBarButtonItems', [items]);
    }

    this.setRightBarButtonItems = function(items)
    {
        rightBarButtonItems = items;
        cordova.exec(function (result) { }, function (error) { },
                     'PSPDFKitPlugin', 'setRightBarButtonItems', [items]);
    }

    this.getLeftBarButtonItems = function(callback)
    {
        callback(leftBarButtonItems);
    }
    
    this.getRightBarButtonItems = function(callback)
    {
        callback(rightBarButtonItems);
    }
    
};
module.exports = PSPDFKitPlugin;
