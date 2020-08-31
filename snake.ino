#include "LedControl.h"

LedControl lc = LedControl(12,11,10,1);

#define buttonpin A0

int gui[8][8] = {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};

int lose_gui[8][8] = {{1,0,0,0,0,0,0,1},
                     {0,1,0,0,0,0,1,0},
                    {0,0,1,0,0,1,0,0},
                    {0,0,0,1,1,0,0,0},
                    {0,0,0,1,1,0,0,0},
                    {0,0,1,0,0,1,0,0},
                    {0,1,0,0,0,0,1,0},
                    {1,0,0,0,0,0,0,1}};

int fruit_x, fruit_y;

int snake_gui[64][2];

int snake_size = 3;
int snake_h_x, snake_h_y; // snake_t_x, snake_t_y; //head and tail

bool game_state = false;

unsigned long timer = 0;

int last_direction = 2;
int direction_ = 3;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    lc.shutdown(0, false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);

    generate_snake(); //fruit_x, fruit_y, gui
    generate_fruit();
    draw_gui();
    print_gui();
    delay(1000);
//    Serial.println(snake_h_x);
//    Serial.println(snake_h_y);
}

void generate_snake(){
    snake_h_x = random(1,4);
    snake_h_y = random(1,4);
    Serial.println(snake_h_x);
    Serial.println(snake_h_y);
    snake_h_x = snake_h_y;
    snake_gui[2][0] = snake_h_x; 
    snake_gui[2][1] = snake_h_y;
    snake_gui[1][0] = snake_h_x + 1; 
    snake_gui[1][1] = snake_h_y;
    snake_gui[0][0] = snake_h_x + 2; 
    snake_gui[0][1] = snake_h_y;
}

bool compare_snake_fruit(){
    for(int i = 0; i <= snake_size; i++){
        if(snake_gui[i][0] == fruit_x and snake_gui[i][1] == fruit_y)
            return true;
    }
    return false;
}

void generate_fruit(){ //int * fruit_x, int * fruit_y, int gui[8][8]
    while(compare_snake_fruit()){
        fruit_x = random(0,7);
        fruit_y = random(0,7);  
        //fruit_x = fruit_y; 
    }
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.print("\n");
}

void draw_gui(){
    gui[fruit_x][fruit_y] = 1;
    for(int i = 0; i < snake_size; i++){
        int x = snake_gui[i][0];
        int y = snake_gui[i][1];
        gui[x][y] = 1;
    }
}

void print_gui(){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            lc.setLed(0, col, row, gui[col][row]);
            //delay(100);
        }
    }
}

void wash_gui(){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            gui[col][row] = 0;
        }
    }
}

void get_cmd(){
    int value = analogRead(buttonpin);
    //Serial.println(value);    

    if(value >= 1020 and last_direction != 2){
        //Serial.println("down");
        direction_ = 1;
    }
    else if(value > 880 and value < 930 and last_direction != 1){
        //Serial.println("up");
        direction_ = 2;
    }
    else if(value > 980 and value < 1000 and last_direction != 4){
       // Serial.println("left");
        direction_ = 3;
    }
    else if(value < 600 and value > 500 and last_direction != 3){
        //Serial.println("right");
        direction_ = 4;
    }

    last_direction = direction_;
}

void snake_control(){
    if(direction_ == 1){ //down
        snake_h_y--;
    }
    else if(direction_ == 2){ //up
        snake_h_y++;
    }
    else if(direction_ == 3){ //left
        snake_h_x--;
    }
    else if(direction_ == 4){ //right
        snake_h_x++;
    }
    
        
    snake_gui[snake_size][0] = snake_h_x;
    snake_gui[snake_size][1] = snake_h_y;
    
    int temp[64][2];
    for(int i = 0; i < snake_size; i++){
        temp[i][0] = snake_gui[i+1][0];
        temp[i][1] = snake_gui[i+1][1];
    }
    for(int i = 0; i < snake_size; i++){
        snake_gui[i][0] = temp[i][0] ;
        snake_gui[i][1] = temp[i][1];
    }
    free(temp);
    Serial.print(snake_h_x);
    Serial.print(" ");
    Serial.print(snake_h_y);
    Serial.print("\n");
}

bool check_game_state(){
    if((snake_h_x < 0 or snake_h_x > 7) or (snake_h_y < 0 or snake_h_y > 7)){
        return false;
    }
    return true;
}

void loop() {
//    generate_fruit(); //fruit_x, fruit_y, gui
      wash_gui();
      get_cmd();
      unsigned long now_ = millis();
      if(check_game_state()){
          if(now_ - timer > 2000){
              snake_control();
              timer = now_;
          }
      }
      if(check_game_state()){
         draw_gui();
         print_gui();
      }

}
