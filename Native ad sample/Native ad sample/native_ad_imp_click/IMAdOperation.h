//
//  InMobiAdOperation.h
//  Native ad sample
//
//

#import <Foundation/Foundation.h>
#import "IMNativeAdData.h"
#import <UIKit/UIKit.h>
#import "IMConstants.h"

/**
 * This subclass of NSOperation is used by the OperationQueue to fire the impression/click pixels for 
 * inmobi native ads.
 */
@class IMAdOperation;
@protocol IMAdOperationDelegate <NSObject>
@required

- (void)adOperationFinishedSuccessfully:(IMAdOperation *)operation;
- (void)adOperation:(IMAdOperation *)operation failedWithError:(NSError *)error;
@end

@class IMWebViewWrapper;
@interface IMAdOperation : NSOperation <UIWebViewDelegate>

/*
 * This object contains the necessary information - contextCode,ns,etc.
 */
@property(nonatomic,retain) IMNativeAdData *data;
/*
 * Webview weak reference, used to trigger javascript
 */
@property(nonatomic,weak) IMWebViewWrapper *webviewWrapper;
/*
 * The operation type - impression or clicks
 */
@property(nonatomic,assign) IMAdOperationType operationType;
/*
 * The UIBackgroundTaskIdentifier associated with this object. Must be set to invalid
 * once the operation gets completed.
 */
@property(nonatomic,assign) UIBackgroundTaskIdentifier bgTask;
/*
 * The delegate associated with this object, to notify if operation was completed successfully.
 */
@property(nonatomic,assign) id<IMAdOperationDelegate> delegate;
/*
 * The constructor, used to instantiate this operation with the required information.
 * w The Webview
 * d native ad metadata
 * type operation type - impression or click
 * del The delegate object, to provide lifecycle callbacks
 * identifier The bgIdentifier, associated with object. Must be invalidated after operation has finished
 */
- (id)initWithWebView:(IMWebViewWrapper *)wrapper adData:(IMNativeAdData *)d
        operationType:(IMAdOperationType)type delegate:(id<IMAdOperationDelegate>)del
                        bgTask:(UIBackgroundTaskIdentifier)identifier;

@end

/**
 * This class is a wrapper around UIWebView.
 * The purpose of wrapping around is to keep a check on which UIWebView is currently executing javascript,
 * and which UIWebView is available for use.
 * index parameter indicates which index is this object referenced from the webviewWrapperArray
 */
@interface IMWebViewWrapper : NSObject
/*
 * Indicates whether this webview is currently executing javascript or not.
 */
@property(atomic,assign) BOOL isExecuting;
/*
 * The UIWebView object, which will execute the contextCode
 */
@property(nonatomic,retain) UIWebView *webview;
/*
 * Index parameter indicates which index is this object referenced from the webviewWrapperArray
 */
@property(atomic,assign) int index;
/*
 * This method executes the javascript, and sets UIWebView's delegate to the provided IMAdOperation instance
 * js The javascript to be executed
 * o The IMAdOperation which handles this IMWebViewWrapper instance
 */
- (void)executeJS:(NSString *)js operation:(IMAdOperation *)o;

@end
