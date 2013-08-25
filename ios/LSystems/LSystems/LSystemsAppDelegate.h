//
//  LSystemsAppDelegate.h
//  LSystems
//
//  Created by Albert Veli at
//  Boomtime, the 18th day of Bureaucracy in the YOLD 3179
//  (k) ALL RIGHTS REVERSED - Reprint what you like

#import <UIKit/UIKit.h>
#import "OpenGLView.h"

@interface LSystemsAppDelegate : NSObject <UIApplicationDelegate> {
   OpenGLView* _glView;
}

@property (nonatomic, retain) IBOutlet UIWindow *window;
@property (nonatomic, retain) IBOutlet OpenGLView *glView;

@end

/**
 * Local Variables:
 * mode: objc
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
