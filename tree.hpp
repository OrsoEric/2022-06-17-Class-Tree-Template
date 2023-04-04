/**********************************************************************************
**  ENVIROMENT VARIABILE
**********************************************************************************/

#ifndef TREE_H_
    #define TREE_H_

/**********************************************************************************
**  GLOBAL INCLUDES
**********************************************************************************/

#include <string>
//Used to store nodes of the tree
#include <vector>
//Used to explore the tree with pseudorecursion
#include <stack>
//Used to sort the nodes of the tree based on priority
#include <algorithm>
//Used to get a lambda to stringfy a node
#include <functional>

//Interface of the class
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
//! @bug swap can be done with std::swap std::vector::swap, the second is the fastest for vectors | https://stackoverflow.com/questions/41090557/c-swap-two-elements-of-two-different-vectors
//! @todo swap between descendence, it's a complex and weird operation that maybe doesn't make sense
//! @todo delete node, two modes, delete subtree, or move up the subtree
//! @todo move node, if the node has children, children are reattached to the father
//! @details
//! \n	A basic tree class that stores a template payload
//! \n	Implements the generic Tree_interface
//! \n	I want to be able to:
//! \n	Search a node by payload and return a callable version of it so I can add move nodes
//! \n	IDEA: add a field dedicated to the scan. an index inside each node that is maintained when a node is created/destroyed
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
			//true = the iterator will automatically fix depth
            static const bool CU1_PEDANTIC_ITERATOR_FIX_DEPTH = true;
        };

        //! @brief Error codes of the class
        union Error_code
        {
            static constexpr const char *CPS8_OK = "OK";
            static constexpr const char *CPS8_ERR = "ERR";
            static constexpr const char *CPS8_ERR_OOB = "ERR Out Of Boundary: Tried to access an index that doesn't exist";
        };
        //! @brief how a node is to be deleted
        enum class Erease_mode
        {
			//Just delete the node, and relinks the children of the node, to the father of the node
			NODE,
			//Delete the node, and delete all the subtree under that node
			DEEP
        };
        //! @brief Moving nodes can be done in two different ways
		enum class Move_mode
		{
			//Move an individual node, and bump the children of the source upward
			NODE,
			//Move a node, and all its children
			SUBTREE
		};
        //! @brief Swapping nodes can be done in fundamentally different ways, see swap method documentation for details
        enum class Swap_mode
        {
            //Payload swap will swap the payload field of two nodes
            PAYLOAD,
            //Priority swap will swap the priority of two children of the same father, changing the order in which they are resolved
            PRIORITY,
            //>>>DEFAULT<<<
            //Subtree swap will swap two nodes along with all their subtrees. Safe means that swap of a father with a children will fail
            SUBTREE_SAFE,
            //Subtree swap will swap two nodes along with all their subtrees. The operation is extended to take care of what happens with the subtrees
            //it will change the father/children father/children hierarchy and have many ambiguous choice to make about priority
            SUBTREE
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
            //Index of this node. Needed when trying to extract an index from an iterator, a very common operation
            size_t n_own_index;
            //Index of the father. using father means that there is no variable number of children index to maintain. all nodes have exactly one father except the root. It also makes it impossible to make loops. Root is the only node that has itself as father.
            size_t n_index_father;
            //Priority, defines the order of this node compared to its siblings, 0 is the highest priority node under the given father. It checks against n_children_max_priority of the father of this node
            size_t n_own_priority;
            //Max Priority, it's the number of children of this node. It also serves as maximum priority of children
            size_t n_children_max_priority;
            //Distance from root of this node, computed by create_child
            size_t n_distance_from_root;
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

        //Overload the square bracket operator to do an index search and return a RHS/LHS reference to the payload
        Payload& operator []( size_t in_index );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC SETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//! @brief allows user to specify a decorator to print out Node as a std::string, useful especially for the payload
        bool link_decorator(std::function<std::string(Payload)> if_decorator )
        {
			this->gf_lambda_decorator = if_decorator;
			return false;
		}
        //Create a child of the root
        size_t create_child( Payload it_payload );
        //Create a child of the node with a given index. Returns the index of the node created
        size_t create_child( size_t in_father_index, Payload it_payload );
        //move a node somewhere else. Two move modes:
        //! @todo
        bool move( size_t in_index_node_source, size_t in_index_father_destination, Move_mode ie_move_mode = Move_mode::NODE );
        //Delete a node from the tree. Two deletion modes: NODE delete a single node and relinks the children, DEEP, delete node and all the children
        //! @todo
        bool erease( size_t in_index, Erease_mode );
        //Swap two nodes of the tree
        bool swap( size_t in_lhs, size_t in_rhs, Swap_mode ie_swap_mode = Swap_mode::SUBTREE );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC GETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Reference to Payload of the root node, overload available to discard success
        Payload &root( bool &orx_fail );
        Payload &root( void )
        {
            bool x_fail;
            return this->root( x_fail );
        }
        //Find the children of a node of a given index, and push their indexes inside a vector
        bool find_children( size_t in_father_index, std::vector<size_t> &ira_children_indexes );
        //Overloads used when a ector has to be created.
        std::vector<size_t> get_children( size_t in_father_index )
        {
			std::vector<size_t> an_children;
			find_children( in_father_index, an_children );
			return an_children;
        }
        //Show the nodes stored inside the vector and their links
        bool show( void );
        bool show( size_t in_index );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC TESTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief check if two nodes are direct relatives
        bool is_descendant( size_t in_lhs, size_t in_rhs );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//! @brief flush the tree except for the root, realign the root
		bool flush( void );
		//Overloads of string that is not static and accept a node index
        std::string to_string( size_t n_index )
        {
			return to_string( this->gast_nodes[ n_index ] );
        }

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  FRIEND METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief overload (extend) the output stream operator to cover the Node structure
        friend std::ostream& operator<<( std::ostream& icl_stream, Tree<Payload>::Node &ist_node )
        {
            //Dedicated static method to stringfy a node
            return icl_stream << Tree<Payload>::to_string( ist_node );
        }

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

        /**
        * @class iterator
        * @tparam T the type of elements in the vector
        *
        * @brief A custom iterator class for a vector of type T.
        * This class overloads the preincrement operator (operator++) and postincrement operator (operator++(int))
        * to allow for movement through the vector using the increment operator (++). It also define
        * Dereference operator (operator*) to access the element of current index of the vector.
        * It also define comparison operator (operator== and operator!=) to compare the position of the iterator
        * with another iterator.
        */
        template <typename T>
        class iterator
        {
            public:
				iterator( Tree<Payload>& ircl_parent_tree, bool ix_begin ) : grcl_tree(ircl_parent_tree)
				{
					this->init( ircl_parent_tree, ix_begin, 0 );
				}

				//! @brief construct a new iterator linked to the underlying tree. can either be a begin or an end iterator
                iterator( Tree<Payload>& ircl_parent_tree, bool ix_begin, size_t in_begin_index ) : grcl_tree(ircl_parent_tree)
                {
					this->init( ircl_parent_tree, ix_begin, in_begin_index );
                }
				//! @brief iterator post increment operator
                iterator<T>& operator++()
                {
                    //Advance to the next element of the tree
                    size_t n_ret = this->next();
                    //if next failed
                    if (n_ret >= this->grcl_tree.gast_nodes.size())
                    {
						//nothing to do, next has already stopped and iterator points to END
                    }
                    return *this;
                }
				//! @brief iterator pre increment operator
                iterator<T> operator++(int)
                {
                    iterator<T> tmp(*this);
                    //Advance to the next element of the tree
                    size_t n_ret = this->next();
                    //if next failed
                    if (n_ret >= this->grcl_tree.gast_nodes.size())
                    {
						//nothing to do, next has already stopped and iterator points to END
                    }
                    return tmp;
                }
				//! @brief iterator dereference operator
                T &operator*(void)
                {
					//Fetch index of current item, item on top of the stack
					size_t n_current_index = this->get_index();
					//valid index
					if (n_current_index < this->grcl_tree.gast_nodes.size())
					{
						return grcl_tree.gast_nodes[n_current_index];
					}
					//invalid index return the root
					else
					{
						return grcl_tree.gast_nodes[0];
					}
                    //DRETURN_ARG("Index: %d | Stack size: %d | Node: %s", n_current_index, this->gcl_pseudorecursive_stack.size(), Tree<Payload>::to_string( grcl_tree.gast_nodes[n_current_index] ).c_str() );
                }
				//! @brief iterator equal operator, meant to find the end
                bool operator==(const iterator<T>& icl_rhs_iterator) const
                {
                    //! @todo there might be need for additional checks e.g. on container sizes?
                    //equal if I counted the same number of items on both iterators
                    return (this->gn_cnt_nodes == icl_rhs_iterator.gn_cnt_nodes);
                }
				//! @brief iterator equal operator, meant to find the end
                bool operator!=(const iterator<T>& icl_rhs_iterator) const
                {
                    //unequal if I counted different number of items on both iterators
                    return (this->gn_cnt_nodes != icl_rhs_iterator.gn_cnt_nodes);
                }
                //! @brief index of the node, works by pulling the top of the stack
				size_t get_index( void )
				{
					 //If the stack is empty
                    if (this->gcl_pseudorecursive_stack.empty() == true)
                    {
                        //ERROR. Return invalid index
                        return this->grcl_tree.gast_nodes.size();
                    }
                    //Fetch the index that is at the top of the stack
                    return this->gcl_pseudorecursive_stack.top();
				}

            private:
				bool init( Tree<Payload>& ircl_parent_tree, bool ix_begin, size_t in_begin_index )
				{
					DENTER_ARG("Parent tree: %p | Begin: %d | Index: %d", &(*this), ix_begin, in_begin_index );
                    //Constructing a begin iterator
                    if (ix_begin == true)
                    {
                        //Push the root inside the stack
                        this->gcl_pseudorecursive_stack.push( in_begin_index );
                        //Start from first node
                        this->gn_cnt_nodes = 0;
                    }
                    //Constructing an end iterator
                    else //if (ix_begin == false)
                    {
                        //No need to initialize the stack of the end iterator
                        //I need to scan a number of nodes equal to the array size, so I just set the number of scanned nodes
                        this->gn_cnt_nodes = ircl_parent_tree.gast_nodes.size();
                    }
                    DRETURN_ARG("Count nodes: %d | Stack size: %d", this->gn_cnt_nodes, this->gcl_pseudorecursive_stack.size() );
                    return false;
				}
				//! @brief flush pseudorecursive stack
                bool flush_stack()
                {
                    //Flush the stack
                    while(this->gcl_pseudorecursive_stack.empty() == false)
                    {
                        this->gcl_pseudorecursive_stack.pop();
                    }
                    return false;
                }
                //! @brief advance the iterator to the next element
                size_t next()
                {
                    DENTER();
                    //If there are elements in the stack
                    if (this->gcl_pseudorecursive_stack.empty() == false)
                    {
                        //Take the top index out of the array
                        size_t n_current_index = this->gcl_pseudorecursive_stack.top();
                        this->gcl_pseudorecursive_stack.pop();
                        if ((Config::CU1_INTERNAL_CHECKS) && (n_current_index >= this->grcl_tree.gast_nodes.size()))
                        {
                            DRETURN_ARG("ERR%d: Wrong index popped from stack. This should NEVER happen. Popped index: %d of %d", __LINE__, n_current_index, this->grcl_tree.gast_nodes.size());
                            return true;
                        }
                        //I find all the children of the node I just popped, and push them
                        std::vector<size_t> an_children_indexes;
                        bool x_fail = this->grcl_tree.find_children( n_current_index, an_children_indexes );
                        if (x_fail == true)
                        {
                            DRETURN_ARG("ERR%d: find children failed", __LINE__);
                            return true;
                        }
                        //for all children of the node that i popped
                        for (auto cl_children_iterator = an_children_indexes.rbegin();cl_children_iterator != an_children_indexes.rend();cl_children_iterator++)
                        {
                            //Push the index of the child of the popped item in the pseudorecursive stack
                            this->gcl_pseudorecursive_stack.push( *cl_children_iterator );
                        }
                        //fix the depth
                        if (Config::CU1_PEDANTIC_ITERATOR_FIX_DEPTH == true)
                        {
							if (n_current_index == 0)
							{
								if (this->grcl_tree.gast_nodes[n_current_index].n_index_father != 0 )
								{
									DPRINT("ERR%d: n_index_father of root is wrong %d ... FIXED\n", __LINE__, this->grcl_tree.gast_nodes[n_current_index].n_index_father );
									this->grcl_tree.gast_nodes[n_current_index].n_index_father = 0;
								}
								if (this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root != 0)
								{
									DPRINT("ERR%d: n_depth of root is wrong %d ... FIXED\n", __LINE__, this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root );
									this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root = 0;
								}
							}
							else
							{
								if (this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root != (this->grcl_tree.gast_nodes[this->grcl_tree.gast_nodes[n_current_index].n_index_father].n_distance_from_root +1))
								{
									DPRINT("ERR%d: n_depth of children %d is inconsistent with depth of father %d... FIXED\n", __LINE__, this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root, this->grcl_tree.gast_nodes[ this->grcl_tree.gast_nodes[n_current_index].n_index_father ].n_distance_from_root );
									this->grcl_tree.gast_nodes[n_current_index].n_distance_from_root = this->grcl_tree.gast_nodes[ this->grcl_tree.gast_nodes[n_current_index].n_index_father ].n_distance_from_root +1;
								}
							}
                        }
                        //I have scanned a node
						this->gn_cnt_nodes++;
						//Return index popped or max size if nothing was popped
						DRETURN_ARG("Count nodes: %d | Index: %d", this->gn_cnt_nodes, n_current_index );
						return n_current_index;
                    }
                    //Stack is empty
                    else
                    {
						//Mark this an END iterator
						this->gn_cnt_nodes = this->grcl_tree.gast_nodes.size();
                        //Return index that points to the element after the last element
                        DRETURN_ARG("last element...");
                        return this->grcl_tree.gast_nodes.size();
                    }
                }	//end private method: next
                //! The reference to the parent class
                Tree<Payload> &grcl_tree;
                //!	Stack to handle the pseudorecursion
                std::stack<size_t> gcl_pseudorecursive_stack;
                //!	Count the nodes that have been scanned
                size_t gn_cnt_nodes;
            //End Private
        };	//Class: iterator

        //! @brief iterator that start from the first element of the tree
        iterator<Node> begin( void )
        {
            //Construct a Begin iterator with a root inside the pseudorecursive stack
            return iterator<Node>(*this, true );
        }
        //! @brief iterator that start from a given node
        iterator<Node> begin( size_t in_index )
        {
            //Construct a Begin iterator with a root inside the pseudorecursive stack
            return iterator<Node>(*this, true, in_index );
        }
        //! @brief iterator that marks the end of the tree
        iterator<Node> end( void )
        {
            //Construct a End iterator
            return iterator<Node>(*this, false );
        }

    //Visible to derived classes
    protected:
        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PROTECTED METHODS
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
		//! @brief initialize the root
		bool init_root( void );
        bool init_root( Payload it_payload )
        {
			bool x_ret = init_root();
			if (x_ret == false) { this->gast_nodes[0].t_payload = it_payload; }
			return x_ret;
        }

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief the children of target nodes are bumped as children of the father of target nodes (from children to siblings)
        bool bump_children( size_t in_index_node );
        //! @brief relink target node under a new father
        bool relink_node_nocheck( size_t in_index_target_node, size_t in_index_father_destination );
        //! @brief iterator to node to be ereased
		bool erease_single_node( typename std::vector<Node>::iterator cl_iterator_erease );
        //! @brief turns a Node into a string
        static std::string to_string( User::Tree<Payload>::Node &ist_node )
        {
            std::string s_ret = "";
			s_ret += "Own Index: " +std::to_string( ist_node.n_own_index );
            s_ret += " | Father Index: ";
            s_ret += std::to_string(ist_node.n_index_father);
            s_ret += " | Own Priority ";
            s_ret += std::to_string(ist_node.n_own_priority);
            s_ret += " | Children: ";
            s_ret += std::to_string(ist_node.n_children_max_priority);
            s_ret += " | Depth: ";
            s_ret += std::to_string(ist_node.n_distance_from_root);
            //construct string from array of char
            return s_ret;
        }
        //Count the children of a node
        bool count_children( typename std::vector<Node>::iterator st_father, size_t &orn_num_children );
        //! @brief updates the indexes used by the iterator to make a deep expolration of the tree
        //bool compute_deep_exploration_indexes( size_t ign_index, size_t in_depth );
        //Report an error. return false: OK | true: Unknown error code
        bool report_error( const char *ips8_error_code );
        //Tries to recover from an error. Automatically called by get_error. return false = OK | true = fail
        bool error_recovery( void );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//! @brief stores decorator to convert a Payload into a std::string
		std::function<std::string(Payload)> gf_lambda_decorator;
        //The nodes are stored inside a standard vector
        std::vector<Node> gast_nodes;
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
    DENTER_ARG("This: %p", &(*this));   //Trace Enter
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
    DENTER_ARG("This: %p", &(*this));   //Trace Enter
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
    DENTER_ARG("This: %p", &(*this));   //Trace Enter
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
//! @return no return
//! @details
//! \n Empty destructor
/***************************************************************************/

template <class Payload>
Tree<Payload>::~Tree( void )
{
    DENTER_ARG("This: %p", &(*this));   //Trace Enter

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();      //Trace Return
    return;         //OK
}   //Destructor: Tree | void

/***************************************************************************/
//!	@brief operator [] | int
/***************************************************************************/
//!	@param ign_index | Numeric index of the node
//! @return Payload & | Reference to content of the node
//!	@details
//! LHS and RHS access to the payload of a node. Returns the reference to the
//!	dummy payload if an invalid index is given, raising an error inside the tree
/***************************************************************************/

template <class Payload>
Payload& Tree<Payload>::operator []( size_t in_index )
{
    //Trace Enter
    DENTER_ARG("Object: %p, Index: %d", &(*this), (int)in_index );
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    //The user is trying to access a node outside the range
    if (in_index >= this->gast_nodes.size())
    {
        DRETURN_ARG("Index OOB: %d of %d", in_index, this->gast_nodes.size() );
        //Return a reference to the dummy payload
        return this->gt_dummy;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    //Trace Return from main
    DRETURN();
    return this->gast_nodes[in_index].t_payload;
}	//end method: operator & | int |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC SETTERS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public Setter: create_child | Payload
/***************************************************************************/
//! @param it_payload | payload to be attached to this leaf
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Create a child of the root
/***************************************************************************/

template <class Payload>
size_t Tree<Payload>::create_child( Payload it_payload )
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

    //Create the child using the generic method
    size_t n_child_index = this->create_child( 0, it_payload );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Child Index: %d", n_child_index ); //Trace Return
    return n_child_index;	//OK
}   //Public Setter: create_leaf | Payload

/***************************************************************************/
//! @brief Public Setter: create_child | size_t | Payload
/***************************************************************************/
//! @param in_father_index | Numeric index of the node that is to be the father of the new node. The father may have nodes already
//! @param it_payload | payload to be attached to this leaf
//! @return size_t | 0 = FAIL. 0 is the index of the root, it can never be created by create_child | >0 index of the node in the array
//! @details
//! \n Create a child of the root
/***************************************************************************/

template <class Payload>
size_t Tree<Payload>::create_child( size_t in_father_index, Payload it_payload )
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
    if (in_father_index >= this->gast_nodes.size())
    {
        DRETURN_ARG("ERR:%d | OOB %d of %d | %s", __LINE__, in_father_index, this->gast_nodes.size(),this->gps8_error_code );
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
    st_node.n_children_max_priority = 0;
    //Index of the root
    st_node.n_index_father = in_father_index;
    //Number of children of the father
    //Also number of siblings of the node being created.
    //Also the priority, resolution order of those children.
    size_t n_num_children = this->gast_nodes[ st_node.n_index_father ].n_children_max_priority;
    //By the end, I will have added a children to the father
    n_num_children++;
    //I update the number of children of the father, thus the max priority of the father
    this->gast_nodes[ st_node.n_index_father ].n_children_max_priority = n_num_children;
    //The newly created node has the lowest priority
    st_node.n_own_priority = n_num_children -1;
    //Distance from the root. The depth of this node is one deeper than the father. For create child it's easy to compute.
    st_node.n_distance_from_root = this->gast_nodes[ st_node.n_index_father ].n_distance_from_root +1;
    //This is what the index of this node should be after the creation operation is complete
    st_node.n_own_index = this->gast_nodes.size();
    //Add the node to the tree
    this->gast_nodes.push_back( st_node );
    //!@todo check that the node has been created with the right content
    if (st_node.n_own_index != this->gast_nodes.size() -1)
    {
        this->report_error( Error_code::CPS8_ERR );
        DRETURN_ARG("ERR%d: push_back seemingly did not create a new node | %d of %d", __LINE__, st_node.n_own_index, this->gast_nodes.size() );
        return true;
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Father Index: %d | Own Index: %d | Nodes under Father: %d", st_node.n_index_father, st_node.n_own_index, n_num_children +1 ); //Trace Return
    return st_node.n_own_index;	//OK
}   //Public Setter: create_child | size_t | Payload

/***************************************************************************/
//! @brief Public Setter: erease | size_t
/***************************************************************************/
//! @param in_index | Delete a node
//! @param ie_delete_mode | NODE = delete node and relinks children | DEEP = delete node and all descendence
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n
//! \n Create a child of the root
//! \n	------------------------------
//! \n	EXAMPLE: erease (102, NODE)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-201
//! \n	|-102			|-202
//! \n		|-202		|-203
//! \n		|-203
//! \n	------------------------------
//! \n	EXAMPLE: erease (102, DEEP)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-201
//! \n	|-102
//! \n		|-202
//! \n		|-203
//! \n	------------------------------
//! \n	EXAMPLE: erease (100, NODE) (ROOT)
//! \n	100				101
//! \n	|-101				|-201
//! \n		|-201		102
//! \n	|-102				|-202
//! \n		|-202			|-203
//! \n		|-203
//! \n	not allowed, this could result in multiple roots. The root must be singular and at index 0.
//! \n	------------------------------
//! \n	EXAMPLE: erease (100, DEEP) (ROOT)
//! \n	100
//! \n	|-101
//! \n		|-201
//! \n	|-102
//! \n		|-202
//! \n		|-203
//! \n	erease the whole tree not allowed because there is no root
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::erease( size_t in_index_erease, Erease_mode ie_delete_mode )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK&INIT
    //--------------------------------------------------------------------------
    //if class is in error
    if (this->gps8_error_code != Error_code::CPS8_OK)
    {
        DRETURN_ARG("ERR:%d Tree is in error: %s | Cannot destroy node", __LINE__, this->gps8_error_code );
        return true;
    }
	//If user tries to erease a node that is OOB
    if ((Config::CU1_EXTERNAL_CHECKS) && (in_index_erease >= this->gast_nodes.size()))
    {
		DRETURN_ARG("ERR%d | index %d of %d | OOB erease index", __LINE__, in_index_erease, this->gast_nodes.size() );
		return true;
    }
    //If user tries to erease the ROOT
    if ((Config::CU1_EXTERNAL_CHECKS) && (in_index_erease == 0))
    {
		DRETURN_ARG( "ERR%d | root cannot be ereased", __LINE__ );
		return true;
    }



    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    switch( ie_delete_mode )
    {
		//--------------------------------------------------------------------------
		case Erease_mode::NODE:
		//--------------------------------------------------------------------------
		//	Erease a single node
		{
			auto cl_iterator = (this->gast_nodes.begin() +in_index_erease);
			this->erease_single_node( cl_iterator );

			/*
			//Bump children of node to be ereased
			bool x_fail = this->bump_children( in_index_erease );
			if (x_fail == true)
			{
				DRETURN_ARG("ERR:%d | Failed to bump children of target %d", __LINE__, in_index_erease );
				return true;
			}
			//Construct an iterator that points to the correct node
			auto cl_iterator = (this->gast_nodes.begin() +in_index_erease);
			//Sanity check size
			size_t n_size_before_erease = this->gast_nodes.size();
			size_t n_priority_of_ereased_node = this->gast_nodes[in_index_erease].n_own_priority;
			//Erease the node, this invalidates the iterator
			this->gast_nodes.erase( cl_iterator );
			//TRICKY: children priority 0 1 2 3. Erease 1. I need to change priority 0 DEL1 2-> 3->2
			//If vector erease does't have the expected result
			if ( (n_size_before_erease -1) != this->gast_nodes.size() )
			{
				DRETURN_ARG("ERR%d | Size: %d->%d | ERR Vector!!! I expected the vector to have size reduced by one...", __LINE__, n_size_before_erease, this->gast_nodes.size() );
				return true;
			}
			//A children with given priority has been removed. Fix priority of father, and priority of all children
			//this->fix_priority(   );
			*/


			break;
		}
		//--------------------------------------------------------------------------
		case Erease_mode::DEEP:
		//--------------------------------------------------------------------------
		{


			break;
		}
		//--------------------------------------------------------------------------
		default:
		//--------------------------------------------------------------------------
		{
			this->report_error( Error_code::CPS8_ERR );
			DPRINT("ERR%d | Unknown erease mode %d", __LINE__, int(ie_delete_mode) );
			return true;
		}
    };	//Delete_mode

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
}   //Public Setter: erease | size_t

/***************************************************************************/
//! @brief Public Setter: swap | size_t | size_t
/***************************************************************************/
//! @param in_lhs | Node to be swapped
//! @param in_rhs | Node to be swapped
//! @return bool | false = OK | true = FAIL
//! @details
//! \n	Swap two nodes of the tree
//! \n	I have two possible swaps:
//! \n	1) swap the nodes only
//! \n	2) swap the nodes and the subtree below the nodes
//! \n	swap with subtree is forbidden if the one node is part of the subtree of the other node?
//! \n	------------------------------
//! \n  "Payload Swap" will swap the content of two nodes
//! \n	EXAMPLE: Payload Swap (101, 102)
//! \n	100				100
//! \n	|-101			|-102
//! \n		|-201			|-201
//! \n	|-102			|-101
//! \n		|-202			|-202
//! \n		|-203			|-203
//! \n	------------------------------
//! \n  "Priority Swap" will swap the priority of two children of the same father, changing the order in which they are resolved
//! \n	EXAMPLE: Priority Swap (101, 102)
//! \n	100				100
//! \n	|-101			|-102
//! \n		|-201			|-202
//! \n	|-102			    |-203
//! \n		|-202		|-101
//! \n		|-203			|-201
//! \n	------------------------------
//! \n  "Swap" will swap two nodes along with all their subtrees. When the targets are not of the same bloodline the operation is straight forward
//! \n	EXAMPLE: Priority Swap (201, 202)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-202
//! \n	|-102		            |-204
//! \n	    |-202       |-102
//! \n		     |-204      |-201
//! \n		|-203			|-203
//! \n	------------------------------
//! \n  "Swap" executed on two nodes that belong to the same subtree is a more complicated operation
//! \n  swap between father and child, will make:
//! \n	child->father, father->child, father.children->children.children and children.children->father.children
//! \n  subtrees if those node are not impacted
//! \n  I am not sure if this complex operation makes sense, maybe it should be a configuration toggle of the tree to allow such a swap to take place
//! \n	EXAMPLE: Priority Swap (102, 202)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-201
//! \n	|-102(R)	    |-202(L)
//! \n	    |-202(L)        |-102(R)
//! \n		     |-204(L.C) |-204 (L.C)
//! \n              |-301       |-301
//! \n		|-203			|-203
//! \n	------------------------------
//! \n	EXAMPLE: Priority Swap (102, 204)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-201
//! \n	|-102(R)	    |-204(L)
//! \n	    |-202       	|-301(L.C)
//! \n			|-204(L)		|-102 (R)
//! \n           	|-301       	|-202 (R.C)
//! \n					|- 409			|-409
//! \n	------------------------------
//! \n
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::swap( size_t in_lhs, size_t in_rhs, Swap_mode ie_swap_mode )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    //If OOB indexes
    if ((in_lhs >= this->gast_nodes.size()) || (in_rhs >= this->gast_nodes.size()))
    {
        this->report_error(Error_code::CPS8_ERR_OOB);
        DRETURN_ARG("ERR%d: Node indexes (%d %d) out of range %d...", __LINE__, in_lhs, in_rhs, this->gast_nodes.size() );
        return true;
    }
    //if nothing to do
    if (in_lhs == in_rhs)
    {
        DRETURN();
        return false;
    }

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------
    //I want to swap node LHS with node RHS

    //true: the nodes are related and belong to the same line
    bool x_lhs_rhs_related;
    bool x_execute_subtree_swap = false;

    DPRINT("Payload Swap | LHS: %s | RHS %s\n", this->to_string( in_lhs ).c_str(), this->to_string( in_rhs ).c_str() );
    switch(ie_swap_mode)
    {
        //Swap the payload of two nodes, it's always possible
        case Swap_mode::PAYLOAD:
        {
			//Payload t_tmp = this->gast_nodes[in_lhs].t_payload;
			//this->gast_nodes[in_lhs].t_payload = this->gast_nodes[in_rhs].t_payload;
			//this->gast_nodes[in_rhs].t_payload = t_tmp;
			std::swap( this->gast_nodes[in_lhs].t_payload, this->gast_nodes[in_rhs].t_payload );
            break;
        }
        //Swap the priority of two nodes that are children to the same father
        case Swap_mode::PRIORITY:
        {
			//Root cannot be target of a priority swap
			if ((in_lhs == 0) || (in_rhs == 0))
			{
				DRETURN_ARG("ERR%d | Priority Swap is only defined for siblings. Root has no sibling.", __LINE__ );
				return true;
			}
			//if not siblings
            if (this->gast_nodes[in_lhs].n_index_father != this->gast_nodes[in_rhs].n_index_father)
            {
                DRETURN_ARG("ERR%d | Priority Swap is only defined for siblings. LHS%d.father is %d | RHS%d.father is %d", __LINE__, in_lhs, this->gast_nodes[in_lhs].n_index_father, in_rhs, this->gast_nodes[in_rhs].n_index_father );
                return true;
            }
            std::swap( this->gast_nodes[in_lhs].n_own_priority, this->gast_nodes[in_rhs].n_own_priority );
            break;
        }
        //Swap two nodes, along with all their descendence. Prevent two descendent to be swapped, as the operation might not yield the result the user wants
        case Swap_mode::SUBTREE_SAFE:
        {
			x_lhs_rhs_related = this->is_descendant(in_lhs, in_rhs);
            //If the nodes belong to the same subtree
            if (x_lhs_rhs_related == true)
            {
                DRETURN_ARG("ERR%d | SUBTREE_SAFE swap is not allowed when two nodes belong to the same subtree and they are relatives.",__LINE__);
                return true;
            }
            x_execute_subtree_swap = true;
            break;
        }
        //Swap two nodes, along with all their descendence. When two nodes of the same bloodline are swapped, it changes the way the children of those nodes relate to each others
        case Swap_mode::SUBTREE:
        {
			x_lhs_rhs_related = this->is_descendant(in_lhs, in_rhs);
			x_execute_subtree_swap = true;
            break;
        }
        default:
		{
			DRETURN_ARG("ERR: Unknown swap mode!!!");
			return true;
            break;
		}
    }
	//If a subtree swap was authorized
    if (x_execute_subtree_swap == true)
    {
        //I relink LHS as children of RHS.father
        //I relink RHS as children of LHS.father
        //The index of LHS and RHS stay the same
        //The index of LHS.father and RHS.father stay the same

        //Latch the original indexes of the two fathers
        //size_t n_lhs_father_index = this->gast_nodes[ in_lhs ].n_index_father;
        //size_t n_rhs_father_index = this->gast_nodes[ in_rhs ].n_index_father;
		//Swap the index of the father of the two nodes
		std::swap( this->gast_nodes[ in_lhs ].n_index_father, this->gast_nodes[ in_rhs ].n_index_father );
        //Update the nodes to the properties of their new father
		std::swap( this->gast_nodes[ in_lhs ].n_own_priority, this->gast_nodes[ in_rhs ].n_own_priority );

		//Update the father to the properties of their new children
		//Nothing to be done

		//Update all the depth information
		//If father had the same depth, no need to change depth information
		if (this->gast_nodes[ in_lhs ].n_distance_from_root != this->gast_nodes[ in_rhs ].n_distance_from_root)
		{
			//Swap subtree fathers depth
			std::swap( this->gast_nodes[ in_lhs ].n_distance_from_root, this->gast_nodes[ in_rhs ].n_distance_from_root );
			//Swap LHS subfather tree depth, the iterator automagically fixes depth information
			for (auto cl_children_iterator = this->begin( in_lhs ); cl_children_iterator != this->end();cl_children_iterator++)
			{
				(*cl_children_iterator);
			}
			//Swap RHS subfather tree depth, the iterator automagically fixes depth information
			for (auto cl_children_iterator = this->begin( in_rhs ); cl_children_iterator != this->end();cl_children_iterator++)
			{
				(*cl_children_iterator);
			}
		}

    }	//If a subtree swap was authorized

    DPRINT("After        | LHS: %s | RHS %s\n", this->to_string( in_lhs ).c_str(), this->to_string( in_rhs ).c_str() );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;
}

