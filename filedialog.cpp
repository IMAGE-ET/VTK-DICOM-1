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
#include "itkGDCMImageIO.h"

//Reading DICOM using ITK
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"

//Morphological Operations
#include "itkBinaryBallStructuringElement.h"
#include <itkOpeningByReconstructionImageFilter.h>
#include <itkClosingByReconstructionImageFilter.h>
#include <itkRegionalMaximaImageFilter.h>
//#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkInvertIntensityImageFilter.h>
#include "itkGrayscaleErodeImageFilter.h"
//#include "itkThresholdImageFilter.h"
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include "itkMinimaImpositionImageFilter.h"
#include <itkBinaryThresholdImageFilter.h>

#include <itkReconstructionByDilationImageFilter.h>
#include <itkReconstructionByErosionImageFilter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>

//vtk headers
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>

#include <itkVTKImageToImageFilter.h>
#include "QuickView.h"

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
/*
// Read all the DICOM files in the specified directory.
   vtkSmartPointer<vtkDICOMImageReader> DICOMreader = vtkSmartPointer<vtkDICOMImageReader>::New();
   DICOMreader->SetFileName(p);
   DICOMreader->Update();

// Visualize
  vtkSmartPointer<vtkImageViewer2> imageViewer = vtkSmartPointer<vtkImageViewer2>::New();
  imageViewer->SetInputConnection(DICOMreader->GetOutputPort());
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
  imageViewer->SetupInteractor(renderWindowInteractor);
  imageViewer->Render();
  imageViewer->GetRenderer()->ResetCamera();
  imageViewer->Render();
 
  renderWindowInteractor->Start();

//VTK to ITK

  typedef itk::VTKImageToImageFilter< InputImageType > FilterType;
  FilterType::Pointer filter = FilterType::New();
  filter->SetInput(DICOMreader->GetOutput());
  
  //InputImageType::ConstPointer myitkImage = filter->GetOutput();
/*try
     {
     filter->Update();
     }
   catch( itk::ExceptionObject & error )
     {
     std::cerr << "Error: " << error << std::endl;
     }
  filter->Print(std::cout);*/


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

//APPLY FILTERS

//FILTER 1 - gradient magnitude image filter
   //typedef itk::GradientMagnitudeImageFilter<InputImageType, InputImageType> MagGradientFilterType;
   typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<InputImageType, InputImageType> MagGradientFilterType;
   MagGradientFilterType::Pointer maggradfilter = MagGradientFilterType::New();
   maggradfilter->SetInput(itkreader->GetOutput());

//FILTER 2 - OPENING BY RECONSTRUCTION

/*typedef itk::InvertIntensityImageFilter< InputImageType, InputImageType > InvertFilterType;
InvertFilterType::Pointer invertFilterIP = InvertFilterType::New();
invertFilterIP->SetInput(itkreader->GetOutput());*/

//create the structuring element
typedef itk::FlatStructuringElement<2> StructuringElementType;
StructuringElementType::RadiusType elementRadius;
elementRadius.Fill(1);
StructuringElementType structuringElement = StructuringElementType::Box(elementRadius);
 
//Erosion of an image
typedef itk::GrayscaleErodeImageFilter <InputImageType, InputImageType, StructuringElementType> GrayscaleErodeImageFilterType;
GrayscaleErodeImageFilterType::Pointer erodeFilter = GrayscaleErodeImageFilterType::New();
//erodeFilter->SetInput(invertFilterIP->GetOutput());
erodeFilter->SetInput(itkreader->GetOutput());
erodeFilter->SetKernel(structuringElement);

//Reconstruction By Dilation
typedef itk::ReconstructionByDilationImageFilter< InputImageType, InputImageType > ReconstructionDilationType;
ReconstructionDilationType::Pointer reconstructionDilation = ReconstructionDilationType::New();
reconstructionDilation->SetMaskImage(itkreader->GetOutput());
reconstructionDilation->SetMarkerImage(erodeFilter->GetOutput());
reconstructionDilation->SetFullyConnected('true');
reconstructionDilation->Update();

