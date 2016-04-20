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
	void loadTextureAsync(const string& path, const function<void(shared_ptr<ofTexture>)>& completeCallback = [](shared_ptr<ofTexture> tex) {}, bool mipmapped=false);
	shared_ptr<ofTexture> loadTextureSync(const string& path, bool mipmapped=false);

	void callCompleteCallbacks();

private:
	bool bRunning;
	GLFWwindow* context;
	std::thread textureLoaderThread;
	struct TextureLoaderTask {
		string path;
		bool bMipmapped;
		shared_ptr<ofTexture> tex;
		function<void(shared_ptr<ofTexture>)> loadCompleteCallback;
	};
	std::vector<struct TextureLoaderTask> loadQueue;
	std::mutex loadQueueMutex;
	std::vector<struct TextureLoaderTask> completeQueue;
	std::mutex completeQueueMutex;

	GLFWwindow* getMainContextWindow();
	bool create2ndContext(GLFWwindow* main);
	void loaderThreadFunction();
	void update(ofEventArgs& updateArgs);
};