/***************************************************************************/
//! @brief Public Setter: root | size_t | size_t
/***************************************************************************/
//! @param oru1_fail | false = OK | true = FAIL
//! @return Payload & | Reference to the payload of the root node |
//! @details
//! \n Return the reference to the payload of the root node
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::move( size_t in_index_node_target, size_t in_index_new_father, Move_mode ie_move_mode )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //if class is in error
    if (this->gps8_error_code != Error_code::CPS8_OK)
    {
		DRETURN();
		return true;
    }
    //if source OOB
    if ((in_index_node_target == 0) || (in_index_node_target >= this->gast_nodes.size()))
    {
		DRETURN_ARG("ERR%d | OOB source index %d of %d", __LINE__, in_index_node_target, this->gast_nodes.size() );
		return true;
    }
    //if new father OOB
	if (in_index_new_father >= this->gast_nodes.size())
    {
		DRETURN_ARG("ERR%d | OOB new father index %d of %d", __LINE__, in_index_new_father, this->gast_nodes.size() );
		return true;
    }
    //Remember previous father
	size_t n_index_target_old_father = this->gast_nodes[in_index_node_target].n_index_father;
    //if new father is same as old father there is nothing to do
    if (in_index_new_father == n_index_target_old_father)
    {
		DRETURN_ARG("Nothing to do. Target is already under father %d", n_index_target_old_father );
		return false;
    }

	bool x_fail = false;
	//--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //I want to move just the target node
	if (ie_move_mode == Move_mode::NODE)
	{
		//Bump all children of target node to be siblings instead
		x_fail = this->bump_children( in_index_node_target );
		if (x_fail == true)
		{
			DRETURN_ARG("ERR%d | couldn't bump children of node %d...", __LINE__, in_index_node_target );
			return x_fail;
		}
	}
	//Now the node I want to move is isolated
	x_fail = this->relink_node_nocheck( in_index_node_target, in_index_new_father );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return x_fail;
}   //Public getter: root | bool & |

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
//! @return bool | false = OK | true = FAIL |
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

    std::cout << "Number of Nodes: " << this->gast_nodes.size() << "\n";
    //Scan vector of nodes
    for (typename std::vector<Node>::iterator pst_node = this->gast_nodes.begin();pst_node < this->gast_nodes.end();pst_node++)
    {
		//Print node
		std::cout << ">" << this->gf_lambda_decorator((*pst_node).t_payload) << " || " << *pst_node << "\n";
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;
}   //Public getter: show | void |

