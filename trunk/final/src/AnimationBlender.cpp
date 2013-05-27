#include "AnimationBlender.h"

AnimationBlender::AnimationBlender ( Entity *entity ) : _mEntity(entity)
  {
    AnimationStateSet *set = _mEntity->getAllAnimationStates();
    AnimationStateIterator it = set->getAnimationStateIterator();

    // Inicializamos los AnimationState de la entidad
    while ( it.hasMoreElements() )
      {
	AnimationState *anim = it.getNext();
	anim->setEnabled(false);
	anim->setWeight(0);
	anim->setTimePosition(0);
      }

    _mSource = NULL;
    _mTarget = NULL;
    _mTimeleft = 0;
  }

void AnimationBlender::blend ( const String &animation,
			       BlendingTransition transition,
			       Real duration, bool l )
  {
    AnimationState *newTarget = _mEntity->getAnimationState ( animation );
    newTarget->setLoop(l);
    _mTransition = transition;
    _mDuration = duration;
    _mLoop = l;

    if  ( ( _mTimeleft <= 0 ) || ( transition == AnimationBlender::Switch ) )
      {
	if ( _mSource != NULL )
	  _mSource->setEnabled ( false );

	_mSource = newTarget;
	_mSource->setEnabled ( true );
	_mSource->setWeight ( 1 );
	_mSource->setTimePosition ( 0 );
	_mTimeleft = _mSource->getLength();
	_mTarget = NULL;
      }
    else
      {
	if ( _mSource != newTarget )
	  {
	    _mTarget = newTarget;
	    _mTarget->setEnabled ( true );
	    _mTarget->setWeight ( 0 );
	    _mTarget->setTimePosition ( 0 );
	  }
      }
  }

void AnimationBlender::addTime ( Real time )
  {
    if ( _mSource == NULL )
      return; // No hay fuente

    _mSource->addTime ( time );
    _mComplete = false;
    _mTimeleft -= time;

    if ( ( _mTimeleft <= 0 ) && ( _mTarget == NULL ) )
      _mComplete = true;

    if ( _mTarget != NULL )
      { // Si hay destino
	if ( _mTimeleft <= 0 )
	  {
	    _mSource->setEnabled ( false );
	    _mSource->setWeight ( 0 );
	    _mSource = _mTarget;
	    _mSource->setEnabled ( true );
	    _mSource->setWeight ( 1 );
	    _mTimeleft = _mSource->getLength();
	    _mTarget = NULL;
	  }
	else
	  { // Queda tiempo en Source... cambiar pesos
	    Real weight = _mTimeleft / _mDuration;

	    if ( weight > 1 )
	      weight = 1.0;

	    _mSource->setWeight ( weight );
	    _mTarget->setWeight ( 1.0 - weight );

	    if ( _mTransition == AnimationBlender::Blend )
	      _mTarget->addTime ( time );
	  }
      }

    if ( ( _mTimeleft <= 0 ) && _mLoop )
      _mTimeleft = _mSource->getLength();
  }
