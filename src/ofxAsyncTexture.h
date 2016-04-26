//
//  ofxAsyncTexture.hpp
//  ofxAsyncTextureLoader
//
//  Created by Gal Sasson on 4/25/16.
//
//

#ifndef ofxAsyncTexture_h
#define ofxAsyncTexture_h

#include "ofxAsyncTextureLoader.h"

class ofxAsyncTexture
{
public:
	void load(const string& path, bool mipmapped=false);
	bool isLoaded() const { return tex!=NULL; }

	// register to get load complete events
	ofEvent<ofxAsyncTexture> eventLoaded;

	// providing a partial ofTexture API
	float getWidth() const;
	float getHeight() const;
	void draw(float x, float y) const;
	void draw(float x, float y, float z) const;
	void draw(float x, float y, float w, float h) const;
	void draw(float x, float y, float z, float w, float h) const;
	void draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const;
	void drawSubsection(float x, float y, float w, float h, float sx, float sy) const;
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const;
	void drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const;
	void drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const;
	void bind(int textureLocation=0) const;
	void unbind(int textureLocation=0) const;

	// get the raw ofTexture
	shared_ptr<ofTexture> getTexture() { return tex; }
	string getTexturePath() { return texturePath; }

private:
	string texturePath;
	shared_ptr<ofTexture> tex;
	static ofxAsyncTextureLoader& getTexLoader() {
		static ofxAsyncTextureLoader texLoader;
		return texLoader;
	}
};

#endif /* ofxAsyncTexture_h */
