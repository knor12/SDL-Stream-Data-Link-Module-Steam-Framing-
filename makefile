#Path to the compiler
CC := C:\MinGW_4_8_2\bin\gcc.exe
#name and  want to place the executable 
EXECUTABLE :=main.exe
#object files compilation flags
#CFLAGS := -Wall -Werror 
CFLAGS := -Wall  
#all directories that contain header files
INCS   :=-I"."
#list all the source files to be comilers
PATH_SOURCE=.
#SRCS := src\main_.c \
#		src\source.c \
#		src\source2.c
SRCS:=$(wildcard $(PATH_SOURCE)/*.c)
#generate the names of all object files to be created from the source files
OBJS := ${SRCS:.c=.o}


all: ${EXECUTABLE}
	@echo "***************************************"
	@echo "              Done                     "
	@echo "***************************************"
	

${EXECUTABLE}: ${OBJS}	
	@echo "***************************************"
	@echo "       Building executable             "
	@echo "***************************************"
	${CC} ${OBJS} -o ${EXECUTABLE}
##	${CC}  ${CFLAGS} ${INCS} -c $< -o $@

	
	
clean:
	@echo "***************************************"
	@echo "           Cleaning up                 "
	@echo "***************************************"
	del ${EXECUTABLE} ${OBJS}
	

	
.c.o:
	@echo "***************************************"
	@echo         Building object ${@} from ${<}
	@echo "***************************************"
	${CC}  ${CFLAGS} ${INCS} -c $< -o $@