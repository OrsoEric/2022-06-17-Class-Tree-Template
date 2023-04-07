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
			std::cout << "show nodes inside the node array\n";
			x_ret = x_ret | this->gcl_tree.show();
			std::cout << "show nodes indented and ordered according to iterator exploration\n";
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
			x_fail = x_fail | this->gcl_tree.show(0);
			std::cout << "---------------------------------------------------------\n";
			std::cout << "Test iterator print\n";
			//Scan the tree class using the iterator
			for (User::Tree<int>::iterator<User::Tree<int>::Node> cl_custom_iterator=this->gcl_tree.begin();cl_custom_iterator!=this->gcl_tree.end();cl_custom_iterator++)
			{
				std::cout << "Iterator Address: " << &(*cl_custom_iterator) << " | Node: " << (*cl_custom_iterator) << "\n";
			}
			std::cout << "---------------------------------------------------------\n";
			std::cout << "Test iterator print from non root\n";
			//Scan the tree class using the iterator
			for (User::Tree<int>::iterator<User::Tree<int>::Node> cl_custom_iterator=this->gcl_tree.begin(1);cl_custom_iterator!=this->gcl_tree.end();cl_custom_iterator++)
			{
				std::cout << "Iterator Address: " << &(*cl_custom_iterator) << " | Node: " << (*cl_custom_iterator) << "\n";
			}
			DRETURN();
			return x_fail;
		}
		//! @brief erease a node from the tree
		bool test_erease( void )
		{
			DENTER();
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST erease\n";
			std::cout << "---------------------------------------------------------\n";

			this->gcl_tree.flush();
			bool x_fail = this->fill_tree();
			x_fail = x_fail | this->gcl_tree.show();
			x_fail = x_fail | this->gcl_tree.show(0);

			std::cout << "---------------------------------------------------------\n";
			std::cout << "Test erease root (should fail)\n";

			const size_t cn_num_erease_patterns = 9;
			size_t an_erease_pattern[cn_num_erease_patterns] =
			{
				7,
				1,
				4,
				1,
				1,
				1,
				1,
				1,
				1,
			};

			//Scan swap patterns
			for (size_t n_erease_pattern_index = 0; n_erease_pattern_index < cn_num_erease_patterns;n_erease_pattern_index++)
			{
				x_fail = this->gcl_tree.erease( an_erease_pattern[n_erease_pattern_index], User::Tree<int>::Erease_mode::NODE );
				std::cout << "Erease node: " << an_erease_pattern[n_erease_pattern_index] << " | Result:" << (x_fail?"FAIL":"OK") << "\n";
				x_fail = x_fail | this->gcl_tree.show();
				std::cout << "Explore tree\n";
				x_fail = x_fail | this->gcl_tree.show(0);
				std::cout << "---------------------------------------------------------\n";
			}

			DRETURN();
			return false;
		}


		//! @brief test expensive method that returns true if two nodes are descendents, needed to understand when swaps can be done safely
		bool test_is_descendent( void )
		{
			DENTER();
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST - is_descendent\n";
			std::cout << "---------------------------------------------------------\n";

			//FILL
			bool x_fail = this->fill_tree();
			x_fail = x_fail | this->gcl_tree.show();
			x_fail = x_fail | this->gcl_tree.show(0);

			const size_t cn_num_swap_patterns = 10;
			size_t an_swap_pattern[cn_num_swap_patterns][2] =
			{
				{0,0},
				{0,1},
				{0,2},
				{1,3},
				{1,4},
				{4,1},
				{5,4},
				{5,2},
				{2,8},
				{8,2},
			};
			//Scan swap patterns
			for (size_t n_swap_pattern_index = 0; n_swap_pattern_index < cn_num_swap_patterns;n_swap_pattern_index++)
			{
				//Execute the swap with the given pattern
				bool x_is_descendent = this->gcl_tree.is_descendant
				(
					an_swap_pattern[n_swap_pattern_index][0],
					an_swap_pattern[n_swap_pattern_index][1]
				);
				std::cout << "is_descendent " << an_swap_pattern[n_swap_pattern_index][0] << " <-> " << an_swap_pattern[n_swap_pattern_index][1] << " | Result: " << (x_is_descendent?"DESCENDENT":"UNRELATED") << "\n";

				std::cout << "---------------------------------------------------------\n";
			}

			DRETURN();
			return false;
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
				x_fail = x_fail | this->gcl_tree.show();
				x_fail = x_fail | this->gcl_tree.show(0);
				std::cout << "---------------------------------------------------------\n";
			}
			DRETURN();
			return false;
		}

		/****************************************************************************
		**	@brief test_swap_priority | void
		****************************************************************************/
		//! @return bool |
		//! @details
		//! swap order in which children are resolved (cheap)
		//! PRIORITY fails when trying to swap amongst non-siblings
		//! PRIORITY will change the order in which children are resolved
		/***************************************************************************/

		bool test_swap_priority( void )
		{
			//Trace Enter with arguments
			DENTER_ARG("PRIORITY: %p", &(gcl_tree));
			//----------------------------------------------------------------
			//	BODY
			//----------------------------------------------------------------
			//! @details algorithm:

			bool x_fail;

			std::cout << "-------------------------------------------PRIORITY-------------------------------------------\n";
			std::cout << "Test swap - Priority - swap order in which children are resolved (cheap)\n";

			//----------------------------------------------------------------
			//	FLUSH/FILL
			//----------------------------------------------------------------

			//Flush and refill the tree with a test tree
			this->gcl_tree.flush();
			this->fill_tree();

			//----------------------------------------------------------------
			//	SWAP PATTERNS
			//----------------------------------------------------------------

            //Swap patterns to be tested
			const size_t cn_num_swap_patterns = 4;
			size_t an_swap_pattern[cn_num_swap_patterns][2] =
			{
				{0,1},
				{1,0},
				{1,4},
				{4,1}
			};
			std::cout << "---------------------------------------------------------\n";
			std::cout << "test swap patterns.\n";
			std::cout << "PRIORITY fails when trying to swap amongst non-siblings\n";
			std::cout << "PRIORITY will change the order in which children are resolved\n";
			//Scan swap patterns
			for (size_t n_swap_pattern_index = 0; n_swap_pattern_index < cn_num_swap_patterns;n_swap_pattern_index++)
			{
				//Execute the swap with the given pattern
				x_fail = this->gcl_tree.swap
				(
					an_swap_pattern[n_swap_pattern_index][0],
					an_swap_pattern[n_swap_pattern_index][1],
					User::Tree<int>::Swap_mode::PRIORITY
				);
				std::cout << "Swap " << an_swap_pattern[n_swap_pattern_index][0] << " <-> " << an_swap_pattern[n_swap_pattern_index][1] << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
				x_fail = x_fail | this->gcl_tree.show();
				x_fail = x_fail | this->gcl_tree.show(0);
				std::cout << "---------------------------------------------------------\n";
			}

			//----------------------------------------------------------------
			//	ASK FOR LIST OF CHILDREN AND SWAP PRIORITY
			//----------------------------------------------------------------

			std::cout << "\nI ask for a vector with the children of a node, and I swap two of those children\n";
			size_t n_father_target_index = 0;
			//Show the list of children of target father
			auto an_children = gcl_tree.get_children( n_father_target_index );
			std::cout << "Father " << n_father_target_index << " has " << an_children.size() << " children:\n";
			for (auto cl_iterator_children = an_children.begin(); cl_iterator_children != an_children.end();cl_iterator_children++)
			{
				std::cout << "Children " << cl_iterator_children -an_children.begin() << " of " << an_children.size() << " | " << gcl_tree.to_string( *cl_iterator_children ) << "\n";
			}
			//Execute the swap
			size_t n_index_a = an_children[0];
			size_t n_index_b = an_children[1];
			x_fail = gcl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::PRIORITY );
			std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
			this->gcl_tree.show();
			this->gcl_tree.show(0);

			//----------------------------------------------------------------
			//	RETURN
			//----------------------------------------------------------------

			//Trace Return vith return value
			DRETURN_ARG("out: %d\n\n", x_fail);
			return x_fail; //OK
		}	//end function: test_swap_priority | bool

		/****************************************************************************
		**	@brief test_swap_independent_subtree | User::Tree<int> &
		****************************************************************************/
		//! @param f bool
		//! @return bool |
		//! @details
		//! Test bench for the subtree swap targeting children not part of the same bloodline
		/***************************************************************************/

		bool test_swap_independent_subtree( void )
		{
			//Trace Enter with arguments
			DENTER_ARG("SUBTREE UNRELATED: %p", &(gcl_tree));
			//----------------------------------------------------------------
			//	FLUSH/FILL
			//----------------------------------------------------------------
			bool x_fail;
			size_t n_index_a, n_index_b;

			std::cout << "-------------------------------------------SUBTREE UNRELATED-------------------------------------------\n";
			std::cout << "Test swap - Subtree with children not part of the same bloodline\n";
			//Flush and refill the tree with a test tree
			this->gcl_tree.flush();
			this->fill_tree();
			this->gcl_tree.show();
			this->gcl_tree.show(0);

			//----------------------------------------------------------------
			//	BODY
			//----------------------------------------------------------------

			//Find children of Root
			std::cout << "\nI ask for a vector with the children of a node, and I swap two of those children\n";
			size_t n_father_target_index = 0;
			//Show the list of children of target father
			auto an_children = gcl_tree.get_children( n_father_target_index );
			if (an_children.size() <= 0)
			{
				DRETURN_ARG("ERR%d: there needs to be some children for this test bench to demonstrate the swap...", __LINE__);
				return true;
			}
			std::cout << "Father " << n_father_target_index << " has " << an_children.size() << " children:\n";
			for (auto cl_iterator_children = an_children.begin(); cl_iterator_children != an_children.end();cl_iterator_children++)
			{
				std::cout << "Children " << (cl_iterator_children -an_children.begin()) << " of " << an_children.size() << " | " << gcl_tree.to_string( *cl_iterator_children ) << "\n";
			}

			//Find a child of one of those children
			auto an_children_children = gcl_tree.get_children( an_children[2] );
			std::cout << "Father.Children " << an_children[2] << " has " << an_children_children.size() << " children:\n";
			if (an_children_children.size() <= 0)
			{
				DRETURN_ARG("ERR%d: there needs to be some children for this test bench to demonstrate the swap...", __LINE__);
				return true;
			}
			for (auto cl_iterator_children = an_children_children.begin(); cl_iterator_children != an_children_children.end();cl_iterator_children++)
			{
				std::cout << "Children " << (cl_iterator_children -an_children_children.begin()) << " of " << an_children_children.size() << " | " << gcl_tree.to_string( *cl_iterator_children ) << "\n";
			}

			//Execute the swap
			n_index_a = an_children[0];
			n_index_b = an_children_children[0];
			x_fail = gcl_tree.swap( n_index_a, n_index_b, User::Tree<int>::Swap_mode::SUBTREE );
			std::cout << "Swap " << n_index_a << " <-> " << n_index_b << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
			gcl_tree.show();
			gcl_tree.show(0);

			//----------------------------------------------------------------
			//	RETURN
			//----------------------------------------------------------------

			//Trace Return vith return value
			DRETURN_ARG("out: %d\n\n", x_fail);
			return x_fail; //OK
		}	//end public method: test_swap_independent_subtree | bool

		/****************************************************************************
		**	@brief test_move | User::Tree<int> &
		****************************************************************************/
		//! @param f bool
		//! @return bool |
		//! @details
		//! Test bench for the move
		/***************************************************************************/

		bool test_move( void )
		{
			const size_t cn_num_swap_patterns = 5;
			size_t an_swap_pattern[cn_num_swap_patterns][2] =
			{
				//target, new father
				{0, 0},
				{7, 0},
				{7, 6},
				{6, 4},
				{1, 0},	//1 is already under father 0
			};
			DENTER();
			std::cout << "---------------------------------------------------------\n";
			std::cout << "TEST - MOVE | patterns: " << cn_num_swap_patterns << "\n";
			std::cout << "---------------------------------------------------------\n";
			//FILL
			bool x_fail;
			x_fail = x_fail | this->gcl_tree.flush();
			x_fail = this->fill_tree();
			std::cout << "Explore tree:\n";
			x_fail = x_fail | this->gcl_tree.show(0);
			std::cout << "---------------------------------------------------------\n";
			//Scan swap patterns
			for (size_t n_swap_pattern_index = 0; n_swap_pattern_index < cn_num_swap_patterns;n_swap_pattern_index++)
			{
				//Execute the swap with the given pattern
				x_fail = this->gcl_tree.move
				(
					an_swap_pattern[n_swap_pattern_index][0],
					an_swap_pattern[n_swap_pattern_index][1],
					User::Tree<int>::Move_mode::NODE
				);
				std::cout << "Move " << an_swap_pattern[n_swap_pattern_index][0] << " <-> " << an_swap_pattern[n_swap_pattern_index][1] << " | Result: " << (x_fail?"FAIL":"OK") << "\n";
				x_fail = x_fail | this->gcl_tree.show();
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

			//Construct a custom decorator to print out the payload, since payload is a template and can even be a complex structure
			std::string (*f_my_decorator)(int) = [](int in_payload){ return std::to_string( in_payload ); };
			this->gcl_tree.link_decorator( f_my_decorator );

			return false;
		}

		//Tree structure under test
		User::Tree<int> gcl_tree;
};

