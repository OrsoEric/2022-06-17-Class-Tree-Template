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

//Test bench for the priority swap
extern bool test_swap_priority( User::Tree<int> &icl_tree );
//Test bench for the subtree swap targeting children not part of the same bloodline
extern bool test_swap_independent_subtree( User::Tree<int> &icl_tree );


class Test_bench
{
	public:
		//! @brief test filling and flushing a tree
		bool test_fill_flush( void )
		{
			DENTER();
			bool x_ret = false;
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST - fill and flush\n";
			std::cout << "---------------------------------------------------------\n";
			//	FILL
			x_ret = x_ret | this->fill_tree();
			x_ret = x_ret | this->gcl_tree.show();
			x_ret = x_ret | this->gcl_tree.show(0);
			std::cout << "---------------------------------------------------------\n";
			//	FLUSH
			x_ret = x_ret | this->gcl_tree.flush();
			x_ret = x_ret | this->gcl_tree.show();
			x_ret = x_ret | this->gcl_tree.show(0);
			DRETURN();
			return x_ret;
		}

		//! @brief test special iterator that explores a tree
		bool test_tree_iterator( void )
		{
			DENTER();
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST tree iterator - deep exploration\n";
			std::cout << "---------------------------------------------------------\n";
			//FILL
			bool x_fail = this->fill_tree();
			x_fail = x_fail | this->gcl_tree.show();
			std::cout << "---------------------------------------------------------\n";
			//Scan the tree class using the iterator
			for (User::Tree<int>::iterator<User::Tree<int>::Node> cl_custom_iterator=this->gcl_tree.begin();cl_custom_iterator!=this->gcl_tree.end();cl_custom_iterator++)
			{
				std::cout << "Iterator Address: " << &(*cl_custom_iterator) << " | Node: " << (*cl_custom_iterator) << "\n";
			}
			DRETURN();
			return x_fail;
		}

		bool test_tree_swap_payload( void )
		{
			const size_t cn_num_swap_patterns = 3;
			size_t an_swap_pattern[cn_num_swap_patterns][2] =
			{
				{0,0},
				{1,2},
				{2,1},
			};
			DENTER();
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST - swap payload | patterns: " << cn_num_swap_patterns << "\n";
			std::cout << "---------------------------------------------------------\n";
			//FILL
			bool x_fail;
			x_fail = x_fail | this->gcl_tree.flush();
			x_fail = this->fill_tree();
			x_fail = x_fail | this->gcl_tree.show(0);
			std::cout << "---------------------------------------------------------\n";
			//Scan swap patterns
			for (size_t n_swap_pattern_index = 0; n_swap_pattern_index < cn_num_swap_patterns;n_swap_pattern_index++)
			{
				//Execute the swap with the given pattern
				x_fail = this->gcl_tree.swap
				(
					an_swap_pattern[n_swap_pattern_index][0],
					an_swap_pattern[n_swap_pattern_index][1],
					User::Tree<int>::Swap_mode::PAYLOAD
				);
				std::cout << "Swap " << an_swap_pattern[n_swap_pattern_index][0] << " <-> " << an_swap_pattern[n_swap_pattern_index][1] << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
				x_fail = x_fail | this->gcl_tree.show(0);
				std::cout << "---------------------------------------------------------\n";
			}
			DRETURN();
			return false;
		}

	private:
		//! @brief fill the tree with a pattern
		bool fill_tree( void )
		{
			//Construct a tree assigning the default payload and the root payload
			this->gcl_tree[0] = 404;
			this->gcl_tree.set_default_payload( 117 );
			//Attach two children to the root
			this->gcl_tree.create_child( 100 );
			this->gcl_tree.create_child( 101 );
			//Create a child of node 1 (ROOT) uses the generig create child method
			this->gcl_tree.create_child( 1, 100 );
			//Create a deep ladder of nodes
			size_t n_child_index = 0;
			n_child_index = this->gcl_tree.create_child( n_child_index, 333 +n_child_index );
			n_child_index = this->gcl_tree.create_child( n_child_index, 333 +n_child_index );
			n_child_index = this->gcl_tree.create_child( n_child_index, 333 +n_child_index );
			n_child_index = this->gcl_tree.create_child( n_child_index, 333 +n_child_index );
			//Create a child of node 1 (ROOT) uses the generig create child method
			this->gcl_tree.create_child( 1, 102 );

			return false;
		}

