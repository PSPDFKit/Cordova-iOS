//
//  PSPDFKit.m
//  PSPDFPlugin for Apache Cordova
//
//  Copyright 2013 PSPDFKit GmbH. All rights reserved.
//
//  THIS SOURCE CODE AND ANY ACCOMPANYING DOCUMENTATION ARE PROTECTED BY AUSTRIAN COPYRIGHT LAW
//  AND MAY NOT BE RESOLD OR REDISTRIBUTED. USAGE IS BOUND TO THE PSPDFKIT LICENSE AGREEMENT.
//  UNAUTHORIZED REPRODUCTION OR DISTRIBUTION IS SUBJECT TO CIVIL AND CRIMINAL PENALTIES.
//  This notice may not be removed from this file.
//

#import "PSPDFKit.h"
#import <PSPDFKit/PSPDFKit.h>
#import <objc/message.h>

@interface PSPDFKit () <PSPDFViewControllerDelegate>

@property (nonatomic, strong) UINavigationController *navigationController;
@property (nonatomic, strong) PSPDFViewController *pdfController;
@property (nonatomic, strong) NSDictionary *defaultOptions;

@end

@implementation PSPDFKit


#pragma mark Private methods

- (NSDictionary *)defaultOptions
{
    //this is an opportunity to provide
    //default options if we so choose
    if (!_defaultOptions)
    {
        _defaultOptions = @{};
    }
    return _defaultOptions;
}

- (void)setOptionsWithDictionary:(NSDictionary *)options animated:(BOOL)animated
{
    //merge with defaults
    NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
    [newOptions addEntriesFromDictionary:options];
    self.defaultOptions = newOptions;
    
    //set document and controller values
    [self setOptions:options forObject:_pdfController.document animated:animated];
    [self setOptions:options forObject:_pdfController animated:animated];
}

- (void)setOptions:(NSDictionary *)options forObject:(id)object animated:(BOOL)animated
{
    //merge with defaults
    NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
    [newOptions addEntriesFromDictionary:options];
    
    for (NSString *key in newOptions)
    {
        NSString *setterString = [NSString stringWithFormat:@"set%@%@:", [[key substringToIndex:1] uppercaseString], [key substringFromIndex:1]];
        
        SEL setter = NSSelectorFromString([setterString stringByAppendingString:@"forObject:animated:"]);
        if ([self respondsToSelector:setter])
        {
            //use custom setter
            objc_msgSend(self, setter, options[key], object, @(animated));
        }
        else
        {
            //use KVC
            setter = NSSelectorFromString(setterString);
            if ([object respondsToSelector:setter])
            {
                [object setValue:options[key] forKey:key];
            }
        }
    }
}

- (NSDictionary *)dictionaryWithError:(NSError *)error
{
    if (error)
    {
        NSMutableDictionary *dict = [NSMutableDictionary dictionary];
        dict[@"code"] = @(error.code);
        if (error.domain) dict[@"domain"] = error.domain;
        if ([error localizedDescription]) dict[@"description"] = [error localizedDescription];
        if ([error localizedFailureReason]) dict[@"reason"] = [error localizedFailureReason];
        return dict;
    }
    return nil;
}

