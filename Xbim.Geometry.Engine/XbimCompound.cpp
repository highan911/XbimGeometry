#include "XbimCompound.h"
#include "XbimGeometryCreator.h"
#include "XbimGeometryObjectSet.h"
#include "XbimOccWriter.h"
#include "XbimSolidSet.h"
#include "XbimShellSet.h"
#include "XbimFaceSet.h"
#include "XbimEdgeSet.h"
#include "XbimVertexSet.h"
#include "XbimConvert.h"
#include "XbimGeometryObjectSet.h"
#include <BRep_Builder.hxx>
#include <BRepBuilderAPI_Sewing.hxx>
#include <BRepBuilderAPI_FastSewing.hxx>
#include <TopTools_ListIteratorOfListOfShape.hxx>
#include <TopExp.hxx>
#include <BRepBuilderAPI_MakePolygon.hxx>
#include <TopTools_IndexedMapOfShape.hxx>
#include <TopExp.hxx>
#include <BRepPrim_Builder.hxx>
#include <ShapeFix_ShapeTolerance.hxx>
#include <BRepAlgoAPI_Cut.hxx>
#include <BRepAlgoAPI_Fuse.hxx>
#include <BRepAlgoAPI_Common.hxx>
#include <BRepBuilderAPI_MakeSolid.hxx>
#include <Bnd_Box.hxx>
#include <BRepBndLib.hxx>
#include <BRepTools.hxx>
#include <BRepBuilderAPI_Transform.hxx>
#include <BRepAlgoAPI_Check.hxx>
#include <ShapeFix_Shape.hxx>
#include <GProp_GProps.hxx>
#include <BRepGProp.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <ShapeFix_Shell.hxx>
using namespace System;
using namespace System::Linq;
using namespace Xbim::Common; 
using namespace Xbim::Common::XbimExtensions;
namespace Xbim
{
	namespace Geometry
	{

		XbimCompound::XbimCompound(double sewingTolerance)
		{
			_sewingTolerance = sewingTolerance;
		}

		/*Ensures native pointers are deleted and garbage collected*/
		void XbimCompound::InstanceCleanup()
		{
			IntPtr temp = System::Threading::Interlocked::Exchange(ptrContainer, IntPtr::Zero);
			if (temp != IntPtr::Zero)			
				delete (TopoDS_Compound*)(temp.ToPointer());
			System::GC::SuppressFinalize(this);
		}

		IEnumerator<IXbimGeometryObject^>^ XbimCompound::GetEnumerator()
		{
			//add all top level objects in to the collection, ignore nested objects
			List<IXbimGeometryObject^>^ result = gcnew List<IXbimGeometryObject^>(1);
			if (!IsValid) return result->GetEnumerator();
			for (TopExp_Explorer expl(*pCompound, TopAbs_SOLID); expl.More(); expl.Next())
				result->Add(gcnew XbimSolid(TopoDS::Solid(expl.Current())));
			for (TopExp_Explorer expl(*pCompound, TopAbs_SHELL, TopAbs_SOLID); expl.More(); expl.Next())
				result->Add(gcnew XbimShell(TopoDS::Shell(expl.Current())));
			for (TopExp_Explorer expl(*pCompound, TopAbs_FACE, TopAbs_SHELL); expl.More(); expl.Next())
				result->Add(gcnew XbimFace(TopoDS::Face(expl.Current())));
			return result->GetEnumerator();
		}

		int XbimCompound::Count::get()
		{
			if (pCompound == nullptr) return 0;
			int count = 0;
			for (TopExp_Explorer expl(*pCompound, TopAbs_SOLID); expl.More(); expl.Next())
				count++;
			for (TopExp_Explorer expl(*pCompound, TopAbs_SHELL, TopAbs_SOLID); expl.More(); expl.Next())
				count++;
			for (TopExp_Explorer expl(*pCompound, TopAbs_FACE, TopAbs_SHELL); expl.More(); expl.Next())
				count++;
			return count;
		}

		IXbimGeometryObject^ XbimCompound::Transform(XbimMatrix3D matrix3D)
		{
			BRepBuilderAPI_Copy copier(this);
			BRepBuilderAPI_Transform gTran(copier.Shape(), XbimConvert::ToTransform(matrix3D));
			TopoDS_Compound temp = TopoDS::Compound(gTran.Shape());
			GC::KeepAlive(this);
			return gcnew XbimCompound(temp, IsSewn, _sewingTolerance);
		}
		
		IXbimGeometryObject^ XbimCompound::TransformShallow(XbimMatrix3D matrix3D)
		{
			TopoDS_Compound shallowCopy = TopoDS::Compound(pCompound->Moved(XbimConvert::ToTransform(matrix3D)));
			GC::KeepAlive(this);
			return gcnew XbimCompound(shallowCopy, IsSewn, _sewingTolerance);
		}

		XbimRect3D XbimCompound::BoundingBox::get()
		{
			if (pCompound == nullptr)return XbimRect3D::Empty;
			Bnd_Box pBox;
			BRepBndLib::Add(*pCompound, pBox);
			Standard_Real srXmin, srYmin, srZmin, srXmax, srYmax, srZmax;
			if (pBox.IsVoid()) return XbimRect3D::Empty;
			pBox.Get(srXmin, srYmin, srZmin, srXmax, srYmax, srZmax);
			return XbimRect3D(srXmin, srYmin, srZmin, (srXmax - srXmin), (srYmax - srYmin), (srZmax - srZmin));
		}

		IXbimGeometryObject^ XbimCompound::First::get()
		{
			if (!IsValid) return nullptr;
			for (TopExp_Explorer expl(*pCompound, TopAbs_SOLID); expl.More(); expl.Next())
				return gcnew XbimSolid(TopoDS::Solid(expl.Current()));
			for (TopExp_Explorer expl(*pCompound, TopAbs_SHELL, TopAbs_SOLID); expl.More(); expl.Next())
				return gcnew XbimShell(TopoDS::Shell(expl.Current()));
			for (TopExp_Explorer expl(*pCompound, TopAbs_FACE, TopAbs_SHELL); expl.More(); expl.Next())
				return gcnew XbimFace(TopoDS::Face(expl.Current()));
			return nullptr;
		}


