#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QMainWindow>

#include "itkImage.h"
#include <iostream>
#include <string>
#include "itkSize.h"
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include <itkCastImageFilter.h>

//Reading DICOM using ITK
#include "itkRescaleIntensityImageFilter.h"
#include "itkGDCMImageIO.h"

//Morphological Operations
#include "itkBinaryBallStructuringElement.h"
#include "itkGrayscaleErodeImageFilter.h"
#include <itkSignedMaurerDistanceMapImageFilter.h>
#include <itkBinaryThresholdImageFilter.h>
#include <itkReconstructionByDilationImageFilter.h>
#include <itkReconstructionByErosionImageFilter.h>
#include <itkGrayscaleMorphologicalClosingImageFilter.h>
#include <itkMorphologicalWatershedImageFilter.h>
#include "itkConnectedComponentImageFilter.h"
#include <itkGrayscaleDilateImageFilter.h>

#include "itkLabelToRGBImageFilter.h"
#include "itkScalarToRGBColormapImageFilter.h"
#include "itkFlipImageFilter.h"
#include "itkScalarToRGBPixelFunctor.h"
#include "itkUnaryFunctorImageFilter.h"

#include "itkGradientMagnitudeRecursiveGaussianImageFilter.h"
#include "itkRegionOfInterestImageFilter.h"
#include <itkExtractImageFilter.h>

#include <itkResampleImageFilter.h>
#include "itkTranslationTransform.h"
/*
#include <itkLabelObject.h>
#include <itkLabelMap.h>
#include <itkLabelImageToLabelMapFilter.h>
#include <itkLabelMapOverlayImageFilter.h>*/

//vtk headers
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <itkImageToVTKImageFilter.h>
 
#include "vtkVersion.h"
//#include "vtkImageViewer.h"
#include "vtkImageActor.h"
#include "vtkInteractorStyleImage.h"
#include <vtkPointData.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkCornerAnnotation.h>
#include <vtkTextProperty.h>
#include <vtkImageData.h>
//#include <vtkCell.h>
#include <vtkImageChangeInformation.h>
#include <vtkPNGReader.h>
#include <vtkViewport.h>
#include<vtkImageMapper.h>
#include <vtkCoordinate.h>
#include<vtkRendererCollection.h>
#include <vtkPointPicker.h>

#include <vtkPNGReader.h>
#include <vtkImageBlend.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageLuminance.h>

#include <vtkImageStencilData.h>
#include <vtkImageToImageStencil.h>
#include <vtkImageStencil.h>
#include <vtkImageData.h>

#include <math.h>

#include <vtkTransform.h>
#include <vtkMatrix4x4.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkCamera.h>

namespace Ui {
class FileDialog;
}


class FileDialog : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FileDialog(QWidget *parent = 0);
    ~FileDialog();
    
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_3_clicked();

	void convertCoordinates(double oldX,double oldY,double oldZ);
void openFile();
void interact();
void segmentation();

//    void SetRenderer(vtkSmartPointer<vtkRenderer> ren);

  //  void SetImageActor(vtkSmartPointer<vtkImageActor> im);


private:
    	Ui::FileDialog *ui;
	const char* p;
    	QString s1, s2;
    	double threshold;
    	float setLevel;
	double width, height, lowerLeftX, lowerLeftY, upperRightX, upperRightY, lowerLeftXold, lowerLeftYold, upperRightXold, upperRightYold,wx, wy, URtempX, URtempY, LLtempX, LLtempY, upperRightZold, lowerLeftZold;
	double seedX, seedY, seed2, seed3, seed4, seed5, seed6, seed7, seed8, seed9, newX, newY, winszX, winszY, sizeitkimage0, sizeitkimage1, seedXnew, seedYnew, seedXnew2, seedYnew2;
	double actorXmin, actorXmax, actorYmin, actorYmax, actorZmin, actorZmax;
	int bounds[6];
	double position[3];
	double matrix[16];

// Type of the image to be used as input to the module.

        typedef signed short InputPixelType;

    	typedef itk::RGBPixel<unsigned char> OutputPixelType;
        
    	typedef itk::Image< InputPixelType, 2 > InputImageType;

    	typedef itk::Image< OutputPixelType, 2 > OutputImageType;

//Reader and Writer
    	typedef itk::ImageFileReader<InputImageType> ReaderType;
	ReaderType::Pointer itkreader;
    	typedef itk::ImageFileWriter< InputImageType > WriterType;

//GDCMIO Type
	typedef itk::GDCMImageIO ImageIOType;
	ImageIOType::Pointer gdcmImageIO;

//png writer type
	typedef unsigned char WriterPixelType1;
	typedef itk::Image<WriterPixelType1,2> WriterTypeShr;
	typedef itk::ImageFileWriter<WriterTypeShr> pngWriterType1;

//Filter types
	typedef itk::RescaleIntensityImageFilter<InputImageType, WriterTypeShr> RescaleFilterType; //Rescale Intensity Filter
	typedef itk::FlipImageFilter<InputImageType> FlipImageFilterType; //Flip the image filter
	FlipImageFilterType::Pointer flipfilter;
	typedef itk::ImageToVTKImageFilter<InputImageType>  ConnectorType; //ITK to VTK converter filter 
//	typedef itk::RegionOfInterestImageFilter< InputImageType,InputImageType >ROIFilterType; //ROI Filter
typedef itk::ExtractImageFilter< InputImageType,InputImageType >ROIFilterType; //ROI Filter
	typedef itk::ResampleImageFilter<InputImageType, InputImageType> ResampleFilterType; //Resample Image filter

   	typedef itk::CastImageFilter< InputImageType, WriterTypeShr> CastFilterType;

};

#endif // FILEDIALOG_H
