#include "filedialog.h"
#include "ui_filedialog.h"

#include "qfiledialog.h"
#include "itkImage.h"
//#include "itkImageFileReader.h"
#include <iostream>
#include "itkSize.h"
//#include "itkImageFileWriter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
//#include "itkGradientMagnitudeImageFilter.h"
#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkWatershedImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"


//Reading DICOM using ITK
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"

//Morphological Operations
#include "itkBinaryBallStructuringElement.h"
//#include <itkRegionalMaximaImageFilter.h>

#include <itkInvertIntensityImageFilter.h>
#include "itkGrayscaleErodeImageFilter.h"
//#include "itkThresholdImageFilter.h"
#include <itkSignedMaurerDistanceMapImageFilter.h>
//#include "itkMinimaImpositionImageFilter.h"
#include <itkBinaryThresholdImageFilter.h>

#include <itkReconstructionByDilationImageFilter.h>
#include <itkReconstructionByErosionImageFilter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>

#include <itkMorphologicalWatershedImageFilter.h>
//#include "itkRegionalMinimaImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include <itkGrayscaleDilateImageFilter.h>
#include "itkLabelToRGBImageFilter.h"

#include "itkScalarToRGBColormapImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include "itkFlipImageFilter.h"


//vtk headers
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <itkImageToVTKImageFilter.h>
 
#include "vtkVersion.h"
#include "vtkImageViewer.h"
#include "vtkImageMapper3D.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"

#include <vtkActor.h>
#include <vtkAssemblyNode.h>
#include <vtkAssemblyPath.h>
#include <vtkBorderRepresentation.h>
#include <vtkCommand.h>
#include <vtkBorderWidget.h>
#include "vtkBorderCallback.h"
#include <vtkCoordinate.h>
#include <vtkPolyData.h>
#include <vtkPropPicker.h>
#include <vtkProperty2D.h>
#include <vtkPointData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include <vtkImageData.h>
#include <vtkCell.h>
#include <vtkImageChangeInformation.h>
#include <vtkPNGReader.h>
#include <vtkViewport.h>



//#include <vtkImageFlip.h>
//#include <itkVTKImageToImageFilter.h>
//#include "QuickView.h"
//#include <itkImageRegionConstIteratorWithIndex.h>



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
    const char* p = ipfile.c_str();


//Read DICOM images using ITK
	typedef itk::ImageFileReader< InputImageType > ReaderType;
	ReaderType::Pointer itkreader = ReaderType::New();
	itkreader->SetFileName( ipfile );
	typedef itk::GDCMImageIO ImageIOType;
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

//Convert it into png

	typedef unsigned char WriterPixelType1;
	typedef itk::Image<WriterPixelType1,2> WriterTypeShr;

	typedef itk::RescaleIntensityImageFilter<InputImageType, WriterTypeShr> RescaleFilterType;
	RescaleFilterType::Pointer rescalar = RescaleFilterType::New();
	rescalar->SetOutputMinimum(0);
	rescalar->SetOutputMaximum(255);
	rescalar->SetInput(itkreader->GetOutput());

	typedef itk::ImageFileWriter<WriterTypeShr> pngWriterType1;
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

//Flip the image
	itk::FixedArray<bool, 2> flipAxes;
	flipAxes[0] = false;
	flipAxes[1] = true;
	typedef itk::FlipImageFilter<InputImageType> FlipImageFilterType;
	FlipImageFilterType::Pointer flipfilter = FlipImageFilterType::New();
	flipfilter->SetInput(itkreader->GetOutput());
	flipfilter->SetFlipAxes(flipAxes);

//Convert the Image to a VTK Image

	//Connector ITK to VTK
	typedef itk::ImageToVTKImageFilter<InputImageType>  ConnectorType;
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(flipfilter->GetOutput());
	connector->Update();