- (UIColor *)colorWithString:(NSString *)string
{
    //TODO: should we support all the standard css color names here?
    static NSDictionary *colors = nil;
    if (colors == nil)
    {
        colors = [[NSDictionary alloc] initWithObjectsAndKeys:
                  [UIColor blackColor], @"black", // 0.0 white
                  [UIColor darkGrayColor], @"darkgray", // 0.333 white
                  [UIColor lightGrayColor], @"lightgray", // 0.667 white
                  [UIColor whiteColor], @"white", // 1.0 white
                  [UIColor grayColor], @"gray", // 0.5 white
                  [UIColor redColor], @"red", // 1.0, 0.0, 0.0 RGB
                  [UIColor greenColor], @"green", // 0.0, 1.0, 0.0 RGB
                  [UIColor blueColor], @"blue", // 0.0, 0.0, 1.0 RGB
                  [UIColor cyanColor], @"cyan", // 0.0, 1.0, 1.0 RGB
                  [UIColor yellowColor], @"yellow", // 1.0, 1.0, 0.0 RGB
                  [UIColor magentaColor], @"magenta", // 1.0, 0.0, 1.0 RGB
                  [UIColor orangeColor], @"orange", // 1.0, 0.5, 0.0 RGB
                  [UIColor purpleColor], @"purple", // 0.5, 0.0, 0.5 RGB
                  [UIColor brownColor], @"brown", // 0.6, 0.4, 0.2 RGB
                  [UIColor clearColor], @"clear", // 0.0 white, 0.0 alpha
                  nil];
    }

    //convert to lowercase
    string = [string lowercaseString];

    //try standard colors first
    UIColor *color = colors[string];
    if (color) return color;
    
    //try rgb(a)
    if ([string hasPrefix:@"rgb"])
    {
        string = [string substringToIndex:[string length] - 1];
        if ([string hasPrefix:@"rgb("])
        {
            string = [string substringFromIndex:4];
        }
        else if ([string hasPrefix:@"rgba("])
        {
            string = [string substringFromIndex:5];
        }
        CGFloat alpha = 1.0f;
        NSArray *components = [string componentsSeparatedByString:@","];
        if ([components count] > 3)
        {
            alpha = [[components[3] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] floatValue];
        }
        if ([components count] > 2)
        {
            NSString *red = [components[0] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
            NSString *green = [components[1] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
            NSString *blue = [components[2] stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
            return [UIColor colorWithRed:[red floatValue] / 255.0f
                                   green:[green floatValue] / 255.0f
                                    blue:[blue floatValue] / 255.0f
                                   alpha:alpha];
        }
        return nil;
    }
    
    //try hex
    string = [string stringByReplacingOccurrencesOfString:@"#" withString:@""];
    switch ([string length])
    {
        case 0:
        {
            string = @"00000000";
            break;
        }
        case 3:
        {
            NSString *red = [string substringWithRange:NSMakeRange(0, 1)];
            NSString *green = [string substringWithRange:NSMakeRange(1, 1)];
            NSString *blue = [string substringWithRange:NSMakeRange(2, 1)];
            string = [NSString stringWithFormat:@"%1$@%1$@%2$@%2$@%3$@%3$@ff", red, green, blue];
            break;
        }
        case 6:
        {
            string = [string stringByAppendingString:@"ff"];
            break;
        }
        default:
        {
            return nil;
        }
    }
    uint32_t rgba;
    NSScanner *scanner = [NSScanner scannerWithString:string];
    [scanner scanHexInt:&rgba];
    CGFloat red = ((rgba & 0xFF000000) >> 24) / 255.0f;
    CGFloat green = ((rgba & 0x00FF0000) >> 16) / 255.0f;
	CGFloat blue = ((rgba & 0x0000FF00) >> 8) / 255.0f;
	return [UIColor colorWithRed:red green:green blue:blue alpha:1.0f];
}

- (BOOL)sendEventWithJSON:(id)JSON
{
    if ([JSON isKindOfClass:[NSDictionary class]])
    {
        JSON = [[NSString alloc] initWithData:[NSJSONSerialization dataWithJSONObject:JSON options:0 error:NULL] encoding:NSUTF8StringEncoding];
    }
    NSString *script = [NSString stringWithFormat:@"PSPDFKit.dispatchEvent(%@)", JSON];
    NSString *result = [self.webView stringByEvaluatingJavaScriptFromString:script];
    return [result length]? [result boolValue]: YES;
}

- (PSPDFBarButtonItem *)standardBarButtonWithName:(NSString *)name
{
    NSString *selectorString = [name stringByAppendingString:@"ButtonItem"];
    if ([_pdfController respondsToSelector:NSSelectorFromString(selectorString)])
    {
        return [_pdfController valueForKey:selectorString];
    }
    return nil;
}

- (UIBarButtonItem *)barButtonItemWithJSON:(id)JSON
{
    if ([JSON isKindOfClass:[NSString class]])
    {
        return [self standardBarButtonWithName:JSON];
    }
    else if ([JSON isKindOfClass:[NSDictionary class]])
    {
        UIImage *image = nil;
        NSString *imagePath = JSON[@"image"];
        if (imagePath)
        {
            imagePath = [@"www" stringByAppendingPathComponent:imagePath];
            image = [UIImage imageNamed:imagePath];
        }
        
        UIImage *landscapeImage = image;
        imagePath = JSON[@"landscapeImage"];
        if (imagePath)
        {
            imagePath = [@"www" stringByAppendingPathComponent:imagePath];
            landscapeImage = [UIImage imageNamed:imagePath] ?: landscapeImage;
        }
        
        UIBarButtonItemStyle style = [self enumValueForKey:JSON[@"style"]
                                              inDictionary:@{
                                      @"bordered": @(UIBarButtonItemStyleBordered),
                                      @"plain": @(UIBarButtonItemStylePlain),
                                      @"done": @(UIBarButtonItemStyleDone)}
                                               withDefault:UIBarButtonItemStyleBordered];
        
        UIBarButtonItem *item = nil;
        if (image)
        {
            item = [[UIBarButtonItem alloc] initWithImage:image landscapeImagePhone:landscapeImage style:style target:self action:@selector(customBarButtonItemAction:)];
        }
        else
        {
            item = [[PSPDFBarButtonItem alloc] initWithTitle:JSON[@"title"] style:style target:self action:@selector(customBarButtonItemAction:)];
            [(PSPDFBarButtonItem *)item setPdfController:_pdfController];
        }

        item.tintColor = JSON[@"tintColor"]? [self colorWithString:JSON[@"tintColor"]]: item.tintColor;
        return item;
    }
    return nil;
}

- (NSArray *)barButtonItemsWithJSON:(NSArray *)JSONArray
{
    NSMutableArray *items = [NSMutableArray array];
    for (id JSON in JSONArray)
    {
        UIBarButtonItem *item = [self barButtonItemWithJSON:JSON];
        if (item)
        {
            [items addObject:item];
        }
        else
        {
            NSLog(@"Unrecognised toolbar button name or format");
        }
    }
    return items;
}

- (void)customBarButtonItemAction:(PSPDFBarButtonItem *)sender
{
    NSInteger index = [_pdfController.leftBarButtonItems indexOfObject:sender];
    if (index == NSNotFound)
    {
        index = [_pdfController.rightBarButtonItems indexOfObject:sender];
        if (index != NSNotFound)
        {
            NSString *script = [NSString stringWithFormat:@"PSPDFKit.dispatchRightBarButtonAction(%i)", index];
            [self.webView stringByEvaluatingJavaScriptFromString:script];
        }
    }
    else
    {
        NSString *script = [NSString stringWithFormat:@"PSPDFKit.dispatchLeftBarButtonAction(%i)", index];
        [self.webView stringByEvaluatingJavaScriptFromString:script];
    }
}

- (NSInteger)enumValueForKey:(NSString *)key inDictionary:(NSDictionary *)dict withDefault:(int)defaultValue
{
    NSNumber *number = dict[key];
    if (number) return [number integerValue];
    return defaultValue;
}

#pragma mark Special-case setters

//TODO: these would work much better as category methods on the respective objects

- (void)setHUDVisible:(NSNumber *)visible forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        [_pdfController setHUDVisible:[visible boolValue] animated:[animated boolValue]];
    }
}

- (void)setPage:(NSNumber *)page forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        [_pdfController setPage:[page integerValue] animated:[animated boolValue]];
    }
}

