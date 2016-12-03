#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

class HelloWorld : public cocos2d::LayerColor
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	bool onContactBegin(const cocos2d::PhysicsContact &contact);
	bool onPostContactSolve(cocos2d::PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve);
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

private:
	void initListeners();
	void initPhysics();

	bool isKeyPressed(cocos2d::EventKeyboard::KeyCode);
	double keyPressedDuration(cocos2d::EventKeyboard::KeyCode);

	const std::string getKeyCodeAsString(cocos2d::EventKeyboard::KeyCode keyCode);
	const std::string _getKeyCodeAsString(cocos2d::EventKeyboard::KeyCode keyCode);

	void checkControlKeys(cocos2d::EventKeyboard::KeyCode keyCode);
	cocos2d::Label * createNewLabel(const std::string &keyStr);
	void createNodeParticles(cocos2d::Node *node);

	std::map<cocos2d::EventKeyboard::KeyCode, std::chrono::high_resolution_clock::time_point> keyMap;
	std::vector<cocos2d::Label *> labelList;
};

#endif // __HELLOWORLD_SCENE_H__
