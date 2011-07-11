/*
 *  ofLight.cpp
 *  openFrameworksLib
 *
 *  Created by Memo Akten on 14/01/2011.
 *  Copyright 2011 MSA Visuals Ltd. All rights reserved.
 *
 */

#include "ofLight.h"
#include "ofConstants.h"
#include "ofLog.h"
#include "ofUtils.h"
#include <map>


//----------------------------------------
void ofEnableLighting() {
	glEnable(GL_LIGHTING);
#ifndef TARGET_OPENGLES  //TODO: fix this
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
#endif
	glEnable(GL_COLOR_MATERIAL);
}

//----------------------------------------
void ofDisableLighting() {
	glDisable(GL_LIGHTING);
}

//----------------------------------------
void ofEnableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR);
#endif
}

//----------------------------------------
void ofDisableSeparateSpecularLight(){
#ifndef TARGET_OPENGLES
	glLightModeli (GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
#endif
}

//----------------------------------------
bool ofGetLightingEnabled() {
	return glIsEnabled(GL_LIGHTING);
}

//----------------------------------------
void ofSetSmoothLighting(bool b) {
	if (b) glShadeModel(GL_SMOOTH);
	else glShadeModel(GL_FLAT);
}

//----------------------------------------
void ofSetGlobalAmbientColor(const ofColor& c) {
	GLfloat cc[] = {c.r/255.f, c.g/255.f, c.b/255.f, c.a/255.f};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, cc);
}

//----------------------------------------
<<<<<<< HEAD
void ofSetGlobalAmbientColor(float r, float g, float b, float a) {
	ofSetGlobalAmbientColor( ofColor(r, g, b, a) );
}

//----------------------------------------
=======
>>>>>>> master
bool* getActiveLights(){
	static bool * lightsActive = new bool[OF_MAX_LIGHTS];
	static bool lightsActiveInited = false;
	// if array hasn't been inited to false, init it
	if(lightsActiveInited == false) {
		for(int i=0; i<OF_MAX_LIGHTS; i++) lightsActive[i] = false;
		lightsActiveInited = true;
	}
	return lightsActive;
}

<<<<<<< HEAD
static void setActiveLight( int id ) {
	if (id < 0 || id >= OF_MAX_LIGHTS) return;
	getActiveLights()[id] = true;
}

static void setInactiveLight( int id ) {
	if (id < 0 || id >= OF_MAX_LIGHTS) return;
	getActiveLights()[id] = false;
}

=======
//--------------------------------------------------------------
>>>>>>> master
static map<GLuint,int> & getIds(){
	static map<GLuint,int> * ids = new map<GLuint,int>;
	return *ids;
}

//--------------------------------------------------------------
static void retain(int id){
	if(id==-1) return;
	getActiveLights()[id] = true;
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]++;
	} else {
		getIds()[id]=1;
	}
}

//--------------------------------------------------------------
static void release(ofLight & light){
	int id = light.getLightID();
	if(id==-1) return;
	//setInactiveLight( id );
	bool lastRef=false;
	if(getIds().find(id)!=getIds().end()){
		getIds()[id]--;
		if(getIds()[id]==0){
			lastRef=true;
			getIds().erase(id);
		}
	}else{
		ofLog(OF_LOG_WARNING,"ofLight: releasing id not found, this shouldn't be happening releasing anyway");
		lastRef=true;
	}
	if(lastRef){
		light.setAmbientColor(ofColor(0,0,0,255));
		if(id>0){
			light.setDiffuseColor(ofColor(0,0,0,255));
			light.setSpecularColor(ofColor(0,0,0,255));
		}else{
			light.setDiffuseColor(ofColor(255,255,255,255));
			light.setSpecularColor(ofColor(255,255,255,255));
		}
		GLfloat cc[] = {0,0,1, 0};
		glLightfv(GL_LIGHT0 + id, GL_POSITION, cc);

		light.disable();
		getActiveLights()[id] = false;
	}
}

