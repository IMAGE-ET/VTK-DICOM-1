#include <vtkBorderWidget.h>
#include <vtkCommand.h>
#include <vtkBorderCallback.h>

 
vtkBorderCallback::vtkBorderCallback()
{
}
vtkBorderCallback::~vtkBorderCallback()
{
}
vtkBorderCallback *vtkBorderCallback::New()
	{
		return new vtkBorderCallback;
	}
 
void vtkBorderCallback::Execute(vtkObject *caller, unsigned long, void*)
	{
		vtkBorderWidget *borderWidget = reinterpret_cast<vtkBorderWidget*>(caller);
 
    		// Get the display coordinates of the two corners of the box
    		vtkCoordinate* lowerLeftCoordinate = static_cast<vtkBorderRepresentation*>(borderWidget->GetRepresentation())->GetPositionCoordinate();
    		//int* lowerLeft;
    		lowerLeft = lowerLeftCoordinate ->GetComputedDisplayValue(this->Renderer);
    		//std::cout << "Lower left coordinate in screen coordinates: " << lowerLeft[0] << ", " << lowerLeft[1] << std::endl;
    		lowerLeft[2] = 0;
		convertCoordinates(lowerLeft[0],lowerLeft[1]);
		//std::cout << "Lower left coordinate in pixel coordinates: " << newX << ", " << newY << std::endl;


    		vtkCoordinate* upperRightCoordinate = static_cast<vtkBorderRepresentation*>(borderWidget->GetRepresentation())->GetPosition2Coordinate();
    		//int* upperRight;
    		upperRight = upperRightCoordinate ->GetComputedDisplayValue(this->Renderer);
    		//std::cout << "Upper right coordinate in screen coordinates: " << upperRight[0] << ", " << upperRight[1] << std::endl;
    		upperRight[2] = 0;
		convertCoordinates(upperRight[0],upperRight[1]); 
		//std::cout << "Upper right coordinate in pixel coordinates: " << newX << ", " << newY << std::endl;

    		// Pick at the two corners of the box
    		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
 
    		picker->Pick( lowerLeft[0],lowerLeft[1], 0.0, this->Renderer );
 
    		vtkAssemblyPath* path = picker->GetPath();
 
    		vtkProp* pickedProp = NULL;
 
    		bool validPick = false;
    		if( path )
      		{
      			//std::cout << "There are " << path->GetNumberOfItems() << " items in the path." << std::endl;
      			vtkCollectionSimpleIterator sit;
      			path->InitTraversal( sit );
      			vtkAssemblyNode *node;
      			for( int i = 0; i < path->GetNumberOfItems(); ++i )
        		{
        			node = path->GetNextNode( sit );
        			pickedProp = node->GetViewProp();
        			if( this->ImageActor == vtkImageActor::SafeDownCast( pickedProp ) )
          			{
          				//std::cout << "Correct actor picked." << std::endl;
          				validPick = true;
          				break;
          			}
        		}
      		}
 
    		if(!validPick)
      		{
      			//std::cout << "Off Image" << std::endl;
      		}
    		else
      		{ 
      			double pos1[3];
      			picker->GetPickPosition( pos1 );
      			//std::cout << "Lower Left Pick: " << pos1[0] << " " << pos1[1] << std::endl;
      		}    

    		//std::cout << std::endl;	
}
 
  void vtkBorderCallback::SetRenderer(vtkSmartPointer<vtkRenderer> ren) {this->Renderer = ren;}
  void vtkBorderCallback::SetImageActor(vtkSmartPointer<vtkImageActor> im) {this->ImageActor = im;} 
 
void vtkBorderCallback::convertCoordinates(double oldX,double oldY)
{
	newX = 0 + ((oldX - 0) / this->RenderWindow->GetSize()[0] ) * 512;
	newY = 0 + ((oldY - 0) / this->RenderWindow->GetSize()[1] ) * 512;
}

void vtkBorderCallback::SetImageRenWin(vtkSmartPointer<vtkRenderWindow> rw) {this->RenderWindow = rw;}
