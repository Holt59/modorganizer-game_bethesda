#ifndef NEHRIM_MODDATACONTENT_H
#define NEHRIM_MODDATACONTENT_H

#include <gamebryomoddatacontent.h>
#include <ifiletree.h>

class NehrimModDataContent : public GamebryoModDataContent
{
public:
  /**
   *
   */
  NehrimModDataContent(GameGamebryo const* gamePlugin)
      : GamebryoModDataContent(gamePlugin)
  {
    // Just need to disable some contents:
    m_Enabled[CONTENT_MCM]     = false;
    m_Enabled[CONTENT_SKYPROC] = false;
  }
};

#endif  // NEHRIM_MODDATACONTENT_H
