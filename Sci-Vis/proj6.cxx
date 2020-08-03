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
#include <vtkCleanPolyData.h>
#include <vtkPolyDataNormals.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDoubleArray.h>
#include <vtkCellArray.h>
#include <vtkDataSetReader.h>
#include <vtkContourFilter.h>
#include <vtkRectilinearGrid.h>
#include <vtkDataSetWriter.h>
#include <vtkRectilinearGridToTetrahedra.h>
#include <vtkUnstructuredGrid.h>

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
    return dims[0]*dims[1]*dims[2];
    // 2D
    //return dims[0]*dims[1];
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
    return (dims[0]-1)*(dims[1]-1)*(dims[2]-1);
    // 2D
    //return (dims[0]-1)*(dims[1]-1);
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
    return idx[2]*dims[0]*dims[1]+idx[1]*dims[0]+idx[0];
    // 2D
    //return idx[1]*dims[0]+idx[0];
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
    return idx[2]*(dims[0]-1)*(dims[1]-1)+idx[1]*(dims[0]-1)+idx[0];
    // 2D
    //return idx[1]*(dims[0]-1)+idx[0];
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
     idx[0] = pointId%dims[0];
     idx[1] = (pointId/dims[0])%dims[1];
     idx[2] = pointId/(dims[0]*dims[1]);

    // 2D
    // idx[0] = pointId%dims[0];
    // idx[1] = pointId/dims[0];
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
    idx[0] = cellId%(dims[0]-1);
    idx[1] = (cellId/(dims[0]-1))%(dims[1]-1);
    idx[2] = cellId/((dims[0]-1)*(dims[1]-1));

    // 2D
    //idx[0] = cellId%(dims[0]-1);
    //idx[1] = cellId/(dims[0]-1);
}


class TriangleList
{
   public:
                   TriangleList() { maxTriangles = 1000000; triangleIdx = 0; pts = new float[9*maxTriangles]; };
     virtual      ~TriangleList() { delete [] pts; };

     void          AddTriangle(float X1, float Y1, float Z1, float X2, float Y2, float Z2, float X3, float Y3, float Z3);
     vtkPolyData  *MakePolyData(void);

   protected:
     float        *pts;
     int           maxTriangles;
     int           triangleIdx;
};

void
TriangleList::AddTriangle(float X1, float Y1, float Z1, float X2, float Y2, float Z2, float X3, float Y3, float Z3)
{
    pts[9*triangleIdx+0] = X1;
    pts[9*triangleIdx+1] = Y1;
    pts[9*triangleIdx+2] = Z1;
    pts[9*triangleIdx+3] = X2;
    pts[9*triangleIdx+4] = Y2;
    pts[9*triangleIdx+5] = Z2;
    pts[9*triangleIdx+6] = X3;
    pts[9*triangleIdx+7] = Y3;
    pts[9*triangleIdx+8] = Z3;
    triangleIdx++;
}

vtkPolyData *
TriangleList::MakePolyData(void)
{
    int ntriangles = triangleIdx;
    int numPoints = 3*(ntriangles);
    vtkPoints *vtk_pts = vtkPoints::New();
    vtk_pts->SetNumberOfPoints(numPoints);
    int ptIdx = 0;
    vtkCellArray *tris = vtkCellArray::New();
    tris->EstimateSize(numPoints,4);
    for (int i = 0 ; i < ntriangles ; i++)
    {
        double pt[3];
        pt[0] = pts[9*i];
        pt[1] = pts[9*i+1];
        pt[2] = pts[9*i+2];
        vtk_pts->SetPoint(ptIdx, pt);
        pt[0] = pts[9*i+3];
        pt[1] = pts[9*i+4];
        pt[2] = pts[9*i+5];
        vtk_pts->SetPoint(ptIdx+1, pt);
        pt[0] = pts[9*i+6];
        pt[1] = pts[9*i+7];
        pt[2] = pts[9*i+8];
        vtk_pts->SetPoint(ptIdx+2, pt);
        vtkIdType ids[3] = { ptIdx, ptIdx+1, ptIdx+2 };
        tris->InsertNextCell(3, ids);
        ptIdx += 3;
    }

    vtkPolyData *pd = vtkPolyData::New();
    pd->SetPoints(vtk_pts);
    pd->SetPolys(tris);
    tris->Delete();
    vtk_pts->Delete();

    return pd;
}