//Read the png file
  	/*vtkSmartPointer<vtkPNGReader> pngreader = vtkSmartPointer<vtkPNGReader>::New();
  	pngreader->SetFileName("OriginalPNG.png");
	pngreader->Update();

//Change (0,0) from bottom left to image centre
	vtkSmartPointer<vtkImageChangeInformation> changeInformation = vtkSmartPointer<vtkImageChangeInformation>::New();
	changeInformation->SetCenterImage(1);
changeInformation->SetInputConnection(pngreader->GetOutputPort());
	changeInformation->Update();*/


//Drawing the rectangle
	//Actor
	vtkSmartPointer<vtkImageActor> actor =  vtkSmartPointer<vtkImageActor>::New();
	actor->GetMapper()->SetInputData(connector->GetOutput());
	/*#if VTK_MAJOR_VERSION <= 5
  		actor->SetInput(changeInformation->GetOutput());
	#else
  		//changeInformation->Update();
  		actor->GetMapper()->SetInputData(changeInformation->GetOutput());
	#endif*/
	
	//Renderer
	vtkSmartPointer<vtkRenderer> renderer = vtkSmartPointer<vtkRenderer>::New();
 
	//Renderer Window
  	vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
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
  	renderWindow->Render();
  	renderWindowInteractor->Initialize();
  	renderWindow->Render();
  	borderWidget->On();
  	renderWindowInteractor->Start();

//Select point which will also serve as the seed
	vtkSmartPointer<vtkImageData> imagevtk = vtkSmartPointer<vtkImageData>::New();
	imagevtk = connector->GetOutput();
	vtkSmartPointer<vtkPropPicker> pointpicker = vtkSmartPointer<vtkPropPicker>::New();
	pointpicker->PickProp(renderWindowInteractor->GetEventPosition()[0], renderWindowInteractor->GetEventPosition()[1],renderer);
	double pos[2];
	pointpicker->GetPickPosition(pos);
	
	std::string message = "Location: ( ";
        message += vtkVariant( pos[0] ).ToString();
        message += ", ";
        message += vtkVariant( pos[1] ).ToString();
        message += " ) ";
	
	std::cout<<message<<endl;

//Convert to display coordinates
	/*vtkSmartPointer<vtkCoordinate> coordinate = vtkSmartPointer<vtkCoordinate>::New();
	coordinate->SetValue(pos);
	coordinate->SetCoordinateSystemToDisplay();
	double *displayPos = coordinate->GetValue();

	std::string message1 = "Location: ( ";
        message1 += vtkVariant( displayPos[0] ).ToString();
        message1 += ", ";
        message1 += vtkVariant( displayPos[1] ).ToString();
        message1 += " ) ";
	
	std::cout<<message1<<endl;*/


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


//Extract the region of interest
	InputImageType::SizeType inSize = itkreader->GetOutput()->GetLargestPossibleRegion().GetSize();
	typedef itk::RegionOfInterestImageFilter< InputImageType,InputImageType >ROIFilterType;
	ROIFilterType::Pointer roifilter = ROIFilterType::New();
	InputImageType::IndexType start;
	start[0] = borderCallback->lowerLeft[0];
	start[1] = borderCallback->lowerLeft[1];
	
	int width, height;
	width = borderCallback->upperRight[0] - borderCallback->lowerLeft[0];
	height = borderCallback->upperRight[1] - borderCallback->lowerLeft[0];
 
	InputImageType::SizeType size;
	size[0] = width*2;
	size[1] = height*2;
 
  	InputImageType::RegionType desiredRegion;
  	desiredRegion.SetSize(size);
  	desiredRegion.SetIndex(start);
 
  	roifilter->SetRegionOfInterest(desiredRegion);
  	roifilter->SetInput(itkreader->GetOutput());

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
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
    		std::cerr << e << std::endl;
	}


 
