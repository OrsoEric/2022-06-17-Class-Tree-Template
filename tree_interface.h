/**********************************************************************************
**  ENVIROMENT VARIABILE
**********************************************************************************/

#ifndef TREE_INTERFACE_H_
    #define TREE_INTERFACE_H_

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
//! @class      Tree_interface
/************************************************************************************/
//!	@author     Orso Eric
//! @version    2022-08-24
//! @brief      Generic tree template implementation
//! @copyright  BSD 3-Clause License Copyright (c) 2022, Orso Eric
//! @details
//! \n	Interface to a generic Tree implementation
//! \n	constructor creates an empty tree
/************************************************************************************/

template <class Payload>
class Tree_interface
{
    //Visible to all
    public:
		/*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC ENUMS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

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

        //! @brief Empty Constructor
        Tree_interface( void )
        {
			return;
        }
        //! @brief Default payload Constructor
        Tree_interface( Payload it_default_payload )
        {
			this->set_default_payload( it_default_payload );
			return;
        }
        //! @brief Default payload and root Constructor
        Tree_interface( Payload it_default_payload,Payload it_root_payload )
        {
			this->set_default_payload( it_default_payload );
			this->root() = it_root_payload;
			return;
        }

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  DESTRUCTORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        //Empty destructor
        ~Tree_interface( void )
        {
			return;
        }

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC OPERATORS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC SETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//! @brief Set the default payload to be used when something goes wrong
        virtual bool set_default_payload( Payload it_payload )
        {
			this->gt_dummy = it_payload;
			return false;
        }
        //! @brief Create a child of the Root
		virtual size_t create_child( Payload it_payload )
		{
			return true;
		}

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **  PUBLIC GETTERS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

		//! @brief Return the payload of the Root of the Tree
        virtual Payload &root( bool &oru1_fail )
        {
			oru1_fail = false;
			return this->gt_dummy;
        }
        virtual Payload &root( void )
        {
			return this->gt_dummy;
        }
		//! @brief show the nodes stored inside the vector and their links
		virtual bool show( void )
		{
			std::cout << "Vector content:\n";
			return true;
		}

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

        //! @brief Error code of the class
        const char *gps8_error_code;
		//Dummy payload
        Payload gt_dummy;

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

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE METHODS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/

        /*********************************************************************************************************************************************************
        **********************************************************************************************************************************************************
        **	PRIVATE VARS
        **********************************************************************************************************************************************************
        *********************************************************************************************************************************************************/
};	//End Class: Tree

/**********************************************************************************
**  NAMESPACE
**********************************************************************************/

}	//end namespace: User

#else
    #warning "Multiple inclusion of hader file TREE_INTERFACE_H_"
#endif