//----------------------------------------
ofLight::ofLight(){
<<<<<<< HEAD
	glIndex = -1;
	isEnabled = false;
	isDirectional = false;
	isSpotlight		= false;
=======
	glIndex			= -1;
	isEnabled		= false;
	setPointLight();
>>>>>>> master
}

//----------------------------------------
ofLight::~ofLight(){
	destroy();
}

//----------------------------------------
void ofLight::destroy(){
	release(*this);
}

//----------------------------------------
ofLight::ofLight(const ofLight & mom){
	ambientColor	= mom.ambientColor;
	diffuseColor	= mom.diffuseColor;
	specularColor	= mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled		= mom.isEnabled;
	isDirectional	= mom.isDirectional;
	isSpotlight		= mom.isSpotlight;
<<<<<<< HEAD
=======
	lightType		= mom.lightType;
>>>>>>> master
}

//----------------------------------------
ofLight & ofLight::operator=(const ofLight & mom){
	if(&mom == this) return *this;
	ambientColor = mom.ambientColor;
	diffuseColor = mom.diffuseColor;
	specularColor = mom.specularColor;

	glIndex = mom.glIndex;
	retain(glIndex);
	isEnabled		= mom.isEnabled;
	isDirectional	= mom.isDirectional;
	isSpotlight		= mom.isSpotlight;
<<<<<<< HEAD
=======
	lightType		= mom.lightType;
>>>>>>> master
	return *this;
}

//----------------------------------------
void ofLight::enable() {
	if(glIndex==-1){
		bool bLightFound = false;
		// search for the first free block
		for(int i=0; i<OF_MAX_LIGHTS; i++) {
			if(getActiveLights()[i] == false) {
				glIndex = i;
				retain(glIndex);
				bLightFound = true;
				break;
			}
		}
		if( !bLightFound ){
			ofLog(OF_LOG_ERROR, "ofLight : Trying to create too many lights: " + ofToString(glIndex));
		}
	}
	
	ofEnableLighting();
	glEnable(GL_LIGHT0 + glIndex);
}

//----------------------------------------
void ofLight::disable() {
	if(glIndex!=-1) {
		glDisable(GL_LIGHT0 + glIndex);
	}
}

//----------------------------------------
int ofLight::getLightID() const{
	return glIndex;
}

//----------------------------------------
bool ofLight::getIsEnabled() const {
	return isEnabled;
}

//----------------------------------------
void ofLight::setDirectional() {
	isDirectional	= true;
	isSpotlight		= false;
	lightType		= OF_LIGHT_DIRECTIONAL;
}

//----------------------------------------
bool ofLight::getIsDirectional() const {
	return isDirectional;
}

//----------------------------------------
void ofLight::setSpotlight(float spotCutOff, float exponent) {
<<<<<<< HEAD
	setDirectional(false);
	isSpotlight		= true;
=======
	isDirectional	= false;
	isSpotlight		= true;
	lightType		= OF_LIGHT_SPOT;
>>>>>>> master
	setSpotlightCutOff( spotCutOff );
	setSpotConcentration( exponent );
}

//----------------------------------------
bool ofLight::getIsSpotlight() {
	return isSpotlight;
}

//----------------------------------------
void ofLight::setSpotlightCutOff( float spotCutOff ) {
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_CUTOFF, CLAMP(spotCutOff, 0, 90) );
}

//----------------------------------------
void ofLight::setSpotConcentration( float exponent ) {
	glLightf(GL_LIGHT0 + glIndex, GL_SPOT_EXPONENT, exponent);
}

//----------------------------------------
void ofLight::setPointLight() {
<<<<<<< HEAD
	setDirectional( false );
	isSpotlight		= false;
=======
	isDirectional	= false;
	isSpotlight	= false;
	lightType	= OF_LIGHT_POINT;
>>>>>>> master
}

//----------------------------------------
bool ofLight::getIsPointLight() {
	return (!isDirectional && !isSpotlight);
}