		XbimCompound::XbimCompound(IIfcConnectedFaceSet^ faceSet)
		{
			_sewingTolerance = faceSet->Model->ModelFactors->Precision;
			Init(faceSet); 
		}

		XbimCompound::XbimCompound(IIfcShellBasedSurfaceModel^ sbsm)
		{
			_sewingTolerance = sbsm->Model->ModelFactors->Precision;
			Init(sbsm); 
		}

		XbimCompound::XbimCompound(IIfcFaceBasedSurfaceModel^ fbsm)
		{
			_sewingTolerance = fbsm->Model->ModelFactors->Precision;
			Init(fbsm); 
		}

		XbimCompound::XbimCompound(IIfcManifoldSolidBrep^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}
		XbimCompound::XbimCompound(IIfcFacetedBrep^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}

		XbimCompound::XbimCompound(IIfcFacetedBrepWithVoids^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}
		XbimCompound::XbimCompound(IIfcAdvancedBrep^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}

		XbimCompound::XbimCompound(IIfcAdvancedBrepWithVoids^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}

		XbimCompound::XbimCompound(IIfcClosedShell^ solid)
		{
			_sewingTolerance = solid->Model->ModelFactors->Precision;
			Init(solid);
		}

		XbimCompound::XbimCompound(const TopoDS_Compound& compound, bool sewn, double tolerance)
		{
			pCompound = new TopoDS_Compound();
			*pCompound = compound;
			_isSewn = sewn;
			_sewingTolerance = tolerance;
			
		}

		void XbimCompound::Move(IIfcAxis2Placement3D^ position)
		{
			if (!IsValid) return;
			gp_Trsf toPos = XbimConvert::ToTransform(position);
			pCompound->Move(toPos);
		}

		XbimCompound::XbimCompound(IIfcTriangulatedFaceSet^ faceSet)
		{
			_sewingTolerance = faceSet->Model->ModelFactors->Precision;
			Init(faceSet);
		}


#pragma region Initialisers

		void XbimCompound::Init(IIfcFaceBasedSurfaceModel^ fbsm)
		{
			pCompound = new TopoDS_Compound();
			BRep_Builder builder;
			builder.MakeCompound(*pCompound);
			for each (IIfcConnectedFaceSet^ faceSet in fbsm->FbsmFaces)
			{
				XbimCompound^ compound = gcnew XbimCompound(faceSet);
				for each (IXbimGeometryObject^ geom in compound)
				{
					if (dynamic_cast<XbimSolid^>(geom))
						builder.Add(*pCompound, (XbimSolid^)geom);
					else if (dynamic_cast<XbimShell^>(geom))
					{
						XbimShell^ shell = (XbimShell^)geom;
						if (shell->IsClosed)
						{
							XbimSolid^ solid = (XbimSolid^)shell->MakeSolid();
							if (solid->IsValid)
							{
								builder.Add(*pCompound, solid);
								continue;
							}
						}
						builder.Add(*pCompound, shell);
					}
					else if (dynamic_cast<XbimFace^>(geom))
						builder.Add(*pCompound, (XbimFace^)geom);
				}
			}
		}

		void XbimCompound::Init(IIfcShellBasedSurfaceModel^ sbsm)
		{
			List<IIfcFace^>^ faces = gcnew List<IIfcFace^>();
			for each (IIfcShell^ shell in sbsm->SbsmBoundary)
			{
				//get the faces
				IIfcConnectedFaceSet^ faceSet = dynamic_cast<IIfcConnectedFaceSet^>(shell);
				if (faceSet != nullptr) //this should never fail
					faces->AddRange(faceSet->CfsFaces);
			}
			Init(faces);
			for each (IXbimGeometryObject^ geomObj in this)
			{
				XbimShell^ shell = dynamic_cast<XbimShell^>(geomObj);
				if (shell != nullptr) shell->Orientate();
			}

		}

		void XbimCompound::Init(IIfcConnectedFaceSet^ faceSet)
		{
			
			if (!Enumerable::Any(faceSet->CfsFaces))
			{
				XbimGeometryCreator::logger->WarnFormat("WC001: IIfcConnectedFaceSet #{0}, is empty", faceSet->EntityLabel);
				return;
			}
			Init(faceSet->CfsFaces);

		}


		void XbimCompound::Init(IIfcManifoldSolidBrep^ solid)
		{
			IIfcFacetedBrep^ facetedBrep = dynamic_cast<IIfcFacetedBrep^>(solid);
			if (facetedBrep != nullptr) return Init(facetedBrep);

			IIfcAdvancedBrep^ advancedBrep = dynamic_cast<IIfcAdvancedBrep^>(solid);
			if (advancedBrep != nullptr) return Init(advancedBrep);


			
			throw gcnew NotImplementedException("Sub-Type of IIfcManifoldSolidBrep is not implemented");
		}

		void XbimCompound::Init(IIfcAdvancedBrep^ solid)
		{

			IIfcAdvancedBrepWithVoids^ advancedBrepWithVoids = dynamic_cast<IIfcAdvancedBrepWithVoids^>(solid);
			if (advancedBrepWithVoids != nullptr) return Init(advancedBrepWithVoids);
			InitAdvancedFaces(solid->Outer->CfsFaces);
		}

		void XbimCompound::Init(IIfcFacetedBrep^ solid)
		{
			IIfcFacetedBrepWithVoids^ facetedBrepWithVoids = dynamic_cast<IIfcFacetedBrepWithVoids^>(solid);
			if (facetedBrepWithVoids != nullptr) return Init(facetedBrepWithVoids);
			Init(solid->Outer);
		}

