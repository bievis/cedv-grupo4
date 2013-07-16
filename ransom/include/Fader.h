#ifndef FADER_H
#define FADER_H

#pragma once

namespace Ogre {
   class TextureUnitState;
   class Overlay;
}

/// \brief Class to receive the callbacks from Fader class
class FaderCallback
{
public:
   /// \brief this callback was launched when the Fader is IN
   virtual void fadeInCallback(void) {}
   /// \brief this callback was launched when the Fader is OUT
   virtual void fadeOutCallback(void) {}
};

/// \brief Class to manage the Fader effect on the overlays
class Fader
{
public:
  /// \brief Fader Constructor
  /// \param OverlayName Overlay identifier to use in the Fader effect
  /// \param MaterialName Material name to use in the Fader effect
  /// \param instance FaderCallback instance to invoke in the IN/OUT Fader effect
  Fader(const char *OverlayName, const char *MaterialName, FaderCallback *instance = 0);
  /// \brief Fader destructor
   ~Fader(void);

  /// \brief Method to configure the duration for the Fader IN effect
  /// \param duration duration in seconds
  void startFadeIn(double duration = 1.0f);
  /// \brief Method to configure the duration for the Fader OUT effect
  /// \param duration duration in seconds
  void startFadeOut(double duration = 1.0f);
  /// \brief Method to execute Fader effect (in the update() on GameState class)
  /// \param timeSinceLastFrame time used in update method to know the time between frames
  void fade(double timeSinceLastFrame);

protected:
  /// \brief alpha channel
  double _alpha;
  /// \brief current duration
  double _current_dur;
  /// \brief total duration
  double _total_dur;
  /// \brief reference to callback instance
  FaderCallback *_inst;
  /// \brief reference to texture unit state
  Ogre::TextureUnitState *_tex_unit;
  /// \brief overlay reference
  Ogre::Overlay *_overlay;

  enum _fadeop {
       FADE_NONE,
       FADE_IN,
       FADE_OUT,
  } _fadeop;

};
#endif // FADER_H
