//
//  OpenGLView.m
//  LSystems
//
// iPhone OpenGL ES 2.0 boilerplate code from:
// http://www.raywenderlich.com/3664/opengl-es-2-0-for-iphone-tutorial
//
//  Created by Albert Veli at
//  Boomtime, the 18th day of Bureaucracy in the YOLD 3179
//  (k) ALL RIGHTS REVERSED - Reprint what you like

#import "OpenGLView.h"
#import "CC3GLMatrix.h"

@implementation OpenGLView

typedef struct {
   float Position[2];
   float Colour[4];
} Vertex;

const Vertex Vertices[] = {
   // Position  Colour
   { {-1, -1}, {0, 0, 1, 1} },
   { { 1, -1}, {0, 1, 0, 1} },
   { { 1,  1}, {1, 1, 0, 1} },
   { {-1,  1}, {1, 0, 0, 1} },
   { {-1, -1}, {0, 0, 1, 1} }
};

+ (Class)layerClass {
   return [CAEAGLLayer class];
}

- (void)setupLayer {
   _eaglLayer = (CAEAGLLayer*) self.layer;
   _eaglLayer.opaque = YES;
}

- (void)setupContext {
   EAGLRenderingAPI api = kEAGLRenderingAPIOpenGLES2;
   _context = [[EAGLContext alloc] initWithAPI:api];
   if (!_context) {
      NSLog(@"Failed to initialize OpenGLES 2.0 context");
      exit(1);
   }

   if (![EAGLContext setCurrentContext:_context]) {
      NSLog(@"Failed to set current OpenGL context");
      exit(1);
   }
}

- (void)setupRenderBuffer {
   glGenRenderbuffers(1, &_colorRenderBuffer);
   glBindRenderbuffer(GL_RENDERBUFFER, _colorRenderBuffer);
   [_context renderbufferStorage:GL_RENDERBUFFER fromDrawable:_eaglLayer];
}

- (void)setupDepthBuffer {
   glGenRenderbuffers(1, &_depthRenderBuffer);
   glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
   glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, self.frame.size.width, self.frame.size.height);
}

- (void)setupFrameBuffer {
   GLuint framebuffer;
   glGenFramebuffers(1, &framebuffer);
   glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, _colorRenderBuffer);
   glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depthRenderBuffer);
}

- (GLuint)compileShader:(NSString*)shaderName withType:(GLenum)shaderType {
   NSString* shaderPath = [[NSBundle mainBundle] pathForResource:shaderName ofType:@"glsl"];
   NSError* error;
   NSString* shaderString = [NSString stringWithContentsOfFile:shaderPath encoding:NSUTF8StringEncoding error:&error];
   if (!shaderString) {
      NSLog(@"Error loading shader: %@", error.localizedDescription);
      exit(1);
   }
   GLuint shaderHandle = glCreateShader(shaderType);
   const char * shaderStringUTF8 = [shaderString UTF8String];
   int shaderStringLength = [shaderString length];
   glShaderSource(shaderHandle, 1, &shaderStringUTF8, &shaderStringLength);
   glCompileShader(shaderHandle);
   GLint compileSuccess;
   glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compileSuccess);
   if (compileSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetShaderInfoLog(shaderHandle, sizeof(messages), 0, &messages[0]);
      NSString *messageString = [NSString stringWithUTF8String:messages];
      NSLog(@"%@", messageString);
      exit(1);
   }
   return shaderHandle;
}

- (void)compileShaders {
   GLuint vertexShader = [self compileShader:@"vertex" withType:GL_VERTEX_SHADER];
   GLuint fragmentShader = [self compileShader:@"fragment" withType:GL_FRAGMENT_SHADER];

   GLuint programHandle = glCreateProgram();
   glAttachShader(programHandle, vertexShader);
   glAttachShader(programHandle, fragmentShader);
   glLinkProgram(programHandle);

   GLint linkSuccess;
   glGetProgramiv(programHandle, GL_LINK_STATUS, &linkSuccess);
   if (linkSuccess == GL_FALSE) {
      GLchar messages[256];
      glGetProgramInfoLog(programHandle, sizeof(messages), 0, &messages[0]);
      NSString *messageString = [NSString stringWithUTF8String:messages];
      NSLog(@"%@", messageString);
      exit(1);
   }

   glUseProgram(programHandle);
   _positionSlot = glGetAttribLocation(programHandle, "Position");
   _colorSlot = glGetAttribLocation(programHandle, "SourceColor");
   glEnableVertexAttribArray(_positionSlot);
   glEnableVertexAttribArray(_colorSlot);

   _projectionUniform = glGetUniformLocation(programHandle, "Projection");
   _modelViewUniform = glGetUniformLocation(programHandle, "Modelview");
}

- (void)setupVBOs {
   glGenBuffers(1, &_vertexBuffer);
   glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
   glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

- (void)render:(CADisplayLink*)displayLink {
   glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
   glEnable(GL_BLEND);

   glClearColor(0, 0, 0, 1.0);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);

   CC3GLMatrix *projection = [CC3GLMatrix matrix];
   float h = 4.0f * self.frame.size.height / self.frame.size.width;
   [projection populateFromFrustumLeft:-2 andRight:2 andBottom:-h/2 andTop:h/2 andNear:4 andFar:10];
   glUniformMatrix4fv(_projectionUniform, 1, 0, projection.glMatrix);

   CC3GLMatrix *modelView = [CC3GLMatrix matrix];
   [modelView populateFromTranslation:CC3VectorMake(sin(CACurrentMediaTime()), 0, -7)];
   _currentRotation += displayLink.duration * 90;
   [modelView rotateBy:CC3VectorMake(_currentRotation, _currentRotation, 0)];
   glUniformMatrix4fv(_modelViewUniform, 1, 0, modelView.glMatrix);
   glViewport(0, 0, self.frame.size.width, self.frame.size.height);

   glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
   glVertexAttribPointer(_positionSlot, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
   glVertexAttribPointer(_colorSlot, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*) (sizeof(float) * 2));
   glDrawArrays(GL_LINE_STRIP, 0, sizeof(Vertices)/sizeof(Vertex));

   [_context presentRenderbuffer:GL_RENDERBUFFER];
}

- (void)setupDisplayLink {
   CADisplayLink* displayLink = [CADisplayLink displayLinkWithTarget:self selector:@selector(render:)];
   [displayLink addToRunLoop:[NSRunLoop currentRunLoop] forMode:NSDefaultRunLoopMode];
}

- (id)initWithFrame:(CGRect)frame
{
   self = [super initWithFrame:frame];
   if (self) {
      [self setupLayer];
      [self setupContext];
      [self setupDepthBuffer];
      [self setupRenderBuffer];
      [self setupFrameBuffer];
      [self compileShaders];
      [self setupVBOs];
      [self setupDisplayLink];
   }
   return self;
}

- (void)dealloc
{
   [_context release];
   _context = nil;
   [super dealloc];
}

@end

/**
 * Local Variables:
 * mode: objc
 * indent-tabs-mode: nil
 * c-basic-offset: 3
 * End:
 */
