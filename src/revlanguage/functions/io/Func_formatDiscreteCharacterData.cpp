//
//  Func_formatDiscreteCharacterData.cpp
//  revbayes-proj
//
//  Created by Michael Landis on 10/1/16.
//  Copyright © 2016 Michael Landis. All rights reserved.
//

#include "Func_formatDiscreteCharacterData.h"

#include <stddef.h>
#include <string>
#include <vector>

#include "ArgumentRule.h"
#include "Natural.h"
#include "RevNullObject.h"
#include "RlAbstractHomologousDiscreteCharacterData.h"
#include "RlString.h"
#include "HomologousDiscreteCharacterData.h"
#include "BitsetCharacterDataConverter.h"
#include "AbstractHomologousDiscreteCharacterData.h"
#include "Argument.h"
#include "ArgumentRules.h"
#include "RevVariable.h"
#include "RlFunction.h"
#include "TypeSpec.h"

namespace RevBayesCore { class NaturalNumbersState; }
namespace RevBayesCore { class StandardState; }


using namespace RevLanguage;


/**
 * The clone function is a convenience function to create proper copies of inherited objected.
 * E.g. a.clone() will create a clone of the correct type even if 'a' is of derived type 'B'.
 *
 * \return A new copy of myself
 */
Func_formatDiscreteCharacterData* Func_formatDiscreteCharacterData::clone( void ) const
{
    
    return new Func_formatDiscreteCharacterData( *this );
}


/**
 * Execute the function.
 * Here we will extract the character data object from the arguments and get the file name
 * into which we shall write the character data. Then we simply create a FastaWriter
 * instance and delegate the work
 *
 * \return NULL because the output is going into a file
 */
RevPtr<RevVariable> Func_formatDiscreteCharacterData::execute( void )
{
    
    // get the information from the arguments for reading the file
    const RevBayesCore::AbstractHomologousDiscreteCharacterData &data = static_cast< const AbstractHomologousDiscreteCharacterData & >( args[0].getVariable()->getRevObject() ).getValue();
    std::string format = static_cast<const RlString&>( args[1].getVariable()->getRevObject() ).getValue();
    size_t num_states = static_cast<const Natural&>( args[2].getVariable()->getRevObject() ).getValue();
    
    if (format == "DEC") {
        
        // cast off constness
        RevBayesCore::AbstractHomologousDiscreteCharacterData* dataPtr = NULL;
        if (const_cast<RevBayesCore::AbstractHomologousDiscreteCharacterData*>(&data) != NULL) {
        
            dataPtr = const_cast<RevBayesCore::AbstractHomologousDiscreteCharacterData*>(&data);
            
        }
        
        // cast to StandardState
        RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::StandardState>* rawData = NULL;
        if (dynamic_cast<RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::StandardState>* >(dataPtr) != NULL)
        {
            rawData = dynamic_cast<RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::StandardState>* >(dataPtr);
        }
        
        // create converter
        RevBayesCore::BitsetCharacterDataConverter* bcdc = new RevBayesCore::BitsetCharacterDataConverter(*rawData, format, num_states);

        // store converted data
        RevBayesCore::HomologousDiscreteCharacterData<RevBayesCore::NaturalNumbersState>* formattedData = bcdc->convertData();

        // return new converted object
        AbstractHomologousDiscreteCharacterData *rlData = new AbstractHomologousDiscreteCharacterData( formattedData );
        return new RevVariable( rlData );
    }

    return NULL;
}


/**
 * Get the argument rules for this function.
 *
 * The argument rules of the writeCharacterDataDelimited function are:
 * (1) the filename which must be a string.
 * (2) the data object that must be some character matrix.
 *
 * \return The argument rules.
 */
const ArgumentRules& Func_formatDiscreteCharacterData::getArgumentRules( void ) const
{
    
    static ArgumentRules argumentRules = ArgumentRules();
    static bool rulesSet = false;
    
    if (!rulesSet)
    {
        argumentRules.push_back( new ArgumentRule( "data"  , AbstractHomologousDiscreteCharacterData::getClassTypeSpec(), "The character data object.", ArgumentRule::BY_VALUE, ArgumentRule::ANY ) );
        argumentRules.push_back( new ArgumentRule( "format", RlString::getClassTypeSpec(), "The data format.", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new RlString("DEC")) );
        argumentRules.push_back( new ArgumentRule( "numStates", Natural::getClassTypeSpec(), "The number of states (format==\"DEC\" only).", ArgumentRule::BY_VALUE, ArgumentRule::ANY, new Natural(0L)) );
        rulesSet = true;
    }
    
    return argumentRules;
}


/**
 * Get Rev type of object
 *
 * \return The class' name.
 */
const std::string& Func_formatDiscreteCharacterData::getClassType(void)
{
    
    static std::string rev_type = "Func_formatDiscreteCharacterData";
    
    return rev_type;
}


/**
 * Get class type spec describing type of an object from this class (static).
 *
 * \return TypeSpec of this class.
 */
const TypeSpec& Func_formatDiscreteCharacterData::getClassTypeSpec(void)
{
    
    static TypeSpec revTypeSpec = TypeSpec( getClassType(), new TypeSpec( Function::getClassTypeSpec() ) );
    
    return revTypeSpec;
}


/**
 * Get the primary Rev name for this function.
 */
std::string Func_formatDiscreteCharacterData::getFunctionName( void ) const
{
    // create a name variable that is the same for all instance of this class
    std::string f_name = "formatDiscreteCharacterData";
    
    return f_name;
}


/**
 * Get type-specification on this object (non-static).
 *
 * \return The type spec of this object.
 */
const TypeSpec& Func_formatDiscreteCharacterData::getTypeSpec( void ) const
{
    
    static TypeSpec typeSpec = getClassTypeSpec();
    
    return typeSpec;
}


/**
 * Get the return type of the function.
 * This function does not return anything so the return type is NULL.
 *
 * \return NULL
 */
const TypeSpec& Func_formatDiscreteCharacterData::getReturnType( void ) const
{
    
    static TypeSpec return_typeSpec = RevNullObject::getClassTypeSpec();
    return return_typeSpec;
}
