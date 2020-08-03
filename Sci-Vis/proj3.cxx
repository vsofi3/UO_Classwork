#include <vtkImageData.h>
#include <vtkPNGWriter.h>
#include <vtkPointData.h>
#include <vtkPoints.h>
#include <vtkUnsignedCharArray.h>
#include <vtkFloatArray.h>
#include <vtkDataSetReader.h>
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

float EvaluateFieldAtLocation(const float *pt, const int *dims, const float *X, const float *Y, const float *F)
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
    // float Tx = (x - X[Apts[0]]) / (X[Bpts[0]] - X[Apts[0]]);
    //f(u) = f(a) + tx * (f(b) - f(a))
    //float uPt = F[A] + (Tx * (F[B] - F[A]));
    float uPt = linearEq(F[A], F[B], X[Apts[0]], X[Bpts[0]], x);
    //f(l) = f(c) + tx * (f(d) - f(c))
    // float lPt = F[C] + (Tx * (F[D] - F[C]));
    float lPt = linearEq(F[C], F[D], X[Cpts[0]], X[Dpts[0]], x);
    //now solve for T using Y values
    // float Ty = (y - Y[Cpts[1]]) / (Y[Apts[1]] - Y[Cpts[1]]);
    // f(x) = f(l) + ty * (f(u) - f(l))
    //float rval = lPt + (Ty * (uPt - lPt));
    float rval = linearEq(lPt, uPt, Y[Cpts[1]], Y[Apts[1]], y);
    return rval;

}


void
WriteImage(vtkImageData *img, const char *filename)
{
    std::string full_filename = filename;
    full_filename += ".png";
    vtkPNGWriter *writer = vtkPNGWriter::New();
    writer->SetInputData(img);
    writer->SetFileName(full_filename.c_str());
    writer->Write();
    writer->Delete();
}

vtkImageData *
NewImage(int width, int height)
{
    vtkImageData *image = vtkImageData::New();
    image->SetDimensions(width, height, 1);
    //image->SetWholeExtent(0, width-1, 0, height-1, 0, 0);
    //image->SetUpdateExtent(0, width-1, 0, height-1, 0, 0);
    //image->SetNumberOfScalarComponents(3);
    image->AllocateScalars(VTK_UNSIGNED_CHAR, 3);
    //image->AllocateScalars();
    return image;
}

// ****************************************************************************
//  Function: ApplyBlueHotColorMap
//
//  Purpose: 
//     Maps a normalized scalar value F (0<=F<=1) to a color using the blue 
//     hot color map.
//
//     The blue hot color map has:
//        F=0: (0,0,128) 
//        F=1: (255,255,255) 
//       and smooth interpolation in between
//
//  Arguments:
//       F (input):     a scalar value between 0 and 1
//       RGB (output):  the location to store the color
//      
// ****************************************************************************

