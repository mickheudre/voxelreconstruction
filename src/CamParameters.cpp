/** @file CameraParameters.h
    
 * Camera parameters with intrinsic/extrinsic parameter * definitions,
 * 
 */
#include <stdlib.h>

#ifndef NO_GUI // CM
#ifndef APPLE
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
//#include "GLUtils.h"
#endif // NO_GUI // CM
#include "CamParameters.h"

//#include <mvl/PMatrixDec.h>
//#include <vnl/algo/vnl_matrix_inverse.h>
//#include <vnl/vnl_transpose.h>
//#include <mvl/HomgPoint3D.h>

#ifdef TRACE_CAMPARAM
#include "trace.h"
#endif

// void CamParameters::update()
// {
//   m_proj=m_k*m_m;
//   Matrix3 tmp=m_proj.extract<3,3>();
//   tmp.getInverse(&m_backProj);
//   PMatrix pmat(vnl_matrix_fixed<Real,3,4>(m_proj.data()));
//   vnl_vector_fixed<Real,3> center=pmat.get_focal_point().get_double3();
//   m_center[0]=center[0]; m_center[1]=center[1]; m_center[2]=center[2];
//   addNoise();
// }

void CamParameters::update()
{
  m_proj=m_k*m_m;
  Matrix3 tmp=m_proj.extract<3,3>();
  tmp.getInverse(&m_backProj);
  Matrix3 m_m_inv=m_m.extract<3,3>();
  m_m_inv.inverse();
  m_center=-(m_m_inv*m_m.column(3));
  addNoise();
}

void CamParameters::addNoise() {
  // add noise on center coordinates: DOES NOT WORK AT ALL
  /*m_m[0][3]*=(1+(((2.*(Real)rand()/(Real)RAND_MAX)-1)/50.));
  m_m[1][3]*=(1+(((2.*(Real)rand()/(Real)RAND_MAX)-1)/50.));
  m_m[2][3]*=(1+(((2.*(Real)rand()/(Real)RAND_MAX)-1)/50.));
  
  m_proj=m_k*m_m;
  PMatrix PM(vnl_matrix_fixed<Real,3,4>(m_proj.data()));
  vnl_vector_fixed<Real,3> res=PM.get_focal_point().get_double3();
  m_center[0]=res[0]; m_center[1]=res[1]; m_center[2]=res[2];*/
}

#ifndef NO_GUI // CM
/// set from opengl matrices, need image width and height
void CamParameters::glRead()
{
  /*  Real proj[4][4];
  Real w2 = (Real)m_w / 2.0f;
  Real h2 = (Real)m_h / 2.0f;

  glGetDoublev(GL_PROJECTION_MATRIX, (Real*)proj);

  m_k[0][0]=w2*proj[0][0];
  m_k[0][1]=0.f;
  m_k[0][2]=w2*(proj[2][0]-1.0f);

  m_k[1][0]=0.f;
  // WARNING: there really should be a minus on both lines because of
  // y-vector inversion; works without for OPENGL case
  // NOTE: corrected 2003/09/22
  //m_k[1][1]= h2*proj[1][1];
  //m_k[1][2]= h2*(proj[2][1]-1.0f);
  m_k[1][1]= - h2*proj[1][1];
  m_k[1][2]= - h2*(proj[2][1]+1.0f);

  m_k[2][0]=0.f;
  m_k[2][1]=0.f;
  m_k[2][2]=-1.;

  assert(proj[2][3]==-1);

  glGetDoublev(GL_MODELVIEW_MATRIX, (Real*)proj);

  //for (int i=0; i<3; ++i)
  //  for (int j=0; j<4; ++jls)
  //    m_m[i][j]=proj[j][i];
  
  for (int i=0; i<3; ++i) {
    m_m[i][0]=proj[0][i];
    m_m[i][1]=-proj[1][i];
    m_m[i][2]=-proj[2][i];
  }
  m_m[0][3]=-proj[3][0]; m_m[1][3]=proj[3][1]; m_m[2][3]=proj[3][2];

  assert(proj[0][3]==0.); assert(proj[1][3]==0.); assert(proj[2][3]==0.);
  assert(proj[3][3]==1.);
  // TRACELN("m_k" << m_k);
  // TRACELN("m_m" << m_m);
    
  update();*/

  Real proj[4][4];
  Real w2 = (Real)m_w / 2.0f;
  Real h2 = (Real)m_h / 2.0f;

  glGetDoublev(GL_PROJECTION_MATRIX, (Real*)proj);

  Matrix3 k;
  k[0][0]=w2*proj[0][0];
  k[0][1]=0.f;
  k[0][2]=w2*(proj[2][0]-1.0f);

  k[1][0]=0.f;
  k[1][1]= - h2*proj[1][1];
  k[1][2]= - h2*(proj[2][1]+1.0f);

  k[2][0]=0.f;
  k[2][1]=0.f;
  k[2][2]=-1.;

  assert(proj[2][3]==-1);

  glGetDoublev(GL_MODELVIEW_MATRIX, (Real*)proj);

  Transform3D m;
  for (int i=0; i<3; ++i)
    for (int j=0; j<4; ++j)
      m[i][j]=proj[j][i];

  assert(proj[0][3]==0.); assert(proj[1][3]==0.); assert(proj[2][3]==0.);
  assert(proj[3][3]==1.);

  m_k=k; m_m=m;
  update();
}

