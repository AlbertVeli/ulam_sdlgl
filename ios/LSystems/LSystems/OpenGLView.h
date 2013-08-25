//
//  OpenGLView.h
//  LSystems
//
//  Created by Albert Veli at
//  Boomtime, the 18th day of Bureaucracy in the YOLD 3179
//  (k) ALL RIGHTS REVERSED - Reprint what you like

#import <UIKit/UIKit.h>
#import <QuartzCore/QuartzCore.h>
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>

@interface OpenGLView : UIView {
   CAEAGLLayer* _eaglLayer;
   EAGLContext* _context;
   GLuint _colorRenderBuffer;
   GLuint _positionSlot;
   GLuint _colorSlot;
   GLuint _projectionUniform;
   GLuint _modelViewUniform;
   float _currentRotation;
   GLuint _depthRenderBuffer;
   GLuint _vertexBuffer;
}

@end

/**
 * Local Variables:
 * mode: objc
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
