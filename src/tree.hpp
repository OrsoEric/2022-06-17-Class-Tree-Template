/**********************************************************************************
BSD 3-Clause License

Copyright (c) 2022, Orso Eric
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**********************************************************************************/

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
//! @TODO Method to access leaves
//! @details
//! \n A basic tree class that stores a template payload
/************************************************************************************/

template <class Payload>
class Tree
{
    //Visible to all
    public:
		/*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC ENUMS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //! @brief Configurations of the class
        typedef union _Config
        {
            //Checks to be performed on user dependent data
			static const bool CU1_EXTERNAL_CHECKS = true;
			//Checks to be performed on input dependent on internal algorithms
			static const bool CU1_INTERNAL_CHECKS = true;
        } Config;

        //! @brief Error codes of the class
        typedef union _Error_code
        {
			//No error
			static constexpr const char *CPS8_OK = "OK";
			//Generic error
			static constexpr const char *CPS8_ERR = "ERR";
			//Out Of Boundary
			static constexpr const char *CPS8_ERR_OOB = "ERR:OOB";
        } Error_code;

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC TYPEDEFS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  CONSTRUCTORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Empty Constructor
        Tree( void );
        //Initialized constructor to Payload
        Tree( Payload it_payload );

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

        Tree<Payload> &operator []( int is32_index );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC SETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//Create a new leaf with a given payload
        bool create_leaf( Payload it_payload );
		//Destroy the leaf of given index, recursively destroy all leaves of that leaf
        //bool destroy_leaf( int is32_leaf_index );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC GETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Get current error state of the library
        const char *get_error( void );

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

		//Tree public method
        bool my_public_method( void );
		//Print the content of the tree in tree form
        bool print( int is32_depth );
        bool print( void )
        {
			//Print everything from root down
			return this->print( 0 );
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

    //Visible to derived classes
    protected:
        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PROTECTED METHODS
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
        **	PRIVATE ENUM
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE TYPEDEFS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE INIT
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Initialize class vars
        bool init_class_vars( void );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Report an error. return false: OK | true: Unknown error code
        bool report_error( const char *ips8_error_code );
        //Tries to recover from an error. Automatically called by get_error. return false = OK | true = fail
        bool error_recovery( void );

        //Tree method to copy the code
        bool my_private_method( void );

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Dummy payload
        static const Payload gccl_dummy;
        //! @brief Error code of the class
        const char *gps8_error_code;
		//! @brief Payload stored inside a node
        Payload gt_payload;
		//! @brief Individual leaves to this leaf are stored inside a vector
        std::vector<Tree> gclat_leaves;
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
    this->init_class_vars();

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();  //Trace Return
    return;
}   //Constructor: Tree | void

/***************************************************************************/
//! @brief Constructor: Tree | Payload
/***************************************************************************/
//! @param it_payload | payload of the root node
//! @return no return
//! @details
//! \n Initialized constructor. Takes care of initializing the root node
/***************************************************************************/

template <class Payload>
Tree<Payload>::Tree( Payload it_payload )
{
    DENTER_ARG("This: %p", this);   //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Initialize class vars
    this->init_class_vars();
	//Initialize payload
    this->gt_payload = it_payload;

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();  //Trace Return
    return;
}   //Constructor: Tree | Payload

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

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC OPERATORS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//!	@brief Reference operator | operator [] | int
/***************************************************************************/
//!	@param is32_index | index to the leaves
//! @return no return
//!	@details
//!	this = source | assignment operator.
//!	take advantage of the std::vector assignment operator that is already implemented.
//!	automagically fixes vector size and content.
/***************************************************************************/

template <class Payload>
Tree<Payload> &Tree<Payload>::operator []( int is32_index )
{
	//Trace Enter
	DENTER_ARG("Object: %p, Index: %d", (void *)this, is32_index );
	//--------------------------------------------------------------------------
    //  BODY
    //--------------------------------------------------------------------------
	//if: user asks for an element outside array range
    if ((is32_index < 0) || (is32_index >= this->gclat_leaves.size()))
    {
		//Out Of Boundary
		this->report_error(Error_code::CPS8_ERR_OOB);
		//Error. Return reference to self
		DRETURN_ARG("ERR:OOB | Index: %d | Size: %d", is32_index, this->gclat_leaves.size() );
		return (*this);
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
	//Trace Return from main
	DRETURN();
	return this->gclat_leaves[is32_index];
}	//end method: operator & | int |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC SETTERS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public method: my_public_method | void
/***************************************************************************/
// @param
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Create a new leaf with a given payload
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::create_leaf( Payload it_payload )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Ask vector to allocate a new leaf with the given payload
    this->gclat_leaves.push_back( Tree( it_payload ) );

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN_ARG("Leaves: %d", this->gclat_leaves.size() ); //Trace Return
    return false;	//OK
}   //Public method: my_public_method | void

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**	PUBLIC GETTERS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public Getter: get_error | void |
/***************************************************************************/
//! @return Error_code
//! @details
//! \n Get current error state of the library
//! \n Try to recover from errors
/***************************************************************************/

template <class Payload>
const char *Tree<Payload>::get_error( void )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //Fetch error
    const char *err_code = this->gps8_error_code;
    //Try to recover from error
    bool f_ret = this -> error_recovery();
    //If: failed to recover
    if (f_ret == true)
    {
        //Do nothing
    }

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return err_code; //OK
}   //Public Getter: get_error | void |