//Create the marker image

	InputImageType::RegionType region1;
	InputImageType::IndexType start1;
	start1[0] = start[0];
	start1[1] = start[1];

	//InputImageType::Pointer itkimage = roifilter->GetOutput();
	//region1= itkimage->GetLargestPossibleRegion();
	InputImageType::SizeType size1;
	size1[0]=size[0];
	size1[1]=size[1];

	region1.SetSize(size1);
	region1.SetIndex(start1);
	InputImageType::Pointer markerimage = InputImageType::New();
	markerimage->SetRegions(region1);
	markerimage->Allocate();

	InputImageType::IndexType pixelindex;
	pixelindex[0] = pos[0]+250;
	pixelindex[1] = pos[1]+250;
	markerimage->SetPixel(pixelindex,255);


	RescaleFilterType::Pointer rescalarM = RescaleFilterType::New();
	rescalarM->SetOutputMinimum(0);
	rescalarM->SetOutputMaximum(255);
	rescalarM->SetInput(markerimage);
	
	pngWriterType1::Pointer pngWriterM = pngWriterType1::New();
	pngWriterM->SetFileName("MarkerImage.png");
	pngWriterM->SetInput(rescalarM->GetOutput());
	try
	{
		pngWriterM->Update();
	}
	catch(itk::ExceptionObject & e)
	{
		std::cerr << "Exception in file reader " << std::endl;
    		std::cerr << e << std::endl;
	}



//FILTER 1 - gradient magnitude image filter
	//typedef itk::GradientMagnitudeImageFilter<InputImageType, InputImageType> MagGradientFilterType;
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
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
        }