void
ApplyBlueHotColorMap(float F, unsigned char *RGB)
{
//        F=0: (0,0,128) 
//        F=1: (255,255,255) 
//        F(X) = (F(0) + (T * (F(1) - F(0)))
    
    unsigned char r = 0   + (F * (255 - 0));
    unsigned char g = 0   + (F * (255 - 0));
    unsigned char b = 128 + (F * (255 - 128)); 
    RGB[0] = r, RGB[1] = g, RGB[2] = b; 
}
// ****************************************************************************
//  Function: ApplyDifferenceColorMap
//
//  Purpose: 
//     Maps a normalized scalar value F (0<=F<=1) to a color using a divergent colormap
//
//     The divergent color map has:
//        F=0: (0,0,128) 
//        F=0.5: (255,255,255) 
//        F=1: (128, 0, 0)
//       and smooth interpolation in between
//
//  Arguments:
//       F (input):     a scalar value between 0 and 1
//       RGB (output):  the location to store the color
//      
// ****************************************************************************
void
ApplyDifferenceColorMap(float F, unsigned char *RGB)
{
    //     F(1)   F=0: (0,0,128) 
    //     F(2)   F=0.5: (255,255,255) 
    //     F(3)   F=1: (128, 0, 0)

    //F(X) = F(1) + (F * (F(2) - F(1)))
    if (F == 0.5){
        RGB[0] = 255, RGB[1] = 255, RGB[2] = 255;    
    }
    else if (F < 0.5){
        float newF = F * 2.0;
        unsigned char r = 0   + (newF * (255 - 0));
        unsigned char g = 0   + (newF * (255 - 0));
        unsigned char b = 128 + (newF * (255 - 128));  
        RGB[0] = r, RGB[1] = g, RGB[2] = b; 
    }
    //F(Y) = (F(2) + (F * (F(3) - F(2))))  
    else if (F > 0.5){
        float newF = (F - 0.5) * 2.0;
        unsigned char r = 255 + (newF * (128 - 255));
        unsigned char g = 255 + (newF * (0 - 255));
        unsigned char b = 255 + (newF * (0 - 255)); 
        RGB[0] = r, RGB[1] = g, RGB[2] =  b; 
    }
}
// ****************************************************************************
//  Function: ApplyBHSVColorMap
//  Purpose: 
//     Maps a normalized scalar value F (0<=F<=1) to a color using an HSV rainbow colormap
//
//     The rainbow colormap uses a saturation =1.0, value = 1.0, 
//     and interpolates hue from 0 to 360 degrees 
//
//  Arguments:
//       F (input):     a scalar value between 0 and 1
//       RGB (output):  the location to store the color
//      
// ****************************************************************************
void
ApplyHSVColorMap(float F, unsigned char *RGB)
{
    float saturation = 1.0;
    float value = 1.0;

    float hue = 0 + (F * (360 - 0));
    hue /= 60.f;
    int cap = floor(hue);
    float f = hue - cap;

    float p = value * (1.0 - saturation);
    float q = value * (1.0 - saturation * f);
    float t = value * (1.0 - saturation * (1.0 - f));
    switch(cap){
        case 0: {
                unsigned char r0 = (value * 255.0);
                unsigned char g0 = (t * 255.0);
                unsigned char b0 = (p * 255.0);
                RGB[0] = r0, RGB[1] = g0, RGB[2] = b0;
                break;
            }
        case 1: {
                unsigned char r1 = (q * 255.0);
                unsigned char g1 = (value * 255.0);
                unsigned char b1 = (p * 255.0);
                RGB[0] = r1, RGB[1] = g1, RGB[2] = b1;
                break;
            }
        case 2: {
                unsigned char r2 = (p * 255.0);
                unsigned char g2 = (value * 255.0);
                unsigned char b2 = (t * 255.0);
                RGB[0] = r2, RGB[1] = g2, RGB[2] = b2;
                break;
            }
        case 3: {
                unsigned char r3 = (p * 255.0);
                unsigned char g3 = (q * 255.0);
                unsigned char b3 = (value * 255.0); 
                RGB[0] = r3, RGB[1] = g3, RGB[2] = b3;
                break;
            }
        case 4: {
                unsigned char r4 = (t * 255.0);
                unsigned char g4 = (p * 255.0);
                unsigned char b4 = (value * 255.0);
                RGB[0] = r4, RGB[1] = g4, RGB[2] = b4;
                break;
            }
        case 5: {
                unsigned char r5 = (value * 255.0);
                unsigned char g5 = (p * 255.0);
                unsigned char b5 = (q * 255.0);
                RGB[0] = r5, RGB[1] = g5, RGB[2] = b5;
                break;
            }
    }
}


int main()
{
    int  i, j;

    vtkDataSetReader *rdr = vtkDataSetReader::New();
    rdr->SetFileName("proj3_data.vtk");
    rdr->Update();

    int dims[3];
    vtkRectilinearGrid *rgrid = (vtkRectilinearGrid *) rdr->GetOutput();
    rgrid->GetDimensions(dims);

    float *X = (float *) rgrid->GetXCoordinates()->GetVoidPointer(0);
    float *Y = (float *) rgrid->GetYCoordinates()->GetVoidPointer(0);
    float *F = (float *) rgrid->GetPointData()->GetScalars()->GetVoidPointer(0);
    
    int nx = 500;
    int ny = 500;
    float min = 100;
    float max = 0; 
    vtkImageData *images[3];
    unsigned char *buffer[3];
    for (i = 0 ; i < 3 ; i++)
    {
        images[i] = NewImage(nx, ny);
        buffer[i] = (unsigned char *) images[i]->GetScalarPointer(0,0,0);
    }

    for (i = 0 ; i < 3*nx*ny ; i++)
        for (j = 0 ; j < 3 ; j++)
            buffer[j][i] = 0;
    for (i = 0 ; i < nx; i++)
        //need to set pt[0] differently
        for (j = 0 ; j < ny ; j++)
        {
            // ITERATE OVER PIXELS
            float pt[2];
            pt[0] = linearEq(-9, 9, 0, 499, i);
            pt[1] = linearEq(-9, 9, 0, 499, j);
            float f = EvaluateFieldAtLocation(pt, dims, X, Y, F);
            float normalizedF = linearEq(0, 1, 1.2, 5.02, f); //(f - 1.2) / (5.02 - 1.2); //see step 5 re 1.2->5.02
            // I TAKE OVER HERE
            int offset = 3*(j*nx+i);
            ApplyBlueHotColorMap(normalizedF, buffer[0]+offset);
            ApplyDifferenceColorMap(normalizedF, buffer[1]+offset);
            ApplyHSVColorMap(normalizedF, buffer[2]+offset);
        }
    WriteImage(images[0], "bluehot");
    WriteImage(images[1], "difference");
    WriteImage(images[2], "hsv");
}