/*********************************************************************************************************************************************************
**********************************************************************************************************************************************************
**  PUBLIC METHODS
**********************************************************************************************************************************************************
*********************************************************************************************************************************************************/

/***************************************************************************/
//! @brief Public method: my_public_method | void
/***************************************************************************/
// @param
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Method
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::my_public_method( void )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
}   //Public method: my_public_method | void

/***************************************************************************/
//! @brief Public method: print | void
/***************************************************************************/
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Print the content of the tree in tree form, from this leaf forward
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::print( int is32_depth )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

	//Print a spacer for each level of descent into the tree
    for (int i = 0;i < is32_depth;i++)
	{
		if ( i != is32_depth-1 )
		{
			std::cout << "    ";
		}
		else
		{
			std::cout << "|-- ";
		}
	}
	std::cout << this->gt_payload << "\n";
	//Recursively explore the tree at the next level of depth for each leaf
	for (int i = 0;i < this->gclat_leaves.size();i++)
	{
		this->gclat_leaves.at(i).print( is32_depth+1 );
	}

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
}   //Public method: print | void

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
bool Tree<Payload>::init_class_vars( void )
{
    DENTER();		//Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

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
		DRETURN_ARG("ERR: Class is already in error: %p | culd not report error: %p", this->gps8_error_code, ips8_error_code );
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
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN();      //Trace Return
    return true;    //FAIL
}   //Private Method: error_recovery | void |

/***************************************************************************/
//! @brief Private Method: my_private_method | void
/***************************************************************************/
// @param
//! @return bool | false = OK | true = FAIL |
//! @details
//! \n Method
/***************************************************************************/

template <class Payload>
bool Tree<Payload>::my_private_method( void )
{
    DENTER(); //Trace Enter
    //--------------------------------------------------------------------------
    //	INIT
    //--------------------------------------------------------------------------

    if (Config::CU1_EXTERNAL_CHECKS == true)
    {
		return true;
    }

    //--------------------------------------------------------------------------
    //	BODY
    //--------------------------------------------------------------------------

    //--------------------------------------------------------------------------
    //	RETURN
    //--------------------------------------------------------------------------
    DRETURN(); //Trace Return
    return false;	//OK
}   //Private Method: my_private_method | void

/**********************************************************************************
**  NAMESPACE
**********************************************************************************/

}	//end namespace: User

#else
    #warning "Multiple inclusion of hader file TREE_H_"
#endif
