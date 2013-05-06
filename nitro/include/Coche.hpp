#ifndef __COCHE_HPP__
#define __COCHE_HPP__

#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"
#include "Constraints/OgreBulletDynamicsRaycastVehicle.h"
#include "OgreBulletCollisionsRay.h"
#include <OgreSceneNode.h>
#include <string>

using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;
using namespace Ogre;
using namespace std;

enum eColour_Chassis {
  DEFAULT,
  BLUE,
  GREEN,
  YELLOW,
  RED
};

/// \brief Clase para el manejo de los coches dentro del juego
/// Esta clase se encargará de la construcción del objeto coche y tambien de su inserción en el juego, ya que se le suministrará el Scene Manager y el objeto world para la fisica
class Coche
  {
  public:
    /// \brief Constructor por defecto del objeto Coche
    /// Este constructor solo crea un coche sin llegar a incrustarlo en el Scene Manager ni en el World de la fisica.
    Coche();
    /// \brief Constructor parametrizado para el objeto Coche
    /// Este constructor crea el coche y además internamente invoca al método build() el cual incrusta el coche dentro del Scene Manager y del motor de física
    /// \param name Nombre identificativo del coche
    /// \param pos_x Posición de la coordenada X del coche en el juego
    /// \param pos_y Posición de la coordenada Y del coche en el juego
    /// \param pos_z Posición de la coordenada Z del coche en el juego
    /// \param sceneMgr Referencia al Scene Manager para la inclusión del coche en el juego
    /// \param world Referencia al objeto World para la inclusión del coche con la física del juego
    /// \param colour Color del chasis del coche a construir
    Coche ( const string& name, float pos_x, float pos_y, float pos_z, Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world, eColour_Chassis colour = DEFAULT );
    /// \brief Destructor del objeto Coche
    ~Coche();

    /// \brief Constructor copia del objeto Coche
    /// Copiará el objeto coche pasado como argumento, creando uno nuevo apartir de éste
    /// \param source objeto Coche que se copia
    Coche ( const Coche& source );
    /// \brief Operador de asignación del objeto Coche
    /// Copiará el objeto coche pasado como argumento, creando uno nuevo apartir de éste
    /// \param source objeto Coche que se copia
    Coche& operator= ( const Coche& source );

    /// \brief Método para obtener el nombre identificador del objeto Coche
    /// \return Nombre identificativo del objeto Coche
    inline string getName() const { return _name; };
    /// \brief Método para obtener las coordenadas INICIALES donde se ubicará el coche en el escenario
    /// Este método acepta tres parámetros (por referencia) para poder devolver los tres a la vez en cada invocación
    /// \param x coordenada X dentro del escenario
    /// \param y coordenada Y dentro del escenario
    /// \param z coordenada Z dentro del escenario
    inline void getInitialPosition ( float& x, float& y, float& z ) const { x = _x; y = _y; z = _z; };
    /// \brief Método para obtener el radio de las ruedas
    /// \return Radio de las ruedas
    inline float getWheelRadius() const { return _wheelRadius; };
    /// \brief Método para obtener la anchura de las ruedas
    /// \return Anchura de las ruedas
    inline float getWheelWidth() const { return _wheelWidth; };
    /// \brief Método para obtener el valor de fricción de las ruedas
    /// \return Valor de fricción de las ruedas
    inline float getWheelFriction() const { return _wheelFriction; };
    /// \brief Método para obtener el valor de la influencia al rodar
    /// \return Valor de la influencia al rodar
    inline float getRollInfluence() const { return _rollInfluence; };
    /// \brief Método para obtener el valor de suspensión
    /// \return Valor de la suspensión
    inline float getSuspensionRestLength() const { return _suspensionRestLength; };
    /// \brief Método para obtener el valor de la fuerza del motor
    /// \return Valor de la fuerza del motor
    inline float getEngineForce() const { return _engineForce; };
    /// \brief Método para obtener la referencia del chasis para la fisica del juego
    /// \return Referencia al chasis para la fisica del juego
    inline OgreBulletDynamics::WheeledRigidBody* getCarChassisPtr() const { return _mCarChassis; };
    /// \brief Método para obtener la referencia al VehicleTuning de la fisica del juego
    /// \return Referencia al VehicleTuning de la fisica del juego
    inline OgreBulletDynamics::VehicleTuning* getTuningPtr() const { return _mTuning; };
    /// \brief Método para obtener la referencia al VehicleRayCaster de la fisica del juego
    /// \return Referencia al VehicleRayCaster de la fisica del juego
    inline OgreBulletDynamics::VehicleRayCaster* getVehicleRayCasterPtr() const { return _mVehicleRayCaster; };
    /// \brief Método para obtener la referencia al RaycastVehicle del motor de la fisica
    /// \return Referencia al RaycastVehicle del motor de la fisica
    inline OgreBulletDynamics::RaycastVehicle* getVehiclePtr() const { return _mVehicle; };
    /// \brief Método para obtener la referencia al chasis del coche
    /// \return Referencia del tipo Ogre::Entity con el chasis del coche
    inline Ogre::Entity* getChassisPtr() const { return _mChassis; };
    /// \brief Método para obtener la referencia a cada una de las ruedas que componen el objeto coche
    /// Este método devuelve la referencia a la entidad "rueda" cuya posición index ocupa dentro del array de cuatro elementos que componen las ruedas del Coche, la cual es del tipo Ogre::Entity
    /// \param index posición en el vector de ruedas que queremos consultar (0-3)
    /// \return Referencia a la rueda consultada
    inline Ogre::Entity* getWheelsPtr ( unsigned int index ) const { return _mWheels[index%4]; };
    /// Este método devuelve la referencia al nodo de escena "rueda" cuya posición index ocupa dentro del array de cuatro elementos que componen las ruedas del Coche, la cual es del tipo Ogre::SceneNode
    /// \param index posición en el vector de ruedas que queremos consultar (0-3)
    /// \return Referencia a la rueda consultada
    inline Ogre::SceneNode* getWheelNodesPtr ( unsigned int index ) const { return _mWheelNodes[index%4]; };
    /// \brief Este método devuelve la dirección en grados del Coche
    /// \return Dirección del Coche
    inline float getSteering() const { return _mSteering; };
    /// \brief Método para establecer el identificador asignado al Coche
    /// \return Identificador dado al objeto Coche
    inline void setName ( const string& source ) { _name = source; };
    /// \brief Método para establecer la posición inicial del Coche dentro del escenario
    /// \param x Coordenada X de la posición del Coche dentro del escenario
    /// \param y Coordenada Y de la posición del Coche dentro del escenario
    /// \param z Coordenada Z de la posición del Coche dentro del escenario
    inline void setInitialPosition ( float x, float y, float z ) { _x = x; _y = y; _z = z; };
    /// \brief Método para establecer el radio de las ruedas
    /// \param newValue valor del radio en coma flotante
    inline void setWheelRadius ( float newValue ) { _wheelRadius = newValue; };
    /// \brief Método para establecer el ancho de las ruedas
    /// \param newValue valor del ancho en coma flotante
    inline void setWheelWidth ( float newValue ) { _wheelWidth = newValue; };
    /// \brief Método para establecer el valor de fricción de las ruedas
    /// \param newValue valor de fricción de las ruedas en coma flotante
    inline void setWheelFriction ( float newValue ) { _wheelFriction = newValue; };
    /// \brief Método para establecer el valor de la influencia al rodar de las ruedas
    /// \param newValue valor de la influencia al rodar de las ruedas en coma flotante
    inline void setRollInfluence ( float newValue ) { _rollInfluence = newValue; };
    /// \brief Método para establecer el valor de la suspensión de las ruedas
    /// \param newValue valor de la suspension de las ruedas en coma flotante
    inline void setSuspensionRestLength ( float newValue ) { _suspensionRestLength = newValue; };
    /// \brief Método para establecer la fuerza del motor
    /// \param newValue valor de la fuerza del motor en coma flotante
    inline void setEngineForce ( float newValue ) { _engineForce = newValue; };
    /// \brief Método para establecer la dirección del Coche
    /// \param newValue valor de la direccion del Coche
    inline void setSteering ( float newValue ) { _mSteering = newValue; };
    /// \brief Método para establecer la referencia del chasis para la fisica del juego
    /// \param source Referencia al chasis para la fisica del juego
    void setCarChassis ( OgreBulletDynamics::WheeledRigidBody* source );
    /// \brief Método para establecer la referencia al VehicleTuning de la fisica del juego
    /// \param source Referencia al VehicleTuning de la fisica del juego
    void setTuning ( OgreBulletDynamics::VehicleTuning* source );
    /// \brief Método para establecer la referencia al VehicleRayCaster de la fisica del juego
    /// \param source Referencia al VehicleRayCaster de la fisica del juego
    void setVehicleRayCaster ( OgreBulletDynamics::VehicleRayCaster* source );
    /// \brief Método para establecer la referencia al RaycastVehicle del motor de la fisica
    /// \param source Referencia al RaycastVehicle del motor de la fisica
    void setVehicle ( OgreBulletDynamics::RaycastVehicle* source );
    /// \brief Método para establecer la referencia al chasis del coche
    /// \param source Referencia del tipo Ogre::Entity con el chasis del coche
    void setChassis ( Ogre::Entity* source );
    /// \brief Método para establecer la referencia a cada una de las ruedas que componen el objeto coche
    /// Este método establece la referencia a la entidad "rueda" cuya posición index ocupa dentro del array de cuatro elementos que componen las ruedas del Coche, la cual es del tipo Ogre::Entity
    /// \param index posición en el vector de ruedas que queremos añadir (0-3)
    /// \param source Referencia a la nueva rueda
    void setWheels ( unsigned int index, Ogre::Entity* source );
    /// Este método establece la referencia al nodo de escena "rueda" cuya posición index ocupa dentro del array de cuatro elementos que componen las ruedas del Coche, la cual es del tipo Ogre::SceneNode
    /// \param index posición en el vector de ruedas que queremos añadir (0-3)
    /// \param source Referencia a la nueva rueda
    void setWheelNodes ( unsigned int index, Ogre::SceneNode* source );
    /// \brief Metodo que limpia todos los objetos que tenemos creados en el Coche
    void clear();
    /// \brief Metodo que construye el objeto Coche dentro del Scene Manager y también dentro de la fisica del juego
    /// \param sceneMgr Referencia al Scene Manager para poder añadir nuestro objeto dentro del escenario
    /// \param world Referencia al objeto manejador de la fisica
    void build ( Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world );
    /// \brief Metodo que provoca la acelarcion del coche
    /// Como parametro a este metodo tenemos la posibilidad de provocar que las ruedas se "enderecen", es decir, si nosotros aceleramos y pulsamos a la vez a la derecha a la izquierda no queremos que las ruedas vuelvan a su estado original, mientras que si por ejemplo aceleramos sin pulsar a la izquierda o a la derecha, pues si queremos que las ruedas queden alineadas con el coche.
    /// \param endereza provocara que las ruedas se alineen al coche mientras se acelera
    void accelerate ( bool endereza = false );
    /// \brief Metodo que provoca la deceleracion del coche (provocando la marcha atras)
    /// Como parametro a este metodo tenemos la posibilidad de provocar que las ruedas se "enderecen", es decir, si nosotros aceleramos y pulsamos a la vez a la derecha a la izquierda no queremos que las ruedas vuelvan a su estado original, mientras que si por ejemplo aceleramos sin pulsar a la izquierda o a la derecha, pues si queremos que las ruedas queden alineadas con el coche.
    /// \param endereza provocara que las ruedas se alineen al coche mientras se acelera
    void decelerate ( bool endereza = false );
    /// \brief Metodo que gira las ruedas delanteras a la izquierda
    void turn_left();
    /// \brief Metodo que gira las ruedas delanteras a la derecha
    void turn_right();
    /// \brief Metodo que imprime la informacion del objeto Coche por pantalla
    void print_info();
    /// \brief Metodo que dice si el coche esta pasando por la meta o no
    /// \return Si se ha pasado por la meta o no
    bool isMeta(Ogre::SceneManager* sceneMgr, OgreBulletDynamics::DynamicsWorld* world);
    /// \brief Metodo que vuelve a dejar el coche en el estado y posicion inicial
    void reset();

  private:
    /// \brief Identificador del Coche
    string _name;
    /// \brief Color del chasis del Coche
    eColour_Chassis _color;
    /// \brief Radio de las ruedas
    float _wheelRadius;
    /// \brief Ancho de las ruedas
    float _wheelWidth;
    /// \brief Valor de fricción de las ruedas
    float _wheelFriction;
    /// \brief Valor de la influencia al rodar
    float _rollInfluence;
    /// \brief Valor de la suspensión
    float _suspensionRestLength;
    /// \brief Valor de la fuerza del motor
    float _engineForce;
    /// \brief Valor de la coordenada X de la posición del Coche en el escenario
    float _x;
    /// \brief Valor de la coordenada Y de la posición del Coche en el escenario
    float _y;
    /// \brief Valor de la coordenada Z de la posición del Coche en el escenario
    float _z;
    /// \brief Referencia al chasis usado para la fisica del juego
    OgreBulletDynamics::WheeledRigidBody  *_mCarChassis;
    /// \brief Referencia al VehicleTuning usado para la fisica del juego
    OgreBulletDynamics::VehicleTuning     *_mTuning;
    /// \brief Referencia al VehicleRayCaster usado para la fisica del juego
    OgreBulletDynamics::VehicleRayCaster  *_mVehicleRayCaster;
    /// \brief Referencia al RaycastVehicle usado para la fisica del juego
    OgreBulletDynamics::RaycastVehicle    *_mVehicle;
    /// \brief Referencia al chasis usado para la escena del juego
    Ogre::Entity                          *_mChassis;
    /// \brief Vector a las referencias a las ruedas usadas para la escena del juego
    Ogre::Entity                          *_mWheels[4];
    /// \brief Vector a las referencias de los nodos con las ruedas para la escena del juego
    Ogre::SceneNode                       *_mWheelNodes[4];
    /// \brief Valor con la dirección inicial del coche
    float _mSteering;

    /// \brief Metodo para la inicialización de todas los atributos del objeto Coche
    void init();
    /// \brief Metodo para la copia de un objeto Coche
    /// Este metodo será utilizado para el constructor copia y para el operador de asignación unificando funcionalidad
    /// \param source Objeto que se toma para copiar
    void copy ( const Coche& source );
    /// \brief Metodo que en función del color seleccionado devolverá el nombre del fichero mesh a cargar
    /// \return Nombre del fichero mesh a cargar
    const char* getFilenameMesh();
  };

#endif
