//
//  ofxAsyncTextureLoader.h
//  ofxAsyncTextureLoader
//
//  Created by Gal Sasson on 4/20/16.
//
//

#pragma once

#include "ofMain.h"

class ofxAsyncTextureLoader
{
public:
	~ofxAsyncTextureLoader();
	ofxAsyncTextureLoader();
	bool setup();
	void loadTextureAsync(const string& path, bool mipmapped=false, bool arb=false, bool loadPixels=false, ofTexCompression compression=OF_COMPRESS_NONE, const function<void(shared_ptr<ofTexture>, shared_ptr<ofPixels>)>& completeCallback = [](shared_ptr<ofTexture> tex, shared_ptr<ofPixels> pixels) {});
	shared_ptr<ofTexture> loadTextureSync(const string& path, bool mipmapped=false, bool arb=false, ofTexCompression compression=OF_COMPRESS_NONE);
	shared_ptr<ofTexture> loadTextureSync(shared_ptr<ofPixels>, bool mipmapped=false, bool arb=false, ofTexCompression compression=OF_COMPRESS_NONE);
	shared_ptr<ofPixels> loadPixelsSync(const string& path);


	void callCompleteCallbacks();

private:
	bool bInitialized;
	bool bRunning;
	GLFWwindow* context;
	std::thread textureLoaderThread;
	struct TextureLoaderTask {
		string path;
		bool bMipmapped;
		bool bLoadPixels;
		bool bLoadAsARB;
		ofTexCompression compression;
		shared_ptr<ofTexture> tex;
		shared_ptr<ofPixels> pixels;
		function<void(shared_ptr<ofTexture>, shared_ptr<ofPixels>)> loadCompleteCallback;
	};
	std::vector<struct TextureLoaderTask> loadQueue;
	std::mutex loadQueueMutex;
	std::vector<struct TextureLoaderTask> completeQueue;
	std::mutex completeQueueMutex;

	GLFWwindow* getMainContextWindow();
	GLFWwindow* create2ndContext(GLFWwindow* main);
	void loaderThreadFunction();
	void update(ofEventArgs& updateArgs);
};