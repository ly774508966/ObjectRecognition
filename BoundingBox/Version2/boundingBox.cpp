#include "boundingBox.h"

BoundingBox::BoundingBox():cloud(new PointCloudT() ){}
void BoundingBox::calculate_OBB()
{
	feature_extractor.getOBB (min_point_OBB, max_point_OBB, position_OBB, rotational_matrix_OBB);
}	

void BoundingBox::calculate_AABB()
{
	feature_extractor.getAABB (min_point_AABB, max_point_AABB);
}

void BoundingBox::initializeExtractor()
{
	feature_extractor.setInputCloud (cloud);
  	feature_extractor.compute ();
  	feature_extractor.getMomentOfInertia (moment_of_inertia);
  	feature_extractor.getEccentricity (eccentricity);
  	feature_extractor.getEigenValues (major_value, middle_value, minor_value);
	feature_extractor.getEigenVectors (major_vector, middle_vector, minor_vector);
	feature_extractor.getMassCenter (mass_center);
}

void BoundingBox::show_OBB(pcl::visualization::PCLVisualizer * viewer)
{
	Eigen::Vector3f position (position_OBB.x, position_OBB.y, position_OBB.z);
  Eigen::Quaternionf quat (rotational_matrix_OBB);
  viewer->addCube (position, quat, max_point_OBB.x - min_point_OBB.x, max_point_OBB.y - min_point_OBB.y, max_point_OBB.z - min_point_OBB.z, "OBB");

  pcl::PointXYZ center (mass_center (0), mass_center (1), mass_center (2));
  pcl::PointXYZ x_axis (major_vector (0) + mass_center (0), major_vector (1) + mass_center (1), major_vector (2) + mass_center (2));
  pcl::PointXYZ y_axis (middle_vector (0) + mass_center (0), middle_vector (1) + mass_center (1), middle_vector (2) + mass_center (2));
  pcl::PointXYZ z_axis (minor_vector (0) + mass_center (0), minor_vector (1) + mass_center (1), minor_vector (2) + mass_center (2));
  viewer->addLine (center, x_axis, 1.0f, 0.0f, 0.0f, "major eigen vector");
  viewer->addLine (center, y_axis, 0.0f, 1.0f, 0.0f, "middle eigen vector");
  viewer->addLine (center, z_axis, 0.0f, 0.0f, 1.0f, "minor eigen vector");
}

std::vector<float> BoundingBox::GetDimensions(){
	std::vector <float> Dimensions;
	Dimensions.push_back(static_cast<float>(max_point_OBB.x - min_point_OBB.x));
	Dimensions.push_back(static_cast<float>(max_point_OBB.y - min_point_OBB.y));
	Dimensions.push_back(static_cast<float>(max_point_OBB.z - min_point_OBB.z));
	return Dimensions;
}

void BoundingBox::show_AABB(pcl::visualization::PCLVisualizer * viewer)
{viewer->addCube (min_point_AABB.x, max_point_AABB.x, min_point_AABB.y, max_point_AABB.y, min_point_AABB.z, max_point_AABB.z, 1.0, 1.0, 0.0, "AABB");}
