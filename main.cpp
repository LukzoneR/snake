#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

//////////////////////////////////////////////////////kolory
Color color = {255,255,255,255};
Color foodColor = {255,255,255,255};
Color snakeColor = {132,94,194,255};
Color background = {255, 150, 113, 255};

/////////////////////////////////////////////////////wymiary planszy
int sizeCell = 30;
int count = 20;
//size*count = wymiary planszy
int border = 75;

//////////////////////////////////////////////////////licznik punktów
int counter = 0;

/////////////////////////////////////////////////////////spowolnienie węża
double lastUpdateTime = 0;

bool event(double interval){
    double currentTime = GetTime();
    if(currentTime - lastUpdateTime >= interval){
        lastUpdateTime = currentTime;
        return true;
    }
    return false;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////klasy
////////////////////////////////////////////////////klasa Food
class Food{
public:
    int positionX = GetRandomValue(0,count-1);
    int positionY = GetRandomValue(0,count-1);

    void Draw(){
        DrawRectangle(border + positionX * sizeCell, border + positionY * sizeCell, sizeCell, sizeCell, foodColor);
    }

};

///////////////////////////////////////////////////////////////////klasa Snake
class Snake{

    public:
        deque<Vector2> body = {{2,3}, {3,3}, {4,3}};


        Vector2 directionXR = {1,0};
        Vector2 directionXL = {-1,0};
        Vector2 directionYU = {0,-1};
        Vector2 directionYD = {0,1};
        Vector2 direction = directionXR;

        bool moved = false;

        void Draw(){
            for(int i = 0; i < body.size(); i++){
                int x = body[i].x;
                int y = body[i].y;

                DrawRectangle(border + x * sizeCell, border + y * sizeCell, sizeCell, sizeCell, snakeColor);
            }
        }

        void Update(){
            //right
            body.pop_front();
            body.push_back(Vector2Add(body[body.size()-1], direction));
            moved = true;
        }

        void Reset(){
            body = {{2,3}, {3,3}, {4,3}};
            direction = directionXR;
            counter = 0;
            Update();
            moved = false;
        }
};


////////////////////////////////////////////////////////klasa Game
class Game{
public:
    Snake snake = Snake();
    Food food = Food();
    Sound foodSound;
    Sound wallSound;


    Game(){
        InitAudioDevice();
        foodSound = LoadSound("sounds/point.mp3");
        wallSound = LoadSound("sounds/hit.mp3");
    }

    ~Game(){
        UnloadSound(foodSound);
        UnloadSound(wallSound);
        CloseAudioDevice();
    }

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        snake.Update();
    }

    void foodCollision(){
        Vector2 foodPos={(float)food.positionX, (float)food.positionY};
        if(Vector2Equals(foodPos, snake.body[snake.body.size()-1])){
            food.positionX = GetRandomValue(0,count-1);
            food.positionY = GetRandomValue(0,count-1);
            snake.body.push_front(snake.body[0]);
            PlaySound(foodSound);
            counter++;
        };
    }

    bool boardCollision(){
        Vector2 left = {-1, snake.body[snake.body.size()-1].y};
        Vector2 right = {(float)count, snake.body[snake.body.size()-1].y};
        Vector2 up = {snake.body[snake.body.size()-1].x, -1};
        Vector2 down = {snake.body[snake.body.size()-1].x, (float)count};

        if(Vector2Equals(left, snake.body[snake.body.size()-1]) || Vector2Equals(right, snake.body[snake.body.size()-1]) || Vector2Equals(up, snake.body[snake.body.size()-1]) || Vector2Equals(down, snake.body[snake.body.size()-1])){
            PlaySound(wallSound);
            snake.Reset();
            return true;
        }else{
            return false;
        }

    }

    bool tailCollision(){
        Vector2 head = snake.body[snake.body.size()-1];

        for(int i = 0; i < snake.body.size() - 1; i++){
            if(Vector2Equals(snake.body[i], head)){
                PlaySound(wallSound);
                snake.Reset();
                return true;
            }

        }

        return false;
    }
};


///////////////////////////////////////////////////////////////////////funkcja zliczająca rekord
int highscore = 0;
int highCounter(int counter){

    if(counter >= highscore){
        highscore = counter;
        return highscore;
    }

    return highscore;
}

////////////////////////////////////////////////////////funkcja main
int main()
{

    const int screenWidth = sizeCell * count;
    const int screenHeight = sizeCell * count;

    InitWindow(2*border + screenWidth, 2*border + screenHeight, "Snake");
    SetTargetFPS(60);

    Game game;

    bool left = false;
    bool right = true;
    bool up = false;
    bool down = false;
    bool gamePaused = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(background);

        game.Draw();

        if (gamePaused) {
            DrawText("Press any key to start..", screenWidth / 2 - border - 30, (screenHeight + border) / 2, 30, BLACK);
            EndDrawing();
            if (GetKeyPressed() != 0) {
                gamePaused = false;
            }
            continue;
        }

        ///////////////////////////////////////////////////////////////////////////ustawienie szybkości poruszania się węża
        if (event(0.15)) {
            game.Update();
        }

        /////////////////////////////////////////////////////////////////////////////////////////reakcja na kolizje
        if (game.boardCollision() || game.tailCollision()) {
            right = true;
            left = false;
            up = false;
            down = false;
            gamePaused = true;
        }

        ///////////////////////////////////////////////////////////////////////////////////////////////poruszanie się węża
        if(game.snake.moved){
            if (IsKeyPressed(KEY_UP) && (right || left) && !down) {
            game.snake.direction = game.snake.directionYU;
            left = false;
            right = false;
            down = false;
            up = true;
            game.snake.moved = false;
        } else if (IsKeyPressed(KEY_DOWN) && (right || left) && !up) {
            game.snake.direction = game.snake.directionYD;
            left = false;
            right = false;
            down = true;
            up = false;
            game.snake.moved = false;
        } else if (IsKeyPressed(KEY_RIGHT) && (up || down) && !left) {
            game.snake.direction = game.snake.directionXR;
            left = false;
            right = true;
            down = false;
            up = false;
            game.snake.moved = false;
        } else if (IsKeyPressed(KEY_LEFT) && (up || down) && !right) {
            game.snake.direction = game.snake.directionXL;
            left = true;
            right = false;
            down = false;
            up = false;
            game.snake.moved = false;
        }
        }
        

        game.foodCollision();

        highCounter(counter);


        //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////rysowanie
        DrawRectangleLinesEx(Rectangle{(float)border-5, (float)border-5, (float)sizeCell*count + 10, (float)sizeCell*count + 10}, 5, color);
        DrawText("Snake", screenWidth/2 + border/2, 20, 30, color);
        DrawText(TextFormat("%i", counter), screenWidth/2 + border - 30, screenHeight + 2*border - border/2, 30, color);
        DrawText(TextFormat("%i", highCounter(counter)), screenWidth/2 + border + 50, screenHeight + 2*border - border/2, 30, color);
        EndDrawing();

    }

    CloseWindow();
    return 0;
}