/*=========================================================================

  Program:   Visualization Toolkit
  Module:    SpecularSpheres.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
//
// This examples demonstrates the effect of specular lighting.
//
#include "vtkSmartPointer.h"
#include "vtkSphereSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkInteractorStyle.h"
#include "vtkObjectFactory.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkProperty.h"
#include "vtkCamera.h"
#include "vtkLight.h"
#include "vtkOpenGLPolyDataMapper.h"
#include "vtkJPEGReader.h"
#include "vtkImageData.h"
#include <vtkPNGWriter.h>

#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkPolyDataReader.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkDataSetReader.h>
#include <vtkContourFilter.h>
#include <vtkRectilinearGrid.h>

#include <vtkCamera.h>
#include <vtkDataSetMapper.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkSmartPointer.h>


// ****************************************************************************
//  Function: GetNumberOfPoints
//
//  Arguments:
//     dims: an array of size 3 with the number of points in X, Y, and Z.
//           2D data sets would have Z=1
//
//  Returns:  the number of points in a rectilinear mesh
//
// ****************************************************************************

int GetNumberOfPoints(const int *dims)
{
    // 3D
    //return dims[0]*dims[1]*dims[2];
    // 2D
    return dims[0]*dims[1];
}

// ****************************************************************************
//  Function: GetNumberOfCells
//
//  Arguments:
//
//      dims: an array of size 3 with the number of points in X, Y, and Z.
//            2D data sets would have Z=1
//
//  Returns:  the number of cells in a rectilinear mesh
//
// ****************************************************************************

int GetNumberOfCells(const int *dims)
{
    // 3D
    //return (dims[0]-1)*(dims[1]-1)*(dims[2]-1);
    // 2D
    return (dims[0]-1)*(dims[1]-1);
}


// ****************************************************************************
//  Function: GetPointIndex
//
//  Arguments:
//      idx:  the logical index of a point.
//              0 <= idx[0] < dims[0]
//              1 <= idx[1] < dims[1]
//              2 <= idx[2] < dims[2] (or always 0 if 2D)
//      dims: an array of size 3 with the number of points in X, Y, and Z.
//            2D data sets would have Z=1
//
//  Returns:  the point index
//
// ****************************************************************************

int GetPointIndex(const int *idx, const int *dims)
{
    // 3D
    //return idx[2]*dims[0]*dims[1]+idx[1]*dims[0]+idx[0];
    // 2D
    return idx[1]*dims[0]+idx[0];
}


// ****************************************************************************
//  Function: GetCellIndex
//
//  Arguments:
//      idx:  the logical index of a cell.
//              0 <= idx[0] < dims[0]-1
//              1 <= idx[1] < dims[1]-1 
//              2 <= idx[2] < dims[2]-1 (or always 0 if 2D)
//      dims: an array of size 3 with the number of points in X, Y, and Z.
//            2D data sets would have Z=1
//
//  Returns:  the cell index
//
// ****************************************************************************

int GetCellIndex(const int *idx, const int *dims)
{
    // 3D
    //return idx[2]*(dims[0]-1)*(dims[1]-1)+idx[1]*(dims[0]-1)+idx[0];
    // 2D
    return idx[1]*(dims[0]-1)+idx[0];
}

// ****************************************************************************
//  Function: GetLogicalPointIndex
//
//  Arguments:
//      idx (output):  the logical index of the point.
//              0 <= idx[0] < dims[0]
//              1 <= idx[1] < dims[1] 
//              2 <= idx[2] < dims[2] (or always 0 if 2D)
//      pointId:  a number between 0 and (GetNumberOfPoints(dims)-1).
//      dims: an array of size 3 with the number of points in X, Y, and Z.
//            2D data sets would have Z=1
//
//  Returns:  None (argument idx is output)
//
// ****************************************************************************

void GetLogicalPointIndex(int *idx, int pointId, const int *dims)
{
    // 3D
    // idx[0] = pointId%dim[0];
    // idx[1] = (pointId/dims[0])%dims[1];
    // idx[2] = pointId/(dims[0]*dims[1]);

    // 2D
    idx[0] = pointId%dims[0];
    idx[1] = pointId/dims[0];
}


// ****************************************************************************
//  Function: GetLogicalCellIndex
//
//  Arguments:
//      idx (output):  the logical index of the cell index.
//              0 <= idx[0] < dims[0]-1
//              1 <= idx[1] < dims[1]-1 
//              2 <= idx[2] < dims[2]-1 (or always 0 if 2D)
//      cellId:  a number between 0 and (GetNumberOfCells(dims)-1).
//      dims: an array of size 3 with the number of points in X, Y, and Z.
//            2D data sets would have Z=1
//
//  Returns:  None (argument idx is output)
//
// ****************************************************************************

void GetLogicalCellIndex(int *idx, int cellId, const int *dims)
{
    // 3D
    // idx[0] = cellId%(dims[0]-1);
    // idx[1] = (cellId/(dims[0]-1))%(dims[1]-1);
    // idx[2] = cellId/((dims[0]-1)*(dims[1]-1));

    // 2D
    idx[0] = cellId%(dims[0]-1);
    idx[1] = cellId/(dims[0]-1);
}


class SegmentList
{
   public:
                   SegmentList() { maxSegments = 10000; segmentIdx = 0; pts = new float[4*maxSegments]; };
     virtual      ~SegmentList() { delete [] pts; };

     void          AddSegment(float X1, float Y1, float X2, float Y2);
     vtkPolyData  *MakePolyData(void);

   protected:
     float        *pts;
     int           maxSegments;
     int           segmentIdx;
};

void
SegmentList::AddSegment(float X1, float Y1, float X2, float Y2)
{
    pts[4*segmentIdx+0] = X1;
    pts[4*segmentIdx+1] = Y1;
    pts[4*segmentIdx+2] = X2;
    pts[4*segmentIdx+3] = Y2;
    segmentIdx++;
}

vtkPolyData *
SegmentList::MakePolyData(void)
{
    int nsegments = segmentIdx;
    int numPoints = 2*(nsegments);
    vtkPoints *vtk_pts = vtkPoints::New();
    vtk_pts->SetNumberOfPoints(numPoints);
    int ptIdx = 0;
    vtkCellArray *lines = vtkCellArray::New();
    lines->EstimateSize(numPoints,2);
    for (int i = 0 ; i < nsegments ; i++)
    {
        double pt[3];
        pt[0] = pts[4*i];
        pt[1] = pts[4*i+1];
        pt[2] = 0.;
        vtk_pts->SetPoint(ptIdx, pt);
        pt[0] = pts[4*i+2];
        pt[1] = pts[4*i+3];
        pt[2] = 0.;
        vtk_pts->SetPoint(ptIdx+1, pt);
        vtkIdType ids[2] = { ptIdx, ptIdx+1 };
        lines->InsertNextCell(2, ids);
        ptIdx += 2;
    }

    vtkPolyData *pd = vtkPolyData::New();
    pd->SetPoints(vtk_pts);
    pd->SetLines(lines);
    lines->Delete();
    vtk_pts->Delete();

    return pd;
}

int identifyCase (float isoValue, float A, float B, float C, float D){
  int theCase = 0;
  if (A > isoValue){
    theCase++; 
  }
  if (B > isoValue){
    theCase += 2; 
  }
  if (C > isoValue){
    theCase += 4;
  }
  if (D > isoValue) {
    theCase += 8;
  }
  return theCase; 
}

int main()
{
    int  i, j;
    float isoValue = 3.2000;

    vtkDataSetReader *rdr = vtkDataSetReader::New();
    rdr->SetFileName("proj5.vtk");
    rdr->Update();

    int dims[3];
    vtkRectilinearGrid *rgrid = (vtkRectilinearGrid *) rdr->GetOutput();
    rgrid->GetDimensions(dims);

    float *X = (float *) rgrid->GetXCoordinates()->GetVoidPointer(0);
    float *Y = (float *) rgrid->GetYCoordinates()->GetVoidPointer(0);
    float *F = (float *) rgrid->GetPointData()->GetScalars()->GetVoidPointer(0);
    
    // Add 4 segments that put a frame around your isolines.  This also
    // documents how to use "AddSegment".
    SegmentList sl;
    sl.AddSegment(-10, -10, +10, -10); // Add segment (-10,-10) -> (+10, -10)
    sl.AddSegment(-10, +10, +10, +10);
    sl.AddSegment(-10, -10, -10, +10);
    sl.AddSegment(+10, -10, +10, +10);

    //Set up the lookup table
    int lup[16][4]; 
    lup[1][0] = 0, lup[1][1] = 3, lup[1][2] = -1, lup[1][3] = -1;
    lup[2][0] = 0, lup[2][1] = 1, lup[2][2] = -1, lup[2][3] = -1;
    lup[3][0] = 1, lup[3][1] = 3, lup[3][2] = -1, lup[3][3] = -1;
    lup[4][0] = 2, lup[4][1] = 3, lup[4][2] = -1, lup[4][3] = -1;
    lup[5][0] = 0, lup[5][1] = 2, lup[5][2] = -1, lup[5][3] = -1; 
    lup[7][0] = 1, lup[7][1] = 2, lup[7][2] = -1, lup[7][3] = -1;
    lup[8][0] = 1, lup[8][1] = 2, lup[8][2] = -1, lup[8][3] = -1;
    lup[10][0] = 0, lup[10][1] = 2, lup[10][2] = -1, lup[10][3] = -1;
    lup[11][0] = 2, lup[11][1] = 3, lup[11][2] = -1, lup[11][3] = -1;
    lup[12][0] = 1, lup[12][1] = 3, lup[12][2] = -1, lup[12][3] = -1;
    lup[13][0] = 0, lup[13][1] = 1, lup[13][2] = -1, lup[13][3] = -1;
    lup[14][0] = 0, lup[14][1] = 3, lup[14][2] = -1, lup[14][3] = -1; 

    //2 Segments (Ambiguous)
    lup[6][0] = 0; lup[6][1] = 1; lup[6][2] = 2; lup[6][3] = 3;
    lup[9][0] = 0, lup[9][1] = 3, lup[9][2] = 1, lup[9][3] = 2;
    // No/All segments
    lup[0][0] = lup[0][1] = lup[0][2] = lup[0][3] = -1; 
    lup[15][0] = lup[15][1] = lup[15][2] = lup[15][3] = -1; 

    //YOUR CODE TO GENERATE ISOLINES SHOULD GO HERE!

    //points for each vertice
    int ptsA[2], ptsB[2], ptsC[2], ptsD[2];
    float pts1[2], pts2[2];
    //scalar values for each vertice
    float fptA, fptB, fptC, fptD = 0.0;
    int icase, nsegments = 0; 
    for (int x = 0; x < dims[0] - 1; x++) {
      //Assign the points
      ptsA[0] = x, ptsC[0] = x;
      ptsB[0] = x + 1, ptsD[0] = x + 1;  
      for (int y = 0; y < dims[1] - 1; y++) {
        ptsA[1] = y, ptsB[1] = y;
        ptsC[1] = y + 1, ptsD[1] = y + 1;
        //Get scalar values for each point
        fptA = F[GetPointIndex(ptsA, dims)], fptB = F[GetPointIndex(ptsB, dims)];
        fptC = F[GetPointIndex(ptsC, dims)], fptD = F[GetPointIndex(ptsD, dims)];
        icase = identifyCase(isoValue, fptA, fptB, fptC, fptD);
        //determine the number of segments based off the case
        if (icase == 0 || icase == 15){
          nsegments = 0;
        }
        else if (icase == 6 || icase == 9) {
          nsegments = 2;
        }
        else {
          nsegments = 1;
        }
        for (int k = 0; k < nsegments; k++){
          int edge1 = lup[icase][k*2];
          int edge2 = lup[icase][k*2 + 1];
          float t;
          //interpolate along the appropriate edges
          switch(edge1) {
            case 0:
              t = (isoValue - fptA) / (fptB - fptA);
              pts1[0] = X[ptsA[0]] + (t * (X[ptsB[0]] - X[ptsA[0]]));
              pts1[1] = Y[ptsA[1]]; 
              break;
            case 1:
              t = (isoValue - fptB) / (fptD - fptB);
              pts1[0] = X[ptsB[0]]; 
              pts1[1] = Y[ptsB[1]] + (t * (Y[ptsD[1]] - Y[ptsB[1]]));
              break;
            case 2:
              t = (isoValue - fptC) / (fptD - fptC);
              pts1[0] = X[ptsC[0]] + (t * (X[ptsD[0]] - X[ptsC[0]]));
              pts1[1] = Y[ptsC[1]]; 
              break;
            case 3:
              t = (isoValue - fptA) / (fptC - fptA);
              pts1[0] = X[ptsA[0]]; 
              pts1[1] = Y[ptsA[1]] + (t * (Y[ptsC[1]] - Y[ptsA[1]]));
              break;
          }
          switch(edge2){
            case 0:
              t = (isoValue - fptA) / (fptB - fptA);
              pts2[0] = X[ptsA[0]] + (t * (X[ptsB[0]] - X[ptsA[0]]));
              pts2[1] = Y[ptsA[1]]; 
              break;
            case 1:
              t = (isoValue - fptB) / (fptD - fptB);
              pts2[0] = X[ptsB[0]]; 
              pts2[1] = Y[ptsB[1]] + (t * (Y[ptsD[1]] - Y[ptsB[1]]));
              break;
            case 2:
              t = (isoValue - fptC) / (fptD - fptC);
              pts2[0] = X[ptsC[0]] + (t * (X[ptsD[0]] - X[ptsC[0]]));
              pts2[1] = Y[ptsC[1]]; 
              break;
            case 3:
              t = (isoValue - fptA) / (fptC - fptA);
              pts2[0] = X[ptsA[0]]; 
              pts2[1] = Y[ptsA[1]] + (t * (Y[ptsC[1]] - Y[ptsA[1]]));
              break;
          } //add the segment
          sl.AddSegment(pts1[0], pts1[1], pts2[0], pts2[1]);
        }
      }  
    }
    

    vtkPolyData *pd = sl.MakePolyData();

    //This can be useful for debugging
/*
    vtkDataSetWriter *writer = vtkDataSetWriter::New();
    writer->SetFileName("paths.vtk");
    writer->SetInputData(pd);
    writer->Write();
 */

    vtkSmartPointer<vtkDataSetMapper> win1Mapper =
    vtkSmartPointer<vtkDataSetMapper>::New();
    win1Mapper->SetInputData(pd);
    win1Mapper->SetScalarRange(0, 0.15);

    vtkSmartPointer<vtkActor> win1Actor =
    vtkSmartPointer<vtkActor>::New();
    win1Actor->SetMapper(win1Mapper);

    vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();

    vtkSmartPointer<vtkRenderWindow> renWin =
      vtkSmartPointer<vtkRenderWindow>::New();
    renWin->AddRenderer(ren1);

    vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    iren->SetRenderWindow(renWin);
    ren1->AddActor(win1Actor);
    ren1->SetBackground(0.0, 0.0, 0.0);
    renWin->SetSize(800, 800);

    ren1->GetActiveCamera()->SetFocalPoint(0,0,0);
    ren1->GetActiveCamera()->SetPosition(0,0,50);
    ren1->GetActiveCamera()->SetViewUp(0,1,0);
    ren1->GetActiveCamera()->SetClippingRange(20, 120);
    ren1->GetActiveCamera()->SetDistance(30);

    // This starts the event loop and invokes an initial render.
    //
    iren->Initialize();
    iren->Start();

    pd->Delete();
}
