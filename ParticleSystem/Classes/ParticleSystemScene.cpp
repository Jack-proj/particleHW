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

	//for bubble
	auto bubblepos = (Sprite *)(rootNode->getChildByName("Button_bubble"));
	Point bubbleloc = bubblepos->getPosition();
	bubblepos->setVisible(false);
	_bubbleBtn = CSwitchButton::create();
	_bubbleBtn->setButtonInfo("bubble.png", "bubble.png", "bubble.png", bubbleloc);
	this->addChild(_bubbleBtn, 2);

	//for circle
	auto circlepos = (Sprite *)(rootNode->getChildByName("Button_circle"));
	Point circleloc = circlepos->getPosition();
	circlepos->setVisible(false);
	_circleBtn = CSwitchButton::create();
	_circleBtn->setButtonInfo("circle.png", "circle.png", "circle.png", circleloc);
	this->addChild(_circleBtn, 2);

	//for cloud
	auto cloudpos = (Sprite *)(rootNode->getChildByName("Button_cloud"));
	Point cloudloc = cloudpos->getPosition();
	cloudpos->setVisible(false);
	_cloudBtn = CSwitchButton::create();
	_cloudBtn->setButtonInfo("cloud.png", "cloud.png", "cloud.png", cloudloc);
	this->addChild(_cloudBtn, 2);

	//for comet
	auto cometpos = (Sprite *)(rootNode->getChildByName("Button_comet"));
	Point cometloc = cometpos->getPosition();
	cometpos->setVisible(false);
	_cometBtn = CSwitchButton::create();
	_cometBtn->setButtonInfo("comet.png", "comet.png", "comet.png", cometloc);
	this->addChild(_cometBtn, 2);

	//for flare
	auto flarepos = (Sprite *)(rootNode->getChildByName("Button_flare"));
	Point flareloc = flarepos->getPosition();
	flarepos->setVisible(false);
	_flareBtn = CSwitchButton::create();
	_flareBtn->setButtonInfo("flare.png", "flare.png", "flare.png", flareloc);
	this->addChild(_flareBtn, 2);

	//for raindrop
	auto raindroppos = (Sprite *)(rootNode->getChildByName("Button_raindrop"));
	Point raindroploc = raindroppos->getPosition();
	raindroppos->setVisible(false);
	_raindropBtn = CSwitchButton::create();
	_raindropBtn->setButtonInfo("raindrop.png", "raindrop.png", "raindrop.png", raindroploc);
	this->addChild(_raindropBtn, 2);

	//for spark
	auto sparkpos = (Sprite *)(rootNode->getChildByName("Button_spark"));
	Point sparkloc = sparkpos->getPosition();
	sparkpos->setVisible(false);
	_sparkBtn = CSwitchButton::create();
	_sparkBtn->setButtonInfo("spark.png", "spark.png", "spark.png", sparkloc);
	this->addChild(_sparkBtn, 2);


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
	
	// Slider of Spin
	auto *SpinSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spin"));
	SpinSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpinEvent, this));
	SpinSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_SpinBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpinBMFont");

	// Slider of Opacity
	auto *OpacitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Opacity"));
	OpacitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::OpacityEvent, this));
	OpacitySlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_OpacityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("OpacityBMFont");

	// Slider of Particles
	auto *ParticlesSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Particles"));
	ParticlesSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ParticlesEvent, this));
	ParticlesSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_ParticlesBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ParticlesBMFont");

	// Slider of Speed
	auto *SpeedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Speed"));
	SpeedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpeedEvent, this));
	SpeedSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_SpeedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpeedBMFont");

	// Slider of Lifetime
	auto *LifetimeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Lifetime"));
	LifetimeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::LifetimeEvent, this));
	LifetimeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_LifetimeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("LifetimeBMFont");

	// Slider of Red
	auto *RedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Red"));
	RedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::RedEvent, this));
	RedSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_RedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("RedBMFont");

	// Slider of Green
	auto *GreenSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Green"));
	GreenSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GreenEvent, this));
	GreenSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_GreenBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GreenBMFont");

	// Slider of Blue
	auto *BlueSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Blue"));
	BlueSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::BlueEvent, this));
	BlueSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_BlueBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("BlueBMFont");
	
	// Slider of Wind
	auto *WindSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Wind"));
	WindSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindEvent, this));
	WindSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_WindBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindBMFont");

	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 0 到 360 之間
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");

	//Slider of EmmiterType
	auto *ETSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_ET"));
	ETSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ETEvent, this));
	ETSlider->setMaxPercent(100); 	// 將 0 到 100 對應到 100 到 110 之間
	_ETBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ETBMFont");



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
	unsigned int eid;
	eid = SimpleAudioEngine::getInstance()->playEffect("CRAM_MOVE.mp3", true);
	SimpleAudioEngine::getInstance()->stopEffect(eid);

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
	//bubble
	if (_bubbleBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("bubble.png", *this);
	}
	//circle
	if (_circleBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("circle.png", *this);
	}
	//cloud
	if (_cloudBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("cloud.png", *this);
	}
	//comet
	if (_cometBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("comet.png", *this);
	}
	//flare
	if (_flareBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("flare.png", *this);
	}
	//raindrop
	if (_raindropBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("raindrop.png", *this);
	}
	//spark
	if (_sparkBtn->touchesBegan(touchLoc))
	{
		_ParticleControl.setNewPic("spark.png", *this);
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

void ParticleSystemScene::SpinEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpin = percent*3.6f; // 0 到 360 之間
		_SpinBMValue->setString(StringUtils::format("%2.1f", fSpin));
		_ParticleControl.setSpin(fSpin);
	}
}

