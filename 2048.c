/*
 * Copyright (c) 2015 Ryan Hanson
 * 
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 * 
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgement in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 */

// Idea for game blatantly stolen from http://gabrielecirulli.github.io/2048/
// The code is at least mine
// Thanks Gabriele Cirulli for the idea

#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define GRID_WIDTH  4
#define GRID_HEIGHT 4
#define GRID_SIZE   (GRID_WIDTH * GRID_HEIGHT)

static void clear_grid(unsigned int * grid)
{
   int i;
   for(i = 0; i < GRID_SIZE; i++)
   {
      grid[i] = 0;
   }
}

static void print_grid(unsigned int * grid)
{
   int i, x, y;
   int size;
   int bigest_size;
   char tmp_buff[16];

   // Try to get the largest sized number
   bigest_size = 1;
   for(i = 0; i < GRID_SIZE; i++)
   {
      size = sprintf(tmp_buff, "%u", grid[i]);
      if(size > bigest_size)
      {
         bigest_size = size;
      }
   }

   // put everything onto the screen
   x = 0;
   y = 0;
   for(i = 0; i < GRID_SIZE; i++)
   {
      if(grid[i] == 0)
      {
         printf("[%*s] ", bigest_size, "");
      }
      else
      {
         printf("[%*u] ", bigest_size, grid[i]);
      }

      x ++;
      if(x >= GRID_WIDTH)
      {
         x = 0;
         y ++;
         printf("\n");
      }
   }
}

static void add_value_at_random(unsigned int * grid, unsigned int value)
{
   int free_cells;
   int spots_left;
   int i;
   int rand_num;

   int target_cell, cell;

   free_cells = 0;
   for(i = 0; i < GRID_SIZE; i++)
   {
      if(grid[i] == 0)
      {
         free_cells ++;
      }
   }

   if(free_cells > 0)
   {
      target_cell = rand() % free_cells;
      cell = 0;
      for(i = 0; i < GRID_SIZE; i++)
      {
         if(grid[i] == 0)
         {
            if(cell == target_cell)
            {
               grid[i] = value;
               break;
            }
            else
            {
               cell ++;
            }
         }
      }
   }
}

static int update_cell(unsigned int * grid, 
                       int num_index, 
                       int end_index, 
                       int delta_index, 
                       unsigned int * score)
{
   int i;
   int first_num_index, second_num_index;
   int valid_move;
   unsigned int value;

   if(grid[num_index] == 0)
   {
      first_num_index = -1;
   }
   else
   {
      first_num_index = num_index;
   }
   second_num_index = -1;

   i = num_index;
   while(i != end_index && second_num_index < 0)
   {

      i += delta_index;
      if(grid[i] != 0)
      {
         if(first_num_index < 0)
         {
            first_num_index = i;
         }
         else if(second_num_index < 0)
         {
            second_num_index = i;
         }
      }

   }

   

   // Move numbers
   if(first_num_index  >= 0 &&
      second_num_index >= 0)
   {
      if(grid[first_num_index] == grid[second_num_index])
      {
         value = grid[first_num_index] + grid[second_num_index];
         grid[first_num_index]  = 0;
         grid[second_num_index] = 0;
         grid[num_index]        = value;
         valid_move             = 1;
         if(score != NULL)
         {
            (*score) += value;
         }
      }
      else
      {
         value                 = grid[first_num_index];
         grid[first_num_index] = 0;
         grid[num_index]       = value;
         if(first_num_index != num_index)
         {
            valid_move = 1;
         }
         else
         {
            valid_move = 0;
         }
      }
   }
   else if(first_num_index >= 0)
   {
      value                 = grid[first_num_index];
      grid[first_num_index] = 0;
      grid[num_index]       = value;
      valid_move            = 1;
      if(first_num_index != num_index)
      {
         valid_move = 1;
      }
      else
      {
         valid_move = 0;
      }

   }
   else
   {
      valid_move = 0;
   }

   return valid_move;
}

static int move_up(unsigned int * grid, unsigned int * score)
{
   int i1, i2, x, y, end_index;
   int valid_move;

   valid_move = 0;
   i1 = 0;
   for(x = 0; x < GRID_WIDTH; x++)
   {
      i2 = i1;
      end_index = i1 + (GRID_SIZE - GRID_WIDTH);
      for(y = 0; y < GRID_HEIGHT; y++)
      {
         if(update_cell(grid, i2, end_index, GRID_WIDTH, score) == 1)
         {
            valid_move = 1;
         }  

         i2 += GRID_WIDTH;
      }
      i1 ++;
   }

   return valid_move;
}

static int move_down(unsigned int * grid, unsigned int * score)
{
   int i1, i2, x, y, end_index;
   int valid_move;

   valid_move = 0;
   i1 = GRID_SIZE - GRID_WIDTH;
   for(x = 0; x < GRID_WIDTH; x++)
   {
      i2 = i1;
      end_index = i1 - (GRID_SIZE - GRID_WIDTH);
      for(y = 0; y < GRID_HEIGHT; y++)
      {
         if(update_cell(grid, i2, end_index, -GRID_WIDTH, score) == 1)
         {
            valid_move = 1;
         }  

         i2 -= GRID_WIDTH;
      }
      i1 ++;
   }

   return valid_move;
}

