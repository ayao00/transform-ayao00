#ifndef PARSER_H
#define PARSER_H

#include "matrix.h"
#include "ml6.h"

void parse_file ( char * filename,
		  struct matrix * transform,
		  struct matrix * edges,
		  screen s);
int arguments (char * line);
void trim(char * input);
char ** parse_args( char * line , char * separator);
#endif
