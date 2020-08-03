
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkDataSetReader.h>
#include <vtkDataSetMapper.h>
#include <vtkContourFilter.h>
#include <vtkActor.h>
#include <vtkLookupTable.h>
#include <vtkCutter.h>
#include <vtkViewport.h>
#include <vtkPlane.h>


int main(int argc, char *argv[])
{
   vtkDataSetReader *reader = vtkDataSetReader::New();
   reader->SetFileName("noise.vtk");
   reader->Update();

   vtkDataSetMapper *mapper = vtkDataSetMapper::New();
   vtkDataSetMapper *mapper2 = vtkDataSetMapper::New();
   
   vtkLookupTable *lut = vtkLookupTable::New();
   mapper->SetLookupTable(lut);
   mapper2->SetLookupTable(lut);
   mapper->SetScalarRange(1,6);
   mapper2->SetScalarRange(1,6);

   double r, b = 0.0; 
   for (int i = 0; i < 256; i++) {
      r = 0 + i;
      b = 256 - i;
      lut->SetTableValue(i, r, 0.0, b, 1.0);
   }
   lut->Build();


   vtkActor *actor = vtkActor::New();
   actor->SetMapper(mapper);

   vtkActor *actor2 = vtkActor::New();
   actor2->SetMapper(mapper2);

   vtkContourFilter* conFilt = vtkContourFilter::New();
   conFilt->SetNumberOfContours(2);
   conFilt->SetValue(0, 2.4);
   conFilt->SetValue(1 , 4.0);
   conFilt->SetInputConnection(reader->GetOutputPort()); 
   conFilt->Update();

   mapper2->SetInputData(conFilt->GetOutput());

   vtkRenderer *ren = vtkRenderer::New();
   ren->AddActor(actor);
   ren->SetViewport(0.0, 0.0, 0.5, 1.0);

   vtkRenderer *ren2 = vtkRenderer::New();
   ren2->AddActor(actor2);
   ren2->SetViewport(0.5, 0.0, 1.0, 1.0);

   vtkCutter* cutter = vtkCutter::New();
   vtkPlane* plane = vtkPlane::New();
   plane->SetOrigin(1, 0, 0);
   plane->SetNormal(0, 0, 1); 
   cutter->SetCutFunction(plane);
   cutter->SetInputConnection(reader->GetOutputPort());
   cutter->Update();
   mapper->SetInputData(cutter->GetOutput());


   vtkRenderWindow *renwin = vtkRenderWindow::New();
   renwin->AddRenderer(ren);
   renwin->AddRenderer(ren2);
   renwin->SetSize(768, 768);
   

   vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::New();
   iren->SetRenderWindow(renwin);
   renwin->Render();
   iren->Start();
}


