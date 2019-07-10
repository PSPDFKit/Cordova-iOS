//
//  PSPDFKit.js
//  PSPDFPlugin for Apache Cordova
//
//  Copyright © 2013-2019 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY AUSTRIAN COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

var PSPDFKitPlugin = new function() {
    
    // Utilities
    
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
                        if (callback) callback(result, null);
                    }, function (error) {
                        console.log(error);
                        if (callback) callback(null, error);
                    }, 'PSPDFKitPlugin', methodName, argArray);
                }
            })();
        }
    }
    
    // Events
    
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
            while ((index = existing.indexOf(listener)) != -1) {
                existing.splice(index,1);
            }
        }
    }

    // License key
    
    addMethods({
        setLicenseKey: ['key'],
    });

    // PDF Generation method
    
    addMethods({
        convertPDFFromHTMLString: ['html', 'fileName', 'options', 'callback'],
    });
    
    // Document methods
    
    addMethods({
        present: ['path', 'callback', 'options'],
        presentWithXFDF: ['path', 'xfdfPath', 'callback', 'options'],
        dismiss: ['callback'],
        reload: [],
        search: ['query', 'animated', 'headless'],
        saveAnnotations: ['callback'],
        getHasDirtyAnnotations: ['callback'],
    });
    
    // Configuration
    
    addMethods({
        setOptions: ['options', 'animated'],
        getOptions: ['names', 'callback'],
        setOption: ['name', 'value', 'animated'],
        getOption: ['name', 'callback'],
    });
    
    // Page scrolling
    
    addMethods({
        setPage: ['page', 'animated'],
        getPage: ['callback'],
        getScreenPage: ['callback'],
        getPageCount: ['callback'],
        scrollToNextPage: ['animated'],
        scrollToPreviousPage: ['animated'],
    });

    // Appearance
    
    addMethods({
        setAppearanceMode: ['appearanceMode'],
    });

    // Cache

    addMethods({
        clearCache: [],
        removeCacheForPresentedDocument: [],
    });

    // Toolbar
    
    var leftBarButtonItems = ['close'];
    var rightBarButtonItems = ['search', 'outline', 'thumbnails'];
    
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

    // Annotation toolbar
    addMethods({
        hideAnnotationToolbar: [],
        showAnnotationToolbar: [],
        toggleAnnotationToolbar: [],
    });
    
    // Instant JSON
    addMethods({
        applyInstantJSON: ['jsonValue', 'callback'],
        addAnnotation: ['jsonAnnotation', 'callback'],
        removeAnnotation: ['jsonAnnotation', 'callback'],
        getAnnotations: ['pageIndex', 'type', 'callback'],
        getAllUnsavedAnnotations: ['callback']
    });
    
    // Forms
    addMethods({
        setFormFieldValue: ['value', 'fullyQualifiedName', 'callback'],
        getFormFieldValue: ['fullyQualifiedName', 'callback'],
    });
    
    // XFDF
    addMethods({
        importXFDF: ['xfdfPath', 'callback'],
        exportXFDF: ['xfdfPath', 'callback'],
    });
	
    // Document Processing
    addMethods({
        processAnnotations: ['annotationChange', 'processedDocumentPath', 'callback', 'annotationType'],
    });
};
module.exports = PSPDFKitPlugin;
