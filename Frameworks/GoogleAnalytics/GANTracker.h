/*!
 @header GANTracker.h

 Google Analytics iOS SDK
 @version 1.5

 @copyright Copyright 2009 Google Inc. All rights reserved.
*/

/*!
 @enum GANErrorCode
 Error codes which may be returned by various GANTracker methods.
 */
typedef enum {
  // This error code is returned when input to a method is incorrect.
  kGANTrackerInvalidInputError = 0xbade7a9,

  // This error code is returned when the number of hits generated in a session
  // exceeds the limit (currently 500).
  kGANTrackerEventsPerSessionLimitError = 0xbad5704e,

  // This error code is returned if the method called requires that the tracker
  // be started.
  kGANTrackerNotStartedError = 0xbada55,

  // This error code is returned if the method call resulted in some sort of
  // database error.
  kGANTrackerDatabaseError = 0xbadbaddb
} GANErrorCode;

/*!
 @var kGANTrackerErrorDomain
 @abstract The error domain for errors originating in the Google Analytics iOS
 SDK. All errors generated by the Google Analytics iOS SDK will belong to
 this domain.
 */
extern NSString* const kGANTrackerErrorDomain;

/*!
 @var kGANMaxCustomVariables
 @abstract The maximum number of custom variables the SDK can support. Custom
 variable indices must be between 1 and this value, inclusive.

 @discussion Standard accounts support up to 5 custom variables, while premium
 accounts support up to 50 custom variables. The SDK will accept custom
 variable indices higher than 5, but unless the account is a premium account,
 those custom variables will not be processed.
 */
extern const NSUInteger kGANMaxCustomVariables;

/*!
 @var kGANMaxCustomVariableLength
 @abstract The maximum length of a custom variable name or value. The length of
 custom variable names and values must not exceed this value.
 */
extern const NSUInteger kGANMaxCustomVariableLength;

/*!
 @enum GANCVScope

 @abstract Custom variable scope enumeration.

 @discussion Possible values are visitor scope, session scope, and page scope.
 Visitor and session scoped custom variables are stored for later use. Session
 and page scoped custom variables are attached to each event. Visitor scoped
 custom variables are sent only on the first event for a session.
 */
typedef enum {
  kGANVisitorScope = 1U,
  kGANSessionScope = 2U,
  kGANPageScope = 3U
} GANCVScope;

@class GANTrackerPrivate;
@protocol GANTrackerDelegate;

/*!
 @class GANTracker

 @abstract Google Analytics tracking class. Tracks pageviews, events, and
 transactions by storing them in a persistent store and dispatching them in the
 background to Google Analytics.
 */
@interface GANTracker : NSObject {
 @private
  GANTrackerPrivate *private_;
  BOOL debug_;
  BOOL dryRun_;
  BOOL anonymizeIp_;
  NSUInteger sampleRate_;
  NSString *private1_;
  NSString *private2_;
  NSString *destinationHost_;
  NSUInteger destinationPort_;
  NSString *destinationPathPrefix_;
}

/*!
 @property debug

 @abstract If true, debug messages will be written to the log.
 This is useful for debugging calls to the Google Analytics SDK.
 By default, this flag is false.
 */
@property(nonatomic, readwrite) BOOL debug;

/*!
 @property dryRun

 @abstract If true, hits will not be sent to Google Analytics.
 This is useful for debugging calls to the Google Analytics SDK.
 By default, this flag is false.
 */
@property(nonatomic, readwrite) BOOL dryRun;

/*!
 @property anonymizeIp

 @abstract If true, Google Analytics will anonymize the IP address information
 collected by zeroing out some of the least significant bits of the address. By
 default, this flag is false.

 @discussion In the case of IPv4 addresses, the last octet is set to zero. For
 IPv6 addresses, the last 10 octets are set to zero, although this is subject to
 change in the future.
 */
@property(nonatomic, readwrite) BOOL anonymizeIp;