/*//apply open by reconstruction
typedef itk::OpeningByReconstructionImageFilter<InputImageType, InputImageType, StructuringElementType> OpenByReconstructionFilterType;
OpenByReconstructionFilterType::Pointer OpenReconstruct = OpenByReconstructionFilterType::New();
//OpenReconstruct->SetKernel(itkreader->GetOutput());
OpenReconstruct->SetKernel(structuringElement);
OpenReconstruct->SetInput(erodeFilter->GetOutput());
OpenReconstruct->Update();*/



//FILTER 3 - CLOSING BY RECONSTRUCTION

//Dilate the output of the open by reconstruction
elementRadius.Fill(3);
StructuringElementType structuringElement2 = StructuringElementType::Box(elementRadius);

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

// invert open by reconstruction output
//typedef itk::InvertIntensityImageFilter< InputImageType, InputImageType > InvertFilterType;
/*InvertFilterType::Pointer invertFilterOR = InvertFilterType::New();
invertFilterOR->SetInput(OpenReconstruct->GetOutput());*/

//invert open by reconstruction dialation
/*InvertFilterType::Pointer invertFilterORD = InvertFilterType::New();
invertFilterORD->SetInput(dilateFilter->GetOutput());

//closing by reconstruction
typedef itk::ClosingByReconstructionImageFilter<InputImageType, InputImageType, StructuringElementType> ClosingByReconstructionFilterType;
ClosingByReconstructionFilterType::Pointer CloseReconstruct = ClosingByReconstructionFilterType::New();
//CloseReconstruct->SetKernel(invertFilterOR->GetOutput());
CloseReconstruct->SetKernel(structuringElement2);
CloseReconstruct->SetInput(invertFilterORD->GetOutput());
CloseReconstruct->Update();

//invert the output of closing by reconstruction
typedef itk::InvertIntensityImageFilter< InputImageType, InputImageType > InvertFilterType;
InvertFilterType::Pointer invertFilterCR = InvertFilterType::New();
invertFilterCR->SetInput(reconstructionErosion->GetOutput());
invertFilterCR->Update();*/


//FILTER 4 - Regional Maxima Of Opening-Closing By Reconstruction
/*typedef itk::RegionalMinimaImageFilter<InputImageType, InputImageType> RegionalMaximaImageFilterType;
RegionalMaximaImageFilterType::Pointer RegionalMaxima = RegionalMaximaImageFilterType::New();
RegionalMaxima->SetFullyConnected('true');
RegionalMaxima->SetInput(reconstructionErosion->GetOutput());
RegionalMaxima->Update();*/

//FILTER 5 - Morphological Closing
typedef itk::GrayscaleMorphologicalClosingImageFilter<InputImageType, InputImageType, StructuringElementType> MorphClosingFilterType;
MorphClosingFilterType::Pointer MorphologicalClosingFilter = MorphClosingFilterType::New();
MorphologicalClosingFilter->SetKernel(structuringElement2);
MorphologicalClosingFilter->SetInput(reconstructionErosion->GetOutput());

//Morphological Eroding
GrayscaleErodeImageFilterType::Pointer erodeFilter2 = GrayscaleErodeImageFilterType::New();
erodeFilter2->SetKernel(structuringElement2);
erodeFilter2->SetInput(MorphologicalClosingFilter->GetOutput());


//FILTER 6 - Thresholding an image - gives the foreground markers
typedef itk::BinaryThresholdImageFilter <InputImageType, InputImageType> ThresholdImageFilterType;
ThresholdImageFilterType::Pointer thresholdFilter = ThresholdImageFilterType::New();
InputPixelType background = 0;
InputPixelType foreground = 255;
thresholdFilter->SetLowerThreshold(10);
  thresholdFilter->SetUpperThreshold(230);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);
thresholdFilter->SetInput( reconstructionErosion->GetOutput() );

