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
    if ( !LayerColor::initWithColor(Color4B(255, 255, 255, 255)))
    {
        return false;
    }
    
	auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto texture = Director::getInstance()->getTextureCache()->addImage("Sprites/sfx/explosion.png");


	initPhysics();
	initListeners();

    return true;
}

void HelloWorld::initListeners() {
	auto eventListener = cocos2d::EventListenerKeyboard::create();
	eventListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	eventListener->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListener, this);

	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	contactListener->onContactPostSolve = CC_CALLBACK_2(HelloWorld::onPostContactSolve, this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
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

bool HelloWorld::onPostContactSolve(PhysicsContact &contact, const cocos2d::PhysicsContactPostSolve &solve) {


	//v1 = dot product velocity 1 and normal collision vector.
	//v2 = dot product velocity 2 and normal collision vector.
	//Impulse = m1v1 - m2v2
	/*CCLOG("Post solve");

	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	auto velocityA = bodyA->getVelocity();
	CCLOG("Post solve Object A Velocity %f %f", velocityA.x, velocityA.y);

	auto velocityB = bodyB->getVelocity();
	CCLOG("Post solveObject B Velocity %f %f", velocityB.x, velocityB.y);*/

	return true;
}

bool HelloWorld::onContactBegin(const PhysicsContact &contact)

{
	//auto veloc
	//auto audio = CocosDenshion::SimpleAudioEngine::getInstance();

	// play a sound effect, just once.
	//audio->playEffect("sounds/sfx/boing.wav", false, 1.0f, 1.0f, 1.0f);

	auto data = contact.getContactData();
	auto normal = data->normal;

	//CCLOG("Colision detected normal %f %f", data->normal.x, data->normal.y);
	auto bodyA = contact.getShapeA()->getBody();
	auto bodyB = contact.getShapeB()->getBody();

	auto ObjA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto ObjB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	auto colorA = ObjA->getColor();
	auto velocityA = bodyA->getVelocity();
	//CCLOG("Object A Velocity %f %f", velocityA.x, velocityA.y);

	auto colorB = ObjB->getColor();
	auto velocityB = bodyB->getVelocity();
	
	//CCLOG("Object B Velocity %f %f", velocityB.x, velocityB.y);
	
	//v1 = dot product velocity 1 and normal collision vector.
	//v2 = dot product velocity 2 and normal collision vector.
	//Impulse = m1v1 - m2v2

	auto v1 = velocityA.x * normal.x + velocityA.y * normal.y;
	auto v2 = velocityB.x * normal.x + velocityB.y * normal.y;
	auto impulse = bodyA->getMass() * v1 - bodyB->getMass() * v2;

	CCLOG("Impulse %f", abs(impulse));

	return true;
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
	//CCLOG("Key released");

	keyMap.erase(keyCode);
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) {
	//CCLOG("Key Pressed");

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
		auto label = createNewLabel(keyStr);
		createNodeParticles(label);
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

void HelloWorld::createNodeParticles(cocos2d::Node *node) {

	auto emitter = ParticleFireworks::createWithTotalParticles(500);

	//auto texture = Director::getInstance()->getTextureCache()->getTextureForKey("Sprites/sfx/explosion.png");

	//emitter->setTexture(texture);
	emitter->setDuration(0.25);
	//emitter->setGravity(Vec2(0, 98.f));
	//emitter->setEmitterMode(ParticleSystem::Mode::GRAVITY);

	emitter->setSpeed(320);
	emitter->setSpeedVar(20);

	//emitter->setRadialAccel(-120);
	//emitter->setRadialAccelVar(0);

	emitter->setTangentialAccel(30);
	emitter->setTangentialAccelVar(0);

	emitter->setAngle(270);
	emitter->setAngleVar(90);

	//emitter->setPosition(Vec2(160, 240));
	//emitter->setPosVar(Vec2(0, 0));

	//emitter->setSourcePosition(Vec2(160, 240));

	emitter->setLife(0.5);
	emitter->setLifeVar(0.25);

	emitter->setStartSpin(30);
	emitter->setStartSpinVar(0);
	emitter->setEndSpin(60);
	emitter->setEndSpinVar(0);

	emitter->setStartSize(3);
	emitter->setStartSizeVar(2);
	emitter->setEndSize(10);
	emitter->setEndSizeVar(4);
	emitter->setEmissionRate(emitter->getTotalParticles() / emitter->getLife());

	//emitter->setBlendAdditive(true);

	this->addChild(emitter);

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	emitter->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - node->getContentSize().height));
}

