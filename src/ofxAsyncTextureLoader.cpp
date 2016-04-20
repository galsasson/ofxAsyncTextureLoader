//
//  ofxAsyncTextureLoader.cpp
//  ofxAsyncTextureLoader
//
//  Created by Gal Sasson on 4/20/16.
//
//

#include "ofxAsyncTextureLoader.h"

ofxAsyncTextureLoader::~ofxAsyncTextureLoader()
{
	bRunning = false;
	textureLoaderThread.join();
}

ofxAsyncTextureLoader::ofxAsyncTextureLoader()
{
	GLFWwindow* mainWindow = getMainContextWindow();
	bool ok = create2ndContext(mainWindow);
	if (!ok) {
		ofLogError("ofxAsyncTextureLoader") << "error creating 2nd openGL context";
		return;
	}
	ofLogNotice("ofxAsyncTextureLoader") << "2nd openGL context created successfully";
	ofAddListener(ofEvents().update, this, &ofxAsyncTextureLoader::callCompleteCallbacks);
	textureLoaderThread = std::thread(&ofxAsyncTextureLoader::loaderThreadFunction, this);
	bRunning = true;
}

void ofxAsyncTextureLoader::loadTextureAsync(const string &path, const function<void(shared_ptr<ofTexture>)>& completeCallback, bool mipmapped)
{
	TextureLoaderTask task;
	task.path = path;
	task.bMipmapped = mipmapped;
	task.tex = NULL;
	task.loadCompleteCallback = completeCallback;
	loadQueueMutex.lock();
	loadQueue.push_back(task);
	loadQueueMutex.unlock();
}

shared_ptr<ofTexture> ofxAsyncTextureLoader::loadTextureSync(const string& path, bool mipmapped)
{
	shared_ptr<ofTexture> tex(make_shared<ofTexture>());
	bool ok = ofLoadImage(*tex, path);
	if (ok) {
		if (mipmapped) {
			tex->bind();
			glGenerateMipmap(tex->getTextureData().textureTarget);
			tex->unbind();
			tex->setTextureMinMagFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
		}

		tex->setTextureWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
	}
	else {
		return NULL;
	}

	return tex;
}

GLFWwindow* ofxAsyncTextureLoader::getMainContextWindow()
{
	ofAppBaseWindow* win = ofGetWindowPtr();
	ofAppGLFWWindow* glfwWin = dynamic_cast<ofAppGLFWWindow*>(win);
	return glfwWin->getGLFWWindow();
}

bool ofxAsyncTextureLoader::create2ndContext(GLFWwindow* main)
{
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	context = glfwCreateWindow(10, 10, "", NULL, main);
	if (context == NULL) {
		return false;
	}
	return true;
}

void ofxAsyncTextureLoader::loaderThreadFunction()
{
	glfwMakeContextCurrent(context);
	while (bRunning) {
		loadQueueMutex.lock();
		std::vector<TextureLoaderTask> tasks = loadQueue;
		loadQueue.clear();
		loadQueueMutex.unlock();

		for (TextureLoaderTask& task : tasks) {
			task.tex = loadTextureSync(task.path, task.bMipmapped);
			glFinish();
			if (task.tex == NULL) {
				ofLogError("BackgroundTextureLoader") << "error loading texture: " << task.path;
			}
			completeQueueMutex.lock();
			completeQueue.push_back(task);
			completeQueueMutex.unlock();
		}

		ofSleepMillis(1);
	}
}

void ofxAsyncTextureLoader::callCompleteCallbacks(ofEventArgs& updateArgs)
{
	completeQueueMutex.lock();
	vector<struct TextureLoaderTask> loaded = completeQueue;
	completeQueue.clear();
	completeQueueMutex.unlock();

	for (TextureLoaderTask& task : loaded) {
		task.loadCompleteCallback(task.tex);
	}
}
