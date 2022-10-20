#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <windows.h>
#include <iostream>
#include <sstream>
using namespace sf;

#define GRIDSIZE 25
#define LVL1_WIDTH 9
#define LVL1_HEIGHT 9
#define LVL1_NUM 10
#define LVL2_WIDTH 16
#define LVL2_HEIGHT 16
#define LVL2_NUM 40
#define LVL3_WIDTH 30
#define LVL3_HEIGHT 16
#define LVL3_NUM 99

// 枚举定义网格状态
enum GRIDSTATE
{
    ncNULL,    //空地
    ncUNDOWN,  //背景方块
    ncMINE,    //地雷
    ncONE,     //数字1
    ncTWO,     //数字2
    ncTHREE,   //数字3
    ncFOUR,    //数字4
    ncFIVE,    //数字5
    ncSIX,     //数字6
    ncSEVEN,   //数字7
    ncEIGHT,   //数字8
    ncFLAG,    //标记
    ncQ,       //问号
    ncX,       //双击检测八邻域状态
    ncBOMBING, //爆炸的雷
    ncUNFOUND  //未检测出来的雷
};

enum GAMEOVERSTATE
{
    ncNo,  //游戏未结束
    ncWIN, //游戏胜利
    ncLOSE //游戏失败
};

enum MOUSEFUNCTION
{
    NULLClick,
    RButtonDownFunc,
    RButtonReleasedFunc,
    LButtonDownFunc,
    LButtonReleasedFunc
};

class LEI
{
public:
    int mState;       //雷的状态
    int mStateBackUp; //备份状态
    bool isPress;     //雷是否被按下
};

class Game
{
public:
    RenderWindow window;
    Game();
    ~Game();
    bool gameOver, gameQuit;
    bool mouseDlbClkReady;
    int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;
    int gamelvl, mTime;                     //游戏难度 游戏计时
    LEI mGameData[LVL3_HEIGHT][LVL3_WIDTH]; //游戏取最高难度的舞台尺寸
    bool isGameBegin;                       //游戏是否开始
    int isGameOverState;                    //游戏结束的状态
    Vector2i mCornPoint;                    //舞台左顶点的坐标
    int gridSize;                           //块的大小
    int imgBGNo = 1, imgSkinNo = 1;
    int mouseAction;
    bool isMineSetBegin;
    Vector2i mousePoint, P1, P2;
    Texture tBackground, tTiles, tButtons, tButtons_h, tButtons_p, tNum, tTimer, tCounter, tGameOver; //创建纹理对象
    Sprite sBackground, sTiles, sButtons, sButtons_h, sButtons_p, sNum, sTimer, sCounter, sGameOver;  //创建精灵对象
    bool isButtonPress = false;
    IntRect ButtonRectEasy, ButtonRectNormal, ButtonRectHard, ButtonRectBG, ButtonRectSkin, ButtonRectRestart, ButtonRectQuit;

    SoundBuffer sbWin, sbBoom;
    Sound soundWin, soundBoom;
    Music bkMusic;
    bool RL_ClkJudge_flag = false; // 是否开启双击邻域判断模式
    int mouse_RL_ClkReady = 0;
    Vector2i RL_Point;
    // SFML 的 Clock 类在对象实例化的时候就开始计时
    Clock gameClock, mouseClickTimer;
    void Run(); // 游戏运行入口函数

    void Initial(); //游戏初始化
    void IniData();
    void LoadMediaData();
    void MineSet(int Py, int Px); // 布雷

    void Input();                        //输入函数
    void RButtonDown(Vector2i mPoint);   // 鼠标右击
    void LButtonDown(Vector2i mPoint);   // 鼠标左击
    void LButtonDblClk(Vector2i mPoint); // 鼠标左键双击

    void NullClick(int x, int y); // 八邻域填色法查找空块
    void Logic();                 // 游戏主逻辑判断
    void isWin();
    void unCover();

    void Draw(); //游戏绘制主函数
    void DrawGrid();
    void DrawButton();
    void DrawScore();
    void DrawTimer();
    void DrawGameEnd();

    void RL_ButtonDown(Vector2i mPoint);
    void RL_ClkJudge();
    void undownOpen(); // 去除所有覆盖
};