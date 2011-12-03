
#ifndef VOLUMEOFINTEREST_H_
#define VOLUMEOFINTEREST_H_

#include <stdio.h>
#include <string>

#include "vtkActor.h"
#include "vtkSmartPointer.h"

#include "vtkCellArray.h"
#include "vtkCellLocator.h"

#include "itkImageFileReader.h"
#include "itkImageToVTKImageFilter.h"
#include "vtkContourFilter.h"

#include "vtkVolume.h"
#include "vtkVolumeProperty.h"

#include "vtkPoints.h"
#include "vtkLinearExtrusionFilter.h"

#include "vtkPolygon.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"

//FTK Includes
#include "CellTrace.h"
#include "CellTraceModel.h"

typedef itk::Image< unsigned char, 3 >   ImageType;

typedef itk::ImageFileReader< ImageType >    ReaderType;
typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;

class VolumeOfInterest
{
public:
	VolumeOfInterest();
	int AddVOIPoint(double* newPT);
	bool ExtrudeVOI();
	vtkSmartPointer<vtkActor> GetActor();
	void CalculateCellDistanceToVOI(CellTraceModel *CellModel);
	void ReadBinaryVOI(std::string filename);
private:
	std::vector<double*> ROIPoints;
	vtkSmartPointer<vtkPolyData> VOIPolyData;
};
#endif