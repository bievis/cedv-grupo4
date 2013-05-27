#ifndef __ANIMATION_BLENDER_H__
#define __ANIMATION_BLENDER_H__

#include <Ogre.h>

using namespace Ogre;

class AnimationBlender
  {

 public:
   enum BlendingTransition {
     Switch,  // Parar fuente y reproduce destino
     Blend    // Cross fade (Mezclado suave)
   };

 private:
   Entity *            _mEntity;      // Entidad sobre la que mezclamos
   AnimationState *    _mSource;      // Animacion inicio de la mezcla
   AnimationState *    _mTarget;      // Animacion destino
   BlendingTransition  _mTransition;  // Tipo de transicion
   bool                _mLoop;        // Animacion en bucle?

   Real                _mTimeleft;    // Tiempo restante de la animacion (segundos)
   Real                _mDuration;    // Tiempo invertido en el mezclado (segundos)
   bool                _mComplete;    // Ha finalizado la animacion?

 public:

   AnimationBlender ( Entity * );

   void                        blend ( const String &animation, BlendingTransition transition, Real duration, bool l = true );
   void                        addTime ( Real );
   Real                        getProgress() { return _mTimeleft / _mDuration; };

   inline Entity*              getEntityPtr() { return _mEntity; };
   inline AnimationState*      getSourcePtr() { return _mSource; };
   inline AnimationState*      getTargetPtr() { return _mTarget; };
   inline BlendingTransition&  getBlendingTransition() { return _mTransition; };
   inline bool                 getLoop() const { return _mLoop; };

   inline Real                 getTimeLeft() const { return _mTimeleft; };
   inline Real                 getDuration() const { return _mDuration; };
   inline bool                 getComplete() const { return _mComplete; };
};

#endif
