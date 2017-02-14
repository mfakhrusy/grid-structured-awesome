#   
#	File name		: Makefile
#	Date			: 
#	Version			: 
#	Author			: 
#

DEST			= .

HDRS			= global.hpp				\
			  math_libs/spline.h			\
			  math_libs/math_function.hpp		\
			  pre_process/airfoil.hpp		\
			  pre_process/initialization.hpp	\
			  grid_generator/o_grid_main.hpp	\
			  grid_generator/c_grid_main.hpp	\
			  grid_generator/grid_process.hpp	\

LIBS			=	

INPS			=	

COMPILER		= g++ 

OPTFLAG			= -std=c++11 -O2

MAKEFILE		= Makefile


PROGRAM			= Structured_Grid

SRCS			= main.cpp					\
			  math_libs/math_function.cpp			\
			  pre_process/airfoil.cpp			\
			  pre_process/initialization.cpp		\
			  grid_generator/o_grid_main.cpp		\
			  grid_generator/c_grid_main.cpp		\
			  grid_generator/grid_process.cpp	\

OBJS			= $(SRCS:.cpp=.o) 	

.cpp.o:
			$(COMPILER) $(OPTFLAG) -c $*.cpp -o $*.o 

all:			$(PROGRAM)

$(PROGRAM):		$(OBJS) $(LIBS)
			@echo -n "Loading Program $(PROGRAM) ... "
			@$(COMPILER) $(OPTFLAG) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
			@echo "done"

clean:;			@rm -f $(SRCS:.cpp=.o) $(SRCS:.cpp=.il) $(PROGRAM)