		void XbimCompound::Init(IIfcAdvancedBrepWithVoids^ brepWithVoids)
		{
			InitAdvancedFaces(brepWithVoids->Outer->CfsFaces);
			XbimShell^ outerShell = (XbimShell^)MakeShell();
			if (!outerShell->IsClosed) //we have a shell that is not able to be made in to a solid
				XbimGeometryCreator::logger->ErrorFormat("ES004: An IIfcClosedShell #{0} has been found that is not topologically correct. ", brepWithVoids->Outer->EntityLabel);
			BRepBuilderAPI_MakeSolid builder(outerShell);
			for each (IIfcClosedShell^ IIfcVoidShell in brepWithVoids->Voids)
			{
				XbimCompound^ voidShapes = gcnew XbimCompound(IIfcVoidShell);
				XbimShell^ voidShell = (XbimShell^)voidShapes->MakeShell();
				if (!voidShell->IsClosed) //we have a shell that is not able to be made in to a solid
					XbimGeometryCreator::logger->ErrorFormat("ES004: An IIfcClosedShell #{0} has been found that is not topologically correct. ", IIfcVoidShell->EntityLabel);
				builder.Add(voidShell);
			}
			if (builder.IsDone())
			{
				//pCompound = new TopoDS_Compound(); no need to do this as it is already completed
				BRep_Builder b;
				b.MakeCompound(*pCompound);
				b.Add(*pCompound, builder.Solid());
			}//leave the outer shell without the voids
			else 
				XbimGeometryCreator::logger->ErrorFormat("ES003: An incorrectly defined IIfcFacetedBrepWithVoids #{0} has been found, a correct shape could not be built and it has been ignored", brepWithVoids->EntityLabel);
		}

		void XbimCompound::Init(IIfcFacetedBrepWithVoids^ brepWithVoids)
		{
			XbimCompound^ shapes = gcnew XbimCompound(brepWithVoids->Outer);
			XbimShell^ outerShell = (XbimShell^)shapes->MakeShell();
			if (!outerShell->IsClosed) //we have a shell that is not able to be made in to a solid
				XbimGeometryCreator::logger->ErrorFormat("ES004: An IIfcClosedShell #{0} has been found that is not topologically correct. ", brepWithVoids->Outer->EntityLabel);
			BRepBuilderAPI_MakeSolid builder(outerShell);
			for each (IIfcClosedShell^ IIfcVoidShell in brepWithVoids->Voids)
			{
				XbimCompound^ voidShapes = gcnew XbimCompound(IIfcVoidShell);
				XbimShell^ voidShell = (XbimShell^)voidShapes->MakeShell();
				if (!voidShell->IsClosed) //we have a shell that is not able to be made in to a solid
					XbimGeometryCreator::logger->ErrorFormat("ES004: An IIfcClosedShell #{0} has been found that is not topologically correct. ", IIfcVoidShell->EntityLabel);
				builder.Add(voidShell);
			}
			if (builder.IsDone())
			{
				pCompound = new TopoDS_Compound();
				BRep_Builder b;
				b.MakeCompound(*pCompound);
				b.Add(*pCompound, builder.Solid());
			}
			else
				XbimGeometryCreator::logger->ErrorFormat("ES003: An incorrectly defined IIfcFacetedBrepWithVoids #{0} has been found, a correct shape could not be built and it has been ignored", brepWithVoids->EntityLabel);
		}

		void XbimCompound::Init(IIfcClosedShell^ closedShell)
		{
			Init((IIfcConnectedFaceSet^)closedShell);
		}

		bool XbimCompound::Sew()
		{

			if (!IsValid || IsSewn)
				return true;
			long tally = 0;
			for (TopExp_Explorer expl(*pCompound, TopAbs_FACE); expl.More(); expl.Next())
			{
				tally++;
				if (tally > MaxFacesToSew) //give up if too many
					return false;
			}

			BRep_Builder builder;
			TopoDS_Compound newCompound;
			builder.MakeCompound(newCompound);
			for (TopExp_Explorer expl(*pCompound, TopAbs_SHELL); expl.More(); expl.Next())
			{
				BRepBuilderAPI_Sewing seamstress(_sewingTolerance);
				seamstress.Add(expl.Current());
				seamstress.Perform();
				TopoDS_Shape result = seamstress.SewedShape();
				builder.Add(newCompound, result);
			}

			*pCompound = newCompound;



			_isSewn = true;
			GC::KeepAlive(this);
			return true;
		}

