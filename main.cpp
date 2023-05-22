/* mbed Microcontroller Library
 * Copyright (c) 2019 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mbed.h"
#include <chrono>
#include <cstdio>
#define pause D2
#define refresh D7
#include "DFRobot_RGBLCD.h"
#include "Timer.h"
#define USE_STM_HAL_API 1
#define BLINKING_RATE 1000ms

using namespace std::chrono;


DFRobot_RGBLCD lcd(16, 2, D14, D15);

Timer t;
float start_time = 60.0;
float remaning_time = 0;//= start_time - t.read();

//#define WAIT_TIME_MS 500 
InterruptIn btn1 (D2, PullUp);
InterruptIn btn2 (D7, PullUp);
InterruptIn btn3 (D4, PullUp);
InterruptIn btn4 (D8, PullUp);

PwmOut buzzer (D3);

void pause_timer(bool *paused) {
    if(*paused) {
        t.start();
    } else {
        t.stop();
    }

    *paused = !*paused;
}

void Reset_timer()
{
    t.reset();
    t.stop();
    start_time = 60.0;
}

void p_timer()
{

    start_time+= 5.0;
    remaning_time = start_time - t.read();

}

void m_timer()
{  
    start_time -= 5.0;
    remaning_time = start_time - t.read();
    
}

void func(int *x) {
t.start();
}



int main()
{
    buzzer.period_ms(1);
    remaning_time = start_time - duration_cast<microseconds>(t.elapsed_time()).count();
   
    
    lcd.init();
    t.start(); 

    bool paused = false;
   
    btn1.fall(callback(pause_timer, &paused));
    btn2.fall(Reset_timer);
    btn3.fall(p_timer);
    btn4.fall(m_timer);
    //btn2.rise(callback(func, &var));
    
    

    while (true)
    {
        remaning_time = start_time - t.read();
      
        //lcd.clear(); 
        lcd.setCursor(0, 0);
         
        if (remaning_time <= 0) {
            lcd.printf("Alarm!");
        } else {
            lcd.printf("Time: %f", remaning_time);
        }
        if (remaning_time <= 0)
        {
        //buzzer.pulsewidth_us(100);
        buzzer.write(0.5f);
        //buzzer.period(0.5f);
        }else 
        {
         //buzzer.suspend();
         buzzer.write(0);
        }  

  

  }

        //wait_us(100000);
        ThisThread::sleep_for(1s);
}
