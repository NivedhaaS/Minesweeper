#include <iostream>
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

struct Textures{
    Texture* happy = new Texture;
    Texture* win = new Texture;
    Texture* lose = new Texture;
    Texture* hidden = new Texture;
    Texture* revealed = new Texture;
    Texture* mine = new Texture;
    Texture* flag = new Texture;
    Texture* play = new Texture;
    Texture* pause = new Texture;
    Texture* leaderboard = new Texture;
    Texture* digits = new Texture;
    Texture* debug = new Texture;
    Texture* one = new Texture;
    Texture* two = new Texture;
    Texture* three = new Texture;
    Texture* four = new Texture;
    Texture* five = new Texture;
    Texture* six = new Texture;
    Texture* seven = new Texture;
    Texture* eight = new Texture;

    Textures(){
        happy->loadFromFile("files/images/face_happy.png");
        win->loadFromFile("files/images/face_win.png");
        lose->loadFromFile("files/images/face_lose.png");
        hidden->loadFromFile("files/images/tile_hidden.png");
        revealed->loadFromFile("files/images/tile_revealed.png");
        one->loadFromFile("files/images/number_1.png");
        two->loadFromFile("files/images/number_2.png");
        three->loadFromFile("files/images/number_3.png");
        four->loadFromFile("files/images/number_4.png");
        five->loadFromFile("files/images/number_5.png");
        six->loadFromFile("files/images/number_6.png");
        seven->loadFromFile("files/images/number_7.png");
        eight->loadFromFile("files/images/number_8.png");
        play->loadFromFile("files/images/play.png");
        pause->loadFromFile("files/images/pause.png");
        leaderboard->loadFromFile("files/images/leaderboard.png");
        digits->loadFromFile("files/images/digits.png");
        debug->loadFromFile("files/images/debug.png");
        mine->loadFromFile("files/images/mine.png");
        flag->loadFromFile("files/images/flag.png");
    }

    ~Textures(){
        delete happy;
        delete win;
        delete lose;
        delete hidden;
        delete revealed;
        delete mine;
        delete flag;
        delete play;
        delete pause;
        delete leaderboard;
        delete digits;
        delete debug;
        delete one;
        delete two;
        delete three;
        delete four;
        delete five;
        delete six;
        delete seven;
        delete eight;
    }
};

class Tile{

public:
    Sprite sprite;

    bool isMine = false;
    bool isFlagged = false;
    bool isRevealed = false;
    int surroundingMines = 0;
    vector<Tile*> surroundingMinesPtrs;


    void setPosition(float x, float y) {
        sprite.setPosition(x, y);
    }

    void drawWindow(sf::RenderWindow& window) {
        window.draw(sprite);
    }

    void setHidden(Texture& hidden){
        sprite.setTexture(hidden);
    }

    void setSmile(Texture& happy){
        sprite.setTexture(happy);
    }

    void setPlay(Texture& play){
        sprite.setTexture(play);
    }

    void setPause(Texture& pause){
        sprite.setTexture(pause);
    }

    void setDebug(Texture& debug){
        sprite.setTexture(debug);
    }

    void setLeaderboard(Texture& leaderboard){
        sprite.setTexture(leaderboard);
    }

    void setWin(Texture& win) {
        sprite.setTexture(win);
    }

    void setLoss(Texture& lose) {
        sprite.setTexture(lose);
    }

    void setDigit(int number, Texture& digits){
        sprite.setTextureRect(Rect<int>(number * 21, 0, 21, 32));
        sprite.setTexture(digits);
    }

    void setMine(Texture& mine){
        sprite.setTexture(mine);
    }

    void setRevealed(Texture& revealed){
        sprite.setTexture(revealed);
    }

    void setOne(Texture& one){
        sprite.setTexture(one);
    }

    void setTwo(Texture& two){
        sprite.setTexture(two);
    }

    void setThree(Texture& three){
        sprite.setTexture(three);
    }

    void setFour(Texture& four){
        sprite.setTexture(four);
    }

    void setFive(Texture& five){
        sprite.setTexture(five);
    }

    void setSix(Texture& six){
        sprite.setTexture(six);
    }

    void setSeven(Texture& seven){
        sprite.setTexture(seven);
    }

    void setEight(Texture& eight){
        sprite.setTexture(eight);
    }

    void setFlag(Texture& flag){
        sprite.setTexture(flag);
    }

    void reveal(int& i){
        if (not isRevealed && not isFlagged){
            i += 1;
        }
        isRevealed = true;
        if (surroundingMines == 0){
            for (auto a : surroundingMinesPtrs){
                if (not a->isRevealed && not a->isFlagged){
                    a->reveal(i);
                }
            }
        }
    }
};


