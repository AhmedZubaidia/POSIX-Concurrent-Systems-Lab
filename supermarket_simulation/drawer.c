
#include "header.h"
#include <GL/glut.h>
#include "shared_memories.h"
#include "customer_header.h"
#include "shelving_team.h"
#include "functions.h"
#include "processing_times.h"

unsigned int read_queue_size();

void sigterm_handler2() ;


// functions for openGl
void display(void);
void reshape(int w, int h);
void timer(int z);
void lines_for_storage();
void manger_box();
void draw_manger_place();
void market_wait_hall();
void draw_open_close_door();
void drawChairs();
void drawText();
void renderText(int, int, void *, char *);
void readDataFromSharedMemory();
void renderShelves();
void renderStorage();
void drawPoints(int chairGridSizeX,int chairGridSizeY, int chairShiftX, int chairShiftY,float *color_filled , float *color_empty ,int variable_to_draw);
void drawProductsOnShelves();
void drawTeams();
void restock_light_lamb();
void blinkTimer(int value);

bool blinkState = true;
bool keepBlinking = true; // Global variable to control blinking

int customer_count = 0;
int cashier1_totalCount = 0;
int  cashier3_totalCount = 0;
int cashier2_totalCnt=0;

Product items_count[MAXIMUM_NUM_OF_PRODUCTS];

int milk_count = 0;
int eggs_count = 0;
int bread_count = 0;
int apples_count = 0;

int milk_in_shelf = 0;
int eggs_in_shelf = 0;
int bread_in_shelf = 0;
int apples_in_shelf = 0;


int fram_rate = 1900 / 2;

float mouseX = 0.0f;
float mouseY = 0.0f;
int windowWidth = 1800;
int windowHeight = 1000;


char *drawer_current_time = "";


void mouseMotion(int x, int y) {
    // Convert pixel coordinates (x, y) to OpenGL coordinates
    mouseX = (x / (float)windowWidth) * 40.0f - 20.0f;
    mouseY = ((windowHeight - y) / (float)windowHeight) * 40.0f - 20.0f;
    glutPostRedisplay(); // Add this line to ensure the display function is called to update the rendering
}



int main(int argc, char **argv)
{

    read_items("supermarket_items.txt");
    c = read_config("config.txt");

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(1800, 1000);
    glutCreateWindow("Super Market");
    glLineWidth(5);
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutPassiveMotionFunc(mouseMotion);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(500, blinkTimer, 0);
    glutMainLoop();
    return 0;
}
void display(void) {
    // Clear the color buffer
    glClear(GL_COLOR_BUFFER_BIT);

    // All your drawing operations.
    lines_for_storage();
    manger_box();
    draw_manger_place();
    market_wait_hall();
    draw_open_close_door();

    float color_filled[3] = {1.0, 0.1, 0.1};
    float color_empty[3] = {0.0, 0.0, 1.0};
    drawPoints(10,10,0,-7,color_filled,color_empty,customer_count); // customer count

    drawText();
    renderShelves();
    renderStorage();
    drawProductsOnShelves();
    drawTeams();
    restock_light_lamb();

    // Render the mouse coordinates
    char mouseCoords[50];
    sprintf(mouseCoords, "Mouse X: %.2f Y: %.2f", mouseX, mouseY);
    renderText(10, -18, GLUT_BITMAP_TIMES_ROMAN_24, mouseCoords);

    // Swap the buffers at the very end after all drawing commands
    glutSwapBuffers();
}
void sigterm_handler2() {

    exit(0);
}


void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-20, 20, -20, 20);
    glMatrixMode(GL_MODELVIEW);
}
void timer(int z)
{
    glutTimerFunc(fram_rate, timer, 0);
    glutPostRedisplay();
    readDataFromSharedMemory();
}
void lines_for_storage()
{
    glBegin(GL_LINES);
    glVertex2f(-20.0, -13.0);
    glVertex2f(-10.0, -13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-20.0, 13.0);
    glVertex2f(-10.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-10.0, 13.0);
    glVertex2f(-10.0, 11.0);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glVertex2f(-10.0, -13.0);
    glVertex2f(-10.0, -10.0);
    glEnd();
    glFlush();

    glBegin(GL_LINES);
    glVertex2f(13.7, -13.0);
    glVertex2f(13.7, 13.0);
    glEnd();
    glFlush();





}
void drawBox(float x, float y, float width, float height) {
    // Top line
    glBegin(GL_LINES);
    glColor3f(1.0, 1.0, 255);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glEnd();

    // Bottom line
    glBegin(GL_LINES);
    glVertex2f(x, y - height);
    glVertex2f(x + width, y - height);
    glEnd();

    // Left line
    glBegin(GL_LINES);
    glVertex2f(x, y);
    glVertex2f(x, y - height);
    glEnd();

    // Right line
    glBegin(GL_LINES);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y - height);
    glEnd();

    glFlush();
}

