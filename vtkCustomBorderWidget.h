#include <vtkSmartPointer.h>
#include <vtkWidgetCallbackMapper.h>
#include <vtkCommand.h>
#include <vtkWidgetEvent.h>
#include <vtkObjectFactory.h>
#include <vtkActor.h>
#include <vtkBorderRepresentation.h>
#include <vtkBorderWidget.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSphereSource.h>
 
class vtkCustomBorderWidget : public vtkBorderWidget
{
public:
  static vtkCustomBorderWidget *New();
  vtkTypeMacro(vtkCustomBorderWidget, vtkBorderWidget);
 
  static void EndSelectAction(vtkAbstractWidget *w);
 
  vtkCustomBorderWidget();
 
};
 
vtkStandardNewMacro(vtkCustomBorderWidget);
 
 