- (void)setLeftBarButtonItems:(NSArray *)items forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        _pdfController.leftBarButtonItems = [self barButtonItemsWithJSON:items] ?: _pdfController.leftBarButtonItems;
    }
}

- (void)setRightBarButtonItems:(NSArray *)items forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        _pdfController.rightBarButtonItems = [self barButtonItemsWithJSON:items] ?: _pdfController.rightBarButtonItems;
    }
}

//TODO: use introspection to automatically detect and set color properties

- (void)setTintColor:(NSString *)color forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        [_pdfController setTintColor:[self colorWithString:color]];
    }
}

- (void)setBackgroundColor:(NSString *)color forObject:(id)object animated:(NSNumber *)animated
{
    if (object == _pdfController)
    {
        [_pdfController setBackgroundColor:[self colorWithString:color]];
    }
    else
    {
        //do nothing - we don't support setting document color in this way because
        //it would conflict with the view controller background color.
        //use pageBackgroundColor instead
    }
}

- (void)setPageBackgroundColor:(NSString *)color forObject:(id)object animated:(NSNumber *)animated
{
    if ([object isKindOfClass:[PSPDFDocument class]])
    {
        [(PSPDFDocument *)object setBackgroundColor:[self colorWithString:color]];
    }
}

#pragma mark Document methods

