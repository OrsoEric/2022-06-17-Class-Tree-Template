/****************************************************************
**	OrangeBot Project
*****************************************************************
**        /
**       /
**      /
** ______ \
**         \
**          \
*****************************************************************
**	Project
*****************************************************************
**  Brief
****************************************************************/

/****************************************************************
**	DESCRIPTION
****************************************************************
**
****************************************************************/

/****************************************************************
**	HISTORY VERSION
****************************************************************
**
****************************************************************/

/****************************************************************
**	KNOWN BUGS
****************************************************************
**
****************************************************************/

/****************************************************************
**	TODO
****************************************************************
**
****************************************************************/

/****************************************************************
**	INCLUDES
****************************************************************/

//Standard C Libraries
#include <cstdio>
//#include <cstdlib>

//Standard C++ libraries
#include <iostream>
//#include <array>
//#include <vector>
//#include <queue>
//#include <string>
//#include <fstream>
//#include <chrono>
//#include <thread>

//OS Libraries
//#define _WIN32_WINNT 0x0500	//Enable GetConsoleWindow
//#include <windows.h>

//User Libraries
//Include user log trace
#define ENABLE_DEBUG
#include "debug.h"

#include "tree.hpp"

/****************************************************************
**	NAMESPACES
****************************************************************/

//Never use a whole namespace. Use only what you need from it.
using std::cout;
using std::endl;

/****************************************************************
**	DEFINES
****************************************************************/

/****************************************************************
**	MACROS
****************************************************************/

/****************************************************************
**	PROTOTYPES
****************************************************************/

extern bool test_bench( void );

/****************************************************************
**	GLOBAL VARIABILES
****************************************************************/

//User::Dummy my_class;

/****************************************************************
**	FUNCTIONS
****************************************************************/

/****************************************************************************
**	@brief main
**	main |
****************************************************************************/
//! @return int |
//! @details Entry point of program
/***************************************************************************/

int main()
{
	//Start Debugging. Show function nesting level 0 and above
	DSTART( 0 );
	//Trace Enter main
	DENTER();

	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------

	cout << "OrangeBot Projects\n";
	//print in the 'debug.log' file. works just like a fully featured printf
	DPRINT("OrangeBot Projects\n");

	test_bench();

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return from main
	DRETURN();
	//Stop Debugging
	DSTOP();

    return 0;
}	//end function: main

/****************************************************************************
**	@brief Function
**	test_bench | bool
****************************************************************************/
//! @return bool |
//! @details test_bench for the code
/***************************************************************************/

bool test_bench( void )
{
	//Trace Enter with arguments
	DENTER();

	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//Temp return
	bool u1_ret;
	//Temp index
	unsigned int u32_index = 0;

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------
	//! @details algorithm:

	//Create a root with a payload
	User::Tree<int> cl_my_instance = User::Tree<int>( 99 );
	std::cout << cl_my_instance.get_error() << "\n";

	std::cout << "----------------------------------------\n";
	std::cout << "Add some leaves and some leaves to those leaves\n";
	//Add two leaves to the root
    cl_my_instance.create_leaf( 33 );
    cl_my_instance.create_leaf( 117 );
	//Add leaves to first leaf
	cl_my_instance[0].create_leaf( 34 );
	cl_my_instance[0].create_leaf( 35 );
	cl_my_instance[0].create_leaf( 36 );
	//Add leaves to second leaf
	cl_my_instance[1].create_leaf( 118 );
	//Test Bracket OOB protection
	u1_ret = cl_my_instance[2].create_leaf( 404 );
	if (u1_ret == true)
	{
		std::cout << "create leaf failed with error: " << cl_my_instance.get_error() << "\n";
	}
	//Printout the Tree
	cl_my_instance.print();

	std::cout << "----------------------------------------\n";
	std::cout << "Destroy a branch with leaves\n";
	//Delete a leaf and all it's sub leaves
    cl_my_instance.destroy_leaf( 0 );
    cl_my_instance.print();

    std::cout << "----------------------------------------\n";
	std::cout << "Create leaves using the branch index from creation\n";
    //Create a new leaf, and attach three leaves to it, knows the branch index
    u1_ret = cl_my_instance.create_leaf( 33, u32_index );
    if (u1_ret == false)
    {
		cl_my_instance[u32_index].create_leaf( 37 );
		cl_my_instance[u32_index].create_leaf( 38 );
		cl_my_instance[u32_index].create_leaf( 39 );
	}
	else
	{
		std::cout << "could not attach create leaf...\n";
	}
	cl_my_instance.print();

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN();
	return false; //OK
}	//end function: Dummy | bool

/****************************************************************************
**	@brief Function
**	f | bool
****************************************************************************/
//! @param f bool
//! @return bool |
//! @details
//! dummy method to copy the code
/***************************************************************************/

bool f( bool f )
{
	//Trace Enter with arguments
	DENTER_ARG("in: %d\n", 0);

	//----------------------------------------------------------------
	//	VARS
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	INIT
	//----------------------------------------------------------------

	//----------------------------------------------------------------
	//	BODY
	//----------------------------------------------------------------
	//! @details algorithm:

	//----------------------------------------------------------------
	//	RETURN
	//----------------------------------------------------------------

	//Trace Return vith return value
	DRETURN_ARG("out: %d\n", 0);
	return false; //OK
}	//end function: Dummy | bool
