#include "hexEditor.h"

/* Variable global para mejor legibilidad */
int fd; // Archivo a leer


char *hazLinea(char *base, int dir) {
	char linea[100]; // La linea es mas pequeña
	int o=0;
	// Muestra 16 caracteres por cada linea
	o += sprintf(linea,"%08x ",dir); // offset en hexadecimal
	for(int i=0; i < 4; i++) {
		unsigned char a,b,c,d;
		a = base[dir+4*i+0];
		b = base[dir+4*i+1];
		c = base[dir+4*i+2];
		d = base[dir+4*i+3];
		o += sprintf(&linea[o],"%02x %02x %02x %02x ", a, b, c, d);
	}
	for(int i=0; i < 16; i++) {
		if (isprint(base[dir+i])) {
			o += sprintf(&linea[o],"%c",base[dir+i]);
		}
		else {
			o += sprintf(&linea[o],".");
		}
	}
	sprintf(&linea[o],"\n");

	return(strdup(linea));
}

char *mapFile(char *filePath) {
    /* Abre archivo */
    fd = open(filePath, O_RDONLY);
    if (fd == -1) {
    	perror("Error abriendo el archivo");
	    return(NULL);
    }

    /* Mapea archivo */
    struct stat st;
    fstat(fd,&st);
    int fs = st.st_size;

    char *map = mmap(0, fs, PROT_READ, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
    	close(fd);
	    perror("Error mapeando el archivo");
	    return(NULL);
    }

  return map;
}

int leeChar() {
  int chars[5];
  int ch,i=0;
  nodelay(stdscr, TRUE);
  while((ch = getch()) == ERR); /* Espera activa */
  ungetch(ch);
  while((ch = getch()) != ERR) {
    chars[i++]=ch;
  }
  /* convierte a numero con todo lo leido */
  int res=0;
  for(int j=0;j<i;j++) {
    res <<=8;
    res |= chars[j];
  }
  return res;
}


int edita(char *filename) {

    int x=0, y=0, c=0, rn, cl;
    /* Lee archivo */
    char *map = mapFile(filename);
    if (map == NULL) {
      exit(EXIT_FAILURE);
      }
    
    for(int i= 0; i<25; i++) {
    	// Haz linea, base y offset
    	char *l = hazLinea(map,i*16);
	    mvprintw(i,0,l);
    }
    do{
      if(cl>60){
        y+=1;
        x=0;
      }
      rn = 0+y;
      cl = (x < 16) ? 9+x*3 : 41+x;
      move(rn, cl); // Renglón, columna
      refresh();

      c = leeChar();
      switch(c){
        case KEY_UP: // Flecha arriba
          y=y-1;
          break;
        case KEY_DOWN:
          y +=1;
          break;
        case KEY_RIGHT:
          x+=1;
          break;
        case KEY_LEFT:
          x-=1;
          break;
      }
    }while(c!=24);

    leeChar();

    if (munmap(map, fd) == -1) {
      perror("Error al desmapear");
    }
    close(fd);
    
   return 0;

}
    

int hexEditor(char *filename){

  // Inicia curses
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho(); // No muestra los caracteres leidos
  cbreak(); 

  edita(filename);
  // Termina curses
  endwin();

  return 0;

}