		//Tree structure under test
		User::Tree<int> gcl_tree;
};

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
    //bool u1_ret;
    //Temp index
    //unsigned int u32_index = 0;

    //----------------------------------------------------------------
    //	BODY
    //----------------------------------------------------------------
    //! @details algorithm:

	//Unit tests
    Test_bench cl_test_bench;
    cl_test_bench.test_fill_flush();
    cl_test_bench.test_tree_iterator();
    cl_test_bench.test_tree_swap_payload();

    if (false)
    {
        //Construct a tree assigning the default payload and the root payload
        User::Tree<int> cl_my_tree( 404, 117 );
        //Special method to return the root
        std::cout << "Root Payload: " << cl_my_tree.root() << "\n";
        //Generic method to return the nth node.
        std::cout << "Root Payload: " << cl_my_tree[0] << "\n";
        std::cout << "Root Payload: " << cl_my_tree[99] << "\n";

        //Attach two children to the root
        cl_my_tree.create_child( 100 );
        cl_my_tree.create_child( 101 );
        //Show content of the array
        cl_my_tree.show();
        //Create a child of node 1 (ROOT) uses the generig create child method
        cl_my_tree.create_child( 1, 102 );
        cl_my_tree.show();

        std::cout << "-------------------------------------\n";

        //Create a deep ladder of nodes
        size_t n_child_index = 0;
        n_child_index = cl_my_tree.create_child( n_child_index, 333+n_child_index );
        n_child_index = cl_my_tree.create_child( n_child_index, 333+n_child_index );
        n_child_index = cl_my_tree.create_child( n_child_index, 333+n_child_index );
        n_child_index = cl_my_tree.create_child( n_child_index, 333+n_child_index );
        //Of that node, create a child
        cl_my_tree.show( 0 );

		std::cout << "-------------------------------------\n";
        std::cout << "Test swap - payload - swap payload (cheap)\n";



		test_swap_priority( cl_my_tree );

		test_swap_independent_subtree( cl_my_tree );
    }

    /*
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
    std::cout << "Index: " << u32_index << "\n";
    if (u1_ret == false)
    {
        cl_my_instance[u32_index].create_leaf( 37 );
        cl_my_instance[u32_index].create_leaf( 38 );
        //Show off the syntax of this tree! I access leaf 1 with bracket bracket operator!
        cl_my_instance[u32_index][1].create_leaf( 338 );
        cl_my_instance[u32_index].create_leaf( 39 );
    }
    else
    {
        std::cout << "could not attach create leaf... | index: " << u32_index << "\n";
    }
    cl_my_instance.print();

    std::cout << "----------------------------------------\n";
    std::cout << "Swap two leaves\n";
    u1_ret = cl_my_instance.swap_leaves( 1, cl_my_instance[0], 0);
    cl_my_instance.print();

    std::cout << "----------------------------------------\n";
    std::cout << "Swap a parent with its child\n";
    u1_ret = cl_my_instance.swap_leaves( 0, cl_my_instance[0], 0);
    cl_my_instance.print();
    */

    //----------------------------------------------------------------
    //	RETURN
    //----------------------------------------------------------------

    //Trace Return vith return value
    DRETURN();
    return false; //OK
}	//end function: Dummy | bool

/****************************************************************************
**	@brief test_swap_priority | User::Tree<int> &
****************************************************************************/
//! @param f bool
//! @return bool |
//! @details
//! dummy method to copy the code
/***************************************************************************/

