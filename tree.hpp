/**********************************************************************************
**  ENVIROMENT VARIABILE
**********************************************************************************/

#ifndef TREE_H_
    #define TREE_H_

/**********************************************************************************
**  GLOBAL INCLUDES
**********************************************************************************/

//Used to store leaves
#include <vector>

#include "Tree_interface.h"

/**********************************************************************************
**  DEFINES
**********************************************************************************/

/**********************************************************************************
**  MACROS
**********************************************************************************/

/**********************************************************************************
**  NAMESPACE
**********************************************************************************/

//! @namespace custom namespace
namespace User
{

/**********************************************************************************
**  TYPEDEFS
**********************************************************************************/

/**********************************************************************************
**  PROTOTYPE: STRUCTURES
**********************************************************************************/

/**********************************************************************************
**  PROTOTYPE: GLOBAL VARIABILES
**********************************************************************************/

/**********************************************************************************
**  PROTOTYPE: CLASS
**********************************************************************************/

/************************************************************************************/
//! @class      Tree
/************************************************************************************/
//!	@author     Orso Eric
//! @version    2022-06-17
//! @brief      Generic tree template implementation
//! @copyright  BSD 3-Clause License Copyright (c) 2022, Orso Eric
//! @bug Avoid push_back | add a move constructor C(C&& c) C(const &c) | use emplace-back | construct/destruct https://stackoverflow.com/questions/21798396/destructor-is-called-when-i-push-back-to-the-vector
//! @bug swap can be done with std::swap std::vector::swap, the second is the fastest for vectors | https://stackoverflow.com/questions/41090557/c-swap-two-elements-of-two-different-vectors
//! @details
//! \n	A basic tree class that stores a template payload
//! \n	Implements the generic Tree_interface
//! \n	I want to be able to:
//! \n	Search a node by payload and return a callable version of it so I can add move nodes
/************************************************************************************/

template <class Payload>
class Tree : public Tree_interface<Payload>
{
    //Visible to all
    public:
		/*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC ENUMS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief Configurations of the class
        union Config
        {
            //Checks to be performed on user dependent data
			static const bool CU1_EXTERNAL_CHECKS = true;
			//Checks to be performed on input dependent on internal algorithms
			static const bool CU1_INTERNAL_CHECKS = true;
			//true = pedantic count children. check all nodes, and make sure all nodes have coherent priority
			//false = fast count children. will return the max priority of the first children with the correct father, if any
			static const bool CU1_PEDANTIC_COUNT_CHILDREN = true;
        };

        //! @brief Error codes of the class
        union Error_code
        {
			static constexpr const char *CPS8_OK = "OK";
			static constexpr const char *CPS8_ERR = "ERR";
			static constexpr const char *CPS8_ERR_OOB = "ERR Out Of Boundary: Tried to access an index that doesn't exist";
        };

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC TYPEDEFS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief Node of the tree
        struct Node
        {
			//Payload inside the node
			Payload t_payload;
			//Index of the father. using father means that there is no variable number of children index to maintain. all nodes have exactly one father except the root. It also makes it impossible to make loops. Root is the only node that has itself as father.
			unsigned int gu32_index_father;
			//Priority, defines the order of this node compared to its siblings, 0 is the highest priority node under the given father.
			unsigned int u32_own_priority;
			//Max Priority, it's the number of siblings. A lone child has priority of 0 and a max_priority of 1. max_priority of 0 is error.
			//Adding a children would require modifying the max priority of all children.
			//Instead the max priority of a node is equivalent to the number of children. I already have father reference to access it.
			//So it's single edit when adding/removing children. Would be multi edit on all children otherwise
			unsigned int u32_children_max_priority;
        };

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  CONSTRUCTORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Empty Constructor
        Tree( void );
        //Default payload Constructor
        Tree( Payload it_default_payload );
        //Default payload and root Constructor
        Tree( Payload it_default_payload, Payload it_root_payload );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  DESTRUCTORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Empty destructor
        ~Tree( void );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC OPERATORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//Overload the square bracket operator to do an index search and return a RRHS/LHS reference to the payload
        Payload& operator []( unsigned int igu32_index );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC SETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//Create a child of the root
		//std::vector<Node>::iterator create_child( Payload it_payload );
		bool create_child( Payload it_payload );
		//Create a child of the node with a given index. Returns the index of the node created
		unsigned int create_child( unsigned int iu32_father_index, Payload it_payload );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC GETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//Reference to Payload of the root node, overload available to discard success
        Payload &root( bool &oru1_fail );
        Payload &root( void )
        {
			bool u1_fail;
			return this->root( u1_fail );
        }
        //Show the nodes stored inside the vector and their links
		bool show( void );
		bool show( unsigned int igu32_index, unsigned int iu32_depth );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC TESTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC STATIC METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		/*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  ITERATORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		template <typename T>
		class custom_iterator
		{
			public:
				custom_iterator(std::vector<T>& vec, size_t index = 0) : gra_vector(vec), gu32_index(index) {}

				// Prefix ++ operator (increments the iterator and returns the new value)
				custom_iterator<T>& operator++()
				{
					++gu32_index;
					return *this;
				}

				// Postfix ++ operator (increments the iterator and returns the old value)
				custom_iterator<T> operator++(int)
				{
					custom_iterator<T> tmp(*this);
					++gu32_index;
					return tmp;
				}

				// Dereference operator (*) (returns a reference to the element at the current position)
				T& operator*(void)
				{
					return gra_vector[gu32_index];
				}

				// Comparison operators (compare the iterator's position with another iterator)
				bool operator==(const custom_iterator<T>& other) const
				{
					return gu32_index == other.gu32_index;
				}
				bool operator!=(const custom_iterator<T>& other) const
				{
					return gu32_index != other.gu32_index;
				}

			private:
				std::vector<T>& gra_vector;
				size_t gu32_index;
			};

			// Iterator methods
			custom_iterator<Node> begin()
			{
				return custom_iterator<Node>(gast_nodes, 0);
			}
			custom_iterator<Node> end()
			{
				return custom_iterator<Node>(gast_nodes, gast_nodes.size());
			}

    //Visible to derived classes
    protected:
        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PROTECTED TYPES
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PROTECTED VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

    //Visible only inside the class
    private:
        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE TYPES
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE INIT
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief initialize class vars to the default
        bool init_class_vars( Payload it_payload );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Count the children of a node
        bool count_children( typename std::vector<Node>::iterator st_father, unsigned int &oru32_num_children );
		//Report an error. return false: OK | true: Unknown error code
        bool report_error( const char *ips8_error_code );
        //Tries to recover from an error. Automatically called by get_error. return false = OK | true = fail
        bool error_recovery( void );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//The nodes are stored inside a standard vector
        std::vector<Node> gast_nodes;
        //Iterator of the root
        //typename std::vector<Node>::iterator gpst_root;
        //Stores the node that is target for tree operations
        //typename std::vector<Node>::iterator pst_working_node;
};	//End Class: Tree

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	CONSTRUCTORS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Constructor: Tree | void
/***************************************************************************/
//! @return no return
//! @details
//! \n Empty constructor
/***************************************************************************/

template <class Payload>
Tree<Payload>::Tree( void )
{
    DENTER_ARG("This: %p", this);   //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Initialize class vars
    this->init_class_vars( this->gt_dummy );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();  //Trace Return
    return;
}   //Constructor: Tree | void

/***************************************************************************/
//! @brief Constructor: Tree | Payload |
/***************************************************************************/
//! @param it_default_payload | default payload
//! @return no return
//! @details
//! \n Empty constructor
/***************************************************************************/

template <class Payload>
Tree<Payload>::Tree( Payload it_default_payload )
{
    DENTER_ARG("This: %p", this);   //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    this->set_default_payload( it_default_payload );
    //Initialize class vars
    this->init_class_vars( it_default_payload );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();  //Trace Return
    return;
}   //Constructor: Tree | Payload |

/***************************************************************************/
//! @brief Constructor: Tree | Payload | Payload |
/***************************************************************************/
//! @param it_default_payload | default payload
//! @return no return
//! @details
//! \n Empty constructor
/***************************************************************************/

template <class Payload>
Tree<Payload>::Tree( Payload it_default_payload, Payload it_root_payload )
{
    DENTER_ARG("This: %p", this);   //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    this->set_default_payload( it_default_payload );
    //Initialize class vars
    this->init_class_vars( it_root_payload );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();  //Trace Return
    return;
}   //Constructor: Tree | Payload | Payload |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	DESTRUCTORS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//!	@brief Destructor: Tree | void
/***************************************************************************/
// @param
//! @return no return
//! @details
//! \n Empty destructor
/***************************************************************************/

template <class Payload>
Tree<Payload>::~Tree( void )
{
    DENTER_ARG("This: %p", this);   //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------



    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();      //Trace Return
    return;         //OK
}   //Destructor: Tree | void

/***************************************************************************/
//!	@brief operator [] | int
/***************************************************************************/
//!	@param igu32_index | Numeric index of the node
//! @return Payload & | Reference to content of the node
//!	@details
//! LHS and RHS access to the payload of a node. Returns the reference to the
//!	dummy payload if an invalid index is given, raising an error inside the tree
/***************************************************************************/

template <class Payload>
Payload& Tree<Payload>::operator []( unsigned int igu32_index )
{
	//Trace Enter
	DENTER_ARG("Object: %p, Index: %d", (void *)this, igu32_index );
	//--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

	//The user is trying to access a node outside the range
    if (igu32_index >= this->gast_nodes.size())
    {
		DRETURN_ARG("Index OOB: %d of %d", igu32_index, this->gast_nodes.size() );
		//Return a reference to the dummy payload
		return this->gt_dummy;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
	//Trace Return from main
	DRETURN();
	return this->gast_nodes[igu32_index].t_payload;
}	//end method: operator & | int |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC SETTERS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public Setter: create_leaf | Payload
/***************************************************************************/
//! @param it_payload | payload to be attached to this leaf
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Create a child of the root
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::create_child( Payload it_payload )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK&INIT
    //--------------------------------------------------------------------------
	//if class is in error, leaf cannot be created
    if (this->gps8_error_code != Error_code::CPS8_OK)
    {
		DRETURN_ARG("ERR:%d Tree is in error: %s | Cannot create leaf", __LINE__, this->gps8_error_code );
		return true;
    }

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------
	//Create the node to add metadata to the payload
	Node st_node;
	st_node.t_payload = it_payload;
	//This node starts with no children
	st_node.u32_children_max_priority = 0;
	//Index of the root
	st_node.gu32_index_father = 0;
	//Number of children of the father, number of siblings of the node being created. Also the priority, resolution order of those children.
	unsigned int u32_num_children = this->gast_nodes[ st_node.gu32_index_father ].u32_children_max_priority;
    //By the end, I will have added a children to the father
	u32_num_children++;
	//I update the number of children of the father, thus the max priority of the father
	this->gast_nodes[ st_node.gu32_index_father ].u32_children_max_priority = u32_num_children;
    //The newly created node has the lowest priority
    st_node.u32_own_priority = u32_num_children -1;
	//Add the node to the tree
	this->gast_nodes.push_back( st_node );



    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Father Index: %d | Own Index: %d | Nodes under Father: %d", st_node.gu32_index_father, this->gast_nodes.size() -1,u32_num_children +1 ); //Trace Return
    return false;	//OK
}   //Public Setter: create_leaf | Payload

/***************************************************************************/
//! @brief Public Setter: create_leaf | Payload
/***************************************************************************/
//! @param iu32_father_index | Numeric index of the node that is to be the father of the new node. The father may have nodes already
//! @param it_payload | payload to be attached to this leaf
//! @return unsigned int | 0 = FAIL. 0 is the index of the root, it can never be created by create_child | >0 index of the node in the array
//! @details
//! \n Create a child of the root
/***************************************************************************/

template <class Payload>
unsigned int Tree<Payload>::create_child( unsigned int iu32_father_index, Payload it_payload )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK&INIT
    //--------------------------------------------------------------------------
	//if class is in error, leaf cannot be created
    if (this->gps8_error_code != Error_code::CPS8_OK)
    {
		DRETURN_ARG("ERR:%d | Tree is in error: %s | Cannot create leaf", __LINE__, this->gps8_error_code );
		return true;
    }
	//If I'm searching for a node OOB
    if (iu32_father_index >= this->gast_nodes.size())
    {
		DRETURN_ARG("ERR:%d | OOB %d of %d | %s", __LINE__, iu32_father_index, this->gast_nodes.size(),this->gps8_error_code );
		this->report_error( Error_code::CPS8_ERR_OOB );
		return true;
    }

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

