#ifndef FILEDIALOG_H
#define FILEDIALOG_H

#include <QMainWindow>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
//#include <itkCastImageFilter.h>

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

private:
    	Ui::FileDialog *ui;
    	QString s1, s2;
    	double threshold;
    	float setLevel;

// Type of the image to be used as input to the module.

        typedef unsigned int InputPixelType;

    	typedef itk::RGBPixel<unsigned char> OutputPixelType;
        
    	typedef itk::Image< InputPixelType, 2 > InputImageType;

    	typedef itk::Image< OutputPixelType, 2 > OutputImageType;

//Reader and Writer
    //typedef itk::ImageFileReader<InputImageType> ReaderType;
    typedef itk::ImageFileWriter< InputImageType > WriterType;
    //typedef itk::CastImageFilter< InitialImageType, InputImageType> CastFilterType;
};

#endif // FILEDIALOG_H
