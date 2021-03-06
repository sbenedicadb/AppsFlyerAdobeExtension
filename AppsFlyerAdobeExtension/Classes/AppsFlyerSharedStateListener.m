//
//  AppsFlyerAdobeShStListener.m
//  appsflyer-adobe-extension
//
//  Created by Benjamin Winestein on 05/08/2018.
//  Copyright © 2018 Benjamin Winestein. All rights reserved.
//

#import "AppsFlyerSharedStateListener.h"
#import <ACPCore_iOS/ACPExtensionEvent.h>

@implementation AppsFlyerSharedStateListener

- (nullable instancetype)init {
    self = [super init];
    if (self) {

        NSLog(@"com.appsflyer.adobeextension Shared State listener was initialized");
    }
    return self;
}

- (void)hear:(nonnull ACPExtensionEvent*)event {
    
    NSDictionary* jsonEventData = [event eventData];

    if (!jsonEventData) {
        NSLog(@"com.appsflyer.adobeextension Retrieved event data is nil");
    }
    
    if ([jsonEventData[@"stateowner"] isEqualToString:@"com.adobe.module.configuration"]) {
        NSError* error = nil;
        NSDictionary* configSharedState = [[[self extension] api] getSharedEventState:@"com.adobe.module.configuration" event:event error:&error];
        if (error) {
            NSLog(@"com.appsflyer.adobeextension Error retrieving shared state %@:%ld.", [error domain], [error code]);
            return;
        }
        
        if (configSharedState) {
            if ([configSharedState objectForKey:@"appsFlyerAppId"] && [configSharedState objectForKey:@"appsFlyerDevKey"]) {
                NSString* appsFlyerAppId = [configSharedState objectForKey:@"appsFlyerAppId"];
                NSString* appsFlyerDevKey = [configSharedState objectForKey:@"appsFlyerDevKey"];
                id appsFlyerIsDebug = [configSharedState objectForKey:@"appsFlyerIsDebug"];
                BOOL isDebug = [appsFlyerIsDebug isKindOfClass:[NSNumber class]] && [appsFlyerIsDebug integerValue] == 1;
                
                [[AppsFlyerAdobeExtension shared] setupAppsFlyerTrackingWithAppId:appsFlyerAppId
                                                                  appsFlyerDevKey:appsFlyerDevKey
                                                                          isDebug:isDebug];
                
                NSLog(@"com.appsflyer.adobeextension Received credentials for app: %@", [configSharedState objectForKey:@"appsFlyerAppId"]);
            } else {
                NSLog(@"com.appsflyer.adobeextension Cannot initilalise AppsFlyer tracking without an appId or devKey");
            }
        } else {
            NSLog(@"com.appsflyer.adobeextension Cannot initilalise AppsFlyer without settings");
        }
    }
}

@end