//! @brief use a tree to represent an equation
class Test_bench_equation
{
	public:
		Test_bench_equation()
		{
			this->fill_tree();
		}

		bool show( void )
		{
			//Construct a custom decorator to print out the payload, since payload is a template and can even be a complex structure
			std::string (*f_my_decorator)(std::string) = [](std::string is_payload){ return is_payload; };
			//Link the provided decorator to replace the default decorator
			this->gcl_tree_equation.link_decorator( f_my_decorator );
			std::cout << "Show list of nodes\n";
			this->gcl_tree_equation.show();
			std::cout << "Show exploration of tree\n";
			this->gcl_tree_equation.show(0);

			for (auto cl_iterator = this->gcl_tree_equation.begin();cl_iterator != this->gcl_tree_equation.end();cl_iterator++)
			{
				std::cout << (*cl_iterator).t_payload << " | ";
			}
			std::cout << "\n";
			return false;
		}

	private:
		bool fill_tree( void )
		{
			//The root is the equal sign
			this->gcl_tree_equation.root() = std::string("=");
			//on the LHS i have a MUL
			size_t n_index = this->gcl_tree_equation.create_child( 0, "*" );
			this->gcl_tree_equation.create_child( n_index, "3" );
			this->gcl_tree_equation.create_child( n_index, "2" );
			//On the RHS i have a SUB
			n_index = this->gcl_tree_equation.create_child( "-" );
			this->gcl_tree_equation.create_child( n_index, "7" );
			this->gcl_tree_equation.create_child( n_index, "1" );
			return false;
		}


		//I represent tokens as strings
		User::Tree<std::string> gcl_tree_equation;
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
    //	BODY
    //----------------------------------------------------------------
    //! @details algorithm:

    if (true)
    {
		//Unit tests
		Test_bench cl_test_bench;
		cl_test_bench.test_fill_flush();
		//cl_test_bench.test_tree_iterator();

		//cl_test_bench.test_is_descendent();

		//cl_test_bench.test_move();

		//
		cl_test_bench.test_erease();



		//cl_test_bench.test_tree_swap_payload();
		//cl_test_bench.test_swap_priority();
		//cl_test_bench.test_swap_independent_subtree();
	}

	if (false)
	{
		Test_bench_equation cl_test_equation;
		cl_test_equation.show();
	}

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

