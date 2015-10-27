#include "../include/camera.h"

typedef glm::dmat3x4 Mat3x4;
typedef glm::dmat3x3 Mat3;
typedef glm::dvec3   Vec3;
typedef boost::array<double, 12> Raw3x4;
typedef boost::array<double, 16> Raw4x4;

Mat3x4 raw2mat(const Raw3x4 &raw)
{
	Mat3x4 mat;
	mat[0][0] = raw[0];
	mat[0][1] = raw[1];
	mat[0][2] = raw[2];
	mat[0][3] = raw[3];

	mat[1][0] = raw[4];
	mat[1][1] = raw[5];
	mat[1][2] = raw[6];
	mat[1][3] = raw[7];

	mat[2][0] = raw[8];
	mat[2][1] = raw[9];
	mat[2][2] = raw[10];
	mat[2][3] = raw[11];	
	return mat;
}


Mat3x4 raw2mat_transpose(const Raw3x4 &raw)
{
	Mat3x4 mat;
	mat[0][0] = raw[0];
	mat[1][0] = raw[1];
	mat[2][0] = raw[2];

	mat[0][1] = raw[3];
	mat[1][1] = raw[4];
	mat[2][1] = raw[5];

	mat[0][2] = raw[6];
	mat[1][2] = raw[7];
	mat[2][2] = raw[8];

	mat[0][3] = raw[9];
	mat[1][3] = raw[10];
	mat[2][3] = raw[11];	
	return mat;
}

void decompose(const Mat3x4& pmat, Mat3& param, Mat3& R, Vec3& t)
{
	Vec3 row1, row2, row3, column4;
	row1    = Vec3(pmat[0][0], pmat[0][1], pmat[0][2]);
	row2    = Vec3(pmat[1][0], pmat[1][1], pmat[1][2]);
	row3    = Vec3(pmat[2][0], pmat[2][1], pmat[2][2]);
	column4 = Vec3(pmat[0][3], pmat[1][3], pmat[2][3]);


	double gamma = glm::length(row3);

	row1 = row1/gamma;
	row2 = row2/gamma;
	row3 = row3/gamma;
	column4 = column4/gamma;

	Vec3 q13 = glm::cross(row1,row3);
	Vec3 q23 = glm::cross(row2,row3);
	double fu = glm::length(q13);
	double fv = glm::length(q23);
	double du = glm::dot(row1,row3);
	double dv = glm::dot(row2,row3);

	Vec3 Rrow1 = glm::cross(row3,q13) / fu ;
	Vec3 Rrow2 = glm::cross(row3,q23) / fv ;
		
	R = Mat3(Rrow1[0],Rrow1[1],Rrow1[2],
			 Rrow2[0],Rrow2[1],Rrow2[2],
			 row3[0] ,row3[1] ,row3[2]);
	
	param = Mat3(fu , 0.0 , du,
				 0.0, fv  , dv,
				 0.0, 0.0 , 1.0);

	
	Vec3 Rt = Vec3((column4[2]*du -column4[0])/fu, 
				   (column4[2]*dv -column4[1])/fv , 
				   -column4[2]);
	t = glm::transpose(R) * Rt;
}

Camera::Camera(const Mat3x4 &pmat)
{
	decompose(pmat, mK, mRw2c, mPos);
}

Camera::Camera(const Raw3x4 &pmat, bool transpose_cam)
{
	if (transpose_cam) {
		decompose(raw2mat_transpose(pmat), mK, mRw2c, mPos);
	} else {
		decompose(raw2mat(pmat), mK, mRw2c, mPos);
	}
}


Raw4x4 Camera::projection_matrix(const unsigned int width,
							const unsigned int height, 
							const double       znear, 
							const double       zfar) const 
{
	


Raw4x4 proj = {{ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }};
/*//up yvector
	proj[0*4+0] = (2.0 *  mK[0][0] / double(width));
	proj[1*4+0] = (-2.0 *  mK[0][1] / double(width));
	proj[2*4+0] = (((double(width) - 2.0 * mK[0][2] )/ double(width)) );
	proj[3*4+0] = 0.0;

	proj[0*4+1] = 0.0;
	proj[1*4+1] = (-2.0 *  mK[1][1] / double(height));
	proj[2*4+1] = (((double(height) - 2.0 * mK[1][2] )/ double(height)) );
	proj[3*4+1] = 0.0;

	proj[0*4+2] = 0.0;
	proj[1*4+2] = 0.0;
	proj[2*4+2] = (-zfar - znear)/(zfar - znear);
	proj[3*4+2] = -2.0 * zfar * znear / (zfar - znear);

	proj[0*4+3] = 0.0;
	proj[1*4+3] = 0.0;
	proj[2*4+3] = -1.0;
	proj[3*4+3] = 0.0;
	return proj;
}

*/
 //down y vector
	proj[0*4+0] = (2.0 *  mK[0][0] / double(width));
	proj[1*4+0] = (-2.0 *  mK[0][1] / double(width));
	proj[2*4+0] = (((double(width) - 2.0 * mK[0][2] )/ double(width)) );
	proj[3*4+0] = 0.0;

	proj[0*4+1] = 0.0;
	proj[1*4+1] = (2.0 *  mK[1][1] / double(height));
	proj[2*4+1] = (((-double(height) + 2.0 * mK[1][2] )/ double(height)) );
	proj[3*4+1] = 0.0;

	proj[0*4+2] = 0.0;
	proj[1*4+2] = 0.0;
	proj[2*4+2] = (-zfar - znear)/(zfar - znear);
	proj[3*4+2] = -2.0 * zfar * znear / (zfar - znear);

	proj[0*4+3] = 0.0;
	proj[1*4+3] = 0.0;
	proj[2*4+3] = -1.0;
	proj[3*4+3] = 0.0;
	return proj;
}







/* Benjamin
Camera::Raw4x4 Camera::projection_matrix(const unsigned int width, 
							const unsigned int height, 
							const double       znear, 
							const double       zfar) const 
{
	Raw4x4 proj = {{ 0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0 }};

	proj[0*4+0] = (2.0 *  mK[0][0] / double(width));
	proj[1*4+0] = (2.0 *  mK[0][1] / double(width));
	proj[2*4+0] = (((2.0 * mK[0][2] + 1.0)/ double(width))  - 1.0);
	proj[3*4+0] = 0.0;

	proj[0*4+1] = 0.0;
	proj[1*4+1] = (2.0 *  mK[1][1] / double(height));
	proj[2*4+1] = (((2.0 * mK[1][2] + 1.0)/ double(height)) - 1.0);
	proj[3*4+1] = 0.0;

	proj[0*4+2] = 0.0;
	proj[1*4+2] = 0.0;
	proj[2*4+2] = (zfar + znear)/(zfar - znear);
	proj[3*4+2] = -2.0 * zfar * znear / (zfar - znear);

	proj[0*4+3] = 0.0;
	proj[1*4+3] = 0.0;
	proj[2*4+3] = 1.0;
	proj[3*4+3] = 0.0;
	return proj;
}
*/











Raw4x4 Camera::model_matrix() const
{
	Raw4x4 model;
	Vec3 Rt = mRw2c * ( mPos * -1.0);
	
	for (int i=0;i<3;++i)
	{
		for (int j=0;j<3;++j)
		{
			model[i*4+j] = mRw2c[j][i];
		}
		model[3*4+i] = Rt[i];
	}
	model[0*4+3] = 0;
	model[1*4+3] = 0;
	model[2*4+3] = 0;
	model[3*4+3] = 1;
	return model;
}

