#define dSINGLE

#include "game.h"

#include <math.h>
#include <time.h>
#include <locale.h>

float dTime = 0.1;

void Run(){

    float want_fps = 60;

    double lastTime = 0;
    int nbFrames = 0;

    while(!EngineWindowIsClosed()){
        double startTime = EngineGetTime();
        EnginePoolEvents();
        Update(dTime);
        Draw();
        engineLoop();
        double endTime = EngineGetTime();

        //Вычисление потенциала обновления кадров, для дальнейшего использования
        dTime = startTime - endTime;        
        if(dTime < 0)
            dTime = -dTime;

        //Ограничение кадров
        int time_to_sleep = (1.f/want_fps - dTime);

        if (time_to_sleep > 0) {
            sleep(time_to_sleep);
        }

        double curr_time = EngineGetTime();

        nbFrames++;
        if (curr_time - lastTime > 1.0f ){
            fpsCounter = nbFrames;
            nbFrames = 0;
            lastTime = curr_time;
        }
    }

    EngineDeviceWaitIdle();
}

int main(){

    setlocale(LC_ALL,"Russian");

    initEngine(1024, 800, "Test");

    Init();

    Run();

    CleanUp();

    return 0;
}
