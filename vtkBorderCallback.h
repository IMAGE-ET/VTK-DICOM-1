#include <vtkSmartPointer.h>
#include <vtkActor.h>
#include <vtkAssemblyNode.h>
#include <vtkAssemblyPath.h>
#include <vtkBorderRepresentation.h>
#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkCoordinate.h>
#include <vtkImageMapper3D.h>
#include <vtkImageActor.h>
#include <vtkInteractorStyleImage.h>
#include <vtkJPEGReader.h>
#include <vtkPolyData.h>
#include <vtkPropPicker.h>
#include <vtkProperty2D.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

class vtkBorderCallback : public vtkCommand
{

private:
  vtkSmartPointer<vtkRenderer> Renderer;
  vtkSmartPointer<vtkImageActor> ImageActor;
  
public:

  int* lowerLeft;
  int* upperRight;

  vtkBorderCallback();
  ~vtkBorderCallback();
  static vtkBorderCallback *New();
  virtual void Execute(vtkObject *caller, unsigned long, void*); 
  void SetRenderer(vtkSmartPointer<vtkRenderer> ren);
  void SetImageActor(vtkSmartPointer<vtkImageActor> im);
  //void PointPicker();

};
