#include "LedControl.h"

LedControl lc = LedControl(12,11,10,1);
#define left_p 4
#define right_p 5
#define up_p 6
#define down_p 7

int left_s = 0;
int right_s = 0;
int up_s = 0;
int down_s = 0;

int gui[8][8] = {{0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0},
                    {0,0,0,0,0,0,0,0}};


int fruit_x = random(0,7);
int fruit_y = random(0,7);
int snake_h_x, snake_h_y, snake_t_x, snake_t_y;

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    
    lc.shutdown(0, false);
    lc.setIntensity(0,8);
    lc.clearDisplay(0);

    pinMode(left_p, INPUT);
    pinMode(right_p, INPUT);
    pinMode(up_p, INPUT);
    pinMode(down_p, INPUT);

    generate_snake(); //fruit_x, fruit_y, gui
    
    delay(1000);
}

void generate_snake(){
    snake_h_x = random(1,6);
    snake_h_y = random(1,6);
    snake_t_y = snake_h_y;
    snake_t_x = snake_h_x - 1;
    
    gui[snake_h_x][snake_h_y] = 1;
    gui[snake_t_x][snake_t_y] = 1;
}

void generate_fruit(){ //int * fruit_x, int * fruit_y, int gui[8][8]
//    while(fruit_x == snake_x and fruit_x == snake_x - 1 and fruit_y == snake_y){
    fruit_x = random(0,7);
    fruit_y = random(0,7);  
    gui[fruit_x][fruit_y] = 1;
//    Serial.print(x);
//    Serial.print(" ");
//    Serial.print(y);
//    Serial.print("\n");
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
    left_s = digitalRead(left_p);
    right_s = digitalRead(right_p);
    up_s = digitalRead(up_p);
    down_s = digitalRead(down_p);
    
    if(left_s == HIGH){
        Serial.println("left");
    }
    else if(right_s == HIGH){
        Serial.println("right");
    }
    else if(up_s == HIGH){
        Serial.println("up");
    }
    else if(down_s == HIGH){
        Serial.println("down");
    }
}

void loop() {
//    generate_fruit(); //fruit_x, fruit_y, gui
      get_cmd();
      print_gui(); //gui
//    delay(100);
//     wash_gui(); //gui
      delay(1);
//    Serial.print(fruit_x);
//    Serial.print(" ");
//    Serial.print(fruit_y);
//    Serial.print("\n");
}
