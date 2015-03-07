// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _BRepClass3d_DataMapIteratorOfMapOfInter_HeaderFile
#define _BRepClass3d_DataMapIteratorOfMapOfInter_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BasicMapIterator.hxx>
#include <Standard_Address.hxx>
#include <Handle_BRepClass3d_DataMapNodeOfMapOfInter.hxx>
class Standard_NoSuchObject;
class TopoDS_Shape;
class TopTools_ShapeMapHasher;
class BRepClass3d_MapOfInter;
class BRepClass3d_DataMapNodeOfMapOfInter;



class BRepClass3d_DataMapIteratorOfMapOfInter  : public TCollection_BasicMapIterator
{
public:

  DEFINE_STANDARD_ALLOC

  
  Standard_EXPORT BRepClass3d_DataMapIteratorOfMapOfInter();
  
  Standard_EXPORT BRepClass3d_DataMapIteratorOfMapOfInter(const BRepClass3d_MapOfInter& aMap);
  
  Standard_EXPORT   void Initialize (const BRepClass3d_MapOfInter& aMap) ;
  
  Standard_EXPORT  const  TopoDS_Shape& Key()  const;
  
  Standard_EXPORT  const  Standard_Address& Value()  const;




protected:





private:





};







#endif // _BRepClass3d_DataMapIteratorOfMapOfInter_HeaderFile
