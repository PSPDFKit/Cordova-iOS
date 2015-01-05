PSPDFKit-Cordova
================

Install
-----------

    cordova plugin add https://github.com/PSPDFKit/PSPDFKit-Cordova.git

Read the message at the end and complete installation (add PSPDFKit framework to your Xcode project manually).


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
    
Displays a PDF in a full-screen modal. The path should be a string containing the file path (not URL) for the PDF. Relative paths are assumed to be relative to the www directory (if the page has a different base URL set, this will be ignored). To specify a path inside the application documents or library directory, use a ~, e.g. "~/Documents/mypdf.pdf" or "~/Library/Application Support/mypdf.pdf". Path can be null, but must not be omitted

The options parameter is an optional object containing configuration properties for the PDF document and/or view controller. All currently supported values are listed below under Options.

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
    
    getOptionss(names, callback(values));

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
    
    getScreenPage(callback(page));
    
This method returns the current screen page (see online documentation for details). The page will be returned as the first argument to the callback function provided. The function itself returns no value.

    getPageCount(callback(count));
    
This method returns the total page count. The page count will be returned as the first argument to the callback function provided. The function itself returns no value.

    scrollToNextPage([animated]);
    
Scrolls to the next page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).

    scrollToPreviousPage([animated]);
    
Scrolls to the previous page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).

    setLeftBarButtonItems(items);
    setRightBarButtonItems(items);

These methods allw you to configure the toolbar items for the PDF viewer. The items should be supplied as an array. Each element in the array should be either a string representing a standard toolbar item (see Standard toolbar items below for a list of supported values) or an object with the forma specified below.

You can optionally set these toolbar buttons using the setOption(s) functions, or the options parameter of the `present()` function by using the keys `leftBarButtonItems` and `rightBarButtonItems`.

    getLeftBarButtonItems(callback(items));
    getRightBarButtonItems(callback(items));

These methods retrieve the current left and right toolbar items arrays.


Custom toolbar button format
----------------------------

Custom toolbar buttons should be specified as an object in the following format:

    {title: 'Hello', action: function() {
        //do something
    });
    
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
  
    editableAnnotationTypes (see online documentation)
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
 
HUD options
    
    HUDVisible
    HUDViewMode (always, automatic, automaticNoFirstLastPage, never)
    HUDViewAnimation (none, fade, slide)
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
    transparentHUD
    shouldHideNavigationBarWithHUD
    shouldHideStatusBarWithHUD
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
    shouldScrollToPage
    didShowPageView
    didRenderPageView
    didLoadPageView
    willUnloadPageView
    didBeginPageDragging
    didBeginPageDragging
    didEndPageScrollingAnimation
    didBeginPageZooming
    didEndPageZooming
    didTapOnPageView (`return false;` to set an event as processed and disable default handling)
    didLongPressOnPageView (`return false;` to set an event as processed and disable default handling)
    shouldSelectText
    didSelectText
    willDismiss
    didDismiss
    shouldShowHUD
    didShowHUD
    shouldHideHUD
    didHideHUD


License
------------

Copyright 2011-2015 PSPDFKit GmbH. All rights reserved.

THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY AUSTRIAN COPYRIGHT LAW
AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.

http://pspdfkit.com/license.html
