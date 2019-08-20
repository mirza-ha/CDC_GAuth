//
//  HOTPGenerator.m
//
//  Copyright 2011 Google Inc.
//
//  Licensed under the Apache License, Version 2.0 (the "License"); you may not
//  use this file except in compliance with the License.  You may obtain a copy
//  of the License at
//
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
//  WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
//  License for the specific language governing permissions and limitations under
//  the License.
//

#import "OTPGenerator.h"

#import <CommonCrypto/CommonHMAC.h>
#import <CommonCrypto/CommonDigest.h>

//#import "GTMDefines.h"

static NSUInteger kPinModTable[] = {
    0,
    10,
    100,
    1000,
    10000,
    100000,
    1000000,
    10000000,
    100000000,
};

NSString *const kOTPGeneratorSHA1Algorithm = @"SHA1";
NSString *const kOTPGeneratorSHA256Algorithm = @"SHA256";
NSString *const kOTPGeneratorSHA512Algorithm = @"SHA512";
NSString *const kOTPGeneratorSHAMD5Algorithm = @"MD5";

@interface OTPGenerator ()
@property (readwrite, nonatomic, copy) NSString *algorithm;
@property (readwrite, nonatomic, copy) NSData *secret;
@end

@implementation OTPGenerator

+ (NSString *)defaultAlgorithm {
    return kOTPGeneratorSHA1Algorithm;
}

+ (NSUInteger)defaultDigits {
    return 6;
}

@synthesize algorithm = algorithm_;
@synthesize secret = secret_;
@synthesize digits = digits_;


- (id)init {
    if ((self = [super init])) {
        NSString *algorithm = kOTPGeneratorSHA1Algorithm;
        NSString *secret = @"secret";
        NSUInteger digits = 6;
        
        //self.algorithm = algorithm;
        
        algorithm_ = [algorithm copy];
        secret_ = [secret copy];
        digits_ = digits;
        
        BOOL goodAlgorithm
        = ([algorithm isEqualToString:kOTPGeneratorSHA1Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHA256Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHA512Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHAMD5Algorithm]);
        if (!goodAlgorithm || digits_ > 8 || digits_ < 6 || !secret_) {
            //_GTMDevLog(@"Bad args digits(min 6, max 8): %d secret: %@ algorithm: %@",
            //           digits_, secret_, algorithm_);
            [self release];
            self = nil;
        }
    }
    return self;
}


/*- (id)init {
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (id)initWithSecret:(NSData *)secret
           algorithm:(NSString *)algorithm
              digits:(NSUInteger)digits {
    if ((self = [super init])) {
        algorithm_ = [algorithm copy];
        secret_ = [secret copy];
        digits_ = digits;
        
        BOOL goodAlgorithm
        = ([algorithm isEqualToString:kOTPGeneratorSHA1Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHA256Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHA512Algorithm] ||
           [algorithm isEqualToString:kOTPGeneratorSHAMD5Algorithm]);
        if (!goodAlgorithm || digits_ > 8 || digits_ < 6 || !secret_) {
            //_GTMDevLog(@"Bad args digits(min 6, max 8): %d secret: %@ algorithm: %@",
            //           digits_, secret_, algorithm_);
            [self release];
            self = nil;
        }
    }
    return self;
}*/

- (void)dealloc {
    self.algorithm = nil;
    self.secret = nil;
    [super dealloc];
}

- (void)assignSecret:(NSData *)secret
{
    self.secret = secret;
}

// Must be overriden by subclass.
- (NSString *)generateOTP {
    [self doesNotRecognizeSelector:_cmd];
    return nil;
}

- (NSString *)generateOTPForCounter:(uint64_t)counter {
    CCHmacAlgorithm alg;
    NSUInteger hashLength = 0;
    if ([algorithm_ isEqualToString:kOTPGeneratorSHA1Algorithm]) {
        alg = kCCHmacAlgSHA1;
        hashLength = CC_SHA1_DIGEST_LENGTH;
    } else if ([algorithm_ isEqualToString:kOTPGeneratorSHA256Algorithm]) {
        alg = kCCHmacAlgSHA256;
        hashLength = CC_SHA256_DIGEST_LENGTH;
    } else if ([algorithm_ isEqualToString:kOTPGeneratorSHA512Algorithm]) {
        alg = kCCHmacAlgSHA512;
        hashLength = CC_SHA512_DIGEST_LENGTH;
    } else if ([algorithm_ isEqualToString:kOTPGeneratorSHAMD5Algorithm]) {
        alg = kCCHmacAlgMD5;
        hashLength = CC_MD5_DIGEST_LENGTH;
    } else {
        //_GTMDevAssert(NO, @"Unknown algorithm");
        return nil;
    }
    
    NSMutableData *hash = [NSMutableData dataWithLength:hashLength];
    
    counter = NSSwapHostLongLongToBig(counter);
    NSData *counterData = [NSData dataWithBytes:&counter
                                         length:sizeof(counter)];
    
    NSUInteger capacity1 = counterData.length * 2;
    NSMutableString *sbuf1 = [NSMutableString stringWithCapacity:capacity1];
    const unsigned char *buf1 = counterData.bytes;
    NSInteger i1;
    for (i1=0; i1<counterData.length; ++i1) {
        [sbuf1 appendFormat:@"%02X", (NSUInteger)buf1[i1]];
    }
    
    
    NSUInteger capacity2 = secret_.length * 2;
    NSMutableString *sbuf2 = [NSMutableString stringWithCapacity:capacity2];
    const unsigned char *buf2 = secret_.bytes;
    NSInteger i2;
    for (i2=0; i2<secret_.length; ++i2) {
        [sbuf2 appendFormat:@"%02X", (NSUInteger)buf2[i2]];
    }
    
    
    CCHmacContext ctx;
    CCHmacInit(&ctx, alg, [secret_ bytes], [secret_ length]);
    CCHmacUpdate(&ctx, [counterData bytes], [counterData length]);
    CCHmacFinal(&ctx, [hash mutableBytes]);
    
    NSUInteger capacity3 = hash.length * 2;
    NSMutableString *sbuf3 = [NSMutableString stringWithCapacity:capacity3];
    const unsigned char *buf3 = hash.bytes;
    NSInteger i3;
    for (i3=0; i3<hash.length; ++i3) {
        [sbuf3 appendFormat:@"%02X", (NSUInteger)buf3[i3]];
    }
    
    
    const char *ptr = [hash bytes];
    unsigned char offset1 = ptr[hashLength-1];
    unsigned int offset = ptr[hashLength-1] & 0x0f;
    unsigned int truncHash1 = *((uint32_t *)&ptr[offset]);
    unsigned int truncHash2 = (truncHash1 & 0x7fffffff);
    unsigned int truncHash3 = NSSwapBigIntToHost(*((uint32_t *)&ptr[offset]));
    uint32_t truncatedHash =
    NSSwapBigIntToHost(*((uint32_t *)&ptr[offset])) & 0x7fffffff;
    uint32_t pinValue = truncatedHash % kPinModTable[digits_];
    
    /*_GTMDevLog(@"secret: %@", secret_);
    _GTMDevLog(@"counter: %llu", counter);
    _GTMDevLog(@"hash: %@", hash);
    _GTMDevLog(@"offset: %d", offset);
    _GTMDevLog(@"truncatedHash: %d", truncatedHash);
    _GTMDevLog(@"pinValue: %d", pinValue);*/
    
    return [NSString stringWithFormat:@"%0*u", (int)digits_, pinValue];
}

@end
