﻿#include "creationgameplugins.h"
#include <safewritefile.h>
#include <report.h>
#include <ipluginlist.h>
#include <report.h>
#include <scopeguard.h>

#include <QDir>
#include <QStringList>
#include <QStringEncoder>
#include <QSet>

using MOBase::IPluginGame;
using MOBase::IPluginList;
using MOBase::IOrganizer;
using MOBase::SafeWriteFile;
using MOBase::reportError;

CreationGamePlugins::CreationGamePlugins(IOrganizer *organizer)
  : GamebryoGamePlugins(organizer)
{
}

QStringList CreationGamePlugins::getLoadOrder() {
  QString loadOrderPath =
    organizer()->profile()->absolutePath() + "/loadorder.txt";
  QString pluginsPath = organizer()->profile()->absolutePath() + "/plugins.txt";

  bool loadOrderIsNew = !m_LastRead.isValid() ||
    !QFileInfo(loadOrderPath).exists() ||
    QFileInfo(loadOrderPath).lastModified() > m_LastRead;
  bool pluginsIsNew = !m_LastRead.isValid() ||
    QFileInfo(pluginsPath).lastModified() > m_LastRead;

  if (loadOrderIsNew || !pluginsIsNew) {
    return readLoadOrderList(m_Organizer->pluginList(), loadOrderPath);
  }
  else {
    return readPluginList(m_Organizer->pluginList());
  }
}

void CreationGamePlugins::writePluginList(const IPluginList *pluginList,
                                          const QString &filePath) {
  SafeWriteFile file(filePath);

  QStringEncoder encoder(QStringConverter::Encoding::System);

  file->resize(0);

  file->write(encoder.encode(
      "# This file was automatically generated by Mod Organizer.\r\n"));

  bool invalidFileNames = false;
  int writtenCount = 0;

  QStringList plugins = pluginList->pluginNames();
  std::sort(plugins.begin(), plugins.end(),
            [pluginList](const QString &lhs, const QString &rhs) {
              return pluginList->priority(lhs) < pluginList->priority(rhs);
            });

  QStringList PrimaryPlugins = organizer()->managedGame()->primaryPlugins();
  QStringList DLCPlugins = organizer()->managedGame()->DLCPlugins();
  QSet<QString> ManagedMods = QSet<QString>(PrimaryPlugins.begin(), PrimaryPlugins.end());
  QSet<QString> DLCSet = QSet<QString>(DLCPlugins.begin(), DLCPlugins.end());
  ManagedMods.subtract(DLCSet);
  PrimaryPlugins.append(QList<QString>(ManagedMods.begin(), ManagedMods.end()));

  //TODO: do not write plugins in OFFICIAL_FILES container
  for (const QString &pluginName : plugins) {
  if (!PrimaryPlugins.contains(pluginName,Qt::CaseInsensitive)) {
    if (pluginList->state(pluginName) == IPluginList::STATE_ACTIVE) {
      auto result = encoder.encode(pluginName);
      if (encoder.hasError()) {
        invalidFileNames = true;
        qCritical("invalid plugin name %s", qUtf8Printable(pluginName));
      }
      else
      {
        file->write("*");
        file->write(result);

      }
      file->write("\r\n");
      ++writtenCount;
    }
    else
    {
        auto result = encoder.encode(pluginName);
        if (encoder.hasError()) {
          invalidFileNames = true;
          qCritical("invalid plugin name %s", qUtf8Printable(pluginName));
        }
        else
        {
          file->write(result);
        }
        file->write("\r\n");
        ++writtenCount;
    }
    }
  }

  if (invalidFileNames) {
    reportError(QObject::tr("Some of your plugins have invalid names! These "
                            "plugins can not be loaded by the game. Please see "
                            "mo_interface.log for a list of affected plugins "
                            "and rename them."));
  }

  file.commitIfDifferent(m_LastSaveHash[filePath]);
}

QStringList CreationGamePlugins::readPluginList(MOBase::IPluginList *pluginList)
{
  const auto plugins = pluginList->pluginNames();
  const auto primaryPlugins = organizer()->managedGame()->primaryPlugins();
  QStringList loadOrder(primaryPlugins);

  for (const QString &pluginName : loadOrder) {
    if (pluginList->state(pluginName) != IPluginList::STATE_MISSING) {
      pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
    }
  }

  QString filePath = organizer()->profile()->absolutePath() + "/plugins.txt";
  QFile file(filePath);
  if (!file.open(QIODevice::ReadOnly)) {
    qWarning("%s not found", qUtf8Printable(filePath));
    return loadOrder;
  }
  ON_BLOCK_EXIT([&]() { file.close(); });

  if (file.size() == 0) {
    // MO stores at least a header in the file. if it's completely empty the
    // file is broken
    qWarning("%s empty", qUtf8Printable(filePath));
    return loadOrder;
  }

  QStringList pluginsFound;
  while (!file.atEnd()) {
    QByteArray line = file.readLine();
    QString pluginName;
    if ((line.size() > 0) && (line.at(0) != '#')) {
      pluginName = QStringEncoder(QStringConverter::Encoding::System).encode(line.trimmed().constData());
    }
    if (!primaryPlugins.contains(pluginName, Qt::CaseInsensitive)) {
      if (pluginName.startsWith('*')) {
        pluginName.remove(0, 1);
        if (pluginName.size() > 0) {
          pluginList->setState(pluginName, IPluginList::STATE_ACTIVE);
          pluginsFound.append(pluginName);
          if (!loadOrder.contains(pluginName, Qt::CaseInsensitive)) {
            loadOrder.append(pluginName);
          }
        }
      }
      else {
        if (pluginName.size() > 0) {
          pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
          pluginsFound.append(pluginName);
          if (!loadOrder.contains(pluginName, Qt::CaseInsensitive)) {
            loadOrder.append(pluginName);
          }
        }
      }
    }
    else {
      pluginName.remove(0, 1);
      pluginsFound.append(pluginName);
    }
  }

  file.close();

  // set all plugins not found inactive
  for (const auto& pluginName : plugins) {
    if (!pluginsFound.contains(pluginName, Qt::CaseInsensitive)) {
      pluginList->setState(pluginName, IPluginList::STATE_INACTIVE);
    }
  }

  return loadOrder;
}

bool CreationGamePlugins::lightPluginsAreSupported()
{
  return true;
}