		double XbimCompound::Volume::get()
		{
			if (IsValid)
			{
				GProp_GProps gProps;
				BRepGProp::VolumeProperties(*pCompound, gProps, Standard_True);
				GC::KeepAlive(this);
				return gProps.Mass();
			}
			else
				return 0;
		}
		//This method copes with faces that may be advanced as well as ordinary
		void XbimCompound::InitAdvancedFaces(IEnumerable<IIfcFace^>^ faces)
		{
			try
			{
				int f = 0;
				XbimOccWriter^ occWriter = gcnew XbimOccWriter();
				BRepPrim_Builder builder;
				TopoDS_Shell shell;
				builder.MakeShell(shell);
				IIfcFace^ aFace = Enumerable::FirstOrDefault(faces);
				if (aFace == nullptr) return;
				IModel^ model = aFace->Model;

				_sewingTolerance = model->ModelFactors->Precision;
				//collect all the geometry components
				Dictionary<int, XbimEdge^>^ edgeCurves = gcnew Dictionary<int, XbimEdge^>();
				Dictionary<int, XbimVertex^>^ vertices = gcnew Dictionary<int, XbimVertex^>();
				for each (IIfcFace^ unloadedFace in  faces)
				{
					IIfcAdvancedFace^ advancedFace = dynamic_cast<IIfcAdvancedFace^>(model->Instances[unloadedFace->EntityLabel]); //improves performance and reduces memory load				

					XbimWire^ outerLoop = nullptr;
					List<XbimWire^>^ innerLoops = gcnew List<XbimWire^>();
					for each (IIfcFaceBound^ ifcBound in advancedFace->Bounds) //build all the loops
					{
						BRepBuilderAPI_MakeWire wireMaker;
						bool isOuter = dynamic_cast<IIfcFaceOuterBound^>(ifcBound) != nullptr;
						IIfcEdgeLoop^ edgeLoop = dynamic_cast<IIfcEdgeLoop^>(ifcBound->Bound);
						if (edgeLoop != nullptr) //they always should be
						{
							XbimWire^ w1;
							for each (IIfcOrientedEdge^ orientedEdge in edgeLoop->EdgeList)
							{
								IIfcEdgeCurve^ edgeCurve = dynamic_cast<IIfcEdgeCurve^>(orientedEdge->EdgeElement);
								if (edgeCurve == nullptr) throw gcnew XbimException("Incorrectly defined Edge, must be an edge curve");
								XbimVertex^ edgeStart;
								if (!vertices->TryGetValue(edgeCurve->EdgeStart->EntityLabel, edgeStart))
								{
									IIfcCartesianPoint^ startPoint = ((IIfcCartesianPoint ^)((IIfcVertexPoint^)edgeCurve->EdgeStart)->VertexGeometry);
									edgeStart = gcnew XbimVertex(startPoint);
									vertices->Add(edgeCurve->EdgeStart->EntityLabel, edgeStart);
								}
								XbimVertex^ edgeEnd;
								if (!vertices->TryGetValue(edgeCurve->EdgeEnd->EntityLabel, edgeEnd))
								{
									IIfcCartesianPoint^ endPoint = ((IIfcCartesianPoint ^)((IIfcVertexPoint^)edgeCurve->EdgeEnd)->VertexGeometry);
									edgeEnd = gcnew XbimVertex(endPoint);
									vertices->Add(edgeCurve->EdgeEnd->EntityLabel, edgeEnd);
								}
								
								//opencascade does not support edges made of multi-linear segments (polyline)
								//these can be exapnded to discrete edges to maintain topological correctness
								IIfcPolyline^ polyline = dynamic_cast<IIfcPolyline^>(edgeCurve->EdgeGeometry);
								if (polyline!=nullptr && Enumerable::Count(polyline->Points) > 2) //we have multi segments
								{ 
									XbimWire^ wire = gcnew XbimWire(polyline);
									wire = wire->Trim(edgeStart, edgeEnd, _sewingTolerance);
									
									if (!orientedEdge->Orientation)
										wire->Reverse();
									for each (XbimEdge^ edge in wire->Edges)
									{
										wireMaker.Add(edge);
									}
								}
								else
								{
									
									XbimEdge^ xBimEdgeCurve;
									if (!edgeCurves->TryGetValue(edgeCurve->EntityLabel, xBimEdgeCurve))
									{
										xBimEdgeCurve = gcnew XbimEdge(edgeCurve->EdgeGeometry, edgeStart, edgeEnd);
										if (!edgeCurve->SameSense)
											xBimEdgeCurve->Reverse();
										edgeCurves->Add(edgeCurve->EntityLabel, xBimEdgeCurve);
									}								
									if (!orientedEdge->Orientation)
										xBimEdgeCurve = xBimEdgeCurve->Reversed();
									wireMaker.Add(xBimEdgeCurve);
								}

							} // we have a wire

							
							TopoDS_Wire loopWire = wireMaker.Wire();
							if (!ifcBound->Orientation) loopWire.Reverse();
							XbimWire^ xbimLoop = gcnew XbimWire(loopWire);
							if (isOuter && outerLoop == nullptr) //only choose one outer loop
								outerLoop = xbimLoop;
							else
								innerLoops->Add(xbimLoop);
						}
					}
					//if we have no outer loop defined, find the longest
					if (outerLoop == nullptr)
					{
						double area = 0;
						for each (XbimWire^ innerLoop in innerLoops)
						{
							double loopArea = innerLoop->Area;
							if (loopArea > area)
							{
								outerLoop = innerLoop;
								area = loopArea;
							}
						}
						innerLoops->Remove(outerLoop); //remove outer loop from inner loops
					}
					
					XbimFace^ xbimAdvancedFace = gcnew XbimFace(advancedFace, outerLoop, innerLoops);

					if (xbimAdvancedFace->IsValid)
					{
						/*String^ fName = "C:\\tmp\\f" + f;
						occWriter->Write(xbimAdvancedFace, fName);*/
						builder.AddShellFace(shell, xbimAdvancedFace);
						f++;
					}
					else
						XbimGeometryCreator::logger->InfoFormat("WC002: Incorrectly defined IIfcFace #{0}, it has been ignored", advancedFace->EntityLabel);
				}

				builder.CompleteShell(shell);

				BRepBuilderAPI_Sewing seamstress(_sewingTolerance);
				seamstress.Add(shell);
				seamstress.Perform();
				TopoDS_Shape result = seamstress.SewedShape();
				ShapeFix_Shape shapeFixer(result);
				shapeFixer.Perform();
				BRep_Builder b;
				pCompound = new TopoDS_Compound();
				b.MakeCompound(*pCompound);
				b.Add(*pCompound, shapeFixer.Shape());
			}
			catch (Standard_Failure e)
			{
				String^ err = gcnew String(Standard_Failure::Caught()->GetMessageString());
				XbimGeometryCreator::logger->WarnFormat("WC035: IfcAdvanced Face Build failed. " + err);
			}
		}