/***************************************************************************/
//! @brief Public getter: show | void |
/***************************************************************************/
//! @param in_index | index of the node. Will traverse and print every node down from this node
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Show the nodes stored inside the vector and their links
//! \n Recursive tree travel function
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::show( size_t in_index )
{
    DENTER_ARG("Index: %d", in_index ); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    if (this->gast_nodes.size() <= 0)
    {
        this->report_error(Error_code::CPS8_ERR);
        DRETURN_ARG("ERR:%d | Vector should contain at least the root...", __LINE__ );
        return true;
    }

    //--------------------------------------------------------------------------
    //	SHOW
    //--------------------------------------------------------------------------

    //! @todo handle pedantic checks on array sizes and failure
    for (auto cl_explore_iterator = this->begin();cl_explore_iterator!=this->end();cl_explore_iterator++)
    {
        //Print a spacer for each level of descent into the tree. print a L line near the node
        for (size_t n_cnt = 0;n_cnt < (*cl_explore_iterator).n_distance_from_root;n_cnt++)
        {
            if (n_cnt < (*cl_explore_iterator).n_distance_from_root -1 )
            {
                std::cout << "    ";
            }
            else
            {
				//std::cout << "|---";
                std::cout << "    ";
            }
        }
        //Print the content of the node
        std::cout << ">" << this->gf_lambda_decorator((*cl_explore_iterator).t_payload) << " || " << this->to_string(*cl_explore_iterator) << "\n";
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();
    return false;
}   //Public getter: show | void |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC TESTER
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public Method | is_descendant | size_t | size_t
/***************************************************************************/
//! @param in_lhs | index of node to be checked
//! @param in_rhs | index of node to be checked
//! @return bool | false = OK | true = FAIL |
//! @todo I feel there is a much more efficient algorithm...
//! @details
//! \n check if two nodes are direct relatives
//! \n I need to explore both subtrees and check if one appears in the other subtree
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::is_descendant(size_t in_lhs, size_t in_rhs)
{
    DENTER_ARG("LHS: %d | RHS: %d", in_lhs, in_rhs ); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    if ((Config::CU1_INTERNAL_CHECKS == true) && ((in_lhs >= this->gast_nodes.size()) || (in_rhs >= this->gast_nodes.size())) )
    {
		this->report_error( Error_code::CPS8_ERR_OOB );
        DRETURN_ARG("ERR%d: OOB size: %d", __LINE__, this->gast_nodes.size() );
        return true;
    }

    //--------------------------------------------------------------------------
    //	QUICK CHECKS
    //--------------------------------------------------------------------------

    //Same nodes
	if (in_lhs == in_rhs)
	{
		DRETURN_ARG("The same node are not descendants...");
		return false;
	}
	//One is the other Father, quick check
	else if (in_lhs == this->gast_nodes[in_rhs].n_index_father)
	{
		DRETURN_ARG("LHS is father of RHS");
		return true;
	}
	else if (in_lhs == this->gast_nodes[in_rhs].n_index_father)
	{
		DRETURN_ARG("RHS is father of LHS");
		return true;
	}

	//--------------------------------------------------------------------------
    //	SLOW CHECK
    //--------------------------------------------------------------------------
    //	Use iterator to build both trees under the nodes, and stop if one is found under that node

    //Scan all the descendence of LHS
	for (User::Tree<Payload>::iterator<User::Tree<Payload>::Node> cl_lhs_iterator=this->begin( in_lhs );cl_lhs_iterator!=this->end();cl_lhs_iterator++)
	{
		DPRINT("LHS scan: %d\n", cl_lhs_iterator.get_index() );
		//if RHS is a descendant of LHS
		if (cl_lhs_iterator.get_index() == in_rhs)
		{
			DRETURN_ARG("RHS is a descendent of LHS through node: %d", cl_lhs_iterator.get_index() );
			return true;
		}
	}
	//Scan all the descendence of RHS
	for (User::Tree<Payload>::iterator<User::Tree<Payload>::Node> cl_rhs_iterator=this->begin( in_rhs );cl_rhs_iterator!=this->end();cl_rhs_iterator++)
	{
		DPRINT("RHS scan: %d\n", cl_rhs_iterator.get_index() );
		//if RHS is a descendant of LHS
		if (cl_rhs_iterator.get_index() == in_lhs)
		{
			DRETURN_ARG("LHS is a descendent of RHS through node: %d", cl_rhs_iterator.get_index() );
			return true;
		}
	}

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    //LHS and RHS are not related
    DRETURN();
    return false;
} 	//Public Method | is_descendant | size_t | size_t

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC METHODS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public Method | flush | void
/***************************************************************************/
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Flush the content of a tree except for the root
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::flush( void )
{
    DENTER_ARG( "Nodes inside the tree: %d", this->gast_nodes.size() ); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

	//while I have more than the root
	while (this->gast_nodes.size() > 1)
	{
		//Remove an element
		this->gast_nodes.pop_back();
	}

	//!re align the root
	bool x_ret = this->init_root();

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Nodes inside the tree: %d", this->gast_nodes.size() ); //Trace Return
    return x_ret;	//OK
} 	//Public Method: report_error | Error_code

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	FRIEND METHODS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Protected Getter | find_children | size_t | std::vector<size_t> &
/***************************************************************************/
//! @param in_father_index | index of the father of which I want to find children
//! @return std::vector<size_t> | array containing the indexes of the children, from the highest priority to the lowest priority
//! @details
//! \n  Find the children of a node of a given index, return those indexes inside a vector. If father has no children, vector will be empty
//! \n  I can't sort easily the priority because std::sort of iran_children_indexes needs the key n_own_priority inside gast_node
//! \n  There is a better way. I can preallocate the vector, and use the priority as index during the search, and put them in the right spot for free! O(n)
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::find_children( size_t in_father_index,std::vector<size_t> &iran_children_indexes )
{
    DENTER_ARG("Father: %d", in_father_index); //Trace Enter
    //--------------------------------------------------------------------------
    //	CHECK
    //--------------------------------------------------------------------------

    //If bad father index
    if (in_father_index > this->gast_nodes.size())
    {
        this->report_error(Error_code::CPS8_ERR_OOB);
        DRETURN_ARG("ERR: OOB trying to access index %d (father) when max size is %d", in_father_index, this->gast_nodes.size() );
        return true;
    }
    //If input array is not empty
    if (iran_children_indexes.size() > 0)
    {
        //Empty the vector from unwanted children
        iran_children_indexes.clear();
    }

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Search index for the next children, skip the root from the search
    size_t n_children_index = 1;
    //Number of children found
    size_t n_num_found_children = 0;
    //Fetch the number of children of the father
    size_t n_num_expected_children = this->gast_nodes[in_father_index].n_children_max_priority;
    //Only activate recursive search under this node if this node has at least one child
    bool x_search_children = (n_num_expected_children > 0);
    //Preallocate the expected number of children
    iran_children_indexes.resize( n_num_expected_children );
    if ((Config::CU1_INTERNAL_CHECKS == true) && (iran_children_indexes.size() != n_num_expected_children))
    {
        DRETURN_ARG("ERR%d: Failed to resize array expected %d | actual %d", __LINE__, n_num_expected_children, iran_children_indexes.size() );
        return true;
    }
    if (Config::CU1_INTERNAL_CHECKS == true)
    {
		//initialize array of children indexes to invalid
		for (auto cl_iter_children = iran_children_indexes.begin(); cl_iter_children<iran_children_indexes.end(); cl_iter_children++ )
		{
			(*cl_iter_children) = this->gast_nodes.size();
		}
	}
    //while authorized to scan for children
    while (x_search_children == true)
    {
        //If I find a node whose father is the index I just printed
        if (in_father_index == this->gast_nodes[n_children_index].n_index_father)
        {
            //Fetch the priority of the node. The priority decides the order in which children are resolved and depends on the number of children the father has
            size_t n_priority = this->gast_nodes[n_children_index].n_own_priority;
            //Pedantic check that the priority of the node is consistent with the number of children of the father
            if ( (Config::CU1_INTERNAL_CHECKS == true) && (n_priority >= n_num_expected_children))
            {
                DRETURN_ARG("ERR%d: Priority %d of %d | Found child (index %d) whose priority exceed the number of children of the father...", __LINE__, n_priority, n_num_expected_children, n_children_index );
                return true;
            }
            //I can presort the array by using the priority as index to the preallocated chidlren array, saving lots of work
            iran_children_indexes[n_priority] = n_children_index;
            DPRINT("Found children || Index %d | %s\n", n_children_index, this->to_string( this->gast_nodes[n_children_index] ).c_str() );
            //I just found a child
            n_num_found_children++;
            //If I found ALL the children of this node
            if (n_num_found_children >= n_num_expected_children)
            {
                x_search_children = false;
            }
        }	//If I find a node whose father is the index I just printed
        //Scan next child
        n_children_index++;
        //Scanned all nodes
        if (n_children_index > this->gast_nodes.size())
        {
            iran_children_indexes.clear();
            DRETURN_ARG("ERR%d: Search for child reached the end of the array without finding one... Found %d of %d expected children...", __LINE__, n_num_found_children, n_num_expected_children );
            return true;
        }
    }	//while authorized to scan for children

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------

    //check if one of the expected priorities wasn't found. can happen if priority got mangled. E.g. GOOD 0 1 2 3 | BAD 0 2 2 3
	if (Config::CU1_INTERNAL_CHECKS == true)
    {
		//initialize array of children indexes to invalid
		for (auto cl_iter_children = iran_children_indexes.begin(); cl_iter_children<iran_children_indexes.end(); cl_iter_children++ )
		{
			if ((*cl_iter_children) == this->gast_nodes.size())
			{
				DRETURN_ARG("ERR%d: priority %d of %d wasn't found after searching for children. priority of children is mangled...", __LINE__, cl_iter_children- iran_children_indexes.begin(), n_num_expected_children );
				return true;
			}
		}
	}
    //If the number of found children doesn't match the number of children of the father
    if (n_num_found_children != n_num_expected_children)
    {
        iran_children_indexes.clear();
        DRETURN_ARG("ERR%d: Father is supposed to have %d children, but %d children have been found...", __LINE__, n_num_expected_children, n_num_found_children );
        return true;
    }
    //if the vector failed to record all the expected indexes it was supposed to record
    if (n_num_found_children != iran_children_indexes.size())
    {
        iran_children_indexes.clear();
        DRETURN_ARG("ERR%d: Array size %d | Expected: %d | Array of children indexes isn't holding the correct number of children indexes ...", __LINE__, iran_children_indexes.size(), n_num_expected_children  );
        return true;
    }

    DRETURN_ARG("Found %d | %d children", n_num_found_children, iran_children_indexes.size() ); //Trace Return
    return false;
}	//Protected Getter | find_children | size_t | std::vector<size_t> &

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

    //If the tree already has nodes
    if ((Config::CU1_INTERNAL_CHECKS == true) && (this->gast_nodes.size() > 0))
    {
		bool x_ret = this->flush();
		DRETURN_ARG("WAR:__LINE__ The tree was already initialized, flush instead.");
		return x_ret;
    }

    Node st_node;
    st_node.t_payload = it_payload;
    //Root starts with no children
    st_node.n_children_max_priority = 0;
    //Root has index 0
    st_node.n_own_index = 0;
    //Root cannot have a priority
    st_node.n_own_priority = 0;
    //Root is at depth 0
    st_node.n_distance_from_root = 0;
    //Register the father index. Root points to itself, special root code
    st_node.n_index_father = 0;
    //Allocate root and fill root with dummy payload
    this->gast_nodes.push_back( st_node );
    if (this->gast_nodes.size() != 1)
    {
        //! @todo: Undo initialization
        this->report_error( Error_code::CPS8_ERR );
        DRETURN_ARG("ERR%d: There should be exactly one node (Root) after initialization, there are %d instead", __LINE__, this->gast_nodes.size() );
        return true;
    }
    //Initialize error code
    this->gps8_error_code = Error_code::CPS8_OK;
    //Link a default decorator to stringify the payload.
    this->gf_lambda_decorator = [](Payload it_payload){ return std::string("use link_decorator to stringfy your Payload"); };

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();      //Trace Return
    return false;   //OK
}   //Private Method: init_class_vars | void

