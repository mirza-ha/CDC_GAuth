//
//  FirstViewController.m
//  kdc.authenticator
//
//  Created by Mirza Hadžiomerović on 17/12/2018.
//  Copyright © 2018 Mirza Hadžiomerović. All rights reserved.
//  Codecta Sarajevo
//

#import "FirstViewController.h"
#import "OTPGenerator.h"
#import "GTMStringEncoding.h"

static NSString *const kBase32Charset = @"ABCDEFGHIJKLMNOPQRSTUVWXYZ234567";
static NSString *const kBase32Synonyms =
@"AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
static NSString *const kBase32Sep = @" -";

//static NSString *const kQuerySecretKey = @"secret";


@interface FirstViewController ()
@property (retain, nonatomic) IBOutlet UILabel *lbOTP;
@property (retain, nonatomic) IBOutlet UILabel *lbStatus;
@property (retain, nonatomic) IBOutlet UITextField *tfNote;

@end

@implementation FirstViewController

- (void)viewDidLoad {
    [super viewDidLoad];
        
    NSString *secretString = @"bmza2ucrfws43i6avncfs6k5hk6vuffm";
    
    NSData *secretData = [FirstViewController base32Decode:secretString];

    OTPGenerator *otpgenerator = [[OTPGenerator alloc] init];
    
    [otpgenerator assignSecret:secretData];
    
    NSDate *date = [NSDate date];

    NSTimeInterval seconds = [date timeIntervalSince1970];

    uint64_t counter = (uint64_t)(seconds / /*self.period*/ 30);
    NSString *OTP = [otpgenerator generateOTPForCounter:counter];
}



- (IBAction)btnOnClick:(id)sender {
    NSString *secretString = @"bmza2ucrfws43i6avncfs6k5hk6vuffm";
    
    NSData *secretData = [FirstViewController base32Decode:secretString];
    
    OTPGenerator *otpgenerator = [[OTPGenerator alloc] init];
    
    [otpgenerator assignSecret:secretData];
    
    NSDate *date = [NSDate date];
    
    NSTimeInterval seconds = [date timeIntervalSince1970];
    
    uint64_t counter = (uint64_t)(seconds / /*self.period*/ 30);
    //TEST:
    //uint64_t counter = 123;
    //secret data hex: "4F84718654A35872CC52D48A2FD7B7D9C4F29DCB" 
    
    NSString *OTP = [otpgenerator generateOTPForCounter:counter];
    
    self.lbOTP.text = OTP;
    
    
    //HEX
    NSData *data = secretData;
    NSUInteger capacity = data.length * 2;
    NSMutableString *sbuf = [NSMutableString stringWithCapacity:capacity];
    const unsigned char *buf = data.bytes;
    NSInteger i;
    for (i=0; i<data.length; ++i) {
        [sbuf appendFormat:@"%02X", (NSUInteger)buf[i]]; // Create HEX string
    }
    
    self.tfNote.text = sbuf;
}



- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}



+ (NSData *)base32Decode:(NSString *)string {
    GTMStringEncoding *coder =
    [GTMStringEncoding stringEncodingWithString:kBase32Charset];
    [coder addDecodeSynonyms:kBase32Synonyms];
    [coder ignoreCharacters:kBase32Sep];
    return [coder decode:string];
}

+ (NSString *)getSecret
{
    NSURL *url = [NSURL URLWithString:@"otpauth://totp/Google%3Amirza.hadziomerovic.codecta%40gmail.com?secret=bmza2ucrfws43i6avncfs6k5hk6vuffm&issuer=Google"];
    
    NSString *secret1 = [url fragment];
    
    return secret1;
}

+ (NSTimeInterval)defaultPeriod {
    return 30;
}

- (NSString *)generateOTP {
    return [self generateOTPForDate:[NSDate date]];
}

- (NSString *)generateOTPForDate:(NSDate *)date {
    if (!date) {
        // If no now date specified, use the current date.
        date = [NSDate date];
    }
    
    NSTimeInterval seconds = [date timeIntervalSince1970];
    uint64_t counter = (uint64_t)(seconds / /*self.period*/ 30);
    return @"abc";
}


- (void)dealloc {
    [_lbOTP release];
    [_lbStatus release];
    [_tfNote release];
    [super dealloc];
}
@end
