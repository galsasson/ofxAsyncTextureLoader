# ofxAsyncTextureLoader
openFrameworks addon that loads textures completely asynchronously without reducing the application framerate.

Loading textures **completely** asynchronously using a second openGL context that shares resources with the main openGL context. This is in contrast to ofxThreadedImageLoader that uses the main openGL context to upload the texture to the GPU which blocks the main thread.

Example usage:

```c++
void loadImages()
{
  loader.loadTextureAsync("textures/image.jpg", [this] (ofTexture* tex) {
	  myImage = tex;
  });
}

void draw()
{
  if (myImage) {
    myImage->draw();
  }
}
```
