/*=========================================================================
Copyright 2009 Rensselaer Polytechnic Institute
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
=========================================================================*/
#ifndef __ftkPreprocess2_h
#define __ftkPreprocess2_h


#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

// NOTE THAT THIS CLASS IS WRITTEN FOR 8-BIT IMAGE PROCESSING

#include <vector>
#include <iostream>
#include <limits>

//ITK Preprocessing includes
#include "itkImage.h"
#include "itkRGBPixel.h"
#include "itkExtractImageFilter.h"
#include "itkNeighborhoodIterator.h"
#include "itkRGBToLuminanceImageFilter.h"
#include "itkRescaleIntensityImageFilter.h"
#include "itkInvertIntensityImageFilter.h"
#include "itkOtsuMultipleThresholdsCalculator.h"
#include "itkScalarImageToHistogramGenerator.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkNumericTraits.h"
#include "itkMedianImageFilter.h"
#include "itkLaplacianRecursiveGaussianImageFilter.h"
#include "itkCurvatureAnisotropicDiffusionImageFilter.h"
#include "itkGradientAnisotropicDiffusionImageFilter.h"
#include "itkVotingBinaryIterativeHoleFillingImageFilter.h"
#include "itkDanielssonDistanceMapImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkBinaryMedianImageFilter.h"
#include "itkGradientVectorFlowImageFilter.h"
#include "itkBinaryThinningImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkGrayscaleMorphologicalOpeningImageFilter.h"
#include "itkGrayscaleMorphologicalClosingImageFilter.h"

#include "GraphCuts/itkMinErrorThresholdImageFilter.h"
#include "GraphCuts/new_graph.h"

namespace ftk
{

class Preprocess
{
public:
	typedef unsigned char PixelType;
	typedef itk::RGBPixel< PixelType > RGBPixelType;
	typedef itk::Image< PixelType, 2 > ImageType2D;
	typedef itk::Image< PixelType, 3 > ImageType3D;
	typedef itk::Image< RGBPixelType, 3> RGBImageType3D;
	typedef itk::Image< float, 2 > FloatImageType2D;
	typedef itk::Image< float, 3 > FloatImageType3D;

	Preprocess(ImageType3D::Pointer img);
	Preprocess(RGBImageType3D::Pointer img);
	Preprocess(RGBImageType3D::Pointer img, char color);

	ImageType3D::Pointer GetImage(){ return myImg; };

	void RunFilter(std::string name, std::vector<double> params);

	void RescaleIntensities(int min = 0, int max = 255);
	void LaplacianOfGaussian(int sigma=5, int min=0);
	void InvertIntensity(void);
	void CurvatureAnisotropicDiffusion( double timestep=0.125, double conductance=1.0, int iterations=1 );
	void GradientAnisotropicDiffusion( double timestep=0.125, double conductance=1.0, int iterations=1 );
	void MedianFilter( int radiusX=3, int radiusY=3, int radiusZ=0);
	void OpeningFilter( int radius=3 );
	void ClosingFilter( int radius=3 );
	void ManualThreshold( PixelType threshold=128, bool binary=false );
	void RemoveConnectedComponents(int minObjSize);

	//Methods specific to Binary Images:
	void GraphCutBinarize(bool shiftDown=false);
	void MinErrorThresholding(float *alpha_B, float *alpha_A, float *P_I, bool overwrite=true);
	void OtsuBinarize(int num_thresholds=2, int num_in_foreground=1, bool fgrnd_dark=false); //To create the binary
	void VotingHoleFilling(int radiusX=5, int radiusY=5, int radiusZ=0, int iterations=100); 
	void MedianHoleFilling(int radiusX=9, int radiusY=9, int radiusZ=0);
	void DanielssonDistanceMap();
	void GradientVectorFlow();
	void BinaryThinning();

	
protected:
	ImageType3D::Pointer myImg;

	ImageType2D::Pointer ExtractSlice(ImageType3D::Pointer img, int slice);
	ImageType3D::Pointer SliceTo3D(ImageType2D::Pointer img);
	ImageType3D::Pointer RescaleFloatToImageType(FloatImageType3D::Pointer img);
	ImageType2D::Pointer RescaleFloatToImageType(FloatImageType2D::Pointer img);
	ImageType3D::Pointer RescaleFloatToImageType(FloatImageType3D::Pointer img, int inMin);
	ImageType2D::Pointer RescaleFloatToImageType(FloatImageType2D::Pointer img, int inMin);
	double ComputePoissonProb(double intensity, double alpha);

};

} // end namespace ftk

#endif