//----------------------------------------
void ofLight::setAttenuation( float constant, float linear, float quadratic ) {
	glLightf(GL_LIGHT0 + glIndex, GL_CONSTANT_ATTENUATION, constant);
	glLightf(GL_LIGHT0 + glIndex, GL_LINEAR_ATTENUATION, linear);
	glLightf(GL_LIGHT0 + glIndex, GL_QUADRATIC_ATTENUATION, quadratic);
}

//----------------------------------------
<<<<<<< HEAD
void ofLight::setAmbientColor(const ofColor& c) {
=======
int ofLight::getType() {
	return lightType;
}

//----------------------------------------
void ofLight::setAmbientColor(const ofFloatColor& c) {
>>>>>>> master
	if(glIndex==-1) return;
	ambientColor = c;
	glLightfv(GL_LIGHT0 + glIndex, GL_AMBIENT, &ambientColor.r);
}

//----------------------------------------
<<<<<<< HEAD
void ofLight::setAmbientColor(float r, float g, float b, float a) {
	setAmbientColor(r, g, b, a);
}

//----------------------------------------
void ofLight::setDiffuseColor(const ofColor& c) {
=======
void ofLight::setDiffuseColor(const ofFloatColor& c) {
>>>>>>> master
	if(glIndex==-1) return;
	diffuseColor = c;
	glLightfv(GL_LIGHT0 + glIndex, GL_DIFFUSE, &diffuseColor.r);
}

<<<<<<< HEAD
//----------------------------------------
void ofLight::setDiffuseColor(float r, float g, float b, float a) {
	setDiffuseColor(ofColor(r, g, b, a));
}

=======
>>>>>>> master
//----------------------------------------
void ofLight::setSpecularColor(const ofFloatColor& c) {
	if(glIndex==-1) return;
	specularColor = c;
	glLightfv(GL_LIGHT0 + glIndex, GL_SPECULAR, &specularColor.r);
}

//----------------------------------------
<<<<<<< HEAD
void ofLight::setSpecularColor(float r, float g, float b, float a) {
	setSpecularColor(ofColor(r, g, b, a));
}

//----------------------------------------
ofColor ofLight::getAmbientColor() const {
	return ofColor(ambientColor.r * 255.f, ambientColor.g * 255.f, ambientColor.b * 255.f, ambientColor.a * 255.f);
}

//----------------------------------------
ofColor ofLight::getDiffuseColor() const {
	return ofColor(diffuseColor.r * 255.0f, diffuseColor.g * 255.f, diffuseColor.b * 255.f, diffuseColor.a * 255.f);
}

//----------------------------------------
ofColor ofLight::getSpecularColor() const {
	return ofColor(specularColor.r * 255.f, specularColor.g * 255.f, specularColor.b * 255.f, specularColor.a * 255.f);
=======
ofFloatColor ofLight::getAmbientColor() const {
	return ambientColor;
}

//----------------------------------------
ofFloatColor ofLight::getDiffuseColor() const {
	return diffuseColor;
}

//----------------------------------------
ofFloatColor ofLight::getSpecularColor() const {
	return specularColor;
>>>>>>> master
}


//----------------------------------------
void ofLight::onPositionChanged() {
	if(glIndex==-1) return;
	// if we are a positional light and not directional, update light position
	if(isDirectional == false) {
		GLfloat cc[] = {getPosition().x, getPosition().y, getPosition().z, 1};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	}
}

//----------------------------------------
void ofLight::onOrientationChanged() {
	if(glIndex==-1) return;
	// if we are a directional light and not positional, update light position (direction)
	if(isDirectional == true) {
		GLfloat cc[] = {getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 0};
		glLightfv(GL_LIGHT0 + glIndex, GL_POSITION, cc);
	} else {
		if(isSpotlight) {
			// determines the axis of the cone light //
			GLfloat spot_direction[] = { getLookAtDir().x, getLookAtDir().y, getLookAtDir().z, 1.0 };
			glLightfv(GL_LIGHT0 + glIndex, GL_SPOT_DIRECTION, spot_direction);
		}
	}
}