class Tetrahedron
{
  public:
    float X[4];
    float Y[4];
    float Z[4];
    float F[4];
    void PrintSelf()
    {
        for (int i = 0 ; i < 4 ; i++)
            printf("\tV%d: (%f, %f, %f) = %f\n", i, X[i], Y[i], Z[i], F[i]);
    };
};

void IsosurfaceTet(Tetrahedron &tet, TriangleList &tl, float isoval)
{
    //The look up table. If an edge must be interpolated on, it has a one. Otherwise, it receives a 0
    //3 1s implies one triangle. 4 ones implies 2 triangles

    int lup[16][6];
    lup[0][0] = lup[0][1] = lup[0][2] = lup[0][3] = lup[0][4] = lup[0][5] = 0;
    lup[1][0] = 1, lup[1][1] = 0, lup[1][2] = 0, lup[1][3] = 1, lup[1][4] = 0, lup[1][5] = 1;
    lup[2][0] = 1, lup[2][1] = 1, lup[2][2] = 0, lup[2][3] = 0, lup[2][4] = 1, lup[2][5] = 0; 
    lup[3][0] = 0, lup[3][1] = 1, lup[3][2] = 0, lup[3][3] = 1, lup[3][4] = 1, lup[3][5] = 1; 
    lup[4][0] = 0, lup[4][1] = 1, lup[4][2] = 1, lup[4][3] = 0, lup[4][4] = 0, lup[4][5] = 1; 
    lup[5][0] = 1, lup[5][1] = 1, lup[5][2] = 1, lup[5][3] = 1, lup[5][4] = 0, lup[5][5] = 0; 
    lup[6][0] = 1, lup[6][1] = 0, lup[6][2] = 1, lup[6][3] = 0, lup[6][4] = 1, lup[6][4] = 1;
    lup[7][0] = 0, lup[7][1] = 0, lup[7][2] = 1, lup[7][3] = 1, lup[7][4] = 1, lup[7][5] = 0;
    lup[8][0] = 0, lup[8][1] = 0, lup[8][2] = 1, lup[8][3] = 3, lup[8][4] = 1, lup[8][5] = 0;
    lup[9][0] = 1, lup[9][1] = 0, lup[9][2] = 1, lup[9][3] = 0, lup[9][4] = 1, lup[9][5] = 1; 
    lup[10][0] = 1, lup[10][1] = 1, lup[10][2] = 1, lup[10][3] = 1, lup[10][4] = 0, lup[10][5] = 0;
    lup[11][0] = 0, lup[11][1] = 1, lup[11][2] = 1, lup[11][3] = 0, lup[11][4] = 0, lup[11][5] = 1; 
    lup[12][0] = 0, lup[12][1] = 1, lup[12][2] = 0, lup[12][3] = 1, lup[12][4] = 1, lup[12][5] = 1;
    lup[13][0] = 1, lup[13][1] = 1, lup[13][2] = 0, lup[13][3] = 0, lup[13][4] = 1, lup[13][5] = 0; 
    lup[14][0] = 1, lup[14][1] = 0, lup[14][2] = 0, lup[14][3] = 1, lup[14][4] = 0, lup[14][5] = 1; 
    lup[15][0] = lup[15][1] = lup[15][2] = lup[15][3] = lup[15][4] = lup[15][5] = 0; 
    int icase = 0;
    if (tet.F[0] >= isoval){
        icase++;
    }
    if (tet.F[1] >= isoval){
        icase +=2;
    }
    if (tet.F[2] >= isoval){
        icase += 4;
    }
    if (tet.F[3] >= isoval){
        icase += 8;
    }
    //TODO: Nothing shows up...
    //ALSO: MUST CONSIDER THE CASES WHERE TRIANGLES SHARE AN EDGE.//
    float pts[4][3]; 
    float t = 0;
    int x = 0, y = 1, z = 2; 
    int numPoints = 0;
    for (int i = 0; i < 6; i++){
        if (lup[icase][i]){
            switch(i) {  
                case 0:
                    t = (isoval - tet.F[0]) / (tet.F[1] - tet.F[0]);
                    pts[numPoints][x] = tet.X[0] + t * (tet.X[1] - tet.X[0]);
                    pts[numPoints][y] = tet.Y[0] + t * (tet.Y[1] - tet.Y[0]);
                    pts[numPoints][z] = tet.Z[0] + t * (tet.Z[1] - tet.Z[0]);
                    numPoints++;
                    break;
                case 1:
                    t = (isoval - tet.F[1]) / (tet.F[2] - tet.F[1]);
                    pts[numPoints][x] = tet.X[1] + t * (tet.X[2] - tet.X[1]);
                    pts[numPoints][y] = tet.Y[1] + t * (tet.Y[2] - tet.Y[1]);
                    pts[numPoints][z] = tet.Z[1] + t * (tet.Z[2] - tet.Z[1]);
                    numPoints++;
                    break;
            
                case 2:
                    t = (isoval - tet.F[2]) / (tet.F[3] - tet.F[2]);
                    pts[numPoints][x] = tet.X[2] + t * (tet.X[3] - tet.X[2]);
                    pts[numPoints][y] = tet.Y[2] + t * (tet.Y[3] - tet.Y[2]);
                    pts[numPoints][z] = tet.Z[2] + t * (tet.Z[3] - tet.Z[2]);
                    numPoints++;
                    break;
                case 3:
                    t = (isoval - tet.F[0]) / (tet.F[3] - tet.F[0]);
                    pts[numPoints][x] = tet.X[0] + t * (tet.X[3] - tet.X[0]);
                    pts[numPoints][y] = tet.Y[0] + t * (tet.Y[3] - tet.Y[0]);
                    pts[numPoints][z] = tet.Z[0] + t * (tet.Z[3] - tet.Z[0]);
                    numPoints++;
                    break;
                case 4:
                    t = (isoval - tet.F[1]) / (tet.F[3] - tet.F[1]);
                    pts[numPoints][x] = tet.X[1] + t * (tet.X[3] - tet.X[1]);
                    pts[numPoints][y] = tet.Y[1] + t * (tet.Y[3] - tet.Y[1]);
                    pts[numPoints][z] = tet.Z[1] + t * (tet.Z[3] - tet.Z[1]);
                    numPoints++;
                    break;
                case 5:
                    t = (isoval - tet.F[0]) / (tet.F[2] - tet.F[0]);
                    pts[numPoints][x] = tet.X[0] + t * (tet.X[2] - tet.X[0]);
                    pts[numPoints][y] = tet.Y[0] + t * (tet.Y[2] - tet.Y[0]);
                    pts[numPoints][z] = tet.Z[0] + t * (tet.Z[2] - tet.Z[0]);
                    numPoints++;
                    break;
            }
            } 
        }
        if (numPoints == 3) {
                tl.AddTriangle(pts[0][0], pts[0][1], pts[0][2], pts[1][0], pts[1][1], pts[1][2], pts[2][0], pts[2][1], pts[2][2]);
                return;
            }
        else if (numPoints == 4) {
            if (icase == 10 || icase == 5){
                tl.AddTriangle(pts[0][0], pts[0][1], pts[0][2], pts[1][0], pts[1][1], pts[1][2], pts[3][0], pts[3][1], pts[3][2]);
                tl.AddTriangle(pts[2][0], pts[2][1], pts[2][2], pts[1][0], pts[1][1], pts[1][2], pts[3][0], pts[3][1], pts[3][2]);
            }
            else {
                tl.AddTriangle(pts[0][0], pts[0][1], pts[0][2], pts[2][0], pts[2][1], pts[2][2], pts[3][0], pts[3][1], pts[3][2]);
                tl.AddTriangle(pts[3][0], pts[3][1], pts[3][2], pts[1][0], pts[1][1], pts[1][2], pts[2][0], pts[2][1], pts[2][2]); 
            }
                return;
        }
            return;
    }

