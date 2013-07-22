#ifndef _CAMARA_H_
#define _CAMARA_H_

class Camara
{
  // Specifies the position of the eye point.
  float _eyeX, _eyeY, _eyeZ;
  // Specifies the position of the reference point.
  float _centerX, _centerY, _centerZ;
  // Specifies the direction of the up vector.
  float _upX, _upY, _upZ;

  void Copia ( const Camara& origen );

 public:

  Camara() : _eyeX(0.0), _eyeY(0.0), _eyeZ(0.0), _centerX(0.0), _centerY(0.0), _centerZ(0.0), _upX(0.0), _upY(0.0), _upZ(0.0) {};
  virtual ~Camara() {};

  Camara ( const Camara& origen );
  Camara operator= ( const Camara& origen );

  inline float getEyeX() const { return _eyeX; };
  inline void setEyeX ( float eyeX ) { _eyeX = eyeX; };
  inline float getEyeY() const { return _eyeY; };
  inline void setEyeY ( float eyeY ) { _eyeY = eyeY; };
  inline float getEyeZ() const { return _eyeZ; };
  inline void setEyeZ ( float eyeZ ) { _eyeZ = eyeZ; };

  inline float getCenterX() const { return _centerX; };
  inline void setCenterX ( float centerX ) { _centerX = centerX; };
  inline float getCenterY() const { return _centerY; };
  inline void setCenterY ( float centerY ) { _centerY = centerY; };
  inline float getCenterZ() const { return _centerZ; };
  inline void setCenterZ ( float centerZ ) { _centerZ = centerZ; };

  inline float getUpX() const { return _upX; };
  inline void setUpX ( float upX ) { _upX = upX; };
  inline float getUpY() const { return _upY; };
  inline void setUpY ( float upY ) { _upY = upY; };
  inline float getUpZ() const { return _upZ; };
  inline void setUpZ ( float upZ ) { _upZ = upZ; };

  void print();

};

#endif