void manger_box() // cashier border
{
    float startY = 13.0;
    float totalSpace = 26.0; // startY - (-13.0)
    int numLines = 4;
    float space = totalSpace / (numLines - 1);

    // Calculate y-coordinates for each cashier
    float y1 = startY - 1 * space;
    float y2 = startY - 2 * space;
    float y3 = startY - 3 * space;

    // Adjust box sizes if necessary
    float boxWidth = 2.0;
    float boxHeight = 2.0;

    // Draw box for cashier 1
    drawBox(-7.0, y1 + 2, boxWidth, boxHeight);

    // Draw box for cashier 2
    drawBox(-7.0, y2 + 2, boxWidth, boxHeight);

    // Draw box for cashier 3
    drawBox(-7.0, y3 + 2, boxWidth, boxHeight);
}



void draw_manger_place() // cashier1 queue,  cashier2 queue , cashier3 queue
{

    float startY = 13.0;   // Starting y-coordinate (upper limit)
    float endY = -13.0;    // Ending y-coordinate (lower limit)
    int numLines = 4;      // Number of lines
    float totalSpace = startY - endY;
    float space = totalSpace / (numLines - 1);

    for (int i = 0; i < numLines; i++) {
        float y = startY - i * space;

        glBegin(GL_LINES);
        glColor3f(0, 255, 0);
        glVertex2f(-10.0, y);
        glVertex2f(-2.0, y);
        glEnd();

        // Drawing points representing cashiers
        if (i > 0) {
            glPointSize(10);
            glBegin(GL_POINTS);
            glColor3f(0, 255, 0);
            glVertex2f(-6.0, y + 1);
            glEnd();
        }
    }

    glFlush();


}