	//Create the node to add metadata to the payload
	Node st_node;
	st_node.t_payload = it_payload;
	//This node starts with no children
	st_node.u32_children_max_priority = 0;
	//Index of the root
	st_node.gu32_index_father = iu32_father_index;
	//Number of children of the father
	//Also number of siblings of the node being created.
	//Also the priority, resolution order of those children.
	unsigned int u32_num_children = this->gast_nodes[ st_node.gu32_index_father ].u32_children_max_priority;
    //By the end, I will have added a children to the father
	u32_num_children++;
	//I update the number of children of the father, thus the max priority of the father
	this->gast_nodes[ st_node.gu32_index_father ].u32_children_max_priority = u32_num_children;
    //The newly created node has the lowest priority
    st_node.u32_own_priority = u32_num_children -1;
	//This is what the index of this node should be after the creation operation is complete
    unsigned int u32_own_index = this->gast_nodes.size();
	//Add the node to the tree
	this->gast_nodes.push_back( st_node );
	//!@todo check that the node has been created with the right content
	if (u32_own_index != this->gast_nodes.size() -1)
	{
		this->report_error( Error_code::CPS8_ERR );
		DRETURN_ARG("ERR%d: push_back seemingly did not create a new node | %d of %d", __LINE__, u32_own_index, this->gast_nodes.size() );
		return true;
	}

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Father Index: %d | Own Index: %d | Nodes under Father: %d", st_node.gu32_index_father, u32_own_index,u32_num_children +1 ); //Trace Return
    return false;	//OK
}   //Public Setter: create_leaf | Payload

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC GETTER
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public getter: root | bool & |
/***************************************************************************/
//! @param oru1_fail | false = OK | true = FAIL
//! @return Payload & | Reference to the payload of the root node |
//! @details
//! \n Return the reference to the payload of the root node
/***************************************************************************/

template <class Payload>
Payload &Tree<Payload>::root( bool &oru1_fail )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    if (this->gast_nodes.size() <= 0)
    {
		this->report_error(Error_code::CPS8_ERR);
		return this->gt_dummy;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return this->gast_nodes[0].t_payload;
}   //Public getter: root | bool & |

/***************************************************************************/
//! @brief Public getter: show | void |
/***************************************************************************/
//! @param bool | false = OK | true = FAIL |
//! @details
//! \n Show the nodes stored inside the vector and their links
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::show( void )
{
    DENTER_ARG("Nodes: %d", this->gast_nodes.size()); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    if (this->gast_nodes.size() <= 0)
    {
		DRETURN_ARG("ERR:%d | Vector should contain at least the root...", __LINE__ );
		return true;
    }

    //--------------------------------------------------------------------------
    //	SHOW
    //--------------------------------------------------------------------------

	//Scan vector of nodes
    for (typename std::vector<Node>::iterator pst_node = this->gast_nodes.begin();pst_node < this->gast_nodes.end();pst_node++)
    {

		//std::ostream my_stream;
		unsigned int u32_node_index = pst_node- this->gast_nodes.begin();
		std::cout << "Index: " << u32_node_index << " | ";
		unsigned int u32_father_index = pst_node->gu32_index_father;
		std::cout << "Father: " << u32_father_index;
		std::cout << " | Payload: " << pst_node->t_payload;
		//Root is the only node that has itself as father
		if (u32_node_index == u32_father_index)
		{
			std::cout << " | ROOT ";
		}
		//
		std::cout << "\n";
    }


    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return this->gast_nodes[0].t_payload;
}   //Public getter: show | void |

/***************************************************************************/
//! @brief Public getter: show | void |
/***************************************************************************/
//! @param igu32_index | index of the node. Will traverse and print every node down from this node
//! @param iu32_depth | recursive depth counter, used to represent as tabs the depth of the node
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Show the nodes stored inside the vector and their links
//! \n Recursive tree travel function
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::show( unsigned int igu32_index, unsigned int iu32_depth )
{
    DENTER_ARG("Index: %d | Depth: %d", igu32_index, iu32_depth ); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    if (this->gast_nodes.size() <= 0)
    {
		DRETURN_ARG("ERR:%d | Vector should contain at least the root...", __LINE__ );
		return true;
    }

    //--------------------------------------------------------------------------
    //	SHOW
    //--------------------------------------------------------------------------

	//Print a spacer for each level of descent into the tree
    for (unsigned int u32_cnt = 0;u32_cnt < iu32_depth;u32_cnt++)
	{
		if (u32_cnt != iu32_depth-1 )
		{
			std::cout << "    ";
		}
		else
		{
			std::cout << "|-- ";
		}
	}
	//Print the content of the node
	std::cout << "Node: " << igu32_index << " | Priority " << this->gast_nodes[igu32_index].u32_own_priority << " / " << this->gast_nodes[ this->gast_nodes[igu32_index].gu32_index_father ].u32_children_max_priority << " | Payload: " << this->gast_nodes[igu32_index].t_payload << "\n";
	//Put in the stack the children
	//The node being worked on has (u32_children_max_priority) that tells the number of children
	//
	//If there is only the root inside the tree
	if (this->gast_nodes.size() <= 1)
	{
		//Stop the search early
        DRETURN();
        return false;
	}
	//unsigned int u32_father_index = igu32_index
	//Search index for the next children, skip the root from the search
	unsigned int u32_children_index = 1;
	//Number of children found
	unsigned int u32_num_children = 0;
	//Only activate recursive search under this node if this node has at least one child
	bool u1_search_children = (this->gast_nodes[igu32_index].u32_children_max_priority > 0);
	//while authorized to scan for children
    while (u1_search_children == true)
    {
		//If I find a node whose father is the index I just printed
		if (igu32_index == this->gast_nodes[u32_children_index].gu32_index_father)
		{
			//Pedantic check that the priority of the node is consistent with the number of children of the father
			if ( (Config::CU1_INTERNAL_CHECKS == true) && (this->gast_nodes[u32_children_index].u32_own_priority >= this->gast_nodes[igu32_index].u32_children_max_priority))
			{
				DRETURN_ARG("ERR%d: Found a child whose priority exceed the number of children of the father...", __LINE__ );
				return true;
			}
			//Launch the recursive search under this node as well
			//!@TODO: This code doesn't take into account the priority. I should explore high priority node first
			bool u1_fail = this->show( u32_children_index, iu32_depth +1);
			if (u1_fail == true)
			{
				DRETURN_ARG("ERR%d: Exploration failed at node %d, depth %d, end the search early...", __LINE__, u32_children_index, iu32_depth );
				return true;
			}
			//I just found a child
			u32_num_children++;
			//If I found ALL the children of this node
			if (u32_num_children >= this->gast_nodes[igu32_index].u32_children_max_priority)
			{
				u1_search_children = false;
			}
		}	//If I find a node whose father is the index I just printed
		//Scan next child
		u32_children_index++;
		//Scanned all nodes
		if (u32_children_index > this->gast_nodes.size())
		{
			DRETURN_ARG("ERR%d: Search for child reached the end of the array without finding one...", __LINE__ );
			return true;
		}
    }	//while authorized to scan for children

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Children: %d", u32_num_children); //Trace Return
    return false;
}   //Public getter: show | void |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC METHODS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/


/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PRIVATE INIT
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Private Method: init_class_vars | void
/***************************************************************************/
// @param
//! @return no return
//! @details
//! \n Initialize class vars
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::init_class_vars( Payload it_payload )
{
    DENTER();		//Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

	Node st_node;
    st_node.t_payload = it_payload;
    //Root starts with no children
	st_node.u32_children_max_priority = 0;
	//Root cannot have a priority
	st_node.u32_own_priority = 0;
    //Allocate root and fill root with dummy payload
    this->gast_nodes.push_back( st_node );
    if (this->gast_nodes.size() != 1)
    {
		//! @todo: Undo initialization
		this->report_error( Error_code::CPS8_ERR );
        DRETURN_ARG("ERR%d: There should be exactly one node (Root) after initialization, there are %d instead", __LINE__, this->gast_nodes.size() );
        return true;
    }
	//Register the father index. Root points to itself
    this->gast_nodes[0].gu32_index_father = 0;

    this->gps8_error_code = Error_code::CPS8_OK;

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();      //Trace Return
    return false;   //OK
}   //Private Method: init_class_vars | void

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PRIVATE METHODS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Private Method | report_error | Error_code
/***************************************************************************/
//! @param ips8_error_code | error string to be reported. Error strings can be stored in Error_code
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Report an error. return false: OK | true: Unknown error code
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::report_error( const char *ips8_error_code )
{
    DENTER_ARG("ERR: %p", ips8_error_code ); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //If class is OK
    if (ips8_error_code != Error_code::CPS8_OK)
    {
		//Report current error
		this->gps8_error_code = ips8_error_code;
    }
    else
    {
		//Class was already in error. Could not
		DRETURN_ARG("ERR: Class is already in error: %p | culd not report error: %p", &this->gps8_error_code, ips8_error_code );
		return true;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
} 	//Private Method: report_error | Error_code

/***************************************************************************/
//! @brief Private Method: error_recovery | void |
/***************************************************************************/
//! @return return false = OK | true = fail
//! @details
//! \n Tries to recover from an error
//! \n Automatically called by get_error.
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::error_recovery( void )
{
    DENTER_ARG("ERR: %s", this->gps8_error_code); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------
	//If class is not OK
    if (this->gps8_error_code != Error_code::CPS8_OK)
    {
		//Class is OK
		this->gps8_error_code = Error_code::CPS8_OK;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Recovered: %s", this->gps8_error_code);      //Trace Return
    return false;    //FAIL
}   //Private Method: error_recovery | void |

/**********************************************************************************
**  NAMESPACE
**********************************************************************************/

}	//end namespace: User

#else
    #warning "Multiple inclusion of hader file TREE_H_"
#endif
