#include<vtkCustomBorderWidget.h>

vtkCustomBorderWidget::vtkCustomBorderWidget()
{
  this->CallbackMapper->SetCallbackMethod(vtkCommand::MiddleButtonReleaseEvent,
                                        vtkWidgetEvent::EndSelect,
                                        this, vtkCustomBorderWidget::EndSelectAction);
}

 
void vtkCustomBorderWidget::EndSelectAction(vtkAbstractWidget *w)
{
  vtkBorderWidget *borderWidget =
    vtkBorderWidget::SafeDownCast(w);
 
  // Get the actual box coordinates/planes
  vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
 
  // Get the bottom left corner
  double* lowerLeft;
  lowerLeft = static_cast<vtkBorderRepresentation*>(borderWidget->GetRepresentation())->GetPosition();
  std::cout << "Lower left: " << lowerLeft[0] << " "
            << lowerLeft[1] << std::endl;
 
  double* upperRight;
  upperRight = static_cast<vtkBorderRepresentation*>(borderWidget->GetRepresentation())->GetPosition2();
  std::cout << "Upper right: " << upperRight[0] << " "
            << upperRight[1] << std::endl;
 
  vtkBorderWidget::EndSelectAction(w);
}