/***************************************************************************/
//! @brief Private Method: init_root | void
/***************************************************************************/
//! @param it_payload | root payload
//! @return no return
//! @details
//! \n Initialize class vars
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::init_root( void )
{
    DENTER();		//Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //If there is no root
    if ((Config::CU1_INTERNAL_CHECKS == true) && (this->gast_nodes.size() < 1))
    {
		DRETURN_ARG("ERR:__LINE__ | There are no nodes! There should be at least the root...");
		return true;
    }
    //Root starts with no children
    this->gast_nodes[0].n_children_max_priority = 0;
    //Root cannot have a priority, it has no father and no siblings
    this->gast_nodes[0].n_own_priority = 0;
    //Root is at depth 0
    this->gast_nodes[0].n_distance_from_root = 0;
    //Register the father index. Root points to itself, the special root code.
    this->gast_nodes[0].n_index_father = 0;

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
//! @brief Private Method | bump_children | in_index_node
/***************************************************************************/
//! @param in_index_node | index of the target node
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n the children of target nodes are bumped as children of the father of target nodes (from children to siblings)
//! \n	------------------------------
//! \n  "Payload Swap" will swap the content of two nodes
//! \n	EXAMPLE: Bump (102)
//! \n	100				100
//! \n	|-101			|-101
//! \n		|-201			|-201
//! \n	|-102			|-102
//! \n		|-202		|-202
//! \n		|-203		|-203
//! \n			|-301		|-301
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::bump_children( size_t in_index_node )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //If trying to bump root's children
    if (in_index_node == 0)
    {
		DRETURN_ARG("ERR%d | Root's children cannot be bumped as root's siblings as root doesn't have a father...", __LINE__ );
		return true;
    }
    else if (in_index_node >= this->gast_nodes.size() )
    {
		DRETURN_ARG("ERR%d | OOB | Index %d of %d", __LINE__, in_index_node, this->gast_nodes.size() );
		return true;
    }

    //Find children of target node
	auto an_children = this->get_children( in_index_node );
	if (an_children.size() <= 0)
	{
		DRETURN_ARG("target node has %d children to be bumped as siblings", an_children.size() );
		return false;
	}

	//--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------
	//TRICKY: children priority 0 1 2 3. Erease 1. I need to change priority 0 DEL1 2-> 3->2

	//Remember the father of target node
	size_t n_index_father_of_target_node = this->gast_nodes[in_index_node].n_index_father;
	//I scan the whole tree and update all node indexes
	for (auto cl_iterator = this->gast_nodes.begin(); cl_iterator < this->gast_nodes.end();cl_iterator++)
	{
		//If scanning the root
		if (cl_iterator==this->gast_nodes.begin())
		{
			//Do nothing
		}
		//If this node has the father of the ereased node as father and is not the root
		else if (cl_iterator->n_index_father == n_index_father_of_target_node)
		{
			//Do nothing
		}
		//The node has the target as father
		else if (cl_iterator->n_index_father == in_index_node)
		{
			//Relinks children of target to be children of father of target, also, sibling of target
			//now have the father of the ereased node as father
			cl_iterator->n_index_father = n_index_father_of_target_node;
			DPRINT("Relinked node: %d as child of node: %d\n", this->gast_nodes.end() -cl_iterator, cl_iterator->n_index_father);
			//priority needs to be updated, they are inserted as newborn children
			cl_iterator->n_own_priority = this->gast_nodes[n_index_father_of_target_node].n_children_max_priority;
			this->gast_nodes[n_index_father_of_target_node].n_children_max_priority++;
			DPRINT("Updated priority of child index: %d of ereased node: %d\n", this->gast_nodes.end() -cl_iterator, cl_iterator->n_own_priority );
			DPRINT("Updated number of children/max priority of node %d that is father of ereased node: %d\n", n_index_father_of_target_node, this->gast_nodes[n_index_father_of_target_node].n_children_max_priority );
			//Adjust depth since I'm bumping them up by one (adjust depth of their children as well)
			cl_iterator->n_distance_from_root--;
			//! @TODO: adjust depth of every node that is child of this node
			//Distance from root is reduced by one for all the descendence of the children of the ereased node
		} //The node that have the ereased node as father
		//every node that points to an index that came after the ereased node needs to be reduced by one
		else
		{
			//Do nothing
		}
	}	//I scan the whole tree and update all node indexes
	//The target father has no children now that they have all been bumped
	this->gast_nodes[in_index_node].n_children_max_priority = 0;
	//The father of target father has lost a child
	//this->gast_nodes[n_index_father_of_target_node].n_children_max_priority--;
	//! @todo I need to update the priority of the siblings

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
} 	//Private Method: report_error | Error_code