/*!
 @property sampleRate

 @abstract The sampleRate parameter controls the probability that the visitor
 will be sampled. By default, sampleRate is 100, which signifies no sampling.
 sampleRate may be set to any integer value between 0 and 100, inclusive. A
 value of 90 requests 90% of visitors to be sampled (10% of visitors to be
 sampled out).

 @discussion When a visitor is not sampled, no data is submitted to Google
 Analytics about that visitor's activity. If your application is subject to
 heavy traffic spikes, you may wish to adjust the sample rate to ensure
 uninterrupted report tracking. Sampling in Google Analytics occurs consistently
 across unique visitors, ensuring integrity in trending and reporting even when
 sampling is enabled, because unique visitors remain included or excluded from
 the sample, as set from the initiation of sampling.
 */
@property(nonatomic, readwrite) NSUInteger sampleRate;

/*!
 @abstract Returns the singleton instance of GANTracker.
 */
+ (GANTracker *)sharedTracker;

/*!
 @abstract Start the Google Analytics tracker.

 @param accountID The Google Analytics account ID (the string that begins with
 "UA-").

 @param dispatchPeriod The desired automatic dispatch period (in seconds).
 The dispatcher will check for events to dispatch every |dispatchPeriod|
 seconds. If a non-positive (e.g. 0 or -1) dispatch period is given, automatic
 dispatch will not be enabled, and the application will need to dispatch events
 manually using @link dispatch @/link or @link dispatchSynchronous: @/link.

 @param delegate An optional delegate which will get notified when hits get
 dispatched.
 */
- (void)startTrackerWithAccountID:(NSString *)accountID
                   dispatchPeriod:(NSInteger)dispatchPeriod
                         delegate:(id<GANTrackerDelegate>)delegate;

/*!
 @abstract Stop the tracker.
 */
- (void)stopTracker;

/*!
 @abstract Track a page view.

 @return YES on success or NO on error.

 @param pageURL The URL of the pageview to track. A forward-slash ('/')
 character will be prepended to this string if it doesn't start with one.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)trackPageview:(NSString *)pageURL
            withError:(NSError **)error;

/*!
 @abstract Track an event view.

 @return YES on success or NO on error.

 @param category The event category. This parameter is required to be non-nil.

 @param action The event action. This parameter is required to be non-nil.

 @param label The event label. This parameter may be nil to indicate no label.

 @param value The event value. This parameter may be -1 to indicate no value.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)trackEvent:(NSString *)category
            action:(NSString *)action
             label:(NSString *)label
             value:(NSInteger)value
         withError:(NSError **)error;

/*!
 @abstract Set a custom variable. If there is an existing custom variable at the
 specified index, it will be overwritten.

 @return YES on success or NO on error.

 @param index The variable index. This must be between 1 and
 @link kGANMaxCustomVariables @/link, inclusive.

 @param name The custom variable name. The length of this string must not exceed
 @link kGANMaxCustomVariableLength @/link.

 @param value The custom variable value. The length of this string must not
 exceed @link kGANMaxCustomVariableLength @/link.

 @param scope The custom variable scope (see @link GANCVScope @/link for
 discussion of what this value means).

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)setCustomVariableAtIndex:(NSUInteger)index
                            name:(NSString *)name
                           value:(NSString *)value
                           scope:(GANCVScope)scope
                       withError:(NSError **)error;

/*!
 @abstract Set a page scoped custom variable. The variable set is included with
 the next even only. If there is an existing custom variable at the specified
 index, it will be overwritten by this one.

 @return YES on success or NO on error.

 @param index The variable index. This must be between 1 and
 @link kGANMaxCustomVariables @/link, inclusive.

 @param name The custom variable name. The length of this string must not exceed
 @link kGANMaxCustomVariableLength @/link.

 @param value The custom variable value. The length of this string must not
 exceed @link kGANMaxCustomVariableLength @/link.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)setCustomVariableAtIndex:(NSUInteger)index
                            name:(NSString *)name
                           value:(NSString *)value
                       withError:(NSError **)error;

/*!
 @abstract Get the value of the custom variable at the index requested.

 @return Returns the value of the custom variable at the specified index.
 Returns nil no variable is present at that index or the index is out of range.

 @param index The variable index. This must be between 1 and
 @link kGANMaxCustomVariables @/link, inclusive.
 */
- (NSString *)getVisitorCustomVarAtIndex:(NSUInteger)index;

