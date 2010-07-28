//
//  SignatureView.h
//  Signature
//
//  Created by Dmitry Soldatenkov on 7/27/10.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>





@interface SignatureView : UIView {

	CGMutablePathRef mPath;
	CGPoint mLastPoint;
	
}


- (void)doClear;
- (UIImage*)makeUIImage;

- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesMoved:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesEnded:(NSSet *)touches withEvent:(UIEvent *)event;
- (void)touchesCancelled:(NSSet *)touches withEvent:(UIEvent *)event;

@end
