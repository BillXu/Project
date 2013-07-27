//
//  GoldAppDelegate.cpp
//  Gold
//
//  Created by Xu BILL on 13-7-25.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "HelloWorldScene.h"
#include "RoomLayer.h"
USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());

    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    std::vector<std::string> vSearch ;
    vSearch.push_back("Image/") ;
    vSearch.push_back("Image/ccbi/") ;
    CCFileUtils::sharedFileUtils()->setSearchPaths(vSearch) ;
    vSearch.clear() ;
    vSearch.push_back("resources-iphonehd/") ;
    CCFileUtils::sharedFileUtils()->setSearchResolutionsOrder(vSearch) ;
    float fHfactor = 640.0 / CCDirector::sharedDirector()->getWinSize().height ;
    float fWfactor = 960.0 / CCDirector::sharedDirector()->getWinSize().width ;
    CCDirector::sharedDirector()->setContentScaleFactor( fHfactor > fWfactor ? fHfactor :fWfactor );
    // create a scene. it's an autorelease object
    CCScene *pScene = CRoomLayer::RoomScene();
    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
