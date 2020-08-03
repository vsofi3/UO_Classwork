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
#include <vtkFloatArray.h>


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
    //return idx[2]*(dims[0]-1)*(dims[1]-1)+idx[1]*(dims[0]-1)*idx[0];
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


// ****************************************************************************
//  Function: EvaluateFieldAtLocation
//
//  Arguments:
//     pt: a two-dimensional location
//     dims: an array of size two.  
//              The first number is the size of the array in argument X, 
//              the second the size of Y.
//     X: an array (size is specified by dims).  
//              This contains the X locations of a rectilinear mesh.
//     Y: an array (size is specified by dims).  
//              This contains the Y locations of a rectilinear mesh.
//     F: a scalar field defined on the mesh.  Its size is dims[0]*dims[1].
//
//   Returns: the interpolated field value. 0 if the location is out of bounds.
//
// ****************************************************************************
float linearEq (float Fa, float Fb, float A, float B, float x){
    float t = (x - A) / (B - A);
    float rval = Fa + (t * (Fb - Fa));
    return rval; 
}

float
EvaluateFieldAtLocation(const float *pt, const int *dims, 
                        const float *X, const float *Y, const float *F)
{
    int Apts[3]; //top left corner      ex: (x, y + d)
    int Bpts[3]; //top right corner     ex: (x + k, y + d)
    int Cpts[3]; //bottom left corner   ex: (x,y)
    int Dpts[3]; //bottom right corner  ex:(x + k, y)
    float x = pt[0], y = pt[1];     // x & y value of point
    int sizeX = dims[0], sizeY = dims[1]; //size of X and Y arrays
    int sizeF = sizeX * sizeY;
    
    if (x >= X[sizeX - 1] || x <= X[0] || y >= Y[sizeY - 1] || y <= Y[0]) {
         return 0;
    }
   //search along X coordinates for X position
    for (int i = 0; i < sizeX; i++) {
        if(x >= X[i] && x <= X[i + 1]){
            Apts[0] = i;   
            Bpts[0] = i + 1;
            Cpts[0] = i;
            Dpts[0] = i + 1;
            break;
        }
    }
    //search along Y coordinates for Y position
    for (int i = 0; i < sizeY; i++) {
        if (y >= Y[i] && y <= Y[i + 1]) {
            Apts[1] = i + 1;
            Bpts[1] = i + 1; 
            Cpts[1] = i;
            Dpts[1] = i;
            break;
        }
    }
    int A = GetPointIndex(Apts, dims);
    int B = GetPointIndex(Bpts, dims);
    int C = GetPointIndex(Cpts, dims);
    int D = GetPointIndex(Dpts, dims);

    // solve T for X
    /**
    float Tx = (x - X[Apts[0]]) / (X[Bpts[0]] - X[Apts[0]]);
    std::cout << Tx << std::endl; 
    //f(u) = f(a) + tx * (f(b) - f(a))
    float uPt = F[A] + (Tx * (F[B] - F[A]));
    //f(l) = f(c) + tx * (f(d) - f(c))
    float lPt = F[C] + (Tx * (F[D] - F[C]));

    //now solve for T using Y values
    float Ty = (y - Y[Cpts[1]]) / (Y[Apts[1]] - Y[Cpts[1]]);
    std::cout << Ty << std::endl; 
    // f(x) = f(l) + ty * (f(u) - f(l))
    float rval = lPt + (Ty * (uPt - lPt)); **/
    float uPt = linearEq(F[A], F[B], X[Apts[0]], X[Bpts[0]], x);
    float lPt = linearEq(F[C], F[D], X[Cpts[0]], X[Dpts[0]], x);
    float rval = linearEq(lPt, uPt, Y[Cpts[1]], Y[Apts[1]], y);

    return rval;
}