/***************************************************************************/
//! @brief Private Method | relink_node_nocheck | size_t, size_t
/***************************************************************************/
//! @param in_index_target_node | index of target node to be relinked
//! @param in_index_father_destination
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Fast relink with no integrity checks
//! \n Relink target to be a child of another father
//! \n Doesn't care to update the descendence of target
//! \n Updates old and new father of target
//! \n Updates priority of old siblings
//! \n
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::relink_node_nocheck( size_t in_index_target_node, size_t in_index_new_father )
{
    DENTER_ARG("Target Index: %d | New Father: %d", in_index_target_node, in_index_new_father );
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Remember old father of target for relinking
    size_t n_index_target_old_father = this->gast_nodes[in_index_target_node].n_index_father;
    size_t n_old_priority_target = this->gast_nodes[in_index_target_node].n_own_priority;
    //Relink node to new father
	this->gast_nodes[in_index_target_node].n_index_father = in_index_new_father;
	DPRINT("Relink Node %d as child of %d\n", in_index_target_node, in_index_new_father );
	//Recompute priority of taget and new father
	this->gast_nodes[in_index_target_node].n_own_priority = this->gast_nodes[in_index_new_father].n_children_max_priority;
	this->gast_nodes[in_index_new_father].n_children_max_priority++;
	DPRINT("With priority %d of %d\n",this->gast_nodes[in_index_target_node].n_own_priority ,this->gast_nodes[in_index_new_father].n_children_max_priority );
	//Recompute depth of target
	this->gast_nodes[in_index_target_node].n_distance_from_root = this->gast_nodes[in_index_new_father].n_distance_from_root +1;
	//Recompute number of children of old father
	this->gast_nodes[n_index_target_old_father].n_children_max_priority--;
	DPRINT("Old father %d now has %d children\n", n_index_target_old_father, this->gast_nodes[n_index_target_old_father].n_children_max_priority );
	//if old father has more children, and
	//!@todo if there is priority to be adjusted
	if (this->gast_nodes[n_index_target_old_father].n_children_max_priority >= 1)
	{
		//Scan old father children to update priority
		for (auto cl_iterator_node = this->gast_nodes.begin();cl_iterator_node < this->gast_nodes.end();cl_iterator_node++)
		{
			//if root
			if (cl_iterator_node == this->gast_nodes.begin())
			{
				//Do nothing. Root can't be sibling of anyone
			}
			//If children of old father/sibling of target
			else if (cl_iterator_node->n_index_father == n_index_target_old_father)
			{
				//if lower priority/higher number/younger sibling
				if (cl_iterator_node->n_own_priority >= n_old_priority_target)
				{
					//Increase the priority/decrease priority number
					cl_iterator_node->n_own_priority--;
					DPRINT("Previous sibling %d now has priority %d of %d\n", cl_iterator_node -this->gast_nodes.begin(), cl_iterator_node->n_own_priority, this->gast_nodes[n_index_target_old_father].n_children_max_priority );
				}
			}
		}
	}

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
} 	//Private Method: Private Method | relink_node_nocheck | size_t, size_t

