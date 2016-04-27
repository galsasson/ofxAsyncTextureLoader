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
	if (textureLoaderThread.joinable()) {
		textureLoaderThread.join();
	}
}

ofxAsyncTextureLoader::ofxAsyncTextureLoader()
{
	bInitialized = false;
	bRunning = false;
	context = NULL;
}

bool ofxAsyncTextureLoader::setup()
{
	if (bInitialized) {
		ofLogWarning("ofxAsyncTextureLoader") << "already initialized";
		return false;
	}

	GLFWwindow* mainWindow = getMainContextWindow();
	if (mainWindow == NULL) {
		ofLogError("ofxAsyncTextureLoader") << "error creating second OpenGL context";
		return false;
	}
	context = create2ndContext(mainWindow);
	if (context == NULL) {
		ofLogError("ofxAsyncTextureLoader") << "error creating second OpenGL context";
		return false;
	}
	ofLogNotice("ofxAsyncTextureLoader") << "second OpenGL context created successfully";
	bInitialized = true;
	bRunning = true;
	ofAddListener(ofEvents().update, this, &ofxAsyncTextureLoader::update);
	textureLoaderThread = std::thread(&ofxAsyncTextureLoader::loaderThreadFunction, this);
	return true;
}

void ofxAsyncTextureLoader::loadTextureAsync(const string &path, const function<void(shared_ptr<ofTexture>)>& completeCallback, bool mipmapped)
{
	if (!bInitialized) {
		ofLogError("ofxAsyncTextureLoader") << "not initialized, call setup() first";
	}
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
	if (mipmapped) {
		tex->enableMipmap();
	}
	bool ok = ofLoadImage(*tex, path);
	if (!ok) {
		return NULL;
	}

	return tex;
}

GLFWwindow* ofxAsyncTextureLoader::getMainContextWindow()
{
	ofAppBaseWindow* win = ofGetWindowPtr();
	if (win == NULL) {
		return NULL;
	}
	ofAppGLFWWindow* glfwWin = dynamic_cast<ofAppGLFWWindow*>(win);
	if (glfwWin == NULL) {
		return NULL;
	}
	return glfwWin->getGLFWWindow();
}

GLFWwindow* ofxAsyncTextureLoader::create2ndContext(GLFWwindow* main)
{
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	return glfwCreateWindow(1, 1, "", NULL, main);
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
				ofLogError("ofxAsyncTextureLoader") << "error loading texture: " << task.path;
			}
			completeQueueMutex.lock();
			completeQueue.push_back(task);
			completeQueueMutex.unlock();
		}

		ofSleepMillis(1);
	}
}

void ofxAsyncTextureLoader::callCompleteCallbacks()
{
	completeQueueMutex.lock();
	vector<struct TextureLoaderTask> loaded = completeQueue;
	completeQueue.clear();
	completeQueueMutex.unlock();

	for (TextureLoaderTask& task : loaded) {
		task.loadCompleteCallback(task.tex);
	}
}

void ofxAsyncTextureLoader::update(ofEventArgs& updateArgs)
{
	callCompleteCallbacks();
}
