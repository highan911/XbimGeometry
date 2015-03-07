// This file is generated by WOK (CPPExt).
// Please do not edit this file; modify original file instead.
// The copyright and license terms as defined for the original file apply to 
// this header file considered to be the "object code" form of the original source.

#ifndef _GeomFill_SequenceOfAx2_HeaderFile
#define _GeomFill_SequenceOfAx2_HeaderFile

#include <Standard.hxx>
#include <Standard_DefineAlloc.hxx>
#include <Standard_Macro.hxx>

#include <TCollection_BaseSequence.hxx>
#include <Handle_GeomFill_SequenceNodeOfSequenceOfAx2.hxx>
#include <Standard_Integer.hxx>
class Standard_NoSuchObject;
class Standard_OutOfRange;
class gp_Ax2;
class GeomFill_SequenceNodeOfSequenceOfAx2;



class GeomFill_SequenceOfAx2  : public TCollection_BaseSequence
{
public:

  DEFINE_STANDARD_ALLOC

  
    GeomFill_SequenceOfAx2();
  
  Standard_EXPORT GeomFill_SequenceOfAx2(const GeomFill_SequenceOfAx2& Other);
  
  Standard_EXPORT   void Clear() ;
~GeomFill_SequenceOfAx2()
{
  Clear();
}
  
  Standard_EXPORT  const  GeomFill_SequenceOfAx2& Assign (const GeomFill_SequenceOfAx2& Other) ;
 const  GeomFill_SequenceOfAx2& operator = (const GeomFill_SequenceOfAx2& Other) 
{
  return Assign(Other);
}
  
  Standard_EXPORT   void Append (const gp_Ax2& T) ;
  
      void Append (GeomFill_SequenceOfAx2& S) ;
  
  Standard_EXPORT   void Prepend (const gp_Ax2& T) ;
  
      void Prepend (GeomFill_SequenceOfAx2& S) ;
  
      void InsertBefore (const Standard_Integer Index, const gp_Ax2& T) ;
  
      void InsertBefore (const Standard_Integer Index, GeomFill_SequenceOfAx2& S) ;
  
  Standard_EXPORT   void InsertAfter (const Standard_Integer Index, const gp_Ax2& T) ;
  
      void InsertAfter (const Standard_Integer Index, GeomFill_SequenceOfAx2& S) ;
  
  Standard_EXPORT  const  gp_Ax2& First()  const;
  
  Standard_EXPORT  const  gp_Ax2& Last()  const;
  
      void Split (const Standard_Integer Index, GeomFill_SequenceOfAx2& Sub) ;
  
  Standard_EXPORT  const  gp_Ax2& Value (const Standard_Integer Index)  const;
 const  gp_Ax2& operator() (const Standard_Integer Index)  const
{
  return Value(Index);
}
  
  Standard_EXPORT   void SetValue (const Standard_Integer Index, const gp_Ax2& I) ;
  
  Standard_EXPORT   gp_Ax2& ChangeValue (const Standard_Integer Index) ;
  gp_Ax2& operator() (const Standard_Integer Index) 
{
  return ChangeValue(Index);
}
  
  Standard_EXPORT   void Remove (const Standard_Integer Index) ;
  
  Standard_EXPORT   void Remove (const Standard_Integer FromIndex, const Standard_Integer ToIndex) ;




protected:





private:





};

#define SeqItem gp_Ax2
#define SeqItem_hxx <gp_Ax2.hxx>
#define TCollection_SequenceNode GeomFill_SequenceNodeOfSequenceOfAx2
#define TCollection_SequenceNode_hxx <GeomFill_SequenceNodeOfSequenceOfAx2.hxx>
#define Handle_TCollection_SequenceNode Handle_GeomFill_SequenceNodeOfSequenceOfAx2
#define TCollection_SequenceNode_Type_() GeomFill_SequenceNodeOfSequenceOfAx2_Type_()
#define TCollection_Sequence GeomFill_SequenceOfAx2
#define TCollection_Sequence_hxx <GeomFill_SequenceOfAx2.hxx>

#include <TCollection_Sequence.lxx>

#undef SeqItem
#undef SeqItem_hxx
#undef TCollection_SequenceNode
#undef TCollection_SequenceNode_hxx
#undef Handle_TCollection_SequenceNode
#undef TCollection_SequenceNode_Type_
#undef TCollection_Sequence
#undef TCollection_Sequence_hxx




#endif // _GeomFill_SequenceOfAx2_HeaderFile
