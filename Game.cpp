#include "Game.h"
#include <iostream>
using namespace std;

Game::Game()
{
    Window_width = 860;
    Window_height = 600;

    gamelvl = 2;
    window.create(sf::VideoMode(Window_width, Window_height), L"Mine-Sweeper", Style::Close);
}
Game::~Game()
{
}

void Game::unCover()
{
    int i, j;
    for (j = 0; j < stageHeight; j++)
        for (i = 0; i < stageWidth; i++)
            if (mGameData[j][i].isPress == false)
            {
                mGameData[j][i].isPress = true;
                if (mGameData[j][i].mState == ncMINE)
                    mGameData[j][i].mState = ncUNFOUND;
            }
}

void Game::undownOpen()
{
    int i, j;
    for (j = 0; j < stageWidth; j++)
        for (i = 0; i < stageHeight; i++)
            if (mGameData[j][i].isPress == false)
            {
                mGameData[j][i].isPress = true;
                // 没有插上 🚩 的雷默认插上了 🚩
                if (mGameData[j][i].mState = ncMINE)
                    mGameData[j][i].mState = ncFLAG;
            }
}

void Game::Initial()
{
    window.setFramerateLimit(10); // 每秒设置帧频

    gridSize = GRIDSIZE; //点击的位置的块的大小

    switch (gamelvl)
    {
    case 1:
        stageWidth = LVL1_WIDTH;
        stageHeight = LVL1_HEIGHT;
        mMineNum = LVL1_NUM;
        // 简单难度格子要放大
        gridSize = GRIDSIZE * LVL2_WIDTH / LVL1_WIDTH;
        break;
    case 2:
        stageWidth = LVL2_WIDTH;
        stageHeight = LVL2_HEIGHT;
        mMineNum = LVL2_NUM;
        break;
    case 3:
        stageWidth = LVL3_WIDTH;
        stageHeight = LVL3_HEIGHT;
        mMineNum = LVL3_NUM;
        break;
    default:
        break;
    }

    gameOver = false;
    gameQuit = false;
    isGameOverState = ncNo; //初始化游戏的结束状态
    mFlagCalc = 0;          //初始化棋子的数量
    isGameBegin = false;    //初始化游戏是否开始
    isMineSetBegin = false;
    mTime = 0; //初始化游戏进行的时间
    //设置舞台的左上角坐标
    mCornPoint.x = (Window_width - stageWidth * gridSize) / 2;
    mCornPoint.y = (Window_height - stageHeight * gridSize) / 2;
    IniData();       //初始化数据
    LoadMediaData(); //加载素材
}
void Game::IniData()
{
    int i, j;

    // 所有块儿置为空且未点击
    for (j = 0; j < stageHeight; j++)
    {
        for (i = 0; i < stageWidth; i++)
        {
            mGameData[j][i].mState = ncUNDOWN;
            // mGameData[j][i].mStateBackUp = ncUNDOWN;
            mGameData[j][i].isPress = false;
        }
    }
}
void Game::LoadMediaData()
{
    std::stringstream ss;
    ss << "./data/images/BK0" << imgBGNo << ".jpg";

    if (!tBackground.loadFromFile(ss.str()))
    {
        std::cout << "BK image 没有找到" << std::endl;
    }
    ss.str(""); //清空字符串
    ss << "./data/images/Game" << imgSkinNo << ".jpg";
    if (!tTiles.loadFromFile(ss.str()))
    {
        cout << "Game Skin Image 没有找到" << endl;
    }
    if (!tNum.loadFromFile("./data/images/num.jpg"))
    {
        cout << "Num.jpg 没有找到" << endl;
    }
    if (!tTimer.loadFromFile("./data/images/jishiqi.jpg"))
    {
        cout << "jishiqi.jpg 没有找到" << endl;
    }
    if (!tCounter.loadFromFile("./data/images/jishuqi.jpg"))
    {
        cout << "jishuqi.jpg 没有找到" << endl;
    }
    if (!tButtons.loadFromFile("./data/images/button.jpg"))
    {
        cout << "button.jpg 没有找到" << endl;
    }
    if (!tButtons_h.loadFromFile("./data/images/button_h.jpg"))
    {
        cout << "button_h.jpg 没有找到" << endl;
    }
    if (!tButtons_p.loadFromFile("./data/images/button_p.jpg"))
    {
        cout << "button_p.jpg 没有找到" << endl;
    }
    if (!tGameOver.loadFromFile("./data/images/gameover.jpg"))
    {
        cout << "gameover.jpg 没有找到" << endl;
    }

    sBackground.setTexture(tBackground);
    sTiles.setTexture(tTiles);
    if (gamelvl == 1)
    {
        float scale = 1.0 * LVL2_WIDTH / LVL1_WIDTH;
        // 难度 1 时，栅格尺寸需要放大
        sTiles.setScale(scale, scale);
        gridSize = GRIDSIZE * scale;
    }
    else
    {
        gridSize = GRIDSIZE;
        // 栅格尺寸取消缩放
        sTiles.setScale(1.0, 1.0);
    }
    sButtons.setTexture(tButtons);
    sButtons_h.setTexture(tButtons_h);
    sButtons_p.setTexture(tButtons_p);
    sNum.setTexture(tNum);
    sTimer.setTexture(tTimer);
    sCounter.setTexture(tCounter);
    sGameOver.setTexture(tGameOver);
}
void Game::MineSet(int Py, int Px)
{
    int mCount, i, j, k, l;
    mCount = 0;

    srand(time(NULL)); // 用当前系统时间作为随机数生成器的种子
    // 布雷
    do
    {
        bool flag = true;
        k = rand() % stageHeight;
        l = rand() % stageWidth;
        // if(k==Py && l==Px)
        // {
        //     continue;
        // }
        for (i = Py - 1; i < Py + 2; i++)
        {
            for (j = Px - 1; j < Px + 2; j++)
            {
                if (i >= 0 && i < stageHeight && j >= 0 && j < stageWidth)
                {
                    if (k == i && l == j)
                    {
                        flag = false;
                    }
                }
            }
        }
        if (mGameData[k][l].mState == ncUNDOWN)
        {
            mGameData[k][l].mState = ncMINE;
            mGameData[k][l].mStateBackUp = ncMINE; // 备份状态
            mCount++;
        }
    } while (mCount != mMineNum); // 避免首次在鼠标点击位置布雷

    // 方格赋值
    for (i = 0; i < stageHeight; i++)
        for (j = 0; j < stageWidth; j++)
            // 有雷就不用标数字了
            if (mGameData[i][j].mState != ncMINE)
            {
                mCount = 0;
                for (k = i - 1; k < i + 2; k++)
                {
                    for (l = j - 1; l < j + 2; l++)
                    {
                        // 计算 (i, j) 周围雷的数目
                        if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
                        {
                            if (mGameData[k][l].mState == ncMINE)
                                mCount++;
                        }
                    }
                }
                switch (mCount) // 保存状态
                {
                case 0:
                    mGameData[i][j].mState = ncNULL;
                    break;
                case 1:
                    mGameData[i][j].mState = ncONE;
                    break;
                case 2:
                    mGameData[i][j].mState = ncTWO;
                    break;
                case 3:
                    mGameData[i][j].mState = ncTHREE;
                    break;
                case 4:
                    mGameData[i][j].mState = ncFOUR;
                    break;
                case 5:
                    mGameData[i][j].mState = ncFIVE;
                    break;
                case 6:
                    mGameData[i][j].mState = ncSIX;
                    break;
                case 7:
                    mGameData[i][j].mState = ncSEVEN;
                    break;
                case 8:
                    mGameData[i][j].mState = ncEIGHT;
                    break;
                }
            }
}

