# MARKER BASED WATERSHED SEGMENTATION
# Version - Final-Version-MBS2
# Date of final commit - 26.9.2016
# Author - Shreeya Sengupta
# Branch on Github - VTK-DICOM/Master

The segmentation pipeline is executing without any error. The pipeline has the following features - 
1. Interactive image viewer - the user can interact with the image and select a Region Of Interest (ROI) and a seed point. 
2. ROI - the algorithm performs segmentation on the ROI chosen by the user. 
3. Seed Point - a seed point is chosen by the user which is then used to create a marker image. 
4. Marker based watershed segmentation algorithm - this algorithm segments the image.

Some terms - 
Marker Image - Marker image marks the minima in the input image and gives them a label. 
Connected Component - Connected component in a 2D image are clusters of pixels with the same value.

Test Cases - 
1. ROI Extraction - Is the ROI being extracted correctly? 
2. Choosing the Seed Point - 
  A. Is the seed point being picked up? 
  B. Is the picked point correct, that is, is the event location returned by the program correct? 
3. Generation Of Marker Image - 
  A. Is the marker image being created correctly? 
  B. Is the location of the seed point correct in the marker image? 
  C. Is it possible to create an inner rectangle by choosing the seed point as the centre of the rectangle? 
4. Segmentation - 
  A. When the entire image is the ROI? 
  B. When a specific area is the ROI?
  C. Test across different images? 
5. Writing the results - 
  A. Write in the DICOM image format. 
  B. Write in the png image format.

Filters Used in the pipeline - 
  1. Segmentation Function - Gradiant of the input image - itk::GradientMagnitudeRecursiveGaussianImageFilter 
  2. ROI Extraction - itk::RegionOfInterestImageFilter 
  3. Rendering and interaction - different VTK filters such as vtkImageActor, vtkRenderWindow, vtkImageViewer2
  4. Marker image creation - itk::Image and itk::ImageFileWriter 
  5. Segmentation - itk::MorphologicalWatershedFromMarkersImageFilter
