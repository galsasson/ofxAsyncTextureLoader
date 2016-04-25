# ofxAsyncTextureLoader
openFrameworks addon that loads textures asynchronously without reducing the application framerate.

This addon loads textures **completely** asynchronously using a second openGL context that shares resources with the main openGL context. This is in contrast to ofxThreadedImageLoader that uses the main openGL context to upload the texture to the GPU which blocks the main thread (especially when loading large textures).

Example usage:

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

This addon relies on GLFW for the second openGL context creation and works only on platforms that use it (Windows, OS X, Linux, Android).
