/*=========================================================================

=========================================================================*/
// Create a grid

#include "GridActors.h"
#include "vtkFloatArray.h"

GridlineActors::GridlineActors()
{
	num_horizontal_lines = 0;
	num_vertical_lines = 0;
}
GridlineActors::~GridlineActors()
{
	//nada
}
void GridlineActors::createGrid(double bounds[])
{
	int gridspacing = 10; //make adjustable

	//if (PROJECTION)//VTKImageGridSource or Unstructured grid
	{
		std::cout << "Projectionmode confirmed" << std::endl;
		////check if this code needed
		//std::cout << "Xinitial: " << bounds[0] << std::endl;
		//std::cout << "Xfinal: " << bounds[1] << std::endl;
		//std::cout << "Yinitial: " << bounds[2] << std::endl;
		//std::cout << "Yfinal: " << bounds[3] << std::endl;
		//std::cout << "Zinitial: " << bounds[4] << std::endl;
		//std::cout << "Zfinal: " << bounds[5] << std::endl;
		// 2D image so limit z range to 1 slice
		bounds[5] = bounds[4];

		//hack since SetDataExtent requires int array but we have double array -_-
		int int_bounds[6];
		for (int k = 0; k < 6; k++)
			int_bounds[k] = bounds[k]/gridspacing; //make adjustable

		//////////////////////////////////////////////////////////
		// could not control line width of the vtkImageGridSource
		// Image of a grid
		//vtkImageGridSource *Gridsource = vtkImageGridSource::New();
		//Gridsource->SetDataExtent(int_bounds);
		//Gridsource->SetDataSpacing(gridspacing,gridspacing,0); //make adjustable
		////Gridsource->SetDataOrigin(bounds[0],bounds[2],bounds[4]);
		//Gridsource->SetDataScalarTypeToUnsignedChar();
		//Gridsource->SetLineValue(255);
		//Gridsource->SetFillValue(NULL);

		//vtkLookupTable *table = vtkLookupTable::New();
		//table->SetRampToLinear();
		//table->SetRange(0.0, 255.0);
		//table->SetValueRange(1.0, 1.0);
		//table->SetSaturationRange(0.0, 0.0);
		//table->SetAlphaRange(0.0, 1.0);
		//table->Build();

		//vtkImageMapToColors *colors = vtkImageMapToColors::New();
		//colors->SetInputConnection(Gridsource->GetOutputPort());
		//colors->SetLookupTable(table);
		//colors->PassAlphaToOutputOn();
		//colors->SetOutputFormatToLuminanceAlpha();

		//vtkSmartPointer<vtkImageCast> castFilter = vtkSmartPointer<vtkImageCast>::New();
		//castFilter->SetInputConnection(colors->GetOutputPort());
		//castFilter->SetOutputScalarTypeToUnsignedChar();
		//castFilter->Update();

		// Create an actor
		//vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
		////actor->SetOpacity(0.9);
		//actor->SetInput(castFilter->GetOutput());
		//Renderer->AddActor(actor);

		//Manually create lines
		/// horizontal lines
		num_horizontal_lines = (bounds[3] - bounds[2]) / gridspacing + 1;
		GridlineActorVectorHorizontal = new vtkSmartPointer<vtkActor>[num_horizontal_lines];
		
		unsigned int horizontal_line_index = 0;
		for (double increment = bounds[2]; increment < bounds[3]; increment+=gridspacing, horizontal_line_index++)
		{	
			// Create two points, P0 and P1
			double p0[3] = {bounds[0], increment, 0.0};
			double p1[3] = {bounds[1], increment, 0.0};

			vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
			lineSource->SetPoint1(p0);
			lineSource->SetPoint2(p1);
			lineSource->Update();

			// Visualize
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputConnection(lineSource->GetOutputPort());
			//Create GridlineActor
			vtkSmartPointer<vtkActor> GridlineActor = vtkSmartPointer<vtkActor>::New();
			//Store it to the GridlineActorVector
			GridlineActorVectorHorizontal[horizontal_line_index] = GridlineActor;
			
			//Add the GridlineActor to the renderer
			GridlineActor->SetMapper(mapper);
			//GridlineActor->GetProperty()->SetLineWidth(4);
		}

		/// vertical lines
		num_vertical_lines = (bounds[1] - bounds[0]) / gridspacing + 1;
		GridlineActorVectorVertical = new vtkSmartPointer<vtkActor>[num_vertical_lines];

		unsigned int vertical_line_index = 0;
		for (double increment = bounds[0]; increment < bounds[1]; increment+=gridspacing, vertical_line_index++)
		{	
			// Create two points, P0 and P1
			double p0[3] = {increment, bounds[2], 0.0};
			double p1[3] = {increment, bounds[3], 0.0};

			vtkSmartPointer<vtkLineSource> lineSource = vtkSmartPointer<vtkLineSource>::New();
			lineSource->SetPoint1(p0);
			lineSource->SetPoint2(p1);
			lineSource->Update();

			// Visualize
			vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
			mapper->SetInputConnection(lineSource->GetOutputPort());
			vtkSmartPointer<vtkActor> GridlineActor = vtkSmartPointer<vtkActor>::New();
			GridlineActorVectorVertical[vertical_line_index] = GridlineActor;
			GridlineActor->SetMapper(mapper);
			//GridlineActor->GetProperty()->SetLineWidth(4);
		}
	}
}
vtkSmartPointer<vtkActor> GridlineActors::GetHorizontalGridlines(int i)
{
	if (i == -1)
	{
		//std::cout << "No gridlines specified."
		return NULL;
	}
	return this->GridlineActorVectorHorizontal[i];
}
vtkSmartPointer<vtkActor> GridlineActors::GetVerticalGridlines(int i)
{
	if (i == -1)
	{
		//std::cout << "No gridlines specified."
		return NULL;
	}
	return this->GridlineActorVectorVertical[i];
}
int GridlineActors::NumberOfLines()
{
	return (num_horizontal_lines + num_vertical_lines);
}
int GridlineActors::NumberOfHorizontalLines()
{
	return num_horizontal_lines;
}
int GridlineActors::NumberOfVerticalLines()
{
	return num_vertical_lines;
}