		void  XbimCompound::Init(IIfcTriangulatedFaceSet^ faceSet)
		{
			BRepPrim_Builder builder;
			TopoDS_Shell shell;
			builder.MakeShell(shell);
			//create a list of all the vertices
			List<XbimVertex^>^ vertices = gcnew List<XbimVertex^>(Enumerable::Count(faceSet->Coordinates->CoordList));
			for each (IEnumerable<Ifc4::MeasureResource::IfcLengthMeasure>^ cp in faceSet->Coordinates->CoordList)
			{
				XbimTriplet<Ifc4::MeasureResource::IfcLengthMeasure> tpl = IEnumerableExtensions::AsTriplet<Ifc4::MeasureResource::IfcLengthMeasure>(cp);
				XbimVertex^ v = gcnew XbimVertex(tpl.A,tpl.B, tpl.C,_sewingTolerance);
				vertices->Add(v);
			}
			//ignore the normals as we cannot observe them in an opencascade model, we will recalculate
			

			//make the triangles
			for each (IEnumerable<Ifc4::MeasureResource::IfcPositiveInteger>^ indices in faceSet->CoordIndex)
			{
				XbimTriplet<Ifc4::MeasureResource::IfcPositiveInteger> tpl = IEnumerableExtensions::AsTriplet<Ifc4::MeasureResource::IfcPositiveInteger>(indices);
				
				XbimVertex^ v1; XbimVertex^ v2; XbimVertex^ v3;
				v1 = vertices[(int)tpl.A - 1]; 
				v2 = vertices[(int)tpl.B - 1]; 
				v3 = vertices[(int)tpl.C - 1];
				BRepBuilderAPI_MakePolygon triangleMaker(v1,v2,v3,Standard_True);
				if (triangleMaker.IsDone())
				{
					BRepBuilderAPI_MakeFace faceMaker(triangleMaker.Wire(),Standard_True);
					if (faceMaker.IsDone())
					{
						builder.AddShellFace(shell, faceMaker.Face());
					}
				}
			}
			BRep_Builder b;
			pCompound = new TopoDS_Compound();
			b.MakeCompound(*pCompound);
			BRepBuilderAPI_Sewing seamstress(_sewingTolerance);
			seamstress.Add(shell);
			seamstress.Perform();
			_isSewn = true;
			TopoDS_Shape result = seamstress.SewedShape();
			b.Add(*pCompound, result);
		}


		void XbimCompound::Init(IEnumerable<IIfcFace^>^ faces)
		{
			double tolerance;
			Xbim::Common::IModel^ model;
			for each (IIfcFace^ face in faces)
			{
				model = face->Model;
				tolerance = model->ModelFactors->Precision;
				_sewingTolerance = model->ModelFactors->Precision;			
				break;
			}
			
			
			BRep_Builder builder;
			TopoDS_Shell shell;
			builder.MakeShell(shell);
			TopTools_DataMapOfIntegerShape vertexStore;
			for each (IIfcFace^ unloadedFace in  faces)
			{
				IIfcFace^ fc = (IIfcFace^) model->Instances[unloadedFace->EntityLabel]; //improves performance and reduces memory load
				List<Tuple<XbimWire^, IIfcPolyLoop^>^>^ loops = gcnew List<Tuple<XbimWire^, IIfcPolyLoop^>^>();
				for each (IIfcFaceBound^ bound in fc->Bounds) //build all the loops
				{
					
					if (!dynamic_cast<IIfcPolyLoop^>(bound->Bound) || !XbimConvert::IsPolygon((IIfcPolyLoop^)bound->Bound)) continue;//skip non-polygonal faces
					IIfcPolyLoop^ polyLoop = (IIfcPolyLoop^)(bound->Bound);
					bool is3D = XbimConvert::Is3D(polyLoop);
					BRepBuilderAPI_MakePolygon polyMaker;
					for each (IIfcCartesianPoint^ p in polyLoop->Polygon) //add all the points into unique collection
					{
						TopoDS_Vertex v;
						if (!vertexStore.IsBound(p->EntityLabel))
						{
							builder.MakeVertex(v, gp_Pnt(p->X, p->Y, is3D ? p->Z : 0), tolerance);
							vertexStore.Bind(p->EntityLabel, v);
						}
						else
							v = TopoDS::Vertex(vertexStore.Find(p->EntityLabel));
						polyMaker.Add(v);
					}
					
					if (polyMaker.IsDone())
					{
						polyMaker.Close();
						XbimWire^ loop = gcnew XbimWire(polyMaker.Wire());
						if (loop->IsValid)
						{
							if (!bound->Orientation)
								loop->Reverse(); 
							loops->Add(gcnew Tuple<XbimWire^, IIfcPolyLoop^>(loop, polyLoop));
						}
					}
				
				}
				XbimFace^ face = BuildFace(loops, fc->EntityLabel);
				for each (Tuple<XbimWire^, IIfcPolyLoop^>^ loop in loops) delete loop->Item1; //force removal of wires
				if (face->IsValid)
					builder.Add(shell, face);
				else
					XbimGeometryCreator::logger->InfoFormat("WC002: Incorrectly defined IIfcFace #{0}, it has been ignored", fc->EntityLabel);
				//delete face;
			}
		
			pCompound = new TopoDS_Compound();
			builder.MakeCompound(*pCompound);
			builder.Add(*pCompound, shell);
			
		}


#pragma endregion


#pragma region Helpers


		
		XbimFace^ XbimCompound::BuildFace(List<Tuple<XbimWire^, IIfcPolyLoop^>^>^ wires, int label)
		{

			if (wires->Count == 0) return gcnew XbimFace();
			IIfcCartesianPoint^ first = Enumerable::First(wires[0]->Item2->Polygon);
			XbimPoint3D p(first->X, first->Y, first->Z);
			XbimVector3D n = XbimConvert::NewellsNormal(wires[0]->Item2);
			XbimFace^ face = gcnew XbimFace(wires[0]->Item1, p, n);
			if (wires->Count == 1) return face; //take the first one

			for (int i = 1; i < wires->Count; i++) face->Add(wires[i]->Item1);
			IXbimWire^ outerBound = face->OuterBound;
			XbimVector3D faceNormal;// = outerBound->Normal;
			for each (Tuple<XbimWire^, IIfcPolyLoop^>^ wire in wires)
			{
				if (wire->Item1->Equals(outerBound))
				{
					faceNormal = XbimConvert::NewellsNormal(wire->Item2);
					break;
				}
			}
		
			face = gcnew XbimFace(outerBound, p, faceNormal); //create  a face with the right bound and direction

			for (int i = 1; i < wires->Count; i++)
			{
				XbimWire^ wire = wires[i]->Item1;
				if (!wire->Equals(outerBound))
				{
					XbimVector3D loopNormal = XbimConvert::NewellsNormal(wires[i]->Item2);
					if (faceNormal.DotProduct(loopNormal) > 0) //they should be in opposite directions, so reverse
						wire->Reverse();
					if (!face->Add(wire))
						XbimGeometryCreator::logger->WarnFormat("WC003: Failed to add an inner bound to IIfcFace #{0}", label);
				}
			}
			return face;
		}

		
		//upgrades the result to the highest level and simplest object without loss of representation
		IXbimGeometryObject^ XbimCompound::Upgrade()
		{
			if (!IsValid) return this;
			//upgrade all shells to solids if we can
			BRep_Builder builder;
			TopoDS_Compound newCompound;
			builder.MakeCompound(newCompound);
			int count = 0;
			TopoDS_Shape lastAdded;
			for (TopExp_Explorer expl(*pCompound, TopAbs_SOLID); expl.More(); expl.Next())
			{
				lastAdded = expl.Current();
				builder.Add(newCompound, TopoDS::Solid(lastAdded));
				count++;
			}
			for (TopExp_Explorer expl(*pCompound, TopAbs_SHELL, TopAbs_SOLID); expl.More(); expl.Next())
			{
				lastAdded = expl.Current();
				XbimShell^ shell = gcnew XbimShell(TopoDS::Shell(lastAdded));
				if (shell->IsClosed)
				{
					XbimSolid^ solid = (XbimSolid^)shell->MakeSolid();
					if (solid->IsValid)
						builder.Add(newCompound,  solid);
					else
						builder.Add(newCompound, TopoDS::Shell(expl.Current()));
				}
				else
					builder.Add(newCompound, TopoDS::Shell(expl.Current()));	
				count++;
			}
			for (TopExp_Explorer expl(*pCompound, TopAbs_FACE, TopAbs_SHELL); expl.More(); expl.Next())
			{
				lastAdded = expl.Current();
				builder.Add(newCompound, TopoDS::Face(lastAdded));
				count++;
			}

			if (count == 1)
			{				
				TopAbs_ShapeEnum st = lastAdded.ShapeType();
				if (st == TopAbs_SOLID)
					return gcnew XbimSolid(TopoDS::Solid(lastAdded));
				else if (st == TopAbs_SHELL)
					return gcnew XbimShell(TopoDS::Shell(lastAdded));
				else if (st == TopAbs_FACE)
					return gcnew XbimFace(TopoDS::Face(lastAdded));
			}
			return gcnew XbimCompound(newCompound, IsSewn, _sewingTolerance); //return the upgraded compound
		}