void Game::Input()
{
    Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            window.close();
            gameQuit = true;
        }
        if (event.type == Event::EventType::KeyReleased && event.key.code == Keyboard::Escape)
        {
            window.close();
            gameQuit = true;
        }
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            mouseAction = LButtonDownFunc;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
            mouseAction = LButtonReleasedFunc;
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
            mouseAction = RButtonDownFunc;
        if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Right)
            mouseAction = RButtonReleasedFunc;
    }
}
void Game::RButtonDown(Vector2i mPoint) // 右键单击
{
    int i, j;
    i = (mPoint.x - mCornPoint.x) / gridSize;
    j = (mPoint.y - mCornPoint.y) / gridSize;

    // 如果点击是在舞台范围内
    if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
    {
        //重新开始
        if (isGameBegin == false)
        {
            isGameBegin = true;
            isMineSetBegin = true;
            MineSet(j, i);
            gameClock.restart();
        }
        //如果是未揭开状态，设立flag并且备份
        if (mGameData[j][i].isPress == false)
        {
            mGameData[j][i].isPress = true;
            mGameData[j][i].mStateBackUp = mGameData[j][i].mState;
            mGameData[j][i].mState = ncFLAG;
            mFlagCalc++;
        }
        else
        {
            // flag变成问号
            if (mGameData[j][i].mState == ncFLAG)
            {
                mGameData[j][i].isPress = true;
                mGameData[j][i].mState = ncQ;
                mFlagCalc--;
            }
            //问号变回未揭开状态
            else if (mGameData[j][i].mState == ncQ)
            {
                mGameData[j][i].isPress = false;
                mGameData[j][i].mState = mGameData[j][i].mStateBackUp;
            }
        }
    }
}
void Game::LButtonDown(Vector2i mPoint) // 左键单击
{
    int i, j;
    // 获取鼠标当前点击的块的位置
    i = (mPoint.x - mCornPoint.x) / gridSize;
    j = (mPoint.y - mCornPoint.y) / gridSize;

    // 如果点击是在舞台范围
    if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
    {
        // 如果游戏未开始
        if (isGameBegin == false)
        {
            isGameBegin = true; // 游戏开始
            gameClock.restart();
            // MineSet(j, i); // 点击之后随机布雷
        }
        // 如果未布雷
        if (isMineSetBegin == false)
        {
            isMineSetBegin = true;
            MineSet(j, i);
        }
        // 如果状态不是棋子
        if (mGameData[j][i].mState != ncFLAG)
        {
            if (mGameData[j][i].isPress == false)
            {
                mGameData[j][i].isPress = true;       // 当前块被点击
                if (mGameData[j][i].mState == ncMINE) // 如果当前为雷
                {
                    isGameBegin = false;
                    isGameOverState = ncLOSE;
                    mGameData[j][i].mState = ncBOMBING;
                    unCover(); // 揭开剩下未找到的雷
                }
            }
        }
        if (mGameData[j][i].mState == ncNULL) //如果当前块儿为空
            NullClick(j, i);                  // 查找未被点击的空块
    }
}
void Game::LButtonDblClk(Vector2i mpoint) // 左键双击
{
    int i, j, k, l, lvl;
    i = (mpoint.x - mCornPoint.x) / gridSize;
    j = (mpoint.y - mCornPoint.y) / gridSize;

    // 如果点击是在范围内
    if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
        // 如果已被点击
        if (mGameData[j][i].isPress == true)
            // 如果当前块不是棋子
            if (mGameData[j][i].mState != ncFLAG)
                // 遍历周围 8 个格子
                for (k = j - 1; k < j + 2; k++)
                    for (l = i - 1; l < i + 2; l++)
                        if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
                            // 如果状态是棋子
                            if (mGameData[k][l].mState == ncFLAG)
                            {
                                // 如果原先状态不是雷
                                if (mGameData[k][l].mStateBackUp != ncMINE)
                                {
                                    isGameOverState = ncLOSE;
                                    isGameBegin = false;
                                    unCover();
                                }
                            }
                            // 如果状态不是棋子
                            else
                            {
                                if (mGameData[k][l].isPress == false)
                                {
                                    mGameData[k][l].isPress = true;
                                    // 如果为雷
                                    if (mGameData[k][l].mState == ncMINE)
                                    {
                                        isGameOverState = ncLOSE;
                                        isGameBegin = false;
                                        mGameData[k][l].mState = ncBOMBING;
                                        unCover();
                                    }
                                    // 如果为空块儿
                                    if (mGameData[k][l].mState == ncNULL)
                                        NullClick(k, l);
                                }
                            }
}
void Game::RL_ClkJudge()
{
    int i, j, k, l, mineNum = 0, flagNum = 0;
    i = (RL_Point.x - mCornPoint.x) / gridSize;
    j = (RL_Point.y - mCornPoint.y) / gridSize;

    if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
    {
        if (mGameData[j][i].isPress == true) // 已经被点击
        {
            if (mGameData[j][i].mState != ncFLAG && mGameData[j][i].mState != ncQ) // 当前块不是棋子也不是问号
                for (k = j - 1; k < j + 2; k++)
                    for (l = i - 1; l < i + 2; l++)
                        if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
                        {
                            if (mGameData[k][l].mState == ncFLAG)
                                flagNum++;
                            if (mGameData[k][l].mState == ncX)
                            {
                                mGameData[k][l].isPress = false;
                                mGameData[k][l].mState = mGameData[k][l].mStateBackUp;
                            }
                        }
        }
        if (mGameData[j][i].mState == flagNum + 2)
            LButtonDblClk(RL_Point);
    }
    RL_ClkJudge_flag = false;
}
void Game::RL_ButtonDown(Vector2i mPoint)
{
    int i, j, k, l;
    i = (mPoint.x - mCornPoint.x) / gridSize;
    j = (mPoint.y - mCornPoint.y) / gridSize;

    if (i >= 0 && i < stageWidth && j >= 0 && j < stageHeight)
        if (mGameData[j][i].isPress == true)
        {
            if (mGameData[j][i].mState != ncFLAG && mGameData[j][i].mState != ncQ)
                for (k = j - 1; k < j + 2; k++)
                    for (l = i - 1; l < i + 2; l++)
                        if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
                            if (mGameData[k][l].isPress == false)
                            {
                                mGameData[k][l].isPress = true;
                                mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
                                mGameData[k][l].mState = ncX;
                            }
            // else
            // {
            //     for (k = j - 1; k < j + 2; k++)
            //         for (l = i - 1; l < i + 2; l++)
            //             if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth)
            //                 if (mGameData[k][l].isPress == false)
            //                 {
            //                     mGameData[k][l].isPress = true;
            //                     mGameData[k][l].mStateBackUp = mGameData[k][l].mState;
            //                     mGameData[k][l].mState = ncX;
            //                 }
            //     mGameData[j][i].isPress = false;
            // }
        }
    RL_Point = mPoint;
    RL_ClkJudge_flag = true;
}
void Game::NullClick(int j, int i)
{
    int k, l;
    for (k = j - 1; k < j + 2; k++)                                    // 遍历所选格子的左边一行到右边一行
        for (l = i - 1; l < i + 2; l++)                                // 遍历所选格子的上面一列到下面一列
            if (k >= 0 && k < stageHeight && l >= 0 && l < stageWidth) // 判断遍历到的格子是否在游戏界面内
                if (mGameData[k][l].isPress == false)                  // 判断遍历到的格子是否被打开
                {
                    if (mGameData[k][l].mState != ncMINE) // 判断该格子是否为雷
                        mGameData[k][l].isPress = true;   // 将 isPress 设置为 true
                    if (mGameData[k][l].mState == ncNULL) // 判断该格子是否为空地
                        NullClick(k, l);                  // 递归调用，继续查找
                }
}
void Game::Logic()
{
    if (mouse_RL_ClkReady == 0 && RL_ClkJudge_flag == true)
        RL_ClkJudge();

    switch (mouseAction)
    {
    case LButtonDownFunc:
        isButtonPress = true;
        if (isGameOverState == ncNo)
        {
            if (mouseClickTimer.getElapsedTime().asMilliseconds() > 300)
            {
                mouseClickTimer.restart();       // 重新计时
                P2 = Mouse::getPosition(window); // 获取鼠标位置

                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    RL_ButtonDown(P2);
                else
                {
                    LButtonDown(P2);
                    mouseDlbClkReady = true;
                }
            }
            mouse_RL_ClkReady++;
        }
        break;
    case LButtonReleasedFunc:
        isButtonPress = false;
        P1 = Mouse::getPosition(window);

        if (isGameOverState == ncNo)
        {
            if (mouseDlbClkReady)
                mouseDlbClkReady = false;
            else
            {
                if (mouseClickTimer.getElapsedTime().asMilliseconds() < 300 &&
                    P2.x - P1.x < gridSize / 4 && P2.y - P1.y < gridSize / 4)
                    LButtonDblClk(P2); //当两次点击的间隔小于 300ms，则判定为鼠标双击
            }

            mouse_RL_ClkReady = 0; //状态清除
            if (isGameOverState == ncNo)
            {
                mouseClickTimer.restart();
                mouseAction = LButtonDownFunc;
                mousePoint = Mouse::getPosition(window);

                //设置游戏难度系数
                if (isGameBegin == false)
                {
                    if (ButtonRectEasy.contains(P1))
                        gamelvl = 1;
                    if (ButtonRectNormal.contains(P1))
                        gamelvl = 2;
                    if (ButtonRectHard.contains(P1))
                        gamelvl = 3;
                    Initial();
                }
            }
            //更换背景
            if (ButtonRectBG.contains(P1))
            {
                imgBGNo++;
                if (imgBGNo > 7) // 大于背景图的总数的时候
                    imgBGNo = 1; // 重新轮换背景图
                LoadMediaData();
            }
            //更换皮肤
            if (ButtonRectSkin.contains(P1))
            {
                imgSkinNo++;
                if (imgSkinNo > 6)
                    imgSkinNo = 1;
                LoadMediaData();
            }
        }
        // 重新开始
        if (ButtonRectRestart.contains(P1))
            Initial();
        // 退出游戏
        if (ButtonRectQuit.contains(P1))
        {
            window.close();
            gameQuit = true;
        }
        break;
    case RButtonDownFunc:
        if (isGameOverState == ncNo)
        {
            mouse_RL_ClkReady++;

            if (mouse_RL_ClkReady == 2)
                RL_ButtonDown(Mouse::getPosition(window));
            else
                RButtonDown(Mouse::getPosition(window));
        }
        break;
    case RButtonReleasedFunc:
        mouse_RL_ClkReady = 0; // 状态清除
        break;
    }
    mouseAction = NULLClick; // 将输入状态置空

    isWin();
}
void Game::isWin()
{
    int i, j, c = 0;
    // 判断插的所有旗是不是都是雷
    // if (mFlagCalc == mMineNum)
    //     for (i = 0; i < stageWidth; i++)
    //         for (j = 0; j < stageHeight; j++)
    //             if (mGameData[j][i].mState == ncFLAG)
    //             {
    //                 if (mGameData[j][i].mStateBackUp == ncMINE)
    //                     c++;
    //             }
    // 判断剩下没插旗的块是不是都是雷
    // else
    //     if (isGameOverState != ncLOSE)
    //         for (i = 0; i < stageWidth; i++)
    //             for (j = 0; j < stageHeight; j++)
    //                 if (mGameData[j][i].isPress == false || mGameData[j][i].mState == ncFlag)
    //                     c++;
    if (isGameOverState != ncLOSE)
        for (i = 0; i < stageWidth; i++)
            for (j = 0; j < stageHeight; j++)
                if (mGameData[j][i].isPress == false || mGameData[j][i].mState == ncFLAG)
                    c++;
    // 如果所有棋子对应都是雷，游戏胜利并结束
    if (c == mMineNum)
    {
        isGameBegin = false;
        mFlagCalc = mMineNum;
        undownOpen();
        isGameOverState = ncWIN;
    }
}