void market_wait_hall()
{
    glColor3ub(255, 0, 255);

    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(-2.0, 11.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(-2.0, -10.0);
    glEnd();


    glBegin(GL_LINES);
    glVertex2f(-2.0, 13.0);
    glVertex2f(20.0, 13.0);
    glEnd();

    glBegin(GL_LINES);
    glVertex2f(-2.0, -13.0);
    glVertex2f(20.0, -13.0);
    glEnd();
    glFlush();
}
void draw_open_close_door()
{

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(0.5, 255, 0);
    glVertex2f(4, -13.0);
    glEnd();
    glFlush();

    glPointSize(20);
    glBegin(GL_POINTS);
    glColor3f(255, 0, 0);
    glVertex2f(-15, -13.0);
    glEnd();
    glFlush();

}



void drawPoints(int chairGridSizeX,int chairGridSizeY, int chairShiftX, int chairShiftY,float *color_filled , float *color_empty, int variable_to_draw) {



    int variable = 0, jTemp = 0, iTemp = 0;

    if (variable_to_draw == 0) {
        glBegin(GL_POINTS);
        glColor3f(color_empty[0], color_empty[1], color_empty[2]);
        for (int i = chairGridSizeX - 1; i >= 0; i--) {
            for (int j = -chairGridSizeY / 2; j < chairGridSizeY / 2; j++) {
                glVertex2i(i + chairShiftX, j + chairShiftY);
            }
        }
        glEnd();
        glFlush();
        return;
    }

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(color_filled[0], color_filled[1], color_filled[2]);
    for (int i = chairGridSizeX - 1; i >= 0; i--)
    {
        iTemp = i;
        int f = 0;
        for (int j = -chairGridSizeY / 2; j < chairGridSizeY / 2; j++)
        {
            jTemp = j;
            glVertex2i(i + chairShiftX, j + chairShiftY);
            variable++;
            if (variable == variable_to_draw)
            {
                f = 1;
                break;
            }
        }
        if (f)
            break;
    }
    glEnd();

    glPointSize(5);
    glBegin(GL_POINTS);
    glColor3f(color_empty[0], color_empty[1], color_empty[2]);
    for (int i = iTemp; i >= 0; i--)
    {
        for (int j = jTemp + 1; j < chairGridSizeY / 2; j++)
        {
            glVertex2i(i + chairShiftX, j + chairShiftY);
        }
        jTemp = -chairGridSizeY / 2 - 1.5;
    }
    glEnd();
    glFlush();
}


void drawProductsOnShelves() {

    for(int i =0 ; i<c.numberOfProducts ; i++){

       float filled[3] = {0.0, 1.0, 0.0};
       float empty[3] = {1.0, 1.0, 1.0};

        if(i+1 >5){
            drawPoints( 2,10, -1 +3*4, -7, filled,empty,items_count[i].shelfAmount);
            break;
        }
        drawPoints( 2,10, -1 +3*i, 7, filled,empty,items_count[i].shelfAmount);

    }

}

void drawTeams(){



            float filled[3] = {1.0, 0.0, 0.0};
            float empty[3] = {1.0, 1.0, 1.0};


            for(int i=0 ; i<c.numberOfShelvingTeams ; i++){

                if(i+1 >3){
                    break;
                }
                drawPoints( 7,2, -9 , 9 - 9*i, filled,empty,c.numberOfEmployeesPerTeam);


            }




}



void formatAndRenderText(float x, float y, void* font, const char* label, int value, char* buffer, size_t bufferSize) {
    snprintf(buffer, bufferSize, "%s%d", label, value);
    renderText(x, y, font, buffer);
}

void drawText() {
    glColor3f(1.0, 0.8, 0.5);
    char buffer[40];

    for(int i =0 ; i<c.numberOfShelvingTeams ; i++){

            if(i+1 >3){
                break;
            }

        if(i == 0)
        formatAndRenderText(-9, 12, GLUT_BITMAP_TIMES_ROMAN_24, "Team 1 Employees Count: ", c.numberOfEmployeesPerTeam, buffer, sizeof(buffer));

        if(i == 1)
        formatAndRenderText(-9, 3, GLUT_BITMAP_TIMES_ROMAN_24, "Team 2 Employees Count: ", c.numberOfEmployeesPerTeam, buffer, sizeof(buffer));

        if(i == 2)
        formatAndRenderText(-9, -15, GLUT_BITMAP_TIMES_ROMAN_24, "Team 3 Employees Count: ", c.numberOfEmployeesPerTeam, buffer, sizeof(buffer));

    }


    formatAndRenderText(2, -2, GLUT_BITMAP_TIMES_ROMAN_24, "Customer Count: ", customer_count, buffer, sizeof(buffer));

    renderText(13, 18, GLUT_BITMAP_TIMES_ROMAN_24, drawer_current_time);
}

void renderText(int x, int y, void *font, char *string)
{
    glRasterPos2f(x, y);
    int len, i;
    len = strlen(string);
    for (i = 0; i < len && string[i] != '\0'; i++)
    {
        glutBitmapCharacter(font, string[i]);
    }
}


void renderShelves() {
    glColor3f(1.0, 0.8, 0.5);

    // Assuming items_count is an array of Item structures
    for (int i = 0; i < c.numberOfProducts; i++) { // Assuming you have 4 items as per your shared memory structure
        char shelfInfo[200];
        snprintf(shelfInfo, sizeof(shelfInfo), "%s -Shelf: %d",
                 items_count[i].name, items_count[i].shelfAmount);

        float yPosition = 10 - i * 2; // Adjust the position for each item
        renderText(15, yPosition, GLUT_BITMAP_TIMES_ROMAN_24, shelfInfo);
    }

}


void renderStorage() {
    glColor3f(1.0, 0.8, 0.5);

    // Assuming items_count is an array of Item structures
    for (int i = 0; i < c.numberOfProducts; i++) { // Assuming you have 4 items as per your shared memory structure
        char shelfInfo[200];
        snprintf(shelfInfo, sizeof(shelfInfo), "%s Storage: %d",
                 items_count[i].name, items_count[i].storageAmount);

        float yPosition = 10 - i * 2; // Adjust the position for each item
        renderText(-18, yPosition, GLUT_BITMAP_TIMES_ROMAN_24, shelfInfo);
    }

}

void blinkTimer(int value) {
    blinkState = !blinkState;  // Toggle blink state
    glutPostRedisplay();       // Redraw screen
    glutTimerFunc(500, blinkTimer, 0); // Set timer for next blink
}

void renderBlinkingText(float x, float y, void* font, const char* text) {
    if (blinkState) {
        renderText(x, y, font, text); // Only render text if blinkState is true
    }
}
void restock_light_lamb() {

    glColor3f(1.0, 1.0, 1.0);
    keepBlinking = false; // Reset blinking control

    // Assuming items_count is an array of Item structures
    for (int i = 0; i < c.numberOfProducts; i++) { // Assuming you have c.numberOfProducts items
        char shelfInfo[200];

        if (i+1 > 5) {
            if ((items_count[i].team_working_on_it != 0) && (i+1 ==6)) {
                keepBlinking = true; // Enable blinking
                snprintf(shelfInfo, sizeof(shelfInfo), "Team:%d Restocking %s ", items_count[i].team_working_on_it, items_count[i].name);
                if (blinkState) {
                    renderText(-1.5 + 3 * 4, -2, GLUT_BITMAP_HELVETICA_10, shelfInfo); // Render text only if blinkState is true
                }
            }
            break;
        }

        if (items_count[i].team_working_on_it != 0) {
            keepBlinking = true; // Enable blinking
            snprintf(shelfInfo, sizeof(shelfInfo), "Team:%d Restocking %s ", items_count[i].team_working_on_it, items_count[i].name);
            if (blinkState) {
                renderText(-1 + 2.9 * i, 12, GLUT_BITMAP_HELVETICA_10, shelfInfo); // Render text only if blinkState is true
            }
        }
    }

    // If no team is working, ensure text is hidden
    if (!keepBlinking) {
        blinkState = false;
        glutPostRedisplay();
    }
}



// read data from shared memory in this function
void readDataFromSharedMemory()
{



    customer_count = get_total_customers();

    get_shared_products(items_count);

    milk_count = items_count[0].storageAmount;
    eggs_count = items_count[1].storageAmount;
    bread_count = items_count[2].storageAmount;
    apples_count = items_count[3].storageAmount;

    milk_in_shelf = items_count[0].shelfAmount;
    eggs_in_shelf = items_count[1].shelfAmount;
    bread_in_shelf = items_count[2].shelfAmount;
    apples_in_shelf = items_count[3].shelfAmount;

    drawer_current_time = time_to_string();





}
