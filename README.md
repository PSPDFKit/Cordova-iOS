PSPDFKit-Cordova
================

Usage
-----------

The plugin is accessed via the PSPDFKit singleton. Here are some example calls:

    //show pdf with shadows disabled and a red background
    PSPDFKit.present('pdf/castles.pdf', {
        shadowEnabled: false,
        backgroundColor: 'red'
    });
    
    //show pdf with callback
    PSPDFKit.present('pdf/castles.pdf', function() {
        alert('pdf has appeared');
    });
    
    //scroll to page 1
    PSPDFKit.setPage(1, true);
    
    //get the page number
    PSPDFKit.getPage(function(page) {
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
    
    search(query, [animated]);
    
This method triggers a search for the specified query text. The optional animated argument determines if the search should be animated (if omitted, the search will not be animated).

    saveChangedAnnotations(callback(error));
    
This method saves any changed annotations in the current or last opened document. On success the error callback parameter will be null, on failure it will be an object containing an error domain, code, and (possibly) a description and reason.
    
    setOptions(options, [animated]);
    
This method can be used to set multiple document and view controller settings at once. The options set will be applied to the current document (if there is one) as well as all subsequently displayed documents. All currently supported values are listed below under Options. The optional animated argument determines if the property should be animated. Not all property changes can be animated, so if the property does not support animation the animated argument will be ignored.
    
    setOption(name, value, [animated]);
    
This method can be used to set a single document or view controller option. All currently supported values are listed below under Options. The optional animated argument determines if the property should be animated. Not all property changes can be animated, so if the property does not support animation the animated argument will be ignored.
    
    setPage(page, [animated]);
    
This method will scroll to the specified page (0-indexed). The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).
    
    getPage(callback(page));
    
This method returns the current page (0-indexed). The page will be returned as the first argument to the callback function provided. The function itself returns no value.
    
    getScreenPage(callback(page));
    
This method returns the current screen page (see online documentation for details). The page will be returned as the first argument to the callback function provided. The function itself returns no value.

    scrollToNextPage([animated]);
    
Scrolls to the next page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).

    scrollToPreviousPage([animated]);
    
Scrolls to the previous page. The optional animated argument determines if the scroll should be animated (if omitted, the scroll will not be animated).


Options
------------

Document options

    title
    metadata
    UID

Document annotation options
  
    annotationsEnabled
    canEmbedAnnotations
    defaultAnnotationUsername
    
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
 
HUD options
    
    HUDVisible
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