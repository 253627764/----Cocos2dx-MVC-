#ifndef __GameDefine_H__
#define __GameDefine_H__
/* 背景 */
#define BACKGROUND0_PLIST                       "background.plist"
#define BACKGROUND0_IMAGE                       "background.pvr.gz"
#define BACKGROUND_FRAMENAME                    "map2.png"

#define BACKGROUND_PLIST						"tbackground.plist"
#define BACKGROUND_IMAGE                        "tbackground.png"
#define BACKGROUND_IMAGE_FORMAT                 "%d.png"
#define BACKGROUND_SAMPLE_IMAGE_ONE             "2.PNG"
#define BACKGROUND_SAMPLE_IMAGE_TWO             "5.PNG"
#define BACKGROUNDE_SAMPLE_IMAGE_THRE           "11.PNG"
/* 整体图像 */
#define GLOBAL_PLIST                            "custom.plist"
#define GLOBAL_IMAGE                            "custom.png"
/* HP血量 */
#define HP_IMAGE                                "mp.png"
/* 暂停按钮 */
#define PAUSE_IMAGE                             "stop.png"
/*字体*/
#define NUM_FONT_IMAGE                          "font.png"
#define FONT_WIDTH                              83
#define FONT_HEIGHT                             78
/* 金币 */
#define COIN_ANIMATE_NAME                       "coinAnimate"
#define COIN_ANIMATE_FORMAT                     "coin_%d.png"
/* 用户飞机 */
#define USERPLANE_MAX_SCALE                     3.0f
#define USERPLANE_READY_MSG                     "userPlaneIsReadyMessage"
#define USERPLANE_HP_CHANGE_MSG                 "userPlaneHpChangeMessage"
#define USERPLANE_CONFIG_PLIST                  "userPlane/userPlane.plist"
#define USERPLANE_DEATH_ANIMATE_NAME            "userPlaneDeathAnimate"
#define USERPLANE_DEATH_ANIMATE_FORMAT          "planeDeathAnimateFrame_%d.png"
#define USERPLANE_DEFAULT_EMIT_SE_FILE          "userPlane/userPlaneDefaultEmit.caf"
#define USERPLANE_DEAD_MESSAGE                  "userPlaneDeadMessage"
#define USERPLANE_TAG                           1
/* 敌机 */
#define ENEMYPLANE_HP_CHANGE_MSG                "enemyPlaneHpChangeMessage"
#define ENEMYPLANE_PLIST                        "userPlane/makeEnemyPlanes.plist"
#define ENEMYPLANE_DEATH_ANIMATE_NAME           "enemyPlaneDeathAnimate"
#define ENEMYPLANE_DEATH_ANIMATE_FORMAT         "deathAnimate_%d.png"
#define ENEMYPLANE_DEAD_MESSAGE                 "enemyPlaneDeadMessage"
#define ENEMYPLANE_TAG                          2
/* 碰撞特效粒子 */
#define  NORMAL_HIT_PARTICLE_PLIST              "userPlane/hit.plist" 
/* 飞机、炮塔、子弹的层次*/
#define USER_PLANE_ZORDER                       1000
#define ENEMY_PLANE_ZORDER                      999
#define BATTERY_ZORDER                          1001
#define FLY_BULLTE_ZORDER                       9980
/*
  常用头文件
*/
//SYSTEM
//1 #include "System/GameDefine.h"
//2 #define INCLUDE_APP_CONFIG                      #include "System/AppConfig.h"
//BASE
//1 #define INCLUDE_GAME_OBJECT                     #include "Base/GameObject.h""
//2 #define INCLUDE_HP_OBJECT                       #include "Base/HpObject.h"
//EXTENSION//PLANE//BATTERY//BULLTE
//1 #define INCLUDE_PLANE                           #include "Plane/Plane.h"
//2 #define INCLUDE_BATTERY                         #include "Battery/Battery.h"
//3 #define INCLUDE_BULLTE                          #include "Bullte/Bullte.h"
//PARTS//LAYER
//1 #define INCLUDE_GAME_WORLD_LAYER                #include "Layer/GameWorldLayer.h"

/* 包含常用的头文件及命名空间 */
#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Conv.h"
#include <string>
#include <math.h>

USING_NS_CC;
using namespace CocosDenshion;
using namespace std;

#endif