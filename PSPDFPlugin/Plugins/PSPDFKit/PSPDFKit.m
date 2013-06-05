//
//  PSPDFKit.m
//  PSPDFPlugin
//
//  Created by Nick Lockwood on 04/06/2013.
//
//

#import "PSPDFKit.h"
#import <PSPDFKit/PSPDFKit.h>


@interface PSPDFKit ()

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
        
        SEL setter = NSSelectorFromString([setterString stringByAppendingString:@"animated:"]);
        if ([self respondsToSelector:setter])
        {
            //use custom setter
            [self performSelector:setter withObject:options[key] withObject:@(animated)];
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

#pragma mark Special-case setters

- (void)setHUDVisible:(NSNumber *)visible animated:(NSNumber *)animated
{
    [_pdfController setHUDVisible:[visible boolValue] animated:[animated boolValue]];
}

- (void)setPage:(NSNumber *)page animated:(NSNumber *)animated
{
    [_pdfController setPage:[page integerValue] animated:[animated boolValue]];
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
    CDVPluginResult *pluginResult = nil;
    NSInteger page = [[command argumentAtIndex:0 withDefault:@(NSNotFound)] integerValue];
    BOOL animated = [[command argumentAtIndex:1 withDefault:@NO] boolValue];
    
    if (page != NSNotFound)
    {
        [_pdfController setPage:page animated:animated];
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    }
    else
    {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR
                                         messageAsString:@"'page' argument was null"];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:command.callbackId];
}

- (void)getPage:(CDVInvokedUrlCommand *)command
{
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsInt:_pdfController.page] callbackId:command.callbackId];
}

- (void)getScreenPage:(CDVInvokedUrlCommand *)command
{
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK messageAsInt:_pdfController.screenPage] callbackId:command.callbackId];
}

@end