		//Makes all the faces in the compound in to a single shell, does not performa nay form of sewing
		IXbimShell^ XbimCompound::MakeShell()
		{
			if (Count == 1) //if we have one shell or a solid with just one shell then just return it
			{
				IXbimGeometryObject^ geom = this->First;
				
					XbimShell^ shell = dynamic_cast<XbimShell^>(geom);
					if (shell != nullptr) return shell;
					XbimSolid^ solid = dynamic_cast<XbimSolid^>(geom);
					if (solid != nullptr && solid->Shells->Count == 1)
						return solid->Shells->First;
				
			}
			//make all the faces in to one shell, this may be a topologically illegal object
			
			BRepPrim_Builder builder;
			TopoDS_Shell shell;
			builder.MakeShell(shell);
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(this, TopAbs_FACE, map);
			for (int i = 1; i <= map.Extent(); i++)
			{
				builder.AddShellFace(shell, TopoDS::Face(map(i)));
			}
			builder.CompleteShell(shell);
			return gcnew XbimShell(shell);
			
		}


		XbimCompound^ XbimCompound::Merge(IXbimSolidSet^ solids, double tolerance)
		{
			
			TopoDS_Compound compound;
			BRep_Builder b;
			
			////first remove any that intersect as simple merging leads to illegal geometries.
			Dictionary<XbimSolid^, HashSet<XbimSolid^>^>^ clusters = gcnew Dictionary<XbimSolid^, HashSet<XbimSolid^>^>();
			for each (IXbimSolid^ solid in solids) //init all the clusters
			{
				XbimSolid^ solidToCheck = dynamic_cast<XbimSolid^>(solid);
				if (solidToCheck != nullptr)
					clusters[solidToCheck] = gcnew HashSet<XbimSolid^>();
			}
			if (clusters->Count == 0) return nullptr; //nothing to do

			
			b.MakeCompound(compound);
			if (clusters->Count == 1 ) //just one so return it
			{
				for each(XbimSolid^ solid in clusters->Keys) //take the first one
				{
					b.Add(compound, solid);
					GC::KeepAlive(solid);
					return gcnew XbimCompound(compound, true, tolerance);
				}
			}
			for each (XbimSolid^ solid in solids)
			{
				XbimSolid^ solidToCheck = dynamic_cast<XbimSolid^>(solid);
				if (solidToCheck != nullptr)
				{
					XbimRect3D bbToCheck = solidToCheck->BoundingBox;
					for each (KeyValuePair<XbimSolid^, HashSet<XbimSolid^>^>^ cluster in clusters)
					{
						if (solidToCheck != cluster->Key && bbToCheck.Intersects(cluster->Key->BoundingBox))
							cluster->Value->Add(solidToCheck);
					}
				}
			}
			List<XbimSolid^>^ toMergeReduced = gcnew List<XbimSolid^>();
			Dictionary<XbimSolid^, HashSet<XbimSolid^>^>^ clustersSparse = gcnew Dictionary<XbimSolid^, HashSet<XbimSolid^>^>();
			for each (KeyValuePair<XbimSolid^, HashSet<XbimSolid^>^>^ cluster in clusters)
			{
				if (cluster->Value->Count>0)
					clustersSparse->Add(cluster->Key, cluster->Value);
				else
					toMergeReduced->Add(cluster->Key); //record the ones to simply merge
			}
			clusters = nullptr;

			XbimSolid^ clusterAround = nullptr;
			for each(XbimSolid^ fsolid in clustersSparse->Keys) //take the first one
			{
				clusterAround = fsolid;
				break;
			}

			while (clusterAround != nullptr)
			{
				HashSet<XbimSolid^>^ connected = gcnew HashSet<XbimSolid^>();
				XbimCompound::GetConnected(connected, clustersSparse, clusterAround);
				
				ShapeFix_ShapeTolerance fixTol;
				TopoDS_Shape unionedShape;
				for each (XbimSolid^ toConnect in connected) //join up the connected
				{
					fixTol.SetTolerance(toConnect, tolerance);
					if (unionedShape.IsNull()) unionedShape = toConnect;
					else
					{
						String^ err = "";
						try
						{
							BRepAlgoAPI_Fuse boolOp(unionedShape, toConnect);
							if (boolOp.ErrorStatus() == 0)
								unionedShape = boolOp.Shape();
							else
								XbimGeometryCreator::logger->WarnFormat("WC004: Boolean Union operation failed." );
						}
						catch (Standard_Failure e)
						{
							err = gcnew String(Standard_Failure::Caught()->GetMessageString());
							XbimGeometryCreator::logger->WarnFormat("WC005: Boolean Union operation failed. " + err);
						}
						
					}
				}
				XbimSolidSet^ solidSet = gcnew XbimSolidSet(unionedShape);

				for each (XbimSolid^ solid in solidSet) toMergeReduced->Add(solid);
				
				for each (XbimSolid^ solid in connected) //remove what we have connected
					clustersSparse->Remove(solid);

				clusterAround = nullptr;
				for each(XbimSolid^ fsolid in clustersSparse->Keys) //take the first one
				{
					clusterAround = fsolid;
					break;
				}
			}

			for each (XbimSolid^ solid in toMergeReduced)
			{
				b.Add(compound, solid);
				GC::KeepAlive(solid);
			}
			
			return gcnew XbimCompound(compound, true, tolerance);

		}