// ****************************************************************************
//  Function: BoundingBoxForCell
//
//  Arguments:
//     X: an array (size is specified by dims).  
//              This contains the X locations of a rectilinear mesh.
//     Y: an array (size is specified by dims).  
//              This contains the Y locations of a rectilinear mesh.
//     dims: an array of size two.  
//              The first number is the size of the array in argument X, 
//              the second the size of Y.
//     cellId: a cellIndex (I.e., between 0 and GetNumberOfCells(dims))
//     bbox (output): the bounding box of cellId.  Format should be
//                     bbox[0]: the minimum X value in cellId.
//                     bbox[1]: the maximum X value in cellId.
//                     bbox[2]: the minimum Y value in cellId.
//                     bbox[3]: the maximum Y value in cellId.
//
//  Returns:  None (argument bbox is output)
//
// ****************************************************************************
void
BoundingBoxForCell(const float *X, const float *Y, const int *dims,
                   int cellId, float *bbox)
{
    if (cellId >= GetNumberOfCells(dims)) {
        bbox[0] = -100;
        bbox[1] = 100;
        bbox[2] = -100;
        bbox[3] = 100;
        return; 
     }
    int minX = 0, maxX = 0, minY = 0, maxY = 0;
    int logicalCell[2];
    GetLogicalCellIndex(logicalCell, cellId, dims);
    int cellX = logicalCell[0], cellY = logicalCell[1];
    minX = cellX, maxX = cellX;
    minY = cellY, maxY = cellY; 
    if (X[cellX + 1] > X[cellX]){
        maxX = cellX + 1;
    }
    else if (X[cellX - 1] < X[cellX]) {
        minX = cellX - 1;
    }
    if (Y[cellY + 1] > Y[cellY]) {
        maxY = cellY + 1;
    }
    else if (Y[cellY - 1] < Y[minY]) {
        minY = cellY - 1;
    } 
    bbox[0] = X[minX];
    bbox[1] = X[maxX];
    bbox[2] = Y[minY];
    bbox[3] = Y[maxY];
    return;
}
// ****************************************************************************
//  Function: CountNumberOfStraddingCells
//
//  Arguments:
//     X: an array (size is specified by dims).  
//              This contains the X locations of a rectilinear mesh.
//     Y: an array (size is specified by dims).  
//              This contains the Y locations of a rectilinear mesh.
//     dims: an array of size two.  
//              The first number is the size of the array in argument X, 
//              the second the size of Y.
//     F: a scalar field defined on the mesh.  Its size is dims[0]*dims[1].
//
//  Returns:  the number of cells that straddle 0, i.e., the number of cells
//            that contains points who have F>0 and also have points with F<0.
//
// ****************************************************************************
int
CountNumberOfStraddlingCells(const float *X, const float *Y, const int *dims,
                             const float *F)
{
    int cells = 0;
    int totalX = dims[0], totalY = dims[1];
    int idx[3];
    idx[2] = 0; //Z is always 0 for 2D
    for (int x = 0; x < totalX; x++){
        idx[0] = x;   //set x coordinate
        for (int y = 0; y < totalY; y++){
            int pos = 0, neg = 0;
            idx[1] = y; //set y coordinate
            int pid = GetPointIndex(idx, dims); //get point index
            if (F[pid] > 0) { 
            //check current scalar and next to see if its "straddling" 0
                for (int i = 0; i < 2; i++){ 
                    if (F[pid + i] > 0){
                        pos++;  //scalar is positive
                    }
                    if (F[pid + i] < 0){
                        neg++;  //scalar is negative
                    }
                }
            }
            if (pos && neg){ //if there is atleast 1 positive and negative scalar
                cells++;
            }
        }
    }
    return cells;
}

int main()
{
    int  i;

    vtkDataSetReader *rdr = vtkDataSetReader::New();
    rdr->SetFileName("proj2_data.vtk");
    rdr->Update();

    int dims[3];
    vtkRectilinearGrid *rgrid = (vtkRectilinearGrid *) rdr->GetOutput();
    rgrid->GetDimensions(dims);

    float *X = (float *) rgrid->GetXCoordinates()->GetVoidPointer(0);
    float *Y = (float *) rgrid->GetYCoordinates()->GetVoidPointer(0);
    float *F = (float *) rgrid->GetPointData()->GetScalars()->GetVoidPointer(0);
    
    int numCells = CountNumberOfStraddlingCells(X, Y, dims, F);
    cerr << "The number of cells straddling zero is " << numCells << endl;

    float bbox[4];
    const int ncells = 5;
    int cellIds[ncells] = { 0, 50, 678, 1000, 1200 };
    for (i = 0 ; i < ncells ; i++)
    {
        BoundingBoxForCell(X, Y, dims, cellIds[i], bbox);
        cerr << "The bounding box for cell " << cellIds[i] << " is " 
             << bbox[0] << "->" << bbox[1] << ", " << bbox[2] << "->" << bbox[3]
             << endl;
    }

    const int npts = 10;
    float pt[npts][3] = 
         {
            {1.01119, 0.122062, 0},
            {0.862376, 1.33839, 0},
            {0.155026, 0.126123, 0},
            {0.69736, 0.0653565, 0},
            {0.2, 0.274117, 0},
            {0.893699, 1.04111, 0},
            {0.608791, -0.0533753, 0},
            {1.00543, 0.138024, 0},
            {0.384128, -0.0768977, 0},
            {0.666757, 0.60259, 0},
         };

    

    for (i = 0 ; i < npts ; i++)
    {
        float f = EvaluateFieldAtLocation(pt[i], dims, X, Y, F);
        cerr << "Evaluated field at (" << pt[i][0] <<"," << pt[i][1] << ") as "
             << f << endl;
    }
    
   
}