#endif // NO_GUI // CM

#ifndef NO_GUI // CM
/// set opengl matrices, need image width and height
void CamParameters::glWrite(Real near, Real far) const
{
  Real invw2 = 2.0f / (Real)m_w;
  Real invh2 = 2.0f / (Real)m_h;
  Real invfmn = 1.0f / (far - near);

  // WARNING: this is the correct formula, but to be consistent with
  // mistake above in glRead, make minus on y axis dissappear for
  // consistency -> otherwise epipolar geometry is y-inverted if redrawn
  // NOTE: corrected 2003/09/22
// Commented by VAgia:
/* Real proj[4][4]={{m_k[0][0]*invw2, 0.f, 0.f, 0.f},
		   {0.f, -m_k[1][1]*invh2, 0.f, 0.f},
	{m_k[0][2]*invw2-1.0f, 1.-m_k[1][2]*invh2, -(far+near)*invfmn , 1.0f},
		   {0.f, 0.f, 2.0f*far*near*invfmn, 0.f}};
*/
//Uncommented by VAgia:
/*Real proj[4][4]={m_k[0][0]*invw2, 0.f, 0.f, 0.f,
	  0.f, m_k[1][1]*invh2, 0.f, 0.f,
	  m_k[0][2]*invw2-1.0f, m_k[1][2]*invh2-1.0f, (far+near)*invfmn , 1.0f,
	  0.f, 0.f, -2.0f*far*near*invfmn, 0.f};
*/
// We Invert the y axis to be consistent with computer vision convention and we need to change for the z axis
Real proj[4][4]={ m_k[0][0]*invw2, 0.f, 0.f, 0.f,
	  0.f, -m_k[1][1]*invh2, 0.f, 0.f,
	  m_k[0][2]*invw2-1.0f, -m_k[1][2]*invh2+1.0f, (far+near)*invfmn , 1.0f,
      0.f, 0.f, -2.0f*far*near*invfmn, 0.f };


  glMatrixMode(GL_PROJECTION);
  glLoadMatrixd((Real*)proj);

  proj[0][3]=0.; proj[1][3]=0.; proj[2][3]=0.; proj[3][3]=1.;
  
  for (int i=0; i<4; ++i)
    for (int j=0; j<3; ++j)
      proj[i][j]=m_m[j][i];






  glMatrixMode(GL_MODELVIEW);
  glLoadMatrixd((Real*)proj);
}

void CamParameters::glWriteTextureProjection(Real near, Real far) const
{

  Real invw2 = 1.0f / (Real)m_w;
  Real invh2 = 1.0f / (Real)m_h;
  Real invfmn = 1.0f / (far - near);

  /*Real proj[16]={m_k[0][0]*invw2, 0.f, 0.f, 0.f,
		 0.f, -m_k[1][1]*invh2, 0.f, 0.f,
		 m_k[0][2]*invw2, 1.0-m_k[1][2]*invh2,-(far+near)*invfmn, 1.,
		 0.f, 0.f, 2.0f*far*near*invfmn, 0.f};*/

  Real proj[16]={m_k[0][0]*invw2, 0.f, 0.f, 0.f,
  	  0.f, m_k[1][1]*invh2, 0.f, 0.f,
  	  m_k[0][2]*invw2-1.0f, m_k[1][2]*invh2-1.0f, -(far+near)*invfmn , 1.0f,
  	  0.f, 0.f, 2.0f*far*near*invfmn, 0.f};

  glMatrixMode(GL_TEXTURE);
  glLoadMatrixd(proj);
//const GLdouble vagia = &(const GLdouble)m_m;
//vagia = &m_m;
	//glMultMatrixd(vagia);
//GLMult(m_m);

  //Transform3D current;
  //GLGetModelView(current);
  //current.inverse();
  //GLMult(current);
  
  glMatrixMode(GL_MODELVIEW);
  
}

#endif // NO_GUI // CM

