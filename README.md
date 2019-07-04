Cordova Wrapper for PSPDFKit 8 for iOS
========================================

The [PSPDFKit SDK](https://pspdfkit.com/pdf-sdk/) is a framework that allows you to view, annotate, sign, and fill PDF forms on iOS, Android, Windows, macOS, and Web. [PSPDFKit Instant](https://pspdfkit.com/instant/) adds real-time collaboration features to seamlessly share, edit, and annotate PDF documents.

PSPDFKit comes with open source wrappers for Cordova on both [iOS](https://pspdfkit.com/guides/ios/current/other-languages/apache-cordova-phonegap/) and [Android](https://pspdfkit.com/guides/android/current/other-languages/apache-cordova-phonegap/).

**This plugin works with Cordova version 8.1.2 and above.**  
**Please also make sure that you're using the latest version of Xcode 10.**

Install
-----------

We assume you have [a current install of Cordova](https://cordova.apache.org/#getstarted).

    cordova plugin add https://github.com/PSPDFKit/Cordova-iOS.git

Read the message at the end and complete installation.

**You will need to manually edit the Xcode project and add PSPDFKit to your project manually.
There is no automatic support for dynamic frameworks in Cordova yet. You will get a linker error if you forget this step.**

**Note:** Make sure to follow [our integration guide](https://pspdfkit.com/guides/ios/current/getting-started/integrating-pspdfkit/) to strip the framework, working around an App Store submission bug.

### Optional: CocoaPods Integration

If you wish to integrate PSPDFKit into your project [using CocoaPods](https://pspdfkit.com/guides/ios/current/getting-started/using-cocoapods/), you can use a third-party plugin like [`cordova-plugin-cocoapods-support`](https://github.com/blakgeek/cordova-plugin-cocoapods-support).

**Important** If you’re an existing customer, you can find the CocoaPods and license keys in your [customer portal](https://customers.pspdfkit.com/). Otherwise, if you don’t already have PSPDFKit, [sign up for our 60-day trial](https://pspdfkit.com/try/) and you will receive an email with the instructions to get started.

Demo
-----------

You can find a demo app in [CordovaDemo](CordovaDemo).

In order to build it, please download PSPDFKit from your customer portal: https://customers.pspdfkit.com  
The .dmg file you downloaded contains two directories called `PSPDFKit.framework` and `PSPDFKitUI.framework`.  
Please copy these directories to `CordovaDemo/platforms/ios`.

You can set your license key in `CordovaDemo/www/js/index.js`.  
(Look for `PSPDFKitPlugin.setLicenseKey('YOUR KEY');`)

After that, you can build the demo app with `$ cordova build`.  
To run the demo app in the simulator use `$ cordova emulate`.

Ionic
-----------

1. Create Ionic app Cordova plugin (replace `IonicDemo` with your app's name): `ionic start IonicDemo blank --cordova`
2. Go into your app's folder: `cd IonicDemo`
3. Add iOS platform: `ionic cordova platform add ios`
4. Add PSPDFKit plugin: `ionic cordova plugin add https://github.com/PSPDFKit/Cordova-iOS.git`
5. Follow the instructions after adding the PSPDFKit plugin (manually edit the Xcode project)
6. Declare `PSPDFKitPlugin` in `src/declarations.d.ts` (create this file first): `declare var PSPDFKitPlugin: any;`
7. Present PDF by modifying `src/app/app.component.ts`:

```javascript
constructor(platform: Platform, statusBar: StatusBar, splashScreen: SplashScreen) {
    platform.ready().then(() => {
      // Okay, so the platform is ready and our plugins are available.
      // Here you can do any higher level native things you might need.
      statusBar.styleDefault();
      splashScreen.hide();

      PSPDFKitPlugin.setLicenseKey('YOUR KEY');
      PSPDFKitPlugin.present('pdf/document.pdf', {});
    });
}
```

8. Place your `document.pdf` in the `www/pdf` subdirectory.
9. Try out your app: `ionic cordova emulate ios`

### Troubleshooting

#### Problem:

```sh
Error: Cannot find plugin.xml for plugin "Cordova-iOS". Please try adding it again.
```

#### Solution:

Run `cordova plugin add https://github.com/PSPDFKit/Cordova-iOS.git` instead of `ionic cordova plugin add https://github.com/PSPDFKit/Cordova-iOS.git`.

Ionic Demo
-----------

You can find an Ionic demo app in [IonicDemo](IonicDemo).

1. Go into the demo app's directory: `cd IonicDemo`
2. Prepare the app: `ionic cordova prepare ios`
3. Download PSPDFKit from your customer portal: https://customers.pspdfkit.com. The .dmg file you downloaded contains two directories called `PSPDFKit.framework` and `PSPDFKitUI.framework`. Please copy these directories to `IonicDemo/platforms/ios`.
4. Follow the instructions that are shown after the PSPDFKit plugin gets added (manually edit the Xcode workspace, `IonicDemo.xcworkspace`)  
5. Set your license key in `IonicDemo/src/app/app.component.ts` (look for `PSPDFKitPlugin.setLicenseKey('YOUR KEY');`)
6. Build the app with `ionic cordova build ios` and run it with `ionic cordova emulate ios`

Usage
-----------

The plugin is accessed via the PSPDFKitPlugin singleton. Here are some example calls:
    
    // set your license key here
    PSPDFKitPlugin.setLicenseKey("YOUR KEY");

    //show pdf with in double page mode, with curl transition red background
    PSPDFKitPlugin.present('pdf/castles.pdf', {
        pageTransition : 'curl',
        pageMode: 'double',
        backgroundColor: 'red'
    });
    
    //show pdf with callback
    PSPDFKitPlugin.present('pdf/castles.pdf', function() {
        alert('pdf has appeared');
    });
    
    //scroll to page 1
    PSPDFKitPlugin.setPage(1, true);
    
    //get the page number
    PSPDFKitPlugin.getPage(function(page) {
        alert('Current page: ' + page);
    });


Functions
------------

The plugin functions currently implemented are:

    present(path, [callback], [options]);
    
Displays a PDF in a full-screen modal. The path should be a string containing the file path (not URL) for the PDF. Relative paths are assumed to be relative to the www directory (if the path has a different base URL set, this will be ignored). To specify a path inside the application documents or library directory, use a `~`, e.g. `"~/Documents/mypdf.pdf"` or `"~/Library/Application Support/mypdf.pdf"`. Path can be null, but must not be omitted

The `options` parameter is an optional object containing configuration properties for the PDF document and/or view controller. All currently supported values are listed below under Options.

The optional callback will be called once the PDF controller has fully appeared on screen. Calling present() when there is already a PDF presented will load the new PDF in the current modal (in which case the callback will fire immediately).

    presentWithXFDF(path, xfdfPath, [callback], [options]);

Displays a PDF in a full-screen modal. The path should be a string containing the file path (not URL) for the PDF. Relative paths are assumed to be relative to the www directory (if the path has a different base URL set, this will be ignored). To specify a path inside the application documents or library directory, use a `~`, e.g. `"~/Documents/mypdf.pdf"` or `"~/Library/Application Support/mypdf.pdf"`. Path can be null, but must not be omitted

The `xfdfPath` should be a string containing the file path (not URL) for the XFDF file backing the PDF document. Relative paths are assumed to be relative to the www directory (if the xfdf path has a different base URL set, we will create an XFDF file in `'"~/Documents/" + xfdfPath'`). To specify a path inside the application documents or library directory, use a ~, e.g. `"~/Documents/myXFDF.xfdf"` or `"~/Library/Application Support/myXFDF.xfdf"`. The xfdfPath cannot be null and must not be omitted.

The `options` parameter is an optional object containing configuration properties for the PDF document and/or view controller. All currently supported values are listed below under Options.

The optional callback will be called once the PDF controller has fully appeared on screen. Calling present() when there is already a PDF presented will load the new PDF in the current modal (in which case the callback will fire immediately).

    dismiss([callback]);
    
This method dismisses the modally presented PDF view. The optional callback will be called once the PDF controller has dissapeared from the screen.
    
    reload();
    
This method reloads the current PDF.
    
    search(query, [animated], [headless]);
    
This method triggers a search for the specified query text. The optional animated argument determines if the search should be animated (if omitted, the search will not be animated). The optional headless argument determines whether the search UI should be disaplyed (if omitted, the search UI *will* be displayed).

    saveAnnotations(callback(error));
    
This method saves any changed annotations in the current or last opened document. On success the error callback parameter will be null, on failure it will be an object containing an error domain, code, and (possibly) a description and reason.
    
    setOptions(options, [animated]);
    
This method can be used to set multiple document and view controller settings at once. The options set will be applied to the current document (if there is one) as well as all subsequently displayed documents. All currently supported values are listed below under Options. The optional animated argument determines if the property should be animated. Not all property changes can be animated, so if the property does not support animation the animated argument will be ignored.
    
    getOptions(names, callback(values));

This method can be used to get several document or view controller options in a single call. The first argument is an array of option names, the second is a callback that will receive an object containing all the specified values by name. All currently supported values are listed below under Options.
    
    setOption(name, value, [animated]);
    
This method can be used to set a single document or view controller option. All currently supported values are listed below under Options. The optional animated argument determines if the property should be animated. Not all property changes can be animated, so if the property does not support animation the animated argument will be ignored.

    getOption(name, callback(value));
    
This method can be used to get a single document or view controller option. The first argument is the option name, the second is a callback that will receive the value. All currently supported values are listed below under Options.

    addEventListener(type, callback(event));
    
This method adds an event listener callback function for a specific event type. The list of possible event types is listed below under events. The callback will receive a single parameter called event, which will always contain the type and may contain other parameters, depending on the event type.

Some events expect a boolean return value. If a value is not returned for these events, the return value is assumed to be true. Returning false from any event listener will prevent any subsequent listeners on that event from being called.

Note that although this API is designed to mimic the standard DOM event binding mechanism as much as possible, the event parameter is not a true event, and features such as capturing or bubbling are not supported.

    addEventListeners(listeners);
    
This method allows you to add several event listeners at once. The listeners argument is an object containing listener functions keyed by event type. Note that it is not possible to bind multiple functions to a single event type with a single call.

    removeEventListener(type, listener);
    
This method unbinds the specified event listener function from the specified event.
    
    setPage(page, [animated]);
    
This method will scroll to the specified page (0-indexed). The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).
    
    getPage(callback(page));
    
This method returns the current page (0-indexed). The page will be returned as the first argument to the callback function provided. The function itself returns no value.

    getPageCount(callback(count));
    
This method returns the total page count. The page count will be returned as the first argument to the callback function provided. The function itself returns no value.

    scrollToNextPage([animated]);
    
Scrolls to the next page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).

    scrollToPreviousPage([animated]);
    
Scrolls to the previous page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).

    setLeftBarButtonItems(items);
    setRightBarButtonItems(items);

These methods allow you to configure the toolbar items for the PDF viewer. The items should be supplied as an array. Each element in the array should be either a string representing a standard toolbar item (see Standard toolbar items below for a list of supported values) or an object with the format specified below.

You can optionally set these toolbar buttons using the setOption(s) functions, or the options parameter of the `present()` function by using the keys `leftBarButtonItems` and `rightBarButtonItems`.

    getLeftBarButtonItems(callback(items));
    getRightBarButtonItems(callback(items));

These methods retrieve the current left and right toolbar items arrays.

    toggleAnnotationToolbar();
    showAnnotationToolbar();
    hideAnnotationToolbar();

These methods control the display of the annotation toolbar.


Custom toolbar button format
----------------------------

Custom toolbar buttons should be specified as an object in the following format:

    {title: 'Hello', action: function() {
        //do something
    }};
    
You can optionally include the following values:

    style
    
A string with a value of 'bordered', 'plain', or 'done', indicating the button style. The default style is 'bordered'.

    tintColor
    
A css color value. This will control the toolbar button tint color.

    image
    landscapeImage
    
A path to an image to display in the toolbar button. If `landscapeImage` is omitted it is assumed to be the same as `image` This path should be relative to the www folder (absolute paths, or paths outside of the application resources are not supported).


Standard toolbar button names
-------------------------------

The following standard toolbar buttons are available:

    close
    outline
    search
    viewMode
    print
    openIn
    email
    annotation
    bookmark
    brightness
    activity
    additionalActions


Options
------------

The following document and controller options can be set using the setOption(s) methods, or the options parameter of the present method.

Document options

    title
    titleLoaded
    fileURL
    metadata
    UID
    pageCount

Document annotation options
  
    annotationSaveMode (disabled, externalFile, embedded, embeddedWithExternalFileAsFallback)
    annotationsEnabled
    canEmbedAnnotations
    defaultAnnotationUsername
    renderAnnotationTypes (None, Undefined, Link, Highlight, StrikeOut, Underline, Squiggly, FreeText, Ink, Square, Circle, Line, Text, Stamp, Caret, RichMedia, Screen, Widget, Sound, FileAttachment, Polygon, PolyLine, Popup, Watermark, TrapNet, 3D, Redact, All)
    
Document hints
  
    aspectRatioEqual
    
Password Protection and Security

    password
    allowsCopying
    
Parser options

    bookmarksEnabled
    pageLabelsEnabled
    
Page appearance

    pageBackgroundColor (this maps to the native backgroundColor property)

PDF Controller options
    
    editableAnnotationTypes (see online documentation)
    pageTransition (scrollPerPage, scrollContinuous, curl)
    viewMode (document, thumbnails)
    pageMode (single, double, automatic)
    scrollDirection (horizontal, vertical)
    linkAction (none, alertView, openSafari, inlineBrowser)
    thumbnailBarMode (none, scrobbleBar, scrollable)
    smartZoomEnabled
    scrollingEnabled
    viewLockEnabled
    rotationLockEnabled
    scrollOnTapPageEndEnabled
    scrollOnTapPageEndAnimationEnabled
    scrollOnTapPageEndMargin
    internalTapGesturesEnabled
    textSelectionEnabled
    imageSelectionEnabled
    passwordDialogEnabled
    useParentNavigationBar
    shouldRestoreNavigationBarStyle
    allowedMenuActions (search, define, wikipedia, speak, all)
 
User Interface options
    
    UserInterfaceVisible
    UserInterfaceViewMode (always, automatic, automaticNoFirstLastPage, never)
    UserInterfaceViewAnimation (none, fade, slide)
    toolbarEnabled
    allowToolbarTitleChange
    pageLabelEnabled
    documentLabelEnabled
    renderAnimationEnabled
    
Appearance options

    doublePageModeOnFirstPage
    zoomingSmallDocumentsEnabled
    pageCurlDirectionLeftToRight
    fitToWidthEnabled
    fixedVerticalPositionForFitToWidthEnabledMode
    clipToPageBoundaries
    minimumZoomScale
    maximumZoomScale
    pagePadding
    shadowEnabled
    tintColor
    shouldTintPopovers
    shouldTintAlertView
    backgroundColor
    navigationBarHidden
    annotationAnimationDuration
    createAnnotationMenuEnabled
    showAnnotationMenuAfterCreation
    
    
Events
-------------

The following events are supported by the PSPDFKitPlugin class

    shouldSetDocument
    willDisplayDocument
    didDisplayDocument
    willBeginDisplayingPageView
    didFinishRenderTaskForPageView
    didConfigurePageView
    didCleanupPageView
    didTapOnPageView (`return false;` to set an event as processed and disable default handling)
    didLongPressOnPageView (`return false;` to set an event as processed and disable default handling)
    shouldSelectText
    didSelectText
    willDismiss
    didDismiss
    shouldShowUserInterface
    didShowUserInterface
    shouldHideUserInterface
    didHideUserInterface
    flexibleToolbarContainerDidShow
    flexibleToolbarContainerDidHide

Annotation API
---------------

The methods below allows you to programmatically access, add, and remove annotations using the Instant JSON format: https://pspdfkit.com/guides/ios/current/importing-exporting/instant-json/

    // Apply Instant JSON Document payload - https://pspdfkit.com/guides/ios/current/importing-exporting/instant-json/#instant-document-json-api 
    // Can be used to add annotations, bookmarks, fill forms, etc.
    applyInstantJSON(jsonValue, [callback]);

    // Add a single annotation using an Instant JSON Annotation payload - https://pspdfkit.com/guides/ios/current/importing-exporting/instant-json/#instant-annotation-json-api
    addAnnotation(jsonAnnotation, [callback]);

    // Remove an annotation.
    removeAnnotation(jsonAnnotation, [callback]);
    
    // Get all the annotations at the specified page index by type.
    getAnnotations(pageIndex, type, [callback]);
    
    // Get all unsaved annotations.
    getAllUnsavedAnnotations([callback]);

    
Forms API
---------------

The following methods allow you to programmatically fill forms and get the value of a form field.

    // Sets the form field value by specifying its fully qualified name.
    setFormFieldValue(value, fullyQualifiedName, [callback]);
    
    // Gets the form field value by specifying its fully qualified name.
    getFormFieldValue(fullyQualifiedName, [callback]);
    
XFDF API
---------------

The following methods allow you to import and export from/to a given XFDF file.

    // Imports all annotations from the specified XFDF file to the current document.
    importXFDF(xfdfPath, [callback]);
	
    // Exports all annotations from the current document to the specified XFDF file path in '"~/Documents/" + xfdfPath'.
    exportXFDF(xfdfPath, [callback]);
    

Document Processing API
-----------------------

The following method allows you to process annotations (embed, remove, flatten, or print) and save the processed document to the given document path.

    // Processes the current document's annotations and saves the processed document at the specified path.
    processAnnotations(annotationChange, processedDocumentPath, [callback], [annotationType])
	

The `annotationChange` is a string parameter. Can be `flatten`, `remove`, `embed` or `print`.

The `processedDocumentPath` should be a string containing the file path (not URL) for the processed PDF document. Relative paths are assumed to be relative to the www directory (if the 
processed document path has a different base URL set, we will create a processed file in '"~/Documents/" + processedDocumentPath'). To specify a path inside the application documents or library directory, use a `~,` e.g. `"~/Documents/processedDocument.pdf"` or `"~/Library/Application Support/processedDocument.pdf"`. The `processedDocumentPath` cannot be null and must not be omitted.

The optional callback will be called when the PDF is processed.

The optional string `annotationType` argument. If omitted, we process `'All'` annotations. The annotation type can have one of the following values: None, Undefined, Link, Highlight, StrikeOut, Underline, Squiggly, FreeText, Ink, Square, Circle, Line, Text, Stamp, Caret, RichMedia, Screen, Widget, Sound, FileAttachment, Polygon, PolyLine, Popup, Watermark, TrapNet, 3D, Redact, All. 

License
------------

Copyright 2011-2019 PSPDFKit GmbH. All rights reserved.

THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY AUSTRIAN COPYRIGHT LAW
AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.

You need a commercial license to use this project. Contact sales@pspdfkit.com for details.

## Contributing
  
Please ensure [you signed our CLA](https://pspdfkit.com/guides/web/current/miscellaneous/contributing/) so we can accept your contributions.