//AUTOMATIC MARKER

	//FILTER 2 - OPENING BY RECONSTRUCTION

	//create the structuring element
	typedef itk::FlatStructuringElement<2> StructuringElementType;
	StructuringElementType::RadiusType elementRadius;
	elementRadius.Fill(1);
	StructuringElementType structuringElement = StructuringElementType::Ball(elementRadius);
 
	//Erosion of an image
	typedef itk::GrayscaleErodeImageFilter <InputImageType, InputImageType, StructuringElementType> GrayscaleErodeImageFilterType;
	GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
	erodeFilter->SetInput(roifilter->GetOutput());
	//erodeFilter->SetInput(itkreader->GetOutput());
	erodeFilter->SetKernel(structuringElement);

	//Reconstruction By Dilation
	typedef itk::ReconstructionByDilationImageFilter< InputImageType, InputImageType > ReconstructionDilationType;
	ReconstructionDilationType::Pointer reconstructionDilation = ReconstructionDilationType::New();
	reconstructionDilation->SetMaskImage(roifilter->GetOutput());
	//reconstructionDilation->SetMaskImage(itkreader->GetOutput());
	reconstructionDilation->SetMarkerImage(erodeFilter->GetOutput());
	reconstructionDilation->SetFullyConnected('true');
	reconstructionDilation->Update();

	//WRITE DICOM IMAGE
   
	WriterType::Pointer writer2 = WriterType::New();
	writer2->SetInput( reconstructionDilation->GetOutput() );
	writer2->SetFileName( "OpenReconstruction.dcm" );   
   
	writer2->SetImageIO( gdcmImageIO );

	try
	{
    		writer2->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}


	//FILTER 3 - CLOSING BY RECONSTRUCTION

	//Dilate the output of the open by reconstruction
	elementRadius.Fill(3);
	StructuringElementType structuringElement2 = StructuringElementType::Ball(elementRadius);

	typedef itk::GrayscaleDilateImageFilter <InputImageType, InputImageType, StructuringElementType> GrayscaleDilateImageFilterType;
	GrayscaleDilateImageFilterType::Pointer dilateFilter = GrayscaleDilateImageFilterType::New();
	dilateFilter->SetInput(reconstructionDilation->GetOutput());
	dilateFilter->SetKernel(structuringElement2);

	typedef itk::ReconstructionByErosionImageFilter< InputImageType, InputImageType > ReconstructionErosionType;
	ReconstructionErosionType::Pointer reconstructionErosion = ReconstructionErosionType::New();
	reconstructionErosion->SetMaskImage(reconstructionDilation->GetOutput());
	reconstructionErosion->SetMarkerImage(dilateFilter->GetOutput());
	reconstructionErosion->SetFullyConnected('true');
	reconstructionErosion->Update();


	//WRITE DICOM IMAGE
   	WriterType::Pointer writer3 = WriterType::New();
   	writer3->SetInput( reconstructionErosion->GetOutput() );
   	writer3->SetFileName( "CloseReconstruction.dcm" );   
   
   	writer3->SetImageIO( gdcmImageIO );

  	try
    	{
    		writer3->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}


	//FILTER 4 - MORPHOLOGICAL CLOSING
	typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType, InputImageType, StructuringElementType> MorphClosingFilterType;
	MorphClosingFilterType::Pointer MorphologicalClosingFilter = MorphClosingFilterType::New();
	MorphologicalClosingFilter->SetKernel(structuringElement2);
	MorphologicalClosingFilter->SetInput(reconstructionErosion->GetOutput());


	//WRITE DICOM IMAGE
   	WriterType::Pointer writer4 = WriterType::New();
   	writer4->SetInput( MorphologicalClosingFilter->GetOutput() );
   	writer4->SetFileName( "MorpClose.dcm" );   
   	writer4->SetImageIO( gdcmImageIO );
  	try
    	{
    		writer4->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}


	//FILTER 5 - MORPHOLOGICAL ERODING
	GrayscaleErodeImageFilterType::Pointer erodeFilter2 = GrayscaleErodeImageFilterType::New();
	erodeFilter2->SetKernel(structuringElement2);
	erodeFilter2->SetInput(MorphologicalClosingFilter->GetOutput());


	//WRITE DICOM IMAGE

   
   	WriterType::Pointer writer5 = WriterType::New();
   	writer5->SetInput( erodeFilter2->GetOutput() );
   	writer5->SetFileName( "morphErode.dcm" );   
   	writer5->SetImageIO( gdcmImageIO );
  	try
    	{
    		writer5->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}


	//FILTER 6 - Thresholding an image
	typedef itk::BinaryThresholdImageFilter <InputImageType, InputImageType> ThresholdImageFilterType;
	ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
	InputPixelType background = 0;
	InputPixelType foreground = 255;
	thresholdFilter->SetLowerThreshold(10);
  	thresholdFilter->SetUpperThreshold(230);
  	thresholdFilter->SetInsideValue(255);
  	thresholdFilter->SetOutsideValue(0);
	thresholdFilter->SetInput( erodeFilter2->GetOutput() );

	//WRITE THE THRESHOLDED IMAGE
   	WriterType::Pointer writer6 = WriterType::New();
   	writer6->SetInput( thresholdFilter->GetOutput() );
   	writer6->SetFileName( "threshold.dcm" );   
   	writer6->SetImageIO( gdcmImageIO );
  	try
    	{
    		writer6->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
	}






	//FILTER 7 - MARKER BASED WATERSHED SEGMENTATION -

	
	//GENERATE MARKERS
  	typedef itk::Image<unsigned short, 2 > OutputImageType2;
  	typedef itk::ConnectedComponentImageFilter <InputImageType, OutputImageType2 > ConnectedComponentImageFilterType;
  	ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New();
  	connected->SetInput(thresholdFilter->GetOutput());
	//connected->SetInput(markerimage);
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
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}


	//WRITE THE ORIGINAL DICOM IMAGE
   
   	WriterType::Pointer writer = WriterType::New();
   	writer->SetInput( itkreader->GetOutput() );
   	writer->SetFileName( "original.dcm" );   
  	// writer->SetInput(erodeFilter2->GetOutput());
   	writer->SetImageIO( gdcmImageIO );

  	try
    	{
    		writer->Update();
    	}
  	catch( itk::ExceptionObject & error )
    	{
    		std::cerr << "Error: " << error << std::endl;
    	}

}

void FileDialog::on_pushButton_2_clicked()
{
      FileDialog::close();
}

void FileDialog::on_doubleSpinBox_valueChanged(double arg1)
{

    threshold = float(arg1);
    ui->doubleSpinBox->setValue(threshold);

}



void FileDialog::on_pushButton_3_clicked()
{
    s2 = ui->lineEdit_2->text();
    setLevel = s2.toFloat();
}
