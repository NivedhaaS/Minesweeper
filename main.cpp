#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <cctype>
#include "Board.h"
using namespace std;
using namespace sf;

int main() {
    string num;
    int row;
    int col;
    int mine;

    ifstream file("files/config.cfg");

    getline(file, num);
    col = stoi(num);
    getline(file, num);
    row = stoi(num);
    getline(file, num);
    mine = stoi(num);
    file.close();

    int width = (col * 32) + 100;
    int height = row * 32;

    sf::RenderWindow window(sf::VideoMode(width, height), "Minesweeper");
    sf::Font font;

    if(!font.loadFromFile("files/font.ttf")){
        cout << endl;
    }

    sf::Text first;
    first.setString("WELCOME TO MINESWEEPER!");
    first.setFont(font);
    first.setCharacterSize(24);
    first.setFillColor(sf::Color::White);
    first.setStyle(sf::Text::Bold | sf::Text::Underlined);

    setText(first, width/2, (height/2) - 150);

    sf::Text second;
    second.setString("Enter your name: ");
    second.setFont(font);
    second.setCharacterSize(20);
    second.setFillColor(sf::Color::White);
    second.setStyle(sf::Text::Bold);
    setText(second, width/2, (height/2) - 75);

    sf::Text third;
    third.setFont(font);
    third.setCharacterSize(18);
    third.setFillColor(sf::Color::Yellow);
    third.setStyle(sf::Text::Bold);

    string username;
    string showDisplay;

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(username.length() == 0){
                showDisplay = "|";
            }
            if(event.type == sf::Event::Closed){
                window.close();
            }
            else if(event.type == sf::Event::TextEntered){
                if(event.text.unicode == '\b' && !username.empty()){
                    username.pop_back();
                }
                if(isalpha(event.text.unicode) && username.length() < 10){
                    username += static_cast<char>(event.text.unicode);
                }
                if(username.length() > 0 && username.length() <= 10){
                    string final = username;
                    int character = 0;
                    for(char&i : final){
                        if(character == 0){
                            i = toupper(i);
                        }
                        if(character > 0){
                            i = tolower(i);
                        }
                        character++;
                    }
                    showDisplay = final;
                    showDisplay += "|";
                }
            }
            if(event.type == sf::Event::KeyPressed){
                if(event.key.code == sf::Keyboard::Enter && username.length() > 0){
                    window.close();

                    Board board(row, col, mine);
                    board.setWidth(col * 32);
                    board.setHeight(row * 32 + 100);
                    board.username = username;
                    board.open();
                }
            }
            third.setString(showDisplay);
            setText(third, width/2, (height/2) - 45);
        }
        window.clear(sf::Color::Blue);
        window.draw(first);
        window.draw(second);
        window.draw(third);
        window.display();
    }
    return 0;
}