/*!
 @abstract Create an e-commerce transaction.

 @discussion All transactions will be queued for dispatch when
 @link trackTransactions: @/link is called.

 @return YES on success or NO on error.

 @param orderID The transaction order ID. If a transaction with this order id is
 already present in the ECommerce buffer, it will be overwritten by this one.

 @param totalPrice The transaction total price.

 @param storeName The transaction store name.

 @param totalTax Total tax incurred in the transaction, if any.

 @param shippingCost Shipping costs incurred in the transaction, if any.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)addTransaction:(NSString *)orderID
            totalPrice:(double)totalPrice
             storeName:(NSString *)storeName
              totalTax:(double)totalTax
          shippingCost:(double)shippingCost
             withError:(NSError **)error;

/*!
 @abstract Add an item to an e-commerce transaction.

 @discussion Adds the specified item to the transaction with the specified order
 ID. All transactions will be queued for dispatch when
 @link trackTransactions: @/link is called.

 @return YES on success or NO on error.

 @param orderID The order ID of the transaction to which the item will be added.
 If no transaction with this order ID exists, one will be created.

 @param itemSKU The item SKU. If an item in the transaction with the same SKU
 exists, it will replaced with this one.

 @param itemPrice The item price.

 @param itemCount The item quantity.

 @param itemName The item name.

 @param itemCategory The item category.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)addItem:(NSString *)orderID
        itemSKU:(NSString *)itemSKU
      itemPrice:(double)itemPrice
      itemCount:(double)itemCount
       itemName:(NSString *)itemName
   itemCategory:(NSString *)itemCategory
      withError:(NSError **)error;

/*!
 @abstract Queue pending e-commerce transactions for dispatch.

 @return YES on success or NO on error.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)trackTransactions:(NSError **)error;

/*!
 @abstract Remove pending e-commerce transactions.

 @return YES on success or NO on error.

 @param error A location to record the error if one occurs. You may pass 'nil'
 here.
 */
- (BOOL)clearTransactions:(NSError **)error;

/*!
 @abstract Set the referrer and create a new session associated with the
 specified referrer.

 @return YES on success or NO on error.

 @param referrer A string specifying the campaign with which to associate this
 session. All events generated after setting the referrer will have the
 campaign attached.

 @param error A location to record the error if one occurs. You may pass nil
 here.
 */
- (BOOL)setReferrer:(NSString *)referrer
          withError:(NSError **)error;

/*!
 @abstract Dispatch pageviews, events, or transactions to Google Analytics.

 @return YES if there were pending pageviews, events, or transactions that
 resulted in a new dispatch; NO otherwise.
 */
- (BOOL)dispatch;

/*!
 @abstract Synchronously dispatch pageviews, events, or transactions to Google
 Analytics.

 @discussion This routine will block until all information has been submitted,
 or the specified timeout has elapsed.

 @param timeout The maximum time, in seconds, this method can spend submitting
 information to Google Analytics before it must return.

 @return YES if there were pending pageviews, events, or transactions that
 resulted in a new dispatch and all were successfully submitted to Google
 Analytics; NO otherwise.
 */
- (BOOL)dispatchSynchronous:(NSTimeInterval)timeout;

@end

/*!
 @protocol GANTrackerDelegate

 @abstract Google Analytics tracker delegate. This delegate gets notified when
 the tracker dispatches a hit successfully, or when there was an error
 dispatching the hit. All delegate methods are optional.
 */
@protocol GANTrackerDelegate <NSObject>

@optional

/*!
 @abstract This method is invoked when a hit has been successfully submitted to
 Google Analytics.

 @param hitString The string representation of the hit, as sent to Google
 Analytics.
 */
- (void)hitDispatched:(NSString *)hitString;

/*!
 @abstract This method is invoked when a dispatch to Google Analytics is
 completed.

 @discussion Note: failed hits will be retried on the next dispatch.

 @param tracker The tracker object for which the dispatch was made.

 @param hitsDispatched The number of hits successfully submitted to Google
 Analytics.

 @param hitsFailedDispatch The number of hits that could not be submitted.
 */
- (void)trackerDispatchDidComplete:(GANTracker *)tracker
                  eventsDispatched:(NSUInteger)hitsDispatched
              eventsFailedDispatch:(NSUInteger)hitsFailedDispatch;

@end
