#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#define ULTRASONIC_THRESHOLD 50
#define CAMERA_COUNT 3
#define ALGORITHM_COUNT 4
#define LIGHT_COUNT 4
#define SIREN_COUNT 2
#define HAND
eye_prob = 0.7


bool open_gate(int gate_number);

bool close_gate(int gate_number);

bool enable_gate(int gate_number);

bool disable_gate(int gate_number);
float capture_ultrasonic(int us_sensor_number);
float capture_image(int camera_number);
float call_algorithm(int camera_number, int algorithm_number);
bool activate_light(int light_number);

bool deactivate_light(int light_number);
bool activate_siren(int siren_number);
bool deactivate_siren(int siren_number);

float capture_hand(void);
bool reset_system(void);

void *ultrasonic_monitor(void *arg);
void *camera_monitor(void *arg);

void *intruder_response(void *arg);

int main() {
    pthread_t ultrasonic_thread, camera_thread, response_thread;

    pthread_create(&ultrasonic_thread, NULL, ultrasonic_monitor, NULL);
    pthread_create(&camera_thread, NULL, camera_monitor, NULL);

    pthread_create(&response_thread, NULL, intruder_response, NULL);

    pthread_join(ultrasonic_thread, NULL);

    pthread_join(camera_thread, NULL);

    pthread_join(response_thread, NULL);

    return 0;
}

void *ultrasonic_monitor(void *arg) {
    while (1) {
        float avg_distance = 0.0;
        for (int i = 0; i < 6; i++) {
            avg_distance += capture_ultrasonic(i);
        }
        avg_distance /= 6;

        if (avg_distance < ULTRASONIC_THRESHOLD) {
            open_gate(1);
            pthread_exit(NULL);
        }
        sleep(1);
    }
}

void *camera_monitor(void *arg) {
    while (1) {
        
        sleep(1);

        // here we capture the cameas and count them 
        for (int i = 0; i < CAMERA_COUNT; i++) {
            capture_image(i);
        }

        float total_probability = 0.0;
        for (int i = 0; i < CAMERA_COUNT; i++) {


            float camera_probability = 0.0;


            for (int j = 0; j < ALGORITHM_COUNT; j++) {


                camera_probability += call_algorithm(i, j);


            }
            camera_probability /= ALGORITHM_COUNT; // fiding the avrage 

            total_probability += camera_probability;
        }

        total_probability /= CAMERA_COUNT;
        
        //  i will make the prb is 0.8 for camera
        if (total_probability <  0.8 ) { 

        }
        sleep(1);
    }
}

void *intruder_response(void *arg) {
    while (1) {
        sleep(1);

        // here we activate  lights and sirens
        for (int i = 0; i < LIGHT_COUNT; i++) {
            activate_light(i);
        }
        for (int i = 0; i < SIREN_COUNT; i++) {
            activate_siren(i);
        }

        // here we closing and  and disable the first gate
        close_gate(1);
        disable_gate(1);

        //  here we disabling  ultrasonic and camera modules
        for (int i = 0; i < 6; i++) {

            // just we disable here 
        }
        for (int i = 0; i < CAMERA_COUNT; i++) {

            // here disable the cameras 
        }

        
        sleep(10); // for wating for spcific time elapce 

        // then here we dictivated them 
        for (int i = 0; i < LIGHT_COUNT; i++) {
            deactivate_light(i);
        }
        for (int i = 0; i < SIREN_COUNT; i++) {
            deactivate_siren(i);
        }

         // then we enable first gate 
        enable_gate(1);
        for (int i = 0; i < 6; i++) {
            // enableing 
        }
        for (int i = 0; i < CAMERA_COUNT; i++) {
            // enabing the cmaersas 
        }

        // we open here the gate two 
        open_gate(2);
        float hand_prob = capture_hand();

        eye = capture_eye()


        if ((hand_prob + eye_prob) / 2 < 0.7) { 
            for (int i = 0; i < 2; i++) {
                activate_light(i + 4);  // here we have different lights
            }
            close_gate(2);
            disable_gate(2);
            while (!reset_system()) {
                sleep(1);
            }
        } else {
            enable_gate(1);
            enable_gate(2);
        }
        sleep(1);
    }
}

bool open_gate(int gate_number) {
    printf("the gate is %d opened.\n", gate_number);
    return true;
}

bool disable_gate(int gate_number) {
    printf("the gate is %d disabled.\n", gate_number);
    return true;
}

float capture_ultrasonic(int us_sensor_number) {

    return 30.0; // make it 30

}



bool activate_light(int light_number) {

    printf("light is  %d activated.\n", light_number);

    return true;
}


bool deactivate_siren(int siren_number) {

    printf("siren with number %d deactivated.\n", siren_number);
    return true;
}

float capture_hand(void) {
    return 0.8; 

bool reset_system(void) {

    printf("the system reset.\n");
    return true;

}