/*
bool CamParameters::setProjection(const Matrix3x4& proj)
{
  m_proj = proj; // CM : corrected to initialise also m_proj

  PMatrixDec PM(vnl_matrix_fixed<Real,3,4>(proj.data()));
  m_k.set(PM.IntrinsicParameters().data_block());
  m_m.set(PM.ExtrinsicParameters().data_block());

  std::cout << "-----\nm_k\n" << m_k;
  std::cout << "m_m\n" << m_m;

  Matrix3 tmp=m_proj.extract<3,3>();
  bool test=tmp.getInverse(&m_backProj);
  vnl_vector_fixed<Real,3> res=PM.get_focal_point().get_double3();
  m_center[0]=res[0]; m_center[1]=res[1]; m_center[2]=res[2];
  addNoise();
  return test;
}*/

#ifndef NAN
#define NAN 0.//((0.)/(0.))
#endif

bool CamParameters::setProjection(const Matrix3x4& proj)
{
  m_proj = proj; // CM : corrected to initialise also m_proj

  // debug
  //PMatrixDec PM(vnl_matrix_fixed<Real,3,4>(proj.data()));
  //m_k.set(PM.IntrinsicParameters().data_block());
  //m_m.set(PM.ExtrinsicParameters().data_block());

  //std::cout << "---------------\nexpect m_k\n" << m_k;
  //std::cout << "expect m_m\n" << m_m;

  //Matrix3 tmp=m_proj.extract<3,3>();
  //tmp.getInverse(&m_backProj);
  //vnl_vector_fixed<Real,3> res=PM.get_focal_point().get_double3();
  //m_center[0]=res[0]; m_center[1]=res[1]; m_center[2]=res[2];
  //std::cout << "expect m_center=" << m_center <<"\n";
  //std::cout << "expect m_backProj\n" << m_backProj;

  // matrix decomposition: extrinsic parameters, intrinsic parameters,
  // center of projection
  Matrix3 p_=m_proj.extract<3,3>();
  // std::cout << "---------\np_\n" << p_;
  Matrix3 p_transp=p_.getTranspose();
  // std::cout << "p_transp\n" << p_transp;
  //m_k=Cholesky(p_*p_transp);

  Matrix3 l(0.),a;
  a=p_*p_transp;

  // upper Cholesky factorization of a
  for (int i=2; i>=0; --i) {
    {
      Real sum=a[i][i];
      for (int k=2; k>=i+1; --k)
	sum -= l[i][k]*l[i][k];

      l[i][i]= sqrt(sum);
    }

    for (int j=i-1; j>=0; --j) {
      Real sum=a[j][i];
      for (int k=2; k>=i+1; --k)
	sum -= l[j][k]*l[i][k];

      l[j][i]=sum/l[i][i];
    }
  }

  m_k=l;

  assert(m_k[2][2]!=0.);

  //if (m_k[2][2]<0.)
  //  std::cout << "Note: " << this << " CamParam with negative k[2][2]\n";

  m_k*=1./m_k[2][2];

  //std::cout << "-------\nm_k\n" << m_k;
  //assert(m_k*m_k.getTranspose()==p_*p_transp);
  // std::cout << "m_k*m_k.getTranspose()\n" << m_k*m_k.getTranspose();
  // std::cout << "p_*p_transp\n" << a;

  Matrix3 kinv;
  //#ifndef NDEBUG
  bool tst1=
    //#endif
    m_k.getInverse(&kinv);

  assert(tst1);
  if (!tst1) {
    std::cerr << "---------------------------------------------------------\n";
    std::cerr << "Projection Matrix ERROR: k matrix inversion unsuccessful\n";
    std::cerr << "Possible cause: rank deficiency of projection matrix\n"
	      << proj << "\n";
  };

  // std::cout << "kinv\n" << kinv;
  Matrix3x4 kinv_p=kinv*proj;
  Matrix3 kinv_p_=kinv_p.extract<3,3>();

  Real det=kinv_p_.det();
  // std::cout << "det kinvp=" << det << "\n";
  if (tst1 && (det==0. || det==NAN)) {
    std::cerr << "---------------------------------------------------------\n";
    std::cerr << "Projection Matrix ERROR: ";
  }

  if (!tst1 || det==0. || det==NAN) {
    std::cerr <<"Note: reverse Cholesky decomposition returns k=\n"<<m_k<<"\n";
    std::cerr <<"Note: det k=" << m_k.det() << "\n";
    std::cerr << "---------------------------------------------------------\n";
    return false;
  }

  Real lambda=exp(log(1./det)/3.);
  // std::cout << "lambda=" << lambda <<"\n";
  m_m=kinv_p; m_m*=lambda;

  //std::cout << "m_m\n" << m_m;
  Matrix3 m_m_inv=m_m.extract<3,3>().getTranspose();

  m_center=-(m_m_inv*m_m.column(3));
  //std::cout << "m_center=" << m_center <<"\n";
  bool test=p_.getInverse(&m_backProj);
  // std::cout << "m_backProj\n" << m_backProj;

  addNoise();
  return test;
}

