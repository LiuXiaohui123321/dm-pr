# This is the Makefile of ABACUS-cell API

#==========================
# Compiler information 
#==========================
CPLUSPLUS     = icpc
CPLUSPLUS_MPI = mpiicpc
OBJ_DIR = cell_obj
NP      = 8

#==========================
# LIBS and INCLUDES
#==========================
LIBS = -lifcore -lm -lpthread

#==========================
# OPTIMIZE OPTIONS
#==========================
INCLUDES = -I. -Icommands 

# -pedantic turns off more extensions and generates more warnings
# -xHost generates instructions for the highest instruction set available on the compilation host processor
OPTS = ${INCLUDES} -Ofast -std=c++11 -simd -march=native -xHost -m64 -qopenmp -Werror -Wall -pedantic -g

include Makefile.Objects

VPATH=../module_base\
:../src_io\
:../src_pw\
:../src_parallel\
:./\

#==========================
# Define HONG
#==========================
HONG= -DMETIS -DMKL_ILP64 -D__MPI -D__CELL -D__NORMAL

FP_OBJS_0=main.o\
$(OBJS_IO)\
$(OBJS_PW)\
$(OBJS_BASE)\
$(OBJS_CELL)\
$(OBJS_PARALELL)\

FP_OBJS=$(patsubst %.o, ${OBJ_DIR}/%.o, ${FP_OBJS_0})

#==========================
# MAKING OPTIONS
#==========================
cell : 
	@ make init
	@ make -j $(NP) parallel

init :
	@ if [ ! -d $(OBJ_DIR) ]; then mkdir $(OBJ_DIR); fi
	@ if [ ! -d $(OBJ_DIR)/README ]; then echo "This directory contains all of the .o files" > $(OBJ_DIR)/README; fi

parallel : ${FP_OBJS} 
	${CPLUSPLUS_MPI} ${OPTS} $(FP_OBJS) ${LIBS} -o ${VERSION}.x 

#==========================
# rules
#==========================
${OBJ_DIR}/%.o:%.cpp
	${CPLUSPLUS_MPI} ${OPTS} ${OPTS_MPI} -c ${HONG} $< -o $@

.PHONY:clean
clean:
	@ if [ -d $(OBJ_DIR) ]; then rm -rf $(OBJ_DIR); fi
	@ if [ -e ${VERSION}.x ]; then rm -f ${VERSION}.x; fi