// HW02: KIẾN TRÚC PHẦN MỀM VÀ QUẢN LÝ BỘ NHỚ SMART BIKE

#include <stdio.h>
#include <stdint.h>

// TASK 1: STRUCT, UNION & BIT-FIELDS

typedef union {
    uint16_t raw_value;
    struct {
        // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

        uint16_t PWR_ON         : 1;         
        uint16_t ASSIST_LEVEL   : 2;   
        uint16_t LIGHT_BRIGHT   : 4;  
        uint16_t RESERVED       : 9;                 
        
        // HỌC VIÊN KẾT THÚC VIẾT CODE
    } fields;
} Bike_Status_t;


// TASK 2: FUNCTION POINTERS & TABLE-DRIVEN ARCHITECTURE

void drive_eco(void) {
    printf("Bike Mode: ECO.\n");
}
void drive_normal(void) {
    printf("Bike Mode: NORMAL.\n");
}
void drive_sport(void) {
    printf("Bike Mode: SPORT.\n");
}

// HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY

typedef void (*Drive_Mode_Func_t)(void);

typedef enum {
    MODE_ECO = 0,
    MODE_NORMAL,
    MODE_SPORT,
    MODE_COUNT
} Bike_Mode_t;

static Drive_Mode_Func_t drive_modes[MODE_COUNT] = {
    drive_eco,
    drive_normal,
    drive_sport
};


// HỌC VIÊN KẾT THÚC VIẾT CODE


// TASK 3: ARCHITECTURE CALLBACKS

void Battery_Monitor(void (*overheat_cb)(void)) {
    int battery_temp = 45; 
    
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    if (battery_temp > 40 && overheat_cb != NULL) {
        overheat_cb();
    }

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}

void Critical_Battery_Handler(void) {
    printf("[WARNING] Battery is overheated!\n");
}


// TASK 4: MEMORY MAP & CRASH LAB

const char BIKE_MODEL[] = "E-Bike X2026"; 
uint32_t total_odometer = 0;             

void crash_simulation(void) {
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    uint32_t *invalid_ptr = NULL;

    printf("CRASH LAB: Trying to write to a NULL pointer...\n");

    *invalid_ptr = 0xDEADBEEF;

    // HỌC VIÊN KẾT THÚC VIẾT CODE
}


// HÀM MAIN KIỂM TRA (Học viên giữ nguyên để chạy thử nghiệm)

int main() {
    Bike_Status_t my_bike;
    
    // 1. Test Task 1
    my_bike.raw_value = 0x0025; 
    printf("REGISTER STRUCTURE: \n");
    printf("PWR_ON: %d | ASSIST_LEVEL: %d | LIGHT_BRIGHT: %d\n\n", 
           my_bike.fields.PWR_ON, my_bike.fields.ASSIST_LEVEL, my_bike.fields.LIGHT_BRIGHT);

    // 2. Test Task 2
    printf("ENGINE CONTROLLING: \n");
    // HỌC VIÊN BẮT ĐẦU VIẾT CODE TỪ ĐÂY
    Bike_Mode_t selected_mode = (Bike_Mode_t)my_bike.fields.ASSIST_LEVEL;

    if ((selected_mode >= MODE_ECO) &&
        (selected_mode < MODE_COUNT) &&
        (drive_modes[selected_mode] != NULL)) {
        (*drive_modes[selected_mode])();
    } else {
        printf("Invalid bike mode!\n");
    }

    printf("\n");


    // HỌC VIÊN KẾT THÚC VIẾT CODE

    // 3. Test Task 3
    Battery_Monitor(Critical_Battery_Handler);
    printf("\n");

    // 4. Test Task 4
    int current_speed = 25; 
    printf("MEMORY ADDRESSES OF VARIABLES \n");
    printf("BIKE_MODEL (ROM/.rodata):   %p\n", (void*)BIKE_MODEL);
    printf("total_odometer (RAM/.bss):  %p\n", (void*)&total_odometer);
    printf("current_speed (RAM/Stack):  %p\n", (void*)&current_speed);

    // Bỏ comment dòng dưới để chạy thử bài Crash Lab
    // crash_simulation();

    return 0;
}