#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
   
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(0, 0, 0, 255)))
    {
        return false;
    }
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	initListeners();

	initPhysics();

    return true;
}

void HelloWorld::initListeners() {
	auto eventListener = cocos2d::EventListenerKeyboard::create();
	eventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	eventListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);
}

void HelloWorld::initPhysics() {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto edgeBody = PhysicsBody::createEdgeBox(visibleSize, PHYSICSBODY_MATERIAL_DEFAULT, 3);

	auto edgeNode = Node::create();
	edgeNode->setPosition(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
	edgeNode->setPhysicsBody(edgeBody);

	this->addChild(edgeNode);
	
}

bool HelloWorld::isKeyPressed(EventKeyboard::KeyCode code) {
	// Check if the key is currently pressed by seeing it it's in the std::map keys
	// In retrospect, keys is a terrible name for a key/value paried datatype isnt it?
	if (keyMap.find(code) != keyMap.end())
		return true;
	return false;
}

double HelloWorld::keyPressedDuration(EventKeyboard::KeyCode code) {
	if (!isKeyPressed(code))
		return 0;  // Not pressed, so no duration obviously

				   // Return the amount of time that has elapsed between now and when the user
				   // first started holding down the key in milliseconds
				   // Obviously the start time is the value we hold in our std::map keys
	return std::chrono::duration_cast<std::chrono::milliseconds>
		(std::chrono::high_resolution_clock::now() - keyMap[code]).count();
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	CCLOG("Key released");

	keyMap.erase(keyCode);
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	CCLOG("Key Pressed");

	if (keyMap.find(keyCode) == keyMap.end()) {
		keyMap[keyCode] = std::chrono::high_resolution_clock::now();
	}

	/*//put this inside keyPressed or keyReleased
	if (keyCode == EventKeyboard::KeyCode::KEY_W)
	{
		CCLog("W key was pressed");
	}*/
	std::string keyStr = getKeyCodeAsString(keyCode);

	if (!keyStr.empty()) {
		createNewLabel(keyStr);
	}
	else {
		checkControlKeys(keyCode);
	}
}

void HelloWorld::checkControlKeys(cocos2d::EventKeyboard::KeyCode keyCode) {
	if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW ||
		keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
		auto physicsWorld = this->getScene()->getPhysicsWorld();
		auto gravity = physicsWorld->getGravity();

		if (keyCode == EventKeyboard::KeyCode::KEY_LEFT_ARROW) {
			physicsWorld->setGravity(Vec2(-98.f, 0));
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) {
			physicsWorld->setGravity(Vec2(98.f, 0));
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_UP_ARROW) {
			physicsWorld->setGravity(Vec2(0, 98.f));
		}
		else if (keyCode == EventKeyboard::KeyCode::KEY_DOWN_ARROW) {
			physicsWorld->setGravity(Vec2(0, -98.f));
		}

		gravity = physicsWorld->getGravity();
		CCLOG("Gravity %f %f", gravity.x, gravity.y);
	}
	else if (keyCode == EventKeyboard::KeyCode::KEY_SPACE) {
		for (std::vector<cocos2d::Label *>::iterator it = labelList.begin(); it != labelList.end(); it++) {
			this->removeChild(*it);
		}

		labelList.clear();
	}
}

void HelloWorld::createNewLabel(const std::string &keyStr) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF(keyStr, "fonts/Marker Felt.ttf", 24);

	auto r = rand() % 255;
	auto g = rand() % 255;
	auto b = rand() % 255;
	label->setColor(Color3B(r, g, b));

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

	auto physicsBody = PhysicsBody::createBox(Size(label->getContentSize().width, label->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));

	// add the label as a child to this layer
	this->addChild(label, 1);
	labelList.push_back(label);

	label->setPhysicsBody(physicsBody);
}

const std::string HelloWorld::_getKeyCodeAsString(cocos2d::EventKeyboard::KeyCode keyCode) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
		return "A";
	case EventKeyboard::KeyCode::KEY_B:
		return "B";
	case EventKeyboard::KeyCode::KEY_C:
		return "C";
	case EventKeyboard::KeyCode::KEY_D:
		return "D";
	case EventKeyboard::KeyCode::KEY_E:
		return "E";
	case EventKeyboard::KeyCode::KEY_F:
		return "F";
	case EventKeyboard::KeyCode::KEY_G:
		return "G";
	case EventKeyboard::KeyCode::KEY_H:
		return "H";
	case EventKeyboard::KeyCode::KEY_I:
		return "I";
	case EventKeyboard::KeyCode::KEY_J:
		return "J";
	case EventKeyboard::KeyCode::KEY_K:
		return "K";
	case EventKeyboard::KeyCode::KEY_L:
		return "L";
	case EventKeyboard::KeyCode::KEY_M:
		return "M";
	case EventKeyboard::KeyCode::KEY_N:
		return "N";
	case EventKeyboard::KeyCode::KEY_O:
		return "O";
	case EventKeyboard::KeyCode::KEY_P:
		return "P";
	case EventKeyboard::KeyCode::KEY_Q:
		return "Q";
	case EventKeyboard::KeyCode::KEY_R:
		return "R";
	case EventKeyboard::KeyCode::KEY_S:
		return "S";
	case EventKeyboard::KeyCode::KEY_T:
		return "T";
	case EventKeyboard::KeyCode::KEY_U:
		return "U";
	case EventKeyboard::KeyCode::KEY_V:
		return "V";
	case EventKeyboard::KeyCode::KEY_W:
		return "W";
	case EventKeyboard::KeyCode::KEY_X:
		return "X";
	case EventKeyboard::KeyCode::KEY_Y:
		return "Y";
	case EventKeyboard::KeyCode::KEY_Z:
		return "Z";
	case EventKeyboard::KeyCode::KEY_0:
		return "0";
	case EventKeyboard::KeyCode::KEY_1:
		return "1";
	case EventKeyboard::KeyCode::KEY_2:
		return "2";
	case EventKeyboard::KeyCode::KEY_3:
		return "3";
	case EventKeyboard::KeyCode::KEY_4:
		return "4";
	case EventKeyboard::KeyCode::KEY_5:
		return "5";
	case EventKeyboard::KeyCode::KEY_6:
		return "6";
	case EventKeyboard::KeyCode::KEY_7:
		return "7";
	case EventKeyboard::KeyCode::KEY_8:
		return "8";
	case EventKeyboard::KeyCode::KEY_9:
		return "9";
	default:
		return "";
	}
}

const std::string HelloWorld::getKeyCodeAsString(cocos2d::EventKeyboard::KeyCode keyCode) {
	std::string key(_getKeyCodeAsString(keyCode));

	if (isKeyPressed(EventKeyboard::KeyCode::KEY_SHIFT)) {
		std::transform(key.begin(), key.end(), key.begin(), ::toupper);
	}
	else {
		std::transform(key.begin(), key.end(), key.begin(), ::tolower);
	}

	return key;
}