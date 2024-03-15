#include "ofApp.h"	

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(25);
	ofSetWindowTitle("openFrameworks");

	ofBackground(39);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(250, 8);
	for (auto& vertex : ico_sphere.getMeshPtr()->getVertices()) {

		this->base_location_list.push_back(vertex);
	}

}

//--------------------------------------------------------------
void ofApp::update() {

	int radius = 5;
	ofColor color;
	while (this->log_list.size() < 15000) {

		int font_location_index = ofRandom(this->base_location_list.size());
		vector<glm::vec3> log;
		log.push_back(this->base_location_list[font_location_index]);
		this->log_list.push_back(log);
		color.setHsb((int)ofRandom(200, 290) % 255, 255, 255);
		this->color_list.push_back(color);
		this->life_list.push_back(ofRandom(10, 60));
	}

	for (int i = this->log_list.size() - 1; i >= 0; i--) {

		this->life_list[i] -= 1;
		if (this->life_list[i] < 0) {

			this->log_list.erase(this->log_list.begin() + i);
			this->color_list.erase(this->color_list.begin() + i);
			this->life_list.erase(this->life_list.begin() + i);

			continue;
		}

		auto deg = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.003, ofGetFrameNum() * 0.003)), 0, 1, -360, 360);
		auto theta = ofMap(ofNoise(glm::vec4(this->log_list[i].back() * 0.003, (ofGetFrameNum() + 10000) * 0.003)), 0, 1, -360, 360);
		auto location = this->log_list[i].back() + glm::vec3(radius * cos(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD) * sin(theta * DEG_TO_RAD), radius * cos(theta * DEG_TO_RAD));
		location = glm::normalize(location) * 250;
		this->log_list[i].push_back(location);
		while (this->log_list[i].size() > 300) {

			this->log_list[i].erase(this->log_list[i].begin());
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 1.44);

	ofNoFill();
	for (int i = 0; i < this->log_list.size(); i++) {

		if (this->life_list[i] > 30) {

			ofSetLineWidth(1);
			ofSetColor(this->color_list[i]);
		}
		else {

			ofSetLineWidth(ofMap(this->life_list[i], 0, 30, 0, 1));
			ofSetColor(ofColor(this->color_list[i], ofMap(this->life_list[i], 0, 30, 32, 255)));
		}

		ofBeginShape();
		ofVertices(this->log_list[i]);
		ofEndShape();
	}

	ofFill();
	ofSetColor(39);
	ofDrawSphere(249);

	this->cam.end();

	/*
	int start = 150;
	if (ofGetFrameNum() > start) {

		ostringstream os;
		os << setw(4) << setfill('0') << ofGetFrameNum() - start;
		ofImage image;
		image.grabScreen(0, 0, ofGetWidth(), ofGetHeight());
		image.saveImage("image/cap/img_" + os.str() + ".jpg");
		if (ofGetFrameNum() - start >= 25 * 20) {

			std::exit(1);
		}
	}
	*/
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}