//FILTER 7 - Distance transform of the CR image
typedef itk::SignedMaurerDistanceMapImageFilter< InputImageType, InputImageType > distanceMapFilterType;
distanceMapFilterType::Pointer distanceMap = distanceMapFilterType::New();
distanceMap->SetInput(thresholdFilter->GetOutput());

//FILTER 8 - Watershed of the distance transform - gives the background markers
typedef itk::WatershedImageFilter<InputImageType> WatershedFilterType;
   WatershedFilterType::Pointer watershed = WatershedFilterType::New();
   watershed->SetLevel(0.20);
   watershed->SetThreshold(0.50);
    watershed->SetInput(distanceMap->GetOutput()); //The output of WatershedImageFilter is an image of unsigned long integer labels
//OutputImageType::Pointer watershedOutput =

// create a new image depicting image background markers
InputImageType::Pointer watershedImage = itkreader->GetOutput();  
InputImageType::RegionType region = watershedImage->GetLargestPossibleRegion();
InputImageType::SizeType size = region.GetSize(); 
  InputImageType::IndexType start;
  start[0] = 0;
  start[1] = 0;
  region.SetSize(size);
  region.SetIndex(start);
InputImageType::Pointer bgm = InputImageType::New();
  bgm->SetRegions(region);
  bgm->Allocate();
/*for(unsigned int i=0;i<size[0];i++)
{
	for(unsigned int j=0;j<size[1];j++)
	{
		if(watershedOutput[i][j] == 0)
		{
			bgm[i][j] = 0;
		}
	}
}*/


typedef itk::Functor::ScalarToRGBPixelFunctor<unsigned long> ColorMapFunctorType;
   typedef WatershedFilterType::OutputImageType  LabeledImageType;
   typedef itk::UnaryFunctorImageFilter<LabeledImageType,OutputImageType,ColorMapFunctorType> ColorMapFilterType;

   ColorMapFilterType::Pointer colorMapFilter = ColorMapFilterType::New();
   colorMapFilter->SetInput(watershed->GetOutput());

//FILTER 9 - MINIMA IMPOSITION IMAGE FILTER
/*typedef itk::MinimaImpositionImageFilter<InputImageType, InputImageType> MinimaImpositionFilterType;
MinimaImpositionFilterType::Pointer impositionFilter = MinimaImpositionFilterType::New();
impositionFilter->SetInput(maggradfilter->GetOutput());
impositionFilter->SetMarkerImage(watershed->GetOutput());
impositionFilter->SetFullyConnected(true);*/

//WRITE DICOM IMAGE

/*   
   WriterType::Pointer writer = WriterType::New();
   writer->SetInput( MorphologicalClosingFilter->GetOutput() );
   writer->SetFileName( "OutputImages/OutputImage.dcm" );   
   
   writer->SetImageIO( gdcmImageIO );

  try
    {
    writer->Update();
    }
  catch( itk::ExceptionObject & error )
    {
    std::cerr << "Error: " << error << std::endl;
    //return EXIT_FAILURE;
    }*/

QuickView viewer;
  viewer.AddImage(itkreader->GetOutput());
viewer.AddImage(maggradfilter->GetOutput());
viewer.AddImage(erodeFilter->GetOutput());
viewer.AddImage(reconstructionDilation->GetOutput());
  //viewer.AddImage(OpenReconstruct->GetOutput());
viewer.AddImage(dilateFilter->GetOutput());
//viewer.AddImage(CloseReconstruct->GetOutput());
viewer.AddImage(reconstructionErosion->GetOutput());
//viewer.AddImage(invertFilterCR->GetOutput());
//viewer.AddImage(RegionalMaxima->GetOutput());
viewer.AddImage(MorphologicalClosingFilter->GetOutput());
viewer.AddImage(erodeFilter2->GetOutput());
viewer.AddImage(thresholdFilter->GetOutput());
viewer.AddImage(distanceMap->GetOutput());
viewer.AddImage(watershed->GetOutput());
//viewer.AddImage(bgm);

  viewer.Visualize();

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