bool test_swap_priority( User::Tree<int> &icl_tree )
{
    //Trace Enter with arguments
    DENTER_ARG("PRIORITY: %p", &(icl_tree));
    //----------------------------------------------------------------
    //	BODY
    //----------------------------------------------------------------
    //! @details algorithm:

    DPRINT("-------------------------------------------PRIORITY-------------------------------------------\n");
	std::cout << "-------------------------------------\n";
	std::cout << "Test swap - Priority - swap order in which children are resolved (cheap)\n";

	size_t n_index_a = 0;
	size_t  n_index_b = 1;
	bool x_fail = icl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::PRIORITY );
	std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";

	n_index_a = 1;
	n_index_b = 4;
	x_fail = icl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::PRIORITY );
	std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
	icl_tree.show();
	icl_tree.show(0);

	std::cout << "\nI ask for a vector with the children of a node, and I swap two of those children\n";
	size_t n_father_target_index = 0;
	//Show the list of children of target father
	auto an_children = icl_tree.get_children( n_father_target_index );
	std::cout << "Father " << n_father_target_index << " has " << an_children.size() << " children:\n";
	for (auto cl_iterator_children = an_children.begin(); cl_iterator_children != an_children.end();cl_iterator_children++)
	{
		std::cout << "Children " << cl_iterator_children -an_children.begin() << "of" << an_children.size() << " | " << icl_tree.to_string( *cl_iterator_children ) << "\n";
	}
	//Execute the swap
	n_index_a = an_children[0];
	n_index_b = an_children[1];
	x_fail = icl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::PRIORITY );
	std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
	icl_tree.show();
	icl_tree.show(0);

    //----------------------------------------------------------------
    //	RETURN
    //----------------------------------------------------------------

    //Trace Return vith return value
    DRETURN_ARG("out: %d\n\n", x_fail);
    return x_fail; //OK
}	//end function: Dummy | bool

/****************************************************************************
**	@brief test_swap_priority | User::Tree<int> &
****************************************************************************/
//! @param f bool
//! @return bool |
//! @details
//! Test bench for the subtree swap targeting children not part of the same bloodline
/***************************************************************************/

bool test_swap_independent_subtree( User::Tree<int> &icl_tree )
{
    //Trace Enter with arguments
    DENTER_ARG("SUBTREE UNRELATED: %p", &(icl_tree));
    //----------------------------------------------------------------
    //	INIT
    //----------------------------------------------------------------

    bool x_fail;
    size_t n_index_a, n_index_b;

    //----------------------------------------------------------------
    //	BODY
    //----------------------------------------------------------------
    //! @details algorithm:

    DPRINT("-------------------------------------------SUBTREE UNRELATED-------------------------------------------\n");
	std::cout << "-------------------------------------\n";
	std::cout << "Test swap - Subtree with children not part of the same bloodline - swap order in which children are resolved (cheap)\n";
	//Find children of Root
	std::cout << "\nI ask for a vector with the children of a node, and I swap two of those children\n";
	size_t n_father_target_index = 0;
	//Show the list of children of target father
	auto an_children = icl_tree.get_children( n_father_target_index );
	if (an_children.size() <= 0)
	{
		DRETURN_ARG("ERR%d: there needs to be some children for this test bench to demonstrate the swap...", __LINE__);
		return true;
	}
	std::cout << "Father " << n_father_target_index << " has " << an_children.size() << " children:\n";
	for (auto cl_iterator_children = an_children.begin(); cl_iterator_children != an_children.end();cl_iterator_children++)
	{
		std::cout << "Children " << cl_iterator_children -an_children.begin() << "of" << an_children.size() << " | " << icl_tree.to_string( *cl_iterator_children ) << "\n";
	}
	//Find a child of one of those children
	auto an_children_children = icl_tree.get_children( an_children[1] );
	std::cout << "Father.Children " << an_children[1] << " has " << an_children_children.size() << " children:\n";
	if (an_children_children.size() <= 0)
	{
		DRETURN_ARG("ERR%d: there needs to be some children for this test bench to demonstrate the swap...", __LINE__);
		return true;
	}
	for (auto cl_iterator_children = an_children_children.begin(); cl_iterator_children != an_children_children.end();cl_iterator_children++)
	{
		std::cout << "Children " << cl_iterator_children -an_children.begin() << "of" << an_children.size() << " | " << icl_tree.to_string( *cl_iterator_children ) << "\n";
	}

	//Execute the swap
	n_index_a = an_children[2];
	n_index_b = an_children_children[0];
	x_fail = icl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::SUBTREE );
	std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
	icl_tree.show();
	icl_tree.show(0);

    //----------------------------------------------------------------
    //	RETURN
    //----------------------------------------------------------------

    //Trace Return vith return value
    DRETURN_ARG("out: %d\n\n", x_fail);
    return x_fail; //OK
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

bool f( )
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

