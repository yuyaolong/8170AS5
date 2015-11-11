#--------------------------------------------------------------
#    Example Makefile for compiling programs that use
#    the Matrix and Vector classes.
#
#    The setup is to compile both example, and exampleSVD
#    example uses only the Vector classes
#    exampleSVD uses both Matrix and Vector classes
#
#    For a project using only Vector classes, all the
#    lines referring to Matrix can be omitted
#
#--------------------------------------------------------------
#
#  Programmer: Donald House
#  Date: March 8, 1999
#
#  Copyright (C) - Donald H. House. 2005
#

CC      = clang++
C	= cpp
H	= h
CFLAGS 	= -g

ifeq ("$(shell uname)", "Darwin")
  LDFLAGS     = -framework Foundation -framework GLUT -framework OpenGL -lm
else
  ifeq ("$(shell uname)", "Linux")
    LDFLAGS     = -L /usr/lib64/ -lglut -lGL -lGLU -lm
  endif
endif

HFILES 	= Vector.${H} Utility.${H} Matrix.${H} Camera.${H} Particle.${H} gauss.${H} rigidstate.${H} rigidstateA.${H} Quaternion.${H}
OFILES 	= Vector.o Utility.o Matrix.o Camera.o Particle.o gauss.o rigidstate.o rigidstateA.o Quaternion.o
PROJECT = main

${PROJECT}:	${PROJECT}.o $(OFILES)
	${CC} $(CFLAGS) -o ${PROJECT} ${PROJECT}.o $(OFILES) $(LDFLAGS)

${PROJECT}.o: ${PROJECT}.${C} $(HFILES)
	${CC} $(CFLAGS) -c ${PROJECT}.${C}

gauss.o: gauss.${C} gauss.${H}
	${CC} $(CFLAGS) -c gauss.${C}

Particle.o: Particle.${C} Particle.${H}
	${CC} $(CFLAGS) -c Particle.${C}

Camera.o: Camera.${C} Camera.${H} Matrix.${H} 
	${CC} $(CFLAGS) -c Camera.${C}

Matrix.o: Matrix.${C} Matrix.${H} Vector.${H} 
	${CC} $(CFLAGS) -c Matrix.${C}

Vector.o: Vector.${C} Vector.${H} Utility.${H} 
	${CC} $(CFLAGS) -c Vector.${C}

Utility.o: Utility.${C} Utility.${H}
	${CC} $(CFLAGS) -c Utility.${C}

rigidstate.o: rigidstate.${H} rigidstateA.${H} Vector.${H} Quaternion.${H}
	${CC} $(CFLAGS) -c rigidstate.${C}

rigidstateA.o: rigidstateA.${H} Vector.${H} Quaternion.${H}
	${CC} $(CFLAGS) -c rigidstateA.${C}

Quaternion.o: Quaternion.${H}
	${CC} $(CFLAGS) -c Quaternion.${C}



debug:
	make 'DFLAGS = /usr/lib/debug/malloc.o'

clean:
	rm *.o *~ ${PROJECT}

