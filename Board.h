#include "Tile.h"
#include <vector>
#include <string>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>
using namespace std;
using namespace sf;


void setText(sf::Text &text, float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

class Board {
private:
    sf::Font font;
    sf::Text first;
    int width;
    int height;
    int reveal_count;
    int rows;
    int columns;
    int flags;
    int mineCount;
    int gameTime;
    int changed_pos = 0;
    vector<vector<Tile *>> boardPtrs;
    bool debugMode;
    bool isPaused;
    bool leaderBoard;
    bool loser;
    bool winner;
    vector<int> leaderboard_times;
    vector<string> leaderboard_names;

    Textures textures_init;
    Tile console;
    Tile digit;

public:
    string username;


    void setWidth(int width) {
        this->width = width;
    }

    void setHeight(int height) {
        this->height = height;
    }

    void open() {
        sf::RenderWindow game(sf::VideoMode(width, height), "Minesweeper");
        auto time1 = chrono::high_resolution_clock::now();
        while (game.isOpen()) {
            if (leaderBoard){
                setText(first, columns * 16/2, (rows * 16 + 50)/2 - 120);

                string displaystring = "";

                for (int k = 1; k < 6; k++){
                    displaystring += to_string(k) + ".\t";
                    if ((leaderboard_times[k-1]/60) < 10) displaystring += "0";
                    displaystring += to_string(leaderboard_times[k-1]/60) + ":";
                    if ((leaderboard_times[k-1]%60) < 10) displaystring += "0";
                    displaystring += to_string(leaderboard_times[k-1]%60) + "\t";
                    displaystring += leaderboard_names[k-1];

                    if (changed_pos == k){
                        displaystring += "*";
                    }
                    displaystring += "\n\n";
                }

                Text second;
                second.setFont(font);
                second.setString(displaystring);
                second.setCharacterSize(18);
                second.setFillColor(sf::Color::White);
                second.setStyle(sf::Text::Bold);
                setText(second, columns * 16/2, (rows * 16 + 50)/2 + 20);

                sf::RenderWindow leaderboard(sf::VideoMode(columns * 16, rows * 16 + 50), "Minesweeper");
                while (leaderboard.isOpen()){
                    Event event1;
                    while(leaderboard.pollEvent(event1)){
                        if (event1.type == sf::Event::Closed){
                            leaderboard.close();
                            leaderBoard = false;
                        }
                    }
                    leaderboard.clear(sf::Color::Blue);
                    leaderboard.draw(first);
                    leaderboard.draw(second);
                    leaderboard.display();
                }
            }

            sf::Event event;
            while (game.pollEvent(event)) {
                if (event.type == sf::Event::MouseButtonPressed){
                    int col_clicked = event.mouseButton.x / 32;
                    int row_clicked = event.mouseButton.y / 32;
                    if(col_clicked < columns && row_clicked < rows && event.mouseButton.button == sf::Mouse::Left && not loser && not isPaused && not winner && not boardPtrs[row_clicked][col_clicked]->isFlagged){
                        if (boardPtrs[row_clicked][col_clicked]->isMine){
                            loser = true;
                        }
                        boardPtrs[row_clicked][col_clicked]->reveal(reveal_count);
                        if (reveal_count == rows * columns - mineCount){
                            winner = true;
                            leaderBoard = true;
                            for (int i = 0; i < 5; i++){
                                if (gameTime < leaderboard_times[i]) {
                                    leaderboard_times.insert(leaderboard_times.begin() + i, gameTime);
                                    leaderboard_times.erase(leaderboard_times.end() - 1);
                                    leaderboard_names.insert(leaderboard_names.begin() + i, username);
                                    leaderboard_names.erase(leaderboard_names.end() - 1);
                                    changed_pos = i + 1;
                                    break;
                                }
                            }

                            ofstream writeleaderboard("files/leaderboard.txt");
                            for (int i = 0; i < 5; i++){
                                string write_string = "";
                                if (leaderboard_times[i]/60 < 10) write_string += "0";
                                write_string += to_string(leaderboard_times[i]/60) + ":";
                                if (leaderboard_times[i]%60 < 10) write_string += "0";
                                write_string += to_string(leaderboard_times[i]%60) + ", " + leaderboard_names[i] + "\n";
                                writeleaderboard << write_string;
                            }
                        }
                    }
                    if(col_clicked < columns && row_clicked < rows && event.mouseButton.button == sf::Mouse::Right && not boardPtrs[row_clicked][col_clicked]->isRevealed && not loser && not isPaused && not winner){
                        boardPtrs[row_clicked][col_clicked]->isFlagged = not boardPtrs[row_clicked][col_clicked]->isFlagged;
                        if (boardPtrs[row_clicked][col_clicked]->isFlagged) flags -= 1;
                        else flags += 1;
                    }

                    //restart button
                    if (event.mouseButton.x > (columns/2.0 * 32) - 32 && event.mouseButton.x < (columns/2.0 * 32) + 32
                    && event.mouseButton.y > 32 * (rows + 0.5) && event.mouseButton.y < 32 * (rows + 0.5) + 64){
                        for (int i = 0; i < boardPtrs.size(); i++){
                            for (int j = 0; j < boardPtrs[i].size(); j++){
                                delete boardPtrs[i][j];
                            }
                        }

                        for (auto a : boardPtrs){
                            a.clear();
                        }
                        boardPtrs.clear();
                        this->reveal_count = 0;
                        this->flags = mineCount;
                        this->gameTime = 0;
                        this->debugMode = false;
                        this->isPaused = false;
                        this->leaderBoard = false;
                        this->loser = false;
                        this->winner = false;

                        for (unsigned i = 0; i < rows; i++) {
                            vector<Tile *> currRow;
                            for (unsigned j = 0; j < columns; j++) {
                                Tile *temp = new Tile;
                                temp->setHidden(*textures_init.hidden);
                                currRow.push_back(temp);
                            }
                            boardPtrs.push_back(currRow);
                        }

                        for (int i = 0; i < mineCount; i++){
                            int rowMine = rand() % (rows-1);

                            int colMine = rand() % (columns-1);

                            while (boardPtrs[rowMine][colMine]->isMine){
                                rowMine = rand() % (rows-1);
                                colMine = rand() % (columns-1);
                            }
                            boardPtrs[rowMine][colMine]->isMine = true;
                        }

                        for(int i = 0; i < rows; i++){
                            for(int j = 0; j < columns; j++){

                                if(i - 1 >= 0){
                                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j]);
                                    if(j - 1 >= 0){
                                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j-1]);
                                    }
                                    if(j + 1 < columns){
                                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j+1]);
                                    }
                                }
                                if(i + 1 < rows){
                                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j]);
                                    if(j - 1 >= 0){
                                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j-1]);
                                    }
                                    if(j + 1 < columns){
                                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j+1]);
                                    }
                                }
                                if(j - 1 >= 0){
                                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i][j-1]);
                                }
                                if(j + 1 < columns){
                                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i][j+1]);
                                }

                                for(auto a : boardPtrs[i][j]->surroundingMinesPtrs){
                                    if (a->isMine){
                                        boardPtrs[i][j]->surroundingMines += 1;
                                    }
                                }

                            }
                        }
                    }

                    //debug button
                    if (event.mouseButton.x > (columns * 32) - 304 && event.mouseButton.x < (columns * 32) - 304 + 64
                    && event.mouseButton.y > 32 * (rows + 0.5) && event.mouseButton.y < 32 * (rows + 0.5) + 64){
                        debugMode = not debugMode;
                    }

                    //pause/play button
                    if (event.mouseButton.x > (columns * 32) - 240 && event.mouseButton.x < (columns * 32) - 240 + 64
                    && event.mouseButton.y > 32 * (rows + 0.5) && event.mouseButton.y < 32 * (rows + 0.5) + 64){
                        isPaused = not isPaused;
                    }

                    //leaderboard
                    if (event.mouseButton.x > (columns * 32) - 176 && event.mouseButton.x < (columns * 32) - 176 + 64
                    && event.mouseButton.y > 32 * (rows + 0.5) && event.mouseButton.y < 32 * (rows + 0.5) + 64){
                        leaderBoard = true;
                    }
                }
                if (event.type == sf::Event::Closed) {
                    game.close();
                }

            }
            game.clear(sf::Color::White);
            for (int i = 0; i < rows; i++){
                for (int j = 0; j < columns; j++){
                    boardPtrs[i][j]->setPosition(j * 32, i * 32);

                    if (isPaused || leaderBoard){
                        boardPtrs[i][j]->setRevealed(*textures_init.revealed);
                        boardPtrs[i][j]->drawWindow(game);
                    }
                    else if (boardPtrs[i][j]->isRevealed){
                        boardPtrs[i][j]->setRevealed(*textures_init.revealed);
                        boardPtrs[i][j]->drawWindow(game);

                        if (boardPtrs[i][j]->isMine){
                            boardPtrs[i][j]->setMine(*textures_init.mine);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 1){
                            boardPtrs[i][j]->setOne(*textures_init.one);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 2){
                            boardPtrs[i][j]->setTwo(*textures_init.two);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 3){
                            boardPtrs[i][j]->setThree(*textures_init.three);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 4){
                            boardPtrs[i][j]->setFour(*textures_init.four);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 5){
                            boardPtrs[i][j]->setFive(*textures_init.five);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 6){
                            boardPtrs[i][j]->setSix(*textures_init.six);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 7){
                            boardPtrs[i][j]->setSeven(*textures_init.seven);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        else if(boardPtrs[i][j]->surroundingMines == 8){
                            boardPtrs[i][j]->setEight(*textures_init.eight);
                            boardPtrs[i][j]->drawWindow(game);
                        }

                    }
                    else {
                        boardPtrs[i][j]->setHidden(*textures_init.hidden);
                        boardPtrs[i][j]->drawWindow(game);

                        if (boardPtrs[i][j]->isFlagged) {
                            boardPtrs[i][j]->setFlag(*textures_init.flag);
                            boardPtrs[i][j]->drawWindow(game);
                        }
                        if (debugMode || loser){
                            if (boardPtrs[i][j]->isMine){
                                boardPtrs[i][j]->setMine(*textures_init.mine);
                                boardPtrs[i][j]->drawWindow(game);
                            }
                        }
                    }
                }
            }

            if (loser) {
                console.setLoss(*textures_init.lose);
            }
            else if (winner) {
                console.setWin(*textures_init.win);
            }
            else{
                console.setSmile(*textures_init.happy);
            }
            console.setPosition((columns/2.0 * 32) - 32, 32 * (rows + 0.5));
            console.drawWindow(game);

            console.setDebug(*textures_init.debug);
            console.setPosition((columns * 32) - 304, 32 * (rows + 0.5));
            console.drawWindow(game);

            if (isPaused){
                console.setPlay(*textures_init.play);
                console.setPosition((columns * 32) - 240, 32 * (rows + 0.5));
                console.drawWindow(game);
            }
            else{
                console.setPause(*textures_init.pause);
                console.setPosition((columns * 32) - 240, 32 * (rows + 0.5));
                console.drawWindow(game);
            }

            console.setLeaderboard(*textures_init.leaderboard);
            console.setPosition((columns * 32) - 176, 32 * (rows + 0.5));
            console.drawWindow(game);

            auto time2 = chrono::high_resolution_clock::now();
            chrono::duration<double, milli> duration = time2 - time1;
            if (duration.count() > 1000 && not isPaused && not winner && not loser){
                gameTime += 1;
                time1 = time2;
            }

            //timer
            digit.setDigit((gameTime/60)/10, *textures_init.digits);
            digit.setPosition((columns * 32) - 97, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            digit.setDigit((gameTime/60)%10, *textures_init.digits);
            digit.setPosition((columns * 32) - 97 + 21, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            digit.setDigit((gameTime%60)/10, *textures_init.digits);
            digit.setPosition((columns * 32) - 54, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            digit.setDigit((gameTime%60)%10, *textures_init.digits);
            digit.setPosition((columns * 32) - 54 + 21, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            //flag count
            digit.setDigit(abs(flags) / 100, *textures_init.digits);
            digit.setPosition(33, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            digit.setDigit(abs(flags) % 100 / 10, *textures_init.digits);
            digit.setPosition(33 + 21, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            digit.setDigit(abs(flags) - (abs(flags) % 100 / 10) * 10 - (abs(flags) / 100) * 100, *textures_init.digits);
            digit.setPosition(33 + 42, 32 * (rows + 0.5) + 16);
            digit.drawWindow(game);

            if (flags < 0){
                digit.setDigit(10, *textures_init.digits);
                digit.setPosition(12, 32 * (rows + 0.5) + 16);
                digit.drawWindow(game);
            }

            game.display();
        }
    }

    Board(int rows, int columns, int mines) {
        ifstream leaderboardfile("files/leaderboard.txt");
        string leaderboardline;

        for(int i = 0; i < 5; i++) {
            getline(leaderboardfile, leaderboardline);
            leaderboard_times.push_back(stoi(leaderboardline.substr(0, 2)) * 60 + stoi(leaderboardline.substr(3, 2)));
            leaderboard_names.push_back(leaderboardline.substr(7));
        }

        leaderboardfile.close();

        this->rows = rows;
        this->columns = columns;
        this-> mineCount = mines;
        this->flags = mineCount;
        this->gameTime = 0;
        this->reveal_count = 0;
        this->debugMode = false;
        this->isPaused = false;
        this->leaderBoard = false;
        this->loser = false;
        this->winner = false;

        if(not font.loadFromFile("files/font.ttf")){
            cout << endl;
        }

        first.setFont(font);
        first.setString("LEADERBOARD");
        first.setCharacterSize(20);
        first.setFillColor(sf::Color::White);
        first.setStyle(sf::Text::Bold | sf::Text::Underlined);


        for (unsigned i = 0; i < rows; i++) {
            vector<Tile *> currRow;
            for (unsigned j = 0; j < columns; j++) {
                Tile *temp = new Tile;
                currRow.push_back(temp);
            }
            boardPtrs.push_back(currRow);
        }

        for (int i = 0; i < mines; i++){
            int rowMine = rand() % (rows-1);

            int colMine = rand() % (columns-1);

            while (boardPtrs[rowMine][colMine]->isMine){
                rowMine = rand() % (rows-1);
                colMine = rand() % (columns-1);
            }
            boardPtrs[rowMine][colMine]->isMine = true;
        }

        for(int i = 0; i < rows; i++){
            for(int j = 0; j < columns; j++){
                if(i - 1 >= 0){
                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j]);
                    if(j - 1 >= 0){
                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j-1]);
                    }
                    if(j + 1 < columns){
                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i-1][j+1]);
                    }
                }
                if(i + 1 < rows){
                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j]);
                    if(j - 1 >= 0){
                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j-1]);
                    }
                    if(j + 1 < columns){
                        boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i+1][j+1]);
                    }
                }
                if(j - 1 >= 0){
                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i][j-1]);
                }
                if(j + 1 < columns){
                    boardPtrs[i][j]->surroundingMinesPtrs.push_back(boardPtrs[i][j+1]);
                }

                for(auto a : boardPtrs[i][j]->surroundingMinesPtrs){
                    if (a->isMine){
                        boardPtrs[i][j]->surroundingMines += 1;
                    }
                }

            }
        }

    }

    ~Board(){
        for (int i = 0; i < boardPtrs.size(); i++){
            for (int j = 0; j < boardPtrs[i].size(); j++){
                delete boardPtrs[i][j];
            }
        }
    }

};