void Game::Draw()
{
    window.clear();

    //绘制背景
    sBackground.setPosition(0, 0);
    window.draw(sBackground);

    //绘制舞台
    DrawGrid();
    DrawButton();
    DrawTimer();
    DrawScore();
    if (isGameOverState)
    {
        gameOver = false;
        DrawGameEnd();
    }
    window.display();
}
void Game::DrawGrid()
{
    // mCornPoint.x = (Window_width - stageWidth * gridSize) / 2;
    // mCornPoint.y = (Window_height - stageHeight * gridSize) / 2;
    for (int j = 0; j < stageHeight; j++)
    {
        for (int i = 0; i < stageWidth; i++)
        {
            if (mGameData[j][i].isPress == true)
            {
                if (mGameData[j][i].mState > 15 | mGameData[j][i].mState < 0)
                    cout << mGameData[j][i].mState << endl;
                sTiles.setTextureRect(IntRect(mGameData[j][i].mState * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
                sTiles.setPosition(mCornPoint.x + i * gridSize, mCornPoint.y + j * gridSize);
            }
            else
            {
                sTiles.setTextureRect(IntRect(ncUNDOWN * GRIDSIZE, 0, GRIDSIZE, GRIDSIZE));
                sTiles.setPosition(mCornPoint.x + i * gridSize, mCornPoint.y + j * gridSize);
            }
            window.draw(sTiles);
        }
    }
}
void Game::DrawButton()
{
    Vector2i LeftCorner;
    int ButtonWidth = 60;
    int ButtonHeight = 36;
    int detaX;
    detaX = (Window_width - ButtonWidth * 7) / 8; //七个按钮在界面上等分
    LeftCorner.y = Window_height - GRIDSIZE * 3;  //指定高度

    sf::Vector2i localPosition = sf::Mouse::getPosition(window);

    // ButtonRectEasy
    LeftCorner.x = detaX;
    ButtonRectEasy.left = LeftCorner.x;
    ButtonRectEasy.top = LeftCorner.y;
    ButtonRectEasy.width = ButtonWidth;
    ButtonRectEasy.height = ButtonHeight;
    if (ButtonRectEasy.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectEasy.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectNormal
    LeftCorner.x = detaX * 2 + ButtonWidth;
    ButtonRectNormal.left = LeftCorner.x;
    ButtonRectNormal.top = LeftCorner.y;
    ButtonRectNormal.width = ButtonWidth;
    ButtonRectNormal.height = ButtonHeight;
    if (ButtonRectNormal.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectNormal.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectEasy
    LeftCorner.x = detaX * 3 + ButtonWidth * 2;
    ButtonRectHard.left = LeftCorner.x;
    ButtonRectHard.top = LeftCorner.y;
    ButtonRectHard.width = ButtonWidth;
    ButtonRectHard.height = ButtonHeight;
    if (ButtonRectHard.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectHard.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(2 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectBG
    LeftCorner.x = detaX * 4 + ButtonWidth * 3;
    ButtonRectBG.left = LeftCorner.x;
    ButtonRectBG.top = LeftCorner.y;
    ButtonRectBG.width = ButtonWidth;
    ButtonRectBG.height = ButtonHeight;
    if (ButtonRectBG.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectBG.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(3 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectSkin
    LeftCorner.x = detaX * 5 + ButtonWidth * 4;
    ButtonRectSkin.left = LeftCorner.x;
    ButtonRectSkin.top = LeftCorner.y;
    ButtonRectSkin.width = ButtonWidth;
    ButtonRectSkin.height = ButtonHeight;
    if (ButtonRectSkin.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectSkin.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(4 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectRestart
    LeftCorner.x = detaX * 6 + ButtonWidth * 5;
    ButtonRectRestart.left = LeftCorner.x;
    ButtonRectRestart.top = LeftCorner.y;
    ButtonRectRestart.width = ButtonWidth;
    ButtonRectRestart.height = ButtonHeight;
    if (ButtonRectRestart.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectRestart.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(5 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
    // ButtonRectQuit
    LeftCorner.x = detaX * 7 + ButtonWidth * 6;
    ButtonRectQuit.left = LeftCorner.x;
    ButtonRectQuit.top = LeftCorner.y;
    ButtonRectQuit.width = ButtonWidth;
    ButtonRectQuit.height = ButtonHeight;
    if (ButtonRectQuit.contains(localPosition.x, localPosition.y) && isButtonPress == false)
    {
        sButtons_h.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_h.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_h);
    }
    else if (ButtonRectQuit.contains(localPosition.x, localPosition.y) && isButtonPress == true)
    {
        sButtons_p.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons_p.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons_p);
    }
    else
    {
        sButtons.setTextureRect(IntRect(6 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
        sButtons.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sButtons);
    }
}
void Game::DrawScore()
{
    Vector2i LeftCorner;
    LeftCorner.x = Window_width - sCounter.getLocalBounds().width * 1.25;
    LeftCorner.y = sCounter.getLocalBounds().height * 0.5;
    sCounter.setPosition(LeftCorner.x, LeftCorner.y); // 计数器纹理的贴图位置
    window.draw(sCounter);

    int NumSize = sNum.getLocalBounds().height;
    LeftCorner.x = LeftCorner.x + sCounter.getLocalBounds().width - NumSize;
    LeftCorner.y = LeftCorner.y + sCounter.getLocalBounds().height * 0.5 - NumSize * 0.5;

    int mScore = mMineNum - mFlagCalc;
    if (mScore >= 0)
    {
        //绘制个位数的数字
        int a = mScore % 10;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
        //绘制十位数的数字
        mScore = mScore / 10;
        a = mScore % 10;
        LeftCorner.x = LeftCorner.x - NumSize;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
        //绘制百位数的数字
        mScore = mScore / 10;
        a = mScore % 10;
        LeftCorner.x = LeftCorner.x - NumSize;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
    }
    else
    {
        //绘制个位数的数字
        int a = 0;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
        //绘制十位数的数字
        a = 0;
        LeftCorner.x = LeftCorner.x - NumSize;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
        //绘制百位数的数字
        a = 0;
        LeftCorner.x = LeftCorner.x - NumSize;
        sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize)); //在贴图上取对应数字字符的纹理贴图
        sNum.setPosition(LeftCorner.x, LeftCorner.y);
        window.draw(sNum);
    }
}
void Game::DrawTimer()
{
    Vector2i LeftCorner;
    LeftCorner.x = sTimer.getLocalBounds().width * 0.25;
    LeftCorner.y = sTimer.getLocalBounds().height * 0.5;
    sTimer.setPosition(LeftCorner.x, LeftCorner.y); // 计数器纹理的贴图位置
    window.draw(sTimer);

    if (isGameBegin)
        mTime = gameClock.getElapsedTime().asSeconds();

    int NumSize = sNum.getLocalBounds().height;
    LeftCorner.x = LeftCorner.x + sTimer.getLocalBounds().width - NumSize * 1.5;
    LeftCorner.y = LeftCorner.y + sTimer.getLocalBounds().height * 0.5 - NumSize * 0.5;
    int mScore = mTime;
    if (mScore > 999)
        mScore = 999;

    //绘制个位数的数字
    int a = mScore % 10;
    sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
    //在贴图上取对应数字字符的纹理贴图
    sNum.setPosition(LeftCorner.x, LeftCorner.y);
    window.draw(sNum);

    //绘制十位数的数字
    mScore = mScore / 10;
    a = mScore % 10;
    LeftCorner.x = LeftCorner.x - NumSize;
    sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
    //在贴图上取对应数字字符的纹理贴图
    sNum.setPosition(LeftCorner.x, LeftCorner.y);
    window.draw(sNum);

    //绘制百位数的数字

    mScore = mScore / 10;
    a = mScore % 10;
    LeftCorner.x = LeftCorner.x - NumSize;
    sNum.setTextureRect(IntRect(a * NumSize, 0, NumSize, NumSize));
    //在贴图上取对应数字字符的纹理贴图
    sNum.setPosition(LeftCorner.x, LeftCorner.y);
    window.draw(sNum);
}
void Game::DrawGameEnd()
{
    Vector2i LeftCorner;
    int ButtonWidth = 200;
    int ButtonHeight = sGameOver.getLocalBounds().height;
    LeftCorner.x = (Window_width - ButtonWidth) / 2;
    LeftCorner.y = (Window_height - ButtonHeight) / 2;

    sGameOver.setPosition(LeftCorner.x, LeftCorner.y);

    if (isGameOverState == ncWIN)
    {
        sGameOver.setTextureRect(IntRect(0 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
    }
    if (isGameOverState == ncLOSE)
    {
        sGameOver.setTextureRect(IntRect(1 * ButtonWidth, 0, ButtonWidth, ButtonHeight));
    }
    window.draw(sGameOver);
}

void Game::Run()
{
    do
    {
        Initial();
        Input();
        while (window.isOpen())
        {
            Input();

            Logic();

            Draw();
        }
    } while (window.isOpen() && !gameQuit);
}