cocos2d::Label * HelloWorld::createNewLabel(const std::string &keyStr) {
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto label = Label::createWithTTF(keyStr, "fonts/Marker Felt.ttf", 24);

	Color4B palette[] = { 
		Color4B(0xFF, 0xFF, 0x30, 0xFF), Color4B(0xFB, 0x8D, 0x2D, 0xFF), Color4B(0xF6, 0x2A, 0x37, 0xFF), Color4B(0xF1, 0x27, 0x9F, 0xFF), 
		Color4B(0xD2, 0x25, 0xEB, 0xFF), Color4B(0x65, 0x23, 0xE5, 0xFF),
		Color4B(0x22, 0x46, 0xDF, 0xFF), Color4B(0x24, 0xA4, 0xD5, 0xFF), Color4B(0x28, 0xC9, 0xA0, 0xFF), Color4B(0x2C, 0xBE, 0x4B, 0xFF)};
	int color = rand() % 10;
	//int color = 0;
	label->setColor(Color3B(palette[color]));
	//label->setTextColor(palette[color]);
	label->enableOutline(Color4B(0, 0, 0, 255), 2);
	//label->enableGlow(Color4B(255, 0, 0, 255));
	label->enableShadow(Color4B(0, 0, 0, 128), Size(2, 2), 25);

	// position the label on the center of the screen
	label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - label->getContentSize().height));

	auto physicsBody = PhysicsBody::createBox(Size(label->getContentSize().width, label->getContentSize().height), PhysicsMaterial(0.1f, 1.0f, 0.0f));
	physicsBody->setContactTestBitmask(0x1);

	// add the label as a child to this layer
	this->addChild(label, 1);
	labelList.push_back(label);

	label->setPhysicsBody(physicsBody);

	auto fadeOut = Sequence::create(
		FadeOut::create(4.0f),
		RemoveSelf::create(true),
		nullptr
	);
	label->runAction(fadeOut);

	return label;
}

const std::string HelloWorld::_getKeyCodeAsString(cocos2d::EventKeyboard::KeyCode keyCode) {
	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_A:
		return "amir";
	case EventKeyboard::KeyCode::KEY_B:
		return "book";
	case EventKeyboard::KeyCode::KEY_C:
		return "color";
	case EventKeyboard::KeyCode::KEY_D:
		return "daddy";
	case EventKeyboard::KeyCode::KEY_E:
		return "yes";
	case EventKeyboard::KeyCode::KEY_F:
		return "hug";
	case EventKeyboard::KeyCode::KEY_G:
		return "giraffe";
	case EventKeyboard::KeyCode::KEY_H:
		return "home";
	case EventKeyboard::KeyCode::KEY_I:
		return "lila";
	case EventKeyboard::KeyCode::KEY_J:
		return "puppy";
	case EventKeyboard::KeyCode::KEY_K:
		return "couch";
	case EventKeyboard::KeyCode::KEY_L:
		return "logan";
	case EventKeyboard::KeyCode::KEY_M:
		return "mommy";
	case EventKeyboard::KeyCode::KEY_N:
		return "night night";
	case EventKeyboard::KeyCode::KEY_O:
		return "outside";
	case EventKeyboard::KeyCode::KEY_P:
		return "please";
	case EventKeyboard::KeyCode::KEY_Q:
		return "potty";
	case EventKeyboard::KeyCode::KEY_R:
		return "poo poo";
	case EventKeyboard::KeyCode::KEY_S:
		return "pee pee";
	case EventKeyboard::KeyCode::KEY_T:
		return "thank you";
	case EventKeyboard::KeyCode::KEY_U:
		return "swing";
	case EventKeyboard::KeyCode::KEY_V:
		return "play";
	case EventKeyboard::KeyCode::KEY_W:
		return "we";
	case EventKeyboard::KeyCode::KEY_X:
		return "share";
	case EventKeyboard::KeyCode::KEY_Y:
		return "friends";
	case EventKeyboard::KeyCode::KEY_Z:
		return "nana";
	case EventKeyboard::KeyCode::KEY_0:
		return "zero";
	case EventKeyboard::KeyCode::KEY_1:
		return "one";
	case EventKeyboard::KeyCode::KEY_2:
		return "two";
	case EventKeyboard::KeyCode::KEY_3:
		return "three";
	case EventKeyboard::KeyCode::KEY_4:
		return "four";
	case EventKeyboard::KeyCode::KEY_5:
		return "five";
	case EventKeyboard::KeyCode::KEY_6:
		return "six";
	case EventKeyboard::KeyCode::KEY_7:
		return "seven";
	case EventKeyboard::KeyCode::KEY_8:
		return "eight";
	case EventKeyboard::KeyCode::KEY_9:
		return "nine";
	case EventKeyboard::KeyCode::KEY_SPACE:
		return "love";
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