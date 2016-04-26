# ofxAsyncTextureLoader
openFrameworks addon that loads textures asynchronously without reducing the application framerate.

This addon loads textures **completely** asynchronously using a second OpenGL context that shares resources with the main OpenGL context. This is in contrast to ofxThreadedImageLoader that uses the main openGL context to upload the texture to the GPU which blocks the main thread (especially when loading large textures).

### Use ofxAsyncTextureLoader:

.h:
```c++
ofxAsyncTextureLoader loader;
shared_ptr<ofTexture> myImage;
```

.cpp:
```c++
void loadImages()
{
  loader.loadTextureAsync("textures/image.jpg", [this] (shared_ptr<ofTexture> tex) {
	  // load complete callback
	  if (tex == NULL) {
		ofLogError() << "error loading image.jpg";
	  }
	  myImage = tex;
  });
}

void draw()
{
  if (myImage) {
    myImage->draw(0, 0);
  }
}
```

The complete callback will be called by the main thread in OF update stage, so no locking is required.

### Or use ofxAsyncTexture for convenience:

.h:
```c++
ofxAsyncTexture myImage;
ofxAsyncTexture myWebImage;
```

.cpp:
```c++
void loadImages()
{
	myImage.load("textures/image.jpg");
	myWebImage.load("http://imagecollection.com/image.jpg");

	// optional: register for load complete events
	ofAddListener(myImage.eventLoaded, this, &ofApp::onImageLoaded);
}

void draw()
{
	myImage.draw(0, 0);			// will only draw once the texture is loaded
	myWebImage.draw(100, 100);	// will only draw once the texture is loaded
}
```

ofxAsyncTexture wraps ofTexture and exposes some of its API, making sure that the texture is loaded before calling its methods. Internally it uses one static ofxAsyncTextureLoader that gets initialized on the first call to 'load'.

This addon relies on GLFW for the second OpenGL context creation and works only on platforms that use it (Windows, OS X, Linux, Android).
