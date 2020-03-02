#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

#include "ml6.h"
#include "display.h"
#include "draw.h"
#include "matrix.h"
#include "parser.h"


/*======== void parse_file () ==========
Inputs:   char * filename
          struct matrix * transform,
          struct matrix * pm,
          screen s
Returns:

Goes through the file named filename and performs all of the actions listed in that file.
The file follows the following format:
     Every command is a single character that takes up a line
     Any command that requires arguments must have those arguments in the second line.
     The commands are as follows:
         line: add a line to the edge matrix -
               takes 6 arguemnts (x0, y0, z0, x1, y1, z1)
         ident: set the transform matrix to the identity matrix -
         scale: create a scale matrix,
                then multiply the transform matrix by the scale matrix -
                takes 3 arguments (sx, sy, sz)
         move: create a translation matrix,
                    then multiply the transform matrix by the translation matrix -
                    takes 3 arguments (tx, ty, tz)
         rotate: create a rotation matrix,
                 then multiply the transform matrix by the rotation matrix -
                 takes 2 arguments (axis, theta) axis should be x y or z
         apply: apply the current transformation matrix to the edge matrix
         display: clear the screen, then
                  draw the lines of the edge matrix to the screen
                  display the screen
         save: clear the screen, then
               draw the lines of the edge matrix to the screen
               save the screen to a file -
               takes 1 argument (file name)
         quit: end parsing

See the file script for an example of the file format

IMPORTANT MATH NOTE:
the trig functions int math.h use radian mesure, but us normal
humans use degrees, so the file will contain degrees for rotations,
be sure to convert those degrees to radians (M_PI is the constant
for PI)
====================*/
void parse_file ( char * filename,
                  struct matrix * transform,
                  struct matrix * edges,
                  screen s) {

  FILE *f;
  char line[256];
  char holder[256];
  char ** inputs;
  color c;
  double x0,y0,z0,x1,y1,z1;
  c.red = 255;
  c.blue = 0;
  c.green = 255;
  struct matrix * current;
  current = new_matrix(4,4);
  clear_screen(s);

  if ( strcmp(filename, "stdin") == 0 )
    f = stdin;
  else
    f = fopen(filename, "r");

  while ( fgets(line, 255, f) != NULL ) {
    line[strlen(line)-1]='\0';
    printf(":%s:\n",line);
    if(!strcmp(line,"line") || !strcmp(line,"scale") || !strcmp(line,"move") || !strcmp(line,"rotate") || !strcmp(line,"save")){
      strcpy(holder,line);
    }else if(!strcmp(line,"quit") || !strcmp(line,"display") || !strcmp(line,"apply") || !strcmp(line,"ident")){
      if(!strcmp(line, "quit")){
        //
      }
      if(!strcmp(line,"display")){
        //clear_screen(s);
        draw_lines(edges,s,c);
        display(s);
      }
      if(!strcmp(line,"apply")){
        matrix_mult(transform, edges);

      }
      if(!strcmp(line,"ident")){
        ident(transform);
      }
    }else{
      inputs = parse_args(line, " ");
      if(!strcmp(holder,"line")){
        sscanf(inputs[0],"%lf",&x0);
        sscanf(inputs[1],"%lf",&y0);
        sscanf(inputs[2],"%lf",&z0);
        sscanf(inputs[3],"%lf",&x1);
        sscanf(inputs[4],"%lf",&y1);
        sscanf(inputs[5],"%lf",&z1);
        add_edge(edges, x0,y0,z0,x1,y1,z1);
      }
      if(!strcmp(holder,"scale")){
        sscanf(inputs[0],"%lf",&x0);
        sscanf(inputs[1],"%lf",&y0);
        sscanf(inputs[2],"%lf",&z0);
        current = make_scale(x0,y0,z0);
        matrix_mult(current, transform);
      }
      if(!strcmp(holder,"move")){
        sscanf(inputs[0],"%lf",&x0);
        sscanf(inputs[1],"%lf",&y0);
        sscanf(inputs[2],"%lf",&z0);
        current = make_translate(x0,y0,z0);
        matrix_mult(current, transform);
      }
      if(!strcmp(holder,"rotate")){
        if(!strcmp("x",inputs[0])){
          sscanf(inputs[1],"%lf",&x0);
          current = make_rotX(x0);
        }
        if(!strcmp("y",inputs[0])){
          sscanf(inputs[1],"%lf",&y0);
          current = make_rotY(y0);
        }
        if(!strcmp("z",inputs[0])){
          sscanf(inputs[1],"%lf",&z0);
          current = make_rotZ(z0);
        }
        matrix_mult(current, transform);
      }
      if(!strcmp(holder,"save")){
        //clear_screen(s);
        draw_lines(edges,s,c);
        save_extension(s, inputs[0]);
      }
    }
  }
}

void trim(char * input){
  char * last;
  int index = 0;
  int i = 0;
  // trims off the leading whitespaces
  while(!strcmp(" ",&input[index])){
    index++;
  }
  while(input[i + index] != '\0'){
    input[i] = input[i+index];
    i++;
  }
  input[i] = '\0';
  //trims off the trailing whitespaces
  last = input + strlen(input) - 1;
  while(last > input && isspace(*last)){
    last--;
  }
  *(last+1) = 0;
}

char ** parse_args( char * line , char * separator){
  //This function separates a line with a provided separator
  char ** parsed_args = malloc(256);
  char * current;
  int i = 0;
  //while you can continue to strsep, continue to strsep.
  while((current = strsep(&line, separator))){
    trim(current);
    parsed_args[i] = current;
    i++;
  }
  //when you have all the pieces, return the array of pieces.
  return parsed_args;
}
