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

- (void)setOptionsWithDictionary:(NSDictionary *)options
{
    //merge with defaults
    NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
    [newOptions addEntriesFromDictionary:options];
    self.defaultOptions = newOptions;
    
    //set document and controller values
    [self setOptions:options forObject:_pdfController.document];
    [self setOptions:options forObject:_pdfController];
}

- (void)setOptions:(NSDictionary *)options forObject:(id)object
{
    //merge with defaults
    NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
    [newOptions addEntriesFromDictionary:options];
    
    for (NSString *key in newOptions)
    {
        SEL setter = NSSelectorFromString([NSString stringWithFormat:@"set%@%@:", [[key substringToIndex:1] uppercaseString], [key substringFromIndex:1]]);
        if ([object respondsToSelector:setter])
        {
            [object setValue:options[key] forKey:key];
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

#pragma mark Document methods

- (void)present:(CDVInvokedUrlCommand *)command
{
    CDVPluginResult *pluginResult = nil;
    NSString *path = [command argumentAtIndex:0 withDefault:nil];
    NSDictionary *options = [command argumentAtIndex:1 withDefault:nil];
    
    if (path)
    {
        //convert to absolute path
        path = [path stringByExpandingTildeInPath];
        if (![path isAbsolutePath])
        {
            path = [[NSBundle mainBundle] pathForResource:path ofType:nil inDirectory:@"www"];
        }
        
        //merge options with defaults
        NSMutableDictionary *newOptions = [self.defaultOptions mutableCopy];
        [newOptions addEntriesFromDictionary:options];
                
        //configure document
        NSURL *url = [NSURL fileURLWithPath:path];
        PSPDFDocument *document = [PSPDFDocument documentWithURL:url];
        [self setOptions:newOptions forObject:document];
        
        //configure controller
        if (!_pdfController)
        {
            _pdfController = [[PSPDFViewController alloc] init];
            _navigationController = [[UINavigationController alloc] initWithRootViewController:_pdfController];
        }
        [self setOptions:newOptions forObject:_pdfController];
        _pdfController.document = document;
        
        //present controller
        if (!_navigationController.presentingViewController)
        {
            [self.viewController presentViewController:_navigationController animated:YES completion:NULL];
        }

        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_OK];
    }
    else
    {
        pluginResult = [CDVPluginResult resultWithStatus:CDVCommandStatus_ERROR
                                         messageAsString:@"'path' argument was null"];
    }
    
    [self.commandDelegate sendPluginResult:pluginResult
                                callbackId:command.callbackId];
}

- (void)dismiss:(CDVInvokedUrlCommand *)command
{
    [_navigationController.presentingViewController dismissViewControllerAnimated:YES completion:NULL];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
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
    NSString *query = [command argumentAtIndex:0 withDefault:nil];
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
    NSDictionary *options = [command argumentAtIndex:0 withDefault:nil];
    [self setOptionsWithDictionary:options];
    [self.commandDelegate sendPluginResult:[CDVPluginResult resultWithStatus:CDVCommandStatus_OK]
                                callbackId:command.callbackId];
}

- (void)setOption:(CDVInvokedUrlCommand *)command
{
    CDVPluginResult *pluginResult = nil;
    NSString *key = [command argumentAtIndex:0 withDefault:nil];
    id value = [command argumentAtIndex:1 withDefault:nil];
    
    if (key && value)
    {
        [self setOptionsWithDictionary:@{key: value}];
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
