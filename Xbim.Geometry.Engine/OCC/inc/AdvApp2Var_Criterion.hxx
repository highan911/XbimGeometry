// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _AdvApp2Var_Criterion_HeaderFile
#define _AdvApp2Var_Criterion_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Real.hxx>
#include <AdvApp2Var_CriterionType.hxx>
#include <AdvApp2Var_CriterionRepartition.hxx>
#include <Standard_Boolean.hxx>
class AdvApp2Var_Patch;
class AdvApp2Var_Context;



//! this class contains a given criterion to be satisfied
class AdvApp2Var_Criterion 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT virtual   void Delete() ;
Standard_EXPORT virtual ~AdvApp2Var_Criterion(){Delete() ; }
  
  Standard_EXPORT virtual   void Value (AdvApp2Var_Patch& P, const AdvApp2Var_Context& C)  const = 0;
  
  Standard_EXPORT virtual   Standard_Boolean IsSatisfied (const AdvApp2Var_Patch& P)  const = 0;
  
  Standard_EXPORT   Standard_Real MaxValue()  const;
  
  Standard_EXPORT   AdvApp2Var_CriterionType Type()  const;
  
  Standard_EXPORT   AdvApp2Var_CriterionRepartition Repartition()  const;




protected:



  Standard_Real myMaxValue;
  AdvApp2Var_CriterionType myType;
  AdvApp2Var_CriterionRepartition myRepartition;


private:





};







#endif // _AdvApp2Var_Criterion_HeaderFile
