#include "filedialog.h"
#include "ui_filedialog.h"

#include "qfiledialog.h"
#include "vtkBorderCallback.h"


FileDialog::FileDialog(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileDialog)
{
    ui->setupUi(this);
}


FileDialog::~FileDialog()
{
    delete ui;
}


void FileDialog::on_pushButton_clicked()
{
    s1 = QFileDialog::getOpenFileName(this,tr("Select your file"),"/home",tr("Images(*.png *.jpg *.xpm *.dcm)"));
    ui->lineEdit->setText(s1);
    const std::string ipfile = s1.toStdString();
    p = ipfile.c_str();
    						

//Read DICOM images using ITK
	itkreader = ReaderType::New();
	itkreader->SetFileName( ipfile );
	ImageIOType::Pointer gdcmImageIO = ImageIOType::New();
	itkreader->SetImageIO( gdcmImageIO );
	try
	{
		itkreader->Update();
	}
	catch (itk::ExceptionObject & e)
	{
		std::cerr << "exception in file reader " << std::endl;
		std::cerr << e << std::endl;
	}

//Write the image in DICOM format
   
   	WriterType::Pointer writerori = WriterType::New();
   	writerori->SetInput( itkreader->GetOutput() );
   	writerori->SetFileName( "Original.dcm" );   
   
   	writerori->SetImageIO( gdcmImageIO );

  	try
    	{
    		writerori->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
        }


//Convert it into png

	RescaleFilterType::Pointer rescalar = RescaleFilterType::New();
	rescalar->SetOutputMinimum(0);
	rescalar->SetOutputMaximum(255);
	rescalar->SetInput(itkreader->GetOutput());
	/*CastFilterType::Pointer caster = CastFilterType::New();
	caster->SetInput(itkreader->GetOutput());*/

	pngWriterType1::Pointer pngWriter1 = pngWriterType1::New();
	pngWriter1->SetFileName("OriginalPNG.png");
	pngWriter1->SetInput(rescalar->GetOutput());
	try
	{
		pngWriter1->Update();
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
	    std::cerr << e << std::endl;
	}

//calculate the size of the input image
	sizeitkimage0 = itkreader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
	sizeitkimage1 = itkreader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
	//std::cout<<"ImageSize: "<<sizeitkimage0<<"\t"<<sizeitkimage1<<endl;
	
//Flip the image
	itk::FixedArray<bool, 2> flipAxes;
	flipAxes[0] = false;
	flipAxes[1] = true;
	flipfilter = FlipImageFilterType::New();
	flipfilter->SetInput(itkreader->GetOutput());
	flipfilter->SetFlipAxes(flipAxes);


//Convert the Image to a VTK Image

	//Connector ITK to VTK
	typedef itk::ImageToVTKImageFilter<InputImageType>  ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(flipfilter->GetOutput());
	connector->Update();

//Drawing the rectangle
	
	//Message
	std::cout<<"Draw the rectangle and press Q to continue..."<<endl;


	vtkImageData *vtkImage = connector->GetOutput();
	double* IOrigin = vtkImage->GetOrigin();
	int IExtent[6];
	vtkImage->GetExtent(IExtent);
	//cout<<"Image Origin: "<<IOrigin[0]<<"\t"<<IOrigin[1]<<"\t"<<IOrigin[2]<<endl;
	//cout<<"Image Extent: "<<IExtent[0]<<"\t"<<IExtent[1]<<"\t"<<IExtent[2]<<"\t"<<IExtent[3]<<"\t"<<IExtent[4]<<"\t"<<IExtent[5]<<endl;
	vtkImage->SetOrigin(0.0,0.0,0.0);
	IOrigin = vtkImage->GetOrigin();
	//cout<<"Image Origin: "<<IOrigin[0]<<"\t"<<IOrigin[1]<<"\t"<<IOrigin[2]<<endl;
	double* ISpacing = vtkImage->GetSpacing();
	//cout<<"Image Spacing: "<<ISpacing[0]<<"\t"<<ISpacing[1]<<endl;

//Rendering
	//Actor
	vtkSmartPointer<vtkImageActor> actor =  vtkSmartPointer<vtkImageActor>::New();
	actor->GetMapper()->SetInputData(connector->GetOutput()); 

	//Renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();

	//Fill the viewport with the image
	double xc, yc, xd, yd, d;
	vtkSmartPointer<vtkCamera> camera = vtkSmartPointer<vtkCamera>::New();	
	xc = IOrigin[0] + 0.5*(IExtent[0] + IExtent[1]) * ISpacing[0];
	yc = IOrigin[1] + 0.5*(IExtent[2] + IExtent[3]) * ISpacing[1];
	xd = (IExtent[1] - IExtent[0] + 1) * ISpacing[0];
	yd = (IExtent[3] - IExtent[2] + 1) * ISpacing[1];
	d = camera->GetDistance();
	camera->SetParallelScale(0.5*yd);
	camera->SetFocalPoint(xc,yc,0.0);
	camera->SetPosition(xc,yc,+d);
	camera->ParallelProjectionOn(); 
 
	renderer->ResetCameraClippingRange();
	renderer->SetActiveCamera(camera);
	renderer->ResetCamera();

	//Renderer Window
  	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	renderWindow->SetSize(sizeitkimage0,sizeitkimage1);
  	renderWindow->AddRenderer(renderer);
 
	//Render Window Interactor
  	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	renderWindowInteractor->SetRenderWindow(renderWindow);

	//Style
  	vtkSmartPointer<vtkInteractorStyleImage> style = vtkSmartPointer<vtkInteractorStyleImage>::New();
  	renderWindowInteractor->SetInteractorStyle(style);
	

	//BorderWidget class
  	vtkSmartPointer<vtkBorderWidget> borderWidget = vtkSmartPointer<vtkBorderWidget>::New();
  	borderWidget->SetInteractor(renderWindowInteractor);
  	static_cast<vtkBorderRepresentation*>(borderWidget->GetRepresentation())->GetBorderProperty()->SetColor(1,0,0);
  	borderWidget->SelectableOff();

  	vtkSmartPointer<vtkBorderCallback> borderCallback = vtkSmartPointer<vtkBorderCallback>::New();
	borderCallback->SetImageRenWin(renderWindow);
  	borderCallback->SetRenderer(renderer);
  	borderCallback->SetImageActor(actor);

	//actor bounds
	actor->GetDisplayExtent(bounds);
	actorXmin = bounds[0];
	actorXmax = bounds[1];
	actorYmin = bounds[2];
	actorYmax = bounds[3];
	actorZmin = bounds[4];
	actorZmax = bounds[5];
	//std::cout<<endl<<"Xmin: "<<actorXmin<<endl<<"Xmax: "<<actorXmax<<endl<<"Ymin: "<<actorYmin<<endl<<"Ymax:"<<actorYmax<<endl<<"Zmin: "<<actorZmin<<endl<<"Zmax: "<<actorZmax<<endl;

	actor->GetPosition(position);
	//std::cout<<"Position: "<<position[0]<<"	"<<position[1]<<" "<<position[2]<<endl;
	double *AOrigin = actor->GetOrigin();
	//std::cout<<"Actor Origin: "<<AOrigin[0]<<"\t"<<AOrigin[1]<<"\t"<<AOrigin[2]<<endl;

	//renderer origin and size
	int* pOrigin = renderer->GetOrigin();
	int* pSize = renderer->GetSize(); 
	//std::cout<<"renderer Size: "<<pSize[0]<<"\t"<<pSize[1]<<endl<<"renderer origin: "<<pOrigin[0]<<"\t"<<pOrigin[1]<<"\t"<<pOrigin[2]<<endl;

  	borderWidget->AddObserver(vtkCommand::InteractionEvent,borderCallback);

	// Add the actors to the scene
  	renderer->AddActor(actor);
  	borderWidget->On();
  	renderWindowInteractor->Start();

//Size of the window
	winszX = ceil(renderWindow->GetSize()[0]);
	winszY = ceil(renderWindow->GetSize()[1]);
	//cout<<endl<<"Window Size: "<<winszX<<" "<<winszY<<endl;


//Width and Height to be used later
	upperRightXold = borderCallback->upperRight[0]+(0);
	upperRightYold = borderCallback->upperRight[1]+(0);
	upperRightZold = borderCallback->upperRight[2]+(0);
	lowerLeftXold = borderCallback->lowerLeft[0]-(0);
	lowerLeftYold = borderCallback->lowerLeft[1]-(0);
	lowerLeftZold = borderCallback->lowerLeft[2]-(0);


	//cout<<endl<<"Old Upper Right Coordinate in screen coordinates: "<<upperRightXold<<"	"<<upperRightYold<<endl;
	URtempY = 0+(winszY - upperRightYold);
	convertCoordinates(upperRightXold, URtempY,upperRightZold);
	upperRightY = ceil(newY);
	upperRightX = ceil(newX);
	//cout<<endl<<"New Upper Right Coordinate in pixel coordinates: "<<upperRightX<<" "<<upperRightY<<endl;
	cout<<endl<<"Lower Right Coordinates: "<<upperRightX<<" "<<upperRightY<<endl;
	
	//cout<<endl<<"Old Lower Left Coordinate in screen coordinates: "<<lowerLeftXold<<" "<<lowerLeftYold<<endl;
	convertCoordinates(lowerLeftXold, lowerLeftYold,lowerLeftZold);
	LLtempX = newX;
	LLtempY = newY;
	//cout<<endl<<"New Lower Left Coordinate in pixel coordinates: "<<LLtempX<<" "<<LLtempY<<endl;
	//wx = LLtempX - 0;
	wy = LLtempY - 0;
	//lowerLeftX = sizeitkimage0 - wx;
	lowerLeftY = ceil(sizeitkimage1 - wy);
	lowerLeftX = ceil(LLtempX);
	//cout<<endl<<"New Lower Left Coordinates after translating the origin: "<<lowerLeftX<<" "<<lowerLeftY<<endl;
	cout<<endl<<"Upper Left Coordinates: "<<LLtempX<<" "<<LLtempY<<endl;
	
	width = ceil(upperRightX - lowerLeftX);
	height = ceil(lowerLeftY - upperRightY);
	//cout<<endl<<"Height: "<<height<<"\t Width: "<<width<<endl;


//Select point which will also serve as the seed

	std::cout<<endl<<"Select a point to serve as the seed point and press Q to continue..."<<endl;
	//std::cout<<endl<<"Picking seed point..."<<endl;	
	//std::cout << "Picking pixel: " << renderWindowInteractor->GetEventPosition()[0] << " " << renderWindowInteractor->GetEventPosition()[1] << std::endl;
      	
	renderWindowInteractor->GetPicker()->Pick(renderWindowInteractor->GetEventPosition()[0], renderWindowInteractor->GetEventPosition()[1], 0, 			   renderWindowInteractor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
      

	vtkSmartPointer<vtkPointPicker> pointpicker = vtkSmartPointer<vtkPointPicker>::New();
	renderWindowInteractor->SetPicker(pointpicker);

	std::string message = "Location: ( ";
        message += vtkVariant( renderWindowInteractor->GetEventPosition()[0] ).ToString();
        message += ", ";
        message += vtkVariant( renderWindowInteractor->GetEventPosition()[1] ).ToString();
        message += " ) ";
	
	//std::cout<<message<<endl;

//Annotations
	vtkSmartPointer<vtkCornerAnnotation> annotation = vtkSmartPointer<vtkCornerAnnotation>::New();
	annotation->SetText(0, message.c_str());
	annotation->GetTextProperty()->SetColor(1,0,0);
	
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> style2 = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	renderWindowInteractor->SetInteractorStyle(style2);
	renderWindowInteractor->SetRenderWindow(renderWindow.Get());
	
  	renderWindowInteractor->Start();
	renderer->AddViewProp(annotation);
	renderWindow->Render();
	 
//Seed points to be used later
	
	seedX = (renderWindowInteractor->GetEventPosition()[0]);
	seedY = (renderWindowInteractor->GetEventPosition()[1]);
	double seedZ = (renderWindowInteractor->GetEventPosition()[2]);

	//cout<<endl<<"Picked points in screen coordinates: "<<seedX<<endl<<seedY<<endl;
	convertCoordinates(seedX, seedY,seedZ);
	seedXnew = newX;
	seedYnew = newY;
	//std::cout<<endl << "Picked points in pixel coordinates: " << seedXnew << ", " << seedYnew << std::endl;

	//wx = seedXnew - 0;
	wy = seedYnew - 0;
	
	//seedXnew2 = sizeitkimage0 - wx;
	seedYnew2 = ceil(sizeitkimage1 - wy);	
	seedXnew2 = ceil(seedXnew);

	//std::cout<<endl << "Picked points in pixel coordinates: " << seedXnew2 << ", " << seedYnew2 << std::endl;
	std::cout<<endl << "Location of picked seed point : (" << seedXnew2 << ", " << seedYnew2 <<")"<< std::endl;
//Message
	std::cout<<"Interaction Complete..."<<endl<<"Press Apply filter to perform segmentation..."<<endl;
	

//VTK PROCESSING ENDS


}

void FileDialog::on_pushButton_2_clicked()
{
      FileDialog::close();
}

void FileDialog::on_doubleSpinBox_valueChanged(double arg1)
{

   //threshold = float(arg1);
    //ui->doubleSpinBox->setValue(threshold);	
}



void FileDialog::on_pushButton_3_clicked()
{
	//FILTERS APPLIED

//Message
	std::cout<<"Start of Segmentation..."<<endl;

//Extract the region of interest
	ROIFilterType::Pointer roifilter = ROIFilterType::New();
	InputImageType::IndexType start;
	start[0] = lowerLeftX;
	start[1] = upperRightY;

	InputImageType::RegionType desiredRegion;

	InputImageType::SizeType size;
	size[0] = width;
	size[1] = height;
 
  	desiredRegion.SetIndex(start);
  	desiredRegion.SetSize(size);
  	
	roifilter->SetExtractionRegion(desiredRegion);
  	roifilter->SetInput(itkreader->GetOutput());
	roifilter->SetDirectionCollapseToIdentity();
	roifilter->Update();

//Write the ROI in DICOM

   
   	WriterType::Pointer writerroi = WriterType::New();
   	writerroi->SetInput( roifilter->GetOutput() );
   	writerroi->SetFileName( "ROI.dcm" );   
   
   	writerroi->SetImageIO( gdcmImageIO );

  	try
    	{
    		writerroi->Update();
		std::cout<<"ROI Generated..."<<endl;
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
        }


//Write the ROI

	RescaleFilterType::Pointer rescalarROI = RescaleFilterType::New();
	rescalarROI->SetOutputMinimum(0);
	rescalarROI->SetOutputMaximum(255);
	rescalarROI->SetInput(roifilter->GetOutput());
	
	pngWriterType1::Pointer pngWriterROI = pngWriterType1::New();
	pngWriterROI->SetFileName("ROI.png");
	pngWriterROI->SetInput(rescalarROI->GetOutput());
	try
	{
		pngWriterROI->Update();
		std::cout<<"ROI Generated..."<<endl;
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
    		std::cerr << e << std::endl;
	}

//create marker image
	//Create the marker image
	InputImageType::RegionType region1;
	InputImageType::IndexType start1;
	start1[0] = start[0];
	start1[1] = start[1];

	InputImageType::Pointer itkimage = itkreader->GetOutput();
	InputImageType::SizeType size1;
	size1[0]=size[0];
	size1[1]=size[1];

	region1.SetSize(size1);
	region1.SetIndex(start1);
	InputImageType::Pointer markerimage = InputImageType::New();
	
	markerimage->SetRegions(region1);
	markerimage->Allocate();
	markerimage->FillBuffer(0);
	InputImageType::IndexType pixelindex;
	
	for(int i=seedXnew2-10;i<=seedXnew2+10;i++)
	{
		for(int j=seedYnew2-10;j<=seedYnew2+10;j++)
		{
			pixelindex[0] = i;
			pixelindex[1] = j;
			markerimage->SetPixel(pixelindex,255);
		}
	}
	

	seed2 = lowerLeftX - 20;
	seed3 = lowerLeftY - 20;	
	for(int i=seed2-5;i<=seed2+5;i++)
	{
		for(int j=seed3-5;j<=seed3+5;j++)
		{
			pixelindex[0] = i;
			pixelindex[1] = j;
			markerimage->SetPixel(pixelindex,255);
		}
	}

	CastFilterType::Pointer caster2 = CastFilterType::New();
	caster2->SetInput(markerimage);
	
	pngWriterType1::Pointer pngWriterM = pngWriterType1::New();
	pngWriterM->SetFileName("MI.png");
	pngWriterM->SetInput(caster2->GetOutput());
	try
	{
		pngWriterM->Update();
		std::cout<<"MI Generated..."<<endl;
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
    		std::cerr << e << std::endl;
	}

//WRITE the marker image in DICOM
   
   	WriterType::Pointer writerMI = WriterType::New();
   	writerMI->SetInput( markerimage );
   	writerMI->SetFileName( "MI.dcm" );   
   
   	writerMI->SetImageIO( gdcmImageIO );

  	try
    	{
    		writerMI->Update();
		std::cout<<"MI Generated..."<<endl;
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
        }



//FILTER 1 - gradient magnitude image filter
   	typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, InputImageType> MagGradientFilterType;
   	MagGradientFilterType::Pointer maggradfilter = MagGradientFilterType::New();
	maggradfilter->SetInput(roifilter->GetOutput());


//Write the gradient image to a png file
	RescaleFilterType::Pointer rescalar2 = RescaleFilterType::New();
	rescalar2->SetOutputMinimum(0);
	rescalar2->SetOutputMaximum(255);
	rescalar2->SetInput(maggradfilter->GetOutput());

	pngWriterType1::Pointer pngWriter2 = pngWriterType1::New();
	pngWriter2->SetFileName("GradientImage.png");
	pngWriter2->SetInput(rescalar2->GetOutput());
	try
	{
		pngWriter2->Update();
		std::cout<<"Gradient Image Generated..."<<endl;
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
    		std::cerr << e << std::endl;
	}

//WRITE DICOM IMAGE
   
   	WriterType::Pointer writer1 = WriterType::New();
   	writer1->SetInput( maggradfilter->GetOutput() );
   	writer1->SetFileName( "GradientImage.dcm" );   
   
   	writer1->SetImageIO( gdcmImageIO );

  	try
    	{
    		writer1->Update();
		std::cout<<"Gradient Image Generated..."<<endl;
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
        }



//FILTER 7 - MARKER BASED WATERSHED SEGMENTATION -

	//Resample the gradient image
	ResampleFilterType::Pointer resample = ResampleFilterType::New();
	resample->SetReferenceImage(markerimage);
	
	ResampleFilterType::OriginPointType ori;
	ori = resample->GetOutputOrigin();
	
	ResampleFilterType::SpacingType space;
	space = resample->GetOutputSpacing();
	
	InputImageType::Pointer resampledImage = maggradfilter->GetOutput();
	resampledImage->SetOrigin(ori);
	resampledImage->SetSpacing(space);
	
	//GENERATE MARKERS
  	typedef itk::Image<unsigned short, 2 > OutputImageType2;
  	typedef itk::ConnectedComponentImageFilter <InputImageType, OutputImageType2 > ConnectedComponentImageFilterType;
  	ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New();
  	connected->SetInput(markerimage);
  	connected->Update();
  	std::cout << "Number of objects: " << connected->GetObjectCount() << std::endl;

  	typedef itk::LabelToRGBImageFilter<OutputImageType2, OutputImageType> RGBFilterType;
  	RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
  	rgbFilter->SetInput( connected->GetOutput() );

  	//WRITE THE MARKER IMAGE
  	typedef itk::ImageFileWriter< OutputImageType2 > WriterType2;
  	WriterType2::Pointer writer7 = WriterType2::New();
  	writer7->SetInput(connected->GetOutput() );
  	writer7->SetFileName( "connected-colour.dcm" );   
  	writer7->SetImageIO( gdcmImageIO );
  	try
    	{
    		writer7->Update();
		std::cout<<"Marker Image Generated..."<<endl;
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}
 

  	//MARKER CONTROLLED WATERSHED
 
  	typedef itk::MorphologicalWatershedFromMarkersImageFilter<InputImageType, OutputImageType> MarkerWatershedType;
  	MarkerWatershedType::Pointer markerwatershed = MarkerWatershedType::New();
  	markerwatershed->SetMarkerImage(rgbFilter->GetOutput());
  	markerwatershed->SetInput(resampledImage);

   	//WRITE THE OUTPUT OF WATERSHED TO A PNG FILE
   	typedef itk::ImageFileWriter<OutputImageType> FileWriterType2;
   	FileWriterType2::Pointer writer8 = FileWriterType2::New();
   	writer8->SetInput( markerwatershed->GetOutput() );
   	writer8->SetFileName( "watershed.png" );   
   	//writer8->SetImageIO( gdcmImageIO );

  	try
    	{
    		writer8->Update();
		std::cout<<"Watershed Image Generated..."<<endl;
		std::cout<<"Segmentation Complete..."<<endl<<"Press Q to continue..."<<endl;
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}
	

	//Display the output in a widget
	vtkSmartPointer<vtkPNGReader> originalImage = vtkSmartPointer<vtkPNGReader>::New(); //Read ROI
	originalImage->SetFileName("ROI.png");
	vtkSmartPointer<vtkPNGReader> segResult = vtkSmartPointer<vtkPNGReader>::New(); //Read Segmentation result
  	segResult->SetFileName("watershed.png");

	// Combine the images (blend takes multiple connections on the 0th input port)
  	vtkSmartPointer<vtkImageBlend> blend = vtkSmartPointer<vtkImageBlend>::New();
	blend->AddInputConnection(segResult->GetOutputPort());
  	blend->AddInputConnection(originalImage->GetOutputPort());
	//blend->SetBlendModeToCompound();
  	blend->SetOpacity(0,.3);
  	blend->SetOpacity(1,.8);

	//Visualise
	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractorBlend = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  	vtkSmartPointer<vtkImageViewer2> imageViewerBlend = vtkSmartPointer<vtkImageViewer2>::New();
  	imageViewerBlend->SetInputConnection(blend->GetOutputPort());
  	imageViewerBlend->SetupInteractor(renderWindowInteractorBlend);
  	imageViewerBlend->GetRenderer()->ResetCamera();
  	renderWindowInteractorBlend->Initialize();
  	renderWindowInteractorBlend->Start();

}

void FileDialog::convertCoordinates(double oldX,double oldY,double oldZ)
{
	newX = 0 + ((oldX - 0) / winszX ) * sizeitkimage0;
	newY = 0 + ((oldY - 0) / winszY ) * sizeitkimage1;
}