int main()
{
    int  i, j;

/* If you want to try on one tetrahedron. 
    Tetrahedron t;
    t.X[0] = 0;
    t.Y[0] = 0;
    t.Z[0] = 0;
    t.F[0] = 0.6;
    t.X[1] = 1;
    t.Y[1] = 0;
    t.Z[1] = 0;
    t.F[1] = 0;
    t.X[2] = 0;
    t.Y[2] = 1;
    t.Z[2] = 0;
    t.F[2] = 0;
    t.X[3] = 0.5;
    t.Y[3] = 0.5;
    t.Z[3] = 1.0;
    t.F[3] = 1;
    TriangleList tl;
    IsosurfaceTet(t, tl, 0.5);  */


    vtkDataSetReader *rdr = vtkDataSetReader::New();
    rdr->SetFileName("proj6.vtk");
    rdr->Update();
    if (rdr->GetOutput() == NULL || rdr->GetOutput()->GetNumberOfCells() == 0)
    {
        cerr << "Could not find input file." << endl;
        exit(EXIT_FAILURE);
    }

    vtkUnstructuredGrid *ugrid = (vtkUnstructuredGrid *) rdr->GetOutput();
    float *pts = (float *) ugrid->GetPoints()->GetVoidPointer(0);
    float *F = (float *) ugrid->GetPointData()->GetScalars()->GetVoidPointer(0);
    vtkCellArray *cell_array = ugrid->GetCells();

    TriangleList tl;
    cell_array->InitTraversal();
    int ncells = cell_array->GetNumberOfCells();
    cerr << "Number of cells to tetrahedralize is " << ncells << endl;
    int cnt = 0;
    float isoval = 12.2;
    std::cout << "generating isosurface..." << std::endl;
    for (int i = 0 ; i < ncells ; i++)
    {
        vtkIdType npts;
        vtkIdType *ids;
        cell_array->GetNextCell(npts, ids);
        if (npts == 4)
        {
            Tetrahedron tet;
            for (int j = 0 ; j < 4 ; j++)
            {
                // This data set is in a huge bounding box.  Normalize as we go.
                tet.X[j] = (pts[3*ids[j]]+3e+7)/6e+7;
                tet.Y[j] = (pts[3*ids[j]+1]+3e+7)/6e+7;
                tet.Z[j] = (pts[3*ids[j]+2]+3e+7)/6e+7;
                tet.F[j] = F[ids[j]];
            }
            IsosurfaceTet(tet, tl, isoval);
        }
        else
        {
            cerr << "Input was non-tetrahedron!!  Ignoring..." << endl;
            cerr << "Type is " << npts << endl;
            cerr << "Cell is " << i << endl;
            cnt++;
            continue;
        }
    }
    std::cout << "completed isosurfacing" << std::endl;

    vtkPolyData *pd = tl.MakePolyData();

/*
    //This can be useful for debugging
    vtkDataSetWriter *writer = vtkDataSetWriter::New();
    writer->SetFileName("proj6_out.vtk");
    writer->SetInputData(pd);
    writer->Write();
*/

    vtkCleanPolyData *cpd = vtkCleanPolyData::New();
    cpd->SetInputData(pd);
    cpd->SetAbsoluteTolerance(0);
    cpd->PointMergingOn();
    cpd->Update();
    vtkPolyDataNormals *pdn = vtkPolyDataNormals::New();
    pdn->SetInputData(cpd->GetOutput());
    //pdn->SetInputData(pd);
    pdn->Update();

    vtkSmartPointer<vtkDataSetMapper> win1Mapper =
      vtkSmartPointer<vtkDataSetMapper>::New();
    win1Mapper->SetInputData(pdn->GetOutput());
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

    ren1->GetActiveCamera()->SetFocalPoint(0.5, 0.5, 0.5);
    ren1->GetActiveCamera()->SetPosition(0,0,-2);
    ren1->GetActiveCamera()->SetViewUp(0,1,0);
    ren1->GetActiveCamera()->SetClippingRange(0.01, 4);
    ren1->GetActiveCamera()->SetDistance(30);

    // This starts the event loop and invokes an initial render.
    //
    iren->Initialize();
    iren->Start();

    pd->Delete();
}
