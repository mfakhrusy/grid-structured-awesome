#   
#	File name		: Makefile
#	Date			: 
#	Version			: 
#	Author			: 
#

DEST			= .

HDRS			= global.hpp				\
			  math_libs/math_function.hpp		\
			  pre_process/airfoil.hpp		\
			  pre_process/initialization.hpp	\
			  o_grid/o_grid_main.hpp		\
			  c_grid/c_grid_main.hpp		\

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
			  o_grid/o_grid_main.cpp			\
			  c_grid/c_grid_main.cpp			\

OBJS			= $(SRCS:.cpp=.o) 	

.cpp.o:
			$(COMPILER) $(OPTFLAG) -c $*.cpp -o $*.o 

all:			$(PROGRAM)

$(PROGRAM):		$(OBJS) $(LIBS)
			@echo -n "Loading Program $(PROGRAM) ... "
			@$(COMPILER) $(OPTFLAG) $(LDFLAGS) $(OBJS) $(LIBS) -o $(PROGRAM)
			@echo "done"

clean:;			@rm -f $(SRCS:.cpp=.o) $(SRCS:.cpp=.il) $(PROGRAM)