void ParticleSystemScene::OpacityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fOpacity = percent * 2.55f;
		// 透過 _GravityBMValue 顯示在畫面上
		_OpacityBMValue->setString(StringUtils::format("%2.1f", fOpacity));
		_ParticleControl.setOpacity(fOpacity);
	}
}

void ParticleSystemScene::ParticlesEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 40 之間
		int iParticles = percent * 2;
		// 透過 _GravityBMValue 顯示在畫面上
		_ParticlesBMValue->setString(StringUtils::format("%d", iParticles));
		_ParticleControl._iNumParticles = iParticles;
	}
}

void ParticleSystemScene::SpeedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 40 之間
		float fSpeed = percent / 2.5f;
		// 透過 _GravityBMValue 顯示在畫面上
		_SpeedBMValue->setString(StringUtils::format("%2.1f", fSpeed));
		_ParticleControl.setSpeed(fSpeed);
	}
}

void ParticleSystemScene::LifetimeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fLifetime = percent / 20.0f;
		// 透過 _GravityBMValue 顯示在畫面上
		_LifetimeBMValue->setString(StringUtils::format("%2.1f", fLifetime));
		_ParticleControl._fLifeTime= fLifetime;
	}
}

void ParticleSystemScene::RedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fRed = percent * 2.55f;
		// 透過 _GravityBMValue 顯示在畫面上
		_RedBMValue->setString(StringUtils::format("%2.1f", fRed));
		_ParticleControl.setRed(fRed);
	}
}

void ParticleSystemScene::GreenEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fGreen = percent * 2.55f;
		// 透過 _GravityBMValue 顯示在畫面上
		_GreenBMValue->setString(StringUtils::format("%2.1f", fGreen));
		_ParticleControl.setGreen(fGreen);
	}
}

void ParticleSystemScene::BlueEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fBlue = percent * 2.55f;
		// 透過 _GravityBMValue 顯示在畫面上
		_BlueBMValue->setString(StringUtils::format("%2.1f", fBlue));
		_ParticleControl.setBlue(fBlue);
	}
}

void ParticleSystemScene::WindEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type) {
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED) {
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent(); // 捲動鈕目前的位置 0 ~ 100
		// 將 0 到 100 對應到 0 到 255 之間
		float fWind = (-50.0f + percent) / 5.0f;
		// 透過 _GravityBMValue 顯示在畫面上
		_WindBMValue->setString(StringUtils::format("%2.1f", fWind));
		_ParticleControl.setWind(fWind);
	}
}

void ParticleSystemScene::TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iType = percent/10; // 0 到 360 之間
		_TypeBMValue->setString(StringUtils::format("%2d", iType));
		_ParticleControl.setType(iType);
	}
}

void ParticleSystemScene::ETEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iET = (percent / 10)+100; // 0 到 360 之間
		_ETBMValue->setString(StringUtils::format("%2d", iET));
		_ParticleControl.setEType(iET);
	}
}