- (void)present:(CDVInvokedUrlCommand *)command
{
    NSString *path = [command argumentAtIndex:0];
    NSDictionary *options = [command argumentAtIndex:1] ?: [command argumentAtIndex:2];
    
    //merge options with defaults
    NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
    [newOptions addEntriesFromDictionary:options];
    
    PSPDFDocument *document = nil;
    if (path)
    {
        //convert to absolute path
        path = [path stringByExpandingTildeInPath];
        if (![path isAbsolutePath])
        {
            path = [[NSBundle mainBundle] pathForResource:path ofType:nil inDirectory:@"www"];
        }
             
        //configure document
        NSURL *url = [NSURL fileURLWithPath:path];
        document = [PSPDFDocument documentWithURL:url];
        [self setOptions:newOptions forObject:document animated:NO];
    }
        
    //configure controller
    if (!_pdfController)
    {
        _pdfController = [[PSPDFViewController alloc] init];
        _pdfController.delegate = self;
        _navigationController = [[UINavigationController alloc] initWithRootViewController:_pdfController];
    }
    [self setOptions:newOptions forObject:_pdfController animated:NO];
    _pdfController.document = document;
    
    //present controller
    if (!_navigationController.presentingViewController)
    {
        [self.viewController presentViewController:_navigationController animated:YES completion:^{
            
            [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                        callbackId:command.callbackId];
        }];
    }
    else
    {
        [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                    callbackId:command.callbackId];
    }
}

- (void)dismiss:(CDVInvokedUrlCommand *)command
{
    [_navigationController.presentingViewController dismissViewControllerAnimated:YES completion:^{
        
        [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                    callbackId:command.callbackId];
    }];
}

- (void)reload:(CDVInvokedUrlCommand *)command
{
    [_pdfController reloadData];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

- (void)search:(CDVInvokedUrlCommand *)command
{
    CDVPluginResult *pluginResult = nil;
    NSString *query = [command argumentAtIndex:0];
    BOOL animated = [[command argumentAtIndex:1 withDefault:@NO] boolValue];
    
    if (query)
    {
        [_pdfController searchForString:query animated:animated];
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    }
    else
    {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR
                                         messageAsString:@"'query' argument was null"];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:command.callbackId];
}

- (void)saveChangedAnnotations:(CDVInvokedUrlCommand *)command
{
    NSError *error = nil;
    [_pdfController.document saveChangedAnnotationsWithError:&error];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsDictionary:[self dictionaryWithError:error]] callbackId:command.callbackId];
}

#pragma mark Configuration

- (void)setOptions:(CDVInvokedUrlCommand *)command
{
    NSDictionary *options = [command argumentAtIndex:0];
    BOOL animated = [[command argumentAtIndex:1 withDefault:@NO] boolValue];
    [self setOptionsWithDictionary:options animated:animated];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

- (void)setOption:(CDVInvokedUrlCommand *)command
{
    CDVPluginResult *pluginResult = nil;
    NSString *key = [command argumentAtIndex:0];
    id value = [command argumentAtIndex:1];
    BOOL animated = [[command argumentAtIndex:2 withDefault:@NO] boolValue];
    
    if (key && value)
    {
        [self setOptionsWithDictionary:@{key: value} animated:animated];
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    }
    else
    {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR
                                         messageAsString:@"'key' and/or 'value' argument was null"];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:command.callbackId];
}

#pragma mark Paging

- (void)setPage:(CDVInvokedUrlCommand *)command
{
    NSInteger page = [[command argumentAtIndex:0 withDefault:@(NSNotFound)] integerValue];
    BOOL animated = [[command argumentAtIndex:1 withDefault:@NO] boolValue];
    
    if (page != NSNotFound)
    {
        [_pdfController setPage:page animated:animated];
        [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                    callbackId:command.callbackId];
    }
    else
    {
        [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR messageAsString:@"'page' argument was null"] callbackId:command.callbackId];
    }
}

- (void)getPage:(CDVInvokedUrlCommand *)command
{
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsInt:_pdfController.page] callbackId:command.callbackId];
}

- (void)getScreenPage:(CDVInvokedUrlCommand *)command
{
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsInt:_pdfController.screenPage] callbackId:command.callbackId];
}

