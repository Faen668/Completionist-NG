# Completionist NG

**Completionist - Skyrim Completion Tracker (NG)**

[![Nexus Mods](https://img.shields.io/badge/Nexus%20Mods-Completionist-orange?logo=nexusmods)](https://www.nexusmods.com/skyrimspecialedition/mods/46358)
[![GitHub](https://img.shields.io/badge/GitHub-Source%20Code-black?logo=github)](https://github.com/Faen668/Completionist-NG)

Completionist is a comprehensive completion-tracking framework for **The Elder Scrolls V: Skyrim Special Edition**.

It provides an in-game interface for tracking quests, items and other collectible or completion-related content across Skyrim, the official DLC and supported third-party mods.

The **NG** version is the native-code continuation of Completionist, built using modern Skyrim modding technologies and **CommonLibSSE-NG**.

---

## Features

### Quest Tracking

Completionist automatically tracks your progress through supported quests and presents completion information through the in-game MCM.

This includes support for:

* Main quests
* Side quests
* DLC quests
* Radiant and repeatable quests
* Supported mod-added quests
* Quest stages that do not use conventional completion states

Where necessary, Completionist records relevant quest-stage information so that quests which stop rather than formally complete can still be tracked accurately.

### Item & Collectible Tracking

Completionist provides tracking for a wide range of collectible content, including:

* Books
* Spell tomes
* Weapons
* Armour
* Unique items
* Collectibles
* Other supported completion-related content

The tracking system is designed to provide a single place to monitor overall completion progress.

### MCM Integration

Completionist provides extensive **SkyUI MCM** integration, allowing you to configure and monitor your completion progress directly in-game.

### Inventory Integration

Completionist can integrate with the player's inventory and provide configurable visual indicators for tracked collectables.

### Dynamic Crosshair Tags

The Dynamic Crosshair Tags system can identify tracked items while they are being targeted.

The system is configurable, including:

* Tag text
* Tag colour
* Category visibility
* Display behaviour

### Extensive Mod Support

Completionist supports tracking content from a large number of popular Skyrim mods.

The project is designed so that additional tracking data can be maintained and extended without requiring the core tracking system to be rewritten.

---

## Requirements

The following are required:

* **Skyrim Special Edition / Anniversary Edition**
* **SKSE64**
* **Address Library for SKSE Plugins**
* **PapyrusUtil SE**
* **SkyUI**

See the [Nexus Mods page](https://www.nexusmods.com/skyrimspecialedition/mods/46358) for the current supported versions and installation information.

### Recommended

Completionist was originally developed and designed with **Dear Diary - Dark Mode** in mind.

Completionist should work with other UI configurations, but UI mods based on Dear Diary Dark Mode may provide the best visual presentation for the MCM.

---

## Installation

Install Completionist using your preferred mod manager.

Ensure that all required dependencies are installed and that **SKSE64** is being used to launch Skyrim.

After installation, launch the game and allow Completionist to initialise.

The Completionist MCM can then be accessed through SkyUI.

For the latest release and installation instructions, visit:

**Nexus Mods:**
https://www.nexusmods.com/skyrimspecialedition/mods/46358

---

## Development

Completionist NG is a C++ SKSE plugin project.

The project uses:

* C++
* CommonLibSSE-NG
* CMake
* vcpkg
* SKSE
* Address Library
* Skyrim's native runtime interfaces

Source code is available on GitHub:

https://github.com/Faen668/Completionist-NG

### Building

Clone the repository:

```bash
git clone https://github.com/Faen668/Completionist-NG.git
cd Completionist-NG
```

Configure and build the project using the supplied CMake configuration.

CommonLibSSE-NG is used as a build dependency and provides the native Skyrim interfaces required by the plugin.

---

## Contributing

Bug reports, improvements and pull requests are welcome.

When submitting an issue, please provide as much information as possible, including:

* Skyrim runtime version
* SKSE version
* Completionist version
* Relevant mod list
* Crash logs, if applicable
* Steps required to reproduce the issue

Pull requests should be focused and should preserve the existing project structure and coding conventions.

---

## Credits

Completionist would not be possible without the work of the Skyrim modding community.

### CommonLibSSE-NG

Completionist NG uses **CommonLibSSE-NG** as its native Skyrim development framework.

https://github.com/alandtse/CommonLibSSE-NG

### SKSE

The Skyrim Script Extender provides the runtime extension framework used by Completionist.

https://skse.silverlock.org/

### Address Library

Address Library provides runtime-independent addresses used by native Skyrim plugins.

### PapyrusUtil

PapyrusUtil provides additional scripting functionality used by Completionist.

### SkyUI

SkyUI provides the MCM framework used by Completionist.

---

## Licence

Completionist NG is released under the:

**GNU General Public License v3.0 or later (GPL-3.0-or-later)**

with the **Modding Exception** and **GPL-3.0 Linking Exception (with Corresponding Source)**.

See [`LICENSE`](LICENSE) and [`EXCEPTIONS.md`](EXCEPTIONS.md) for the complete licence terms.

This licensing arrangement follows the licensing model used by CommonLibSSE-NG.

In particular, the Modding Exception permits the software to be linked with the proprietary code it is intended to modify, including Skyrim, while retaining the GPL requirements for the applicable open-source code.

---

## Distribution & Mod Permissions

The source code of Completionist NG is licensed according to the terms described above.

The licence applies to the **source code of the project**. It does not automatically grant permission to redistribute Bethesda's game assets, third-party assets, or other copyrighted material included or referenced by the project.

Third-party assets remain subject to their respective licences and permissions.

For the latest user-facing distribution permissions, please also refer to the Permissions and Credits section of the [Nexus Mods page](https://www.nexusmods.com/skyrimspecialedition/mods/46358).

---

## Disclaimer

Completionist NG is provided **"AS IS"**, without warranty of any kind, express or implied.

The authors and contributors are not responsible for loss of save data, corrupted saves, crashes, incompatibilities, or other damage resulting from the use of this software.

Always maintain appropriate backups of your Skyrim installation and save files.

---

## Project Links

* **Nexus Mods:** https://www.nexusmods.com/skyrimspecialedition/mods/46358
* **GitHub:** https://github.com/Faen668/Completionist-NG
* **CommonLibSSE-NG:** https://github.com/alandtse/CommonLibSSE-NG
* **SKSE:** https://skse.silverlock.org/

---

**Completionist NG**
Copyright © 2026 Faen90 / Faen668

Licensed under the GNU General Public License v3.0 or later, with the applicable modding and linking exceptions.