		List<XbimSolid^>^ XbimCompound::GetDiscrete(List<XbimSolid^>^% toProcess)
		{
			List<XbimSolid^>^ discrete = gcnew List<XbimSolid^>(toProcess->Count);
			if (toProcess->Count > 0)
			{
				
				List<XbimSolid^>^ connected = gcnew List<XbimSolid^>(toProcess->Count);

				for each (XbimSolid^ solid in toProcess)
				{
					if (discrete->Count == 0)
						discrete->Add(solid);
					else
					{
						XbimRect3D solidBB = solid->BoundingBox;
						bool isConnected = false;
						for each (XbimSolid^ discreteSolid in discrete)
						{
							if (discreteSolid->BoundingBox.Intersects(solidBB))
							{
								connected->Add(solid);
								isConnected = true;
								break;
							}	
						}
						if (!isConnected) discrete->Add(solid);
					}
				}
				toProcess = connected;
			}
			return discrete;
		}

		void  XbimCompound::GetConnected(HashSet<XbimSolid^>^ connected, Dictionary<XbimSolid^, HashSet<XbimSolid^>^>^ clusters, XbimSolid^ clusterAround)
		{
			if (connected->Add(clusterAround))
			{
				for each (KeyValuePair<XbimSolid^, HashSet<XbimSolid^>^>^ polysets in clusters)
				{
					if (!connected->Contains(polysets->Key) && !(polysets->Key == clusterAround) && polysets->Value->Contains(clusterAround))  //don't do the same one twice
					{
						GetConnected(connected, clusters, polysets->Key);
						for each (XbimSolid^ poly in polysets->Value)
						{
							GetConnected(connected, clusters, poly);
						}
					}
				}
			}
		}

		XbimCompound^ XbimCompound::Cut(XbimCompound^ solids, double tolerance)
		{
			if (!IsSewn) Sew();
			ShapeFix_ShapeTolerance fixTol;
			fixTol.SetTolerance(solids, tolerance);
			fixTol.SetTolerance(this, tolerance);
			String^ err = "";
			try
			{
				BRepAlgoAPI_Cut boolOp(this, solids);
				GC::KeepAlive(this);
				GC::KeepAlive(solids);
				
				if (boolOp.ErrorStatus() == 0)
				{
					XbimCompound^ result = gcnew XbimCompound(TopoDS::Compound(boolOp.Shape()), true, tolerance);
					if (result->BoundingBox.Length() - this->BoundingBox.Length() > tolerance) //nonsense result forget it
						return this;
					else
						return result;
				}
			}
			catch (Standard_Failure e)
			{
				err = gcnew String(Standard_Failure::Caught()->GetMessageString());
			}
			XbimGeometryCreator::logger->WarnFormat("WC001: Boolean Cut operation failed. " + err);
			return XbimCompound::Empty;
		}