/***************************************************************************/
//! @brief Private Method | erease_single_node | std::vector<Node>::iterator
/***************************************************************************/
//! @param cl_iterator_erease | iterator to node to be ereased
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Erease a single node from the tree
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::erease_single_node( typename std::vector<Node>::iterator cl_iterator_erease )
{
    DENTER();
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

	//Remember the father of the ereased node
	size_t n_index_father_of_ereased_node = cl_iterator_erease->n_index_father;
	//Sanity check size
	size_t n_size_before_erease = this->gast_nodes.size();
	size_t n_priority_of_ereased_node = cl_iterator_erease->n_own_priority;
	//Erease the node, this invalidates the iterator
	this->gast_nodes.erase( cl_iterator_erease );
	//TRICKY: children priority 0 1 2 3. Erease 1. I need to change priority 0 DEL1 2-> 3->2
	//If vector erease does't have the expected result
	if ( (n_size_before_erease -1) != this->gast_nodes.size() )
	{
		DRETURN_ARG("ERR%d | Size: %d->%d | ERR Vector!!! I expected the vector to have size reduced by one...", __LINE__, n_size_before_erease, this->gast_nodes.size() );
		return true;
	}
	//The father of ereased node has one fewer child
	this->gast_nodes[n_index_father_of_ereased_node].n_children_max_priority--;
	//I scan the whole tree and update all node indexes
	for (auto cl_iterator = this->gast_nodes.begin(); cl_iterator < this->gast_nodes.end();cl_iterator++)
	{
		//If scanning the root
		if (cl_iterator==this->gast_nodes.begin())
		{
			//Do nothing
		}
		//Update the priorities of the children of the father of the ereased node. decrease all priorities of following children to ereased one.
		//If this node has the father of the ereased node as father and is not the root
		else if (cl_iterator->n_index_father == n_index_father_of_ereased_node)
		{
			//If this node has a priority that comes after the priority of the ereased node
			if (cl_iterator->n_own_priority > n_priority_of_ereased_node)
			{
				cl_iterator->n_own_priority--;
			}
		}

		//The node that have the ereased node as father
		if (cl_iterator->n_index_father == cl_iterator->n_own_index)
		{
			//Adjust depth since I'm bumping them up by one (adjust depth of their children as well)
			cl_iterator->n_distance_from_root--;
			//now have the father of the ereased node as father
			cl_iterator->n_index_father = n_index_father_of_ereased_node;
			DPRINT("Relinked node: %d as child of node: %d\n", this->gast_nodes.end() -cl_iterator, cl_iterator->n_index_father);
			//priority needs to be updated, they are inserted as newborn children
			cl_iterator->n_own_priority = this->gast_nodes[n_index_father_of_ereased_node].n_children_max_priority;
			this->gast_nodes[n_index_father_of_ereased_node].n_children_max_priority++;
			DPRINT("Updated priority of child index: %d of ereased node: %d\n", this->gast_nodes.end() -cl_iterator, cl_iterator->n_own_priority );
			DPRINT("Updated number of children/max priority of node %d that is father of ereased node: %d\n", n_index_father_of_ereased_node, this->gast_nodes[n_index_father_of_ereased_node].n_children_max_priority );
			//Distance from root is reduced by one for all the descendence of the children of the ereased node


		} //The node that have the ereased node as father
		//every node that points to an index that came after the ereased node needs to be reduced by one
		else
		{
			//Fix the father index of later index nodes
			if (cl_iterator->n_index_father > cl_iterator_erease->n_own_index )
			{
				cl_iterator->n_index_father--;
			}
		}
	}	//I scan the whole tree and update all node indexes

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();
    return false;	//OK
} 	//Private Method | erease_single_node | std::vector<Node>::iterator

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
    DENTER_ARG("ERR: %p", ips8_error_code );
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
    DRETURN();
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
