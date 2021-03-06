#ifndef RlAbstractHomologousDiscreteCharacterData_H
#define RlAbstractHomologousDiscreteCharacterData_H

#include <string>
#include <vector>
#include <iosfwd>

#include "AbstractHomologousDiscreteCharacterData.h"
#include "ModelObject.h"
#include "RlHomologousCharacterData.h"
#include "ConstantNode.h"
#include "DagNode.h"
#include "DeterministicNode.h"
#include "DynamicNode.h"
#include "IndirectReferenceFunction.h"
#include "RevObject.h"
#include "RevPtr.h"
#include "RlConstantNode.h"
#include "TypedDagNode.h"
#include "TypedFunction.h"
#include "UserFunctionNode.h"


namespace RevLanguage {
class Argument;
class RevVariable;
class TypeSpec;
    
    
    class AbstractHomologousDiscreteCharacterData : public ModelObject<RevBayesCore::AbstractHomologousDiscreteCharacterData>, public HomologousCharacterData {
        
    public:
        AbstractHomologousDiscreteCharacterData(void);                                                                                                  //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(const RevBayesCore::AbstractHomologousDiscreteCharacterData &d);                                              //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(RevBayesCore::AbstractHomologousDiscreteCharacterData *d);                                              //!< Constructor requires character type
        AbstractHomologousDiscreteCharacterData(RevBayesCore::TypedDagNode<RevBayesCore::AbstractHomologousDiscreteCharacterData>*d);                   //!< Constructor requires character type
        
        virtual ~AbstractHomologousDiscreteCharacterData();
        
        
            // Basic utility functions
        virtual AbstractHomologousDiscreteCharacterData*        clone(void) const;                                                                      //!< Clone object
        static const std::string&                               getClassType(void);                                                                     //!< Get Rev type
        static const TypeSpec&                                  getClassTypeSpec(void);                                                                 //!< Get class type spec
        virtual const TypeSpec&                                 getTypeSpec(void) const;                                                                //!< Get language type of the object
        
            // Member method functions
        virtual RevPtr<RevVariable>                             executeMethod(const std::string& name, const std::vector<Argument>& args, bool &f);     //!< Override to map member methods to internal functions
        
        void                                                    concatenate(const RevObject& d, std::string type="") const;                             //!< Concatenate two sequences
        void                                                    concatenate(const AbstractHomologousDiscreteCharacterData& d, std::string type="") const;//!< Concatenate two sequences
        
        
    private:
        
        void                                                    initMethods(void);
        
    };
    
}


#endif