		XbimCompound^ XbimCompound::Union(XbimCompound^ solids, double tolerance)
		{
			if (!IsSewn) Sew();
			ShapeFix_ShapeTolerance fixTol;
			fixTol.SetTolerance(solids, tolerance);
			fixTol.SetTolerance(this, tolerance);
			String^ err = "";
			try
			{
				BRepAlgoAPI_Fuse boolOp(this, solids);
				GC::KeepAlive(this);
				GC::KeepAlive(solids);
				if (boolOp.ErrorStatus() == 0)
					return gcnew XbimCompound(TopoDS::Compound(boolOp.Shape()),true,tolerance);
			}
			catch (Standard_Failure e)
			{
				err = gcnew String(Standard_Failure::Caught()->GetMessageString());
			}
			XbimGeometryCreator::logger->WarnFormat("WC002: Boolean Union operation failed. " + err);
			return XbimCompound::Empty;
		}

		
		XbimCompound^ XbimCompound::Intersection(XbimCompound^ solids, double tolerance)
		{
			if (!IsSewn) Sew();
			ShapeFix_ShapeTolerance fixTol;
			fixTol.SetTolerance(solids, tolerance);
			fixTol.SetTolerance(this, tolerance);
			String^ err = "";
			try
			{
				BRepAlgoAPI_Common boolOp(this, solids);
				GC::KeepAlive(this);
				GC::KeepAlive(solids);
				if (boolOp.ErrorStatus() == 0)
					return gcnew XbimCompound(TopoDS::Compound(boolOp.Shape()),true,tolerance);
			}
			catch (Standard_Failure e)
			{
				err = gcnew String(Standard_Failure::Caught()->GetMessageString());
			}
			XbimGeometryCreator::logger->WarnFormat("WC003: Boolean Intersection operation failed. " + err);
			return XbimCompound::Empty;
		}

		IXbimSolidSet^ XbimCompound::Solids::get()
		{
			XbimSolidSet^ solids = gcnew XbimSolidSet();
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(*pCompound, TopAbs_SOLID, map);
			for (int i = 1; i <= map.Extent(); i++)
				solids->Add(gcnew XbimSolid(TopoDS::Solid(map(i))));
			return solids;
		}

		IXbimShellSet^ XbimCompound::Shells::get()
		{
			List<IXbimShell^>^ shells = gcnew List<IXbimShell^>();
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(*pCompound, TopAbs_SHELL, map);
			for (int i = 1; i <= map.Extent(); i++)
				shells->Add(gcnew XbimShell(TopoDS::Shell(map(i))));
			return gcnew XbimShellSet(shells);
		}

		IXbimFaceSet^ XbimCompound::Faces::get()
		{
			List<IXbimFace^>^ faces = gcnew List<IXbimFace^>();
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(*pCompound, TopAbs_FACE, map);
			for (int i = 1; i <= map.Extent(); i++)
				faces->Add(gcnew XbimFace(TopoDS::Face(map(i))));
			return gcnew XbimFaceSet(faces);
		}

		IXbimEdgeSet^ XbimCompound::Edges::get()
		{
			List<IXbimEdge^>^ edges = gcnew List<IXbimEdge^>();
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(*pCompound, TopAbs_EDGE, map);
			for (int i = 1; i <= map.Extent(); i++)
				edges->Add(gcnew XbimEdge(TopoDS::Edge(map(i))));
			return gcnew XbimEdgeSet(edges);
		}

		IXbimVertexSet^ XbimCompound::Vertices::get()
		{
			List<IXbimVertex^>^ vertices = gcnew List<IXbimVertex^>();
			TopTools_IndexedMapOfShape map;
			TopExp::MapShapes(*pCompound, TopAbs_VERTEX, map);
			for (int i = 1; i <= map.Extent(); i++)
				vertices->Add(gcnew XbimVertex(TopoDS::Vertex(map(i))));
			return gcnew XbimVertexSet(vertices);
		}

		void XbimCompound::Add(IXbimGeometryObject^ geomObj)
		{
			XbimOccShape^ occ = dynamic_cast<XbimOccShape^>(geomObj);
			if (occ != nullptr)
			{
				BRep_Builder builder;
				if (ptrContainer == IntPtr::Zero)
				{
					pCompound = new TopoDS_Compound();
					builder.MakeCompound(*pCompound);
				}			
				builder.Add(*pCompound,occ);
			}
		}

		IXbimGeometryObjectSet^ XbimCompound::Cut(IXbimSolidSet^ solids, double tolerance)
		{

			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_CUT, (IEnumerable<IXbimGeometryObject^>^)this, solids, tolerance);
		}


		IXbimGeometryObjectSet^ XbimCompound::Cut(IXbimSolid^ solid, double tolerance)
		{
			if (Count == 0) return XbimGeometryObjectSet::Empty;
			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_CUT, (IEnumerable<IXbimGeometryObject^>^)this, gcnew XbimSolidSet(solid), tolerance);
		}


		IXbimGeometryObjectSet^ XbimCompound::Union(IXbimSolidSet^ solids, double tolerance)
		{

			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_FUSE, (IEnumerable<IXbimGeometryObject^>^)this, solids, tolerance);
		}

		IXbimGeometryObjectSet^ XbimCompound::Union(IXbimSolid^ solid, double tolerance)
		{
			if (Count == 0) return XbimGeometryObjectSet::Empty;
			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_FUSE, (IEnumerable<IXbimGeometryObject^>^)this, gcnew XbimSolidSet(solid), tolerance);
		}

		IXbimGeometryObjectSet^ XbimCompound::Intersection(IXbimSolidSet^ solids, double tolerance)
		{

			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_COMMON, (IEnumerable<IXbimGeometryObject^>^)this, solids, tolerance);
		}


		IXbimGeometryObjectSet^ XbimCompound::Intersection(IXbimSolid^ solid, double tolerance)
		{
			if (Count == 0) return XbimGeometryObjectSet::Empty;
			return XbimGeometryObjectSet::PerformBoolean(BOPAlgo_COMMON, (IEnumerable<IXbimGeometryObject^>^)this, gcnew XbimSolidSet(solid), tolerance);
		}
#pragma endregion


	}
}