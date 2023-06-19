#include <SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include <string>
using namespace sf;
int minesaround(bool bgrid[12][12], int grid[12][12], int i, int j) {
    int n = 0;
    if (grid[i][j] != 9) {
        for (int a = -1; a < 2; a++) {
            for (int b = -1; b < 2; b++) {
                if ((0 == a && 0 == b) || (0 > a + i || 0 > b + j || 12 == a + i || 12 == b + j))
                {
                    continue;
                }
                if (grid[i + a][j + b] == 9) {
                    n++;
                }
            }
        }
    }
    return n;
}
void Visit(int& game, int grid[12][12], int i, int j, bool bgrid[12][12], int sgrid[12][12],Sound& s1,SoundBuffer& b1) {
    if (i >= 0 && j >= 0 && i < 12 && j < 12) {
        if (minesaround(bgrid, grid, i, j) == 0 && grid[i][j] != 9 && grid[i][j] != 11 && bgrid[i][j] == false) {
            sgrid[i][j] = grid[i][j];
            bgrid[i][j] = true;
            Visit(game, grid, i - 1, j, bgrid, sgrid,s1,b1);
            Visit(game, grid, i + 1, j, bgrid, sgrid,s1,b1);
            Visit(game, grid, i, j + 1, bgrid, sgrid,s1,b1);
            Visit(game, grid, i, j - 1, bgrid, sgrid,s1,b1);
            Visit(game, grid, i - 1, j + 1, bgrid, sgrid,s1,b1);
            Visit(game, grid, i + 1, j + 1, bgrid, sgrid,s1,b1);
            Visit(game, grid, i - 1, j - 1, bgrid, sgrid,s1,b1);
            Visit(game, grid, i + 1, j - 1, bgrid, sgrid,s1,b1);
        }
        else if (minesaround(bgrid, grid, i, j) > 0) {
            sgrid[i][j] = grid[i][j];
            bgrid[i][j] = true;
            return;
        }
        else if (grid[i][j] == 9) {
            if (!b1.loadFromFile("Exp.wav"))
            {
            }
            s1.setBuffer(b1);
            s1.play();
            sgrid[i][j] = grid[i][j];
            bgrid[i][j] = true;           
            game = -1;
        }
        else {
            return;
        }
    }
    else {
        return;
    }
}
int gameover(int& game, int minesnumber, bool bgrid[12][12],SoundBuffer& b2,Sound& s2,int &count) {
    int counts = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            if (bgrid[i][j] == false) {
                counts++;
            }
        }
    if (counts == minesnumber) {
        game = 1;
        String st1 = "Win";
        String st2 = ".wav";
        String st3 = std::to_string(count);
        String st4 = st1 + st3 + st2;
        if (!b2.loadFromFile(st4))
        {
        }
        s2.setBuffer(b2);
        s2.play();
        if (count < 4) {
        count++;
    }
        else {
            count = 1;
        }
    }
    return game;
}
void reset(int count, int nom, bool bgrid[12][12], int grid[12][12], int sgrid[12][12], RenderWindow& i_window, int& game) {
    i_window.clear(Color::White);
    while (count != nom) {
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                sgrid[i][j] = 10;
                if (rand() % 6 == 0 && count < nom) {
                    grid[i][j] = 9;
                    count++;
                }
                else grid[i][j] = 0;
            }
    }
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            bgrid[i][j] = false;
            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;
        }
    game = 0;
}
void draw_text(bool i_black, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
    short character_x = i_x;
    short character_y = i_y;
    unsigned char character_width;
    sf::Sprite character_sprite;
    sf::Texture font_texture;
    font_texture.loadFromFile("images/Font.png");
    character_width = font_texture.getSize().x / 96;
    character_sprite.setTexture(font_texture);
    if (1 == i_black)
    {
        character_sprite.setColor(sf::Color(0, 0, 0));
    }
    for (const char a : i_text)
    {
        if ('\n' == a)
        {
            character_x = i_x;
            character_y += font_texture.getSize().y;
            continue;
        }
        character_sprite.setPosition(character_x, character_y);
        character_sprite.setTextureRect(sf::IntRect(character_width * (a - 32), 0, character_width, font_texture.getSize().y));
        character_x += character_width;
        i_window.draw(character_sprite);
    }
}
int noofflags(int nom, int sgrid[12][12]) {
    int count = 0;
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++) {
            if (sgrid[i][j] == 11 && count < nom) {
                count++;
            }
        }
    return count;
}
int main()
{
    int soundcount=1;
    int nom = 10;
    int game = 0;
    int w = 32;
    int grid[12][12];
    int sgrid[12][12];
    bool bgrid[12][12];
    Texture t;
    t.loadFromFile("images/tiles.jpg");
    Sprite s(t);
    int count = 0;
    srand(time(0));
    int alexanderstate = 0;
    Sprite Alexander;
    Texture Alexander_texture;
    Alexander_texture.loadFromFile("images/Alexander.png");
    RenderWindow app(VideoMode(400, 400), "Minesweeper");
    Alexander.setPosition(static_cast<float>(w * 11 - Alexander_texture.getSize().y), w * 11);
    Alexander.setTexture(Alexander_texture);
    SoundBuffer b1,b2,b3,b4;
    Sound s1,s2,s3,s4;
    while (count != nom) {
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                sgrid[i][j] = 10;
                if (rand() % 6 == 0 && count < nom && grid[i][j] != 9) {
                    grid[i][j] = 9;
                    count++;
                }
                else grid[i][j] = 0;
            }
    }
    for (int i = 1; i <= 10; i++)
        for (int j = 1; j <= 10; j++)
        {
            bgrid[i][j] = false;
            int n = 0;
            if (grid[i][j] == 9) continue;
            if (grid[i + 1][j] == 9) n++;
            if (grid[i][j + 1] == 9) n++;
            if (grid[i - 1][j] == 9) n++;
            if (grid[i][j - 1] == 9) n++;
            if (grid[i + 1][j + 1] == 9) n++;
            if (grid[i - 1][j - 1] == 9) n++;
            if (grid[i - 1][j + 1] == 9) n++;
            if (grid[i + 1][j - 1] == 9) n++;
            grid[i][j] = n;
        }
    while (app.isOpen())
    {
        alexanderstate = 0;
        Vector2i pos = Mouse::getPosition(app);
        int x = pos.x / w;
        int y = pos.y / w;
        Event e;
        if(x<11&&y<11){
        while (app.pollEvent(e))
        {
            switch (e.type) {
            case Event::KeyPressed:
                switch (e.key.code)
                {
                case Keyboard::Enter:
                    reset(0, nom, bgrid, grid, sgrid, app, game);
                    alexanderstate = 0;
                }
                break;
            }
            if (e.type == Event::Closed)
                app.close();
            if (e.type == Event::MouseButtonPressed)
                if (e.key.code == Mouse::Left && bgrid[x][y] == false) {
                    alexanderstate = 1;
                    if (!b3.loadFromFile("Open.wav"))
                    {
                    }
                    s3.setBuffer(b3);
                    s3.play();
                    Visit(game, grid, x, y, bgrid, sgrid,s1,b1);
                }
                else if (e.key.code == Mouse::Right && bgrid[x][y] != true) {
                    if (sgrid[x][y] == 11) {
                        alexanderstate = 1;
                        
                        sgrid[x][y] = 10;
                    }
                    else {
                        alexanderstate = 1;
                        if (!b4.loadFromFile("Flag.wav"))
                        {
                        }
                        s4.setBuffer(b4);
                        s4.play();
                        sgrid[x][y] = 11;
                    }
                }
        }
        app.clear(Color::White);
        for (int i = 1; i <= 10; i++)
            for (int j = 1; j <= 10; j++)
            {
                if (sgrid[x][y] == 9) {
                    sgrid[i][j] = grid[i][j];
                    bgrid[i][j] = true;   
                }
                s.setTextureRect(IntRect(sgrid[i][j] * w, 0, w, w));
                s.setPosition(i * w, j * w);
                app.draw(s);
            }
    }
        gameover(game, nom, bgrid,b2,s2,soundcount);
        if (game > 0) {      
            alexanderstate = 3;                    
            draw_text(1, static_cast<unsigned short>(round(0.5f * (8 * 12 - 8 * 5))), static_cast<unsigned short>(round(0.5f * (8 * 12 - 85))), "VICTORY!", app);
            for (int i = 1; i <= 10; i++)
                for (int j = 1; j <= 10; j++) {
                    bgrid[i][j] = true;
                    if (sgrid[i][j] == 10) {
                        sgrid[i][j] = 11;
                    }
                }
        }
        else if (game < 0) {
            alexanderstate = 2;
            draw_text(1, static_cast<unsigned short>(round(0.5f * (8 * 12 - 8 * 5))), static_cast<unsigned short>(round(0.5f * (8 * 12 - 85))), "Game Over!", app);
            for (int i = 1; i <= 10; i++)
                for (int j = 1; j <= 10; j++) {
                    bgrid[i][j] = true;
                }
        }
        Alexander.setTextureRect(sf::IntRect(alexanderstate * Alexander_texture.getSize().y, 0, Alexander_texture.getSize().y, Alexander_texture.getSize().y));
        draw_text(1, 5, 8 * 45, "Mines:" + std::to_string(nom - noofflags(nom, sgrid)), app); 
        app.draw(Alexander);
        app.display();
        }
    return 0;
}