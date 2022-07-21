#include "game.h"

#include <math.h>
#include <time.h>
#include <locale.h>

float dTime;

void Run(){

    float want_fps = 60;

    while(!glfwWindowShouldClose(window)){
        float startTime = glfwGetTime();
        glfwPollEvents();
        Update(dTime);
        Draw();
        engineLoop();
        float endTime = glfwGetTime();

        //Вычисление потенциала обновления кадров, для дальнейшего использования
        dTime = startTime - endTime;
        if(dTime < 0)
            dTime = -dTime;

        //Ограничение кадров
        int time_to_sleep = (1.f/want_fps - dTime);

        if (time_to_sleep > 0) {
            sleep(time_to_sleep);
        }
    }

    vkDeviceWaitIdle(device);
}

int main(){

    setlocale(LC_ALL,"Russian");

    initEngine(1024, 800, "My Game");

    Init();

    Run();

    CleanUp();

    return 0;
}
