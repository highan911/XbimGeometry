// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepOffset_Interval_HeaderFile
#define _BRepOffset_Interval_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <Standard_Real.hxx>
#include <BRepOffset_Type.hxx>



class BRepOffset_Interval 
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT BRepOffset_Interval();
  
  Standard_EXPORT BRepOffset_Interval(const Standard_Real U1, const Standard_Real U2, const BRepOffset_Type Type);
  
      void First (const Standard_Real U) ;
  
      void Last (const Standard_Real U) ;
  
      void Type (const BRepOffset_Type T) ;
  
      Standard_Real First()  const;
  
      Standard_Real Last()  const;
  
      BRepOffset_Type Type()  const;




protected:





private:



  Standard_Real f;
  Standard_Real l;
  BRepOffset_Type type;


};


#include <BRepOffset_Interval.lxx>





#endif // _BRepOffset_Interval_HeaderFile
