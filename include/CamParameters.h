/** @file CameraParameters.h
    
 * Camera parmeters with intrinsic/extrinsic parameter definitions
 */

#ifndef _CameraParameters_h_
#define _CameraParameters_h_

// IObject base definitions
//#include <GL/gl.h>
//#include <mvl/PMatrix.h>

#include <iostream>
//#include "Object.h"
#include "Matrix.h"
//#include "trace.h"

/**
 * CamParameters object, stores intrinsic/extrinsic camera parameters.
 * Intrinsic parameters: Matrix3 K
 * Extrinsic parameters: Matrix4 M
 */

class CamParameters //: public IObject
{
public:

  /// Empty Constructor, does absolutely nothing
  /// (leaves camera in undefined state)
  inline CamParameters()
    :m_w(-1)
    {}

  /// Partial constructor, projection still missing
  inline CamParameters(int w, int h)
    :m_w(w), m_h(h)
    {}

  /// Complete Constructor using the projection matrix
  inline CamParameters(int w, int h, const Matrix<3,4>& proj)
    :m_w(w), m_h(h)
    {setProjection(proj);}

  /// Complete constructor
  /// @param k 3x3 intrinsinc param matrix, is copied internally
  /// @param m 3x4 extrinsic (positionning) matrix
  inline CamParameters(int w, int h, const Matrix3& k, const Transform3D& m)
    : m_k(k), m_m(m), m_w(w), m_h(h)
    {
      update();
    }

  /// access instrinsic params matrix
  inline const Matrix3& getK() const
    {
      return m_k;
    }

  /// access exstrinsic params matrix
  inline const Transform3D& getM() const
    {
      return m_m;
    }

  /// get width of camera image
  inline int getWidth() const {return m_w;}

  /// get heigth of camera image
  inline int getHeight() const {return m_h;}
  
  /// set instrinsic params matrix
  /// side effect: recomputation of projection matrix
  inline void setK(const Matrix3& k)
    {
      m_k=k; update();
    }
  
  /// set exstrinsic params matrix
  /// side effect: recomputation of projection matrix
  inline void setM(const Transform3D& m)
    {
      m_m=m; update();
    }

  /// set both intrinsic & exstrinsic params matrix
  /// side effect: recomputation of projection matrix
  inline void setKM(const Matrix3& k, const Transform3D& m)
    {
      m_k=k; m_m=m; update();
    }
  
  /// get projection matrix (K*M) - precalculated, no op performed
  inline const Matrix3x4& getProjection() const
    {
      //LTRACELN(5,"getProjection()=" << m_proj);
      return m_proj;
    }

  /// set projection matrix. side-effect: computation of intrinsic/extrinsic
  /// parameter matrices; returns true if success, else the supplied Matrix
  /// is not a projection matrix and this object is undefined.
  bool setProjection(const Matrix3x4& proj);

  /// set sizes
  inline void setSize(int w, int h) {
    m_w=w; m_h=h;
  }

  /// initialization test
  bool isInitialized() const {
    return m_w!=-1;
  }

  /// get direction backprojection matrix t(M)*K^(-1) (cached, constant
  /// cost per call) 
  const Matrix3& getBackProjection() const
    {
      return m_backProj;
    }

  /// get center of view
  const Vector3& getCenter() const
    {return m_center;}
 
#ifndef NO_GUI // CM 
  /// set from opengl matrices, need image width and height
  void glRead();

  /// set opengl matrices according to CamParmeters
  /// need image width and height
  void glWrite(Real near=0.1, Real far=1.9) const;

  void glWriteTextureProjection(Real near=0.1, Real far=1.9) const;

#endif // NO_GUI // CM      
  
protected:
  
  /// calculation of projection matrix
  void update();

  /// experimental: add noise on inputs
  void addNoise();

  /// intrinsic params
  Matrix3 m_k;
  
  /// extrinsic (positioning) params
  Transform3D m_m;

  /// cached back projection matrix
  Matrix3 m_backProj;

  /// word coords of optical center of the camera: cached
  Vector3 m_center;
  
  /// projection matrix(calculated from other data, used as internal cache)
  Matrix3x4 m_proj;

  /// size of image
  int m_w, m_h;
  
};

inline std::ostream& operator <<(std::ostream& o, const CamParameters& cp) {
  return o << cp.getProjection();
}

inline std::istream& operator >>(std::istream& i, CamParameters& cp) {
  Matrix3x4 p;
  i >> p; cp.setProjection(p);
  return i;
}

#endif //_CameraParameters_h_
