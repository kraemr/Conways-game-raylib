
#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <iostream>
#include <string.h>
#include "include/raylib.h"
const int WIDTH = 800;
const int HEIGHT = 800;
const int SIZE = WIDTH * HEIGHT * sizeof(unsigned int);
const int ALIVE = 1;
const int DEAD = 0;
typedef std::unique_ptr<unsigned int[]> cells_ptr;

int mod(int x, int y) {
    int m=x%y;
    if(m<0)m+=y;
    return m;
}

unsigned int count_neighbours(const cells_ptr& current_cells, int x, int y) {
    unsigned int neighbour_count = 0;
    for(int i = -1; i< 2; i++ ){
        for(int j = -1; j < 2; j++){
            if(i == 0 && j == 0) continue;
            if (current_cells[mod((x+i), WIDTH) + mod((y+j), WIDTH) * WIDTH])neighbour_count++;
        }
    }
    return neighbour_count;
}

void Draw_current(cells_ptr& current_cells,unsigned int x, unsigned int y){
    DrawPixel(x,y, current_cells[x*WIDTH+y] == ALIVE ? GREEN : BLACK);
}

void make_next_gen(cells_ptr& next_cells, std::unique_ptr<unsigned int[]>& current_cells) {
    memcpy(next_cells.get(), current_cells.get(), SIZE);
    for (int y=0; y<WIDTH; y++) {
        for (int x=0; x<WIDTH; x++) {
            uint32_t current_cell = current_cells[y * WIDTH + x];
            int neighbour_count = count_neighbours(current_cells,x, y);
            if (current_cell && !(neighbour_count == 2 || neighbour_count == 3)) {
                next_cells[y * WIDTH + x] = DEAD;
            }
            if (!current_cell && neighbour_count == 3) {
                next_cells[y * WIDTH + x] = ALIVE;
            }
            Draw_current(current_cells,x,y);
        }
    }
    std::swap(current_cells, next_cells);
}

void Init(const cells_ptr& current_cells ){
        for(int i=0;i<HEIGHT;i++){
            for(int j=0;j<WIDTH;j++){
                current_cells[j*WIDTH+i] = rand() % 100 +1 > 50 ? 1 : 0 ;
            }
        }
}

int main(int argc,char * argv[])
{
    InitWindow(WIDTH, HEIGHT, "ConwaysGameOfLife");
    SetTargetFPS(1000);
    std::unique_ptr<unsigned int[]> current_cells (new unsigned int[SIZE]);
    std::unique_ptr<unsigned int[]> next_cells (new unsigned int[SIZE]);
    memset(current_cells.get(), 0, SIZE);
    memset(next_cells.get(), 0, SIZE);
    Init(current_cells);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);
        make_next_gen(next_cells,current_cells);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
