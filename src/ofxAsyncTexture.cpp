//
//  ofxAsyncTexture.cpp
//  ofxAsyncTextureLoader
//
//  Created by Gal Sasson on 4/25/16.
//
//

#include "ofxAsyncTexture.h"

void ofxAsyncTexture::load(const string &path, bool mipmapped)
{
	getTexLoader().loadTextureAsync(path, [this, path] (shared_ptr<ofTexture> t) {
		if (t == NULL) {
			ofLogError("ofxAsyncTexture") << "could not load texture: "<<path;
			return;
		}
		texturePath = path;
		tex = t;
		ofNotifyEvent(eventLoaded, *this, this);
	}, mipmapped);
}

float ofxAsyncTexture::getWidth() const
{
	if (!isLoaded()) {
		ofLogWarning("ofxAsyncTexture") << "getWidth(): texture not loaded";
		return 0;
	}
	return tex->getWidth();
}

float ofxAsyncTexture::getHeight() const
{
	if (!isLoaded()) {
		ofLogWarning("ofxAsyncTexture") << "getHeight(): texture not loaded";
		return 0;
	}
	return tex->getHeight();
}

void ofxAsyncTexture::draw(float x, float y) const
{
	if (!isLoaded()) {
		return;
	}
	tex->draw(x, y);
}

void ofxAsyncTexture::draw(float x, float y, float z) const
{
	if (!isLoaded()) {
		return;
	}
	tex->draw(x, y, z);
}

void ofxAsyncTexture::draw(float x, float y, float w, float h) const
{
	if (!isLoaded()) {
		return;
	}
	tex->draw(x, y, w, h);
}

void ofxAsyncTexture::draw(float x, float y, float z, float w, float h) const
{
	if (!isLoaded()) {
		return;
	}
	tex->draw(x, y, z, w, h);
}

void ofxAsyncTexture::draw(const ofPoint & p1, const ofPoint & p2, const ofPoint & p3, const ofPoint & p4) const
{
	if (!isLoaded()) {
		return;
	}
	tex->draw(p1, p2, p3, p4);
}

void ofxAsyncTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy) const
{
	if (!isLoaded()) {
		return;
	}
	tex->drawSubsection(x, y, w, h, sx, sy);
}

void ofxAsyncTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy) const
{
	if (!isLoaded()) {
		return;
	}
	tex->drawSubsection(x, y, z, w, h, sx, sy);
}

void ofxAsyncTexture::drawSubsection(float x, float y, float w, float h, float sx, float sy, float sw, float sh) const
{
	if (!isLoaded()) {
		return;
	}
	tex->drawSubsection(x, y, w, h, sx, sy, sw, sh);
}

void ofxAsyncTexture::drawSubsection(float x, float y, float z, float w, float h, float sx, float sy, float sw, float sh) const
{
	if (!isLoaded()) {
		return;
	}
	tex->drawSubsection(x, y, z, w, h, sx, sy, sw, sh);
}

void ofxAsyncTexture::bind(int textureLocation) const
{
	if (!isLoaded()) {
		return;
	}
	tex->bind(textureLocation);
}

void ofxAsyncTexture::unbind(int textureLocation) const
{
	if (!isLoaded()) {
		return;
	}
	tex->unbind(textureLocation);
}
