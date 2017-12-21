#include "ParticleSystemScene.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* ParticleSystemScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ParticleSystemScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

ParticleSystemScene::~ParticleSystemScene()
{
	this->removeAllChildren();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool ParticleSystemScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);

	// For Emitter
	_EmitterSprite = CDraggableSprite::create();
	_EmitterSprite->setSpriteInfo("emittericon.png", Point(-125.0f + visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	_EmitterSprite->setVisible(false);
	_bEmitterOn = false;
	this->addChild(_EmitterSprite, 5);

	auto emiterpos = (Sprite *)(rootNode->getChildByName("emitterpos"));
	Point loc = emiterpos->getPosition();
	emiterpos->setVisible(false);
	_emitterSwitchBtn = CSwitchButton::create();
	_emitterSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_emitterSwitchBtn, 2);

	// Particle Control System
	// 最好的方式是，以下的 Slider 根據這裡的設定值，顯示出正確的數值與位置
	_ParticleControl.init(*this);
	_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	_ParticleControl._fDir = 90.0;
	_ParticleControl._iNumParticles = 100;
	_ParticleControl._iGenParticles = 0;
	_ParticleControl._fSpread = 180.0f;
	_ParticleControl._fVelocity = 2.5;	 // 分子的離開速度
	_ParticleControl._fLifeTime = 3.5f;	 // 分子的存活時間
	_ParticleControl._fSpin = 0;
	_ParticleControl._fGravity = 0;
	_ParticleControl._fElpasedTime = 0;
	_ParticleControl.setType(STAY_FOR_TWOSECONDS); // 分子運動的型態，預設為 0
	_ParticleControl._windDir = Point(0, 0); // 本範例沒有實作此項功能

	// 分子的可調整參數的捲動軸
	// Slider of Gravity
	auto *GravitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Gravity"));
	GravitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GravityEvent, this));
	GravitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 -10 到 10 之間
	_GravityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GravityBMFont");

	// Slider of Spread
	auto *SpreadSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spread"));
	SpreadSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpreadEvent, this));
	SpreadSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 180 之間
	_SpreadBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpreadBMFont");

	// Slider of Direction
	auto *DirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Direction"));
	DirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::DirectionEvent, this));
	DirectionSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_DirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("DirectionBMFont");


	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");


	_listener1 = EventListenerTouchOneByOne::create();	//創建一個一對一的事件聆聽器
	_listener1->onTouchBegan = CC_CALLBACK_2(ParticleSystemScene::onTouchBegan, this);		//加入觸碰開始事件
	_listener1->onTouchMoved = CC_CALLBACK_2(ParticleSystemScene::onTouchMoved, this);		//加入觸碰移動事件
	_listener1->onTouchEnded = CC_CALLBACK_2(ParticleSystemScene::onTouchEnded, this);		//加入觸碰離開事件

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//加入剛創建的事件聆聽器
	this->schedule(CC_SCHEDULE_SELECTOR(ParticleSystemScene::doStep));

    return true;
}

void ParticleSystemScene::doStep(float dt)
{
	_ParticleControl.doStep(dt);
}

bool ParticleSystemScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//觸碰開始事件
{
	Point touchLoc = pTouch->getLocation();
	//顯示 Emitter 時，可拖曳該圖式
	if( _bEmitterOn ) _EmitterSprite->touchesBegan(touchLoc);
	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子
	if ( !_emitterSwitchBtn->touchesBegan(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesBegan(touchLoc);
	return true;
}

void  ParticleSystemScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰移動事件
{
	Point touchLoc = pTouch->getLocation();	
	if (_bEmitterOn) {
		if (_EmitterSprite->touchesMoved(touchLoc)) // 移動並更新 Emitter 的位置
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}
	// 沒有顯示 Emitter，而且沒有按在 Emitter 切換按鈕上，才讓 touch 可以點選顯示分子
	if ( !_emitterSwitchBtn->touchesMoved(touchLoc) && !_bEmitterOn ) _ParticleControl.onTouchesMoved(touchLoc);
}

void  ParticleSystemScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //觸碰結束事件 
{
	Point touchLoc = pTouch->getLocation();
	if (_bEmitterOn) {
		if( _EmitterSprite->touchesEnded(touchLoc) ) 
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}
	// 點在 Emitter 切換的圖示上，進行必要的狀態改變
	if (_emitterSwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitterOn = _emitterSwitchBtn->getStatus();
		if ( _bEmitterOn ) { // 顯示 Emitter 圖示
			_EmitterSprite->setVisible(true);			
		}
		else { // 關閉 Emitter 圖示
			_EmitterSprite->setVisible(false);
		}
		_ParticleControl.setEmitter(_bEmitterOn); // 更新控制系統中的 Emitter 狀態
	}
}

void ParticleSystemScene::GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		float fGravity = (-50.0f + percent) / 5.0f;
		_GravityBMValue->setString(StringUtils::format("%2.1f", fGravity));
		_ParticleControl.setGravity(fGravity);
	}
}

void ParticleSystemScene::SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpread = percent*1.8f; // 0 到 180 之間
		_SpreadBMValue->setString(StringUtils::format("%2.1f", fSpread));
		_ParticleControl._fSpread = fSpread;
	}
}

void ParticleSystemScene::DirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fDir = percent*3.6f; // 0 到 360 之間
		_DirectionBMValue->setString(StringUtils::format("%2.1f", fDir));
		_ParticleControl._fDir = fDir;
	}
}


void ParticleSystemScene::TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iType = percent/20; // 0 到 360 之間
		_TypeBMValue->setString(StringUtils::format("%2d", iType));
		_ParticleControl.setType(iType);
	}
}