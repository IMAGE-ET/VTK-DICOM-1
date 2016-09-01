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

	/*RescaleFilterType::Pointer rescalar = RescaleFilterType::New();
	rescalar->SetOutputMinimum(0);
	rescalar->SetOutputMaximum(255);*/
	CastFilterType::Pointer caster = CastFilterType::New();
	caster->SetInput(itkreader->GetOutput());

	pngWriterType1::Pointer pngWriter1 = pngWriterType1::New();
	pngWriter1->SetFileName("OriginalPNG.png");
	pngWriter1->SetInput(caster->GetOutput());
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
	int sizeitkimage0 = itkreader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
	int sizeitkimage1 = itkreader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
	std::cout<<sizeitkimage0<<endl<<sizeitkimage1;
	
//Flip the image
	itk::FixedArray<bool, 2> flipAxes;
	flipAxes[0] = false;
	flipAxes[1] = true;
	FlipImageFilterType::Pointer flipfilter = FlipImageFilterType::New();
	flipfilter->SetInput(itkreader->GetOutput());
	flipfilter->SetFlipAxes(flipAxes);


//Convert the Image to a VTK Image

	//Connector ITK to VTK
	typedef itk::ImageToVTKImageFilter<InputImageType>  ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(flipfilter->GetOutput());
	connector->Update();



//Drawing the rectangle
	//Actor
	vtkSmartPointer<vtkImageActor> actor =  vtkSmartPointer<vtkImageActor>::New();
	actor->GetMapper()->SetInputData(connector->GetOutput());

	//Renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
 
	//Renderer Window
  	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
	//renderWindow->SetSize(sizeitkimage0,sizeitkimage1);
	//renderWindow->SetSize(700,700);
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
  	borderCallback->SetRenderer(renderer);
  	borderCallback->SetImageActor(actor);
 
  	borderWidget->AddObserver(vtkCommand::InteractionEvent,borderCallback);

	// Add the actors to the scene
  	renderer->AddActor(actor);
	renderer->SetBackground(1,1,1);
	//renderWindow->SetFullScreen(true);
  	//renderWindow->Render();
  	//renderWindowInteractor->Initialize();
  	//renderWindow->Render();
  	borderWidget->On();
  	renderWindowInteractor->Start();


//Select point which will also serve as the seed
	/*vtkSmartPointer<vtkPropPicker> pointpicker = vtkSmartPointer<vtkPropPicker>::New();	
	pointpicker->PickProp(renderWindowInteractor->GetEventPosition()[0], renderWindowInteractor->GetEventPosition()[1],renderer);
	double pos[3];
	pointpicker->GetPickPosition(pos);
	
	std::string message = "Location: ( ";
        message += vtkVariant( pos[0] ).ToString();
        message += ", ";
        message += vtkVariant( pos[1] ).ToString();
        message += " ) ";
	
	std::cout<<message<<endl;*/


	std::cout << "Picking pixel: " << renderWindowInteractor->GetEventPosition()[0] << " " << renderWindowInteractor->GetEventPosition()[1] << std::endl;
      	renderWindowInteractor->GetPicker()->Pick(renderWindowInteractor->GetEventPosition()[0], renderWindowInteractor->GetEventPosition()[1], 0, renderWindowInteractor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
      
	/*double picked[3];
	renderWindowInteractor->GetPicker()->GetPickPosition(picked);
        std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;*/

	vtkSmartPointer<vtkPointPicker> pointpicker = vtkSmartPointer<vtkPointPicker>::New();
	renderWindowInteractor->SetPicker(pointpicker);

	std::string message = "Location: ( ";
        message += vtkVariant( renderWindowInteractor->GetEventPosition()[0] ).ToString();
        message += ", ";
        message += vtkVariant( renderWindowInteractor->GetEventPosition()[1] ).ToString();
        message += " ) ";
	
	std::cout<<message<<endl;

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
	
	//Width and Height to be used later
	width = (borderCallback->upperRight[0] - borderCallback->lowerLeft[0])*2.5;
	height = (borderCallback->upperRight[1] - borderCallback->lowerLeft[0])*2.5;
	lowerLeftX = borderCallback->lowerLeft[0];
	lowerLeftY = borderCallback->lowerLeft[1];
 
	//Seed points to be used later
	
	seedX = (renderWindowInteractor->GetEventPosition()[0])*1.5;
	seedY = (renderWindowInteractor->GetEventPosition()[1])*1.5;
	cout<<seedX<<endl<<seedY<<endl;

//Message
	std::cout<<"Interaction Complete..."<<endl;

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
    //s2 = ui->lineEdit_2->text();
    //setLevel = s2.toFloat();
	
	//FILTERS APPLIED

//Message
	std::cout<<"Start of Segmentation..."<<endl;

//Extract the region of interest
	ROIFilterType::Pointer roifilter = ROIFilterType::New();
	InputImageType::IndexType start;
	//start[0] = lowerLeftX;
	//start[1] = lowerLeftY;
	start[0] = 0;
	start[1] = 0;

	InputImageType::SizeType size;
	size[0] = width;
	size[1] = height;
	//size[0] = 400;
	//size[1] = 400;
 
  	InputImageType::RegionType desiredRegion;
  	desiredRegion.SetSize(size);
  	desiredRegion.SetIndex(start);
 
  	roifilter->SetRegionOfInterest(desiredRegion);
  	roifilter->SetInput(itkreader->GetOutput());
	//InputImageType::Pointer ROI = roifilter->GetOutput();

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

	/*RescaleFilterType::Pointer rescalarROI = RescaleFilterType::New();
	rescalarROI->SetOutputMinimum(0);
	rescalarROI->SetOutputMaximum(255);*/
	CastFilterType::Pointer caster1 = CastFilterType::New();
	caster1->SetInput(roifilter->GetOutput());
	
	pngWriterType1::Pointer pngWriterROI = pngWriterType1::New();
	pngWriterROI->SetFileName("ROI.png");
	pngWriterROI->SetInput(caster1->GetOutput());
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
	//region1= itkimage->GetLargestPossibleRegion();
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
	
	for(int i=seedX-30;i<=seedX+30;i++)
	{
		for(int j=seedY-30;j<=seedY+30;j++)
		{
			pixelindex[0] = i;
			pixelindex[1] = j;
			markerimage->SetPixel(pixelindex,255);
		}
	}
	

	/*RescaleFilterType::Pointer rescalarM = RescaleFilterType::New();
	rescalarM->SetOutputMinimum(0);
	rescalarM->SetOutputMaximum(255);*/
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
   	//maggradfilter->SetInput(itkreader->GetOutput());
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
	connected->FullyConnectedOn();
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
  	markerwatershed->SetInput(maggradfilter->GetOutput());
   
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
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}
	std::cout<<"Segmentation Complete..."<<endl;
}