static int move_left(unsigned int * grid, unsigned int * score)
{
   int i1, i2, x, y, end_index;
   int valid_move;

   valid_move = 0;
   i1 = 0;
   for(y = 0; y < GRID_HEIGHT; y++)
   {
      i2 = i1;
      end_index = i1 + (GRID_WIDTH - 1);
      for(x = 0; x < GRID_WIDTH; x++)
      {
         if(update_cell(grid, i2, end_index, 1, score) == 1)
         {
            valid_move = 1;
         }  

         i2 ++;
      }
      i1 += GRID_WIDTH;
   }

   return valid_move;
}

static int move_right(unsigned int * grid, unsigned int * score)
{
   int i1, i2, x, y, end_index;
   int valid_move;

   valid_move = 0;
   i1 = GRID_WIDTH - 1;
   for(y = 0; y < GRID_HEIGHT; y++)
   {
      i2 = i1;
      end_index = i1 - (GRID_WIDTH - 1);
      for(x = 0; x < GRID_WIDTH; x++)
      {
         if(update_cell(grid, i2, end_index, -1, score) == 1)
         {
            valid_move = 1;
         }  

         i2 --;
      }
      i1 += GRID_WIDTH;
   }

   return valid_move;
}


// Returns 1 if a move is avaliable
static int can_move(unsigned int * grid)
{
   int i, x, y;
   int result;

   result = 0;
   x      = 0;
   y      = 0;
   for(i = 0; i < GRID_SIZE; i++)
   {
      // Check for an empty cell
      if(grid[i] == 0)
      {
         result = 1;
         break;
      }
      
      // Check to see if the cell to the right matches
      if(x < GRID_WIDTH - 1)
      {
         if(grid[i] == grid[i + 1])
         {
            result = 1;
            break;
         }
      }

      // Check to see if the cell to the left matches
      if(y < GRID_HEIGHT - 1)
      {
         if(grid[i] == grid[i + GRID_WIDTH])
         {
            result = 1;
            break;
         }
      }

      // Update x and y
      x ++;
      if(x >= GRID_WIDTH)
      {
         x = 0;
         y ++;
      }
   }

   return result;
}

unsigned int max_value(unsigned int * grid)
{
   int i;
   unsigned int value;
   value = 0;
   for(i = 0; i < GRID_SIZE; i++)
   {
      if(grid[i] > value)
      {
         value = grid[i];
      }
   }
   return value;
}

int main(int args, char * argc[])
{
   unsigned int grid[GRID_SIZE];
   unsigned int score;
   unsigned int moves;
   unsigned int mv;

   int cmd;
   int running;
   int move_result;
   int valid_cmd;
   clock_t start, diff;
   float seconds;

   // Init board
   srand(time(NULL));
   clear_grid(grid);
   add_value_at_random(grid, 2);
   add_value_at_random(grid, 2);

   score = 0;
   moves = 0;

   printf("? <Enter> for help\n");

   // Play
   valid_cmd = 1;
   running   = 1;
   start = clock();
   while(running == 1)
   {
      if(valid_cmd == 1)
      {
         printf("s: %u\n", score);
         print_grid(grid);         
      }      
      cmd = getchar();
      if(cmd == EOF || cmd == 'q')
      {
         running     = 0;
         move_result = 0;
         valid_cmd   = 0;
      }
      else if(cmd == 'w')
      {
         move_result = move_up(grid, &score);
         valid_cmd   = 1;
      }
      else if(cmd == 's')
      {
         move_result = move_down(grid, &score);
         valid_cmd   = 1;
      }
      else if(cmd == 'a')
      {
         move_result = move_left(grid, &score);
         valid_cmd   = 1;
      }
      else if(cmd == 'd')
      {
         move_result = move_right(grid, &score);
         valid_cmd   = 1;
      }
      else if(cmd == '?')
      {
         valid_cmd   = 0;
         move_result = 0;
         printf("Move: [wasd] <Enter>\n");
         printf("Quit: q <Enter>\n");
      }
      else
      {
         move_result = 0;
         valid_cmd   = 0;
      }

      if(move_result == 1)
      {
         if(rand() >= (RAND_MAX / 10)) // 10% chance to add 4
         {
            add_value_at_random(grid, 2);
         }
         else
         {
            add_value_at_random(grid, 4);
         }
         moves ++;

         if(can_move(grid) == 0)
         {
            printf("s: %u\n", score);
            print_grid(grid);
            printf("No Moves Left\n");
            running = 0;
         }
      }

   }

   diff = clock() - start;
   
   seconds = (float)(diff / (double)CLOCKS_PER_SEC);
   mv = max_value(grid);

   printf("Score:          %i\n", score);
   printf("Max Value:      %i\n", mv);
   printf("Time:           %.2f seconds\n", seconds);
   printf("Moves:          %i\n", moves);
   if(moves > 0)
   {
      printf("Sec Per Move:   %.2f\n", seconds / (float)moves);
   }
   if(seconds > 0.01)
   {
      printf("Score Per Sec:  %.2f\n", score / seconds);
   }
   return 0;
}
