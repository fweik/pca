/*
  Copyright (C) 2013 Florian Weik <florianweik@gmail.com>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
*/

#include <stdio.h>
#include <string.h>

#define FLOAT_TYPE double

#define MAXL 512

int main(int argc, char **argv) {
  FILE *b, *p, *f;
  char buf[MAXL];
  int id;
  FLOAT_TYPE x,y,z;
  int cnt = 0;

  if(argc != 3) {
    fprintf(stderr, "usage: %s <blockfile> <positionsfile>\n", argv[0]);
    return 128;
  }

  b = fopen( argv[1], "r");
  p = fopen( argv[2], "w");

  while(!feof(b)) {
    do {
      fgets(buf, MAXL, b);
    } while((strncmp("{part", buf, 5) != 0) && !feof(b));

    printf("Found configuration %d.\n", cnt++);

    while(!feof(b)) {
      int Tl = 0;
      fgets(buf, MAXL, b);

      if(strncmp("}", buf, 1) == 0)
	break;

      Tl = sscanf(buf, " {%d %lf %lf %lf} ", &id, &x, &y, &z);
      if(Tl == 4) {
	fprintf(p, "%.22f %.22f %.22f\n", x,y,z);
      }
    }
    fflush(p);
  }
  fclose(b);
  fclose(p);
}
