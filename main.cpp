#include <raylib.h>
#include <deque>
#include <raymath.h>
#include <chrono>
#include <thread>

using namespace std;

//////////////////////////////////////////////////////kolory
Color color = {25,25,52,255};
Color foodColor = {100,100,100,255};
Color snakeColor = {200,200,200,255};

/////////////////////////////////////////////////////wymiary komórek planszy
int sizeCell = 30;
int count = 25;
//size*count = wymiary planszy

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

//////////////////////////////////////////////////////////klasy
class Food{
public:
    int positionX = GetRandomValue(0,count-1);
    int positionY = GetRandomValue(0,count-1);

    public:
        void Draw(){
            DrawRectangle(positionX * sizeCell, positionY * sizeCell, sizeCell, sizeCell, foodColor);
        }

};

class Snake{

    public:
        deque<Vector2> body = {{2,3}, {3,3}, {4,3}};

        
        Vector2 directionXR = {1,0};
        Vector2 directionXL = {-1,0};
        Vector2 directionYU = {0,-1};
        Vector2 directionYD = {0,1};
        Vector2 direction = directionXR;

        void Draw(){
            for(int i = 0; i < body.size(); i++){
                int x = body[i].x;
                int y = body[i].y;

                DrawRectangle(x * sizeCell, y * sizeCell, sizeCell, sizeCell, snakeColor);
            }
        }

        void Update(){

            //right
            body.pop_front();
            body.push_back(Vector2Add(body[body.size()-1], direction));

        }
};

class Game{
public:
    Snake snake = Snake();
    Food food = Food();
    int counter = 0;

    void Draw(){
        snake.Draw();
        food.Draw();
    }

    void Update(){
        snake.Update();
    }

    void foodCollision(){
        Vector2 foodPos={food.positionX, food.positionY};
        if(Vector2Equals(foodPos, snake.body[snake.body.size()-1])){
            food.positionX = GetRandomValue(0,count-1);
            food.positionY = GetRandomValue(0,count-1);
            snake.body.push_front(snake.body[0]);
            counter++;
        };
    }

    bool boardCollision(){
        Vector2 left = {-1, snake.body[snake.body.size()-1].y};
        Vector2 right = {25, snake.body[snake.body.size()-1].y};
        Vector2 up = {snake.body[snake.body.size()-1].x, -1};
        Vector2 down = {snake.body[snake.body.size()-1].x, 25};

        if(Vector2Equals(left, snake.body[snake.body.size()-1]) || Vector2Equals(right, snake.body[snake.body.size()-1]) || Vector2Equals(up, snake.body[snake.body.size()-1]) || Vector2Equals(down, snake.body[snake.body.size()-1])){
            snake.body = {{2,3}, {3,3}, {4,3}};
            snake.direction = snake.directionXR;
            snake.Update();
            counter = 0;
        };

        return true;

    }
};



////////////////////////////////////////////////////////funkcja main
int main()
{

    const int screenWidth = 750;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(60);

    Game game;

    bool left = false;
    bool right = true;
    bool up = false;
    bool down = false;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(color);

        game.Draw();

        if(event(0.2)){
            game.Update();
        }

        game.boardCollision();    
        
        if(IsKeyPressed(KEY_UP) && (right == true || left == true) && down == false){
            game.snake.direction = game.snake.directionYU;
            left = false;
            right = false;
            down = false;
            up = true;
        }else if(IsKeyPressed(KEY_DOWN) && (right == true || left == true) && up == false){
            game.snake.direction = game.snake.directionYD;
            left = false;
            right = false;
            down = true;
            up = false;
        }else if(IsKeyPressed(KEY_RIGHT) && (up == true || down == true) && left == false){
            game.snake.direction = game.snake.directionXR;
            left = false;
            right = true;
            down = false;
            up = false;
        }else if(IsKeyPressed(KEY_LEFT) && (up == true || down == true) && right == false){
            game.snake.direction = game.snake.directionXL;
            left = true;
            right = false;
            down = false;
            up = false;
        }

        game.foodCollision();

        EndDrawing();
        
    }

    CloseWindow();
    return 0;
}