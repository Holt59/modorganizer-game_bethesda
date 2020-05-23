#ifndef ENDERAL_MODATACHECKER_H
#define ENDERAL_MODATACHECKER_H

#include <gamebryomoddatachecker.h>

class EnderalModDataChecker : public GamebryoModDataChecker
{
public:
  using GamebryoModDataChecker::GamebryoModDataChecker;

protected:
  virtual const FileNameSet& possibleFolderNames() const override {
    static FileNameSet result{
      "fonts", "interface", "menus", "meshes", "music", "scripts", "shaders",
      "sound", "strings", "textures", "trees", "video", "facegen", "materials",
      "skse", "distantlod", "asi", "Tools", "MCM", "distantland", "mits",
      "dllplugins", "SkyProc Patchers", "CalienteTools", "NetScriptFramework", "shadersfx"
    };
    return result;
  }
  virtual const FileNameSet& possibleFileExtensions() const override {
    static FileNameSet result{
      "esp", "esm", "bsa", ".modgroups"
    };
    return result;
  }

  const GameGamebryo* m_Game;
};

#endif // ENDERAL_MODATACHECKER_H