- (void)scrollToNextPage:(CDVInvokedUrlCommand *)command
{
    BOOL animated = [[command argumentAtIndex:0 withDefault:@NO] boolValue];
    [_pdfController scrollToNextPageAnimated:animated];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

- (void)scrollToPreviousPage:(CDVInvokedUrlCommand *)command
{
    BOOL animated = [[command argumentAtIndex:0 withDefault:@NO] boolValue];
    [_pdfController scrollToPreviousPageAnimated:animated];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

#pragma mark Toolbar Items

- (void)setLeftBarButtonItems:(CDVInvokedUrlCommand *)command
{
    NSArray *items = [command argumentAtIndex:0 withDefault:@[]];
    [self setOptionsWithDictionary:@{@"leftBarButtonItems": items} animated:NO];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

- (void)setRightBarButtonItems:(CDVInvokedUrlCommand *)command
{
    NSArray *items = [command argumentAtIndex:0 withDefault:@[]];
    [self setOptionsWithDictionary:@{@"rightBarButtonItems": items} animated:NO];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

#pragma mark Delegate methods

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldSetDocument:(PSPDFDocument *)document
{
    return [self sendEventWithJSON:@{@"type": @"shouldSetDocument", @"path": [document.fileURL path]?: [NSNull null]}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController willDisplayDocument:(PSPDFDocument *)document
{
    [self sendEventWithJSON:@{@"type": @"willDisplayDocument", @"path": [document.fileURL path]?: [NSNull null]}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didDisplayDocument:(PSPDFDocument *)document
{
    [self sendEventWithJSON:@{@"type": @"didDisplayDocument", @"path": [document.fileURL path]?: [NSNull null]}];
}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldScrollToPage:(NSUInteger)page
{
    return [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'shouldScrollToPage',page:%i}", page]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didShowPageView:(PSPDFPageView *)pageView
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didShowPageView',page:%i}", pageView.page]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didRenderPageView:(PSPDFPageView *)pageView
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didRenderPageView',page:%i}", pageView.page]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didLoadPageView:(PSPDFPageView *)pageView
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didLoadPageView',page:%i}", pageView.page]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController willUnloadPageView:(PSPDFPageView *)pageView
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'willUnloadPageView',page:%i}", pageView.page]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didBeginPageDragging:(UIScrollView *)scrollView
{
    [self sendEventWithJSON:@"{type:'didBeginPageDragging'}"];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate withVelocity:(CGPoint)velocity targetContentOffset:(inout CGPoint *)targetContentOffset
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didBeginPageDragging',willDecelerate:%@,velocity:{%g,%g}}", decelerate? @"true": @"false", velocity.x, velocity.y]];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageScrollingAnimation:(UIScrollView *)scrollView
{
    [self sendEventWithJSON:@"{type:'didEndPageScrollingAnimation'}"];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didBeginPageZooming:(UIScrollView *)scrollView
{
    [self sendEventWithJSON:@"{type:'didBeginPageZooming'}"];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didEndPageZooming:(UIScrollView *)scrollView atScale:(CGFloat)scale
{
    [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didEndPageZooming',scale:%g}", scale]];
}

//- (PSPDFDocument *)pdfViewController:(PSPDFViewController *)pdfController documentForRelativePath:(NSString *)relativePath
//{
//    
//}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController didTapOnPageView:(PSPDFPageView *)pageView atPoint:(CGPoint)viewPoint
{
    return [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didTapOnPageView',viewPoint:{%g,%g}}", viewPoint.x, viewPoint.y]];
}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController didLongPressOnPageView:(PSPDFPageView *)pageView atPoint:(CGPoint)viewPoint gestureRecognizer:(UILongPressGestureRecognizer *)gestureRecognizer
{
    return [self sendEventWithJSON:[NSString stringWithFormat:@"{type:'didLongPressOnPageView',viewPoint:{%g,%g}}", viewPoint.x, viewPoint.y]];
}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldSelectText:(NSString *)text withGlyphs:(NSArray *)glyphs atRect:(CGRect)rect onPageView:(PSPDFPageView *)pageView
{
    return [self sendEventWithJSON:@{@"type": @"shouldSelectText", @"text": text, @"rect": [NSString stringWithFormat:@"{%g,%g,%g,%g}", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height]}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didSelectText:(NSString *)text withGlyphs:(NSArray *)glyphs atRect:(CGRect)rect onPageView:(PSPDFPageView *)pageView
{
    [self sendEventWithJSON:@{@"type": @"didSelectText", @"text": text, @"rect": [NSString stringWithFormat:@"{%g,%g,%g,%g}", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height]}];
}

//- (NSArray *)pdfViewController:(PSPDFViewController *)pdfController shouldShowMenuItems:(NSArray *)menuItems atSuggestedTargetRect:(CGRect)rect forSelectedText:(NSString *)selectedText inRect:(CGRect)textRect onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (NSArray *)pdfViewController:(PSPDFViewController *)pdfController shouldShowMenuItems:(NSArray *)menuItems atSuggestedTargetRect:(CGRect)rect forSelectedImage:(PSPDFImageInfo *)selectedImage inRect:(CGRect)textRect onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (NSArray *)pdfViewController:(PSPDFViewController *)pdfController shouldShowMenuItems:(NSArray *)menuItems atSuggestedTargetRect:(CGRect)rect forAnnotation:(PSPDFAnnotation *)annotation inRect:(CGRect)annotationRect onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldDisplayAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (BOOL)pdfViewController:(PSPDFViewController *)pdfController didTapOnAnnotation:(PSPDFAnnotation *)annotation annotationPoint:(CGPoint)annotationPoint annotationView:(UIView <PSPDFAnnotationViewProtocol> *)annotationView pageView:(PSPDFPageView *)pageView viewPoint:(CGPoint)viewPoint
//{
//    
//}

//- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldSelectAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController didSelectAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (UIView <PSPDFAnnotationViewProtocol> *)pdfViewController:(PSPDFViewController *)pdfController annotationView:(UIView <PSPDFAnnotationViewProtocol> *)annotationView forAnnotation:(PSPDFAnnotation *)annotation onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController willShowAnnotationView:(UIView <PSPDFAnnotationViewProtocol> *)annotationView onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController didShowAnnotationView:(UIView <PSPDFAnnotationViewProtocol> *)annotationView onPageView:(PSPDFPageView *)pageView
//{
//    
//}

//- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldShowController:(id)viewController embeddedInController:(id)controller animated:(BOOL)animated
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController didShowController:(id)viewController embeddedInController:(id)controller animated:(BOOL)animated
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController requestsUpdateForBarButtonItem:(UIBarButtonItem *)barButtonItem animated:(BOOL)animated
//{
//    
//}

//- (void)pdfViewController:(PSPDFViewController *)pdfController didChangeViewMode:(PSPDFViewMode)viewMode
//{
//    
//}

- (void)pdfViewControllerWillDismiss:(PSPDFViewController *)pdfController
{
    [self sendEventWithJSON:@"{type:'willDismiss'}"];
}

- (void)pdfViewControllerDidDismiss:(PSPDFViewController *)pdfController
{
    //release the pdf document and controller
    _pdfController = nil;
    _navigationController = nil;
    
    //send event
    [self sendEventWithJSON:@"{type:'didDismiss'}"];
}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldShowHUD:(BOOL)animated
{
    return [self sendEventWithJSON:@{@"type": @"shouldShowHUD", @"animated": @(animated)}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController willShowHUD:(BOOL)animated
{
    [self sendEventWithJSON:@{@"type": @"willShowHUD", @"animated": @(animated)}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didShowHUD:(BOOL)animated
{
    [self sendEventWithJSON:@{@"type": @"didShowHUD", @"animated": @(animated)}];
}

- (BOOL)pdfViewController:(PSPDFViewController *)pdfController shouldHideHUD:(BOOL)animated
{
    return [self sendEventWithJSON:@{@"type": @"shouldHideHUD", @"animated": @(animated)}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController willHideHUD:(BOOL)animated
{
    [self sendEventWithJSON:@{@"type": @"willHideHUD", @"animated": @(animated)}];
}

- (void)pdfViewController:(PSPDFViewController *)pdfController didHideHUD:(BOOL)animated
{
    [self sendEventWithJSON:@{@"type": @"didHideHUD", @"animated": @(animated)}];
}

@end
