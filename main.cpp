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


//////////////////////////////////////////////////////////klasy
class Food{

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
            body.push_back(Vector2Add(body[1], directionXR));

            //slower snake moving
            this_thread::sleep_for(chrono::milliseconds(200));

            
        }
};



////////////////////////////////////////////////////////funkcja main
int main()
{

    const int screenWidth = 750;
    const int screenHeight = 750;

    InitWindow(screenWidth, screenHeight, "Snake");
    SetTargetFPS(75);

    Food food;
    Snake snake;

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(color);
        food.Draw();
        snake.Draw();
        snake.Update();
        EndDrawing();
        
    }

    CloseWindow();
    return 0;
}