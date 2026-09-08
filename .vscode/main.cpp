#include<iostream>
#include<raylib.h>
using namespace std;

Color Dark_Green  = Color{20, 27, 45, 255};   //! dark blue
Color Green  = Color{48, 20, 32, 255};    //! deep red
Color Light_Green = Color{255, 255, 255, 30};  //! ring color 
Color Yellow      = Color{245, 190, 44, 255};  //? gold ball

int player_score =0;
int cpu_score =0;


class Ball
{
    public:
        float x,y;
        int raduis;
        int speed_x,speed_y;

        void Draw()
        {
            DrawCircle(x,y,raduis,Yellow);
        }

            void Update()
            {
              x+= speed_x;
              y+= speed_y;

              if (y+raduis >= GetScreenHeight() || y-raduis <=0)
                  speed_y *= -1 ;


                if (x+raduis >= GetScreenWidth())
                 {
                    cpu_score++;
                        ResetBall();
                 }
                if (x-raduis <=0)
                 {
                     player_score++ ;    
                        ResetBall();
                 }
            }

                 void ResetBall()
                 {
                    x = GetScreenWidth()/2;
                    y = GetScreenHeight()/2;
                    int speed_choices[2]={-1,1};
                    speed_x *= speed_choices[GetRandomValue(0,1)];
                    speed_y *= speed_choices[GetRandomValue(0,1)];

                 }
 };

 class Paddle
 {
    protected:
     void LimitMovement()
     {
        if (y <= 0)
         y =0;

        if (y + height >= GetScreenHeight())
          y = GetScreenHeight() - height;
     }

    public:
        float x,y;
        float width,height;
        int speed;

        void Draw()
        {
           DrawRectangleRounded(Rectangle{x, y, width, height}, 0.8, 0, WHITE);
        }    

            void Update()
             {
                if (IsKeyDown(KEY_UP))
                    y -= speed;
                
                 if (IsKeyDown(KEY_DOWN))
                    y+= speed;
                 
                  LimitMovement();
             }
 };

 class CpuPaddle : public Paddle
 {
    public :
     void Update(int ball_y)
      {
        if (y + height/2 > ball_y )
            y-= speed;

          if (y + height/2 <= ball_y )
            y+= speed;   
    
            LimitMovement();
      }
 };

CpuPaddle cpu;
Ball ball;
Paddle player;

int main()
{
    const int screen_width = 1200;
    const int screen_hight = 800;
    InitWindow(screen_width,screen_hight,"Khaldoze game");

    InitAudioDevice();
    Music bg_music = LoadMusicStream("bg_music.mp3"); 
    SetMusicVolume(bg_music, 1.0f);                   
    PlayMusicStream(bg_music);


    SetTargetFPS(60);

    ball.raduis=20;
    ball.x = screen_width/2;
    ball.y = screen_hight/2;
    ball.speed_x = 7;
    ball.speed_y = 7;
//!------------------
    player.width=25;
    player.height=120;
    player.x = screen_width - player.width -10;
    player.y = screen_hight/2 - player.height /2; 
    player.speed = 6;
//!------------------
    cpu.height = 120;
    cpu.width = 25;
    cpu.speed = 6;
    cpu.x = 10;
    cpu.y = screen_hight/2 - cpu.height/2;


    const char* title_text = "khaldoze pong";
    int font_size = 40;
    int text_width = MeasureText(title_text, font_size);

    while (WindowShouldClose() == false)
    {
        UpdateMusicStream(bg_music);
        BeginDrawing();
        //! Update
          ball.Update();
          player.Update();
          cpu.Update(ball.y);
        //! check
            if( CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.raduis, Rectangle{player.x, player.y, player.width, player.height}) )
                ball.speed_x *= -1;
            
            if( CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.raduis, Rectangle{cpu.x, cpu.y, cpu.width, cpu.height}) )
                ball.speed_x *= -1;

        //! drawing
        ClearBackground(Dark_Green);
        DrawRectangle(screen_width/2,0,screen_width /2 , screen_hight ,Green);
        DrawCircle(screen_width / 2, screen_hight / 2, 150, Light_Green);
        DrawLine(screen_width/2,0,screen_width/2,screen_hight, WHITE);
        DrawText(title_text, (screen_width / 2) - (text_width / 2), (screen_hight / 2) - (font_size / 2), font_size, Fade(WHITE, 0.7));
        ball.Draw();
        cpu.Draw();
        player.Draw();
        DrawText(TextFormat("%i",cpu_score),screen_width/4 -20,20,80,WHITE);
        DrawText(TextFormat("%i",player_score),3 * screen_width/4 -20,20,80,WHITE);

        EndDrawing();
    }
    




    CloseWindow();
    return 0;
}